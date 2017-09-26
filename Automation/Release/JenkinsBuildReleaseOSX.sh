echo "Version number is ${Version}"
echo "Build number is ${BUILD_NUMBER}"

git checkout tags/v${Version}
if [ $? -ne 0 ]
then
	echo "Failed to checkout tag"
	exit 1
fi

mkdir ReleaseBuild
if [ $? -ne 0 ]
then
	echo "Failed to make ReleaseBuild directory"
	exit 1
fi

cd ReleaseBuild
if [ $? -ne 0 ]
then
	echo "Failed to change dir into ReleaseBuild"
	exit 1
fi

/usr/local/bin/cmake -D VERSION_NUMBER=${Version} -D BUILD_NUMBER=${BUILD_NUMBER} -D CMAKE_BUILD_TYPE=Release -G "Xcode" ../Source
if [ $? -ne 0 ]
then
	echo "Executing cmake failed"
	exit 1
fi

xcodebuild -project PhaseVocoder.xcodeproj -configuration Release
if [ $? -ne 0 ]
then
	echo "xcodebuild failed"
	exit 1
fi

zip -j PhaseVocoder.zip "${WORKSPACE}/ReleaseBuild/Application/Release/PhaseVocoder"
if [ $? -ne 0 ]
then
	echo "Failed to zip PhaseVocoder"
	exit 1
fi

cp PhaseVocoder.zip "${ReleaseDestination}/PhaseVocoder-OSXIntel64-${Version}-${BUILD_NUMBER}.zip"
if [ $? -ne 0 ]
then
	echo "Failed to copy PhaseVocoder.zip to release directory"
	exit 1
fi

exit 0
