#include <iostream>
#include <string>
#include <iterator>
#include <vector>

namespace datatype{
    struct id{};
    struct name{};

    struct employee{
	int id;
	std::string name;

	employee(int id,const std::string& name):id(id),name(name){}
	bool operator <(const employee &e)const{
	    std::cout << "id:" << id << std::endl;
	    return id < e.id;
	}

	bool operator <=(const employee& e)const {
	    return id <= e.id;
	}
	bool operator ==(const employee& right){
	    return id == right.id && name == right.name;
	}
	friend std::ostream& operator << (std::ostream& os,const employee& dt){
	    os << "EMPLOYEE id:" << dt.id << ",EMPLOYEE.name:" << dt.name;
	    return os;
	}
    };
}

using namespace datatype;
using namespace std;

int main(){ 
    ostream_iterator<int> intWriter(cout,"\n");
    *intWriter = 42;
    intWriter++;
    *intWriter = 77;
    intWriter++;
    *intWriter = -5;

    vector<int> coll;
    for(int i=0; i <= 9 ; i++){
	coll.push_back(i);
    }
    copy(coll.begin(),coll.end(),ostream_iterator<int>(cout,"<"));
    cout << endl;
    
    /*
    istream_iterator<int> intReader(cin);
    istream_iterator<int> intReaderEOF;
    while(intReader != intReaderEOF){
	cout << "once:" << *intReader << endl;
	cout << "again:" << *intReader << endl;
	++intReader;
    }
    */

    employee em1(10,"em100");    
    employee em2(20,"em200");    
    employee em3(30,"em300");    
    std::vector<employee> tmpVec;
    tmpVec.push_back(em1);
    tmpVec.push_back(em2);
    tmpVec.push_back(em3);
    vector<employee>::iterator iterVec;
    iterVec = tmpVec.begin();
    while( iterVec != tmpVec.end()){
	cout << *iterVec << endl;
	iterVec++;
    }
    std::cout << "################## " << std::endl;
    //std::copy(tmpVec.begin().tmpVec.end(),iostream_iterator<employee>(std::cout,"\n"));
    std::copy(tmpVec.begin(),tmpVec.end(),ostream_iterator<employee>(std::cout,"\n"));
    std::cout << "################## " << std::endl;
    
    if(em1 == em2){
	std::cout << "em1 == em2" << std::endl;
    }else{
	std::cout << "em1 != em2" << std::endl;
    }
    
    if(em1 < em2){
	std::cout << "em1 < em2" << std::endl;
    }


    std::cout << em1 << std::endl;
    return 0;
}
