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
        Records_.push_back(Person::createPerson<Student>(Student{firstname, lastname, address, indexNr, peselNr, sexType}));
        rebuildIndex();
        };
         if (type==PersonType::Worker) {
        Records_.push_back(Person::createPerson<Worker>(Worker{firstname, lastname, address, peselNr, sexType, Salary}));
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
ErrorCheck Db::addPerson(std::shared_ptr<Person>& completePerson)
{
    switch ( 
        (completePerson->getPersonType()==PersonType::Student) ?
    (checkIdxAndPeselUnique(Person::isTargetClassObject<Person,Student>(completePerson.get())->getIndexNr(), completePerson->getPeselNr(), completePerson->getSex())) 
    : 
    (checkPeselUnique(completePerson->getPeselNr(), completePerson->getSex())) 
           )
    {
    case ErrorCheck::OK:
    {
        Records_.push_back(completePerson);
     
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
std::shared_ptr<Person>Db::findPersonByLastName_Linear(const std::string &lastname)
{
    auto it = (std::find_if(Records_.begin(), Records_.end(), [&lastname](std::shared_ptr<Person> &student_ptr)
                            {
                                if (student_ptr->getLastname() == lastname)
                                {
                                    return true;
                                };
                                return false;
                            }));
    if (it != Records_.end())
    {
        return *it;
    }
    else
    {
        return nullptr;
    };
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Person>Db::findPersonByPesel_Linear(const std::string &peselNr)
{
    auto it = (std::find_if(Records_.begin(), Records_.end(), [&peselNr](std::shared_ptr<Person> &student_ptr)
                            {
                                if (student_ptr->getPeselNr() == peselNr)
                                {
                                    return true;
                                };
                                return false;
                            }));
    if (it != Records_.end())
    {
        return *it;
    }
    else
    {
        return nullptr;
    };
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Person>Db::findPersonByPesel_Binary(const std::string &peselNr)
{
    auto it = (IndexOfPersonsPesels_.find(peselNr));

    if (it != IndexOfPersonsPesels_.end())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    };
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Person>Db::findPersonByIdx_Binary(const int &indexNr)
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
    if (Records_.empty())
    {
        return false;
    };
    auto it = findPersonByIdx_Binary(indexNr);
    if (it)
    {
        Records_.erase(std::remove(Records_.begin(), Records_.end(), it), Records_.end());
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
    if (Records_.empty())
    {
        return ErrorCheck::OK;
    };

    if (!peselValidator(peselNr, sexType))
    {
        return ErrorCheck::PeselInvalid;
    }

    if (findPersonByPesel_Binary(peselNr))
    {
        return ErrorCheck::PeselInUse;
    }
    return ErrorCheck::OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::checkIdxUnique(const int &indexNr)
{
    if (Records_.empty())
    {
        return ErrorCheck::OK;
    };
   
    if (findPersonByIdx_Binary(indexNr))
    {
        return ErrorCheck::IndexInUse;
    }
    return ErrorCheck::OK;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Db::sortByLastName(Order O)
{
    if (Records_.empty())
    {
        return;
    };
    std::sort(Records_.begin(), Records_.end(), [&O](std::shared_ptr<Person> &student_ptr1, std::shared_ptr<Person> &student_ptr2)
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

    if (Records_.empty())
    {
        return;
    };

    std::sort(Records_.begin(), Records_.end(), [&O](std::shared_ptr<Person> &student_ptr1, std::shared_ptr<Person> &student_ptr2)
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
void Db::sortBySalary(Order O)
{

    if (Records_.empty())
    {
        return;
    };

    std::sort(Records_.begin(), Records_.end(), [&O](std::shared_ptr<Person> &student_ptr1, std::shared_ptr<Person> &student_ptr2)
              {
                
                   auto lhs = (Person::isTargetClassObject<Person,Worker>(student_ptr1.get()))?Person::isTargetClassObject<Person,Worker>(student_ptr1.get())->getSalary():0;
                   auto rhs = (Person::isTargetClassObject<Person,Worker>(student_ptr2.get()))?Person::isTargetClassObject<Person,Worker>(student_ptr2.get())->getSalary():0;
       
                  if (lhs < rhs)
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });
    rebuildIndex();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::shared_ptr<Person>> Db::sortByLastNameTemporary(Order O)
{

    std::vector<std::shared_ptr<Person>> tmpSortedList;

    if (Records_.empty())
    {
        return tmpSortedList;
    };

    for (auto &onePerson : Records_)
    {
        tmpSortedList.push_back(onePerson);
    };

    std::sort(tmpSortedList.begin(), tmpSortedList.end(), [&O](std::shared_ptr<Person>student_ptr1, std::shared_ptr<Person>student_ptr2)
              {
                  if (std::lexicographical_compare(student_ptr1->getLastname().begin(), student_ptr1->getLastname().end(), student_ptr2->getLastname().begin(), student_ptr2->getLastname().end()))
                  {
                      return (O == Order::Asc) ? true : false;
                  }
                  return (O == Order::Asc) ? false : true;
              });

    return tmpSortedList;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::shared_ptr<Person>> Db::sortByPeselTemporary(Order O)
{
    std::vector<std::shared_ptr<Person>> tmpSortedList;

    if (Records_.empty())
    {
        return tmpSortedList;
    };

    for (auto &onePerson : Records_)
    {
        tmpSortedList.push_back(onePerson);
    };

    std::sort(tmpSortedList.begin(), tmpSortedList.end(), [&O](std::shared_ptr<Person>student_ptr1, std::shared_ptr<Person>student_ptr2)
              {
                  if (student_ptr1->getPeselNr() < student_ptr2->getPeselNr())
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
    IndexOfPersonsPesels_.clear();
    if (Records_.empty())
        return;

    for (auto &onePerson : Records_)
    {
        auto student_ptr = Person::isTargetClassObject<Person,Student>(onePerson.get());
        if (student_ptr) {IndexOfStudentIdxs_.insert({student_ptr->getIndexNr(), onePerson});};
        IndexOfPersonsPesels_.insert({onePerson->getPeselNr(), onePerson});
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

    for (auto &onePerson : Records_)
    {

        // Save string firstname_
        tmpSizeVar = onePerson->getFirstname().length();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        fileObject.write(reinterpret_cast<char*>(onePerson->getFirstname().data()), sizeof(char) * tmpSizeVar);

        // Save string lastname_
        tmpSizeVar = onePerson->getLastname().length();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        fileObject.write(reinterpret_cast<char*>(onePerson->getLastname().data()), sizeof(char) * tmpSizeVar);

        // Save string address_
        tmpSizeVar = onePerson->getAddress().length();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        fileObject.write(reinterpret_cast<char*>(onePerson->getAddress().data()), sizeof(char) * tmpSizeVar);

        // Save int indexNr_ or Salary
        auto tmpSizeVar2 = 0;
        auto student_ptr = Person::isTargetClassObject<Person,Student>(onePerson.get());
        if (student_ptr) { tmpSizeVar2 = student_ptr->getIndexNr(); };
        auto worker_ptr = Person::isTargetClassObject<Person,Worker>(onePerson.get());
        if (worker_ptr) { tmpSizeVar2 = worker_ptr->getSalary(); };
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar2), sizeof(tmpSizeVar2));

         // Save string peselNr_
        tmpSizeVar = onePerson->getPeselNr().length();
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        fileObject.write(reinterpret_cast<char*>(onePerson->getPeselNr().data()), sizeof(char) * tmpSizeVar);

        // Save sex
        tmpSizeVar2 = static_cast<int>(onePerson->getSex());
        fileObject.write(reinterpret_cast<char*>(&tmpSizeVar2), sizeof(tmpSizeVar2));

        // Save person type
        tmpSizeVar2 = static_cast<int>(onePerson->getPersonType());
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

        // Read indexNr_ or salary
        int indexNrOrSalary;
        fileObject.read(reinterpret_cast<char*>(&indexNrOrSalary), sizeof(indexNrOrSalary));

        // Read pesel nr
        fileObject.read(reinterpret_cast<char*>(&tmpSizeVar), sizeof(tmpSizeVar));
        auto peselNrTmp = std::make_unique<char[]>(tmpSizeVar + 1);
        fileObject.read(peselNrTmp.get(), sizeof(char) * tmpSizeVar);

        // Read sex
        Sex sexTmp;
        fileObject.read(reinterpret_cast<char*>(&sexTmp), sizeof(sexTmp));

        //  Read personType_
        PersonType personTypeTmp;
       fileObject.read(reinterpret_cast<char*>(&personTypeTmp), sizeof(personTypeTmp));

        // Add new student
        if (personTypeTmp==PersonType::Student) {
        addPerson(personTypeTmp,firstnameTmp.get(), lastnameTmp.get(), addressTmp.get(), indexNrOrSalary, peselNrTmp.get(), sexTmp);
        }
        else 
        {
        addPerson(personTypeTmp,firstnameTmp.get(), lastnameTmp.get(), addressTmp.get(), 0, peselNrTmp.get(), sexTmp, indexNrOrSalary);
        }
    }
    fileObject.close();
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Db::eraseDatabase()
{
    Records_.clear();
    rebuildIndex();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findPersonAndModifyFirstname(const std::string& peselNr, const std::string &newFirstname)
{
    auto found = findPersonByPesel_Binary(peselNr);
    if (found)
    {
        found->setFirstname(newFirstname);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findPersonAndModifyLastname(const std::string& peselNr, const std::string &newLastname)
{
    auto found = findPersonByPesel_Binary(peselNr);
    if (found)
    {
        found->setLastname(newLastname);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findPersonAndModifyAddress(const std::string& peselNr, const std::string &newAddress)
{
    auto found = findPersonByPesel_Binary(peselNr);
    if (found)
    {
        found->setAddress(newAddress);
        return ErrorCheck::OK;
    }
    return ErrorCheck::NotFound;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ErrorCheck Db::findPersonAndModifySalary(const std::string& peselNr, const int &newSalary)
{
    auto found = findPersonByPesel_Binary(peselNr);
    if (found)
    {
        auto worker_ptr = Person::isTargetClassObject<Person,Worker>(found.get());
        if (worker_ptr) {
        worker_ptr->setSalary(newSalary);
        rebuildIndex();
        return ErrorCheck::OK;
        };
    }
    return ErrorCheck::NotFound;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findPersonAndModifyindexNr(const std::string& peselNr, const int &newindexNr)
{
    auto found = findPersonByPesel_Binary(peselNr);
    if (found)
    {
        if (findPersonByIdx_Binary(newindexNr))
        {
            return ErrorCheck::IndexInUse;
        };
        auto student_ptr = Person::isTargetClassObject<Person,Student>(found.get());
        if (student_ptr) {
        student_ptr->setindexNr(newindexNr);
        rebuildIndex();
        return ErrorCheck::OK;
        };
        
    }
    return ErrorCheck::NotFound;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ErrorCheck Db::findPersonAndModifypeselNr(const std::string& peselNr, const std::string &newpeselNr)
{
    auto found = findPersonByPesel_Binary(peselNr);
    if (found)
    {
        if (findPersonByPesel_Binary(newpeselNr))
        {
            return ErrorCheck::PeselInUse;
        };

        if (!peselValidator(newpeselNr, found.get()->getSex()))
        {
            return ErrorCheck::PeselInvalid;
        }

        found->setpeselNr(newpeselNr);
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
