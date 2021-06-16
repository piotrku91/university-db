#pragma once
#include <fstream>
#include <map>
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
    std::vector<std::unique_ptr<Student>> Students_;
    std::map<int,std::unique_ptr<Student>*> IndexOfStudentIdxs_;
    std::map<long int,std::unique_ptr<Student>*> IndexOfStudentPesels_;
    std::unique_ptr<Student>& getStudent(std::unique_ptr<Student>* StudentPtr) { return *StudentPtr;};

public: // Setters / main operations
    ErrorCheck addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType);
    ErrorCheck checkIdxAndPeselUnique(const int& IdxNr,const long int& PeselNr);
    bool deleteByIndexNr(const int &IdxNr);
    bool peselValidator(const long int& PeselNr) {return true;}; // TO IMPLEMENT - for now pass everything
    void sortByLastName(Order O=Order::Asc);
    void sortByPesel(Order O=Order::Asc);
    std::vector<std::unique_ptr<Student>*> sortByLastNameTemporary(Order O=Order::Asc);
    std::vector<std::unique_ptr<Student>*> sortByPeselTemporary(Order O=Order::Asc);
    void rebuildIndex();
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);

    // Getters
    std::unique_ptr<Student>* findStudentByLastName_Linear(const std::string &lastName);
    std::unique_ptr<Student>* findStudentByPesel_Linear(const long int &PeselNr);
    std::unique_ptr<Student>* findStudentByPesel_Binary(const long int &PeselNr);
    std::unique_ptr<Student>* findStudentByIdx_Binary(const long int &IdxNr);
    size_t getCount() const {return Students_.size();};
    const std::vector<std::unique_ptr<Student>> &getFullList() const { return Students_; };
};