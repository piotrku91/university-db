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
    std::vector<std::unique_ptr<Person>> Students_;
    std::map<int,std::unique_ptr<Person>*> IndexOfStudentIdxs_;
    std::map<std::string,std::unique_ptr<Person>*> IndexOfStudentPesels_;
    const std::unique_ptr<Person>& getStudent(std::unique_ptr<Person>* student_ptr) { return *student_ptr;};
    bool peselValidation_;

public: // Setters / main operations
    ErrorCheck addPerson(PersonType type, const std::string& firstname, const std::string& lastname, const std::string& address, const int indexNr, const std::string& peselNr, const Sex sexType, const int Salary=0);
    ErrorCheck checkPeselUnique(const std::string &peselNr, Sex sexType);
    ErrorCheck checkIdxUnique(const int &indexNr);
    ErrorCheck checkIdxAndPeselUnique(const int& indexNr,const std::string& peselNr, Sex sexType);
    bool deleteByIndexNr(const int& indexNr);
    bool peselValidator(const std::string& peselNr, Sex sexType); 
    std::vector<std::unique_ptr<Person>*> sortByLastNameTemporary(Order O=Order::Asc);
    std::vector<std::unique_ptr<Person>*> sortByPeselTemporary(Order O=Order::Asc);
    void sortByLastName(Order O=Order::Asc);
    void sortByPesel(Order O=Order::Asc);
    void rebuildIndex();
    void eraseDatabase(); // Delete all records and rebuild index
    void enablePeselValidation(bool status=true) {peselValidation_=status;};
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);
    
    // Getters
    std::unique_ptr<Person>* findStudentByLastName_Linear(const std::string& lastname);
    std::unique_ptr<Person>* findStudentByPesel_Linear(const std::string& peselNr);
    std::unique_ptr<Person>* findStudentByPesel_Binary(const std::string& peselNr);
    std::unique_ptr<Person>* findStudentByIdx_Binary(const int& indexNr);

    ErrorCheck findStudentAndModifyFirstname(const int &indexNr, const std::string& newFirstname);
    ErrorCheck findStudentAndModifyLastname(const int &indexNr, const std::string& newLastname);
    ErrorCheck findStudentAndModifyAddress(const int &indexNr, const std::string& newAddress);
    ErrorCheck findStudentAndModifyindexNr(const int &indexNr, const int& newindexNr);
    ErrorCheck findStudentAndModifypeselNr(const int &indexNr, const std::string& newpeselNr);

    size_t getCount() const {return Students_.size();};
    const std::vector<std::unique_ptr<Person>> &getFullList() const { return Students_; };

    Db(bool loadOnStart=true): peselValidation_{true} {if (loadOnStart) {loadFromFile("data.db");};}; // c - tor
    Db(Db&) = delete; // c - tor deleted




//////////////////////////////////////// TEMPLATE FUNCTIONS ///////////////////////////////////////////////




};