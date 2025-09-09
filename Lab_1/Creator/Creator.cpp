#include <iostream>
#include <fstream>

using namespace std;

struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours; 
};

int main(int argc, char* argv[])
{
    int numRec = atoi(argv[2]);
    employee* employees = new employee[numRec];

    cout << "Введите список сотрудников:" << endl;
    for (int i = 1; i <= numRec; i++) {
        cout << i << ".";
        cin >> employees[i].num;
        cin >> employees[i].name;
        cin >> employees[i].hours;
    }

}
