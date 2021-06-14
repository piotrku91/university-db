 #include "db.hpp"

 bool db::addStudent(const std::string &firstName, const std::string &lastName, const std::string &address, const int indexNr, const long int peselNr, const Sex sexType)
 {

Students.push_back(std::make_unique<Student>(Student{firstName,lastName,address,indexNr,peselNr,sexType}));

return true;
 };