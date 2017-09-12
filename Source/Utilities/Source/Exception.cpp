#include <Utilities/Exception.h>
#include <Utilities/Stringify.h>

Utilities::Exception::Exception()
{

}

Utilities::Exception::Exception(const std::string& what, const std::string& file, std::size_t lineNumber) :
	what_(what),
	file_(file),
	lineNumber_(lineNumber)
{

}

const char* Utilities::Exception::what() const noexcept
{
	if(file_.size())
	{
		// The extendedWhat_ member is necessary so that when we return the char* the memory exists
		// outside the scope of this if and function as opposed to just calling a c_str() on a
		// Utilities::Stringify object that will be destroyed once we leave the if.
		extendedWhat_ = Utilities::Stringify(what_.c_str()) +
								  Utilities::Stringify(" ") +
								  file_ + Utilities::Stringify(":") +
								  Utilities::Stringify(lineNumber_);
		return extendedWhat_.c_str();
	}

	return what_.c_str();
}
