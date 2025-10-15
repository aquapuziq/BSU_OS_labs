#include <iostream>
#include <windows.h>

using namespace std;

volatile int dim;
int* arr;

CRITICAL_SECTION cs;
HANDLE hStartSignal, hBreakSignal, hContinueSignal;
HANDLE* hTerminateSignals;

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
            continue;
        }
        else {
            cout << "Порядковый номер потока: " << n << endl;
            cout << "Количество помеченных элементов: " << mark << endl;
            cout << "Индекс элемента массива, который невозможно пометить: " << index;

            LeaveCriticalSection(&cs);
            SetEvent(hBreakSignal);

            HANDLE hWait[2] = { hContinueSignal, hTerminateSignals[n - 1] };
            DWORD res = WaitForMultipleObjects(2, hWait, FALSE, INFINITE);

            if (res == WAIT_OBJECT_0 + 1) {
                EnterCriticalSection(&cs);
                for (int i = 0; i < dim; i++) {
                    if (marks[i] == 1) {
                        arr[i] = 0;
                    }
                }
                LeaveCriticalSection(&cs);
                delete[] marks;
                return 0;
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

    hStartSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hStartSignal == NULL) {
        return GetLastError();
    }

    hBreakSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hBreakSignal == NULL) {
        return GetLastError();
    }

    hContinueSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hContinueSignal == NULL) {
        return GetLastError();
    }

    for (int i = 0; i < num; i++) {
        hMarkers[i] = CreateThread(NULL, 0, marker, (LPVOID)(i + 1), 0, &IDMarkers[i]);
        if (hMarkers[i] == NULL) {
            cerr << "Не удалось создать поток marker по номеру " << (i + 1) << endl;
            return 1;
        }

        hTerminateSignals[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (hTerminateSignals[i] == NULL) {
            cerr << "Не удалось создать событие завершения для маркера " << (i + 1) << endl;
            return 1;
        }
    }

    SetEvent(hStartSignal);

    int iteration = num;
    while (iteration > 0) {
        WaitForSingleObject(hBreakSignal, INFINITE);
        EnterCriticalSection(&cs);

        cout << "На данный момент массив имеет вид: " << endl;
        for (int i = 0; i < dim; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;

        LeaveCriticalSection(&cs);

        cout << "Введите порядковый номер потока marker, который необходимо завершить: ";
        int delThread;
        cin >> delThread;

        if (delThread < 1 || delThread > num || hMarkers[delThread - 1] == NULL) {
            cout << "Некорректный номер потока, пожалуйста, попробуй ёщё раз" << endl;
            SetEvent(hContinueSignal);
            continue;
        }
            
        SetEvent(hTerminateSignals[delThread - 1]);

        WaitForSingleObject(hMarkers[delThread - 1], INFINITE);
        CloseHandle(hMarkers[delThread - 1]);
        hMarkers[delThread - 1] = NULL;

        iteration--;

        EnterCriticalSection(&cs);

        cout << "Поток " << delThread << " завершён, текущее состояние массива:" << endl;
        for (int i = 0; i < dim; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;

        LeaveCriticalSection(&cs);

        SetEvent(hContinueSignal);
    }

    cout << "Все потоки завершены";
    cout << "Заключительное состояние массива:" << endl;

    for (int i = 0; i < dim; i++) {
        cout << arr[i] << " ";
    }

    for (int i = 0; i < num; i++) {
        CloseHandle(hMarkers[i]);
    }

    CloseHandle(hStartSignal);
    CloseHandle(hBreakSignal);
    CloseHandle(hContinueSignal);
    DeleteCriticalSection(&cs);

    return 0;
}

