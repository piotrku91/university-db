#include "Db.hpp"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::addPerson(PersonType type, const std::string& firstname, const std::string& lastname, const std::string& address, const int indexNr, const std::string& peselNr, const Sex sexType, const int Salary)
{
    switch ( (type==PersonType::Student)?(checkIdxAndPeselUnique(indexNr, peselNr, sexType)) : (checkPeselUnique(peselNr, sexType)))
    {
    case ErrorCheck::OK:
    {
        if (type==PersonType::Student) {
        Students_.push_back(std::make_unique<Student>(Student{firstname, lastname, address, indexNr, peselNr, sexType}));
        rebuildIndex();
        };
         if (type==PersonType::Worker) {
        Students_.push_back(std::make_unique<Worker>(Worker{firstname, lastname, address, peselNr, sexType, Salary}));
        rebuildIndex();
        };
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
std::unique_ptr<Person> *Db::findStudentByLastName_Linear(const std::string &lastname)
{
    auto it = (std::find_if(Students_.begin(), Students_.end(), [&lastname](std::unique_ptr<Person> &student_ptr)
                            {
                                if (student_ptr->getLastname() == lastname)
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
std::unique_ptr<Person> *Db::findStudentByPesel_Linear(const std::string &peselNr)
{
    auto it = (std::find_if(Students_.begin(), Students_.end(), [&peselNr](std::unique_ptr<Person> &student_ptr)
                            {
                                if (student_ptr->getPeselNr() == peselNr)
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
std::unique_ptr<Person> *Db::findStudentByPesel_Binary(const std::string &peselNr)
{
    auto it = (IndexOfStudentPesels_.find(peselNr));

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
std::unique_ptr<Person> *Db::findStudentByIdx_Binary(const int &indexNr)
{
    auto it = (IndexOfStudentIdxs_.find(indexNr));

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
bool Db::deleteByIndexNr(const int &indexNr)
{
    if (Students_.empty())
    {
        return false;
    };
    auto it = findStudentByIdx_Binary(indexNr);
    if (it)
    {
        Students_.erase(std::remove(Students_.begin(), Students_.end(), *it), Students_.end());
        rebuildIndex();
        return true;
    };
    return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::checkIdxAndPeselUnique(const int &indexNr, const std::string &peselNr, Sex sexType)
{   
    ErrorCheck peselTest=checkPeselUnique(peselNr,sexType);
    ErrorCheck indexTest=checkIdxUnique(indexNr);

    if (peselTest!=ErrorCheck::OK) {return peselTest;};
    if (indexTest!=ErrorCheck::OK) {return indexTest;};
    
    return ErrorCheck::OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::checkPeselUnique(const std::string &peselNr, Sex sexType)
{
    if (Students_.empty())
    {
        return ErrorCheck::OK;
    };

    if (!peselValidator(peselNr, sexType))
    {
        return ErrorCheck::PeselInvalid;
    }

    if (findStudentByPesel_Binary(peselNr))
    {
        return ErrorCheck::PeselInUse;
    }
    return ErrorCheck::OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::checkIdxUnique(const int &indexNr)
{
    if (Students_.empty())
    {
        return ErrorCheck::OK;
    };
   
    if (findStudentByIdx_Binary(indexNr))
    {
        return ErrorCheck::IndexInUse;
    }
    return ErrorCheck::OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Db::sortByLastName(Order O)
{
    if (Students_.empty())
    {
        return;
    };
    std::sort(Students_.begin(), Students_.end(), [&O](std::unique_ptr<Person> &student_ptr1, std::unique_ptr<Person> &student_ptr2)
              {
                  if (std::lexicographical_compare(student_ptr1->getLastname().begin(), student_ptr1->getLastname().end(), student_ptr2->getLastname().begin(), student_ptr2->getLastname().end()))
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
    rebuildIndex();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Db::sortByPesel(Order O)
{

    if (Students_.empty())
    {
        return;
    };

    std::sort(Students_.begin(), Students_.end(), [&O](std::unique_ptr<Person> &student_ptr1, std::unique_ptr<Person> &student_ptr2)
              {
                  if (student_ptr1->getPeselNr() < student_ptr2->getPeselNr())
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
    rebuildIndex();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<Person> *> Db::sortByLastNameTemporary(Order O)
{

    std::vector<std::unique_ptr<Person> *> tmpSortedList;

    if (Students_.empty())
    {
        return tmpSortedList;
    };

    for (auto &OneStudent : Students_)
    {
        tmpSortedList.push_back(&OneStudent);
    };

    std::sort(tmpSortedList.begin(), tmpSortedList.end(), [&O](std::unique_ptr<Person> *student_ptr1, std::unique_ptr<Person> *student_ptr2)
              {
                  if (std::lexicographical_compare(student_ptr1->get()->getLastname().begin(), student_ptr1->get()->getLastname().end(), student_ptr2->get()->getLastname().begin(), student_ptr2->get()->getLastname().end()))
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });

    return tmpSortedList;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::unique_ptr<Person> *> Db::sortByPeselTemporary(Order O)
{
    std::vector<std::unique_ptr<Person> *> tmpSortedList;

    if (Students_.empty())
    {
        return tmpSortedList;
    };

    for (auto &OneStudent : Students_)
    {
        tmpSortedList.push_back(&OneStudent);
    };

    std::sort(tmpSortedList.begin(), tmpSortedList.end(), [&O](std::unique_ptr<Person> *student_ptr1, std::unique_ptr<Person> *student_ptr2)
              {
                  if (student_ptr1->get()->getPeselNr() < student_ptr2->get()->getPeselNr())
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });

    return tmpSortedList;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Db::rebuildIndex()
{
    IndexOfStudentIdxs_.clear();
    IndexOfStudentPesels_.clear();
    if (Students_.empty())
        return;

    for (auto &OneStudent : Students_)
    {
        if (OneStudent->getIndexNr()) {IndexOfStudentIdxs_.insert({OneStudent->getIndexNr(), &OneStudent});};
        IndexOfStudentPesels_.insert({OneStudent->getPeselNr(), &OneStudent});
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Db::saveToFile(const std::string &filename)
{
    std::ofstream fileObject(filename, std::ios::out | std::ios::binary);
    if (!fileObject)
    {
        return false;
    };
    // Save count of all records
    auto tmpSizeVar = getCount();
    fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));

    for (auto &OneStudent : Students_)
    {

        // Save string firstname_
        tmpSizeVar = OneStudent->getFirstname().length();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        fileObject.write(reinterpret_cast<char*>(OneStudent->getFirstname().data()), sizeof(char) * tmpSizeVar);

        // Save string lastname_
        tmpSizeVar = OneStudent->getLastname().length();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        fileObject.write(reinterpret_cast<char*>(OneStudent->getLastname().data()), sizeof(char) * tmpSizeVar);

        // Save string address_
        tmpSizeVar = OneStudent->getAddress().length();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        fileObject.write(reinterpret_cast<char*>(OneStudent->getAddress().data()), sizeof(char) * tmpSizeVar);

        // Save int indexNr_
        auto tmpSizeVar2 = OneStudent->getIndexNr();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar2), sizeof(tmpSizeVar2));

         // Save string peselNr_
        tmpSizeVar = OneStudent->getPeselNr().length();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        fileObject.write(reinterpret_cast<char*>(OneStudent->getPeselNr().data()), sizeof(char) * tmpSizeVar);

        // Save sex
        tmpSizeVar2 = static_cast<int>(OneStudent->getSex());
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar2), sizeof(tmpSizeVar2));

        // Save person type
        tmpSizeVar2 = static_cast<int>(OneStudent->getPersonType());
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar2), sizeof(tmpSizeVar2));
    }
    fileObject.close();
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Db::loadFromFile(const std::string &filename)
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
    fileObject.read(reinterpret_cast<char*>(&Counter), sizeof(Counter));

    for (size_t i = 0; i < Counter; i++)
    {

        // Read firstname_
        fileObject.read(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        auto firstnameTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(firstnameTmp.get(), sizeof(char) * tmpSizeVar);

        // Read lastname_
        fileObject.read(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        auto lastnameTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(lastnameTmp.get(), sizeof(char) * tmpSizeVar);

        // Read address_
        fileObject.read(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        auto addressTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(addressTmp.get(), sizeof(char) * tmpSizeVar);

        // Read indexNr_
        int indexNr;
        fileObject.read(reinterpret_cast<char*>(&indexNr), sizeof(indexNr));

        // Read indexNr_
        fileObject.read(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        auto peselNrTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(peselNrTmp.get(), sizeof(char) * tmpSizeVar);

        // Read indexNr_
        Sex sexTmp;
        fileObject.read(reinterpret_cast<char*>(&sexTmp), sizeof(sexTmp));

        //  Read personType_
        PersonType personTypeTmp;
       fileObject.read(reinterpret_cast<char*>(&personTypeTmp), sizeof(personTypeTmp));

        // Add new student
        addPerson(personTypeTmp,firstnameTmp.get(), lastnameTmp.get(), addressTmp.get(), indexNr, peselNrTmp.get(), sexTmp);
    }
    fileObject.close();
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Db::eraseDatabase()
{
    Students_.clear();
    rebuildIndex();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findStudentAndModifyFirstname(const int &indexNr, const std::string &newFirstname)
{
    auto found = findStudentByIdx_Binary(indexNr);
    if (found)
    {
        found->get()->setFirstname(newFirstname);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findStudentAndModifyLastname(const int &indexNr, const std::string &newLastname)
{
    auto found = findStudentByIdx_Binary(indexNr);
    if (found)
    {
        found->get()->setLastname(newLastname);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findStudentAndModifyAddress(const int &indexNr, const std::string &newAddress)
{
    auto found = findStudentByIdx_Binary(indexNr);
    if (found)
    {
        found->get()->setAddress(newAddress);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findStudentAndModifyindexNr(const int &indexNr, const int &newindexNr)
{
    auto found = findStudentByIdx_Binary(indexNr);
    if (found)
    {
        if (findStudentByIdx_Binary(newindexNr))
        {
            return ErrorCheck::IndexInUse;
        };

        found->get()->setindexNr(newindexNr);
        rebuildIndex();
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findStudentAndModifypeselNr(const int &indexNr, const std::string &newpeselNr)
{
    auto found = findStudentByIdx_Binary(indexNr);
    if (found)
    {
        if (findStudentByPesel_Binary(newpeselNr))
        {
            return ErrorCheck::PeselInUse;
        };

        if (!peselValidator(newpeselNr, found->get()->getSex()))
        {
            return ErrorCheck::PeselInvalid;
        }

        found->get()->setpeselNr(newpeselNr);
        rebuildIndex();
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Db::peselValidator(const std::string &peselNr, Sex sexType)
{
    if (!peselValidation_)
    {
        return true;
    }
    std::string tmpStringPesel = peselNr;
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
