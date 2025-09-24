#include "pch.h" 
#include <windows.h>
#include <vector>
#include <iostream>

using namespace std;

extern volatile int* arr;
extern volatile int minElement, maxElement, averageElement;

DWORD WINAPI min_max(LPVOID dim);
DWORD WINAPI average(LPVOID dim);


void runThreadsInitArray(vector<int>& mas) {
    int n = mas.size();
    arr = new int[n];
    for (int i = 0; i < n; i++)
        arr[i] = mas[i];

    HANDLE hMinMax, hAverage;
    DWORD IDMinMax, IDAverage;

    hMinMax = CreateThread(NULL, 0, min_max, (void*)n, 0, &IDMinMax);
    ASSERT_TRUE(hMinMax != NULL);

    hAverage = CreateThread(NULL, 0, average, (void*)n, 0, &IDAverage);
    ASSERT_TRUE(hAverage != NULL);

    WaitForSingleObject(hAverage, INFINITE);
    CloseHandle(hAverage);

    WaitForSingleObject(hMinMax, INFINITE);
    CloseHandle(hMinMax);

    for (int i = 0; i < n; i++) {
        if (arr[i] == maxElement || arr[i] == minElement) {
            arr[i] = averageElement;
        }
        mas[i] = arr[i];
    }

    delete[] arr;
}

TEST(IntegrationThreads, CorrectMinMaxAverage) {
    vector<int> mas = {1, 2, 3, 4, 5};
    runThreadsInitArray(mas);

    EXPECT_EQ(minElement, 1);
    EXPECT_EQ(maxElement, 5);
    EXPECT_EQ(averageElement, (1 + 2 + 3 + 4 + 5) / 5);

    vector<int> expected = {3, 2, 3, 4, 3};
    EXPECT_EQ(mas, expected);
}