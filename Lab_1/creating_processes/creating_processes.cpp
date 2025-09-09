#include <iostream>
#include <string> 

using namespace std;

struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours; // количество отработанных часов
};

int main()
{
    string binfName, tmp;
    int numRec;

    cout << "Введите имя бинарного файла и количество записей в этом файле";
    cin >> binfName;
    cin >> numRec;

    tmp = "Creator.exe " + binfName + " " + to_string(numRec);

    cout << "Была вызвана утилита Creator";
}

