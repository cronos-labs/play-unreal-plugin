set PLAYCPPSDK=v0.0.3-alpha
set WINDOWSSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/play_cpp_sdk_Windows_x86_64.zip
set WINDOWSHASH="9ad8bedaaa96e8a5898126d40da527a4180b8c7cfe8ab7a2d6d01445ad476efd play_cpp_sdk_Windows_x86_64.zip"

rmdir install\windows\ /s /q
rmdir Source\ThirdParty\cronosplay\lib\Win64\ /s /q

mkdir -p install\windows
mkdir -p Source\ThirdParty\cronosplay\lib\Win64

cd install\windows
powershell -Command "Invoke-WebRequest %WINDOWSSRC% -OutFile play_cpp_sdk_Windows_x86_64.zip"

echo %WINDOWSHASH% | sha256sum -c  -
unzip play_cpp_sdk_Windows_x86_64.zip

Xcopy /E/I sdk\lib ..\..\Source\ThirdParty\cronosplay\lib\Win64

cd ..\..
