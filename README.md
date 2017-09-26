PhaseVocoder
============

A command line utility implementing a [phase vocoder](https://en.wikipedia.org/wiki/Phase_vocoder).  The phase vocoder allows for time compression/expansion, pitch shifting and resampling of audio with quite a good degree of quality.  For audio samples and additional information please see [this page](http://www.tmdarwen.com/projects/phase-vocoder).  I've successfully built the PhaseVocoder on Windows 10, OS X (El Capitan) and Linux (Lubuntu).  For building on your platform, please see the steps below.

 

**How the PhaseVocoder Works**

For information on how the PhaseVocoder works, please see [this PDF document](Documentation/HowThePhaseVocoderWorks.pdf) I created.

 

**Build Dependencies**

-   Building this project requires [CMake](https://cmake.org) version 3.0 or later.

-   Building this project requires a C++14 compliant compiler.  This project is routinely built in my continous integration system which uses MS Visual Studio 2017, GCC 5.3.1 and Apple LLVM version 7.3.0 (clang 703.0.31).

-   External dependencies are [GoogleTest](https://github.com/google/googletest) and [yaml-cpp](https://github.com/jbeder/yaml-cpp).  You do *not* need to clone or install these dependencies manually. The GitHub repos will be cloned automatically when CMake runs.

 

**Steps for Building**

1.   Clone this repo.

1.   Create a new directory at the parallel level as the cloned repo.  This directory will hold the project files CMake creates.

1.   cd into this new directory.

1.   From the command line, run _cmake -G YourDesiredGeneratorType ../PhaseVocoder/Source_

1.   Build the project

 

**Usage Examples**

Running the PhaseVocoder application from the command line with no arguments will show all possible usage.  The following examples show some of the more common usages.

Time Stretching Example - Increase the length of the input by a factor of two:<br>
```PhaseVocoder -i in.wav -o out.wav -s 2.0```

Time Compression Example - Reduce the length of the input by twenty percent:<br>
```PhaseVocoder -input in.wav -output out.wav -stretch 0.8```

Pitch Shift Example - Raise the pitch of the audio by 2 semitones:<br>
```PhaseVocoder -i in.wav -o out.wav -s -p 2.0```

Pitch Shift Example - Drop the pitch of the audio by 3.1 semitones:<br>
```PhaseVocoder -i in.wav -o out.wav -s -p -3.1```

Resample Example - Change the sample rate to 88,200 Hz:<br>
```PhaseVocoder -i in.wav -o out.wav -s -r 88200```

 

**Tests**

Unit test coverage is extensive.  You'll notice every component within the source directory has a UT directory which contains unit tests.  These of course automatically build and run as part of the build process.

 

**Continuous Integration and Automated Release**

The Automation directory contains scripts that can be used with [Jenkins](https://jenkins.io/) to setup contininuous integration and automated release building of the PhaseVocoder project.  These scripts work on Windows, OS X and Linux.  For more information on how to setup Jenkins using these scripts for continuous integration and automated release building please see [this document](Documentation/JenkinsSetup.md).

 

**Miscellaneous Notes Concerning the Project**

-   Currently supports mono 16 bit wave files as the only form of input.


 

**To Do**

-   Support for stereo wave files (currently only supports mono audio) of any bit resolution (currently only supports 16 bit).

-   Add multithreading to speed up processing.

 

**Licensing**

The MIT License applies to this software and its supporting documentation:

*Copyright (c) Terence M. Darwen - tmdarwen.com*

*Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:*

*The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.*

*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*
