#include <vector>
#include "Db.hpp"
#include "gtest/gtest.h"
#include "generator.hpp"

struct MainOperations : public ::testing::Test {
    Db dbManager{false};
};


TEST_F(MainOperations, ShouldAddNewStudent) {
    EXPECT_EQ(dbManager.getCount(), 0);
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    EXPECT_EQ(dbManager.getCount(), 1);
}

TEST_F(MainOperations, ShouldAddNewStudentValid) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    EXPECT_EQ(dbManager.getFullList()[0]->getFirstname(), "Roman");
    EXPECT_EQ(dbManager.getFullList()[0]->getLastname(), "Szpicruta");
    EXPECT_EQ(dbManager.getFullList()[0]->getAddress(), "Durnia 50");
    if (Person::isTargetClassObject<Person, Student>(dbManager.getFullList()[0].get())) {
        EXPECT_EQ((Person::isTargetClassObject<Person, Student>(dbManager.getFullList()[0].get()))->getIndexNr(), 29481);
    };
    EXPECT_EQ(dbManager.getFullList()[0]->getPeselNr(), "90121464913");
    EXPECT_EQ(dbManager.getFullList()[0]->getSex(), Sex::Male);
}

TEST_F(MainOperations, ShouldAddMoreStudents) {
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female), ErrorCheck::OK);
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male), ErrorCheck::OK);
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female), ErrorCheck::OK);
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male), ErrorCheck::OK);
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male), ErrorCheck::OK);
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getCount(), 6);
}

TEST_F(MainOperations, ShouldANotAddDuplicatesIndex) {
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "49032157276", Sex::Male), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getCount(), 1);
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "66011781239", Sex::Male), ErrorCheck::IndexInUse);
    EXPECT_EQ(dbManager.getCount(), 1);
}

TEST_F(MainOperations, ShouldANotAddDuplicatesPesel) {
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 21123, "92121377633", Sex::Male), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getCount(), 1);
    EXPECT_EQ(dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 21122, "92121377633", Sex::Male), ErrorCheck::PeselInUse);
    EXPECT_EQ(dbManager.getCount(), 1);
}

TEST_F(MainOperations, ShouldFindByLastName) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);
    auto found = dbManager.findPersonByLastName_Linear("Znicz");
    ASSERT_TRUE(found);

    EXPECT_EQ(found->getFirstname(), "Jurek");
    EXPECT_EQ(found->getLastname(), "Znicz");
    EXPECT_EQ(found->getAddress(), "Luny 222");
    if (Person::isTargetClassObject<Person, Student>(found.get())) {
        EXPECT_EQ((Person::isTargetClassObject<Person, Student>(found.get()))->getIndexNr(), 10128);
    };
    EXPECT_EQ(found.get()->getPeselNr(), "66011781239");
    EXPECT_EQ(found.get()->getSex(), Sex::Male);
}

TEST_F(MainOperations, ShouldNotFindByLastName) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);

    auto found = dbManager.findPersonByLastName_Linear("Bandzior");
    EXPECT_EQ(found, nullptr);
}

TEST_F(MainOperations, ShouldFindByPesel) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);
    auto found = dbManager.findPersonByPesel_Binary("66011781239");
    ASSERT_TRUE(found);

    EXPECT_EQ(found.get()->getFirstname(), "Jurek");
    EXPECT_EQ(found.get()->getLastname(), "Znicz");
    EXPECT_EQ(found.get()->getAddress(), "Luny 222");
    if (Person::isTargetClassObject<Person, Student>(found.get())) {
        EXPECT_EQ((Person::isTargetClassObject<Person, Student>(found.get()))->getIndexNr(), 10128);
    };
    EXPECT_EQ(found.get()->getPeselNr(), "66011781239");
    EXPECT_EQ(found.get()->getSex(), Sex::Male);
}

TEST_F(MainOperations, ShouldNotFindByPesel) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);
    auto found = dbManager.findPersonByPesel_Binary("5454544544");
    EXPECT_EQ(found, nullptr);
}

TEST_F(MainOperations, ShouldDelete) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);
    EXPECT_EQ(dbManager.getCount(), 7);
    EXPECT_TRUE(dbManager.findPersonByIdx_Binary(10128));
    EXPECT_TRUE(dbManager.deleteByIndexNr(10128));
    EXPECT_FALSE(dbManager.findPersonByIdx_Binary(10128));
    EXPECT_EQ(dbManager.getCount(), 6);
}

TEST_F(MainOperations, ShouldSortByLastName) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);
    EXPECT_EQ(dbManager.getFullList().back()->getLastname(), "Ameba");
    dbManager.sortByLastName(Order::Desc);
    EXPECT_EQ(dbManager.getFullList().back()->getLastname(), "Ameba");
    dbManager.sortByLastName(Order::Asc);
    EXPECT_EQ(dbManager.getFullList().front()->getLastname(), "Ameba");
}

TEST_F(MainOperations, ShouldSortByPesel) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);

    dbManager.sortByPesel(Order::Desc);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "95042936259");
    dbManager.sortByPesel(Order::Asc);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "54022845648");
}

TEST_F(MainOperations, ShouldSave_EraseDatabase_And_LoadFromFile) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);
    EXPECT_TRUE(dbManager.saveToFile("test_data.db"));
    EXPECT_EQ(dbManager.getCount(), 7);
    dbManager.eraseDatabase();
    ASSERT_EQ(dbManager.getCount(), 0);
    EXPECT_TRUE(dbManager.loadFromFile("test_data.db"));
    EXPECT_EQ(dbManager.getCount(), 7);
}

TEST_F(MainOperations, ShouldModify) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addPerson(PersonType::Student, "Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);
    ASSERT_NE(dbManager.getCount(), 0);
    dbManager.sortByPesel(Order::Asc);
    EXPECT_EQ(dbManager.getFullList().front()->getFirstname(), "Danka");
    EXPECT_EQ(dbManager.findPersonAndModifyFirstname(dbManager.getFullList().front()->getPeselNr(), "Danuta"), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getFullList().front()->getFirstname(), "Danuta");

    EXPECT_EQ(dbManager.getFullList().front()->getLastname(), "Koziol");
    EXPECT_EQ(dbManager.findPersonAndModifyLastname(dbManager.getFullList().front()->getPeselNr(), "Kozlowska"), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getFullList().front()->getLastname(), "Kozlowska");

    EXPECT_EQ(dbManager.getFullList().front()->getAddress(), "Wuja 22");
    EXPECT_EQ(dbManager.findPersonAndModifyAddress(dbManager.getFullList().front()->getPeselNr(), "Luny 134"), ErrorCheck::OK);

    EXPECT_EQ(dbManager.findPersonAndModifyindexNr(dbManager.getFullList().front()->getPeselNr(), 10000), ErrorCheck::OK);
    if (Person::isTargetClassObject<Person, Student>(dbManager.getFullList().front().get())) {
        EXPECT_EQ((Person::isTargetClassObject<Person, Student>(dbManager.getFullList().front().get()))->getIndexNr(), 10000);
    };

    EXPECT_EQ(dbManager.findPersonAndModifypeselNr(dbManager.getFullList().front()->getPeselNr(), "49081828686"), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "49081828686");
}

TEST_F(MainOperations, ShouldNotModifyIndexWhenIsDuplicate) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    EXPECT_EQ(dbManager.findPersonAndModifyindexNr(dbManager.getFullList().front()->getPeselNr(), 29481), ErrorCheck::IndexInUse);
    if (Person::isTargetClassObject<Person, Student>(dbManager.getFullList().front().get())) {
        EXPECT_EQ((Person::isTargetClassObject<Person, Student>(dbManager.getFullList().front().get()))->getIndexNr(), 29481);
    };
    }

TEST_F(MainOperations, ShouldNotModifyPeselWhenIsDuplicate) {
    dbManager.addPerson(PersonType::Student, "Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    dbManager.addPerson(PersonType::Student, "Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    EXPECT_EQ(dbManager.findPersonAndModifypeselNr(dbManager.getFullList().front()->getPeselNr(), "87070462648"), ErrorCheck::PeselInUse);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "90121464913");
}

TEST_F(MainOperations, ShouldValidatePesel) {
    dbManager.enablePeselValidation(true);
    EXPECT_TRUE(dbManager.peselValidator("55030101193", Sex::Male));
    EXPECT_FALSE(dbManager.peselValidator("55030101193", Sex::Female));
    EXPECT_FALSE(dbManager.peselValidator("55030101197", Sex::Male));
    EXPECT_TRUE(dbManager.peselValidator("01240239252", Sex::Male));
    EXPECT_FALSE(dbManager.peselValidator("01240239252", Sex::Female));
    EXPECT_TRUE(dbManager.peselValidator("15210214538", Sex::Male));
}

TEST_F(MainOperations, ShouldAddWorkerTypeAccount) {
    EXPECT_EQ(dbManager.addPerson(PersonType::Worker, "Jurek", "Tir", "Flamangfa 3", 0, "47031223895", Sex::Male, 2300), ErrorCheck::OK);
    EXPECT_EQ((Person::isTargetClassObject<Person, Worker>(dbManager.getFullList().back().get()))->getSalary(), 2300);
}

TEST_F(MainOperations, SortBySalary) {
    EXPECT_EQ(dbManager.addPerson(PersonType::Worker, "Janek", "Czomolungma", "Patison 23", 0, "88051187895", Sex::Male, 3900), ErrorCheck::OK);
    dbManager.sortBySalary(Order::Desc);
    EXPECT_EQ((Person::isTargetClassObject<Person, Worker>(dbManager.getFullList().front().get()))->getSalary(), 3900);
}

TEST_F(MainOperations, ShouldFindByPeselAndModifySalary) {
    EXPECT_EQ(dbManager.addPerson(PersonType::Worker, "Jurek", "Tir", "Flamangfa 3", 0, "47031223895", Sex::Male, 2300), ErrorCheck::OK);
    EXPECT_EQ(dbManager.addPerson(PersonType::Worker, "Janek", "Czomolungma", "Patison 23", 0, "88051187895", Sex::Male, 3900), ErrorCheck::OK);
    EXPECT_EQ(dbManager.findPersonAndModifySalary("88051187895", 4000), ErrorCheck::OK);
    EXPECT_EQ((Person::isTargetClassObject<Person, Worker>(dbManager.getFullList().back().get()))->getSalary(), 4000);
}


TEST_F(MainOperations, ShouldAddNewStudentFromGenerator) {
    Generator dbGen;
    EXPECT_EQ(dbManager.getCount(), 0);
    auto validFuncPointer= &Db::peselValidator;
    dbManager.addPerson(dbGen.generatePerson(&dbManager,validFuncPointer));
    EXPECT_EQ(dbManager.getCount(), 1);
} 

TEST_F(MainOperations, ShouldAddNewStudentByAmountFromGenerator) {
    Generator dbGen;
    EXPECT_EQ(dbManager.getCount(), 0);
    auto validFuncPointer= &Db::peselValidator;
    for (auto& newGeneratedGuy : dbGen.generatePerson(&dbManager,validFuncPointer,5)) {
    dbManager.addPerson(newGeneratedGuy);
    }
    EXPECT_EQ(dbManager.getCount(), 5);
} 

