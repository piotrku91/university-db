#include "menu.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::help_command()
{
    std::cout << " Available commands to use in database:\n";

    for (auto &entry : MapCommands_)
    {
        std::cout << entry.first << "\n";
    };
    std::cout << "\nType: [command] help to get help of specify command. Example: showdb help";
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
                  << "Type: showdb by [pesel/lastname] [asc/desc]\n"
                  << "Example: showdb sort pesel asc (Shows view sorted by PESEL in ascending order\n\n";
    };

    if (CommandArgs_.at(1) == "by")
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::sortdb_command()
{
    if (CommandArgs_.size() == 1) // simple showdb command
    {
        {
            std::cout << "Missing parameter - try: command + help\n";
            return;
        };
    }

    if (CommandArgs_.at(1) == "help")
    {
        std::cout << "sortdb - Sorting full original datebase and returns original view\n"
                  << "You can use switch for set how you want to sort database\n"
                  << "Type: sortdb by [pesel/lastname] [asc/desc]\n"
                  << "Example: sortdb by pesel asc (Sorts database by PESEL in ascending order\n\n";
    };

    if (CommandArgs_.at(1) == "by")
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
                dbManager_.sortByPesel(Order::Asc);
                showDb();
                return;
            };

            if (CommandArgs_.at(3) == "desc")
            {
                dbManager_.sortByPesel(Order::Desc);
                showDb();
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
                dbManager_.sortByLastName(Order::Asc);
                showDb();
                return;
            };

            if (CommandArgs_.at(3) == "desc")
            {
                dbManager_.sortByLastName(Order::Desc);
                showDb();
                return;
            };
            std::cout << "Wrong parameter! - try: asc or desc\n";
        };
        std::cout << "Wrong parameter! - try: pesel or lastname\n";
    };

    std::cout << "Wrong parameter! - try: sort";
    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::add_command()
{
    if ((CommandArgs_.size() > 1) && (CommandArgs_.size() < 6))
    {
        {
            if (CommandArgs_.at(1) == "help")
            {
                std::cout << "add - Begins prodecdure of adding new student\n"
                          << "Type details part by part.\n"
                          << "Type: add\n"
                          << "Example: add\n\n";
            };
            std::cout << "Type: add\n";
            return;
        };
    }

    std::tuple<std::string, std::string, std::string, int, long int, Sex> tmpVars;

    std::cout << "Procedure of addition new strudent open\n\n";
    std::cout << "Type firstname of new student: ";
    std::getline(std::cin, std::get<0>(tmpVars));
    if ((std::get<0>(tmpVars) == "") || (std::any_of(std::get<0>(tmpVars).begin(), std::get<0>(tmpVars).end(), [](char x)
                                                     { return !std::isalpha(x); })))
    {
        std::cout << "Add procedure failed! - Wrong firstname. \n";
        return;
    };

    std::cout << "Type lastname of new student: ";
    std::getline(std::cin, std::get<1>(tmpVars));
    if ((std::get<1>(tmpVars) == "") || (std::any_of(std::get<1>(tmpVars).begin(), std::get<1>(tmpVars).end(), [](char x)
                                                     { return !std::isalpha(x); })))
    {
        std::cout << "Add procedure failed! - Wrong lastname. \n";
        return;
    };

    std::cout << "Type address of new student: ";
    std::getline(std::cin, std::get<2>(tmpVars));
    if ((std::get<2>(tmpVars) == ""))
    {
        std::cout << "Add procedure failed! - Wrong address. \n";
        return;
    };

    std::cout << "Type index number of new student: ";
    std::string tmpVar;
    std::getline(std::cin, tmpVar);
    if ((tmpVar == "") || (std::any_of(tmpVar.begin(), tmpVar.end(), [](char x)
                                       { return !std::isdigit(x); })))
    {
        std::cout << "Add procedure failed! - Wrong Index number . \n";
        return;
    };
    std::get<3>(tmpVars) = std::stoi(tmpVar);

    std::cout << "Type PESEL number of new student: ";
    std::getline(std::cin, tmpVar);
    if ((tmpVar == "") || (tmpVar.size() < 11) || (std::any_of(tmpVar.begin(), tmpVar.end(), [](char x)
                                                               { return !std::isdigit(x); })))
    {
        std::cout << "Add procedure failed! - Wrong PESEL number. \n";
        return;
    };
    std::get<4>(tmpVars) = std::stoll(tmpVar);

    std::cout << "Type sex of new student (try: f / m): ";
    std::getline(std::cin, tmpVar);
    if ((tmpVar == "") || ((std::tolower(tmpVar[0]) != 'f') && (std::tolower(tmpVar[0]) != 'm')))
    {
        std::cout << "Add procedure failed! - Wrong Sex. \n";
        return;
    };
    std::get<5>(tmpVars) = (tmpVar == "f") ? Sex::Female : Sex::Male;

    std::cout << "Attempt to add student to database... ";
    addNewUser(std::get<0>(tmpVars), std::get<1>(tmpVars), std::get<2>(tmpVars), std::get<3>(tmpVars), std::get<4>(tmpVars), std::get<5>(tmpVars));
    // std::apply(dbmanager_.addStudent,tmpVars);
    std::cout << "";
    return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::find_command()
{
    if (CommandArgs_.size() == 1) // simple showdb command
    {
        {
            std::cout << "Missing parameter - try: command + help\n";
            return;
        };
    }

    if (CommandArgs_.at(1) == "help")
    {
        std::cout << "find - Search for some record by pesel or lastname\n"
                  << "Type: find by [pesel/lastname] [record]\n"
                  << "Example: find by pesel 87070462648\n\n";
    };

    if (CommandArgs_.at(1) == "by")
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
            std::string tmpVar = CommandArgs_.at(3);
            if ((tmpVar == "") || (tmpVar.size() < 11) || (std::any_of(tmpVar.begin(), tmpVar.end(), [](char x)
                                                                       { return !std::isdigit(x); })))
            {
                std::cout << "Failed! - Wrong PESEL number. \n";
                return;
            };

            searchAndShow(std::stoll(tmpVar));
            return;
        };

        if (CommandArgs_.at(2) == "lastname")
        {
            if (CommandArgs_.size() == 3)
            {
                std::cout << "Missing parameter - try: command + help\n";
                return;
            };
            std::string tmpVar = CommandArgs_.at(3);
            if ((tmpVar == "") || (std::any_of(tmpVar.begin(), tmpVar.end(), [](char x)
                                               { return !std::isalpha(x); })))
            {
                std::cout << "Failed! - Wrong lastname. \n";
                return;
            };

            searchAndShow(tmpVar);
            return;
        };
    };
    std::cout << "Missing parameter or wrong parameter - try: command + help\n";
    return;
}

void menu::delete_command()
{
    if (CommandArgs_.size() == 1) // simple showdb command
    {
        {
            std::cout << "Missing parameter - try: command + help\n";
            return;
        };
    }

    if (CommandArgs_.at(1) == "help")
    {
        std::cout << "delete - Delete record from database\n"
                  << "Type: delete index [index number]\n"
                  << "Example: delete 12654\n\n";
    };

    if (CommandArgs_.at(1) == "index")
    {
        if (CommandArgs_.size() == 2)
        {
            std::cout << "Missing parameter - try: command + help\n";
            return;
        };

        std::string tmpVar = CommandArgs_.at(2);
        if ((tmpVar == "") || (std::any_of(tmpVar.begin(), tmpVar.end(), [](char x)
                                           { return !std::isdigit(x); })))
        {
            std::cout << "Failed! - Wrong Index number . \n";
            return;
        };

        deleteUser(std::stoi(tmpVar));
        return;
    };
    std::cout << "Missing parameter or wrong parameter - try: command + help\n";
    return;
}

void menu::save_command()
{
    if (CommandArgs_.size() == 1) // simple showdb command
    {

        if (dbManager_.saveToFile("data.db"))
        {
            std::cout << "\nSUCCESS! Database saved !\n";
        }
        else
        {
            std::cout << "\nERROR! Database not saved !\n";
        };
        return;
    }

    if (CommandArgs_.at(1) == "help")
    {
        std::cout << "save - Save database to file. Default save to data.db\n"
                  << "If you need specify name of file write as argument\n"
                  << "Type: save \n"
                  << "Type: save newfile.db\n"
                  << "Example: save\n\n";
    };

    std::string tmpVar = CommandArgs_.at(1);

    if (dbManager_.saveToFile(tmpVar))
    {
        std::cout << "\nSUCCESS! Database saved to " << tmpVar << " !\n";
    }
    else
    {
        std::cout << "\nERROR! Database not saved !\n";
    };
    return;
    return;
}

void menu::load_command()
{
    if (CommandArgs_.size() == 1) // simple showdb command
    {

        if (dbManager_.loadFromFile("data.db"))
        {
            std::cout << "\nSUCCESS! Database loaded !\n";
        }
        else
        {
            std::cout << "\nERROR! Database not loaded !\n";
        };
        return;
    }

    if (CommandArgs_.at(1) == "help")
    {
        std::cout << "load - load database from file. Default load from data.db\n"
                  << "If you need specify name of file write as argument\n"
                  << "Type: load \n"
                  << "Type: load newfile.db\n"
                  << "Example: load\n\n";
    };

    std::string tmpVar = CommandArgs_.at(1);

    if (dbManager_.loadFromFile(tmpVar))
    {
        std::cout << "\nSUCCESS! Database loaded from " << tmpVar << " !\n";
    }
    else
    {
        std::cout << "\nERROR! Database not loaded!\n";
    };
    return;
    return;
}

void menu::clear_command()
{
    if (system("clear"))
    {
    } // Clear console
}

void menu::generate_command()
{
    std::cout << "\nExporting database to HTML file.\n";
    std::vector<std::string> tmpHTML;
    if (dbManager_.getFullList().empty()) {std::cout << "\nDatabase is empty! Nothing to generate\n"; return;};

    readFileToVector("templates/html1", tmpHTML);

    for (auto &person : dbManager_.getFullList())
    {
        tmpHTML.push_back("<tr><td>" + person->getFirstname() + "</td><td>" + person->getLastname() + "</td><td>" + person->getAddress() + "</td><td>" + std::to_string(person->getIndexNr()) + "</td><td>" + std::to_string(person->getPeselNr()) + "</td><td>" + person->sexToString(person->getSex()) + "</td></tr>");
                std::cout
            << std::endl;
    };

    readFileToVector("templates/html2", tmpHTML);
    if (saveVectorToFile("generated.html",tmpHTML)) {std::cout << "\nDone.\n";} else {;std::cout << "\nError.\n";};
}
