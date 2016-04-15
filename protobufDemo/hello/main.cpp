#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"
using namespace std;

// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main(int argc, char* argv[]) {
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;
   
    tutorial::IdList id_list;
    id_list.add_list(10);
    id_list.add_list(11);
    id_list.add_list(12);
    for(int i=0; i < id_list.list_size(); i++){
        cout << "asdasd:" << id_list.list(i) << endl;
    }

    cout << "########################################A " <<endl;
    // 写消息
    tutorial::Person person;
    person.set_id(111);
    person.set_name("nidaye");
    person.set_email("aaaa@.com");

    tutorial::Person::PhoneNumber* phone_number = person.add_phone();
    phone_number->set_number("13712345678");
    phone_number->set_type(tutorial::Person::MOBILE);
    phone_number = person.add_phone();
    phone_number->set_number("13787654321");
    phone_number->set_type(tutorial::Person::HOME);
    cout << "DebugString:" << person.DebugString() << endl;  
    
    cout << "###############################" << endl;  
   
   /*
    // Write the new address book back to disk.
    fstream output(argv[1], ios::out| ios::trunc | ios::binary);
    if(!person.SerializeToOstream(&output)){
        cerr << "Failed to write person " << endl;
    }
   */

    tutorial::Person tmpPerson;
    // Read the existing address book.
    fstream input(argv[1], ios::in| ios::binary);
    if(!tmpPerson.ParseFromIstream(&input)){
        cerr <<"Failed to parse address book."<< endl;
        return -1;
    }

    // 序列化消息，将存储字节的以string方式输出。注意字节是二进制，而非文本
    string outString;
    if(tmpPerson.SerializeToString(&outString)) 
        cout << outString << endl;
    // 解析给定的string
    // bool ParseFromString(const string& data)
    cout << "###############################" << endl;  

    // 读消息
    cout<< "Person Id:" << person.id() << endl;
    cout<< "Person name:" << person.name() << endl;
    if(person.has_email())
        cout << "Person email:" << person.email() << endl;
    
    for(int i=0; i < person.phone_size(); i++){
        const tutorial::Person::PhoneNumber& tmp = person.phone(i);
        switch (tmp.type()) {
            case tutorial::Person::MOBILE:
                cout << "  Mobile phone #: ";
                break;
            case tutorial::Person::HOME:
                cout << "  Home phone #: ";
                break;
            case tutorial::Person::WORK:
                cout << "  Work phone #: ";
                break;
        }
        cout << tmp.number() << endl;
    } 
    
    
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
