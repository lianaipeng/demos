#include <string>
#include <set>
#include <iostream>
using namespace std;

struct P
{
    int rp;
    std::string name;
};

struct compare
{
    bool operator () (P p1,P p2){
        if(p1.name < p2.name){
            return true;
            //return p1.rp < p2.rp;
        } else {
            return false;
            //return p1.rp < p2.rp;
        }
    }
};

int main()
{
    multiset<P,compare> multiSet;
    for(int i=0; i< 5; i++){
        P p;
        p.rp = i;
        p.name = "1111";
        multiSet.insert(p);
    }
    for(int i=4; i>=0; i--){
        P p;
        p.rp = i;
        p.name = "2222";
        multiSet.insert(p);
    }
    multiset<P,compare>::iterator iter= multiSet.begin();
    while(iter!=multiSet.end())
    {
        std::cout <<  "name:" << iter->name << ",rp:" <<iter->rp <<std::endl;
        iter++;
    }

    return 0;
}
