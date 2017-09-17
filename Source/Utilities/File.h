/*
 * PhaseVocoder
 *
 * Copyright (c) Terence M. Darwen - tmdarwen.com
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
