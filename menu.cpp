#include "menu.hpp"

void menu::addNewUser(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType)
{
    switch (dbManager_.addStudent(firstName, lastName, address, indexNr, peselNr, sexType))
    {
    case ErrorCheck::OK:
    {
        std::cout << "SUCCESS! - Student added to database. \n";
        break;
    }
    case ErrorCheck::IndexInUse:
    {
        std::cout << "ERROR! - Index is already in database! \n";
        break;
    }
    case ErrorCheck::PeselInUse:
    {
        std::cout << "ERROR! - Pesel is already in database! \n";
        break;
    }
    case ErrorCheck::PeselInvalid:
    {
        std::cout << "ERROR! - Invalid PESEL number! \n";
        break;
    }
    case ErrorCheck::UnknownError:
    {
        std::cout << "ERROR! - Unknown Error! \n";
        break;
    }
    };
}

void menu::searchAndShow(const std::string &lastName)
{
    auto &found = dbManager_.findStudentByLastName_Linear(lastName);

    if (found)
    {
        std::cout << "Student found by lastname! : ";
        showStudent(found);
    }
    else
    {
        std::cout << "Student not found. ";
    };
    std::cout << std::endl;
}

void menu::searchAndShow(const long int &PeselNr)
{
    auto found = dbManager_.findStudentByPesel_Binary(PeselNr);

    if (found)
    {
        std::cout << "Student found by PESEL number! : ";
        showStudent(*found);
    }
    else
    {
        std::cout << "Student not found. ";
    };
    std::cout << std::endl;
}

void menu::deleteUser(const int &IndexNr)
{
    if (dbManager_.deleteByIndexNr(IndexNr)) {std::cout << "SUCCESS! Student removed from database.";} else {std::cout << "ERROR! Student for remove not found!";};
    std::cout << "\n";
}

void menu::showStudent(const std::unique_ptr<Student> &person)
{
    std::cout << person->getFirstname() << " " << person->getLastname() << " - " << person->getAddress() << ", Index Nr: " << person->getIndexNr() << ", Pesel Nr: " << person->getPeselNr() << " Sex: " << person->sexToString(person->getSex());
}

void menu::showDb()
{
    std::cout << "\n ----------------------------------------- \n"
              << "     ORIGINAL VIEW OF STUDENTS DATABASE"
              << "\n ----------------------------------------- \n"
              << "All records: " << dbManager_.getCount() << "\n\n";
    for (auto &person : dbManager_.getFullList())
    {
        showStudent(person);
        std::cout << std::endl;
    };
    std::cout << std::endl;
};


void menu::showDbView_Pesel(Order O)
{
    std::cout << "\n ----------------------------------------- \n"
              << " TEMPORARY VIEW OF SORTED STUDENTS DATABASE"
              << "\n ----------------------------------------- \n"
              << "All records: " << dbManager_.getCount() <<
              " | Sort by PESEL - " << getOrderString(O) << "\n\n";
    for (auto &person : dbManager_.sortByPeselTemporary(O))
    {
        showStudent(*person);
        std::cout << std::endl;
    };
    std::cout << std::endl;
};

void menu::showDbView_LastName(Order O)
{
    std::cout << "\n ----------------------------------------- \n"
              << " TEMPORARY VIEW OF SORTED STUDENTS DATABASE"
              << "\n ----------------------------------------- \n"
              << "All records: " << dbManager_.getCount() <<
              " | Sort by last name - " << getOrderString(O) << "\n\n";
    for (auto &person : dbManager_.sortByLastNameTemporary(O))
    {
        showStudent(*person);
        std::cout << std::endl;
    };
    std::cout << std::endl;
};
