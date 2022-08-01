set PLAYCPPSDK=v0.0.2-alpha
set WINDOWSSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/play_cpp_sdk_Windows_x86_64.zip
set WINDOWSHASH="43624bcb713b901986ce0d526f7f718bfff4a27c29d413130cf4a4469d0cac68 play_cpp_sdk_Windows_x86_64.zip"

rmdir install\windows\ /s /q
rmdir ThirdParty\cronosplay\lib\Win64\ /s /q

mkdir -p install\windows
mkdir -p ThirdParty\cronosplay\lib\Win64

cd install\windows
powershell -Command "Invoke-WebRequest %WINDOWSSRC% -OutFile play_cpp_sdk_Windows_x86_64.zip"

echo %WINDOWSHASH% | sha256sum -c  -
unzip play_cpp_sdk_Windows_x86_64.zip

Xcopy /E/I sdk\lib ..\..\ThirdParty\cronosplay\lib\Win64

cd ..\..
