#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

int main() {
    setlocale(LC_ALL, "ru");
    std::string fileName;
    int numS;

    std::cout << "Введите имя бинарного файла: ";
    std::cin >> fileName;

    std::ofstream(fileName, std::ios::binary | std::ios::trunc).close();

    std::cout << "Введите количество Sender процессов: ";
    std::cin >> numS;

    for (int i = 0; i < numS; i++) {
        STARTUPINFOA si{};
        PROCESS_INFORMATION pi{};
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(si);

        std::string cmd = "C:\\dev\\projects\\msVS\\OS_labs\\Lab_4\\sender\\out\\build\\x64-debug\\sender.exe " + fileName;

        if (CreateProcessA(NULL, cmd.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cout << "Процесс Sender " << (i + 1) << " запущен" << std::endl;
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
        else {
            std::cerr << "Ошибка запуска процесса " << (i + 1) << ", код ошибки: " << GetLastError() << std::endl;
        }
    }

    std::cout << "\nФайл " << fileName << " готов, команды: read / finish" << std::endl;

    while (true) {
        std::string command;
        std::cout << "> ";
        std::cin >> command;

        if (command == "finish")
            break;

        if (command == "read") {
            std::ifstream file(fileName, std::ios::binary);
            if (!file) {
                std::cerr << "Ошибка открытия файла" << std::endl;
                continue;
            }

            while (true) {
                uint8_t len;
                if (!file.read(reinterpret_cast<char*>(&len), sizeof(len)))
                    break; 

                std::vector<char> buf(len + 1);
                file.read(buf.data(), len);
                buf[len] = '\0';

                std::cout << "Сообщения в файле: " << buf.data() << std::endl;
            }

            std::ofstream(fileName, std::ios::binary | std::ios::trunc).close();
        }
    }

    std::cout << "Процесс Receiver завершил работу" << std::endl;
    return 0;
}

