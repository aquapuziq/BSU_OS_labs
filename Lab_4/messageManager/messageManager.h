#pragma once
#include <string>
#include <vector>

struct Message {
    std::string text;
};

// запись одного сообщения в бинарный файл
bool writeMessage(const std::string& fileName, const std::string& msg);

// чтение всех сообщений из бинарного файла
std::vector<Message> readMessages(const std::string& fileName);

// очистка (обнуление) файла
void clearFile(const std::string& fileName);
