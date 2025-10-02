#pragma once
#define ENABLE_SLEEP
#include <iostream>
#include <thread>

using namespace std;

volatile int* arr;
volatile int minElement, maxElement, averageElement;

void min_max(int n) {
    minElement = arr[0];
    maxElement = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxElement) {
            maxElement = arr[i];
#ifdef ENABLE_SLEEP
            this_thread::sleep_for(chrono::milliseconds(7));
#endif
        }

        if (arr[i] < minElement) {
            minElement = arr[i];
#ifdef ENABLE_SLEEP
            this_thread::sleep_for(chrono::milliseconds(7));
#endif
        }
    }
    cout << "Минимальный элемент массива: " << minElement << endl;
    cout << "Максимальный элемент массива: " << maxElement << endl;
}

void average(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
#ifdef ENABLE_SLEEP
        this_thread::sleep_for(chrono::milliseconds(12));
#endif
    }

    averageElement = sum / n;
    cout << "Среднее арифметическое элементов массива: " << averageElement;
}