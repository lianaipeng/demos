#ifndef __USERS__
#define __USERS__

#include "common.h"



class Users
{
public:
  class Model:public redis3m::patterns::model
  {
  public:
    static string getid(int userid,bool is_customer){
      return lexical_cast<string>(userid)+"_"+lexical_cast<string>(is_customer);
    }
  public:
    Model(){}
    Model(const std::string& id, const std::map<std::string, std::string>& mss)
      :model(id,mss),
      _is_customer(read_bool_from_map(mss,"is_customer")),
      _servername(read_str_from_map(mss,"servername")),
      _state(read_int_from_map(mss,"state",-1)),
      _userid(read_int_from_map(mss,"userid",-1)),
      _sellerid(read_int_from_map(mss,"sellerid",-1)),
      _is_after_sale(read_int_from_map(mss,"is_after_sale",0)),
      _is_pre_sale(read_int_from_map(mss,"is_pre_sale",0))
	{
	}
  Model(int userid,bool is_customer,int state,const string& servername,int sellerid, int is_pre_sale, int is_after_sale)
    : _userid(userid),
      _is_customer(is_customer),
      _state(state),
      _servername(servername),
      _sellerid(sellerid),
      _is_pre_sale(is_pre_sale),
      _is_after_sale(is_after_sale)
      {
	_id=Users::Model::getid(userid,is_customer);
	_loaded=true;
      }
    

    std::map<std::string, std::string> to_map() const
    {
      //return boost::assign::map_list_of("userid",lexical_cast<string>(_userid))("is_customer",lexical_cast<string>(_is_customer))("state",lexical_cast<string>(_state))("servername",_servername)("sellerid",lexical_cast<string>(_sellerid))("is_after_sale",lexical_cast<int>(_is_after_sale))("is_pre_sale",lexical_cast<int>(_is_pre_sale));
      return boost::assign::map_list_of("userid",lexical_cast<string>(_userid))("is_customer",lexical_cast<string>(_is_customer))("state",lexical_cast<string>(_state))("servername",_servername)("sellerid",lexical_cast<string>(_sellerid))("is_pre_sale",lexical_cast<string>(_is_pre_sale))("is_after_sale",lexical_cast<string>(_is_after_sale));
    }
    static std::string model_name()
    {
      return "user_info";
    }

    REDIS3M_MODEL_RO_ATTRIBUTE(bool, is_customer)
    REDIS3M_MODEL_RO_ATTRIBUTE(int, userid)
    REDIS3M_MODEL_RO_ATTRIBUTE(int, state)
    REDIS3M_MODEL_RO_ATTRIBUTE(std::string, servername)
    REDIS3M_MODEL_RO_ATTRIBUTE(int, sellerid)
    REDIS3M_MODEL_RO_ATTRIBUTE(int, is_after_sale)
    REDIS3M_MODEL_RO_ATTRIBUTE(int, is_pre_sale)

  };
public:
  boost::mutex m_mutex;
public:
  bool Add(int userid,bool is_customer,const string& servername,int sellerid, int is_pre_sale, int is_after_sale);
  bool Del(int userid,bool is_customer,const string& servername);
  void Save(Users::Model& model);
  void InsertToServer(const string& servername, int userid, bool is_customer);
  void DelFromServer(const string& servername, int userid, bool is_customer);
  void MultiDel(connection::ptr_t c,std::vector<reply> suserids,std::string& setname);
  void ClearInvalidUsers(const string& servername);
  Users::Model Get(int userid,bool is_customer);
};

extern Users g_users;



#endif//__USERS__
