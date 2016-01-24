#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <iostream>

// 模板类
template <class Type> class Queue;
// 模板输出函数
template <class T> std::ostream& operator<< (std::ostream&, const Queue<T>&);
// 模板函数
template <class Type> int compare2(const Type& ,const Type&);


template <class Type> class QueueItem{
    friend class Queue<Type>;
    friend std::ostream& operator<< <Type> (std::ostream&,const Queue<Type>&);
    friend int compare2 <Type> (const Type&,const Type&);
    public:
        // private class: no public sectioin 
        QueueItem(const Type& t):item(t),next(0){}
    private:
        Type item;      //value stored in this element 
        QueueItem* next; // pointer to next element in the Queue 
};

template <class Type> class Queue{
    friend std::ostream& operator<< <Type> (std::ostream&,const Queue<Type>&);
    public:
        // empty Queue
        Queue():head(0),tail(0){}
        Queue(const Queue& Q):head(0),tail(0){
            copy_emems(Q);
        }
        Queue& operator = (const Queue&);
        ~Queue(){
            destroy();
        }
        // return element from head of Queue
        // unchecked operation: front on an empty Queue is undefined 
        Type& front(){
            return head->item;
        }
        const Type& front() const {
            return head->item;
        }
        // add element to back of Queue
        void push(const Type&);
        // remove element from head of Queue
        void pop();
        // true if no elements in the Queue
        bool empty() const {
            return head == 0;
        }
        
    private:
        QueueItem<Type>* head;
        QueueItem<Type>* tail;
        // delete all the elements 
        void destroy();
        void copy_elems(const Queue&); 
};

#endif
