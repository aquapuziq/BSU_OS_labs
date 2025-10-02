#include "pch.h"
#include "CppUnitTest.h"
#include "creating.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
    TEST_CLASS(ArrayTests)
    {
    public:

        TEST_METHOD(TestMinMaxAverage)
        {
            int n = 5;
            int localArr[] = { 1, 2, 3, 4, 5 };

            arr = localArr;

            min_max(n);
            average(n);

            Assert::AreEqual(1, (int)minElement, L"Минимум работает неправильно");
            Assert::AreEqual(5, (int)maxElement, L"Максимум работает неправильно");
            Assert::AreEqual(3, (int)averageElement, L"Среднее работает неправильно");
        }

        TEST_METHOD(TestOneElement)
        {
            int n = 1;
            int localArr[] = {3};

            arr = localArr;

            min_max(n);
            average(n);

            Assert::AreEqual(3, (int)minElement, L"Минимум работает неправильно");
            Assert::AreEqual(3, (int)maxElement, L"Максимум работает неправильно");
            Assert::AreEqual(3, (int)averageElement, L"Среднее работает неправильно");
        }
    };
}
