#define BOOST_TEST_MODULE EmployeeTests
#include <boost/test/included/unit_test.hpp>
#include "../ClientServerHead/employee.h"

BOOST_AUTO_TEST_CASE(test_find_employee_found) {
    Employee arr[3] = {
        {1, "A", 10.5},
        {2, "B", 11},
        {3, "C", 12}
    };

    int pos = findEmployee(arr, 3, 2);
    BOOST_CHECK_EQUAL(pos, 1);
}

BOOST_AUTO_TEST_CASE(test_find_employee_not_found) {
    Employee arr[2] = {
        {1, "A", 10},
        {2, "B", 11}
    };

    int pos = findEmployee(arr, 2, 3);
    BOOST_CHECK_EQUAL(pos, -1);
}
