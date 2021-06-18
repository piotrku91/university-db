#include "db.hpp"
#include <algorithm>
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType)
{
    switch (checkIdxAndPeselUnique(indexNr, peselNr, sexType))
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
std::unique_ptr<Student> *db::findStudentByLastName_Linear(const std::string &lastName)
{
    auto it = (std::find_if(Students_.begin(), Students_.end(), [&lastName](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getLastname() == lastName)
                                {
                                    return true;
                                };
                                return false;
                            }));
    if (it != Students_.end())
    {
        return &(*it);
    }
    else
    {
        return nullptr;
    };
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Student> *db::findStudentByPesel_Linear(const long int &PeselNr)
{
    auto it = (std::find_if(Students_.begin(), Students_.end(), [&PeselNr](std::unique_ptr<Student> &StudentPtr)
                            {
                                if (StudentPtr->getPeselNr() == PeselNr)
                                {
                                    return true;
                                };
                                return false;
                            }));
    if (it != Students_.end())
    {
        return &(*it);
    }
    else
    {
        return nullptr;
    };
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
    if (Students_.empty())
    {
        return false;
    };
    auto it = findStudentByIdx_Binary(IdxNr);
    if (it)
    {
        Students_.erase(std::remove(Students_.begin(), Students_.end(), *it), Students_.end());
        rebuildIndex();
        return true;
    };
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::checkIdxAndPeselUnique(const int &IdxNr, const long int &PeselNr, Sex sexType)
{

    if (Students_.empty())
    {
        return ErrorCheck::OK;
    };

    if (!peselValidator(PeselNr, sexType))
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
std::vector<std::unique_ptr<Student> *> db::sortByLastNameTemporary(Order O)
{
    std::vector<std::unique_ptr<Student> *> tmpSortedList;

    for (auto &OneStudent : Students_)
    {
        tmpSortedList.push_back(&OneStudent);
    };

    std::sort(tmpSortedList.begin(), tmpSortedList.end(), [&O](std::unique_ptr<Student> *StudentPtr1, std::unique_ptr<Student> *StudentPtr2)
              {
                  if (std::lexicographical_compare(StudentPtr1->get()->getLastname().begin(), StudentPtr1->get()->getLastname().end(), StudentPtr2->get()->getLastname().begin(), StudentPtr2->get()->getLastname().end()))
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });

    return tmpSortedList;
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
bool db::saveToFile(const std::string &filename)
{
    std::ofstream fileObject(filename, std::ios::out | std::ios::binary);
    if (!fileObject)
    {
        return false;
    };
    // Save count of all records
    size_t &&tmpSizeVar = std::move(getCount());
    fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));
    for (auto &OneStudent : Students_)
    {
        // Save string firstName_
        tmpSizeVar = std::move(OneStudent->getFirstname().size());
        fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        fileObject.write((char *)OneStudent->getFirstname().data(), sizeof(char) * tmpSizeVar);

        // Save string lastName_
        tmpSizeVar = std::move(OneStudent->getLastname().size());
        fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        fileObject.write((char *)OneStudent->getLastname().data(), sizeof(char) * tmpSizeVar);

        // Save string address_
        tmpSizeVar = std::move(OneStudent->getAddress().size());
        fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        fileObject.write((char *)OneStudent->getAddress().data(), sizeof(char) * tmpSizeVar);

        // Save int indexNr_
        int &&tmpSizeVar2 = std::move(OneStudent->getIndexNr());
        fileObject.write((char *)&tmpSizeVar2, sizeof(tmpSizeVar2));

        // Save long int indexNr_
        long int &&tmpSizeVar3 = std::move(OneStudent->getPeselNr());
        fileObject.write((char *)&tmpSizeVar3, sizeof(tmpSizeVar3));

        // Save sex
        tmpSizeVar2 = std::move(static_cast<int>(OneStudent->getSex()));
        fileObject.write((char *)&tmpSizeVar2, sizeof(tmpSizeVar2));
    }
    fileObject.close();
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool db::loadFromFile(const std::string &filename)
{
    std::ifstream fileObject(filename, std::ios::out | std::ios::binary);
    if (!fileObject)
    {
        return false;
    };
    size_t Counter;
    size_t tmpSizeVar;

    // Read counter
    fileObject.read((char *)&Counter, sizeof(Counter));

    for (size_t i = 0; i < Counter; i++)
    {
        // Read firstname_
        fileObject.read((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        std::unique_ptr<char> firstNameTmp = std::make_unique<char>(tmpSizeVar + 1);
        fileObject.read(firstNameTmp.get(), tmpSizeVar);

        // Read lastname_
        fileObject.read((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        std::unique_ptr<char> lastNameTmp = std::make_unique<char>(tmpSizeVar + 1);
        fileObject.read(lastNameTmp.get(), tmpSizeVar);

        // Read address_
        fileObject.read((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        std::unique_ptr<char> addressTmp = std::make_unique<char>(tmpSizeVar + 1);
        fileObject.read(addressTmp.get(), tmpSizeVar);

        // Read indexNr_
        int indexNr;
        fileObject.read((char *)&indexNr, sizeof(indexNr));

        // Read indexNr_
        long int peselNr;
        fileObject.read((char *)&peselNr, sizeof(peselNr));

        // Read indexNr_
        Sex sexTmp;
        fileObject.read((char *)&sexTmp, sizeof(int));

        // Add new student
        addStudent(firstNameTmp.get(), lastNameTmp.get(), addressTmp.get(), indexNr, peselNr, sexTmp);
    }
    fileObject.close();
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void db::eraseDatabase()
{
    Students_.clear();
    rebuildIndex();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::findStudentAndModifyFirstname(const int &IdxNr, const std::string &newFirstname)
{
    auto found = findStudentByIdx_Binary(IdxNr);
    if (found)
    {
        found->get()->setFirstname(newFirstname);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::findStudentAndModifyLastname(const int &IdxNr, const std::string &newLastname)
{
    auto found = findStudentByIdx_Binary(IdxNr);
    if (found)
    {
        found->get()->setLastname(newLastname);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::findStudentAndModifyAddress(const int &IdxNr, const std::string &newAddress)
{
    auto found = findStudentByIdx_Binary(IdxNr);
    if (found)
    {
        found->get()->setAddress(newAddress);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::findStudentAndModifyIndexNr(const int &IdxNr, const int &newIndexNr)
{
    auto found = findStudentByIdx_Binary(IdxNr);
    if (found)
    {
        if (findStudentByIdx_Binary(newIndexNr))
        {
            return ErrorCheck::IndexInUse;
        };

        found->get()->setIndexNr(newIndexNr);
        rebuildIndex();
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::findStudentAndModifyPeselNr(const int &IdxNr, const long int &newPeselNr)
{
    auto found = findStudentByIdx_Binary(IdxNr);
    if (found)
    {
        if (findStudentByPesel_Binary(newPeselNr))
        {
            return ErrorCheck::PeselInUse;
        };

        if (!peselValidator(newPeselNr, found->get()->getSex()))
        {
            return ErrorCheck::PeselInvalid;
        }

        found->get()->setPeselNr(newPeselNr);
        rebuildIndex();
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool db::peselValidator(const long int &PeselNr, Sex sexType)
{
    if (!PeselValidation_)
    {
        return true;
    }
    std::string tmpStringPesel = std::to_string(PeselNr);
    long int Sum;

    std::array<int, 11> PESEL;
    for (int i = 0; i < PESEL.size(); ++i)
    {
        PESEL[i] = (tmpStringPesel[i] - 48);
    }

    Sum = PESEL[0] * 1 + PESEL[1] * 3 + PESEL[2] * 7 + PESEL[3] * 9 + PESEL[4] * 1 + PESEL[5] * 3 + PESEL[6] * 7 + PESEL[7] * 9 + PESEL[8] * 1 + PESEL[9] * 3;

Sex sexInPesel;
if (!(PESEL[9]%2)) {sexInPesel=Sex::Female;} else {sexInPesel=Sex::Male;};

    if ((PESEL.back() == ((10 - Sum % 10) % 10)) && (sexInPesel==sexType))
    {
        return true;
    };

    return false;
};