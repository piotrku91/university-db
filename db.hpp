#pragma once
#include <array>
#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include "Student.hpp"
#include "enums.hpp"

class db
{
private:
    std::vector<std::unique_ptr<Student>> Students_;
    std::map<int,std::unique_ptr<Student>*> IndexOfStudentIdxs_;
    std::map<long int,std::unique_ptr<Student>*> IndexOfStudentPesels_;
    const std::unique_ptr<Student>& getStudent(std::unique_ptr<Student>* StudentPtr) { return *StudentPtr;};
    bool PeselValidation_;

public: // Setters / main operations
    ErrorCheck addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType);
    ErrorCheck checkIdxAndPeselUnique(const int& IdxNr,const long int& PeselNr, Sex sexType);
    bool deleteByIndexNr(const int &IdxNr);
    bool peselValidator(const long int& PeselNr, Sex sexType); // TO IMPLEMENT - for now pass everything
    std::vector<std::unique_ptr<Student>*> sortByLastNameTemporary(Order O=Order::Asc);
    std::vector<std::unique_ptr<Student>*> sortByPeselTemporary(Order O=Order::Asc);
    void sortByLastName(Order O=Order::Asc);
    void sortByPesel(Order O=Order::Asc);
    void rebuildIndex();
    void eraseDatabase();
    void enablePeselValidation(bool Status=true) {PeselValidation_=Status;};
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
    // Getters
    std::unique_ptr<Student>* findStudentByLastName_Linear(const std::string &lastName);
    std::unique_ptr<Student>* findStudentByPesel_Linear(const long int &PeselNr);
    std::unique_ptr<Student>* findStudentByPesel_Binary(const long int &PeselNr);
    std::unique_ptr<Student>* findStudentByIdx_Binary(const long int &IdxNr);

    ErrorCheck findStudentAndModifyFirstname(const int &IdxNr, const std::string &newFirstname);
    ErrorCheck findStudentAndModifyLastname(const int &IdxNr, const std::string &newLastname);
    ErrorCheck findStudentAndModifyAddress(const int &IdxNr, const std::string &newAddress);
    ErrorCheck findStudentAndModifyIndexNr(const int &IdxNr, const int &newIndexNr);
    ErrorCheck findStudentAndModifyPeselNr(const int &IdxNr, const long int &newPeselNr);

    size_t getCount() const {return Students_.size();};
    const std::vector<std::unique_ptr<Student>> &getFullList() const { return Students_; };

    db(bool loadOnStart=true): PeselValidation_{false} {if (loadOnStart) {loadFromFile("data.db");};}; // c - tor
    db(db&) = delete; // c - tor deleted
};