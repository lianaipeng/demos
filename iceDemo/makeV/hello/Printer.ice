module Demo{
    struct MStruct{
        long number;
        string name;
        //bool operator>(const MStruct &) const;

    };

    sequence<MStruct> MStructVec; 
    dictionary<int,MStructVec> MStructVecMap;

    interface Printer{
        void printString(string s);
        int getInt(string s);
        int getNumber(MStruct stru);
        //int getNumber(out MStruct stru);
    };
};
