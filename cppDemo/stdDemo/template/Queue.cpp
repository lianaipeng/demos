
/*
 * 类模板成员函数
 * 1.必须以关键字template开头，后接类的模板形参表。
 * 2.必须指出他是那个类的成员。
 * 3.类名必须包含其模板形参。
 * 在类外定义的Queue类的成员函数的开头应该是：
 * template <class Type> ret-type Queue<Type>::member-name
 * 在作用域操作符(::)之前使用的Queue<Type>指定成员函数所属的类
 */

#include "Queue.h"

// 模板类
template <class Type> void Queue<Type>::destroy()
{
    while(!empty()){
        pop();
    }
}
template <class Type> void Queue<Type>::pop()
{
    QueueItem<Type>* p = head;
    head = head->next;
    delete p;
}
template <class Type> void Queue<Type>::push(const Type& val)
{
    // allocate a new QueueItem object
    QueueItem<Type> *pt = new QueueItem<Type>(val);
    // put item onto existing queue
    if( empty() ){
        head = tail = pt; 
    }else{
        tail->next = pt;
        tail = pt;
    }
}
template <class Type> void Queue<Type>::copy_elems(const Queue& orig)
{
    for(QueueItem<Type> *pt=orig.head; pt; pt=pt->next){
        push(pt->item);
    }
}


// 模板输出函数
template <class T> std::ostream& operator<< (std::ostream& os, const Queue<T>& q){
    os << "<# ";
    QueueItem<T> *p;
    for(p=q.head ; p ; p = p->next)
        os << p->item << " ";
    os << "#>";
    return os;
}


// 模板函数 
template <class Type> int compare2(const Type& a,const Type& b){
    int ret = -1;
    if(a > b)
        ret = 1;
    else if(a == b)
        ret = 0;
    return ret;
}

