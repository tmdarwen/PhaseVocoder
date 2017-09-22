# Do a debug build
mkdir DebugBuild
cd DebugBuild
/usr/local/bin/cmake -G "Xcode" -DCMAKE_BUILD_TYPE=Debug ../
xcodebuild -project PhaseVocoder.xcodeproj -configuration Debug

cd ..

# Do a releaase build
mkdir ReleaseBuild
cd ReleaseBuild
/usr/local/bin/cmake -G "Xcode" -DCMAKE_BUILD_TYPE=Release ../
xcodebuild -project PhaseVocoder.xcodeproj -configuration Release
