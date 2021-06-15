#include "db.hpp"
#include <algorithm>

ErrorCheck db::addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType)
{

    switch (checkIdxAndPeselUnique(indexNr, peselNr))
    {
    case ErrorCheck::OK:
    {
        Students.push_back(std::make_unique<Student>(Student{firstName, lastName, address, indexNr, peselNr, sexType}));
        return ErrorCheck::OK;
        break;
    }
    case ErrorCheck::IndexInUse:
    {
        return ErrorCheck::IndexInUse;
        break;
    }
    case ErrorCheck::PeselInUse:
    {
        return ErrorCheck::PeselInUse;
        break;
    }
    case ErrorCheck::PeselInvalid:
    {
        return ErrorCheck::PeselInvalid;
        break;
    }
    default:
    {
        return ErrorCheck::UnknownError;
        break;
    }
    };
};

std::unique_ptr<Student> &db::findStudentByLastName(const std::string &lastName)
{
    auto it = (std::find_if(Students.begin(), Students.end(), [&lastName](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getLastname() == lastName)
                                {
                                    return true;
                                };
                                return false;
                            }));
    return *it;
}

std::unique_ptr<Student> &db::findStudentByPesel(const long int &PeselNr)
{
    auto it = (std::find_if(Students.begin(), Students.end(), [&PeselNr](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getPeselNr() == PeselNr)
                                {
                                    return true;
                                };
                                return false;
                            }));
    return *it;
}

bool db::deleteByIndexNr(const int &IdxNr)
{
    auto it = (std::find_if(Students.begin(), Students.end(), [&IdxNr](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getIndexNr() == IdxNr)
                                {
                                    return true;
                                };
                                return false;
                            }));
    if (*it)
    {
        Students.erase(it);
        return true;
    };
    return false;
}

ErrorCheck db::checkIdxAndPeselUnique(const int &IdxNr, const long int &PeselNr)
{
    ErrorCheck results;
    if (Students.empty())
    {
        return results = ErrorCheck::OK;
    };
    auto it = (std::find_if(Students.begin(), Students.end(), [&IdxNr, &PeselNr, &results](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getIndexNr() == IdxNr)
                                {
                                    results = ErrorCheck::IndexInUse;
                                    return true;
                                };
                                if (StudentPtr->getPeselNr() == PeselNr)
                                {
                                    results = ErrorCheck::PeselInUse;
                                    return true;
                                };
                                results = ErrorCheck::OK;
                                return false;
                            }));

    return results;
}

void db::sortByLastName(Order O)
{
    std::sort(Students.begin(), Students.end(), [&O](std::unique_ptr<Student> &StudentPtr1, std::unique_ptr<Student> &StudentPtr2)
              {
                  if (std::lexicographical_compare(StudentPtr1->getLastname().begin(), StudentPtr1->getLastname().end(), StudentPtr2->getLastname().begin(), StudentPtr2->getLastname().end()))
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
};

void db::sortByPesel(Order O)
{
    std::sort(Students.begin(), Students.end(), [&O](std::unique_ptr<Student> &StudentPtr1, std::unique_ptr<Student> &StudentPtr2)
              {
                  if (StudentPtr1->getPeselNr()<StudentPtr2->getPeselNr())
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
};