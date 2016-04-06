#include <iostream>
#include <string.h>
#include <cstring>
#include <vector>
using namespace std;

//template <typename T> 
int stringToVector(const string& str,vector<string>& vec){
    if(str.empty())
        return -1;
    
    char* tempStr = (char *)str.c_str();
    char *tokenPtr = strtok( tempStr, "," );
    int ret = 0;
    while( tokenPtr!=NULL ){
        vec.push_back(string(tokenPtr));      
        tokenPtr = strtok(NULL,",");
        ret++;
    }
    return ret;
}


int main()
{
    string temp = "12,13,144";
    vector<string> vec;
    int ret = stringToVector(temp,vec);
    for(int i=0; i<vec.size() ;i++){
        cout << vec.at(i) << endl;
    }
    cout << "ret:" << ret << endl;
    
    return 0;
}
