#include "menu.hpp"


void menu::help_command()
{
 std::cout << " Available commands to use in database:\n";

for (auto& entry : MapCommands_)
{
   std::cout << entry.first << "\n";
};
 std::cout << "\nType: [command] help to get help of specify command. Example: showdb help"; 
    
};

void menu::showdb_command()
{
    if (CommandArgs_.size() == 1) // simple showdb command
    {
        {
            showDb();
            return;
        };
    }

    if (CommandArgs_.at(1) == "help")
    {
        std::cout << "showdb - Display full view of original datebase\n"
                  << "You can use switch for get view of sorted list (without any change in database)\n"
                  << "Type: showdb sort [pesel/lastname] [asc/desc]\n"
                  << "Example: showdb sort pesel asc (Shows view sorted by PESEL in ascending order\n\n";
    };

    if (CommandArgs_.at(1) == "sort")
    {
        if (CommandArgs_.size() == 2)
        {
            std::cout << "Missing parameter - try: command + help\n";
            return;
        };

        if (CommandArgs_.at(2) == "pesel")
        {
            if (CommandArgs_.size() == 3)
            {
                std::cout << "Missing parameter - try: command + help\n";
                return;
            };

            if (CommandArgs_.at(3) == "asc")
            {
                showDbView_Pesel(Order::Asc);
                return;
            };

            if (CommandArgs_.at(3) == "desc")
            {
                showDbView_Pesel(Order::Desc);
                return;
            };
            std::cout << "Wrong parameter! - try: asc or desc\n";
        };

        if (CommandArgs_.at(2) == "lastname")
        {
            if (CommandArgs_.size() == 3)
            {
                std::cout << "Missing parameter - try: command + help\n";
                return;
            };
            if (CommandArgs_.at(3) == "asc")
            {
                showDbView_LastName(Order::Asc);
                return;
            };

            if (CommandArgs_.at(3) == "desc")
            {
                showDbView_LastName(Order::Desc);
                return;
            };
            std::cout << "Wrong parameter! - try: asc or desc\n";
        };
        std::cout << "Wrong parameter! - try: pesel or lastname\n";
    };

    std::cout << "Wrong parameter! - try: sort";
    return;
}