
#include <iostream>
#include "menu.hpp"

int main()
{
    db dbManager;
    menu mainMenu(dbManager);

    mainMenu.addNewUser("Roman", "Szpicruta", "Durnia 50", 29481, 90010120190, Sex::Male);
    mainMenu.addNewUser("Anna", "Torbisko", "Flaszki 1", 29222, 89010120190, Sex::Female);
    mainMenu.addNewUser("Tomek", "Kola", "Janka 2", 29121, 92010120190, Sex::Male);
    mainMenu.addNewUser("Tomek", "Kola", "Janka 2", 29128, 92010120190, Sex::Male);
    mainMenu.addNewUser("Danka", "Koziol", "Wuja 22", 20128, 42010120190, Sex::Female);
    mainMenu.addNewUser("Jurek", "Znicz", "Luny 222", 10128, 62010140190, Sex::Male);
    mainMenu.addNewUser("Zenobiusz", "Gorizek", "Stefana 232", 10127, 92090140190, Sex::Male);
     mainMenu.addNewUser("Piotr", "Ameba", "Stefana 232", 17127, 72090140190, Sex::Male);

    mainMenu.showDb();
    mainMenu.searchAndShow("Torbisko");
    mainMenu.searchAndShow(90010120190);
    mainMenu.deleteUser(29121);
    mainMenu.showDb();
    mainMenu.searchAndShow(90010120190);
    //dbManager.sortByPesel(Order::Asc);
    mainMenu.showDbView_Pesel();
    mainMenu.showDbView_LastName(Order::Asc);
    mainMenu.showDb();
}