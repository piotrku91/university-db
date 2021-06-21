#include <vector>
#include "gtest/gtest.h"
#include "db.hpp"

db dbManager(false);

TEST(MainOperations, ShouldAddNewStudent)
{
    EXPECT_EQ(dbManager.getCount(), 0);               
    dbManager.addStudent("Roman", "Szpicruta", "Durnia 50", 29481, "90121464913", Sex::Male);
    EXPECT_EQ(dbManager.getCount(), 1);
}

TEST(MainOperations, ShouldAddNewStudentValid)
{
    if (dbManager.getFullList().empty()) return;
    EXPECT_EQ(dbManager.getFullList()[0]->getFirstname(), "Roman");
    EXPECT_EQ(dbManager.getFullList()[0]->getLastname(), "Szpicruta");
    EXPECT_EQ(dbManager.getFullList()[0]->getAddress(), "Durnia 50");
    EXPECT_EQ(dbManager.getFullList()[0]->getIndexNr(), 29481);
    EXPECT_EQ(dbManager.getFullList()[0]->getPeselNr(), "90121464913");
    EXPECT_EQ(dbManager.getFullList()[0]->getSex(), Sex::Male);
}

TEST(MainOperations, ShouldAddMoreStudents)
{
    EXPECT_EQ(dbManager.getCount(), 1);
    dbManager.addStudent("Anna", "Torbisko", "Flaszki 1", 29222, "87070462648", Sex::Female);
    dbManager.addStudent("Tomek", "Kola", "Janka 2", 29121, "92121377633", Sex::Male);
    dbManager.addStudent("Danka", "Koziol", "Wuja 22", 20128, "54022845648", Sex::Female);
    dbManager.addStudent("Jurek", "Znicz", "Luny 222", 10128, "66011781239", Sex::Male);
    dbManager.addStudent("Zenobiusz", "Gorizek", "Stefana 232", 10127, "95042936259", Sex::Male);
    dbManager.addStudent("Piotr", "Ameba", "Stefana 232", 17127, "67051489435", Sex::Male);
    EXPECT_EQ(dbManager.getCount(), 7);
}

TEST(MainOperations, ShouldANotAddDuplicatesIndex)
{
    EXPECT_EQ(dbManager.getCount(), 7);
    EXPECT_EQ(dbManager.addStudent("Tomek", "Kola", "Janka 2", 29121, "49032157276", Sex::Male), ErrorCheck::IndexInUse);
    EXPECT_EQ(dbManager.getCount(), 7);
}

TEST(MainOperations, ShouldANotAddDuplicatesPesel)
{
    EXPECT_EQ(dbManager.getCount(), 7);
    dbManager.addStudent("Tomek", "Kola", "Janka 2", 21123, "92121377633", Sex::Male);
    EXPECT_EQ(dbManager.getCount(), 7);
}

TEST(MainOperations, ShouldFindByLastName)
{
    auto found = dbManager.findStudentByLastName_Linear("Znicz");
    EXPECT_TRUE(found);

    if (found)
    {
        EXPECT_EQ(found->get()->getFirstname(), "Jurek");
        EXPECT_EQ(found->get()->getLastname(), "Znicz");
        EXPECT_EQ(found->get()->getAddress(), "Luny 222");
        EXPECT_EQ(found->get()->getIndexNr(), 10128);
        EXPECT_EQ(found->get()->getPeselNr(), "66011781239");
        EXPECT_EQ(found->get()->getSex(), Sex::Male);
    }
}

TEST(MainOperations, ShouldNotFindByLastName)
{
    auto found = dbManager.findStudentByLastName_Linear("Bandzior");
    EXPECT_EQ(found, nullptr);
}

TEST(MainOperations, ShouldFindByPesel)
{
    auto found = dbManager.findStudentByPesel_Binary("66011781239");
    EXPECT_TRUE(found);

    if (found)
    {
        EXPECT_EQ(found->get()->getFirstname(), "Jurek");
        EXPECT_EQ(found->get()->getLastname(), "Znicz");
        EXPECT_EQ(found->get()->getAddress(), "Luny 222");
        EXPECT_EQ(found->get()->getIndexNr(), 10128);
        EXPECT_EQ(found->get()->getPeselNr(), "66011781239");
        EXPECT_EQ(found->get()->getSex(), Sex::Male);
    }
}

TEST(MainOperations, ShouldNotFindByPesel)
{
    auto found = dbManager.findStudentByPesel_Binary("5454544544");
    EXPECT_EQ(found, nullptr);
}

TEST(MainOperations, ShouldDelete)
{
    EXPECT_EQ(dbManager.getCount(), 7);
    EXPECT_TRUE(dbManager.findStudentByIdx_Binary(10128));
    EXPECT_TRUE(dbManager.deleteByIndexNr(10128));
    EXPECT_FALSE(dbManager.findStudentByIdx_Binary(10128));
    EXPECT_EQ(dbManager.getCount(), 6);
}

TEST(MainOperations, ShouldSortByLastName)
{
    EXPECT_EQ(dbManager.getFullList().back()->getLastname(), "Ameba");
    dbManager.sortByLastName(Order::Desc);
    EXPECT_EQ(dbManager.getFullList().back()->getLastname(), "Ameba");
    dbManager.sortByLastName(Order::Asc);
    EXPECT_EQ(dbManager.getFullList().front()->getLastname(), "Ameba");
}

TEST(MainOperations, ShouldSortByPesel)
{
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "67051489435");
    dbManager.sortByPesel(Order::Desc);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "95042936259");
    dbManager.sortByPesel(Order::Asc);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "54022845648");
}

TEST(MainOperations, ShouldSaveToFile)
{
    EXPECT_TRUE(dbManager.saveToFile("test_data.db"));
}

TEST(MainOperations, ShouldEraseDatabaseAndLoadFromFile)
{
    EXPECT_EQ(dbManager.getCount(), 6);
    dbManager.eraseDatabase();
    EXPECT_EQ(dbManager.getCount(), 0);
    EXPECT_TRUE(dbManager.loadFromFile("test_data.db"));
    EXPECT_EQ(dbManager.getCount(), 6);
}

TEST(MainOperations, ShouldModify)
{
    EXPECT_EQ(dbManager.getFullList().front()->getFirstname(), "Danka");
    EXPECT_EQ(dbManager.findStudentAndModifyFirstname(dbManager.getFullList().front()->getIndexNr(), "Danuta"), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getFullList().front()->getFirstname(), "Danuta");

    EXPECT_EQ(dbManager.getFullList().front()->getLastname(), "Koziol");
    EXPECT_EQ(dbManager.findStudentAndModifyLastname(dbManager.getFullList().front()->getIndexNr(), "Kozlowska"), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getFullList().front()->getLastname(), "Kozlowska");

    EXPECT_EQ(dbManager.getFullList().front()->getAddress(), "Wuja 22");
    EXPECT_EQ(dbManager.findStudentAndModifyAddress(dbManager.getFullList().front()->getIndexNr(), "Luny 134"), ErrorCheck::OK);

    EXPECT_EQ(dbManager.findStudentAndModifyIndexNr(dbManager.getFullList().front()->getIndexNr(), 10000), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getFullList().front()->getIndexNr(), 10000);

    EXPECT_EQ(dbManager.findStudentAndModifyPeselNr(dbManager.getFullList().front()->getIndexNr(), "49081828686"), ErrorCheck::OK);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "49081828686");
}

TEST(MainOperations, ShouldNotModifyIndexWhenIsDuplicate)
{
    EXPECT_EQ(dbManager.findStudentAndModifyIndexNr(dbManager.getFullList().front()->getIndexNr(), 10000), ErrorCheck::IndexInUse);
    EXPECT_EQ(dbManager.getFullList().front()->getIndexNr(), 10000);
}

TEST(MainOperations, ShouldNotModifyPeselWhenIsDuplicate)
{
    EXPECT_EQ(dbManager.findStudentAndModifyPeselNr(dbManager.getFullList().front()->getIndexNr(), "49081828686"), ErrorCheck::PeselInUse);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), "49081828686");
}

TEST(MainOperations, ShouldValidatePesel)
{
    dbManager.enablePeselValidation(true);
    EXPECT_TRUE(dbManager.peselValidator("55030101193",Sex::Male));
    EXPECT_FALSE(dbManager.peselValidator("55030101193",Sex::Female));
    EXPECT_FALSE(dbManager.peselValidator("55030101197",Sex::Male));
    EXPECT_TRUE(dbManager.peselValidator("01240239252",Sex::Male));
    EXPECT_FALSE(dbManager.peselValidator("01240239252",Sex::Female));
    EXPECT_TRUE(dbManager.peselValidator("15210214538",Sex::Male));

    
}

//
