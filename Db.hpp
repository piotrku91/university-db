#pragma once
#include <array>
#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include "Student.hpp"
#include "enums.hpp"

class Db
{
private:
    std::vector<std::unique_ptr<Student>> Students_;
    std::map<int,std::unique_ptr<Student>*> IndexOfStudentIdxs_;
    std::map<std::string,std::unique_ptr<Student>*> IndexOfStudentPesels_;
    const std::unique_ptr<Student>& getStudent(std::unique_ptr<Student>* student_ptr) { return *student_ptr;};
    bool peselValidation_;

public: // Setters / main operations
    ErrorCheck addStudent(const std::string& firstname, const std::string& lastname, const std::string& address, const int indexNr, const std::string& peselNr, const Sex sexType);
    ErrorCheck checkPeselUnique(const std::string &peselNr, Sex sexType);
    ErrorCheck checkIdxUnique(const int &indexNr);
    ErrorCheck checkIdxAndPeselUnique(const int& indexNr,const std::string& peselNr, Sex sexType);
    bool deleteByIndexNr(const int& indexNr);
    bool peselValidator(const std::string& peselNr, Sex sexType); 
    std::vector<std::unique_ptr<Student>*> sortByLastNameTemporary(Order O=Order::Asc);
    std::vector<std::unique_ptr<Student>*> sortByPeselTemporary(Order O=Order::Asc);
    void sortByLastName(Order O=Order::Asc);
    void sortByPesel(Order O=Order::Asc);
    void rebuildIndex();
    void eraseDatabase(); // Delete all records and rebuild index
    void enablePeselValidation(bool status=true) {peselValidation_=status;};
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
    // Getters
    std::unique_ptr<Student>* findStudentByLastName_Linear(const std::string& lastname);
    std::unique_ptr<Student>* findStudentByPesel_Linear(const std::string& peselNr);
    std::unique_ptr<Student>* findStudentByPesel_Binary(const std::string& peselNr);
    std::unique_ptr<Student>* findStudentByIdx_Binary(const int& indexNr);

    ErrorCheck findStudentAndModifyFirstname(const int &indexNr, const std::string& newFirstname);
    ErrorCheck findStudentAndModifyLastname(const int &indexNr, const std::string& newLastname);
    ErrorCheck findStudentAndModifyAddress(const int &indexNr, const std::string& newAddress);
    ErrorCheck findStudentAndModifyindexNr(const int &indexNr, const int& newindexNr);
    ErrorCheck findStudentAndModifypeselNr(const int &indexNr, const std::string& newpeselNr);

    size_t getCount() const {return Students_.size();};
    const std::vector<std::unique_ptr<Student>> &getFullList() const { return Students_; };

    Db(bool loadOnStart=true): peselValidation_{true} {if (loadOnStart) {loadFromFile("data.db");};}; // c - tor
    Db(Db&) = delete; // c - tor deleted
};