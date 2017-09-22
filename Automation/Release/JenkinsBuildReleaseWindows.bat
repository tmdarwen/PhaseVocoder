ECHO Building PhaseVocoder version number %Version% 
ECHO Build number is %BUILD_NUMBER%

call "%VisualStudioPath%\vcvarsall.bat" amd64

REM Switch to the tag
git checkout -b PhaseVocoderTag v%Version%

REM Run CMake
mkdir Build
cd Build
cmake -D VERSION_NUMBER=%Version% -D BUILD_NUMBER=%BUILD_NUMBER% -G "%CMakeGeneratorString%" ..\Source
IF %ERRORLEVEL% NEQ 0 (
    ECHO CMake failed
    EXIT 1
)

REM Build only PhaseVocoder
cd PhaseVocoder
devenv PhaseVocoder.vcxproj /Build Release
IF %ERRORLEVEL% NEQ 0 (
    ECHO The build failed    
    EXIT 1
)

REM Zip the executable
echo "7z" a -tzip PhaseVocoder.zip "%WORKSPACE%\Build\Application\Release\PhaseVocoder.exe" 
"7z" a -tzip PhaseVocoder.zip "%WORKSPACE%\Build\Application\Release\PhaseVocoder.exe" 

IF %ERRORLEVEL% NEQ 0 (
    ECHO Zipping PhaseVocoder.exe failed
    EXIT 1
)
