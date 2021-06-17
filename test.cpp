#include <vector>
#include "gtest/gtest.h"
#include "db.hpp"

db dbManager;

TEST(MainOperations, ShouldAddNewStudent)
{
    EXPECT_EQ(dbManager.getCount(), 0);
    dbManager.addStudent("Roman", "Szpicruta", "Durnia 50", 29481, 90010120190, Sex::Male);
    EXPECT_EQ(dbManager.getCount(), 1);
}

TEST(MainOperations, ShouldAddNewStudentValid)
{
    EXPECT_EQ(dbManager.getFullList()[0]->getFirstname(), "Roman");
    EXPECT_EQ(dbManager.getFullList()[0]->getLastname(), "Szpicruta");
    EXPECT_EQ(dbManager.getFullList()[0]->getAddress(), "Durnia 50");
    EXPECT_EQ(dbManager.getFullList()[0]->getIndexNr(), 29481);
    EXPECT_EQ(dbManager.getFullList()[0]->getPeselNr(), 90010120190);
    EXPECT_EQ(dbManager.getFullList()[0]->getSex(), Sex::Male);
}

TEST(MainOperations, ShouldAddMoreStudents)
{
    EXPECT_EQ(dbManager.getCount(), 1);
    dbManager.addStudent("Anna", "Torbisko", "Flaszki 1", 29222, 89010120190, Sex::Female);
    dbManager.addStudent("Tomek", "Kola", "Janka 2", 29121, 92010120190, Sex::Male);
    dbManager.addStudent("Danka", "Koziol", "Wuja 22", 20128, 42010120190, Sex::Female);
    dbManager.addStudent("Jurek", "Znicz", "Luny 222", 10128, 62010140190, Sex::Male);
    dbManager.addStudent("Zenobiusz", "Gorizek", "Stefana 232", 10127, 92090140190, Sex::Male);
    dbManager.addStudent("Piotr", "Ameba", "Stefana 232", 17127, 72090140190, Sex::Male);
    EXPECT_EQ(dbManager.getCount(), 7);
}

TEST(MainOperations, ShouldANotAddDuplicatesIndex)
{
    EXPECT_EQ(dbManager.getCount(), 7);
    EXPECT_EQ(dbManager.addStudent("Tomek", "Kola", "Janka 2", 29121, 92010120199, Sex::Male), ErrorCheck::IndexInUse);
    EXPECT_EQ(dbManager.getCount(), 7);
}

TEST(MainOperations, ShouldANotAddDuplicatesPesel_Test)
{
    EXPECT_EQ(dbManager.getCount(), 7);
    EXPECT_EQ(dbManager.addStudent("Tomek", "Kola", "Janka 2", 29124, 92010120190, Sex::Male), ErrorCheck::PeselInUse);
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
        EXPECT_EQ(found->get()->getPeselNr(), 62010140190);
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
    auto found = dbManager.findStudentByPesel_Binary(62010140190);
    EXPECT_TRUE(found);

    if (found)
    {
        EXPECT_EQ(found->get()->getFirstname(), "Jurek");
        EXPECT_EQ(found->get()->getLastname(), "Znicz");
        EXPECT_EQ(found->get()->getAddress(), "Luny 222");
        EXPECT_EQ(found->get()->getIndexNr(), 10128);
        EXPECT_EQ(found->get()->getPeselNr(), 62010140190);
        EXPECT_EQ(found->get()->getSex(), Sex::Male);
    }
}

TEST(MainOperations, ShouldNotFindByPesel)
{
    auto found = dbManager.findStudentByPesel_Binary(5454544544);
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
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), 72090140190);
    dbManager.sortByPesel(Order::Desc);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), 92090140190);
    dbManager.sortByPesel(Order::Asc);
    EXPECT_EQ(dbManager.getFullList().front()->getPeselNr(), 42010120190);
}

//
