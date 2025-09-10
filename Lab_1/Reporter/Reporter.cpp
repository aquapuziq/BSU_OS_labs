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

    vector<employee> employees;
    employee tmpEmployee;


    while (binf.read((char*)&tmpEmployee, sizeof(employee))) {
        employees.push_back(tmpEmployee);
    }

    rept << "Отчёт по файлу " << argv[1] << ":" << endl;
    rept << "Номер сотрудника, имя сотрудника, часы, зарплата." << endl;

    for (int i = 0; i < employees.size(); i++) {
        rept << employees[i].num << " " << employees[i].name << " " <<
            employees[i].hours << " " << employees[i].hours * payPerHour << endl;
    }

    return  0;
}
