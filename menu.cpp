#include "menu.hpp"

void menu::searchAndShow(const std::string &lastName)
{
    auto &found = dbManager_.findStudentByLastName(lastName);

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
    auto &found = dbManager_.findStudentByPesel(PeselNr);

    if (found)
    {
        std::cout << "Student found by PESEL number! : ";
        showStudent(found);
    }
    else
    {
        std::cout << "Student not found. ";
    };
    std::cout << std::endl;
}

void menu::showStudent(const std::unique_ptr<Student> &person)
{
    std::cout << person->getFirstname() << " " << person->getLastname() << " - " << person->getAddress() << ", Index Nr: " << person->getIndexNr() << ", Pesel Nr: " << person->getpeselNr() << " Sex: " << person->sexToString(person->getSex());
}

void menu::showDb()
{
    std::cout << "\n -----------------------------------------" << "\n" << "All records: " << dbManager_.getFullList().size() << "\n\n";
    for (auto &person : dbManager_.getFullList())
    {
        showStudent(person);
        std::cout << std::endl;
    };
    std::cout << std::endl;
};