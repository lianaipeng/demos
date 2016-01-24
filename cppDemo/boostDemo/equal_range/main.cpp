 #include <iostream>
 #include <map>
using namespace std;

int main(){
    map<char,int> mymap;

    mymap['a']=10;
    mymap['b']=20;
    mymap['c']=30;
    mymap['d']=40;
    mymap['e']=50;

    pair<map<char,int>::iterator,map<char,int>::iterator> ret;
    ret = mymap.equal_range('c');

    cout << "lower bound points to: ";
    cout << ret.first->first << " => " << ret.first->second << endl;

    cout << "upper bound points to: ";
    cout << ret.second->first << " => " << ret.second->second << endl;

    return 0;
}
