#include <iostream>
#include <fstream>

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
    ofstream binf(argv[1], ios::binary | ios::out);

    if (!binf) {
        cerr << "Ошибка открытия файла" << argv[1] << endl;
        return 1;
    }

    int numRec = _wtoi(argv[2]);
    employee* employees = new employee[numRec];

    cout << "Введите список сотрудников:" << endl;
    for (int i = 0; i < numRec; i++) {
        cout << i << ". ";
        cin >> employees[i].num;
        cin >> employees[i].name;
        cin >> employees[i].hours;
    }

    for (int j = 0; j < numRec; j++) {
        binf.write(reinterpret_cast<char*>(&employees[j]), sizeof(employee));
    }

    binf.close();
    
    return 0;
}
