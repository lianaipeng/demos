

#include <iostream>
#include <json/json.h>

int main(int argc,char **argv){
    Json::Value rootObj; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array 
    
    std::cout << "################# Write" << std::endl;
    //Json::Value stringObj;   // 构建对象  
    rootObj["string"] = "string";
    rootObj["code"]    = 10;
    
    Json::Value arrayObj;   // 构建对象  
    Json::Value item1, item2;  
    item1["item1"] = "item1";  
    item2["item2"] = "item2"; 
  //  arrayObj[0] = item1;
  //  arrayObj[1] = item2;
     arrayObj.append(item1);  // 插入数组成员  
     arrayObj.append(item2);
    /*
    arrayObj.append("item1");
    arrayObj.append("item2");
    */
    rootObj["array"] = arrayObj;

    // 输出无格式json字符串  
    Json::FastWriter writer;  
    std::string strWrite = writer.write(rootObj);
    std::cout << "rootObj:"  << strWrite << std::endl; 
    
    std::cout << "################# Read" << std::endl; 
    // 访问节点，Return the member named key if it exist, defaultValue otherwise. 
    std::cout << "rootObj.get(\"string\",\"null\"):" << rootObj.get("string","null").asString() << std::endl;   
    
    Json::Reader reader;  
    Json::Value root;  
    if (reader.parse(strWrite, root)){  // reader将Json字符串解析到root，root将包含Json里所有子元素  
        std::string string = root["string"].asString();  // 访问节点，upload_id = "UP000000"  
        int code = root["code"].asInt();    // 访问节点，code = 100 
        std::cout << "string:" << string << ",code:" << code << std::endl;
        
        int itemSize = root["array"].size(); 
        std::cout << "itemSize:" << itemSize << std::endl;
        for(int i=0 ; i < itemSize ; i++){
            std::cout << "root[\"array\"][i]:" << root["array"][i].asString() << std::endl;
        }
    }   
	return 0;
}
