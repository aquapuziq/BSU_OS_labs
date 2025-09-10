#include <iostream>
#include <string> 
#include <fstream>
#include <windows.h>
#include <stdexcept>
#include <vector>

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
    wstring binfName;
    int numRec;

    cout << "Введите имя бинарного файла и количество записей в этом файле: ";
    wcin >> binfName;

    try {
        cin >> numRec;

        if (cin.fail()) {
            throw invalid_argument("Ошибка: введено не число");
        }

        if (numRec <= 0) {
            throw out_of_range("Ошибка: число записей не может быть меньше единицы");
        }
    }

    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1; 
    }
    
    wstring exePathCr = L"Creator.exe";
    wstring cmdParamsCr = L"\"" + exePathCr + L"\" " + binfName + L" " + to_wstring(numRec);

    vector<wchar_t> cmdLineCr(cmdParamsCr.begin(), cmdParamsCr.end());
    cmdLineCr.push_back(L'\0');

    STARTUPINFO create;
    PROCESS_INFORMATION startCreator;

    ZeroMemory(&create, sizeof(STARTUPINFO));
    create.cb = sizeof(create);
    ZeroMemory(&startCreator, sizeof(startCreator));

    cout << "Была вызвана утилита Creator" << endl;

    if (!CreateProcessW(exePathCr.c_str(), cmdLineCr.data(), NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &create, &startCreator)) {
        cout << "Ошибка: процесс не был создан, пожалуйста, проверьте имя и путь файла. Код ошибки: " << GetLastError() << endl;
        return 1;
    }
    
    WaitForSingleObject(startCreator.hProcess, INFINITE);
    CloseHandle(startCreator.hThread);
    CloseHandle(startCreator.hProcess);

    cout << "Утилита Creator отработала и создала файл, содержащий:" << endl;

    ifstream binf(binfName, ios::binary);
    employee procEmployee;
    for (int i = 0; i < numRec; i++) {
        binf.read(reinterpret_cast<char*>(&procEmployee), sizeof(employee));
        cout << procEmployee.num << " " << procEmployee.name << " " << procEmployee.hours << endl;
    }

    wstring repName;
    float payPerHour;

    cout << "Введите величину оплаты за час работы: ";
    try {
        cin >> payPerHour;

        if (cin.fail()) {
            throw invalid_argument("Ошибка: введено не число");
        }

        if (payPerHour < 0) {
            throw out_of_range("Ошибка: ставка не может быть отрицательной");
        }
    }

    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    cout << "Введите имя файла для отчёта: ";
    wcin >> repName;

    wstring exePathRe = L"Reporter.exe";
    wstring cmdParamsRe = L"\"" + exePathRe + L"\" " + binfName + L" " + L" " + repName + L" " + to_wstring(payPerHour);

    vector<wchar_t> cmdLineRe(cmdParamsRe.begin(), cmdParamsRe.end());
    cmdLineRe.push_back(L'\0');

    STARTUPINFO report;
    PROCESS_INFORMATION startReporter;

    ZeroMemory(&report, sizeof(STARTUPINFO));
    report.cb = sizeof(report);
    ZeroMemory(&startReporter, sizeof(startReporter));

    cout << "Была вызвана утилита Reporter" << endl;

    if (!CreateProcessW(exePathRe.c_str(), cmdLineRe.data(), NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, NULL, NULL, &report, &startReporter)) {
        cout << "Ошибка: процесс не был создан, пожалуйста, проверьте имя и путь файла. Код ошибки: " << GetLastError() << endl;
        return 1;
    }

    WaitForSingleObject(startReporter.hProcess, INFINITE);
    CloseHandle(startReporter.hThread);
    CloseHandle(startReporter.hProcess);

    cout << "Утилита Reporter отработала. Результаты отчета:" << endl;
    
    return 0;
}

