#include <iostream>
#include <windows.h>

using namespace std;

volatile int dim;
volatile int rem = 0; 
int* arr;

CRITICAL_SECTION cs;
HANDLE hStartSignal, hContinueSignal, hRemoveEvent;
HANDLE* hTerminateSignals;
HANDLE* hFinishEvents;

DWORD WINAPI marker(LPVOID number) {
    WaitForSingleObject(hStartSignal, INFINITE);

    int n = (int)number;
    srand(n);

    int mark = 0;
    int* marks = new int[dim] {};

    while (true) {
        int tmp = rand();
        int index = tmp % dim;

        EnterCriticalSection(&cs);
        if (arr[index] == 0) {
            Sleep(5);
            arr[index] = n;
            marks[index] = 1;
            mark++;
            Sleep(5);
            LeaveCriticalSection(&cs);
        }
        else {
            cout << "Порядковый номер потока: " << n << endl;
            cout << "Количество помеченных элементов: " << mark << endl;
            cout << "Индекс элемента массива, который невозможно пометить: " << index << endl;
            cout << endl;
            LeaveCriticalSection(&cs);

            SetEvent(hFinishEvents[n - 1]);

            WaitForSingleObject(hRemoveEvent, INFINITE);

            if (rem == n) {
                EnterCriticalSection(&cs);
                for (int i = 0; i < dim; i++) {
                    if (marks[i] == 1) {
                        arr[i] = 0;
                    }
                }
                LeaveCriticalSection(&cs);
                delete[] marks;
                SetEvent(hFinishEvents[n - 1]); 
                return 0;
            }
            else {
                ResetEvent(hFinishEvents[n - 1]);
                WaitForSingleObject(hContinueSignal, INFINITE);
            }
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

    InitializeCriticalSection(&cs);

    DWORD* IDMarkers = new DWORD[num];
    HANDLE* hMarkers = new HANDLE[num];
    hTerminateSignals = new HANDLE[num];
    hFinishEvents = new HANDLE[num];

    hStartSignal = CreateEvent(NULL, TRUE, FALSE, NULL);
    hContinueSignal = CreateEvent(NULL, TRUE, FALSE, NULL); 
    hRemoveEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 

    for (int i = 0; i < num; i++) {
        hMarkers[i] = CreateThread(NULL, 0, marker, (LPVOID)(i + 1), 0, &IDMarkers[i]);
        if (hMarkers[i] == NULL) {
            cerr << "Не удалось создать поток marker по номеру " << (i + 1) << endl;
            return 1;
        }

        hTerminateSignals[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        hFinishEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
    }

    SetEvent(hStartSignal);

    int iteration = num;
    while (iteration > 0) {
        WaitForMultipleObjects(num, hFinishEvents, TRUE, INFINITE);

        EnterCriticalSection(&cs);
        cout << "\nНа данный момент массив имеет вид:\n";
        for (int i = 0; i < dim; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
        LeaveCriticalSection(&cs);

        cout << "Введите порядковый номер потока marker, который необходимо завершить: ";
        int delThread;
        cin >> delThread;

        if (delThread < 1 || delThread > num || hMarkers[delThread - 1] == NULL) {
            cout << "Некорректный номер потока. Повторите попытку.\n";
            SetEvent(hContinueSignal);
            continue;
        }

        rem = delThread;
        PulseEvent(hRemoveEvent);
        WaitForSingleObject(hMarkers[delThread - 1], INFINITE);
        CloseHandle(hMarkers[delThread - 1]);
        hMarkers[delThread - 1] = NULL;

        iteration--;

        EnterCriticalSection(&cs);
        cout << "Поток " << delThread << " завершён. Текущее состояние массива:\n";
        for (int i = 0; i < dim; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;
        LeaveCriticalSection(&cs);

        ResetEvent(hRemoveEvent);
        PulseEvent(hContinueSignal); 
    }

    cout << "\nВсе потоки завершены.\nЗаключительное состояние массива:\n";
    for (int i = 0; i < dim; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < num; i++) {
        if (hMarkers[i]) CloseHandle(hMarkers[i]);
        CloseHandle(hTerminateSignals[i]);
        CloseHandle(hFinishEvents[i]);
    }

    CloseHandle(hStartSignal);
    CloseHandle(hContinueSignal);
    CloseHandle(hRemoveEvent);
    DeleteCriticalSection(&cs);

    return 0;
}
