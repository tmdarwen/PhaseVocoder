#include <Utilities/Stringify.h>
#include <sstream>
#include <iostream>
#include <string>
#include <algorithm>

std::vector<std::string> Utilities::DelimitedStringToVectorOfStrings(const std::string &delimitedString, char delimiter)
{
    std::vector<std::string> elements;

    std::stringstream ss;
    ss.str(delimitedString);
    std::string currentElement;
    while(std::getline(ss, currentElement, delimiter))
	{
        elements.push_back(currentElement);
    }

    return elements;
}

std::string Utilities::ConvertStringToUppercase(const std::string &inputString)
{
	std::string uc = inputString;
	std::transform(uc.begin(), uc.end(), uc.begin(), ::toupper);
	return uc;
}
