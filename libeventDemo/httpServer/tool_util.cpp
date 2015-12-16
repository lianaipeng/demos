
#include "tool_util.h"

int splitStr(const std::string& str,std::vector<std::string> &strVec,const std::string &seper)
{
    std::string::size_type start = 0,end =0;
    std::string::size_type len = seper.size();
    while( (end = str.find(seper,start)) != std::string::npos )
    {
        std::string strSlice = str.substr(start,end-start);
        if(!strSlice.empty())
            strVec.push_back(strSlice);
        end += len;
        start = end;
    }
    // 尾部处理
    std::string strTail = str.substr(start,str.size()-start);
    if(!strTail.empty())
        strVec.push_back(strTail);

    return strVec.size();
}

int startWith(const std::string& str,const std::string &seper)
{
    return str.compare(0,seper.size(),seper);
}
int endWith(const std::string& str,const std::string &seper)
{
    return str.compare(str.size()-seper.size(),seper.size(),seper);
}
