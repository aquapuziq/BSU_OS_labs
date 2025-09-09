#include <iostream>
#include <string> 
#include <fstream>
#include <windows.h>
#include <stdexcept>

using namespace std;

struct employee
{
    int num; // идентификационный номер сотрудника
    char name[10]; // имя сотрудника
    double hours; // количество отработанных часов
};

int main()
{
    setlocale(LC_ALL, "ru");
    wstring binfName, tmp;
    int numRec;
    wchar_t callLine[50];

    cout << "Введите имя бинарного файла и количество записей в этом файле: ";
    wcin >> binfName;

    try {
        cin >> numRec;

        if (cin.fail()) {
            throw invalid_argument("Ошибка: введено не число");
        }

        if (numRec < 0) {
            throw out_of_range("Ошибка: число не может быть отрицательным");
        }
    }

    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1; 
    }
    
    tmp = L"Creator.exe " + binfName + L" " + to_wstring(numRec);
    lstrcpyW(callLine, tmp.data());


    STARTUPINFO create;
    PROCESS_INFORMATION startCreator;

    ZeroMemory(&create, sizeof(STARTUPINFO));
    create.cb = sizeof(STARTUPINFO);

    cout << "Была вызвана утилита Creator" << endl;

    if (!CreateProcess(NULL, callLine, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &create, &startCreator)) {
        cout << "Ошибка: процесс не был создан, пожалуйста, проверьте имя файла" << endl;
        return 1;
    }

    WaitForSingleObject(startCreator.hProcess, INFINITE);
    CloseHandle(startCreator.hThread);
    CloseHandle(startCreator.hProcess);

    cout << "Утилита Creator отработала и создала файл: ";

    ifstream binf(binfName, ios::binary);
    employee procEmployee;
    for (int i = 0; i < numRec; i++) {
        binf.read((char*)&procEmployee, sizeof(employee));
        cout << procEmployee.num << " " << procEmployee.name << " " << procEmployee.hours << endl;
    }
}

