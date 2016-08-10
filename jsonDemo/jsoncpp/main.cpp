

#include <iostream>
#include <json/json.h>
#include <vector>

int main(int argc,char **argv){
    Json::Value rootObj; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array 
    
    std::cout << "################# Write" << std::endl;
    //Json::Value stringObj;   // 构建对象  
    rootObj["string"] = "string";
    rootObj["code"]    = 10;
    
    Json::Value arrayObj;   // 构建对象  
    /*
    Json::Value item1, item2;  
    item1["item1"] = "item1";  
    item2["item2"] = "item2"; 
  //  arrayObj[0] = item1;
  //  arrayObj[1] = item2;
     arrayObj.append(item1);  // 插入数组成员  
     arrayObj.append(item2);
     */
    /*
    arrayObj.append("item1");
    arrayObj.append("item2");
    */
    arrayObj.append(10);
    arrayObj.append(11);
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




// 不同的数据类型
    //strWrite = "{\"category_id\":104,\"content\":\"嘉宾\",\"has_img\":1,\"hot_display\":1,\"id\":129975,\"img_id\":0,\"img_lock\":1,\"ip\":\"61.148.244.234\",\"is_display\":1,\"is_favor\":0,\"is_forbidden\":0,\"is_top\":0,\"main_thread_id\":0,\"platform\":0,\"recommend\":0,\"review\":0,\"sku_num\":0,\"tags\":\"88\",\"title\":\"非常完美\",\"user_id\":2758285,\"img_list\":[1,2,3]}";
    strWrite = "{\"CNY_USD\":0.15677,\"USD_CNY\":6.3788,\"CNY_KRW\":174.85574,\"KRW_CNY\":0.00572,\"USD_KRW\":1115.36982,\"KRW_USD\":0.0009,\"CNY_GBP\":0.10267,\"GBP_CN    Y\":9.7401,\"USD_GBP\":0.6549,\"GBP_USD\":1.52695,\"CNY_HKD\":1.21507,\"HKD_CNY\":0.823,\"USD_HKD\":7.75067,\"HKD_USD\":0.12902,\"CNY_JPY\":19.24446,\"JPY_CNY\":0.05196,\"USD_JPY\":122.75658,\"JPY_USD\":0.00815,\"CNY_AUD\":0.21893,\"AUD_CNY\":4.5676,\"USD_AUD\":1.39653,\"AUD_USD\":0.71606,\"CNY_EUR\":0.1453,\"EUR_CNY\":6.8822,\"USD_EUR\":0.92685,\"EUR_USD\":1.07892}";
    Json::Value requestObj;
    if(reader.parse(strWrite,requestObj)){
        Json::Value tempObj;
        tempObj = requestObj["CNY_USD"];
        if( !tempObj.isNull() ){
            std::cout << "content:" << tempObj.asDouble() << std::endl;           
        }else{
            std::cout << "content not exist" << std::endl;
        }
        tempObj = requestObj["CNY_USDd"];
        if( !tempObj.isNull() ){
            std::cout << "content:" << tempObj.asDouble() << std::endl;           
        }else{
            std::cout << "content not exist" << std::endl;
        }
        /*
        if(!requestObj["CNY_USD"].isNull() )
            std::cout << "content:" << requestObj["CNY_USD"].asDouble() << std::endl;
        */
    }



// json与容器
    std::vector<Json::Value> json_vec;
    for(int i=0; i<5; i++){
        Json::Value tempObj;
        tempObj["id"] = i;
        tempObj["name"] = i;
        json_vec.push_back(tempObj);
    }
   
    Json::Value vecObj;
    for(auto item : json_vec){
        if(item.isObject()){
            std::cout << "item is object" << endl;
        }

        if(item.isMember("id")){
            std::cout << "has member" << std::endl;
        }
        if(item.isMember("idss")){
            std::cout << "no has member" << std::endl;
        }
        vecObj.append(item);
    }
    std::string vec_str = writer.write(vecObj);
    std::cout << "### VEC ###:" << vec_str << std::endl;

	return 0;
}
