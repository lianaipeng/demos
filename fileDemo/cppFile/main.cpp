#include <fstream>
#include <iostream>
#include <cstdlib>


int main(int argc,char **argv){
    if(argc != 2){
        std::cout << "argc must be to two!" << std::endl;
        exit(1);
    }
    std::cout << "file name is " << argv[1] << std::endl; 
    
    std::ofstream fout(argv[1]);
    fout << "hhaksdhfajksdhflaksjd" << std::endl;
    fout << std::flush ;
    fout.close();
    
    
    std::ifstream fin(argv[1]);
    for(std::string s;getline(fin,s);)
        std::cout << s << std::endl;

    return 0;
}
