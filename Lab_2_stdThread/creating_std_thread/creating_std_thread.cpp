#include <thread>
#include <iostream>
#include "creating.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "ru");
    
	int n; int tmp;
	cout << "Введите размерность массива: ";
	if (!(cin >> n) || n <= 0) {
		cerr << "Неверный размер массива\n";
		return 1;
	}


	arr = new int[n];
	cout << "Введите элементы массива: ";
	for (int i = 0; i < n; ++i) {
		cin >> tmp;
		arr[i] = tmp;
	}
	cout << endl;


	thread minMax(min_max, n);
	if (!minMax.joinable()) {
		cerr << "Не удалось создать поток min_max\n";
		delete[] arr;
		return 1;
	}

	thread averageTh(average, n);
	if (!averageTh.joinable()) {
		cerr << "Не удалось создать поток min_max\n";
		delete[] arr;
		return 1;
	}

	minMax.join();
	averageTh.join();

	cout << "\n\nМассив после форматирования: ";
	for (int i = 0; i < n; ++i) {
		if (arr[i] == maxElement || arr[i] == minElement) {
			arr[i] = averageElement;
		}
		cout << arr[i] << " ";
	}
	cout << '\n';
}
