#include "generator.hpp"
#include <random>


std::shared_ptr<Person> Generator::generatePerson(std::function<bool(const std::string& peselNr, Sex sexType)> peselValidatorFunction)
{
if (firstnames_.empty() || lastnames_.empty() || addresses_.empty()) {return nullptr;};

std::uniform_int_distribution<int> rndType{0,1};
auto tmpType = (rndType(rd)==0)?PersonType::Student:PersonType::Worker;

std::uniform_int_distribution<int> rndIdxFirstnames{0, static_cast<int>(firstnames_.size()-1)};
auto tmpFirstname = firstnames_[rndIdxFirstnames(rd)];

std::uniform_int_distribution<int> rndIdxLastnames{0, static_cast<int>(lastnames_.size()-1)};
auto tmpLastname = lastnames_[rndIdxLastnames(rd)];

std::uniform_int_distribution<int> rndIdxAdressess{0, static_cast<int>(addresses_.size()-1)};
auto tmpAddress = addresses_[rndIdxAdressess(rd)];

std::uniform_int_distribution<int> rndSex{0,1};
auto tmpSex = (rndSex(rd)==0)?Sex::Male:Sex::Female;

auto tmpIndexNr=0;  
auto tmpSalary=0;  

if (tmpType==PersonType::Student) {
std::uniform_int_distribution<int> rndIndexNr{0, 99999};
tmpIndexNr = rndIndexNr(rd);
} 
else
{
std::uniform_int_distribution<int> rndSalary{0, 99999};
tmpSalary = rndSalary(rd);
}


std::string tmpPeselNr;
std::uniform_int_distribution<int> rndIdxPesels{0, 9};
for (int i=0; i<10; ++i)
{
tmpPeselNr+=static_cast<char>(rndIdxPesels(rd));
}

for (int i=0; i<10; ++i)
{
if (peselValidatorFunction((tmpPeselNr+std::to_string(i)),tmpSex)) {
tmpPeselNr+=std::to_string(i);
break;
}
}

if (tmpType==PersonType::Student) {
return Person::createPerson<Student>(Student{tmpFirstname, tmpLastname, tmpAddress, tmpIndexNr, tmpPeselNr, tmpSex});
}
return Person::createPerson<Worker>(Worker{tmpFirstname, tmpLastname, tmpAddress, tmpPeselNr, tmpSex, tmpSalary});

}


bool Generator::readFileToVector(std::string filename, std::vector<std::string>& content) {
    std::ifstream input(filename.c_str());
    std::string linestr;
    if (!input)
        return false;
    while (std::getline(input, linestr)) {
        if (linestr.size() > 0)
            content.push_back(linestr);
    }
    input.close();
    return true;
}

bool Generator::saveVectorToFile(std::string filename, std::vector<std::string>& content) {
    std::ofstream OutFile(filename);
    if (!OutFile) {
        return false;
    };
    std::ostream_iterator<std::string> output_iterator(OutFile, "\n");
    std::copy(content.begin(), content.end(), output_iterator);

    OutFile.close();
    return true;
}
