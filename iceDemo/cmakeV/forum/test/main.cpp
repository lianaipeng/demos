
#include "common.h"
#include "Forum.h"

using namespace Forum;

int main(int argc,char **argv){
    int status = 0;
    Ice::CommunicatorPtr ic; 
    try {
        ic = Ice::initialize(argc, argv);
        //Ice::ObjectPrx base = ic->stringToProxy("SimpleForum:default -p 10001");
        Ice::ObjectPrx base = ic->stringToProxy("ForumObjectId@ForumAdapter");
        //Ice::ObjectPrx base = ic->stringToProxy("SimpleForum");
        ForumInterfacePrx forum = ForumInterfacePrx::checkedCast(base);
        if (!forum)
            throw "Invalid proxy";

        UserInfo user_info;
        forum->sayHello("Hello World!");
        forum->getUserInfo(10,user_info);
        std::cout << "id:" << user_info.id << ",name:" << user_info.name << std::endl;

    } catch (const Ice::Exception& ex) {
        cerr << ex << endl;
        status = 1;
    } catch (const char* msg) {
        cerr << msg << endl;
        status = 1;
    }   
    if (ic)
        ic->destroy();
    return status;
}
