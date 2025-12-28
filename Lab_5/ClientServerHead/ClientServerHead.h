#pragma once
#include <windows.h>

class ReadWriteLock {
private:
    HANDLE hSemaphore;
    HANDLE hMutex;
    int n;

public:
    ReadWriteLock(const char* semName, const char* mutName);
    ~ReadWriteLock();

    void startRead();
    void endRead();
    void startWrite();
    void endWrite();
};
