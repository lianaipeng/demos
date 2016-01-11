#include "users.h"

extern simple_pool::ptr_t g_pool;

bool Users::Add(int userid,bool is_customer,const string& servername,int sellerid, int is_pre_sale, int is_after_sale){
//  boost::lock_guard<boost::mutex> lock(m_mutex);
  
  connection::ptr_t c = g_pool->get();
  patterns::simple_obj_store<Users::Model> store;
  
  Users::Model um(userid,is_customer,USER_ONLINE,servername,sellerid, is_pre_sale, is_after_sale);
  store.save(c,um);
  InsertToServer(servername,userid,is_customer);
  if(is_customer){//设置客服在线超时
    std::string expire = "10";
    std::string suserid = "user_info:"+boost::lexical_cast<std::string>(userid)+"_1";
    BLT_I<<"2222222222222222 suserid="<<suserid; 
    c->run(command("expire")(suserid)(expire)); 
  }
  g_pool->put(c);
  BOOST_LOG_TRIVIAL(trace)<<"user add:userid="<<um.userid();
  return true;
}
void Users::InsertToServer(const string& servername, int userid, bool is_customer){
//  boost::lock_guard<boost::mutex> lock(m_mutex);
  
  connection::ptr_t c = g_pool->get();
  string skey  =   servername  + "_set_" + servername;
  string suserid  =  Users::Model::getid(userid, is_customer);
  c->run(command("sadd")(skey)(suserid));
  g_pool->put(c);
}

void Users::DelFromServer(const string& servername, int userid, bool is_customer){
//  boost::lock_guard<boost::mutex> lock(m_mutex);
  
  connection::ptr_t c = g_pool->get();
  string skey  =   servername + "_set_" + servername;
  string suserid  =  Users::Model::getid(userid, is_customer);
  c->run(command("srem")(skey)(suserid));
  g_pool->put(c);
}
void Users::MultiDel(connection::ptr_t c,std::vector<reply> suserids,std::string& setname){
//  boost::lock_guard<boost::mutex> lock(m_mutex);
  size_t len  =  suserids.size(); 
  int count  =  0;

  command cmd  =  command("del");
  command setcmd  =  command("srem")(setname);
  for(int i = 0; i < len; i++){
      reply r  =  suserids[i];
      std::string tmpstr  =  Users::Model::model_name() + ":" + r.str();
      cmd  << tmpstr;
      setcmd  <<  r.str();
      if(count  == 100){
          c->run(cmd);
          c->run(setcmd);
          count  =  0;
      }
      count++;
  }
  if(count > 0){
      c->run(cmd);
      c->run(setcmd);
  }
}
void Users::ClearInvalidUsers(const string& servername) {
//  boost::lock_guard<boost::mutex> lock(m_mutex);
  
  connection::ptr_t c = g_pool->get();
  string skey  =   servername + "_set_" + servername;
  reply r  =  c->run(command("smembers")(skey));
  std::vector<reply> members  =  r.elements();
  BOOST_LOG_TRIVIAL(trace)<<"servername = " << servername  << ",set size="<<members.size();
  if(members.size()){
      MultiDel(c,members,skey);
  }
  g_pool->put(c);
}
void Users::Save(Users::Model& model){
//  boost::lock_guard<boost::mutex> lock(m_mutex);
  connection::ptr_t c = g_pool->get();
  patterns::simple_obj_store<Users::Model> store;
  store.save(c,model);
  g_pool->put(c);
}

bool Users::Del(int userid,bool is_customer,const string& servername){
//  boost::lock_guard<boost::mutex> lock(m_mutex);
  connection::ptr_t c = g_pool->get();
  patterns::simple_obj_store<Users::Model> store;
  
  Users::Model um;
  try{
    um=store.find(c,Users::Model::getid(userid,is_customer));
  }
  catch(...){
    BLT_E<<"redis error";
  }
  if(um.loaded()) {
    store.remove(c,um);
    DelFromServer(servername,userid,is_customer);
  }
  g_pool->put(c);
}

Users::Model Users::Get(int userid,bool is_customer){
//  boost::lock_guard<boost::mutex> lock(m_mutex);
  
  connection::ptr_t c = g_pool->get();
  patterns::simple_obj_store<Users::Model> store;
  Users::Model mdl;
  try{
    mdl= 
     store.find(c,Users::Model::getid(userid,is_customer));
  }
  catch(...){
    BLT_E<<"redis error";
  }
  
  g_pool->put(c);
  return mdl;
  //return store.find(c,Users::Model::getid(userid,is_customer));
}

Users g_users;
