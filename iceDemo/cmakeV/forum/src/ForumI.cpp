
#include "ForumI.h"

void ForumI::sayHello(const string& str,const Ice::Current&){
    cout << "######### " << str << endl;    
}
