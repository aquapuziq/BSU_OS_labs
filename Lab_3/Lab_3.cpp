#include <iostream>
#include <windows.h>

using namespace std;

volatile int dim;
volatile int rem;
int* arr;

HANDLE hStartSignal, hBreakSignal, hContinueSignal;

DWORD WINAPI marker(LPVOID number) {
    WaitForSingleObject(hStartSignal, INFINITE);

    int n = (int)number;
    srand(n);

    int mark = 0;

    while (true) {
        int tmp = rand();
        int index = tmp % dim;
        if (arr[index] == 0) {
            Sleep(5);
            arr[index] = n;
            Sleep(5);
            mark++;
            continue;
        }
        else {
            cout << "Порядковый номер потока: " << n << endl;
            cout << "Количество помеченных элементов: " << mark << endl;
            cout << "Индекс элемента массива, который невозможно пометить: " << index;

            WaitForSingleObject(hBreakSignal, INFINITE);
            WaitForSingleObject(hContinueSignal, INFINITE);
        }
    }

}

int main()
{
    setlocale(LC_ALL, "RU");
    int dimTemp;
    cout << "Введите размерность массива: ";
    cin >> dimTemp;
    dim = dimTemp;

    arr = new int[dim] {};

    int num;
    cout << "Введите количество потоков marker: ";
    cin >> num;

    DWORD* IDMarkers = new DWORD[num];
    HANDLE* hMarkers = new HANDLE[num];

    hStartSignal = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hStartSignal == NULL) {
        return GetLastError();
    }

    hBreakSignal = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hBreakSignal == NULL) {
        return GetLastError();
    }

    hContinueSignal = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hContinueSignal == NULL) {
        return GetLastError();
    }

    for (int i = 0; i < num; i++) {
        hMarkers[i] = CreateThread(NULL, 0, marker, (LPVOID)(i + 1), 0, &IDMarkers[i]);
        if (hMarkers[i] == NULL) {
            cerr << "Не удалось создать поток marker по номеру " << (i + 1) << endl;
            return 1;
        }
    }
}

