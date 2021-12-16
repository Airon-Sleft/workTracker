#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <time.h>

class DataBase
{
    public:
    DataBase();
    ~DataBase();
    private:
    bool split(char* text, int size, std::string &date, int& count);
    void read();
    void updateLast();
    void getDate(std::string& writeValue);
    void reWriteOneDay(const int & dayCountBack, std::string& word );
    public:
    std::map<std::string, int> dataList; 
};