#pragma once
#include <string>
#include "enums.hpp"

class Student
{
private:
    std::string firstname_;
    std::string lastname_;
    std::string address_;
    int indexNr_;
    std::string peselNr_;
    Sex sexType_;

public:
    // Getters
    std::string getFirstname() const { return firstname_; };
    std::string getLastname() const { return lastname_; };
    std::string getAddress() const { return address_; };
    int getIndexNr() const { return indexNr_; };
    std::string getPeselNr() const { return peselNr_; };
    Sex getSex() const { return sexType_; };

    // Setters
    void setFirstname(const std::string& firstname) { firstname_ = firstname; };
    void setLastname(const std::string& lastname) { lastname_ = lastname; };
    void setAddress(const std::string& address) { address_ = address; };
    void setindexNr(const int& indexNr) { indexNr_ = indexNr; }; // Be careful - after use this function db should rebuild indexes
    void setpeselNr(const std::string& peselNr) { peselNr_ = peselNr; }; // Be careful - after use this function db should rebuild indexes

    std::string sexToString(Sex sexType) { return (sexType == Sex::Male) ? "Male" : "Female"; };

   
    // c - tor
    Student(const std::string &firstname, const std::string &lastname, const std::string &address, const int indexNr, const std::string peselNr, const Sex sexType) // c - tor
        : firstname_{firstname}
        , lastname_{lastname}
        , address_{address}
        , indexNr_{indexNr}
        , peselNr_{peselNr}
        , sexType_{sexType}
        {};
};
