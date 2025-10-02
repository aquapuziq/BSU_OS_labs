#pragma once
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
            this_thread::sleep_for(chrono::milliseconds(7));
        }

        if (arr[i] < minElement) {
            minElement = arr[i];
            this_thread::sleep_for(chrono::milliseconds(7));
        }
    }
    cout << "Минимальный элемент массива: " << minElement << endl;
    cout << "Максимальный элемент массива: " << maxElement << endl;
}

void average(int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        this_thread::sleep_for(chrono::milliseconds(12));
    }

    averageElement = sum / n;
    cout << "Среднее арифметическое элементов массива: " << averageElement;
}