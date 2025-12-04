#include "ClientServerHead.h"

ReadWriteLock::ReadWriteLock(const char* semName, const char* mutName) {
    hSemaphore = CreateSemaphore(NULL, 1, 1, semName);
    hMutex = CreateMutex(NULL, FALSE, mutName);
    n = 0;
}

ReadWriteLock::~ReadWriteLock() {
    CloseHandle(hSemaphore);
    CloseHandle(hMutex);
}

void ReadWriteLock::startRead() {
    WaitForSingleObject(hMutex, INFINITE);
    n++;
    if (n == 1)
        WaitForSingleObject(hSemaphore, INFINITE);
    ReleaseMutex(hMutex);
}

void ReadWriteLock::endRead() {
    WaitForSingleObject(hMutex, INFINITE);
    n--;
    if (n == 0)
        ReleaseSemaphore(hSemaphore, 1, NULL);
    ReleaseMutex(hMutex);
}

void ReadWriteLock::startWrite() {
    WaitForSingleObject(hSemaphore, INFINITE);
}

void ReadWriteLock::endWrite() {
    ReleaseSemaphore(hSemaphore, 1, NULL);
}
