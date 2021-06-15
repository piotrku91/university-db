
#include <iostream>
#include "menu.hpp"

int main()
{
    db dbManager;
    menu mainMenu(dbManager);

    std::cout << "added? " << std::boolalpha << dbManager.addStudent("Roman", "Szpicruta", "Durnia 50", 29481, 90010120190, Sex::Male) << "\n";
    std::cout << "added? " << std::boolalpha << dbManager.addStudent("Anna", "Torbisko", "Flaszki 1", 29222, 89010120190, Sex::Female) << "\n";
    std::cout << "added? " << std::boolalpha << dbManager.addStudent("Tomek", "Kola", "Janka 2", 29121, 92010120190, Sex::Male) << "\n";
    std::cout << "added? " << std::boolalpha << dbManager.addStudent("Tomek", "Kola", "Janka 2", 29121, 92010120190, Sex::Male) << "\n";

    mainMenu.showDb();
    mainMenu.searchAndShow("Torbisko");
    mainMenu.searchAndShow(90010120190);
    std::cout << "deleted? " << std::boolalpha << dbManager.deleteByIndexNr(29121);
    mainMenu.showDb();
    mainMenu.searchAndShow(90010120190);
};