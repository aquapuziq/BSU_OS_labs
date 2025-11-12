#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

const int MAXLEN = 20;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

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
            if (message.size() > MAXLEN) message.resize(MAXLEN);

            std::ofstream file(fileName, std::ios::binary | std::ios::app);
            if (!file) {
                std::cerr << "Ошибка открытия файла" << std::endl;
                continue;
            }

            uint8_t len = static_cast<uint8_t>(message.size());
            file.write(reinterpret_cast<char*>(&len), sizeof(len));
            file.write(message.c_str(), len);
            file.flush();

            std::cout << "Сообщение записано" << std::endl;
        }
    }

    std::cout << "Sender завершил работу.\n";
    return 0;
}
