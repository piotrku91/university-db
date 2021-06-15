#include "db.hpp"
#include <algorithm>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType)
{

    switch (checkIdxAndPeselUnique(indexNr, peselNr))
    {
    case ErrorCheck::OK:
    {
        Students_.push_back(std::make_unique<Student>(Student{firstName, lastName, address, indexNr, peselNr, sexType}));
        rebuildIndex();
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Student> &db::findStudentByLastName_Linear(const std::string &lastName)
{
    auto it = (std::find_if(Students_.begin(), Students_.end(), [&lastName](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getLastname() == lastName)
                                {
                                    return true;
                                };
                                return false;
                            }));
    return *it;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Student> &db::findStudentByPesel_Linear(const long int &PeselNr)
{
    auto it = (std::find_if(Students_.begin(), Students_.end(), [&PeselNr](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getPeselNr() == PeselNr)
                                {
                                    return true;
                                };
                                return false;
                            }));
    return *it;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Student> *db::findStudentByPesel_Binary(const long int &PeselNr)
{
    auto it = (IndexOfStudentPesels_.find(PeselNr));

    if (it != IndexOfStudentPesels_.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    };
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Student> *db::findStudentByIdx_Binary(const long int &IdxNr)
{
    auto it = (IndexOfStudentIdxs_.find(IdxNr));

    if (it != IndexOfStudentIdxs_.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    };
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool db::deleteByIndexNr(const int &IdxNr)
{
    auto it = (std::find_if(Students_.begin(), Students_.end(), [&IdxNr](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getIndexNr() == IdxNr)
                                {
                                    return true;
                                };
                                return false;
                            }));
    if (*it)
    {
        Students_.erase(it);
        rebuildIndex();
        return true;
    };
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::checkIdxAndPeselUnique(const int &IdxNr, const long int &PeselNr)
{

    if (Students_.empty())
    {
        return ErrorCheck::OK;
    };

    if (!peselValidator(PeselNr))
    {
        return ErrorCheck::PeselInvalid;
    }

    if (findStudentByPesel_Binary(PeselNr))
    {
        return ErrorCheck::PeselInUse;
    }

    if (findStudentByIdx_Binary(IdxNr))
    {
        return ErrorCheck::IndexInUse;
    }
    return ErrorCheck::OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void db::sortByLastName(Order O)
{
    std::sort(Students_.begin(), Students_.end(), [&O](std::unique_ptr<Student> &StudentPtr1, std::unique_ptr<Student> &StudentPtr2)
              {
                  if (std::lexicographical_compare(StudentPtr1->getLastname().begin(), StudentPtr1->getLastname().end(), StudentPtr2->getLastname().begin(), StudentPtr2->getLastname().end()))
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
    rebuildIndex();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void db::sortByPesel(Order O)
{
    std::sort(Students_.begin(), Students_.end(), [&O](std::unique_ptr<Student> &StudentPtr1, std::unique_ptr<Student> &StudentPtr2)
              {
                  if (StudentPtr1->getPeselNr() < StudentPtr2->getPeselNr())
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
    rebuildIndex();
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<Student> *> db::sortByPeselTemporary(Order O)
{
    std::vector<std::unique_ptr<Student> *> tmpSortedList;

    if (O == Order::Asc)
    {
        for (auto it = IndexOfStudentPesels_.begin(); (it != IndexOfStudentPesels_.end()); ++it)
        {
            tmpSortedList.push_back(it->second);
        };
    }
    else
    {
       for (auto it = IndexOfStudentPesels_.rbegin(); (it != IndexOfStudentPesels_.rend()); ++it)
        {
            tmpSortedList.push_back(it->second);
        };
    }

    return tmpSortedList;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void db::rebuildIndex()
{
    IndexOfStudentIdxs_.clear();
    IndexOfStudentPesels_.clear();
    if (Students_.empty())
        return;
    for (auto &OneStudent : Students_)
    {
        IndexOfStudentIdxs_.insert({OneStudent->getIndexNr(), &OneStudent});
        IndexOfStudentPesels_.insert({OneStudent->getPeselNr(), &OneStudent});
    }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////