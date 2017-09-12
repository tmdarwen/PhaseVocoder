#include <string>
#include <vector>

class TransientConfigFile
{
	public:
		TransientConfigFile(const std::string& filename);
		virtual ~TransientConfigFile();

		const std::vector<std::size_t>& GetTransients() const;

	private:
		std::vector<std::size_t> transients_;
};