#include "messageManager.h"
#include <fstream>
#include <algorithm>
#include <cstdint>

const int MAXLEN = 20;

bool writeMessage(const std::string& fileName, const std::string& message) {
    uint8_t len = static_cast<uint8_t>(std::min<size_t>(message.size(), MAXLEN));

    std::ofstream file(fileName, std::ios::binary | std::ios::app);
    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast<char*>(&len), sizeof(len));
    file.write(message.c_str(), len);
    file.close();

    return true;
}

std::vector<Message> readMessages(const std::string& fileName) {
    std::vector<Message> result;
    std::ifstream file(fileName, std::ios::binary);

    if (!file.is_open()) {
        return result;
    }

    while (true) {
        uint8_t len;
        if (!file.read(reinterpret_cast<char*>(&len), sizeof(len)))
            break;

        std::string buf(len, '\0');
        file.read(&buf[0], len);

        result.push_back({ buf });
    }

    file.close();
    return result;
}

void clearFile(const std::string& fileName) {
    std::ofstream(fileName, std::ios::binary | std::ios::trunc).close();
}
