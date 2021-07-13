#pragma once
#include <array>
#include <fstream>
#include <map>
#include <memory>
#include <vector>

#include "Person.hpp"
#include "enums.hpp"

class Db
{
private:
    std::vector<std::unique_ptr<Person>> Records_;
    std::map<int,std::unique_ptr<Person>*> IndexOfStudentIdxs_;
    std::map<std::string,std::unique_ptr<Person>*> IndexOfPersonsPesels_;
    const std::unique_ptr<Person>& getStudent(std::unique_ptr<Person>* student_ptr) { return *student_ptr;};
    bool peselValidation_;

public: 
    // Setters - main operations
    ErrorCheck addPerson(PersonType type, const std::string& firstname, const std::string& lastname, const std::string& address, const int indexNr, const std::string& peselNr, const Sex sexType, const int Salary=0);
    ErrorCheck checkPeselUnique(const std::string &peselNr, Sex sexType);
    ErrorCheck checkIdxUnique(const int &indexNr);
    ErrorCheck checkIdxAndPeselUnique(const int& indexNr,const std::string& peselNr, Sex sexType);
    bool deleteByIndexNr(const int& indexNr);
    void rebuildIndex(); 
    void eraseDatabase(); // Delete all records and rebuild index

    // Setters - sort operations
    std::vector<std::unique_ptr<Person>*> sortByLastNameTemporary(Order O=Order::Asc);
    std::vector<std::unique_ptr<Person>*> sortByPeselTemporary(Order O=Order::Asc);
    void sortByLastName(Order O=Order::Asc);
    void sortByPesel(Order O=Order::Asc);
    void sortBySalary(Order O=Order::Asc);

    bool peselValidator(const std::string& peselNr, Sex sexType);
    void enablePeselValidation(bool status=true) {peselValidation_=status;};

    // Save / load functions
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
    // Getters
    std::unique_ptr<Person>* findPersonByLastName_Linear(const std::string& lastname);
    std::unique_ptr<Person>* findPersonByPesel_Linear(const std::string& peselNr);
    std::unique_ptr<Person>* findPersonByPesel_Binary(const std::string& peselNr);
    std::unique_ptr<Person>* findPersonByIdx_Binary(const int& indexNr);

    ErrorCheck findPersonAndModifyFirstname(const std::string& peselNr, const std::string& newFirstname);
    ErrorCheck findPersonAndModifyLastname(const std::string& peselNr, const std::string& newLastname);
    ErrorCheck findPersonAndModifyAddress(const std::string& peselNr, const std::string& newAddress);
    ErrorCheck findPersonAndModifyindexNr(const std::string& peselNr, const int& newindexNr);
    ErrorCheck findPersonAndModifypeselNr(const std::string& peselNr, const std::string& newpeselNr);

    size_t getCount() const {return Records_.size();};
    const std::vector<std::unique_ptr<Person>> &getFullList() const { return Records_; };

    // c - tor
    Db(bool loadOnStart=true): peselValidation_{true} {if (loadOnStart) {loadFromFile("data.db");};}; 
    Db(Db&) = delete; // c - tor deleted
};