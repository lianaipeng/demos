
/*
 * 类模板成员函数
 * 1.必须以关键字template开头，后接类的模板形参表。
 * 2.必须指出他是那个类的成员。
 * 3.类名必须包含其模板形参。
 * 在类外定义的Queue类的成员函数的开头应该是：
 * template <class Type> ret-type Queue<Type>::member-name
 * 在作用域操作符(::)之前使用的Queue<Type>指定成员函数所属的类
 */

/* 
 * 模板类不支持分离实现，你把声明和实现文件放一起吧
 * 或是使用export关键字，但需要编译器支持这个关键字
 * 或者在用到这个类的时候把.cpp文件也include进来
 * */
