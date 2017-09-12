#pragma once

#include <exception>
#include <string>
#include <Utilities/Stringify.h>

namespace Utilities {

class Exception : public std::exception
{
	public:
		Exception();
		Exception(const std::string& what, const std::string& file="", std::size_t lineNumber=0);
		const char* what() const noexcept override;
	
	private:
	    std::string what_;
	    std::string file_;
	    std::size_t lineNumber_;
		mutable std::string extendedWhat_;
};

template<typename... Params>
void ThrowException(Params... params)
{
	std::string message = Utilities::CreateString("|", params...);
	throw Utilities::Exception(message);
}

} // End of namespace
