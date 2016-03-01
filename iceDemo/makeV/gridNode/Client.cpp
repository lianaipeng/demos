#include <Ice/Ice.h>
#include <Printer.h>

using namespace std;
using namespace Demo;


int main(int argc,char **argv){
    int status = 0;
    Ice::CommunicatorPtr ic;
    
    try{
        ic = Ice::initialize(argc,argv);
        //Ice::ObjectPrx base = ic->stringToProxy("Printer:default -p 10001");
        //Ice::ObjectPrx base = ic->stringToProxy("Printer@ServerId.AdapterId");
        Ice::ObjectPrx base = ic->stringToProxy("Printer@Adapter");
        PrinterPrx printer = PrinterPrx::checkedCast(base);
        if(!printer)
            throw "Invalid proxy";

        printer->printString("nidaye");
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
