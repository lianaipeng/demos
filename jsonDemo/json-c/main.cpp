

#include <stdio.h>
#include <json-c/json.h>

int main(int argc,char **argv){

	/*
	struct json_object *newObj;
	int pageCount;
	newObj = json_tokener_parse("{ \"glossary\": { \"title\": \"example glossary\", \"pageCount\": 100, \"GlossDiv\": { \"title\": \"S\", \"GlossList\": [ { \"ID\": \"SGML\", \"SortAs\": \"SGML\", \"GlossTerm\": \"Standard Generalized Markup Language\", \"Acronym\": \"SGML\", \"Abbrev\": \"ISO 8879:1986\", \"GlossDef\": \"A meta-markup language, used to create markup languages such as DocBook.\", \"GlossSeeAlso\": [\"GML\", \"XML\", \"markup\"] } ] } } }");
	printf("newObj.to_string() = %s\n", json_object_to_json_string(newObj));
	//newObj = json_object_new_object();
	//
	const char * temp = "glossary";
 	struct json_object *newObj1;
	json_object_object_get_ex(newObj, temp,&newObj1);
	printf("newObj.to_string() = %s\n", json_object_to_json_string(newObj1));

	struct json_object *newObj2;
	json_object_object_get_ex(newObj1, "pageCount",&newObj2);
	pageCount = json_object_get_int(newObj2);
	printf("Page count = %d\n", pageCount);

	json_object_put(newObj2);
	json_object_put(newObj1);
	json_object_put(newObj);
	//#####################################################################################

	json_object *jmessage = json_object_new_object();
	
	json_object_object_add(jmessage,"userid",json_object_new_int(111));
	json_object_object_add(jmessage,"username",json_object_new_string("Chunk"));
	//json_object_object_add(jmessage,"username",json_object_new_array());
	printf(" jmessqage.toString() = %s\n",json_object_to_json_string(jmessage));
	
	json_object *idObj;
	json_object_object_get_ex(jmessage,"userid",&idObj);
	printf(" idObj.toString() = %s\n",json_object_to_json_string(idObj));
	
	json_object_put(idObj);
	json_object_put(jmessage);

	//#####################################################################################
	
	json_object *array = json_object_new_array();
	for(int i=0; i<5 ; i++){
		json_object_array_add(array,json_object_new_string("hello"));
	}
	printf(" array.toString() = %s\n",json_object_to_json_string(array));
	for(int i=0; i<json_object_array_length(array);i++){
		struct json_object *obj = json_object_array_get_idx(array,i);
		printf(" array%d.toString() = %s\n",i,json_object_to_json_string(obj));
		json_object_put(obj);
	}
	
	json_object_put(array);

	//#####################################################################################
    json_object *jsonObj = json_object_new_object();
    json_object *replyObj = json_object_new_object();
	json_object *arrayObj = json_object_new_array();
	
    json_object_object_add(replyObj,"result",json_object_new_string("ok"));
	for(int i=0; i<5 ;i++){
		json_object_array_add(arrayObj,json_object_new_string("hellooo"));
	}
   	json_object_object_add(replyObj,"chat_user_list",arrayObj);   
   	

    json_object_object_add(jsonObj,"reply",replyObj);
	printf(" jsonObj.toString() = %s\n",json_object_to_json_string(jsonObj));
    
	json_object_put(arrayObj);
    json_object_put(replyObj);
    json_object_put(jsonObj);
	*/
	return 0;
}
