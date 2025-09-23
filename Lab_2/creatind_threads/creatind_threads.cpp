#include <iostream>
#include <windows.h>

using namespace std;

volatile int* arr;
volatile int minElement, maxElement, averageElement;

DWORD WINAPI min_max(LPVOID dim) {
    int n = (int)dim;
    minElement = arr[0];
    maxElement = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > maxElement) {
            maxElement = arr[i];
            Sleep(7);
        }

        if (arr[i] < minElement) {
            minElement = arr[i];
            Sleep(7);
        }
    }

    cout << "Минимальный элемент массива: " << minElement << endl;
    cout << "Максимальный элемент массива: " << maxElement << endl;
    
    return 1;
}

DWORD WINAPI average(LPVOID dim) {
    int n = (int)dim;
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
        Sleep(12);
    }

    averageElement = sum / n;
    cout << "Среднее арифметическое элементов массива: " << averageElement;

    return 1;
}

int main()
{
    setlocale(LC_ALL, "ru");
    int n; int element;
    cout << "Введите размерность массива: ";
    cin >> n;
    arr = new int[n];

    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        cin >> element;
        arr[i] = element;
    }

    cout << endl;
    
    HANDLE hMinMax, hAverage;
    DWORD IDMinMax, IDAverage;


    hMinMax = CreateThread(NULL, 0, min_max, (void*)n, 0, &IDMinMax);
    if (hMinMax == NULL)
        return GetLastError();

    hAverage = CreateThread(NULL, 0, average, (void*)n, 0, &IDAverage);
    if (hAverage == NULL)
        return GetLastError();

    WaitForSingleObject(hAverage, INFINITE);
    CloseHandle(hAverage);

    WaitForSingleObject(hMinMax, INFINITE);
    CloseHandle(hMinMax);

    cout << "\n\nМассив после форматирования: ";
    for (int i = 0; i < n; i++) {
        if (arr[i] == maxElement || arr[i] == minElement) {
            arr[i] = averageElement;
        }
        cout << arr[i] << " ";
    }

    return 0;
}