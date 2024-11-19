#include "Auxiliary.h"
/*
This is a 'static' method that receives a string(line) and returns a vector of the string's arguments.

For example:
parseArguments("settlement KfarSPL 0") will return vector with ["settlement", "KfarSPL", "0"]

To execute this method, use Auxiliary::parseArguments(line)
*/
std::vector<std::string> Auxiliary::parseArguments(const std::string& line) {
    std::vector<std::string> arguments;
    std::istringstream stream(line);
    std::string argument;

    while (stream >> argument) {
        arguments.push_back(argument);
    }
<<<<<<< HEAD
    std::cout<<"Arguments: ";
    return arguments;
}
=======
    std::cout << "Arguments: ";
    std::cout << "Arguments2: ";
    return arguments;
}
>>>>>>> 1736a1ce9992d477409965dd33bbf5dcf87a9dc4
