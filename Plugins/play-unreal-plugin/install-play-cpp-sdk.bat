REM Set the play cpp sdk version
set PLAYCPPSDK=v0.0.8-alpha

REM Set NDK versions (to see what NDK_VERSION is available, please check play-cpp-sdk release page)
set NDK_VERSION=21.4.7075529

REM Set names of play cpp sdk library files
set MAC_FILE=play_cpp_sdk_Darwin_x86_64.tar.gz
set WINDOWS_FILE=play_cpp_sdk_Windows_x86_64.zip
set LINUX_FILE=play_cpp_sdk_libc++_Linux_x86_64.tar.gz
set ARM64_V8A_FILE=play_cpp_sdk_aarch64-linux-android-%NDK_VERSION%.tar.gz
set ARMEABI_V7A_FILE=play_cpp_sdk_armv7-linux-androideabi-%NDK_VERSION%.tar.gz
set X86_64_FILE=play_cpp_sdk_x86_64-linux-android-%NDK_VERSION%.tar.gz

REM Set names of checksum files
set MAC_CHECKSUM_FILE=checksums-Darwin_x86_64.txt
set WINDOWS_CHECKSUM_FILE=checksums-Windows_x86_64.txt
set LINUX_CHECKSUM_FILE=checksums-libc++_Linux_x86_64.txt
set ANDROID_ARM64_V8A_CHECKSUM_FILE=checksums-aarch64-linux-android-%NDK_VERSION%.txt
set ANDROID_ARMEABI_V7A_CHECKSUM_FILE=checksums-armv7-linux-androideabi-%NDK_VERSION%.txt
set ANDROID_X86_64_CHECKSUM_FILE=checksums-x86_64-linux-android-%NDK_VERSION%.txt

set MAC_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%MAC_FILE%
set MAC_CHEKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%MAC_CHECKSUM_FILE%
set WINDOWS_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%WINDOWS_FILE%
set WINDOWS_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%WINDOWS_CHECKSUM_FILE%
set LINUX_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%LINUX_FILE%
set LINUX_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%LINUX_CHECKSUM_FILE%
set ANDROID_ARM64_V8A_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%ARM64_V8A_FILE%
set ANDROID_ARM64_V8A_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%ANDROID_ARM64_V8A_CHECKSUM_FILE%
set ANDROID_ARMEABI_V7A_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%ARMEABI_V7A_FILE%
set ANDROID_ARMEABI_V7A_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%ANDROID_ARMEABI_V7A_CHECKSUM_FILE%
set ANDROID_X86_64_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%X86_64_FILE%
set ANDROID_X86_64_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%ANDROID_X86_64_CHECKSUM_FILE%

rmdir install\ /s /q

mkdir install\mac
mkdir install\windows
mkdir install\linux
mkdir install\android\arm64-v8a
mkdir install\android\armeabi-v7a
mkdir install\android\x86_64
mkdir Source\ThirdParty\PlayCppSdkLibrary\Lib\Mac
mkdir Source\ThirdParty\PlayCppSdkLibrary\Lib\Win64
mkdir Source\ThirdParty\PlayCppSdkLibrary\Lib\Linux
mkdir Source\ThirdParty\PlayCppSdkLibrary\Lib\Android\arm64-v8a
mkdir Source\ThirdParty\PlayCppSdkLibrary\Lib\Android\armeabi-v7a
mkdir Source\ThirdParty\PlayCppSdkLibrary\Lib\Android\x86_64

powershell -Command "Invoke-WebRequest %MAC_SRC% -OutFile install\mac\%MAC_FILE%"
powershell -Command "Invoke-WebRequest %WINDOWS_SRC% -OutFile install\windows\%WINDOWS_FILE%"
powershell -Command "Invoke-WebRequest %LINUX_SRC% -OutFile install\linux\%LINUX_FILE%"
powershell -Command "Invoke-WebRequest %ANDROID_ARM64_V8A_SRC% -OutFile install\android\arm64-v8a\%ARM64_V8A_FILE%"
powershell -Command "Invoke-WebRequest %ANDROID_ARMEABI_V7A_SRC% -OutFile install\android\armeabi-v7a\%ARMEABI_V7A_FILE%"
powershell -Command "Invoke-WebRequest %ANDROID_X86_64_SRC% -OutFile install\android\x86_64\%X86_64_FILE%"

REM Check checksum
REM echo %WINDOWSHASH% | sha256sum -c  -

powershell -Command "Expand-Archive -LiteralPath install\windows\%WINDOWS_FILE% -DestinationPath install\windows"
copy install\windows\sdk\lib\play_cpp_sdk.lib Source\ThirdParty\PlayCppSdkLibrary\Lib\Win64
tar xvf install\mac\%MAC_FILE% -C Source\ThirdParty\PlayCppSdkLibrary\Lib\Mac --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf install\linux\%LINUX_FILE% -C Source\ThirdParty\PlayCppSdkLibrary\Lib\Linux --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf install\android\arm64-v8a\%ARM64_V8A_FILE% -C Source\ThirdParty\PlayCppSdkLibrary\Lib\Android\arm64-v8a --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf install\android\armeabi-v7a\%ARMEABI_V7A_FILE% -C Source\ThirdParty\PlayCppSdkLibrary\Lib\Android\armeabi-v7a --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf install\android\x86_64\%X86_64_FILE% -C Source\ThirdParty\PlayCppSdkLibrary\Lib\Android\x86_64 --strip-components=2 sdk/lib/libplay_cpp_sdk.a
