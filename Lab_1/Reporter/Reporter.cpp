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

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");

    ifstream binf(argv[1], ios::binary);
    fstream rept(argv[2]);
    float payPerHour = atof(argv[3]);

    vector<employee> employees;
    employee tmpEmployee;

    while (binf.read((char*)&tmpEmployee, sizeof(employee))) {
        employees.push_back(tmpEmployee);
    }

    rept << "Отчёт по файлу " << argv[1] << ":" << endl;
    rept << "Номер сотрудника, имя сотрудника, часы, зарплата." << endl;

    for (int i = 0; i < employees.size(); i++) {
        float salary = float(employees[i].hours) * payPerHour;
        rept << employees[i].num << " " << employees[i].name << " " <<
            employees[i].hours << " " << salary << endl;
    }

    rept.close();
    return  0;
}
