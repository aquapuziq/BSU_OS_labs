#include <iostream>
#include <windows.h>
#include "C:\dev\projects\msVS\OS_labs\Lab_5\ClientServerHead\employee.h"

using namespace std;

int main()
{
    char c[20];
    char command[20];
    int data;
    Employee employee;
    emp
    DWORD dw;
    bool b;

    HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "WriteSemaphore");
    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "SyncMutex");
    HANDLE hServer = CreateFileA(
        "\\\\.\\pipe\\server_pipe",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        (LPSECURITY_ATTRIBUTES)NULL,
        OPEN_EXISTING,
        0,
        (HANDLE)NULL
    );
    if (hServer == INVALID_HANDLE_VALUE) {
        cout << "Открытие канала вызвало ошибку";
        cin >> c;
        return 1;
    }


    while (true) {
        cout << "Введите команду (read, write or exit): ";
        cin >> command;
        if (strcmp(command, "exit") == 0) {
            break;
        }
        if (!WriteFile(hServer, &command, sizeof(command), &dw, NULL)) {
            cerr << "Ошибка записи в канал";
            cin >> c;
            return 1;
        }


        else if (strcmp(command, "read") == 0) {
            cout << "Введите ID сотрудника: ";
            cin >> data;
            if (!WriteFile(hServer, &data, sizeof(data), &dw, NULL)) {
                cerr << "Ошибка записи в канал";
                cin >> c;
                return 1;
            }

            if (!ReadFile(hServer, &b, sizeof(bool), &dw, NULL)) {
                cerr << "Ошибка чтения канала";
                CloseHandle(hServer);
                return 1;
            }

            if (b == false) {
                cout << "Employee not found" << endl;
                continue;
            }
            else {
                if (!ReadFile(hServer, &employee, sizeof(employee), &dw, NULL)) {
                    cerr << "Ошибка чтения канала";
                    CloseHandle(hServer);
                    return 1;
                }
                cout << "Клиент получил следующего сотрудника: " << employee.num << " " << employee.name
                    << " " << employee.hours << endl;
            }
            cout << "Нажмите любую клавишу для чтения...";
            cin >> c;
        }
        else if (strcmp(command, "write") == 0) {
            cout << "Введите ID сотрудника: ";
            cin >> data;
            if (!WriteFile(hServer, &data, sizeof(data), &dw, NULL)) {
                cerr << "Ошибка чтения канала";
                cin >> c;
                return 1;
            }

            if (!ReadFile(hServer, &b, sizeof(bool), &dw, NULL)) {
                cerr << "Ошибка чтения канала";
                CloseHandle(hServer);
                return 1;
            }

            if (b == false) {
                cout << "Сотрудник не найден" << endl;
                continue;
            }
            else {
                if (!ReadFile(hServer, &employee, sizeof(employee), &dw, NULL)) {
                    cerr << "Ошибка чтения канала";
                    CloseHandle(hServer);
                    return 1;
                }
                cout << "Клиент получил следующего сотрудника: " << employee.num << " " << employee.name
                    << " " << employee.hours << endl;
                cout << "Введите нового сотрудника (ID, имя, часы): ";
                cin >> employee.num >> employee.name >> employee.hours;

                cout << "Введите что-либо для отправки данных на сервер: ";
                cin >> c;

                if (!WriteFile(hServer, &employee, sizeof(employee), &dw, NULL)) {
                    cerr << "Ошибка чтения канала";
                    cin >> c;
                    return 1;
                }
            }

            cout << "Введите что-либо для записи: ";
            cin >> c;
        }
    }

    CloseHandle(hServer);
    cout << "Введите что-либо для завершения работы";
    cin >> c;
    return 0;
}