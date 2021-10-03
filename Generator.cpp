#include "Generator.hpp"
#include <random>


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
