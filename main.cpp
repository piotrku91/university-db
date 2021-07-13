// university-db - Project for homework in CODERS SCHOOL (https:://coders,school)
/* 
Authors:
Main Code: Piotr K (https://github.com/piotrku91, piotrq.eu)
Function tokenize() from our another project: Piotr R (https://github.com/Rogal255)

*/

#include <iostream>
#include "Menu.hpp"

constexpr auto VERSION = "0.13";

int main()
{
   Db dbManager;
   Menu mainMenu(dbManager,VERSION);
   mainMenu.run();
}