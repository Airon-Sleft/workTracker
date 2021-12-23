#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <time.h>
#include <cmath>

class DataBase
{
    public:
    DataBase();
    ~DataBase();
    private:
    bool putToMemory(char* text, int size, const int& bytePosition);
    bool load(const int& startDay, const int& countDay);
    void update(const int& oneDayCount);
    void getDate(std::string& writeValue);
    void reWriteOneDay(const int & byteOffset, const std::string& word );
    void createFileIfNot();
    int getTodayCountDB();
    std::map<std::string, std::vector<int>> dataList; 
    public:
    int getTodayCount();
    void setTodayCount(const int& newCount);
};