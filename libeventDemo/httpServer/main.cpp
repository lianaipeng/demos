
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <event.h>
#include <evhttp.h>

/*
static void make_response_exception(std::string& response, const int32_t& message_type, const std::string& what, const std::string& why)
{
    struct json_object* jresponse = json_object_new_object();
    json_object_object_add(jresponse, "version", json_object_new_string("2.0"));
    json_object_object_add(jresponse, "message_type", json_object_new_int(10002));
    json_object_object_add(jresponse, "code", json_object_new_string(what.c_str()));
    json_object_object_add(jresponse, "description", json_object_new_string(why.c_str()));
    response = json_object_to_json_string(jresponse);
    json_object_put(jresponse);
}
*/

void default_handle(struct evhttp_request *req,void *arg){
    const char *uri = evhttp_request_get_uri(req);
    printf("###################### uri=%s\n",uri);
    char * decode_uri = evhttp_decode_uri(uri);
    printf("###################### decode_uri=%s\n",decode_uri);

    const char* host = evhttp_find_header(req->input_headers, "Host");
    const char* xri = evhttp_find_header(req->input_headers, "X-Real-IP");
    const char* xff = evhttp_find_header(req->input_headers, "X-Forwarded-For");
    printf("### Host=%s  X-Real-IP=%s  X-Forwarded-For=%s\n",host,xri,xff);

    /* Response the client */
    /*
    if (evhttp_request_get_command(req) != EVHTTP_REQ_GET) {
        //dump_request_cb(req, arg);
        evhttp_send_reply(req, 200, "OK", NULL);
        return;
    } */

    struct evbuffer *evb = evbuffer_new();
    std::string data = "Welcome to libevent.\n";
    evbuffer_add_printf(evb,data.c_str());
    evhttp_send_reply(req,200,"OK",evb);
    evbuffer_free(evb);
}

void post_handle(struct evhttp_request *req,void *arg){
    const char *uri = evhttp_request_get_uri(req);
    printf("###################### uri=%s\n",uri);
    char * decode_uri = evhttp_decode_uri(uri);
    printf("###################### decode_uri=%s\n",decode_uri);

    const char* host = evhttp_find_header(req->input_headers, "Host");
    const char* xri = evhttp_find_header(req->input_headers, "X-Real-IP");
    const char* xff = evhttp_find_header(req->input_headers, "X-Forwarded-For");
    printf("### Host=%s  X-Real-IP=%s  X-Forwarded-For=%s\n",host,xri,xff);

    if (evhttp_request_get_command(req) == EVHTTP_REQ_POST) {
        printf("############### post\n");
        
        int buffer_data_len = EVBUFFER_LENGTH(req->input_buffer);
        printf("############## size:%d\n",buffer_data_len);

        char *post_data = (char *) malloc(buffer_data_len + 1);
        memset(post_data, '\0', buffer_data_len + 1);
        memcpy(post_data, EVBUFFER_DATA(req->input_buffer), buffer_data_len);
        printf("########### data_len=%d  data=%s\n",buffer_data_len,post_data);
        
        /*       
        std::string request_buff; // 请求数据缓冲区
        // 获取缓冲区
        struct evbuffer * evcontent = evhttp_request_get_input_buffer(req);
        // 获取缓冲区大小
        int content_length = evbuffer_get_length(evcontent);
        request_buff.resize(content_length);
        
        // 读取缓冲区中的数据(缓冲区不清空) -1表示全部
        memcpy(&request_buff[0], evbuffer_pullup(evcontent, -1), content_length);
        printf("### %d  %s\n",content_length,&request_buff[0]);

        // 判断请求是否以"data=" 开头
        if( strncmp(request_buff.c_str(), "data=", strlen("data=") ) != 0) {
            return;
        }   
        */
    }   

    struct evbuffer *evb = evbuffer_new();
    std::string data = "Welcome to libevent.";
    evbuffer_add_printf(evb,data.c_str());
    //evhttp_send_reply(req,HTTP_OK,"OK",buf);
    evhttp_send_reply(req,200,"OK",evb);
    evbuffer_free(evb);
}
void get_handle(struct evhttp_request *req,void *arg){
    const char *uri = evhttp_request_get_uri(req);
    printf("###################### uri=%s\n",uri);
    char * decode_uri = evhttp_decode_uri(uri);
    printf("###################### decode_uri=%s\n",decode_uri);

    struct evkeyvalq params;
    evhttp_parse_query(decode_uri, &params);
    char* id_key = (char*)"id";
    const char* id_value = evhttp_find_header(&params, id_key);
    printf("********************** id = %s\n", id_value);
    char* name_key = (char*)"name";
    const char* name_value = evhttp_find_header(&params, name_key);
    printf("********************** name = %s\n", name_value);

    if( (id_value==NULL)||(name_value==NULL) ){
        evhttp_send_error(req, 400, "Bad Request");
    }
}
int main(int argc,char **argv){
    //  提供对外接口
    struct evhttp *httpd;
    event_init();

    //std::string monitor_host = "127.0.0.1";
    std::string monitor_host = "192.168.1.224";
    int monitor_port   = 11111;
    httpd = evhttp_start(monitor_host.c_str(),monitor_port);
    if(httpd == NULL){
        fprintf(stderr,"Error:Unable to listen on %s:%d \n",monitor_host.c_str(),monitor_port);
        exit(1);
    }  
    evhttp_set_timeout(httpd,2000);

    evhttp_set_gencb(httpd,default_handle,NULL);
    evhttp_set_cb(httpd,"/get_test",get_handle,NULL);
    evhttp_set_cb(httpd,"/post_test",post_handle,NULL);
    //  end 对外接口
    
    event_dispatch();
    evhttp_free(httpd);
    
    return 0;
}
