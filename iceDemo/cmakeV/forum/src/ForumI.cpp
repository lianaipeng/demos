
#include "ForumI.h"

void ForumI::sayHello(const string& str,const Ice::Current&){
    cout << "######### " << str << endl;    
}
void ForumI::getUserInfo(int id,UserInfo& user_info,const Ice::Current&){
    cout << "######### " << id << endl;    
    
    user_info.id = id;
    user_info.name = "name";
}
