#include <gtest/gtest.h>
#include "head.h"
#include "marker.h"
#include <windows.h>
#include <vector>

CRITICAL_SECTION cs;
HANDLE hStartSignal, hContinueSignal, hRemoveEvent;
HANDLE* hFinishEvents;
int* arr;
volatile int dim;
volatile int rem;

TEST(MarkerTest, SingleMarkerMarksArrayAndClearsOnRemove) {
    dim = 10;
    arr = new int[dim] {};

    InitializeCriticalSection(&cs);

    hStartSignal = CreateEvent(NULL, TRUE, FALSE, NULL);
    hContinueSignal = CreateEvent(NULL, TRUE, FALSE, NULL);
    hRemoveEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    hFinishEvents = new HANDLE[1];
    hFinishEvents[0] = CreateEvent(NULL, TRUE, FALSE, NULL);

    DWORD id;
    HANDLE hMarker = CreateThread(NULL, 0, marker, (LPVOID)1, 0, &id);

    SetEvent(hStartSignal);

    Sleep(50);

    bool marked = false;
    EnterCriticalSection(&cs);
    for (int i = 0; i < dim; i++) {
        if (arr[i] == 1) {
            marked = true;
            break;
        }
    }
    LeaveCriticalSection(&cs);

    ASSERT_TRUE(marked) << "Marker должен был пометить хот€ бы один элемент массива";

    rem = 1;
    PulseEvent(hRemoveEvent);
    WaitForSingleObject(hMarker, INFINITE);

    EnterCriticalSection(&cs);
    for (int i = 0; i < dim; i++) {
        ASSERT_EQ(arr[i], 0);
    }
    LeaveCriticalSection(&cs);

    CloseHandle(hMarker);
    CloseHandle(hFinishEvents[0]);
    CloseHandle(hStartSignal);
    CloseHandle(hContinueSignal);
    CloseHandle(hRemoveEvent);
    DeleteCriticalSection(&cs);
    delete[] arr;
    delete[] hFinishEvents;
}

