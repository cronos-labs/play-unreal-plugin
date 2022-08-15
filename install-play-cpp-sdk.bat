set PLAYCPPSDK=v0.0.5-alpha
set WINDOWSSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/play_cpp_sdk_Windows_x86_64.zip
set WINDOWSHASH="b92730b41015ede99d704916ae1a784a8bcab0ffcfc485857b0198d0bc1fbf4d play_cpp_sdk_Windows_x86_64.zip"

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
