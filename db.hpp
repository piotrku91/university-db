#pragma once
#include <memory>
#include <vector>

#include "Student.hpp"

class db
{
private:
    std::vector<std::unique_ptr<Student>> Students;
    std::unique_ptr<Student> &getStudent(size_t idx) { return Students[idx]; };

public: // Setters / main operations
    bool addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType);
    std::unique_ptr<Student> &findStudentByLastName(const std::string &lastName);
    std::unique_ptr<Student> &findStudentByPesel(const long int &PeselNr);
    bool deleteByIndexNr(const int &IdxNr);
    bool checkIdxUnique(const int& IdxNr);
    // Getters

    const std::vector<std::unique_ptr<Student>> &getFullList() { return Students; };
};