#pragma once
#include <iostream>
#include "db.hpp"

class menu
{
private:
    db &dbManager_;

public:
    // Main functions to communication with database
    void addNewUser(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType);
    void deleteUser(const  int &IndexNr);
    void searchAndShow(const std::string &lastName);
    void searchAndShow(const long int &PeselNr);
    void showStudent(const std::unique_ptr<Student> &person);
    void showDb();
    void showDbView_Pesel(Order O=Order::Asc);
    void showDbView_LastName(Order O=Order::Asc);
    std::string getOrderString(Order O) {if (O==Order::Asc) {return "Ascending";}; return "Descending";};

    void OldMainTests(); // TO DELETE 

    // Functions to communication with user and other
    void run() {mainLoop();};
    void mainLoop();

    menu(db &dbManager) : dbManager_(dbManager){};
};