#include <Ice/Ice.h>
#include <Printer.h>
 
 using namespace std;
 using namespace Demo;

int main(int argc, char* argv[])
{
    int status = 0;
    Ice::CommunicatorPtr ic;
    try {
        ic = Ice::initialize(argc, argv);
        Ice::ObjectPrx base = ic->stringToProxy("SimplePrinter:default -p 10001");
        PrinterPrx printer = PrinterPrx::checkedCast(base);
        if (!printer)
            throw "Invalid proxy";

        printer->printString("Hello World!");

        int temp = printer->getInt("");
        cout << temp << endl;

        MStruct stru;
        stru.number = 101;
        stru.name = "myame";
        temp = printer->getNumber(stru);
        cout << temp << endl;

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
