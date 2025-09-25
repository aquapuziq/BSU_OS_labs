#include "pch.h" 
#include <windows.h>
#include <vector>
#include <iostream>
#include "C:\dev\projects\msVS\OS_labs\Lab_2\creatind_threads\creating_threads.h"

using namespace std;

DWORD WINAPI min_max(LPVOID dim);
DWORD WINAPI average(LPVOID dim);

static void runThreadsInitArray(vector<int>& mas) {
	int n = static_cast<int>(mas.size());
	arr = new int[n];
	for (int i = 0; i < n; ++i) arr[i] = mas[i];


	HANDLE hMinMax = CreateThread(NULL, 0, min_max, (void*)n, 0, NULL);
	if (hMinMax == NULL) {
		delete[] arr; arr = nullptr;
		throw runtime_error("CreateThread failed for min_max");
	}


	HANDLE hAverage = CreateThread(NULL, 0, average, (void*)n, 0, NULL);
	if (hAverage == NULL) {
		CloseHandle(hMinMax);
		delete[] arr; arr = nullptr;
		throw runtime_error("CreateThread failed for average");
	}


	WaitForSingleObject(hAverage, INFINITE);
	CloseHandle(hAverage);
	WaitForSingleObject(hMinMax, INFINITE);
	CloseHandle(hMinMax);


	for (int i = 0; i < n; ++i) {
		if (arr[i] == maxElement || arr[i] == minElement) {
			arr[i] = averageElement;
		}
		mas[i] = arr[i];
	}


	delete[] arr; arr = nullptr;
}


TEST(IntegrationThreads, CorrectMinMaxAverage) {
	vector<int> mas = { 1,2,3,4,5 };
	ASSERT_NO_THROW(runThreadsInitArray(mas));


	EXPECT_EQ(minElement, 1);
	EXPECT_EQ(maxElement, 5);
	EXPECT_EQ(averageElement, (1 + 2 + 3 + 4 + 5) / 5);

	vector<int> expected = { 3,2,3,4,3 };
	EXPECT_EQ(mas, expected);
}