// university-db - Project for homework in CODERS SCHOOL (https:://coders,school)
// Authors - Piotr K (https://github.com/piotrku91, piotrq.eu)

/* 
NOTES:
peselValidator function is default disabled. To turn on from the code use function enableValidation(true); . In runtime type validation true as command. In tests validation is check as well.
*/

#include <iostream>
#include "menu.hpp"

int main()
{
   db dbManager;
   menu mainMenu(dbManager);
   mainMenu.run();
}