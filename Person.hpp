#pragma once
#include <string>
#include "enums.hpp"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Person
{
private:
    std::string firstname_;
    std::string lastname_;
    std::string address_;
    std::string peselNr_;
    Sex sexType_;
    
public:
    // TEMPLATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T>
    static std::shared_ptr<Person> createPerson(T&& Args) // Person factory function
    {
        return std::make_unique<T>(Args);
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename T,typename T2> // Functions checks if object pointer is specify type of derived class object
    static T2* isTargetClassObject(T* derivedClass)
    {
    T * test = derivedClass;
    T2* cast_test = dynamic_cast<T2*>(test);
    if (cast_test) {return cast_test;};
    return nullptr;
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

    // Getters
    std::string getFirstname() const { return firstname_; };
    std::string getLastname() const { return lastname_; };
    std::string getAddress() const { return address_; };
    std::string getPeselNr() const { return peselNr_; };
    
    
    Sex getSex() const { return sexType_; };
    virtual PersonType getPersonType() const = 0;


    // Setters
    void setFirstname(const std::string& firstname) { firstname_ = firstname; };
    void setLastname(const std::string& lastname) { lastname_ = lastname; };
    void setAddress(const std::string& address) { address_ = address; };
    void setpeselNr(const std::string& peselNr) { peselNr_ = peselNr; };
    std::string sexToString(Sex sexType) { return (sexType == Sex::Male) ? "Male" : "Female"; };

   
    // c - tor
    Person(const std::string &firstname, const std::string &lastname, const std::string &address, const std::string peselNr, const Sex sexType) // c - tor
        : firstname_{firstname}
        , lastname_{lastname}
        , address_{address}
        , peselNr_{peselNr}
        , sexType_{sexType}
        {};
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Student: public Person
{
private:
    int indexNr_;

public:
bool setindexNr(int indexNr) { indexNr_ = indexNr; return true;};
int getIndexNr() const { return indexNr_; };

PersonType getPersonType() const override {return PersonType::Student;};

// c -tor
Student(const std::string &firstname, const std::string &lastname, const std::string &address, const int indexNr, const std::string peselNr, const Sex sexType) // c - tor
        : Person(firstname,lastname,address,peselNr,sexType)
        , indexNr_{indexNr}
     {};
    
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Worker: public Person
{
private:
int Salary_;

public:
PersonType getPersonType() const override {return PersonType::Worker;};
void setSalary(int newSalary) {Salary_=newSalary;};
int getSalary() const {return Salary_;};



// c -tor
Worker(const std::string &firstname, const std::string &lastname, const std::string &address, const std::string peselNr, const Sex sexType, const int salary) // c - tor
        : Person(firstname,lastname,address,peselNr,sexType)
        , Salary_{salary}
     {};

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////