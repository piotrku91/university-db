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
    std::vector<std::shared_ptr<Person>> Records_;
    std::map<int,std::shared_ptr<Person>> IndexOfStudentIdxs_;
    std::map<std::string,std::shared_ptr<Person>> IndexOfPersonsPesels_;
    bool peselValidation_;

public: 
    // Setters - main operations
    ErrorCheck addPerson(PersonType type, const std::string& firstname, const std::string& lastname, const std::string& address, const int indexNr, const std::string& peselNr, const Sex sexType, const int Salary=0);
    ErrorCheck addPerson(const std::shared_ptr<Person>& completePerson);
    ErrorCheck checkPeselUnique(const std::string &peselNr, Sex sexType);
    ErrorCheck checkIdxUnique(const int &indexNr);
    ErrorCheck checkIdxAndPeselUnique(const int& indexNr,const std::string& peselNr, Sex sexType);
    bool deleteByIndexNr(const int& indexNr);
    void rebuildIndex(); 
    void eraseDatabase(); // Delete all records and rebuild index

    // Setters - sort operations
    std::vector<std::shared_ptr<Person>> sortByLastNameTemporary(Order O=Order::Asc);
    std::vector<std::shared_ptr<Person>> sortByPeselTemporary(Order O=Order::Asc);
    void sortByLastName(Order O=Order::Asc);
    void sortByPesel(Order O=Order::Asc);
    void sortBySalary(Order O=Order::Asc);

    bool peselValidator(const std::string& peselNr, Sex sexType);
    void enablePeselValidation(bool status=true) {peselValidation_=status;};

    // Save / load functions
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
    // Getters
    std::shared_ptr<Person> findPersonByLastName_Linear(const std::string& lastname);
    std::shared_ptr<Person> findPersonByPesel_Linear(const std::string& peselNr);
    std::shared_ptr<Person> findPersonByPesel_Binary(const std::string& peselNr);
    std::shared_ptr<Person> findPersonByIdx_Binary(const int& indexNr);

    ErrorCheck findPersonAndModifyFirstname(const std::string& peselNr, const std::string& newFirstname);
    ErrorCheck findPersonAndModifyLastname(const std::string& peselNr, const std::string& newLastname);
    ErrorCheck findPersonAndModifyAddress(const std::string& peselNr, const std::string& newAddress);
    ErrorCheck findPersonAndModifyindexNr(const std::string& peselNr, const int& newindexNr);
    ErrorCheck findPersonAndModifypeselNr(const std::string& peselNr, const std::string& newpeselNr);
    ErrorCheck findPersonAndModifySalary(const std::string& peselNr, const int &newSalary);

    size_t getCount() const {return Records_.size();};
    const std::vector<std::shared_ptr<Person>> &getFullList() const { return Records_; };

    // c - tor
    Db(bool loadOnStart=true): peselValidation_{true} {if (loadOnStart) {loadFromFile("data.db");};}; 
    Db(const Db&) = delete; // c - tor deleted
    Db& operator=(const Db&) = delete;
};