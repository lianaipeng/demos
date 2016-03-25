
#include "timer.h"
//#include <sys/time.h>
//#include <string>
#include <iostream>

using namespace std;
int main(){
    
    struct timeval tbv;
    gettimeofday(&tbv, NULL);
    cout << "#########sec:" << tbv.tv_sec << ",usec:" << tbv.tv_usec << endl;
    
     
    TIMER_BEGIN;
    cout << HELLO << endl;
    TIMER_END;

    return 0;
}
