#include <iostream>
#include <windows.h>

using namespace std;

DWORD WINAPI min_max(LPVOID element) {}

DWORD WINAPI average(LPVOID element) {}

int main()
{
    setlocale(LC_ALL, "ru");
    int n; int a;
    cout << "Введите размерность массива: ";
    cin >> n;
    int* arr = new int[n];

    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    
    HANDLE hMinMax, hAverage;
    DWORD IDMinMax, IDAverage;


    hMinMax = CreateThread(NULL, 0, min_max, (void*)a, 0, &IDMinMax);
    if (hMinMax == NULL)
        return GetLastError();
    WaitForSingleObject(hMinMax, INFINITE);
    CloseHandle(hMinMax);

    hAverage = CreateThread(NULL, 0, average, (void*)a, 0, &IDAverage);
    if (hAverage == NULL)
        return GetLastError();

    WaitForSingleObject(hAverage, INFINITE);
    CloseHandle(hAverage);

    WaitForSingleObject(hMinMax, INFINITE);
    CloseHandle(hMinMax);

    /*cout << "Массив после форматирования: ";
    for (int i = 0; i < n; i++) {
        if (a[i] == max || a[i] == min) {
            a[i] = average;
        }
        cout << arr[i] << " ";
    }*/
}