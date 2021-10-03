#pragma once
#include <chrono>
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
    std::mt19937 g;

    bool readFileToVector(std::string filename, std::vector<std::string>& content);
    bool saveVectorToFile(std::string filename, std::vector<std::string>& content);
    typedef std::chrono::high_resolution_clock clock_;
    clock_::time_point beginT;

public:
    long int time_reseed() {
        clock_::duration dur = clock_::now() - beginT;
        long int seed = dur.count();
        return seed;
    }

    template <typename T1, typename T2>
    std::vector<std::shared_ptr<Person>> generatePerson(T1* dbManagerPtr, T2&& peselValidatorFunction, size_t amountToGenerate) {
        std::vector<std::shared_ptr<Person>> tmpGenerated(amountToGenerate);

        std::generate(tmpGenerated.begin(), tmpGenerated.end(), [&]() {
            return generatePerson(dbManagerPtr, peselValidatorFunction);
        });
        return tmpGenerated;
    }

    template <typename T1, typename T2>
    std::shared_ptr<Person> generatePerson(T1* dbManagerPtr, T2&& peselValidatorFunction) {
        if (firstnames_.empty() || lastnames_.empty() || addresses_.empty()) {
            return nullptr;
        };
        time_reseed();
        std::uniform_int_distribution<int> rndType{0, 1};
        auto tmpType = (rndType(g) == 0) ? PersonType::Student : PersonType::Worker;

        std::uniform_int_distribution<int> rndIdxFirstnames{0, static_cast<int>(firstnames_.size() - 1)};
        auto tmpFirstname = firstnames_[rndIdxFirstnames(g)];

        std::uniform_int_distribution<int> rndIdxLastnames{0, static_cast<int>(lastnames_.size() - 1)};
        auto tmpLastname = lastnames_[rndIdxLastnames(g)];

        std::uniform_int_distribution<int> rndIdxAdressess{0, static_cast<int>(addresses_.size() - 1)};
        auto tmpAddress = addresses_[rndIdxAdressess(g)];

        // std::uniform_int_distribution<int> rndSex{0, 1};

        auto tmpIndexNr = 0;
        auto tmpSalary = 0;

        if (tmpType == PersonType::Student) {
            std::uniform_int_distribution<int> rndIndexNr{1, 99999999};
            tmpIndexNr = rndIndexNr(g);
        } else {
            std::uniform_int_distribution<int> rndSalary{1, 99999999};
            tmpSalary = rndSalary(g);
        }
        std::string tmpPeselNr = "";
        for (int i = 0; i < 10; ++i) {
            std::uniform_int_distribution<int> rndIdxPesels{0, 9};
            tmpPeselNr += std::to_string(rndIdxPesels(g));
        }

        auto tmpSex = (tmpPeselNr[9] % 2 == 0) ? Sex::Female : Sex::Male;

        for (int i = 0; i < 10; i++) {
            if ((dbManagerPtr->*peselValidatorFunction)(tmpPeselNr + std::to_string(i), tmpSex)) {
                tmpPeselNr += std::to_string(i);
                break;
            }
        }

        if (tmpType == PersonType::Student) {
            return Person::createPerson<Student>(Student{tmpFirstname, tmpLastname, tmpAddress, tmpIndexNr, tmpPeselNr, tmpSex});
        }
        return Person::createPerson<Worker>(Worker{tmpFirstname, tmpLastname, tmpAddress, tmpPeselNr, tmpSex, tmpSalary});
    }

    Generator()
        : beginT(clock_::now()) {  // c-tor
        time_reseed();
        readFileToVector("templates/firstnames.txt", firstnames_);
        readFileToVector("templates/lastnames.txt", lastnames_);
        readFileToVector("templates/addresses.txt", addresses_);
    };
};