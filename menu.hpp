#pragma once
#include <iostream>
#include "db.hpp"


class menu
{
    private:
 db& dbManager_;
public:
    void searchAndShow(const std::string &lastName);
    void searchAndShow(const long int& PeselNr);
    void showStudent(const std::unique_ptr<Student>& person);
    void showDb();

menu(db& dbManager): dbManager_(dbManager) {};
    
};