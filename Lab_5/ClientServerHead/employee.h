#pragma once
#include <windows.h>

struct Employee {
    int num;
    char name[10];
    double hours;
};


extern const char* PIPE;
extern const char* SEMAPHORE;
extern const char* MUTEX;

int findEmployee(Employee* arr, int count, int number);


