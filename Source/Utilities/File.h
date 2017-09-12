#include <string>
#include <vector>
#include <fstream>

namespace Utilities { namespace File {

// Returns the dirname of the given path and filename
std::string GetDirname(const std::string& filenameWithPath);

// Returns the basename of the given path and filename
std::string GetBasename(const std::string& filenameWithPath);

// Removed the extension from the given filename
void RemoveExtension(std::string& filename);

// A fairly simple class to read data from a file
class FileReader
{
	public:
		FileReader(const std::string filename);
		virtual ~FileReader();

		std::size_t GetFileSize();

		std::vector<char> ReadData(std::size_t position, std::size_t bytes);

	private:
		std::string filename_;
		std::ifstream fileStream_;
		std::size_t fileSize_;

		void InitializeFileReading();
};

// Diffs two files returning true if they match and false otherwise.
bool CheckIfFilesMatch(const std::string& filenameA, const std::string& filenameB);
// The diffInfo string contains information about the differences if they files do not match.
bool CheckIfFilesMatch(const std::string& filenameA, const std::string& filenameB, std::string& diffInfo);

}} // End of namespace
