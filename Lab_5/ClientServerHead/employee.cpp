#include "employee.h"

const char* PIPE = "\\\\.\\pipe\\server_pipe";
const char* SEMAPHORE = "WriteSemaphore";
const char* MUTEX = "SyncMutex";

int findEmployee(Employee* arr, int count, int number) {
    for (int i = 0; i < count; i++)
        if (arr[i].num == number)
            return i;
    return -1;
}
