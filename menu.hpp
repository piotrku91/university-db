#pragma once
#include <ctype.h>
#include <iostream>
#include <iterator>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <iomanip>
#include <tuple>
#include "stdlib.h"
#include "db.hpp"

class menu
{
private:
    db &dbManager_;

    std::string UserCommand_;
    std::vector<std::string> CommandArgs_;
    bool Exit_{false};
    std::vector<std::string> m_CommandArgs;

    std::map<std::string, std::function<void()>> MapCommands_{
        {"add", [this]()
         { add_command(); }},
        {"help", [this]()
         { help_command(); }},
        {"showdb", [this]()
         { showdb_command(); }},
        {"sortdb", [this]()
         { sortdb_command(); }},
        {"find", [this]()
         { find_command(); }},

        {"delete", [this]()
         { delete_command(); }},
        {"save", [this]()
         { save_command(); }},
        {"load", [this]()
         { load_command(); }},
         {"generate", [this]()
         { generate_command(); }},
        {"clear", [this]()
         { clear_command(); }},
        {"exit", [this]()
         { Exit_ = true; }}};

public:
    // Main functions to communication with database
    void addNewUser(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const std::string peselNr, const Sex sexType);
    void deleteUser(const int &IndexNr);
    void searchAndShow_Lastname(const std::string &lastName);
    void searchAndShow_Pesel(const std::string &PeselNr);
    void showStudent(const std::unique_ptr<Student> &person);
    void showDb();
    void showDbView_Pesel(Order O = Order::Asc);
    void showDbView_LastName(Order O = Order::Asc);
    void OldMainTests(); // TO DELETE
    std::string getOrderString(Order O);

    // Functions / Helpers for display
    void Display_prepareHeaderCaption(const std::string &Text);
    void Display_prepareHeader();
    void Display_prepareFooter(const std::string &Text = "");

    // Functions to communication with user and other
    void run() { mainLoop(); };
    void mainLoop();
    void tokenize(const std::string &t_UserCommand, const char t_Delim, std::vector<std::string> &t_Args);
   bool readFileToVector(std::string filename, std::vector<std::string>& content);
   bool saveVectorToFile(std::string filename, std::vector<std::string>& content);

    void showdb_command();
    void sortdb_command();
    void help_command();
    void add_command();
    void find_command();
    void delete_command();
    void clear_command();
    void save_command();
    void load_command();
    void generate_command();

    menu(db &dbManager) : dbManager_(dbManager){};
};