#ifndef __TIMER_H__
#define __TIMER_H__

#include <sys/time.h>
#include <string>
#include <iostream>

#ifdef TIME_TEST
#define TIMER_BEGIN \
        struct timeval tb; \
        gettimeofday(&tb, NULL);

#define TIMER_END \
        struct timeval te; \
        gettimeofday(&te, NULL); \
        cout << "### sec:" << (te.tv_sec-tb.tv_sec)*1000000 << ",usec:" << (te.tv_usec-tb.tv_usec)/1000 << endl;
#endif
        //cout << "### sec:" << (te.tv_sec-tb.tv_sec) << ",usec:" << (te.tv_usec-tb.tv_usec) << endl;\


#define HELLO 10
/*
using namespace std;
int main(){
    struct timeval tb;
    gettimeofday(&tb, NULL);


    cout << "#########:sec" << tb.tv_sec << ",usec:" << tb.tv_usec << endl;
    return 0;
}
*/

#endif 
