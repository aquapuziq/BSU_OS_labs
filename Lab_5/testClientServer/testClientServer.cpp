#include "testClientServer.h"
#include "../ClientServerHead/employee.h"

bool findById(int id, Employee& employee, std::fstream& file) {
    file.clear();
    file.seekg(0, std::ios::beg);

    while (file.read((char*)&employee, sizeof(Employee))) {
        if (employee.num == id) return true;
    }
    return false;
}

bool writeById(const Employee& employee, std::fstream& file) {
    file.clear();
    file.seekg(0, std::ios::beg);

    Employee cur;
    while (file.read((char*)&cur, sizeof(Employee))) {
        if (cur.num == employee.num) {
            file.clear();
            file.seekp((int)file.tellg() - sizeof(Employee));
            file.write((char*)&employee, sizeof(Employee));
            file.flush();
            return true;
        }
    }
    return false;
}
