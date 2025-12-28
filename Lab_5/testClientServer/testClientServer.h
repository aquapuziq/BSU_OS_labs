#include <iostream>
#pragma once
#include <fstream>
#include "../ClientServerHead/employee.h"

bool findById(int id, Employee& employee, std::fstream& file);
bool writeById(const Employee& employee, std::fstream& file);
