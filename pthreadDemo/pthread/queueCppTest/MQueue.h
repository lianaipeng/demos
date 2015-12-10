
#ifndef __MQUEUE_H__
#define __MQUEUE_H__

#include <list>
#include <string>

using namespace std;

class MQueue{
	public:
		MQueue(){
			listStr.clear();
		}
		~MQueue(){
			
		}

		bool isEmpty();
		string popNode();
		static void pushNode(const string & node);

	private:
		static list<string> listStr;
};

#endif
