#include "db.hpp"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck db::addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const std::string peselNr, const Sex sexType)
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
}
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
std::unique_ptr<Student> *db::findStudentByPesel_Linear(const std::string &PeselNr)
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
std::unique_ptr<Student> *db::findStudentByPesel_Binary(const std::string &PeselNr)
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
std::unique_ptr<Student> *db::findStudentByIdx_Binary(const int &IdxNr)
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
ErrorCheck db::checkIdxAndPeselUnique(const int &IdxNr, const std::string &PeselNr, Sex sexType)
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
    if (Students_.empty())
    {
        return;
    };
    std::sort(Students_.begin(), Students_.end(), [&O](std::unique_ptr<Student> &StudentPtr1, std::unique_ptr<Student> &StudentPtr2)
              {
                  if (std::lexicographical_compare(StudentPtr1->getLastname().begin(), StudentPtr1->getLastname().end(), StudentPtr2->getLastname().begin(), StudentPtr2->getLastname().end()))
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
    rebuildIndex();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void db::sortByPesel(Order O)
{

    if (Students_.empty())
    {
        return;
    };

    std::sort(Students_.begin(), Students_.end(), [&O](std::unique_ptr<Student> &StudentPtr1, std::unique_ptr<Student> &StudentPtr2)
              {
                  if (StudentPtr1->getPeselNr() < StudentPtr2->getPeselNr())
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
    rebuildIndex();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<Student> *> db::sortByLastNameTemporary(Order O)
{

    std::vector<std::unique_ptr<Student> *> tmpSortedList;

    if (Students_.empty())
    {
        return tmpSortedList;
    };

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
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<Student> *> db::sortByPeselTemporary(Order O)
{
    std::vector<std::unique_ptr<Student> *> tmpSortedList;

    if (Students_.empty())
    {
        return tmpSortedList;
    };

    for (auto &OneStudent : Students_)
    {
        tmpSortedList.push_back(&OneStudent);
    };

    std::sort(tmpSortedList.begin(), tmpSortedList.end(), [&O](std::unique_ptr<Student> *StudentPtr1, std::unique_ptr<Student> *StudentPtr2)
              {
                  if (StudentPtr1->get()->getPeselNr() < StudentPtr2->get()->getPeselNr())
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });

    return tmpSortedList;
}
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
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool db::saveToFile(const std::string &filename)
{
    std::ofstream fileObject(filename, std::ios::out | std::ios::binary);
    if (!fileObject)
    {
        return false;
    };
    // Save count of all records
    auto tmpSizeVar = getCount();
    fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));

    for (auto &OneStudent : Students_)
    {

        // Save string firstName_
        tmpSizeVar = OneStudent->getFirstname().length();
        fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        fileObject.write((char *)OneStudent->getFirstname().c_str(), sizeof(char) * tmpSizeVar);

        // Save string lastName_
        tmpSizeVar = OneStudent->getLastname().length();
        fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        fileObject.write((char *)OneStudent->getLastname().data(), sizeof(char) * tmpSizeVar);

        // Save string address_
        tmpSizeVar = OneStudent->getAddress().length();
        fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        fileObject.write((char *)OneStudent->getAddress().data(), sizeof(char) * tmpSizeVar);

        // Save int indexNr_
        auto tmpSizeVar2 = OneStudent->getIndexNr();
        fileObject.write((char *)&tmpSizeVar2, sizeof(tmpSizeVar2));

         // Save string peselNr_
        tmpSizeVar = OneStudent->getPeselNr().length();
        fileObject.write((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        fileObject.write((char *)OneStudent->getPeselNr().data(), sizeof(char) * tmpSizeVar);

        // Save sex
        tmpSizeVar2 = static_cast<int>(OneStudent->getSex());
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
    eraseDatabase();
    size_t Counter = 0;
    size_t tmpSizeVar = 0;

    // Read counter
    fileObject.read((char *)&Counter, sizeof(Counter));

    for (size_t i = 0; i < Counter; i++)
    {

        // Read firstname_
        fileObject.read((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        auto firstNameTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(firstNameTmp.get(), sizeof(char) * tmpSizeVar);

        // Read lastname_
        fileObject.read((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        auto lastNameTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(lastNameTmp.get(), sizeof(char) * tmpSizeVar);

        // Read address_
        fileObject.read((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        auto addressTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(addressTmp.get(), sizeof(char) * tmpSizeVar);

        // Read indexNr_
        int indexNr;
        fileObject.read((char *)&indexNr, sizeof(indexNr));

        // Read indexNr_
        fileObject.read((char *)&tmpSizeVar, sizeof(tmpSizeVar));
        auto PeselNrTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(PeselNrTmp.get(), sizeof(char) * tmpSizeVar);

        // Read indexNr_
        Sex sexTmp;
        fileObject.read((char *)&sexTmp, sizeof(sexTmp));

        // Add new student
        addStudent(firstNameTmp.get(), lastNameTmp.get(), addressTmp.get(), indexNr, PeselNrTmp.get(), sexTmp);
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
ErrorCheck db::findStudentAndModifyPeselNr(const int &IdxNr, const std::string &newPeselNr)
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
bool db::peselValidator(const std::string &PeselNr, Sex sexType)
{
    if (!PeselValidation_)
    {
        return true;
    }
    std::string tmpStringPesel = PeselNr;
    if (tmpStringPesel.length() != 11)
    {
        return false;
    }
    long int Sum;

    std::array<int, 11> PESEL;
    for (size_t i = 0; i < PESEL.size(); ++i)
    {
        PESEL[i] = (tmpStringPesel[i] - 48);
    }

    Sum = PESEL[0] * 1 + PESEL[1] * 3 + PESEL[2] * 7 + PESEL[3] * 9 + PESEL[4] * 1 + PESEL[5] * 3 + PESEL[6] * 7 + PESEL[7] * 9 + PESEL[8] * 1 + PESEL[9] * 3;

    Sex sexInPesel;
    if (!(PESEL[9] % 2))
    {
        sexInPesel = Sex::Female;
    }
    else
    {
        sexInPesel = Sex::Male;
    };

    long int M = (Sum % 10);

    if (M)
    {
        if ((PESEL.back() == ((10 - M))) && (sexInPesel == sexType))
        {
            return true;
        }
    }
    else
    {
        if ((PESEL.back() == M) && (sexInPesel == sexType))
        {
            return true;
        }
    }

    return false;
}
