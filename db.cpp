#include "db.h"
using namespace std;

#define DAY_SIZE 30
#define FILE_NAME "db.txt"
DataBase::DataBase()
{
    load(0, 1);
    // for (const pair<string, vector<int>>& data : dataList)
    // {
    //     cout << "Date: " << data.first << ". Count: " << data.second[0] << " OFFSERT POS: " << data.second[1] << endl;
    // }
}

int DataBase::getTodayCount()
{
    return getTodayCountDB()*1000;
}

int DataBase::getTodayCountDB()
{
    string toDay;
    getDate(toDay);
    if (dataList.count(toDay) == 0) return 0;
    return dataList[toDay][0];
}

void DataBase::setTodayCount(const int& newCount)
{
    update( floor(newCount/1000.0) );
}

bool DataBase::load(const int& startDay, const int& countDay) // countDay - back
{
    std::fstream fin(FILE_NAME, std::ios_base::in);
    fin.seekg(0, ios_base::end);
    int size = fin.tellg();
    if (size == 0 or size == -1) return false; 
    cout << "Data base size: " << size << " bytes." << endl;
    int offset = (startDay+1)*DAY_SIZE;
    for (int iter = 0; iter < countDay; ++iter)
    {
        offset -= iter*DAY_SIZE;
        fin.seekg(-offset, ios_base::end);
        if (!fin.good() || fin.eof()) break;
        char oneDay[DAY_SIZE];
        fin.read(oneDay, sizeof(oneDay));
        putToMemory(oneDay, sizeof(oneDay), offset);
    }
    fin.close();
    return true;
}

void DataBase::reWriteOneDay(const int &byteOffset, const string& word)
{
    createFileIfNot();
    fstream fin;
    char binaryWord[DAY_SIZE];
    int wordSize = word.size();
    for (int i = 0; i < DAY_SIZE; ++i)
    {
        if (i < wordSize) binaryWord[i] = word[i];
        else if(i == wordSize) binaryWord[i] = '|';
        else binaryWord[i] = '0';
    }
    fin.open(FILE_NAME, std::ios_base::in | std::ios_base::out);
    fin.seekg(-byteOffset, ios_base::end);
    fin.write(binaryWord, sizeof(binaryWord));
    fin.close();     
}

void DataBase::createFileIfNot()
{
    fstream fin;
    fin.open(FILE_NAME, std::ios_base::in);
    fin.seekg(0, ios_base::end);
    int size = fin.tellg();
    fin.close();
    if (size == -1 or size == 0 )
    {
        fin.open(FILE_NAME, std::ios_base::out);
        char binaryWord[DAY_SIZE];
        for (int i = 0; i < DAY_SIZE; ++i)
        {
            binaryWord[i] = '0';
        }
        fin.write(binaryWord, sizeof(binaryWord));
        fin.close();
    } 
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

void DataBase::update(const int& oneDayCount) // эта версия функции пишет только в сегодняшний день. Для перезаписи предыдущих (зачем?) нужно сделать перегрузку
{ // не тестировалось
    string dateNow;
    getDate(dateNow);
    if (dataList.count(dateNow) == 0) 
    {
        vector <int> valuesList;
        valuesList.push_back(oneDayCount);
        valuesList.push_back(0);
        dataList[dateNow] = valuesList;
    }
    else
    {
        dataList[dateNow][0] = oneDayCount;
    }
    string word = string(dateNow) + ":" + string(to_string(dataList[dateNow][0]));
    reWriteOneDay(dataList[dateNow][1], word);
    if (dataList[dateNow][1] == 0) dataList[dateNow][1] = DAY_SIZE;
}

bool DataBase::putToMemory(char* text, int size, const int& bytePosition)
{
    if(!text[0]) return false;
    string date;
    int count;
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
        else if(text[i] == '|')
        {
            break;
        }
        else 
        {
            temp += text[i];
        }
    }
    if (temp.empty() || date.empty()) return false;
    count = stoi(temp);
    vector<int> values;
    values.push_back(count);
    values.push_back(bytePosition);
    dataList[date] = values;
    return true;
}

DataBase::~DataBase()
{
    int count = getTodayCountDB();
    if (count > 0)
    {
        update(count);
    }  
}

int main()
{
    DataBase db;
    return 0;
}