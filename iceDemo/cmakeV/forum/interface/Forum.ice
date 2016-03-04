#pragma once
/*
    社区
*/

module Forum{
    struct UserInfo{
        int id;
        string name;
    };
    // sequence<Fruit> FruitPlatter;
    // dictionary<long, Employee> EmployeeMap;

    interface ForumInterface{
        void sayHello(string str);
        void getUserInfo(int id,out UserInfo user_info);
    };
};
