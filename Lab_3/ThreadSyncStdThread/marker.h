#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>

using namespace std;

struct MarkerInfo {
    int id;
    vector<int> marks;
    int markCount = 0;
};

extern mutex cs;
extern condition_variable cvStart, cvFinish, cvRemove, cvContinue;
extern bool startSignal, removeSignal, continueSignal;
extern int removeId;
extern vector<int> arr;
extern atomic<int> dim;
extern atomic<int> remainingThreads;

void markerThread(MarkerInfo* info);