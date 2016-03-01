#include <Ice/Ice.h>
#include <Printer.h>

using namespace std;
using namespace Demo;

class PrinterI : public Printer{
public:
    virtual void printString(const string&s ,const Ice::Current&);
    virtual int getInt(const string &s,const Ice::Current &);
    virtual int getNumber(const MStruct &s,const Ice::Current &);
};

void PrinterI::printString(const string& s,const Ice::Current &){
    cout << s << endl;
}

int PrinterI::getInt(const string &s,const Ice::Current &){
    return 10;
}
int PrinterI::getNumber(const MStruct &s,const Ice::Current &){
    return s.number;
}

int main(int argc,char *argv[]){
    int status = 0;
    Ice::CommunicatorPtr ic;
    try{
        ic = Ice::initialize(argc, argv);
        Ice::ObjectAdapterPtr adapter = ic->createObjectAdapterWithEndpoints("SimplePrinterAdapter", "default -p 10001");
        Ice::ObjectPtr object = new PrinterI;

        adapter->add(object,ic->stringToIdentity("SimplePrinter"));
        adapter->activate();
        ic->waitForShutdown();
    } catch (const Ice::Exception& e) {
        cerr << e << endl;
        status = 1;
    } catch (const char *msg) {
        cerr << msg << endl;
        status = 1;
    }

    if(ic){
        try{
            ic->destroy();
        }catch (const Ice::Exception& e) {
            cerr << e << endl;
            status = 1;
        }
    }

    return status;
}
