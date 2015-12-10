

#include "MQueue.h"

//static void MQueue::pushNode(const string &node);
list<string> MQueue::listStr; 

bool MQueue::isEmpty(){
	if(listStr.empty()){
		return true;
	}
	return false;
}
string MQueue::popNode(){
	string temp = "";
	if(!listStr.empty()){
		temp = listStr.front();
		listStr.pop_front();
	}
	return temp;
}
void MQueue::pushNode(const string & node){
	listStr.push_back(node);
}
