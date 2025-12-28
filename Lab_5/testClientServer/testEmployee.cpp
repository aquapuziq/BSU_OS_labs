#include <boost/test/unit_test.hpp> 
#include "../ClientServerHead/employee.h"

BOOST_AUTO_TEST_CASE(testEmployeeFound) {
    Employee arr[3] = {
        {1, "A", 10.5},
        {2, "B", 11},
        {3, "C", 12}
    };

    int pos = findEmployee(arr, 3, 2);
    BOOST_CHECK_EQUAL(pos, 1);
}

BOOST_AUTO_TEST_CASE(tesEmployeeNFound) {
    Employee arr[2] = {
        {1, "A", 10},
        {2, "B", 11}
    };

    int pos = findEmployee(arr, 2, 3);
    BOOST_CHECK_EQUAL(pos, -1);
}
