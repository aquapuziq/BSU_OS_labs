#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours;
};

int wmain(int argc, wchar_t* argv[])
{
    setlocale(LC_ALL, "ru");

    ifstream binf(argv[1], ios::binary);
    ofstream rept(argv[2]);
    float payPerHour = _wtoi(argv[3]);

    return  0;
}
