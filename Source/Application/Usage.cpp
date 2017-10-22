/*
 * PhaseVocoder
 *
 * Copyright (c) 2017 - Terence M. Darwen - tmdarwen.com
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

#include <iostream>
#include <Application/Usage.h>

#ifndef VERSION_NUMBER
#define VERSION_NUMBER Non-Production Build
#endif

#ifndef BUILD_NUMBER
#define BUILD_NUMBER Non-Production Build
#endif

#define MACRO_TO_STRING_INDIRECT(s) #s
#define MACRO_TO_STRING(s) MACRO_TO_STRING_INDIRECT(s)

void DisplayHeaderInfo()
{
	std::cout << "PhaseVocoder" << std::endl;
	std::cout << "Version: " << MACRO_TO_STRING(VERSION_NUMBER) << "  Build: " << MACRO_TO_STRING(BUILD_NUMBER) << std::endl;
	std::cout << "Built: " << __DATE__ << " " __TIME__ << std::endl;	
}

void DisplaySimpleUsage()
{
	std::cout << "Command Line Options:" << std::endl;
	std::cout << "   --help            (-h): This info" << std::endl;
	std::cout << "   --longhelp        (-l): More informative help info" << std::endl;
	std::cout << "   --version         (-v): Show version info" << std::endl;
	std::cout << "   --input           (-i): The input wave file you want to process" << std::endl;
	std::cout << "   --output          (-o): The resulting output wave filename" << std::endl;
	std::cout << "   --stretch         (-s): The stretch/compress ratio" << std::endl;
	std::cout << "   --pitch           (-p): Pitch adjustment in semitones" << std::endl;
	std::cout << "   --resample        (-r): Set the sample rate of the output" << std::endl;
	std::cout << "   --peakvalleyratio (-a): Specific transient valley-to-peak ratio" << std::endl;
	std::cout << "   --transientconfig (-c): Use a config file for input parameters" << std::endl;
	std::cout << "   --showtransients  (-t): Display transient sample positions" << std::endl;
	std::cout << "Usage Example: -i inputfile.wav -o outputfile.wav --stretch 1.25" << std::endl;
}

void DisplayLimitations()
{
	std::cout << "Limitations: Supports mono 16 bit uncompressed wave files only." << std::endl;
}

void DisplayCopyrightInfo()
{
	std::cout << "Copyright - Terence Darwen - Some rights reserverd" << std::endl;
	std::cout << "http://www.tmdarwen.com" << std::endl;
}

void DisplayDescription()
{
	std::cout << "Description:                                                                     " << std::endl;
	std::cout << "    The PhaseVocoder allows for high quality stretching and compressing of audio " << std::endl; 
	std::cout << "    with respect to time.  The ability to pitch shift and resample audio, as well" << std::endl;
	std::cout << "    as detect audio transients is also included.  Example usage listed below. " << std::endl;
}

void DisplayExamples()
{
	std::cout << "Stretch Example:" << std::endl;
	std::cout << "    Increase the length of the input by a factor of two:" << std::endl;
	std::cout << "    -i in.wav -o out.wav -s 2.0" << std::endl;
	std::cout << std::endl;
	std::cout << "Compress Example:" << std::endl;
	std::cout << "    Reduce the length of the input by twenty percent:" << std::endl;
	std::cout << "    -input in.wav -output out.wav -stretch 0.8" << std::endl;
	std::cout << std::endl;
	std::cout << "Pitch Shift Example - Raise the pitch:" << std::endl;
	std::cout << "    Raise the pitch of the audio by 2 semitones:" << std::endl;
	std::cout << "    -i in.wav -o out.wav -s -p 2.0" << std::endl;
	std::cout << std::endl;
	std::cout << "Pitch Shift Example - Drop the pitch" << std::endl;
	std::cout << "    Drop the pitch of the audio by 3.1 semitones:" << std::endl;
	std::cout << "    -i in.wav -o out.wav -s -p -3.1" << std::endl;
	std::cout << std::endl;
	std::cout << "Resample Example" << std::endl;
	std::cout << "    Change the sample rate to 88,200 Hz:" << std::endl;
	std::cout << "    -i in.wav -o out.wav -s -r 88200" << std::endl;
	std::cout << std::endl;
	std::cout << "Displaying Transient Positions:" << std::endl;
	std::cout << "    Stretch in.wav by twenty-five percent and also display the sample positions " << std::endl;
	std::cout << "    of detected transients:" << std::endl;
	std::cout << "    -input in.wav -output out.wav -stretch 1.25 -showtransients" << std::endl;
	std::cout << std::endl;
	std::cout << "Stretching Using a Transient Config File:" << std::endl;
	std::cout << "    Stretch in.wav by ten percent using a config file of specific transient " << std::endl;
	std::cout << "    positions:" << std::endl;
	std::cout << "    -input in.wav -output out.wav -stretch 1.10 -transientconfig transients.cfg" << std::endl;
}

void DisplayTransientConfigExample()
{
	std::cout << "Transient Config File Example:" << std::endl;
	std::cout << "    The transient config file is simply a YAML file consisting of a list of " << std::endl;
	std::cout << "    increasing integers which are the sample positions of the transients in the" << std::endl;
	std::cout << "    input wave file.  The following example shows transients occurring at four " << std::endl;
	std::cout << "    different sample positions (100, 14700, 35329, 51922):" << std::endl;
	std::cout << "    transients : [100, 14700, 35329, 51922]" << std::endl;
}

void DisplayValleyToPeakRatioInfo()
{
	std::cout << "Valley-to-Peak Ratio:" << std::endl;
	std::cout << "    The valley-to-peak ratio is the amount of growth in audio amplitude        " << std::endl;
	std::cout << "    occurring during a transient event.  The ratio value is the required       " << std::endl;
	std::cout << "    minimum ratio for a transient to be treated as a transient event during    " << std::endl;
    std::cout << "    processing.  The default ratio is 1.5.  A lower value will have lower      " << std::endl;
    std::cout << "    requirements for a transient to be treated as a transient event during     " << std::endl;
	std::cout << "    processing.  A higher value will have stricter requirements." << std::endl;
}

void DisplayShortHelp()
{
	DisplayHeaderInfo();
	DisplaySimpleUsage();
	DisplayLimitations();
	DisplayCopyrightInfo();
}

void DisplayLongHelp()
{
	DisplayHeaderInfo();
	std::cout << std::endl;

	DisplayDescription();
	std::cout << std::endl;

	DisplaySimpleUsage();
	std::cout << std::endl;

	DisplayLimitations();
	std::cout << std::endl;

	DisplayExamples();
	std::cout << std::endl;

	DisplayTransientConfigExample();
	std::cout << std::endl;

	DisplayValleyToPeakRatioInfo();
	std::cout << std::endl;

	DisplayCopyrightInfo();
}

void DisplayVersion()
{
	DisplayHeaderInfo();
	DisplayCopyrightInfo();
}
