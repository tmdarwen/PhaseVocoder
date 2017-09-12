#pragma once

#include <string>
#include <vector>

namespace Utilities {

template<class T>
typename std::enable_if<std::is_fundamental<T>::value, std::string>::type Stringify(const T& t)
{
	return std::to_string(t);
}

template<class T>
typename std::enable_if<!std::is_fundamental<T>::value, std::string>::type Stringify(const T& t)
{
	return std::string(t);
}

template<typename T>
std::string CreateString(const std::string& delimiter, T param)
{
	return Utilities::Stringify(param);
}

template<typename T, typename... Tail>
std::string CreateString(const std::string& delimiter, T head, Tail... tail)
{
	return Utilities::Stringify(head) + Utilities::Stringify(delimiter) + Utilities::CreateString(delimiter, tail...);	
}

std::vector<std::string> DelimitedStringToVectorOfStrings(const std::string &delimitedString, char delimiter);

std::string ConvertStringToUppercase(const std::string &inputString);

}  // End of namespace
