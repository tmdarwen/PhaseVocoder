PhaseVocoder
============

A command line utility implementing a [phase vocoder](https://en.wikipedia.org/wiki/Phase_vocoder).  The phase vocoder allows for time stretching/compressing, pitch shifting and resampling of audio with quite a good degree of quality.  For audio samples and additional information please see [this page](http://www.tmdarwen.com/projects/phase-vocoder).  I've successfully built this project on Windows 10, OS X (El Capitan) and Linux (Lubuntu).  For building on your platform please see the steps below.

 

**Steps for Building**

1.   Clone this repo.

1.   Create a new directory at the parallel level as the cloned repo.  This directory will hold the project files CMake creates.

1.   cd into this new directory.

1.   From the command line, run _cmake -G <GeneratorType> ../PhaseVocoder/Source_

1.   Build the project

 

**Usage**

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

Unit test coverage is extensive.  You'll notice every component within the source directory has a UT directory which contains unit tests.  These of course automatically build and run as part of the application build process.

 

**Miscellaneous Notes Concerning the Project**

-   Currently only support mono 16 bit wave files as input.

-   External dependencies are [GoogleTest](https://github.com/google/googletest) and [yaml-cpp](https://github.com/jbeder/yaml-cpp).  These GitHub repos will be cloned automatically when CMake runs.

 

**To Do**

-   Allow for stereo wave files (currently only supports mono audio)

-   Add multithreading to speed up processing

