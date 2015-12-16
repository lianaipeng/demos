#ifndef __TOOL_UTIL__
#define __TOOL_UTIL__

//#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

int splitStr(const std::string& str,std::vector<std::string> &strVec,const std::string &seper);

int startWith(const std::string& str,const std::string &seper);
int endWith(const std::string& str,const std::string &seper);


#endif
