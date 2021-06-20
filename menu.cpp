#include "menu.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
      case ErrorCheck::NotFound:
    {
        std::cout << "ERROR! - Student not found! \n";
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::searchAndShow(const std::string &lastName)
{
    auto found = dbManager_.findStudentByLastName_Linear(lastName);

    if (found)
    {
        Display_prepareHeaderCaption("Student found by lastname");
        Display_prepareHeader();
        showStudent(*found);
        Display_prepareFooter();
    }
    else
    {
        std::cout << "Student not found. ";
    };
    std::cout << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::searchAndShow(const long int &PeselNr)
{
    auto found = dbManager_.findStudentByPesel_Binary(PeselNr);

    if (found)
    {

        Display_prepareHeaderCaption("Student found by PESEL number");
        Display_prepareHeader();
        showStudent(*found);
        Display_prepareFooter();
    }
    else
    {
        std::cout << "Student not found. ";
    };
    std::cout << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::deleteUser(const int &IndexNr)
{
    if (dbManager_.deleteByIndexNr(IndexNr))
    {
        std::cout << "SUCCESS! Student removed from database.";
    }
    else
    {
        std::cout << "ERROR! Student for remove not found!";
    };
    std::cout << "\n";
}

void menu::showStudent(const std::unique_ptr<Student> &person)
{
    std::cout << std::left;
    std::cout << " | " << std::setw(20) << person->getFirstname() << " | " << std::setw(20) << person->getLastname() << " | " << std::setw(20) << person->getAddress() << " | " << std::setw(20) << person->getIndexNr() << " | " << std::setw(20) << person->getPeselNr() << " | " << std::setw(20) << person->sexToString(person->getSex()) << " | ";

    // std::cout << person->getFirstname() << " " << person->getLastname() << " - " << person->getAddress() << ", Index Nr: " << person->getIndexNr() << ", Pesel Nr: " << person->getPeselNr() << " Sex: " << person->sexToString(person->getSex());
}

void menu::showDb()
{
    Display_prepareHeaderCaption("ORIGINAL VIEW OF STUDENTS DATABASE");
    Display_prepareHeader();
    for (auto &person : dbManager_.getFullList())
    {
        showStudent(person);
        std::cout << std::endl;
    };
    Display_prepareFooter("All records: " + std::to_string(dbManager_.getCount()));
    std::cout << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::showDbView_Pesel(Order O)
{
    Display_prepareHeaderCaption("TEMPORARY VIEW OF SORTED STUDENTS DATABASE");
    Display_prepareHeader();
    for (auto &person : dbManager_.sortByPeselTemporary(O))
    {
        showStudent(*person);
        std::cout << std::endl;
    };
    Display_prepareFooter("All records: " + std::to_string(dbManager_.getCount()) + " | Sort by PESEL - " + getOrderString(O));
    std::cout << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::showDbView_LastName(Order O)
{

    Display_prepareHeaderCaption("TEMPORARY VIEW OF SORTED STUDENTS DATABASE");
    Display_prepareHeader();
    for (auto &person : dbManager_.sortByLastNameTemporary(O))
    {
        showStudent(*person);
        std::cout << std::endl;
    };
    Display_prepareFooter("All records: " + std::to_string(dbManager_.getCount()) + " | Sort by Lastname - " + getOrderString(O));
    std::cout << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string menu::getOrderString(Order O)
{
    if (O == Order::Asc)
    {
        return "Ascending";
    };
    return "Descending";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::OldMainTests()
{
    addNewUser("Roman", "Szpicruta", "Durnia 50", 29481, 90010120190, Sex::Male);
    addNewUser("Anna", "Torbisko", "Flaszki 1", 29222, 89010120190, Sex::Female);
    addNewUser("Tomek", "Kola", "Janka 2", 29121, 92010120190, Sex::Male);
    addNewUser("Tomek", "Kola", "Janka 2", 29128, 92010120190, Sex::Male);
    addNewUser("Danka", "Koziol", "Wuja 22", 20128, 42010120190, Sex::Female);
    addNewUser("Jurek", "Znicz", "Luny 222", 10128, 62010140190, Sex::Male);
    addNewUser("Zenobiusz", "Gorizek", "Stefana 232", 10127, 92090140190, Sex::Male);
    addNewUser("Piotr", "Ameba", "Stefana 232", 17127, 72090140190, Sex::Male);

    showDb();
    searchAndShow("Torbisko");
    searchAndShow(90010120190);
    deleteUser(10128);
    showDb();
    searchAndShow(90010120190);
    //dbManager.sortByPesel(Order::Asc);
    showDbView_Pesel();
    showDbView_LastName(Order::Asc);
    showDb();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::tokenize(const std::string &t_UserCommand, const char t_Delim, std::vector<std::string> &t_Args)
{
    size_t start;
    size_t end{0};

    while ((start = t_UserCommand.find_first_not_of(t_Delim, end)) != std::string::npos)
    {
        end = t_UserCommand.find(t_Delim, start);
        t_Args.push_back(t_UserCommand.substr(start, end - start));
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::mainLoop()
{
    while (!Exit_)
    {

        std::cout << "\n--------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "university-db | Type command to execute: " << std::endl;
        CommandArgs_.clear();
        UserCommand_.clear();

        std::getline(std::cin, UserCommand_);
        std::cout << std::endl;
        tokenize(UserCommand_, ' ', CommandArgs_);
        try
        {
            MapCommands_.at(CommandArgs_[0])();
        }
        catch (...)
        {

            std::cout << "\nUnknown command or ERROR!\n Type: help to check all available commands or: [command] help to get help of specify command.\n";
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu::Display_prepareHeaderCaption(const std::string &Text)
{
    std::cout << std::left;
    std::cout << std::setfill('-') << std::setw(120 + (6 * std::size(" | ")) - 5) << " +";
    std::cout << std::setw(0) << std::setfill(' ');
    std::cout << "+";

    std::cout << "\n";
    std::cout << " | " << std::setw(120 + (6 * std::size(" | ")) - 9) << Text << std::right << " |";
    std::cout << std::setw(0);
    std::cout << "\n";
}

void menu::Display_prepareHeader()
{
    std::cout << std::left;
    std::cout << std::setfill('-') << std::setw(120 + (6 * std::size(" | ")) - 5) << " +";
    std::cout << std::setw(0) << std::setfill(' ');
    std::cout << "+";

    std::cout << "\n";
    std::cout << " | " << std::setw(20) << "Firstname"
              << " | " << std::setw(20) << "Lastname"
              << " | " << std::setw(20) << "Address"
              << " | " << std::setw(20) << "Index number"
              << " | " << std::setw(20) << "Pesel number"
              << " | " << std::setw(20) << "Sex"
              << " | " << std::endl;
    std::cout << std::setw(0);

    std::cout << std::setfill('-') << std::setw(120 + (6 * std::size(" | ")) - 5) << " +";
    std::cout << std::setw(0) << std::setfill(' ');
    std::cout << "+\n";
}

void menu::Display_prepareFooter(const std::string &Text)
{

    std::cout << std::setfill('-') << std::setw(120 + (6 * std::size(" | ")) - 5) << " +";
    std::cout << std::setw(0) << std::setfill(' ');
    std::cout << "+";

    std::cout << "\n";
    std::cout << " | " << std::setw(120 + (6 * std::size(" | ")) - 9) << Text << " |";
    std::cout << std::setw(0);
    std::cout << "\n";

    std::cout << std::setfill('-') << std::setw(120 + (6 * std::size(" | ")) - 5) << " +";
    std::cout << std::setw(0) << std::setfill(' ');
    std::cout << "+\n";
}