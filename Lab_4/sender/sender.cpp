#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include "messageManager.h"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        std::cerr << "Ошибка: не указано имя файла." << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::cout << "Процесс Sender запущен, команды: send / finish" << std::endl;

    while (true) {
        std::string command;
        std::cout << "> ";
        std::cin >> command;

        if (command == "finish")
            break;

        if (command == "send") {
            std::cin.ignore();
            std::string message;
            std::cout << "Введите сообщение (до 20 символов): ";
            std::getline(std::cin, message);

            if (message.empty()) {
                std::cout << "Пустое сообщение не будет записано." << std::endl;
                continue;
            }

            if (!writeMessage(fileName, message)) {
                std::cerr << "Ошибка записи" << std::endl;
                continue;
            }

            std::cout << "Сообщение записано" << std::endl;
        }
    }

    std::cout << "Sender завершил работу" << std::endl;
    return 0;
}
