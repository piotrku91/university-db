#pragma once
#include <memory>
#include <vector>

#include "Student.hpp"

enum class ErrorCheck
{
IndexInUse,
PeselInUse,
PeselInvalid,
OK,
UnknownError
};

enum class Order 
{
Asc,
Desc
};

class db
{
private:
    std::vector<std::unique_ptr<Student>> Students;
    std::unique_ptr<Student> &getStudent(size_t idx) { return Students[idx]; };

public: // Setters / main operations
    ErrorCheck addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType);
    ErrorCheck checkIdxAndPeselUnique(const int& IdxNr,const long int& PeselNr);
    std::unique_ptr<Student>& findStudentByLastName(const std::string &lastName);
    std::unique_ptr<Student>& findStudentByPesel(const long int &PeselNr);
    bool deleteByIndexNr(const int &IdxNr);
    void sortByLastName(Order O=Order::Asc);
    void sortByPesel(Order O=Order::Asc);

    // Getters

    const std::vector<std::unique_ptr<Student>> &getFullList() { return Students; };
};