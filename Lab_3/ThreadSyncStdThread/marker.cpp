#include "marker.h"
#include <iostream>
#include <chrono>
#include <cstdlib>

using namespace std;

mutex cs;
condition_variable cvStart;
condition_variable cvRemove;
condition_variable cvFinish;

bool startSignal = false;
bool removeSignal = false;
bool continueSignal = false;
int removeId = -1;

vector<int> arr;
atomic<int> dim{ 0 };
atomic<int> remainingThreads{ 0 };

void markerThread(MarkerInfo* info) {
    {
        unique_lock<mutex> lock(cs);
        cvStart.wait(lock, [] { return startSignal; });
    }

    srand(info->id);
    int markCount = 0;
    info->marks.assign(static_cast<size_t>(dim.load()), 0);

    while (true) {
        int index = rand() % dim;
        {
            unique_lock<mutex> lock(cs);
            if (arr[index] == 0) {
                this_thread::sleep_for(chrono::milliseconds(5));
                arr[index] = info->id;
                info->marks[index] = 1;
                markCount++;
                this_thread::sleep_for(chrono::milliseconds(5));
                continue;
            }
            else {
                cout << "Порядковый номер потока: " << info->id << endl;
                cout << "Количество помеченных элементов: " << info->markCount << endl;
                cout << "Индекс элемента массива, который невозможно пометить: " << index << endl;
                cout << endl;

                remainingThreads--;
            }
        }
        cvFinish.notify_all();

        {
            unique_lock<mutex> lock(cs);
            cvRemove.wait(lock, [info] {
                return (removeSignal && removeId == info->id) || continueSignal;
                });

            if (removeSignal && removeId == info->id) {
                for (int i = 0; i < dim; ++i) {
                    if (info->marks[i] == 1) arr[i] = 0;
                }
                return;
            }
            else if (continueSignal) {
                removeSignal = false;
                continueSignal = false;
                remainingThreads++;
            }
        }
    }
}

