
#include <iostream>
#include "db.hpp"

void showDb(db &dbManager)
{
    std::cout << "All records: " << dbManager.getFullList().size() << "\n\n";
    for (auto &person : dbManager.getFullList())
    {
        std::cout << person->getFirstname() << " " << person->getLastname() << " - " << person->getAddress() << ", Index Nr: " << person->getIndexNr() << ", Pesel Nr: " << person->getpeselNr() << " Sex: " << person->sexToString(person->getSex());
        std::cout << std::endl;
    };
    std::cout << std::endl;
};

int main()
{
    db dbManager;

    dbManager.addStudent("Roman", "Szpicruta", "Durnia 50", 29481, 90010120190, Sex::Male);
    dbManager.addStudent("Anna", "Torbisko", "Flaszki 1", 29222, 89010120190, Sex::Female);
    dbManager.addStudent("Tomek", "Kola", "Janka 2", 29121, 90010120190, Sex::Male);

    showDb(dbManager);
};