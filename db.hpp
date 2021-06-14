#pragma once
#include <memory>
#include <vector>

#include "Student.hpp"

class db
{
private:
    std::vector<std::unique_ptr<Student>> Students;
    std::unique_ptr<Student>& getStudent(size_t idx) {return Students[idx];};

public: // Setters / main operations
bool addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType);

// Getters

const std::vector<std::unique_ptr<Student>>& getFullList() {return Students;};


};