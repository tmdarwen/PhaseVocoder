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

#include <Application/TransientConfigFile.h>
#include <Utilities/Exception.h>
#include <Utilities/Stringify.h>
#include <yaml-cpp/yaml.h>
#include <cstddef>

TransientConfigFile::TransientConfigFile(const std::string& filename)
{
	ReadTransientConfigFile(filename);
	SortTransientVectors();
}

TransientConfigFile::~TransientConfigFile() { }

void TransientConfigFile::ReadTransientConfigFile(const std::string& filename)
{
	try
	{
		YAML::Node config = YAML::LoadFile(filename);

		for(auto transient : config["transients"])
		{
			transients_.push_back(transient.as<std::size_t>());
		}

		for(auto transient : config["left_channel_transients"])
		{
			leftChannelTransients_.push_back(transient.as<std::size_t>());
		}
		leftChannelTransients_.insert(leftChannelTransients_.begin(), transients_.begin(), transients_.end());

		for(auto transient : config["right_channel_transients"])
		{
			rightChannelTransients_.push_back(transient.as<std::size_t>());
		}
		rightChannelTransients_.insert(rightChannelTransients_.begin(), transients_.begin(), transients_.end());

	}
	catch(std::exception theException)
	{
		auto exceptionWhat{Utilities::CreateString(" ", "Exception trying to open transient configuration file", filename, "Message from yaml-cpp lib:", theException.what())};
		Utilities::ThrowException(exceptionWhat);
	}
}

void TransientConfigFile::SortTransientVectors()
{
	std::sort(transients_.begin(), transients_.end());
	std::sort(leftChannelTransients_.begin(), leftChannelTransients_.end());
	std::sort(rightChannelTransients_.begin(), rightChannelTransients_.end());
}

const std::vector<std::size_t>& TransientConfigFile::GetTransients() const
{
	return transients_;
}

const std::vector<std::size_t>& TransientConfigFile::GetLeftChannelTransients() const
{
	return leftChannelTransients_;
}

const std::vector<std::size_t>& TransientConfigFile::GetRightChannelTransients() const
{
	return rightChannelTransients_;
}