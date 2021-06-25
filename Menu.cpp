#include "Menu.hpp"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::addNewUser(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const std::string peselNr, const Sex sexType)
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
void Menu::searchAndShow_Lastname(const std::string &lastName)
{
    auto found = dbManager_.findStudentByLastName_Linear(lastName);

    if (found)
    {
        Display_prepareHeaderCaption("Student found by lastname");
        Display_prepareHeader();
        showStudent(*found);
        std::cout << std::endl;
        Display_prepareFooter();
    }
    else
    {
        std::cout << "Student not found. ";
    };
    std::cout << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::searchAndShow_Pesel(const std::string &PeselNr)
{
    auto found = dbManager_.findStudentByPesel_Binary(PeselNr);

    if (found)
    {

        Display_prepareHeaderCaption("Student found by PESEL number");
        Display_prepareHeader();
        showStudent(*found);
        std::cout << std::endl;
        Display_prepareFooter();
    }
    else
    {
        std::cout << "Student not found. ";
    };
    std::cout << std::endl;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::deleteUser(const int& indexNr)
{
    if (dbManager_.deleteByIndexNr(indexNr))
    {
        std::cout << "SUCCESS! Student removed from database.";
    }
    else
    {
        std::cout << "ERROR! Student for remove not found!";
    };
    std::cout << "\n";
}

void Menu::showStudent(const std::unique_ptr<Student> &person)
{
    std::cout << std::left;
    std::cout << " | " << std::setw(20) << person->getFirstname() << " | " << std::setw(20) << person->getLastname() << " | " << std::setw(20) << person->getAddress() << " | " << std::setw(20) << person->getIndexNr() << " | " << std::setw(20) << person->getPeselNr() << " | " << std::setw(20) << person->sexToString(person->getSex()) << " | ";

    // std::cout << person->getFirstname() << " " << person->getLastname() << " - " << person->getAddress() << ", Index Nr: " << person->getIndexNr() << ", Pesel Nr: " << person->getPeselNr() << " Sex: " << person->sexToString(person->getSex());
}

void Menu::showDb()
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
void Menu::showDbView_Pesel(Order O)
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
void Menu::showDbView_LastName(Order O)
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
std::string Menu::getOrderString(Order O)
{
    if (O == Order::Asc)
    {
        return "Ascending";
    };
    return "Descending";
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::tokenize(const std::string &t_UserCommand, const char t_Delim, std::vector<std::string> &t_Args)
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
void Menu::mainLoop()
{
    clear_command();
    while (!Exit_)
    {

        std::cout << "\n--------------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "piotrq university-db v." << appVersion_ << " | Type command to execute: " << std::endl;
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
void Menu::Display_prepareHeaderCaption(const std::string &Text)
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

void Menu::Display_prepareHeader()
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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Menu::Display_prepareFooter(const std::string &Text)
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

bool Menu::readFileToVector(std::string filename, std::vector<std::string> &content)
{
    std::ifstream input(filename.c_str());
    std::string linestr;
    if (!input)
        return false;
    while (std::getline(input, linestr))
    {
        if (linestr.size() > 0)
            content.push_back(linestr);
    }
    input.close();
    return true;
}

bool Menu::saveVectorToFile(std::string filename, std::vector<std::string> &content)
{

    std::ofstream OutFile(filename);
    if (!OutFile) {return false;};
    std::ostream_iterator<std::string> output_iterator(OutFile, "\n");
    std::copy(content.begin(), content.end(), output_iterator);
   
    OutFile.close();
    return true;
}