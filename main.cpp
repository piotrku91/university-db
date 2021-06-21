// university-db - Project for homework in CODERS SCHOOL (https:://coders,school)
// Authors - Piotr K (https://github.com/piotrku91, piotrq.eu)


#include <iostream>
#include "menu.hpp"

int main()
{
   db dbManager;
   menu mainMenu(dbManager);
   mainMenu.run();
}