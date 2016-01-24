#include <algorithm>
#include <set>
#include <exception>
#include "common.h"
#include "servers.h"
#include "users.h"
#include "user_to_customer.h"
#include "business.h"
#include "thrift_pool.h"
#include "send_message.h"
#include "customer_sched.h"
#include "stat_client.h"
#include "util.h"

string g_server_port;

string g_customer_service_ip;
string g_customer_service_port;

string g_hichao_image_service_url;

string g_throne_service_ip;
string g_throne_service_port;

string g_customer_message_service_ip;
string g_customer_message_service_port;

string g_redis_ip;
string g_redis_port;

string g_mysql_connect_str;
string g_mysql_user;
string g_mysql_passwd;
string g_mysql_database_name;

string g_mysql_upload_connect_str;
string g_mysql_upload_user;
string g_mysql_upload_passwd;
string g_mysql_upload_database_name;

string g_activemq_url;
string g_activemq_username;
string g_activemq_password;
string g_activemq_topic;

string g_push_url;

simple_pool::ptr_t g_pool;

ThriftPool<throne::ThroneClient,TBufferedTransport>* g_pthrone_client_pool=NULL;

vector<string> g_userids_tests;

std::string g_business_info_url;

#define PRE_CUSTOMR_TYPE    "售前"
#define AFTER_CUSTOMR_TYPE  "售后"
#define ALL_CUSTOMR_TYPE    "售前兼售后"

std::string display_customer_privilege(const int32_t type_id) {
    if(type_id == 0){
        return PRE_CUSTOMR_TYPE;
    }else if(type_id == 1){
        return AFTER_CUSTOMR_TYPE;
    }else if(type_id == 2){
        return ALL_CUSTOMR_TYPE;
    }
}

/* trans shop's type value to chat's */
int32_t trans_customer_privilege(const int32_t shop_service_type){
    int32_t chat_customer_type = 0;
    switch(shop_service_type){
    case 1:
    case 3:{
        chat_customer_type = 2;
        break;
    }
    case 4:{
        chat_customer_type = 0;
        break;
    }
    case 5:{
        chat_customer_type = 1;
        break;
    }
    default:{
        chat_customer_type = 0;
        break;
    }
    }
    return chat_customer_type;
}

class ChatServiceHandler : virtual public ChatServiceIf {
public:
  ChatServiceHandler() {
    BLT_I<<"get up!"<<endl;
  }
  bool LoginServer(const std::string& name, const std::string& server_type, const std::string& ip, const std::string& port)
  {
    BLT_I<<"login server begin";

    bool ret=g_servers.Add(Servers::Info{name,server_type,ip,port});
    g_servers.Expire(name,SERVER_MONITOR_TIMEOUT);
    g_users.ClearInvalidUsers(name);

    BLT_I<<"login server:name="<<name<<",ret="<<ret<<",ip="<<ip<<",port="<<port;
    return ret;
  }

  void HeartbeatTrigger(const std::string& server_name) {
    //BLT_D<<"heart beat:name="<<server_name;

    g_servers.Expire(server_name, SERVER_MONITOR_TIMEOUT);
  }

  bool ServerStatusReader(const std::string& server_name) {
    BLT_D<<"server status reader:name="<<server_name;

    return g_servers.IsExpire(server_name);
  }

  void SetState(const string&sourcetype,int userid,int state){
    BLT_D<<"set state:sourcetype="<<sourcetype<<",userid="<<userid<<",state="<<state;
    std::cout <<"set state:sourcetype="<<sourcetype<<",userid="<<userid<<",state="<<state << std::endl;
    
    bool is_customer=(sourcetype!="app");
    //source is online 
    Users::Model um_source=g_users.Get(userid,is_customer);
    if(!um_source.loaded()  &&  g_servers.IsExpire(um_source.servername())){
      BLT_W<<"source is not online,error:source="<<userid;
      return;
    }

    Users::Model um_new(um_source.userid(),um_source.is_customer(),state,um_source.servername(),um_source.sellerid(), um_source.is_pre_sale(), um_source.is_after_sale());
    g_users.Save(um_new);
  }
  int GetState(const string& sourcetype,int userid){
    BLT_D<<"get state:sourcetype="<<sourcetype<<",userid="<<userid;
    bool is_customer=(sourcetype!="app");

    Users::Model um_source=g_users.Get(userid,is_customer);
    if(!um_source.loaded()  &&  g_servers.IsExpire(um_source.servername())){
      BLT_E<<"source is not online,error:source="<<userid;
      return USER_OFFLINE;
    }

    BLT_I<<"get state:sourcetype="<<sourcetype<<",userid="<<userid<<",state="<<um_source.state();
    return um_source.state();
  }

  void GetCustomerListByBusinessId(std::vector<CustomerInfo> & _return, const int32_t businessid)
  {
    BLT_D<<"get customer list by business id:businessid="<<businessid;
    
      std::vector<service_t> customerlist;
      IBusiness business;
      int rt = business.get_services(g_business_info_url, lexical_cast<std::string>(businessid), "0", customerlist);  
      if(rt < 0){
          BLT_E<<"Failed to get customer list of business,error:businessid="<<businessid;
          return;
      }
      BLT_D<<"customer list size:="<<customerlist.size();
      std::vector<service_t>::iterator iter  =  customerlist.begin();
      while (iter  != customerlist.end()) {
          CustomerInfo ci;
          ci.id  =  (*iter).service_id;
          ci.title  =  (*iter).service_title;
          ci.nickname  =  (*iter).service_nickname;
          ci.type  =  (*iter).service_type;
          ci.token  =  (*iter).token;
          _return.push_back(ci);
          iter++;
      }
  }

  void GetOrderDetailByOrderId(OrderDetail& od, const int32_t orderid, const int32_t customerid) {
    
    order_t order;
    IBusiness business;
    // 判断是否为虚拟客服，虚拟客服不能访问商家订单(不提供order_url)
    service_t customer_info;
    business.get_service_by_id(g_business_info_url, lexical_cast<string>(customerid), customer_info);
    bool is_proxy_customer = lexical_cast<bool>(customer_info.is_agent);
    business.get_order_detail_by_id(g_business_info_url,lexical_cast<string>(orderid),order);
    od.order_id = order.order_id;
    if(is_proxy_customer)
        od.order_url="";
    else
        od.order_url=order.order_url;
    od.order_status = order.order_status;
    od.shipping_status  = order.shipping_status;
    od.pay_status = order.pay_status;
    od.address = order.address;
    od.order_add_time=order.order_add_time;
    od.order_total_amount=order.order_total_amount;
    od.order_sn=order.order_sn;
    for(int j=0;j<order.goodslist.size();j++){
      goodslist_detail_t gl_detail;
      gl_detail.url  = order.goodslist[j].url;
      gl_detail.goods_image  = order.goodslist[j].goods_image;
      gl_detail.goods_name  = order.goodslist[j].goods_name;
      gl_detail.market_price = order.goodslist[j].market_price;
      gl_detail.goods_number  = order.goodslist[j].goods_number;
      gl_detail.goods_price  = order.goodslist[j].goods_price;
      od.goodslist.push_back(gl_detail);
    }
  }

  void __GetCustomerOnlineState(const int32_t business_id,vector<BusinessOnlineState>& rets)
  {
    std::vector<service_t> services;

    vector<int> ids;
    IBusiness business;
    business.get_services(g_business_info_url,lexical_cast<string>(business_id),"0", services);
    for(service_t& s:services){
      ids.push_back(lexical_cast<int>(s.service_id));
    }
    
    for(int id:ids){
      BusinessOnlineState item;
      item.id=id;

      Users::Model um=g_users.Get(id,true);
      item.state=USER_OFFLINE;

      if(um.loaded() && !g_servers.IsExpire(um.servername())){
	item.state=um.state();
      }
      rets.push_back(item);
    }
  }

  void GetCustomerOnlineState(BusinessOnlineStates& _return,const int32_t business_id)
  {
    __GetCustomerOnlineState(business_id,_return.current);
    __GetCustomerOnlineState(PLATFORM_CUSTOMER_USERID,_return.platform);
    BLT_D<<"GetCustomerOnlineState:"<<_return.current.size()<<","<<_return.platform.size();
  }

  void GetLastContactUsers(std::vector<ContactInfos> & _return, const int32_t id, const int32_t type){

    BLT_D<<"get last contact users of "<<id<<"("<<type<<")";

    sql::Driver* driver;
    sql::Connection* conn;
    sql::Statement* stmt;
    sql::ResultSet* res;

    driver = get_driver_instance();
    conn = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    conn->setSchema(g_mysql_database_name);

    sql::PreparedStatement* prep_stmt = conn->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = conn->createStatement();

    if ((type!=ROLE_USER) and (type!=ROLE_CUSTOMER)){
        //throw exception
        return;
    }

    time_t tt = time(NULL);
    struct tm t;
    localtime_r(&tt, &t);
    tt = tt - (t.tm_sec + t.tm_min*60 + t.tm_hour*60*60 + 1*24*60*60); 
    localtime_r(&tt, &t);
    stringstream st;
    //st<<1900+t.tm_year<<"-"<<t.tm_mon+1<<"-"<<t.tm_mday<<" "<<t.tm_hour<<":"<<t.tm_min<<":"<<t.tm_sec;

    stringstream ss;
    /*
    ss<<"select distinct sender, sender_role, receiver, receiver_role from message_log where ((sender_role="<<
        type<<" and sender="<<id<<") or (receiver_role="<<type<<" and receiver="<<id<<")) and (send_time >= \""<<
        st<<"\") order by send_time desc";
    */
    /*
    ss<<"select distinct sender, sender_role, receiver, business_id from message_log where ((sender_role="<<
        type<<" and sender="<<id<<") or (receiver_role="<<type<<" and receiver="<<id<<")) and (send_time >= \""<<
        st.str()<<"\") order by send_time desc limit 99";
    */
    ss<<"select distinct a.sender, a.sender_role, a.receiver, a.receiver_role, a.business_id  from (select sender, sender_role, receiver, receiver_role, business_id from message_log where (sender_role="<<type<<" and sender="<<id<<") or (receiver_role="<<type<<" and receiver="<<id<<") order by send_time desc limit 1000) a limit 99";
    BLT_D<<"GetLastContactUsers sql:"<<ss.str();
    res=stmt->executeQuery(ss.str());

    vector<int> uid_vec;
    vector<throne::UserWithName> user_Vec;
    vector<throne::UserWithName>::iterator user_Vec_iter;
    int sender;
    int sender_role;
    int receiver;
    int business_id;
    //int receiver_role;

    std::map< int, std::set<int> > business_users_map;
    vector<int> business_id_vec;

    while(res->next()){
        sender        = res->getInt("sender");
        sender_role   = res->getInt("sender_role");
        receiver      = res->getInt("receiver"); 
        business_id = res->getInt("business_id"); 

        int user_id = sender;
        if (sender_role == type) 
            user_id = receiver;
        uid_vec.push_back(user_id);
        business_id_vec.push_back(business_id);
        business_users_map[business_id].insert(user_id);
    }
    // 获取用户名
    std::map<int, UserInfo> users_map;
    if (uid_vec.size() > 0){
        std::sort(uid_vec.begin(), uid_vec.end());
        std::vector<int>::iterator it_ = std::unique(uid_vec.begin(), uid_vec.end()); // 去重
        uid_vec.resize(std::distance(uid_vec.begin(), it_));
        try{
            INTERFACE2(user_Vec, g_pthrone_client_pool, get_user_with_name_by_uids, uid_vec);
        }catch(TException e){
            BLT_E<<"throne thow exception: "<<e.what();
        }
        for(user_Vec_iter=user_Vec.begin(); user_Vec_iter!=user_Vec.end(); user_Vec_iter++){
            UserInfo ui;
            ui.id = user_Vec_iter->user_id;
            ui.username = user_Vec_iter->loginname;
            ui.nickname = user_Vec_iter->nickname;
            users_map[ui.id] = ui;
        }
    }
    // 获得商家品牌名称
    std::map<int, UserInfo> business_map;
    if (business_id_vec.size() > 0) {
        std::sort(business_id_vec.begin(), business_id_vec.end());
        std::vector<int>::iterator it_ = std::unique(business_id_vec.begin(), business_id_vec.end());  // 去重
        business_id_vec.resize(std::distance(business_id_vec.begin(), it_));
        std::vector<business_t> business_vec;
        IBusiness business;
        // 批量接口
        if(business.get_business_detail(g_business_info_url, business_id_vec, business_vec)<0){
            BLT_E<<"get_business_detail failed";
            return;
        }
        for(int i=0; i<business_vec.size(); i++)
        {
            UserInfo person;
            person.id = boost::lexical_cast<int>(business_vec[i].business_id);
            person.username = business_vec[i].brand_name;
            business_map[person.id] = person;
        }
        /*
        for(int i=0; i<business_id_vec.size(); i++)
        {
            business_t bn;
            business.get_business_detail(g_business_info_url, boost::lexical_cast<string>(business_id_vec[i]), bn);
            UserInfo person;
            person.id = boost::lexical_cast<int>(bn.business_id);
            person.username = bn.brand_name;
            business_map[person.id] = person;
        }
        */
    }
    std::map<int, ContactInfos> ret;
    {
        // 组成层次状结构
        std::map<int, std::set<int> >::iterator iter = business_users_map.begin();
        for(; iter!=business_users_map.end(); iter++) {
            int business_id = iter->first;
            std::set<int>& user_ids = iter->second;
            ContactInfos& contact_info = ret[business_id];
            contact_info.business_id = business_id;
            contact_info.brand_name = business_map[business_id].username;
            std::set<int>::iterator iter_set = user_ids.begin();
            for(; iter_set != user_ids.end(); iter_set++) {
                int user_id = *iter_set;
                UserInfo& userinfo = users_map[user_id];
                contact_info.users.push_back(userinfo);
            }
        }
    }

    std::map<int, ContactInfos>::iterator iter = ret.begin();
    for(; iter!=ret.end(); iter++) {
        _return.push_back(iter->second);
    }

    for(int i=0; i<_return.size(); i++) {
        ContactInfos& info = _return[i];
        std::cout<<"business_id: "<< info.business_id<<std::endl;
        std::cout<<"brand_name: "<< info.brand_name<<std::endl;
        for(int j=0; j<info.users.size(); j++) {
            std::cout<<"  id: "<< info.users[j].id;
            std::cout<<"  name: "<< info.users[j].username;
            std::cout<<"  nick: "<< info.users[j].nickname<<std::endl;
        }   
        std::cout<<std::endl;
    }

    delete res;
    delete stmt;
    delete conn;
    delete prep_stmt;
  }

  void FindHistoryMessages(std::vector<HistoryMessage> & _return, const int32_t userid, const int32_t businessid, const int32_t customerid, const int64_t begin_time, const int64_t end_time, const int32_t pagenumber, const int32_t pagesize)
  {
    BLT_D<<"find history message:userid="<<userid<<",businessid="<<businessid<<",customerid="<<customerid<<",begin_time"<<begin_time<<",end_time="<<end_time<<",pagenumber="<<pagenumber<<",pagesize="<<pagesize;
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    con->setSchema(g_mysql_database_name);

    sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = con->createStatement();

    stringstream ss;
    ss<<"select id,sender,sender_role,business_id,receiver,receiver_role,UNIX_TIMESTAMP(send_time),content,msg_type from message_log where "
      <<"business_id="<<businessid<<" and send_time>FROM_UNIXTIME("<<begin_time<<") and send_time<FROM_UNIXTIME("<<end_time<<")";
    if(userid>0){
      ss<<" and ((sender_role="<<ROLE_USER<<" and sender="<<userid<<") or (receiver_role="<<ROLE_USER<<" and receiver="<<userid<<"))";
    }
    if(customerid>0){
      ss<<" and ((sender_role="<<ROLE_CUSTOMER<<" and sender="<<customerid<<") or (receiver_role="<<ROLE_CUSTOMER<<" and receiver="<<customerid<<"))";
    }
    ss << " order by id asc limit "  << (pagenumber-1)*pagesize  << ","  <<  pagesize;


    BLT_D<<"FindHistoryMessages sql:"<<ss.str();
    res=stmt->executeQuery(ss.str());
    vector<std::string> ids;
    int count=0;
    while(res->next()){
      count++;
      HistoryMessage m;

      int sender_role=res->getInt(3);

      int type=res->getInt(9);
      m.type  =  type;
      /*
      if(type==MSG_TYPE_TEXT){
	if(sender_role==ROLE_USER){
	  m.type=HISTORY_TYPE_USER_TO_SELLER_TEXT;
	}
	else{
	  m.type=HISTORY_TYPE_SELLER_TO_USER_TEXT;
	}
      }
      else if(type==MSG_TYPE_IMAGE){
	if(sender_role==ROLE_USER){
	  m.type=HISTORY_TYPE_USER_TO_SELLER_IMAGE;
	}
	else{
	  m.type=HISTORY_TYPE_SELLER_TO_USER_IMAGE;
	}
      }
      else if(type==MSG_TYPE_GOODS)
	m.type=HISTORY_TYPE_GOODS;
      else if(type==MSG_TYPE_ORDER)
	m.type=HISTORY_TYPE_ORDER;
    */
      
      m.timestamp=res->getInt(7);
      m.text=res->getString(8);
      if(m.type == MSG_TYPE_IMAGE){
         ids.push_back(m.text);
      }
      m.businessid=res->getInt(4);
      m.senderid=res->getInt(2);
      m.receiverid=res->getInt(5);
      
      if(sender_role==ROLE_USER){
	m.userid=res->getInt(2);
	m.customerid=res->getInt(5);
      }
      else{
	m.userid=res->getInt(5);
	m.customerid=res->getInt(2);
      }
      
      _return.push_back(m);
    }
    
    BLT_D<<"history message count:"<<count;
    std::vector<std::string> urls;
    urls  =  get_image_urls_from_hichao_image_by_ids(ids);
    std::vector<HistoryMessage>::iterator iter  =  _return.begin();
    int i = 0;
    if((urls.size()  != 0)  && (urls.size()  ==  ids.size())){
      while (iter != _return.end()) {
          if(i  == ids.size()){
              break;
          }
       if(((*iter).type == MSG_TYPE_IMAGE) && ((*iter).text  == ids[i])){
          (*iter).text  =  urls[i]; 
          i++;
       }
       iter++;
      }
    }
//    reverse(_return.begin(),_return.end());

    delete con;
    delete stmt;
    delete res;
    delete prep_stmt;
  }

  void GetHistoryMessagesForWeb(HistoryMessageWeb & _return, const string& sourcetype, const int32_t userid, const int32_t businessid, const int32_t pagenumber, const int32_t pagesize) {
    BLT_D<<"get history messages for web:sourcetype="<<sourcetype<<",userid="<<userid<<",businessid="<<businessid<<",pagenumber="<<pagenumber<<",pagesize="<<pagesize;

    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res,*rescnt;

    driver = get_driver_instance();
    con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    con->setSchema(g_mysql_database_name);

    sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = con->createStatement();
    
    stringstream ss;
      ss<<"select sender,sender_role,business_id,receiver,receiver_role,UNIX_TIMESTAMP(send_time),content,msg_type from message_log where "
	<<"(sender_role="<<ROLE_USER<<" and sender="<<userid<<" and business_id="<<businessid<<") or ("
	<<"receiver_role="<<ROLE_USER<<" and receiver="<<userid<<" and business_id="<<businessid<<") ";
      ss<<" order by id desc limit "<<(pagenumber-1)*pagesize << ","  << pagesize;
    stringstream sscnt;
      sscnt<<"select count(id) from message_log where "
	<<"(sender_role="<<ROLE_USER<<" and sender="<<userid<<" and business_id="<<businessid<<") or ("
	<<"receiver_role="<<ROLE_USER<<" and receiver="<<userid<<" and business_id="<<businessid<<") ";

      BLT_D<<"sql:"<<ss.str();
    res=stmt->executeQuery(ss.str());
    BLT_D<<"sql:"<<sscnt.str();
    rescnt=stmt->executeQuery(sscnt.str());
    
    vector<std::string> ids;
    int count=0;
    while(res->next()){
      HistoryMessage m;

      count++;
      int sender_role=res->getInt(2);

      int type=res->getInt(8);
      m.type  =  type;
/*
      if(type==MSG_TYPE_TEXT){
	if(sender_role==ROLE_USER){
	  m.type=HISTORY_TYPE_USER_TO_SELLER_TEXT;
	}
	else{
	  m.type=HISTORY_TYPE_SELLER_TO_USER_TEXT;
	}
      }
      else{
	if(sender_role==ROLE_USER){
	  m.type=HISTORY_TYPE_USER_TO_SELLER_IMAGE;
	}
	else{
	  m.type=HISTORY_TYPE_SELLER_TO_USER_IMAGE;
	}
      }
      */
      
      m.timestamp=res->getInt(6);
      m.text=res->getString(7);
      if(m.type == MSG_TYPE_IMAGE){
         ids.push_back(m.text);
      }
      m.businessid=res->getInt(3);
      m.senderid=res->getInt(1);
      m.receiverid=res->getInt(4);
      
      if(sender_role==ROLE_USER){
	m.userid=res->getInt(1);
	m.customerid=res->getInt(4);
      }
      else{
	m.userid=res->getInt(4);
	m.customerid=res->getInt(1);
      }
      _return.msgs.push_back(m);
    }

    BLT_D<<"web history message count:"<<count;
    std::vector<std::string> urls;
    urls  =  get_image_urls_from_hichao_image_by_ids(ids);
    std::vector<HistoryMessage>::iterator iter  =  _return.msgs.begin();
    int i = 0;
    if((urls.size()  != 0)  && (urls.size()  ==  ids.size())){
      while (iter != _return.msgs.end()) {
          if(i  == ids.size()){
              break;
          }
       if(((*iter).type == MSG_TYPE_IMAGE) && ((*iter).text  == ids[i])){
          (*iter).text  =  urls[i]; 
          i++;
       }
       iter++;
      }
    }
    reverse(_return.msgs.begin(),_return.msgs.end());

    while(rescnt->next()) {
        _return.msgSumTotal  =  rescnt->getInt(1);
    }

    delete con;
    delete stmt;
    delete res;
    delete rescnt;
    delete prep_stmt;
  }

  void GetInfoListById(std::vector<PersonInfo> & _return, const std::vector<int32_t> & idslist, const int32_t type, const int32_t businessid){
      BLT_D<<"get info list by id";

      if(type  ==  ID_CUSTOMER_TYPE) {
          service_t service;
          IBusiness business;
          std::vector<int32_t>::const_iterator iter  =  idslist.begin();
          while (iter  != idslist.end()) {
              if(business.get_service_by_id(g_business_info_url,lexical_cast<string>(*iter),service) < 0){
                  BLT_E<<"userid is not found,exit:id="<<*iter;
              }
              PersonInfo info;
              info.id  =  lexical_cast<string>(*iter);
              info.name  =  service.service_title;
              info.nickname  =  service.service_nickname;
              _return.push_back(info);
              iter++;
          }
      } else if (type  ==  ID_USER_TYPE) {
          IBusiness business;
          connection::ptr_t c = g_pool->get();
          std::vector<throne::UserWithName> userVec;
          try {
              INTERFACE2(userVec,g_pthrone_client_pool,get_user_with_name_by_uids,idslist);
          } catch(TException e){
              BLT_W<<"exceed the uid number limit,max = 99, in getInfoListByIds";
          }
          std::vector<throne::UserWithName>::const_iterator iter  =  userVec.begin();
          while (iter  != userVec.end()) {
              PersonInfo info;
              std::string tmp  =  lexical_cast<string>((*iter).user_id);
              if(businessid!=PLATFORM_CUSTOMER_USERID){
                  patterns::simple_obj_store<UserToCustomer> store;
                  UserToCustomer utc = store.find(c,UserToCustomer::getid(lexical_cast<int>(tmp),businessid));
                  if(!utc.loaded()){
                      BOOST_LOG_TRIVIAL(warning)<<"no utc in redis: " << tmp << " businessid:" << businessid;
                      //v1.4.0
                      //检查是否有过订单
                      bool has_order = business.user_has_orders(g_business_info_url, lexical_cast<string>(businessid), tmp);
                      if(!has_order){ // 不存在订单
                        BLT_D<<"no order business_id="<<businessid<<" user_id="<<tmp;
                        iter++;
                        continue;
                      }
                  }
              }
              info.id  =  tmp;
              info.name  =  (*iter).loginname;
              info.nickname  =  (*iter).nickname;
              _return.push_back(info);
              iter++;
          }
          g_pool->put(c);
      }
  }

  void GetContactListById(std::vector<ContactInfo> & _return, const int32_t businessid, const int32_t customerid, const int32_t userid, const int64_t begin_time, const int64_t end_time){
    BLT_D<<"get contact list by id:businessid="<<businessid<<",customerid="<<customerid<<",userid="<<userid<<",begin_time="<<begin_time<<",end_time="<<end_time;
    
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    if ((businessid <=  0)  &&  (customerid <=  0)  &&  (userid  <= 0)) {
        BLT_E<<"GetContactListById invalid param!!!";
        return;
    }

    driver = get_driver_instance();
    con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    con->setSchema(g_mysql_database_name);

    sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = con->createStatement();

    stringstream ss;
    if(userid <= 0) {
        // select user list of customerid
      ss<<"select tmp.sender,tmp.send_time from (SELECT sender,max(send_time) AS send_time from message_log where receiver="<< customerid  << " and receiver_role = " << ROLE_CUSTOMER <<  " and "
          <<" business_id="<<businessid<<" and send_time>FROM_UNIXTIME("<<begin_time<<") and send_time<FROM_UNIXTIME("<<end_time<<") GROUP BY sender) AS tmp  ORDER BY tmp.send_time DESC";
    } else {
        // select customer list of userid
        ss<<"select tmp.receiver,tmp.send_time from (SELECT receiver,max(send_time) AS send_time from message_log where sender=" << userid  << " and sender_role =  " << ROLE_USER <<  " and "
          <<" business_id="<<businessid<<" and send_time>FROM_UNIXTIME("<<begin_time<<") and send_time<FROM_UNIXTIME("<<end_time<<") GROUP BY receiver) AS tmp  order by tmp.send_time DESC";
    }

    BLT_D<<ss.str();
    res=stmt->executeQuery(ss.str());
    if(userid  <=  0){
        std::vector<int> useridVec;
        while(res->next()){
            int userid  =  res->getInt(1);
            if(userid < 0){
                continue;
            }
	    useridVec.push_back(userid);
        }
        std::vector<throne::UserWithName> userVec;
        try {
            INTERFACE2(userVec,g_pthrone_client_pool,get_user_with_name_by_uids,useridVec);
        } catch(TException e){
	        BLT_E<<"exceed the uid number limit, max = 99, in getContactListById";
        }
        std::vector<throne::UserWithName>::const_iterator iter  =  userVec.begin();
        while (iter  != userVec.end()) {
            ContactInfo info;
            info.id  =  lexical_cast<string>((*iter).user_id);
            info.name  =  (*iter).loginname;
            info.nickname  =  (*iter).nickname;
            _return.push_back(info);
            iter++;
        }
    } else {
        service_t service;
        IBusiness business;
        while(res->next()){
            int customerid  =  res->getInt(1);

            if(customerid < 0){
                continue;
            }
	        if(business.get_service_by_id(g_business_info_url,lexical_cast<string>(customerid),service) < 0){
	              BLT_W<<"customerid is not found,exit:id="<<customerid;
            }
            ContactInfo info;
            info.id  =  lexical_cast<string>(customerid);
            info.name  =  service.service_title;
            info.nickname  =  service.service_nickname;
            _return.push_back(info);
        }
    }
    reverse(_return.begin(),_return.end());

    delete con;
    delete stmt;
    delete res;
    delete prep_stmt;
  }
  
  void GetHistoryMessages(HistoryMessageTag & _return, const string& sourcetype,const int32_t id, const int32_t businessid, const int32_t msgid, const int32_t psize){
    BLT_D<<"get history mesages:sourcetype="<<sourcetype<<",userid="<<id<<",businessid="<<businessid<<",msgid="<<msgid<<",psize="<<psize;
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    con->setSchema(g_mysql_database_name);

    sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = con->createStatement();
    
    stringstream ss;
    //if(sourcetype=="app"){
      ss<<"select id,sender,sender_role,business_id,receiver,receiver_role,UNIX_TIMESTAMP(send_time),content,msg_type from message_log where "
	<<"((sender_role="<<ROLE_USER<<" and sender="<<id<<" and business_id="<<businessid<<") or ("
	<<"receiver_role="<<ROLE_USER<<" and receiver="<<id<<" and business_id="<<businessid<<")) " << " and msg_type<3 ";
      if(msgid>=0)
	ss<<"and id<"<<msgid; 
      ss<<" order by id desc limit "<<psize+1;
      //}
    /*
    else if(sourcetype=="web"){
      ss<<"select id,sender,sender_role,business_id,receiver,receiver_role,UNIX_TIMESTAMP(send_time),content,msg_type from message_log where "
	<<"(sender_role="<<ROLE_CUSTOMER<<" and sender="<<id<<" and business_id="<<businessid<<") or ("
	<<"receiver_role="<<ROLE_CUSTOMER<<" and receiver="<<id<<" and business_id="<<businessid<<") ";
      if(msgid>=0)
	ss<<"and id<"<<msgid;
      ss<<" order by send_time desc limit "<<psize+1;
    }
    */
      BLT_D<<"sql:"<<ss.str();
    res=stmt->executeQuery(ss.str());


    int lastid=INT_MAX;
    
    std::vector<std::string> ids;
    while(res->next()){
      HistoryMessage m;

      int sender_role=res->getInt(3);

      m.text=res->getString(8);
      if(m.text.empty()){
          continue;
      }
      int type=res->getInt(9);
      if((type==MSG_TYPE_TEXT) || (type == MSG_TYPE_GOODS)){
	if(sender_role==ROLE_USER){
	  m.type=HISTORY_TYPE_USER_TO_SELLER_TEXT;
	}
	else{
	  m.type=HISTORY_TYPE_SELLER_TO_USER_TEXT;
	}
      }
      else{
	if(sender_role==ROLE_USER){
	  m.type=HISTORY_TYPE_USER_TO_SELLER_IMAGE;
	}
	else{
	  m.type=HISTORY_TYPE_SELLER_TO_USER_IMAGE;
	}
      BLT_D<<"image id ----:"<<m.text;
         ids.push_back(m.text);
      }
      
      m.timestamp=res->getInt(7);
      //m.text=res->getString(8);
      m.businessid=res->getInt(4);
      m.senderid=res->getInt(2);
      m.receiverid=res->getInt(5);
      
      if(sender_role==ROLE_USER){
	m.userid=res->getInt(2);
	m.customerid=res->getInt(5);
      }
      else{
	m.userid=res->getInt(5);
	m.customerid=res->getInt(2);
      }
      int id=res->getInt(1);
      if(id<lastid){
	lastid=id;
      }
      _return.msgs.push_back(m);
    }

    BLT_D<<"get "<<_return.msgs.size()<<" messages";

    std::vector<std::string> urls;
    urls  =  get_image_urls_from_hichao_image_by_ids(ids);
    std::vector<HistoryMessage>::iterator iter  =  _return.msgs.begin();
    int i = 0;
    if((urls.size()  != 0)  && (urls.size()  ==  ids.size())){
      while (iter != _return.msgs.end()) {
          if(i  == ids.size()){
              break;
          }

       if((((*iter).type == HISTORY_TYPE_SELLER_TO_USER_IMAGE) || ((*iter).type  == HISTORY_TYPE_USER_TO_SELLER_IMAGE) || ((*iter).type  ==  MSG_TYPE_IMAGE)) && ((*iter).text  == ids[i])){
           if(!urls[i].empty()) {
              (*iter).text  =  urls[i]; 
           }
      BLT_D<<"image id:"<<ids[i]<< " url:" <<urls[i];
          i++;
       }
       iter++;
      }
    }

    _return.msgid=lastid;
    _return.isnext=false;
    
    if(_return.msgs.size()==(psize+1)){
      _return.isnext=true;
      _return.msgs.pop_back();
    }
    
      BLT_D<<"history msg id:"<<_return.msgid << " has_next_page:" << _return.isnext;

    reverse(_return.msgs.begin(),_return.msgs.end());

    delete con;
    delete stmt;
    delete res;
    delete prep_stmt;
  }
/*
  void LoginWeb(LoginRetWeb& _ret,const std::string& servername,const std::string& session_id, int business_id){
  
  	  std::cout << "#########################################  LoginWeb ##################################" << std::endl;
      ptime pt1=microsec_clock::local_time();
      BLT_D<<"************ pt1="<<pt1;
      BLT_D<<"loginweb servername="<<servername<<",session_id="<<session_id<<",business_id="<<business_id;
      _ret.result=2;
    
      IBusiness business;
      service_t service;
      if(business.get_service_by_token(g_business_info_url,session_id,service)<0){
          BLT_E<<"customer is not found session_id="<<session_id;
          return;
      }
      int customer_id = lexical_cast<int>(service.service_id);
      if (customer_id <= 0){
          return;
      }
      _ret.result = 0;
      _ret.customer_id = customer_id;
      _ret.customer_name = service.service_nickname;

      // 1商家本身 3所有客服(既是售前也是售后) 4售前 5售后
      int32_t chat_customer_type = trans_customer_privilege(lexical_cast<int>(service.service_type));
      int32_t is_pre_sale = (chat_customer_type==0)||(chat_customer_type==2);
      int32_t is_after_sale = (chat_customer_type==1)||(chat_customer_type==2);
      customer_sched::set(g_pool, customer_id, 0);

      g_users.Add(customer_id,true,servername,-1, is_pre_sale, is_after_sale);
      BLT_D<<"g_users.Add customer_id="<<customer_id<<","<<servername<<",-1,"<<is_pre_sale<<","<<is_after_sale;


      ptime pt2=microsec_clock::local_time();
      BLT_D<<"************ pt2="<<pt1<<" pt2-pt1="<<pt2-pt1;
      // 获得客服服务的商家列表
      int ret=0;
      std::vector<business_t> bns;
      if((ret=business.get_business(g_business_info_url, service.service_id, bns)) < 0){
          BLT_E<<"get_business failed session_id="<<session_id<<" customer_id="<<customer_id<<" ret="<<ret;
          return;
      }
      
      ptime pt3=microsec_clock::local_time();
      BLT_D<<"************ pt3="<<pt3<<" pt3-pt1="<<pt3-pt1;
      vector<int> business_id_vec;
      map<int, string> business_map;
      for(int i=0; i<bns.size(); i++){
        int business_id = lexical_cast<int>(bns[i].business_id);
        business_id_vec.push_back(business_id);
        business_map[business_id] = bns[i].brand_name;
      }
      GetOfflineMessageWeb(business_id_vec, _ret.messages, chat_customer_type);
      vector<int> business_id_with_off_msg_vec;
      for(int i=0; i<_ret.messages.size(); i++){
          _ret.messages[i].business_name = business_map[_ret.messages[i].business_id];
          business_id_with_off_msg_vec.push_back(_ret.messages[i].business_id);
      }
      ptime pt4=microsec_clock::local_time();
      BLT_D<<"************ pt4="<<pt4<<" pt4-pt1="<<pt4-pt1;
      update_message_customer(customer_id, business_id_with_off_msg_vec, chat_customer_type);
      std::string business_id_vec_str = util_int_vec_to_string(business_id_vec);
      BLT_I<<"a customer is logined session_id="<<session_id<<",userid="<<_ret.customer_id<<",name="<<_ret.customer_name<<",business="<<business_id_vec_str<<",servername="<<servername;
      ptime pt5=microsec_clock::local_time();
      BLT_D<<"************ pt5="<<pt5<<" pt5-pt1="<<pt5-pt1;

      StatClient* stat = new StatClient(g_mysql_connect_str, g_mysql_database_name, g_mysql_user, g_mysql_passwd);
      stat->op(STAT_CLIENT_OP_LOGIN, STAT_CLIENT_ROLE_CUSTOMER, customer_id, 0, 0, business_id);
      delete stat;
      return;
  }
*/
  void LoginWeb(LoginRetWeb& _ret1,const std::string& servername1,const std::string& session_id, int business_id){
	  std::cout << "#########################################  Login ##################################" << std::endl;
	  //app_thrift_test,token=5qKNblvFvGfOf6IgBSKMkOKOR6TzMDrHE5FSA9NSAgI,sellerid=75,sku_id=0,type=0
	  LoginRet _ret;
	  const std::string servername = "app_thrift_test";
	  const std::string token = "5qKNblvFvGfOf6IgBSKMkOKOR6TzMDrHE5FSA9NSAgI";
	  int sellerid = 75;
	  int sku_id = 0;
	  int type = 0;
	  std::cout << "########################################### Login ##################################" << std::endl;
	  BLT_D<<"login:servername="<<servername<<",token="<<token<<",sellerid="<<sellerid<<",sku_id="<<sku_id<<",type="<<type;
	  ptime tb=microsec_clock::local_time();

 	//   int GetState(const string& sourcetype,int userid);
	 

	/*
	  bool is_customer=(sellerid<0)?true:false;
	  std::cout << " is_customer: " << is_customer << std::endl;
	  if(is_customer){
		  return ;
	  }
	*/
	  int userid=-1;
	  IBusiness business;
	  int need_after_sale = (sku_id>0 && type==LOGIN_TYPE_ORDER)?true:false; // 售前还是售后(指定订单id)
	  int need_pre_sale = (need_after_sale+1)%2;

	  //user
	  for(int i=0;i<g_userids_tests.size();i++){
		  if(token==g_userids_tests[i]){
			  userid=lexical_cast<int>(g_userids_tests[i]);
			  break;
		  }
	  }
	  string s;

	  if(userid<0){
		  try{
			  INTERFACE(userid,g_pthrone_client_pool,get_uid_by_token,token);
		  }catch(TException e){
			  BLT_E<<"throne throw exception: "<<e.what();
		  }
		  {
			  ptime te=microsec_clock::local_time();
			  BLT_D<<"user get:"<<te-tb;
		  }
		  if(userid<0){
			  _ret.result=2001;
			  return;
		  }
	  }
	  _ret.uid=userid;

	  /* 客服统计 */
	  StatClient* stat = new StatClient(g_mysql_connect_str, g_mysql_database_name, g_mysql_user, g_mysql_passwd);
	  stat->op(STAT_CLIENT_OP_LOGIN, STAT_CLIENT_ROLE_USER, userid, 0, 0, sellerid);
	  delete stat;

	  bool ret=g_users.Add(userid,false,servername,sellerid, need_pre_sale, need_after_sale);
	  {
		  ptime te=microsec_clock::local_time();
		  BLT_D<<"user add:"<<te-tb;
	  }
	  BLT_D<<"login:userid="<<userid<<",ret="<<ret<<",servername="<<servername;

	  if(!ret){
		  _ret.result=2;
		  return;
	  }
	  //user login
	  _ret.welcome_text="您好，请问有什么能帮到您？";

	  business_t bn;
	  if(business.get_business_detail(g_business_info_url,lexical_cast<string>(sellerid),bn)<0){
		  BLT_E<<"businessid is not found,exit:token="<<token;
		  return;
	  }
	  _ret.business_name  =  bn.brand_name;
	  BLT_D<<"businessid: " << sellerid  << " businessname: "<<bn.brand_name;
	  //{
	  //  ptime te=microsec_clock::local_time();
	  //  BLT_D<<"get businesid:"<<te-tb;
	  //}

	  //leave message
	  //GetOfflineMessage("app",userid,sellerid,_ret.leave_messages);
	  //{
	  // ptime te=microsec_clock::local_time();
	  // BLT_D<<"offline message:"<<te-tb;
	  //}

	  try{
		  boost::shared_ptr<message::CustomerMessageServiceClient> customer_message_server=GetCustomerMessageServer();
		  customer_message_server->clear_count(userid,sellerid);
	  } catch(TException e){
		  BLT_E<<"Customer message server clear count exception:"<<e.what();
	  }

	  //send welcome to customer
	  int customerid=-1;
	  try{
		  customerid=select_customer_login(userid, sellerid, need_after_sale);
		  BLT_D<<"select_customer_login userid="<<userid<<" business_id="<<sellerid<<" sale_type="<<need_after_sale<<" slected customerid="<<customerid;
	  }catch(std::exception& e){
		  BLT_E<<"catch exception: "<<e.what()<<endl;
	  }

	  if(customerid>0){
		  business.get_message_by_id(g_business_info_url,lexical_cast<string>(sellerid),"1",_ret.welcome_text);
		  //客服选举：客服引用递增
		  int usf_result = customer_sched::usr_stat_flter::get(g_pool, userid);
		  if (usf_result == 0){
			  customer_sched::incr(g_pool, customerid);
			  customer_sched::usr_stat_flter::set(g_pool, userid, 1);
		  }
	  }
	  else{
		  business.get_message_by_id(g_business_info_url,lexical_cast<string>(sellerid),"2",_ret.welcome_text);
	  }

	  //  获取客服在线状态 和 回复语
	  //int get_customer_status_and_reply(std::string business_id,std::string customer_id,std::string &json_str);
	  IBusiness ib;
	  std::string  json_ret;
	  ib.get_customer_status_and_reply(business_id,customerid,json_ret);


	  if(customerid <= 0){
		  _ret.welcome_text="商家客服离线请留言，或拨打明星衣橱官方客服电话：4006506468（每天9:00-22:00）";
		  _ret.result=0;
	  } else {
		  _ret.result=1;
	  }
	  BLT_D<<"customerid:"<<customerid<<" welcome_text:"<<_ret.welcome_text<<" result="<<_ret.result;

	  if(sku_id>0){
		  string u;
		  //IBusiness ib;
		  switch(type){
			  case LOGIN_TYPE_SKU:
				  {
					  goods_t goods;
					  ib.get_goods_detail_by_id(g_business_info_url,lexical_cast<string>(sellerid),lexical_cast<string>(sku_id),goods);
					  u=goods.goods_url;
					  BLT_D<<"InsertUserSkuOrder sku:userid="<<userid<<",sellerid="<<sellerid<<",sku_id="<<sku_id<<",u="<<u;
					  InsertUserSkuOrder(userid,sku_id,-1,sellerid);
				  }
				  break;
			  case LOGIN_TYPE_ORDER:
				  {
					  order_t order;
					  ib.get_order_detail_by_id(g_business_info_url,lexical_cast<string>(sku_id),order);
					  u=order.order_url;
					  BLT_D<<"InsertUserSkuOrder order:userid="<<userid<<",sellerid="<<sellerid<<",sku_id="<<sku_id<<",u="<<u;
					  InsertUserSkuOrder(userid,-1,sku_id,sellerid);
				  }
				  break;
		  }

		  if(customerid > 0){
			  BLT_D<<"send MSG_TYPE_LOGIN_NOTIFY message userid="<<userid<<" business_id="<<sellerid<<" customerid="<<customerid;
			  boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
			  SendMessage(lexical_cast<string>(a_uuid),"app",userid,sellerid,"",MSG_TYPE_LOGIN_NOTIFY, sellerid); // 对app来说sellerid即为business_id
		  }
	  }
	  if(type==LOGIN_TYPE_SKU){
		  goods_t gdt;
		  business.get_goods_detail_by_id(g_business_info_url,lexical_cast<std::string>(sellerid),lexical_cast<std::string>(sku_id),gdt);
		  GoodsDetail gd;
		  gd.goods_id=gdt.goods_id;
		  gd.goods_type=gdt.goods_type;
		  gd.goods_url=gdt.goods_url;
		  gd.goods_name=gdt.goods_name;
		  gd.goods_price=gdt.goods_shop_price;
		  gd.business_id=gdt.business_id;
		  gd.goods_img=gdt.goods_img;
		  _ret.gd=gd;
	  }
	  std::cout << "########################################### Login end ##############################" << std::endl;
  }

  void Login(LoginRet& _ret,const std::string& servername,const std::string& token,int sellerid,int sku_id,int type) {
  	std::cout << "####################################### Login ##################################" << std::endl;
    BLT_D<<"login:servername="<<servername<<",token="<<token<<",sellerid="<<sellerid<<",sku_id="<<sku_id<<",type="<<type;
    ptime tb=microsec_clock::local_time();

    bool is_customer=(sellerid<0)?true:false;
    if(is_customer){
        return ;
    }
    
    int userid=-1;
    IBusiness business;
    int need_after_sale = (sku_id>0 && type==LOGIN_TYPE_ORDER)?true:false; // 售前还是售后(指定订单id)
    int need_pre_sale = (need_after_sale+1)%2;

    //user
    for(int i=0;i<g_userids_tests.size();i++){
      if(token==g_userids_tests[i]){
		userid=lexical_cast<int>(g_userids_tests[i]);
		break;
      }
    }
    string s;
    
    if(userid<0){
      try{
        INTERFACE(userid,g_pthrone_client_pool,get_uid_by_token,token);
      }catch(TException e){
        BLT_E<<"throne throw exception: "<<e.what();
      }
      {
		ptime te=microsec_clock::local_time();
		BLT_D<<"user get:"<<te-tb;
      }
      if(userid<0){
		_ret.result=2001;
		return;
      }
    }
    
    _ret.uid=userid;

    /* 客服统计 */
    StatClient* stat = new StatClient(g_mysql_connect_str, g_mysql_database_name, g_mysql_user, g_mysql_passwd);
    stat->op(STAT_CLIENT_OP_LOGIN, STAT_CLIENT_ROLE_USER, userid, 0, 0, sellerid);
    delete stat;

    bool ret=g_users.Add(userid,false,servername,sellerid, need_pre_sale, need_after_sale);
    {
      ptime te=microsec_clock::local_time();
      BLT_D<<"user add:"<<te-tb;
    }
    BLT_D<<"login:userid="<<userid<<",ret="<<ret<<",servername="<<servername;

    if(!ret){
      _ret.result=2;
      return;
    }
    //user login
    _ret.welcome_text="您好，请问有什么能帮到您？";

	business_t bn;
	if(business.get_business_detail(g_business_info_url,lexical_cast<string>(sellerid),bn)<0){
	  BLT_E<<"businessid is not found,exit:token="<<token;
	  return;
	}
    _ret.business_name  =  bn.brand_name;
	BLT_D<<"businessid: " << sellerid  << " businessname: "<<bn.brand_name;
    //{
    //  ptime te=microsec_clock::local_time();
    //  BLT_D<<"get businesid:"<<te-tb;
    //}

    //leave message
    //GetOfflineMessage("app",userid,sellerid,_ret.leave_messages);
    //{
     // ptime te=microsec_clock::local_time();
     // BLT_D<<"offline message:"<<te-tb;
    //}

    try{
        boost::shared_ptr<message::CustomerMessageServiceClient> customer_message_server=GetCustomerMessageServer();
        customer_message_server->clear_count(userid,sellerid);
    } catch(TException e){
	    BLT_E<<"Customer message server clear count exception:"<<e.what();
    }

    //send welcome to customer
    int customerid=-1;
    try{
      customerid=select_customer_login(userid, sellerid, need_after_sale);
      BLT_D<<"select_customer_login userid="<<userid<<" business_id="<<sellerid<<" sale_type="<<need_after_sale<<" slected customerid="<<customerid;
    }catch(std::exception& e){
      BLT_E<<"catch exception: "<<e.what()<<endl;
    }

    if(customerid>0){
      business.get_message_by_id(g_business_info_url,lexical_cast<string>(sellerid),"1",_ret.welcome_text);
      //客服选举：客服引用递增
      int usf_result = customer_sched::usr_stat_flter::get(g_pool, userid);
      if (usf_result == 0){
        customer_sched::incr(g_pool, customerid);
        customer_sched::usr_stat_flter::set(g_pool, userid, 1);
      }
    }
    else{
      business.get_message_by_id(g_business_info_url,lexical_cast<string>(sellerid),"2",_ret.welcome_text);
    }
    if(customerid <= 0){
      _ret.welcome_text="商家客服离线请留言，或拨打明星衣橱官方客服电话：4006506468（每天9:00-22:00）";
      _ret.result=0;
    } else {
       _ret.result=1;
    }
    BLT_D<<"customerid:"<<customerid<<" welcome_text:"<<_ret.welcome_text<<" result="<<_ret.result;
    
    if(sku_id>0){
      string u;
      IBusiness ib;
      switch(type){
      case LOGIN_TYPE_SKU:
	{
	  goods_t goods;
	  ib.get_goods_detail_by_id(g_business_info_url,lexical_cast<string>(sellerid),lexical_cast<string>(sku_id),goods);
	  u=goods.goods_url;
      BLT_D<<"InsertUserSkuOrder sku:userid="<<userid<<",sellerid="<<sellerid<<",sku_id="<<sku_id<<",u="<<u;
	  InsertUserSkuOrder(userid,sku_id,-1,sellerid);
	}
	break;
      case LOGIN_TYPE_ORDER:
	{
	  order_t order;
	  ib.get_order_detail_by_id(g_business_info_url,lexical_cast<string>(sku_id),order);
	  u=order.order_url;
      BLT_D<<"InsertUserSkuOrder order:userid="<<userid<<",sellerid="<<sellerid<<",sku_id="<<sku_id<<",u="<<u;
	  InsertUserSkuOrder(userid,-1,sku_id,sellerid);
	}
	break;
      }

      if(customerid > 0){
          BLT_D<<"send MSG_TYPE_LOGIN_NOTIFY message userid="<<userid<<" business_id="<<sellerid<<" customerid="<<customerid;
          boost::uuids::uuid a_uuid = boost::uuids::random_generator()();
          SendMessage(lexical_cast<string>(a_uuid),"app",userid,sellerid,"",MSG_TYPE_LOGIN_NOTIFY, sellerid); // 对app来说sellerid即为business_id
      }
    }
    if(type==LOGIN_TYPE_SKU){
      goods_t gdt;
      business.get_goods_detail_by_id(g_business_info_url,lexical_cast<std::string>(sellerid),lexical_cast<std::string>(sku_id),gdt);
      GoodsDetail gd;
      gd.goods_id=gdt.goods_id;
      gd.goods_type=gdt.goods_type;
      gd.goods_url=gdt.goods_url;
      gd.goods_name=gdt.goods_name;
      gd.goods_price=gdt.goods_shop_price;
      gd.business_id=gdt.business_id;
      gd.goods_img=gdt.goods_img;
      _ret.gd=gd;
    }
  }


  int GetOfflineMessageNumberByBusinessId(const string& session_id, int business_id)
  {
    
    BLT_I<<"{\"method\":\""<<__func__<<"\", input:{\"session_id\":\""<<session_id<<"\", \"business_id\":"<<business_id<<"}}";
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    con->setSchema(g_mysql_database_name);

    sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = con->createStatement();
    stringstream ss;

    IBusiness business;
    service_t service;
    if(business.get_service_by_token(g_business_info_url, session_id, service)<0){
        BLT_E<<"business_id="<<business_id<<", session_id="<<session_id<<" 客服信息未找到.";
        return 0;
    }
    int customer_id = lexical_cast<int>(service.service_id);
    int chat_customer_type = trans_customer_privilege(lexical_cast<int>(service.service_type));

    if (customer_id <= 0){
        BLT_E<<"business_id="<<business_id<<", session_id="<<session_id<<", customer_id <= 0";
        return 0;
    }
    std::vector<business_t> vecBusiness;
    business.get_business(g_business_info_url, lexical_cast<std::string>(customer_id), vecBusiness);
    if(vecBusiness.size()==0){
        BLT_E<<"business_id="<<business_id<<", session_id="<<session_id<<" 客服没有关联的商家(customer_id="<<customer_id<<").";
        return 0;
    }

    int start=0;
    ss<<"select id from offline_message_log where sender_role=0 and business_id in (";
    for(std::vector<business_t>::iterator iterVecBusiness=vecBusiness.begin(); iterVecBusiness!=vecBusiness.end(); iterVecBusiness++){
        if(start++==0){
            ss<<iterVecBusiness->business_id;
        }else{
            ss<<","<<iterVecBusiness->business_id;
        } 
    }
    ss<<")";
    if(chat_customer_type==0||chat_customer_type==1){
        ss<<" and is_after_sale="<<chat_customer_type;
    }
    ss<<" limit 1";
    BLT_D<<" sql:"<<ss.str();
    res=stmt->executeQuery(ss.str());
    int count = 0;
    while(res->next()){
        count++;
    }
    BLT_I<<"customer_id="<<customer_id<<", session_id="<<session_id<<", business_id="<<business_id<<", privilege="<<display_customer_privilege(chat_customer_type)<<", 是否有离线消息:"<<(count>0?"是":"否");

    delete con;
    delete stmt;
    delete res;
    delete prep_stmt;
    return count>0?1:0;
    //return msgnumber;
  }

  void GetOfflineMessageWeb(vector<int> business_id_vec, vector<OfflineMessageWeb>& offline_message_vec, int sale_type=0){
    if (business_id_vec.size()==0){
        BLT_I<<"你要读取哪个商家的离线消息?";
        BLT_I<<"sale_type="<<sale_type;
        return;
    }
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    con->setSchema(g_mysql_database_name);

    sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = con->createStatement();

    stringstream ss;
    string business_id_vec_str = util_int_vec_to_string(business_id_vec);
    BLT_I<<"business_id_vec_str = "<<business_id_vec_str;
    ss<<"select id,sender,receiver,business_id,UNIX_TIMESTAMP(send_time),content,msg_type from offline_message_log where business_id in ("<<business_id_vec_str<<") and sender_role=0 ";
    ((sale_type==0)||(sale_type==1))?(ss<<" and is_after_sale="<<sale_type):false;
    ss<<" order by send_time desc";

    BLT_D<<" sql:"<<ss.str();
    res=stmt->executeQuery(ss.str());
    
    vector<int> id_vec;
    vector<int> image_id_vec;
    map<int, vector<Message>> message_map;
    while(res->next()){
      Message m;
      m.sourceid=res->getInt(2);
      int business_id = res->getInt(4);
      m.timestamp=res->getInt(5);
      m.type=res->getInt(7);
      m.text=res->getString(6);
      if(m.type==MSG_TYPE_IMAGE){
        image_id_vec.push_back(lexical_cast<int>(m.text));
      }
      message_map[business_id].push_back(m);
      id_vec.push_back(res->getInt(1));
      BLT_D<<"offline message:"<<m.sourceid<<","<<m.timestamp<<","<<m.text<<","<<m.type;
    }

    map<int, string> image_map;
    get_image_url_from_hichao_image_by_id_vec(image_map, image_id_vec);

    for(map<int, vector<Message>>::iterator message_map_iter = message_map.begin(); message_map_iter!= message_map.end(); message_map_iter++){
        for(vector<Message>::iterator message_vec_iter=message_map_iter->second.begin(); message_vec_iter!= message_map_iter->second.end(); message_vec_iter++){
            if(message_vec_iter->type==MSG_TYPE_IMAGE){
                int image_id = lexical_cast<int>(message_vec_iter->text);
                message_vec_iter->text = image_map[image_id];
            }
        }
        OfflineMessageWeb offline_message;
        BLT_I<<"business_id = "<<offline_message.business_id;
        offline_message.business_id = message_map_iter->first;
        offline_message.messages = message_map_iter->second;
        offline_message_vec.push_back(offline_message);
    }
    
    if(id_vec.size())
    {
      stringstream ss;
      string id_vec_str = util_int_vec_to_string(id_vec);
      ss<<"delete from offline_message_log where id in ("<<id_vec_str<<")";
      BLT_D<<ss.str();
      stmt->execute(ss.str());
    }

    delete con;
    delete stmt;
    delete res;
    delete prep_stmt;
  }

  void GetOfflineMessage(const string& sourcetype, int userid,int sellerid,vector<Message>& messages, int sale_type=0)
  {
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    con->setSchema(g_mysql_database_name);

    sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = con->createStatement();
    stringstream ss;
    if (sourcetype  ==  "app") {
        ss<<"select id,sender,receiver,business_id,UNIX_TIMESTAMP(send_time),content,msg_type from offline_message_log where business_id="<<sellerid<<" and receiver="<<userid<<" order by send_time desc";
    } else if(sourcetype  ==  "web") {
        ss<<"select id,sender,receiver,business_id,UNIX_TIMESTAMP(send_time),content,msg_type from offline_message_log where business_id="<<sellerid<< " and sender_role=0";
        // 消息类型区分售前和售后
        switch(sale_type) {
            case 0: // 售前
            case 1: // 售后
                ss<<" and is_after_sale="<<sale_type;
                break;
            case 2: // 售前 + 售后
                // ss<<" and (is_after_sale=0 or is_after_sale=1 or is_after_sale=2) "; // 售前+售后: 所有消息都可以
                break;
            default:
                break;
        }
        ss<<" order by send_time desc";
    } else {
        delete con;
        delete stmt;
        BLT_D<<"get offline message sourcetype error:"<<sourcetype;
        return;
    }
    BLT_D<<"GetOfflineMessage sql:"<<ss.str();
    res=stmt->executeQuery(ss.str());
    vector<int> ids;
    vector<string> imageids;
    while(res->next()){
      Message m;
      if(sourcetype=="app")
	m.sourceid=res->getInt(4);
      else if(sourcetype=="web")
	m.sourceid=res->getInt(2);
      m.timestamp=res->getInt(5);
      m.type=res->getInt(7);
      m.text=res->getString(6);
      if(m.type==MSG_TYPE_IMAGE){
      imageids.push_back(m.text);
      }
      BLT_D<<"offline message:"<<m.sourceid<<","<<m.timestamp<<","<<m.text<<","<<m.type;
      messages.push_back(m);
      ids.push_back(res->getInt(1));
    }

    std::vector<std::string> urls;
    urls  =  get_image_urls_from_hichao_image_by_ids(imageids);
    std::vector<Message>::iterator iter  =  messages.begin();
    int i = 0;
    if((urls.size()  != 0)  && (urls.size()  ==  imageids.size())){
      while (iter != messages.end()) {
          if(i == imageids.size()){
              break;
          }
       if(((*iter).type == MSG_TYPE_IMAGE) && ((*iter).text  == imageids[i])){
          (*iter).text  =  urls[i]; 
          i++;
       }
       iter++;
      }
    }
    //delete 
    if(ids.size())
    {
      stringstream ss;
      ss<<"delete from offline_message_log where id in (";
      for(int i=0;i<ids.size();i++){
	ss<<lexical_cast<string>(ids[i]);
	if(i!=ids.size()-1)
	  ss<<",";
      }
      ss<<")";
      BLT_D<<ss.str();
      stmt->execute(ss.str());
    }

    delete con;
    delete stmt;
    delete res;
    delete prep_stmt;
  }

  void Logout(const std::string& servername,int userid) {
    BLT_D<<"logout:userid="<<userid;
    connection::ptr_t c = g_pool->get();

    patterns::simple_obj_store<Servers::Model> sm_store;
    Servers::Model sm;
    try{
      sm=sm_store.find(c,servername);
    }catch(std::exception e){
      BLT_D<<"throw exception when access redis: "<<e.what();
    }
    if(!sm.loaded()){
      BLT_E<<"logout not found userid:userid="<<userid;
      g_pool->put(c);
      return;
    }
    bool is_customer=((sm.type()=="app")?false:true);
    if(!is_customer){

      Users::Model um=g_users.Get(userid,is_customer);
    
      patterns::simple_obj_store<UserToCustomer> utc_store;
      BLT_D<<"logout refresh utc time:userid="<<userid<<",sellerid:"<<um.sellerid();
      UserToCustomer utc=utc_store.find(c,UserToCustomer::getid(userid,um.sellerid()));
      //客服选举：客服引用计数递减
      int customerid = utc.customerid();
      /* 判断用户状态标志，是1:递减客服引用计数并且将用户状态维护标志清0; 是0:什么都不做. */
      int usf_result = customer_sched::usr_stat_flter::get(g_pool, userid);
      if(customerid > 0){
        if(usf_result == 1){
          customer_sched::decr(g_pool, customerid);
          customer_sched::usr_stat_flter::set(g_pool, userid, 0);
        }
      }
      /* 客服统计 用户退出*/
      StatClient* stat = new StatClient(g_mysql_connect_str, g_mysql_database_name, g_mysql_user, g_mysql_passwd);
      stat->op(STAT_CLIENT_OP_LOGOUT, STAT_CLIENT_ROLE_USER, userid, 0, 0,utc.sellerid());
      delete stat;
      BLT_D<<"logout refresh utc time:loaded:"<<um.loaded()<<"userid="<<userid<<",time"<<utc.logintime();
      UserToCustomer utc_new(utc.userid(),utc.sellerid(),utc.customerid(), utc.is_after_sale());
      BLT_D<<"logout refresh utc time:userid="<<userid<<",time"<<utc_new.logintime();
      utc_store.save(c,utc_new);
      BLT_D<<"logout refresh utc time:userid="<<userid<<",time"<<utc_new.logintime();
    }else{
        /* 客服统计 客服退出*/
      StatClient* stat = new StatClient(g_mysql_connect_str, g_mysql_database_name, g_mysql_user, g_mysql_passwd);
      stat->op(STAT_CLIENT_OP_LOGOUT, STAT_CLIENT_ROLE_CUSTOMER, userid, 0, 0, 0);
      delete stat;
    }

    g_users.Del(userid,is_customer,servername);

    
    g_pool->put(c);
  }

  void GetGoodsDetailByGoodsId(GoodsDetail& gd,int sellerid, int skuid) {
      goods_t gdt;
      IBusiness business;
      business.get_goods_detail_by_id(g_business_info_url,lexical_cast<std::string>(sellerid),lexical_cast<std::string>(skuid),gdt);
      gd.goods_id=gdt.goods_id;
      gd.goods_type=gdt.goods_type;
      gd.goods_url=gdt.goods_url;
      gd.goods_name=gdt.goods_name;
      gd.goods_price=gdt.goods_price;
      gd.goods_shop_price=gdt.goods_shop_price;
      gd.business_id=gdt.business_id;
      gd.goods_img=gdt.goods_img;
  }

  virtual void GetLastestOrdersByUserId(std::vector<OrderDetail> & _return, const int32_t userid, const int32_t number)
  {
    vector<order_t> orders;
    IBusiness business;
    business.get_lastest_order_detail_by_user_id(g_business_info_url,lexical_cast<string>(userid),lexical_cast<string>(number),orders);
    for(int i=0;i<orders.size();i++){
      OrderDetail od;
      od.order_id = orders[i].order_id;
      od.order_url=orders[i].order_url;
      od.order_add_time=orders[i].order_add_time;
      od.order_total_amount=orders[i].order_total_amount;
      od.order_status = orders[i].order_status;
      od.shipping_status  = orders[i].shipping_status;
      od.pay_status = orders[i].pay_status;
      od.address = orders[i].address;
      for(int j=0;j<orders[i].goodslist.size();j++){
          goodslist_detail_t gl_detail;
          gl_detail.url  = orders[i].goodslist[j].url;
          gl_detail.goods_image  = orders[i].goodslist[j].goods_image;
          gl_detail.goods_name  = orders[i].goodslist[j].goods_name;
          gl_detail.market_price = orders[i].goodslist[j].market_price;
          gl_detail.goods_number  = orders[i].goodslist[j].goods_number;
          gl_detail.goods_price  = orders[i].goodslist[j].goods_price;
          od.goodslist.push_back(gl_detail);
      }
    }
  }
#if 1
  void InsertUserSkuOrder(int userid,int skuid,int orderid,int sellerid) {
      sql::Driver *driver;
      sql::Connection *con;
      sql::Statement *stmt;
      
      //try{
        driver = get_driver_instance();
        con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
        con->setSchema(g_mysql_database_name);
        
        sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
        prep_stmt->executeQuery();
        sql::mysql::MySQL_Connection * mysql_conn  =  dynamic_cast<sql::mysql::MySQL_Connection*>(con);
        stmt = con->createStatement();
        stringstream ss;
        ss<<"replace into user_sku_order (userid,skuid,orderid,sellerid) values ("<<userid<<","<<skuid<<","<<orderid<<","<<sellerid<<")";
        
        BLT_D<<ss.str();
        stmt->execute(ss.str());
        delete con;
        delete stmt;
        delete prep_stmt;
      //}
      //catch(Exception e){
      //  BOOST_LOG_TRIVIAL(warning)<<"InsertUserSkuOrder mysql connector is error:"<<e.what();
      //}
  }
#endif
  void GetUserSkuOrderByUserId(UserSkuOrder& uso, const int32_t userid, const int32_t sellerid){
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    driver = get_driver_instance();
    con = driver->connect(g_mysql_connect_str, g_mysql_user, g_mysql_passwd);
    con->setSchema(g_mysql_database_name);

    sql::PreparedStatement *prep_stmt = con->prepareStatement("set names utf8mb4");
    prep_stmt->executeQuery();
    stmt = con->createStatement();
    stringstream ss;
    ss<<"select userid,skuid,orderid from user_sku_order where userid="<<userid<<" and sellerid="<<sellerid;
    BLT_D<<"GetUserSkuOrderByUserId sql:"<<ss.str();
    res=stmt->executeQuery(ss.str());
    while(res->next()){
        uso.user_id = res->getString(1);
        uso.sku_id = res->getString(2);
        uso.order_id = res->getString(3);
        uso.seller_id = lexical_cast<string>(sellerid);
    }

    delete con;
    delete stmt;
    delete res;
    delete prep_stmt;
  }

  void SendMessage(const string& id,const string& sourcetype,int sourceid,int targetid, const std::string& text,int type, int business_id) {
    send_message(id,sourcetype,business_id,sourceid,targetid,text,type);
  }

  void SendFile(int ,int, const std::string& filename) {
    printf("SendFile\n");
  }

  void AddGroupUser() {
    printf("AddGroupUser\n");
  }

  void RemoveGroupUser() {
    printf("RemoveGroupUser\n");
  }

};

  
int main(int argc, char **argv) {
  activemq::library::ActiveMQCPP::initializeLibrary();

  log_init("chat_server");
  BLT_I << "boost log begin";

  for(int i=0;i<1000;i++){
    g_userids_tests.push_back(lexical_cast<string>(i));
  }
  
  ptree pt;
  read_ini(argv[1],pt);
  
  g_server_port=pt.get<std::string>("server.port");

  g_customer_service_ip=pt.get<std::string>("customer.ip");
  g_customer_service_port=pt.get<std::string>("customer.port");

  g_hichao_image_service_url=pt.get<std::string>("hichaoimage.url");

  g_throne_service_ip=pt.get<std::string>("throne.ip");
  g_throne_service_port=pt.get<std::string>("throne.port");

  g_redis_ip=pt.get<std::string>("redis.ip");
  g_redis_port=pt.get<std::string>("redis.port");

  g_pool = simple_pool::create(g_redis_ip,lexical_cast<int>(g_redis_port));

  g_customer_message_service_ip  =  pt.get<std::string>("customermessage.ip");
  g_customer_message_service_port  =  pt.get<std::string>("customermessage.port");

  g_mysql_connect_str=pt.get<std::string>("mysql.connection_str");
  g_mysql_user=pt.get<std::string>("mysql.user");
  g_mysql_passwd=pt.get<std::string>("mysql.passwd");
  g_mysql_database_name=pt.get<std::string>("mysql.database");

  g_mysql_upload_connect_str=pt.get<std::string>("mysql.upload_connection_str");
  g_mysql_upload_user=pt.get<std::string>("mysql.upload_user");
  g_mysql_upload_passwd=pt.get<std::string>("mysql.upload_passwd");
  g_mysql_upload_database_name=pt.get<std::string>("mysql.upload_database");

  g_activemq_url=pt.get<std::string>("activemq.url");
  g_activemq_username=pt.get<std::string>("activemq.username");
  g_activemq_password=pt.get<std::string>("activemq.password");
  g_activemq_topic=pt.get<std::string>("activemq.topic");
  
  g_business_info_url=pt.get<std::string>("business_info.url");

  g_push_url=pt.get<std::string>("push.url");

  g_pthrone_client_pool=new ThriftPool<throne::ThroneClient,TBufferedTransport>(1,g_throne_service_ip, lexical_cast<int>(g_throne_service_port),4000);


  for(int i=0;i<5;i++){
    thread* t=new thread(send_message_thread);
  }

  /*
    boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  boost::shared_ptr<ChatServiceHandler> handler(new ChatServiceHandler());
  boost::shared_ptr<TProcessor> processor(new ChatServiceProcessor(handler));
  boost::shared_ptr<TServerTransport> serverTransport(new TServerSocket(lexical_cast<int>(g_server_port)));
  boost::shared_ptr<TTransportFactory> transportFactory(new TFramedTransportFactory());

  boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(256);
  boost::shared_ptr<PosixThreadFactory> threadFactory =boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();
  TThreadPoolServer server(processor,
                           serverTransport,
                           transportFactory,
                           protocolFactory,
                           threadManager);

  printf("Starting the server...\n");
  server.serve();
  */

  boost::shared_ptr<ChatServiceHandler> handler(new ChatServiceHandler());
  boost::shared_ptr<TProcessor> processor(new ChatServiceProcessor(handler));
  boost::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());
  boost::shared_ptr<ThreadManager> threadManager = ThreadManager::newSimpleThreadManager(64);
  boost::shared_ptr<PosixThreadFactory> threadFactory = boost::shared_ptr<PosixThreadFactory>(new PosixThreadFactory());
  threadManager->threadFactory(threadFactory);
  threadManager->start();
  TNonblockingServer server(processor, protocolFactory, lexical_cast<int>(g_server_port), threadManager);
  server.serve();
  return 0;
}

