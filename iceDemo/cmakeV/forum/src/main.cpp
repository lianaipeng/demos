
#include "common.h"
#include "ForumI.h"

int main(int argc ,char* argv[]){
    int status = 0;
    Ice::CommunicatorPtr ic;
    try{
        ic = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("SimpleForumAdapter", "default -p 10001");
        Ice::ObjectPtr object = new ForumI;

        adapter->add(object,ic->stringToIdentity("SimpleForum"));
        adapter->activate();
        ic->waitForShutdown();
    } catch(const Ice::Exception& e) {
        cerr << e << endl;
        status = 1;
    } catch(const char *msg) {
        cerr << msg << endl;
        status = 1;
    }
    
    cout << "hello" << endl;

    if( ic ){
        try {
            ic->destroy();
        } catch (const Ice::Exception& e) {
            cerr << e << endl;
            status = 1;
        }   
    }   
    
    return status;
}
