// university-db - Project for homework in CODERS SCHOOL (https:://coders,school)
/* 
Authors:
Main Code: Piotr K (https://github.com/piotrku91, piotrq.eu)
Function tokenize() from our another project: Piotr R (https://github.com/Rogal255)

*/

#include <iostream>
#include "Db.hpp"
#include "Generator.hpp"
#include "crow_all.h"

constexpr auto VERSION = "0.16 demo http web api";

bool readFileToString(std::string filename, std::string& content) {
    std::ifstream input(filename.c_str());
    std::string linestr;
    if (!input)
        return false;
    while (std::getline(input, linestr)) {
        if (linestr.size() > 0)
            content += linestr;
    }
    input.close();
    return true;
}

int main() {
    crow::SimpleApp HTTPapp;
    Db dbManager;
    std::string templateCode1;
    std::string templateCode2;
    readFileToString("templates/html1", templateCode1);
    readFileToString("templates/html2", templateCode2);

    CROW_ROUTE(HTTPapp, "/")
        .name("index")([&templateCode1, &templateCode2, &dbManager] {
            std::ostringstream tmpHTML;
            int z=0;
            for (auto& person : dbManager.getFullList()) {
                tmpHTML << "<tr><td>" + person->getFirstname() + "</td><td>" + person->getLastname() + "</td><td>" + person->getAddress() + "</td><td>" + std::to_string((Person::isTargetClassObject<Person, Student>(person.get())) ? (Person::isTargetClassObject<Person, Student>(person.get()))->getIndexNr() : 0) + "</td><td>" + person->getPeselNr() + "</td><td>" + person->sexToString(person->getSex()) + "</td><td>" + std::to_string((Person::isTargetClassObject<Person, Worker>(person.get())) ? (Person::isTargetClassObject<Person, Worker>(person.get()))->getSalary() : 0) + "</td><td>" + ((person->getPersonType() == PersonType::Student) ? "Student" : "Worker") + "</td></tr>";
            };
            return templateCode1 + tmpHTML.str() + std::to_string(z++) + templateCode2;
        });

    CROW_ROUTE(HTTPapp, "/operation").methods("GET"_method)([&](const crow::request& req, crow::response& res) {
        if (req.url_params.get("sort") != nullptr) {
            int a = boost::lexical_cast<int>(req.url_params.get("sort"));
            int b = 0;
            if (req.url_params.get("ord") != nullptr) {
             b = boost::lexical_cast<int>(req.url_params.get("ord"));
            }

            Order choosedOrder = (b == 0) ? Order::Asc : Order::Desc;

            switch (a) {
            case 0: {
                dbManager.sortByLastName(choosedOrder);
                break;
            }
            case 1: {
                dbManager.sortByPesel(choosedOrder);
                break;
            }
            case 2: {
                dbManager.sortBySalary(choosedOrder);
                break;
            }
            default: {
                break;
            };
            }
        }
        res.redirect("/");
        res.end();
    });

     CROW_ROUTE(HTTPapp, "/generate").methods("GET"_method)([&](const crow::request& req, crow::response& res) {
        if (req.url_params.get("amount") != nullptr) {
            int a = boost::lexical_cast<int>(req.url_params.get("amount"));
            Generator dbGen;
            
            auto validFuncPointer = &Db::peselValidator;
            for (auto& newGeneratedGuy : dbGen.generatePerson(&dbManager, validFuncPointer, a)) {
                dbManager.addPerson(newGeneratedGuy);
            }
        };
            res.redirect("/");
            res.end();
        });

        HTTPapp.port(18080).multithreaded().run();
}