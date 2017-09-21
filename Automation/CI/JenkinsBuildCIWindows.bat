call "%VisualStudioPath%\vcvarsall.bat" amd64
mkdir PhaseVocoderBuilt
cd PhaseVocoderBuilt
cmake -G "%CMakeGeneratorString%" ../Source
devenv DarwenAudio.sln /Build Debug
devenv DarwenAudio.sln /Build Release
