call "%VisualStudioPath%\vcvarsall.bat" amd64
cd %WORKSPACE%
mkdir PhaseVocoderBuilt
cd PhaseVocoderBuilt
cmake -G "%CMakeGeneratorString%" ../Source
devenv PhaseVocoder.sln /Build Debug
devenv PhaseVocoder.sln /Build Release
