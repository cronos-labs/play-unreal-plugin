# for mac
# fix dylib install name
install_name_tool -id @rpath/libplay_cpp_sdk.dylib ./Plugins/CronosPlayUnreal/Source/CronosPlayUnreal/Private/cronosplay/lib/Mac/libplay_cpp_sdk.dylib 
# check
otool -L ./Plugins/CronosPlayUnreal/Source/CronosPlayUnreal/Private/cronosplay/lib/Mac/libplay_cpp_sdk.dylib 
# copy
cp ./Plugins/CronosPlayUnreal/Source/CronosPlayUnreal/Private/cronosplay/lib/Mac/libplay_cpp_sdk.dylib  ./Binaries/Mac

# for windows
cp ./Plugins/CronosPlayUnreal/Source/CronosPlayUnreal/Private/cronosplay/lib/Win64/play_cpp_sdk.dll  ./Binaries/Win64


