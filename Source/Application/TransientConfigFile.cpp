#include <Application/TransientConfigFile.h>
#include <Utilities/Exception.h>
#include <Utilities/Stringify.h>
#include <yaml-cpp/yaml.h>
#include <cstddef>

TransientConfigFile::TransientConfigFile(const std::string& filename)
{
	try
	{
		YAML::Node config = YAML::LoadFile(filename);

		for(auto transient : config["transients"])
		{
			transients_.push_back(transient.as<std::size_t>());
		}
	}
	catch(std::exception theException)
	{
		auto exceptionWhat{Utilities::CreateString(" ", "Exception trying to open transient configuration file", filename, "Message from yaml-cpp lib:", theException.what())};
		Utilities::ThrowException(exceptionWhat);
	}
}

TransientConfigFile::~TransientConfigFile() { }

const std::vector<std::size_t>& TransientConfigFile::GetTransients() const
{
	return transients_;
}