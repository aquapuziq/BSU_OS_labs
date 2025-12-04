#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include  "C:\dev\projects\msVS\OS_labs\Lab_5\ClientServerHead\employee.h"
#include "C:\dev\projects\msVS\OS_labs\Lab_5\ClientServerHead\ClientServerHead.h"

using namespace std;

static ReadWriteLock* rwLock = nullptr;
static string fileName;

bool findById(int id, Employee& employee, fstream& file) {
    file.seekg(0, ios::beg);
    while (file.read((char*)&employee, sizeof(Employee))) {
        if (employee.num == id) return true;
    }
    return false;
}

bool writeById(const Employee& employee, fstream& file) {
    file.seekg(0, ios::beg);
    Employee cur;
    while (file.read((char*)&cur, sizeof(Employee))) {
        if (cur.num == employee.num) {
            file.seekp((int)file.tellg() - sizeof(Employee));
            file.write((char*)&employee, sizeof(Employee));
            return true;
        }
    }
    return false;
}
DWORD WINAPI InstanceThread(LPVOID pipe) {
    HANDLE hPipe = (HANDLE)pipe;
    while (true) {
        char command[20] = "";
        DWORD dwRead = 0;
        if (!ReadFile(hPipe, &command, sizeof(command), &dwRead, NULL) || dwRead == 0)
            break;

        fstream file(fileName, ios::binary | ios::in | ios::out);
        if (!file.is_open()) {
            cout << "Невозможно открыть файл" << endl;
            break;
        }

        if (strcmp(command, "read") == 0) {
            rwLock->startRead();

            int id;
            ReadFile(hPipe, &id, sizeof(id), &dwRead, NULL);

            Employee employee{};
            bool found = findById(id, employee, file);

            WriteFile(hPipe, &found, sizeof(found), &dwRead, NULL);
            if (found) WriteFile(hPipe, &employee, sizeof(employee), &dwRead, NULL);

            int endMarker;
            ReadFile(hPipe, &endMarker, sizeof(endMarker), &dwRead, NULL);

            rwLock->endRead();
        }
        else if (strcmp(command, "write") == 0) {
            rwLock->startWrite();

            int id;
            ReadFile(hPipe, &id, sizeof(id), &dwRead, NULL);

            Employee e{};
            bool found = findById(id, e, file);

            WriteFile(hPipe, &found, sizeof(found), &dwRead, NULL);
            if (found) WriteFile(hPipe, &e, sizeof(e), &dwRead, NULL);

            Employee updated{};
            ReadFile(hPipe, &updated, sizeof(updated), &dwRead, NULL);
            writeById(updated, file);

            int endMarker;
            ReadFile(hPipe, &endMarker, sizeof(endMarker), &dwRead, NULL);

            rwLock->endWrite();
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
    }

    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return 0;
}

void printFile() {
    fstream f(fileName, ios::binary | ios::in);
    Employee employee;

    cout << "\nСодержимое файла" << endl;
    while (f.read((char*)&employee, sizeof(Employee))) {
        cout << "ID: " << employee.num << ", Имя: " << employee.name << ", часы: " << employee.hours << endl;
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "ru");
    cout << "Введите имя файла: ";
    cin >> fileName;

    int n;
    cout << "Введите число сотрудников: ";
    cin >> n;

    vector<Employee> list(n);
    cout << "Введите сотрудников (id, имя, часы): " << endl;
    for (int i = 0; i < n; i++) cin >> list[i].num >> list[i].name >> list[i].hours;

    ofstream fout(fileName, ios::binary);
    for (auto& e : list) fout.write((char*)&e, sizeof(Employee));
    fout.close();

    printFile();

    rwLock = new ReadWriteLock("WriteSemaphore", "SyncMutex");

    int clients;
    cout << "Введите число процессов-клиентов: ";
    cin >> clients;

    HANDLE* hThreads = new HANDLE[clients];
    DWORD dwThreadId;

    for (int i = 0; i < clients; i++) {
        HANDLE hPipe = CreateNamedPipeA(
            "\\\\.\\pipe\\server_pipe",
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            sizeof(Employee),
            sizeof(Employee),
            0, NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            cout << "Невозможно создать канал" << endl;
            return 1;
        }

        STARTUPINFOA si{};
        PROCESS_INFORMATION pi{};
        si.cb = sizeof(si);

        if (!CreateProcessA("C:\\dev\\projects\\msVS\\OS_labs\\Lab_5\\Client\\out\\build\\x64-debug\\Client.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            cout << "Невозможно запустить процесс-клиент " << i << endl;
            CloseHandle(hPipe);
            continue;
        }
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        if (ConnectNamedPipe(hPipe, NULL) || GetLastError() == ERROR_PIPE_CONNECTED) {
            hThreads[i] = CreateThread(NULL, 0, InstanceThread, (LPVOID)hPipe, 0, &dwThreadId);
            if (!hThreads[i]) cout << "Невозможно создать поток для клиента " << i << endl;
        }
        else CloseHandle(hPipe);
    }

    cout << "Программа запущена";

    WaitForMultipleObjects(clients, hThreads, TRUE, INFINITE);

    for (int i = 0; i < clients; i++) CloseHandle(hThreads[i]);

    printFile();
    delete[] hThreads;
    delete rwLock;
    return 0;
}

