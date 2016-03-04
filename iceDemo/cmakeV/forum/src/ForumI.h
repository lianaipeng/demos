
#include "common.h"
#include "Forum.h"

using namespace Forum;
class ForumI : public Forum::ForumInterface{
    public:
        virtual void sayHello(const string& str,const Ice::Current&);
        virtual void getUserInfo(int id,UserInfo& user_info,const Ice::Current&);
};
