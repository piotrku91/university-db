#pragma once
#include <fstream>
#include <functional>
#include <iterator>
#include <memory>
#include <random>
#include <string>
#include "Person.hpp"

class Generator {
private:
    std::vector<std::string> firstnames_;
    std::vector<std::string> lastnames_;
    std::vector<std::string> addresses_;
    std::random_device rd{};
    std::mt19937 g{rd()};

    bool readFileToVector(std::string filename, std::vector<std::string>& content);
    bool saveVectorToFile(std::string filename, std::vector<std::string>& content);

public:
    std::shared_ptr<Person> generatePerson(std::function<bool(const std::string& peselNr, Sex sexType)> peselValidatorFunction);

Generator(){ // c-tor
readFileToVector("templates/firstnames.txt",firstnames_);
readFileToVector("templates/lastnames.txt",lastnames_);
readFileToVector("templates/address.txt",lastnames_);
};

};