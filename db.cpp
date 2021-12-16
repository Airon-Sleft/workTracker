#include "db.h"
using namespace std;

#define DAY_SIZE 14
DataBase::DataBase()
{
    read();
    for (const pair<string, int>& data : dataList)
    {
        cout << "Date: " << data.first << ". Count: " << data.second << endl;
    }
}

void DataBase::read()
{
    std::fstream fin("test.txt", std::ios_base::in | std::ios_base::out);
    fin.seekg(0, ios_base::end);
    int size = fin.tellg(); 
    cout << "Data base size: " << size << " bytes." << endl;
    fin.seekg(0, ios_base::beg);
    while (size > 0 && fin.good() && !fin.eof())
    {
        char test[70];
        fin.getline(test, sizeof(test));
        // int pos = fin.tellg();
        // fin.seekg(-4, ios_base::cur);
        // fin << "%";
        // fin.seekg(pos, ios_base::beg);
        if (fin.eof()) break;
        string date;
        int count;
        if (DataBase::split(test, sizeof(test), date, count)) dataList[date] = count;
    }
    fin.close();    
    string word = "2021.12.13:37";
    reWriteOneDay(1, word);
}

void DataBase::reWriteOneDay(const int &dayCountBack, string& word)
{
    fstream fin;
    fin.open("test.txt", std::ios_base::in | std::ios_base::out);
    fin.seekg(-DAY_SIZE*(dayCountBack+1), ios_base::end);
    fin << word << endl;
    fin.close();     
}

void DataBase::getDate(string& writeValue)
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    writeValue = buf;
}

void DataBase::updateLast()
{
    string dateNow;
    getDate(dateNow);
    if (dataList.count(dateNow) == 0) return;
    int lastLineSize = dateNow.size() + sizeof((char) dataList[dateNow]);
    cout << "SIZE: " << lastLineSize << endl;
}

bool DataBase::split(char* text, int size, string &date, int & count)
{
    if(!text[0]) return false;
    string temp = "";
    for (int i = 0; i < size; ++i)
    {
        if (!text[i]) break;
        if (text[i] == ':')
        {
            //words.push_back(temp);
            date = temp;
            temp = "";
        }
        else
        {
            temp += text[i];
        }
    }
    if (temp.empty() || date.empty()) return false;
    count = stoi(temp);
    cout << date << " " << count << endl;
    return true;
}

DataBase::~DataBase()
{

}


int main()
{
    DataBase db;
    return 0;
}