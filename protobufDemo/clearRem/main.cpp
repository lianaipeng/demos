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
   
/*
message Person {
    required string name = 1;
    required int32 id = 2;
    optional string email = 3;
} 

message AddressBook {
    repeated Person person = 1; 
}
*/ 
    
    cout << "########################################A " <<endl;
    // 写消息
    tutorial::AddressBook addBook;
    for(int i=0; i<100 ; i++){
        tutorial::Person* person = addBook.add_person();
        person->set_id(111);
        person->set_name("nidaye");
        person->set_email("aaaa@.com");
    }

    /*
    tutorial::Person::PhoneNumber* phone_number = person.add_phone();
    phone_number->set_number("13712345678");
    phone_number->set_type(tutorial::Person::MOBILE);
    phone_number = person.add_phone();
    phone_number->set_number("13787654321");
    phone_number->set_type(tutorial::Person::HOME);
    */
    //cout << "DebugString:" << person.DebugString() << endl;  
    cout << "###############################" << endl;  
   
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
