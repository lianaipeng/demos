
#include "common.h"
#include "Forum.h"

class ForumI : public Forum::ForumInterface{
    public:
        virtual void sayHello(const string& str,const Ice::Current&);
};
