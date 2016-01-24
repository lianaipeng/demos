#include <iostream>
#include "Queue.h"
/* 
 * 模板类不支持分离实现，你把声明和实现文件放一起吧
 * 或是使用export关键字，但需要编译器支持这个关键字(不会用)
 * 或者在用到这个类的时候把.cpp文件也include进来
 * */
#include "Queue.cpp"

using namespace std;


// 模板函数 
template <class Type> int compare1(const Type& a,const Type& b){
    int ret = -1;
    if(a > b)
        ret = 1;
    else if(a == b)
        ret = 0;
    return ret;
}

int main(){ 
    // ########## 模板函数
    short f1 = 20,f2 = 21;
    std::cout << "f1:" << f1 << " compare f2:" << f2 << " ret:"<< compare1(f1,f2) << std::endl; 
    string as = "hello",bs = "hello";
    std::cout << "as:" << as << " compare bs:" << bs << " ret:"<< compare1(as,bs) << std::endl; 
     
    // ########## 模板类
    Queue<int> qi;
    short s1 = 22;
    short s2 = 21;
    qi.push(s1);
    qi.push(s2);
    
    for(int i=0; i < 5; i++ ){
        qi.push(30+i);
    }
    // 标准输出重载
    std::cout << qi << std::endl;
    
    // ########## 模板友元函数   
    std::cout << "s1:" << s1 << " compare s2:" << s2 << " ret:"<< compare2(s1,s2) << std::endl; 
        
    return 0;
}
