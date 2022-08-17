set PLAYCPPSDK=v0.0.6-alpha
set WINDOWSSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/play_cpp_sdk_Windows_x86_64.zip
set WINDOWSHASH="56797bc1f67ea4e00a0cffcc8fe048b58703fd913d44ec81a68402f238e363f8 play_cpp_sdk_Windows_x86_64.zip"

rmdir install\windows\ /s /q
rmdir Source\ThirdParty\PlayCppSdkLibrary\Lib\Win64\ /s /q

mkdir -p install\windows
mkdir -p Source\ThirdParty\PlayCppSdkLibrary\Lib\Win64

cd install\windows
powershell -Command "Invoke-WebRequest %WINDOWSSRC% -OutFile play_cpp_sdk_Windows_x86_64.zip"

echo %WINDOWSHASH% | sha256sum -c  -
unzip play_cpp_sdk_Windows_x86_64.zip

Xcopy /E/I sdk\lib ..\..\Source\ThirdParty\cronosplay\lib\Win64

cd ..\..
