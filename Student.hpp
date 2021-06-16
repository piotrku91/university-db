#pragma once
#include <string>

enum class Sex
{
    Male,
    Female
};

class Student
{
private:
    std::string firstname_;
    std::string lastname_;
    std::string address_;
    int indexNr_;
    long int peselNr_;
    Sex sexType_;

public:
    // Getters
    std::string getFirstname() const { return firstname_; };
    std::string getLastname() const { return lastname_; };
    std::string getAddress() const { return address_; };
    int getIndexNr() const { return indexNr_; };
    long int getPeselNr() const { return peselNr_; };
    Sex getSex() const { return sexType_; };

    std::string sexToString(Sex sexType) {return (sexType==Sex::Male)?"Male":"Female";};

    Student() {};

    Student(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType) // c - tor
        : firstname_(firstName)
        , lastname_(lastName)
        , address_(address)
        , indexNr_(indexNr)
        , peselNr_(peselNr)
        , sexType_(sexType){};
};