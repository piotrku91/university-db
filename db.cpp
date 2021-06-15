#include "db.hpp"
#include <algorithm>

bool db::addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType)
{

    if (checkIdxUnique(indexNr)) {
    Students.push_back(std::make_unique<Student>(Student{firstName, lastName, address, indexNr, peselNr, sexType}));

    return true;

    }
    return false;
};

std::unique_ptr<Student>& db::findStudentByLastName(const std::string &lastName)
{
    auto it = (std::find_if(Students.begin(),Students.end(),[&lastName](std::unique_ptr<Student>& StudentPtr)
    {
        if (StudentPtr->getLastname()==lastName) {return true;};
    return false;
    }
    ));
    return *it;
}

std::unique_ptr<Student>& db::findStudentByPesel(const long int &PeselNr)
{
    auto it = (std::find_if(Students.begin(),Students.end(),[&PeselNr](std::unique_ptr<Student>& StudentPtr)
    {
        if (StudentPtr->getpeselNr()==PeselNr) {return true;};
    return false;
    }
    ));
    return *it;
}

bool db::deleteByIndexNr(const int& IdxNr)
{
    auto it = (std::find_if(Students.begin(),Students.end(),[&IdxNr](std::unique_ptr<Student>& StudentPtr)
    {
        if (StudentPtr->getIndexNr()==IdxNr) {return true;};
    return false;
    }
    ));
    if (*it) {Students.erase(it); return true; };
    return false; 
}

bool db::checkIdxUnique(const int& IdxNr)
{   
    if (Students.empty()) {return true;};
    auto it = (std::find_if(Students.begin(),Students.end(),[&IdxNr](std::unique_ptr<Student>& StudentPtr)
    {
        if (StudentPtr->getIndexNr()==IdxNr) {return true;};
    return false;
    }
    ));
    if (*it) {return false; };
    return true; 
}