REM Set the play cpp sdk version
set PLAYCPPSDK=v0.0.16-alpha

REM Set the play-cpp-sdk cache path
set PLAYCPPSDK_CACHE_DIR=C:\play-cpp-sdk\%PLAYCPPSDK%

REM Set the play-cpp-sdk target path
set PLAYCPPSDK_TARGET_DIR=Source\ThirdParty\PlayCppSdkLibrary\Lib

REM Set NDK versions (to see what NDK_VERSION is available, please check play-cpp-sdk release page)
set NDK_VERSION=21.4.7075529

REM Set names of play cpp sdk library files
set MAC_FILE=play_cpp_sdk_Darwin_x86_64.tar.gz
set WINDOWS_FILE=play_cpp_sdk_Windows_x86_64.zip
set LINUX_FILE=play_cpp_sdk_libc++_Linux_x86_64.tar.gz
set ARM64_V8A_FILE=play_cpp_sdk_aarch64-linux-android-%NDK_VERSION%.tar.gz
set ARMEABI_V7A_FILE=play_cpp_sdk_armv7-linux-androideabi-%NDK_VERSION%.tar.gz
set X86_64_FILE=play_cpp_sdk_x86_64-linux-android-%NDK_VERSION%.tar.gz
set IOS_FILE=play_cpp_sdk_aarch64-apple-ios.tar.gz

REM Set names of checksum files
set MAC_CHECKSUM_FILE=checksums-Darwin_x86_64.txt
set WINDOWS_CHECKSUM_FILE=checksums-Windows_x86_64.txt
set LINUX_CHECKSUM_FILE=checksums-libc++_Linux_x86_64.txt
set ANDROID_ARM64_V8A_CHECKSUM_FILE=checksums-aarch64-linux-android-%NDK_VERSION%.txt
set ANDROID_ARMEABI_V7A_CHECKSUM_FILE=checksums-armv7-linux-androideabi-%NDK_VERSION%.txt
set ANDROID_X86_64_CHECKSUM_FILE=checksums-x86_64-linux-android-%NDK_VERSION%.txt
set IOS_CHECKSUM_FILE=checksums-aarch64-apple-ios.txt

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
set IOS_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%IOS_FILE%
set IOS_CHEKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%IOS_CHECKSUM_FILE%

REM rmdir %PLAYCPPSDK_CACHE_DIR%\ /s /q

if exist %PLAYCPPSDK_CACHE_DIR% (
   echo "%PLAYCPPSDK_CACHE_DIR% exists"
) else (
    mkdir %PLAYCPPSDK_CACHE_DIR%\Mac
    mkdir %PLAYCPPSDK_CACHE_DIR%\Win64
    mkdir %PLAYCPPSDK_CACHE_DIR%\Linux
    mkdir %PLAYCPPSDK_CACHE_DIR%\Android\arm64-v8a
    mkdir %PLAYCPPSDK_CACHE_DIR%\Android\armeabi-v7a
    mkdir %PLAYCPPSDK_CACHE_DIR%\Android\x86_64
    mkdir %PLAYCPPSDK_CACHE_DIR%\iOS\arm64

    powershell -Command "Invoke-WebRequest %MAC_SRC% -OutFile %PLAYCPPSDK_CACHE_DIR%\Mac\%MAC_FILE%"
    powershell -Command "Invoke-WebRequest %WINDOWS_SRC% -OutFile %PLAYCPPSDK_CACHE_DIR%\Win64\%WINDOWS_FILE%"
    powershell -Command "Invoke-WebRequest %LINUX_SRC% -OutFile %PLAYCPPSDK_CACHE_DIR%\Linux\%LINUX_FILE%"
    powershell -Command "Invoke-WebRequest %ANDROID_ARM64_V8A_SRC% -OutFile %PLAYCPPSDK_CACHE_DIR%\Android\arm64-v8a\%ARM64_V8A_FILE%"
    powershell -Command "Invoke-WebRequest %ANDROID_ARMEABI_V7A_SRC% -OutFile %PLAYCPPSDK_CACHE_DIR%\Android\armeabi-v7a\%ARMEABI_V7A_FILE%"
    powershell -Command "Invoke-WebRequest %ANDROID_X86_64_SRC% -OutFile %PLAYCPPSDK_CACHE_DIR%\Android\x86_64\%X86_64_FILE%"
    powershell -Command "Invoke-WebRequest %IOS_SRC% -OutFile %PLAYCPPSDK_CACHE_DIR%\iOS\arm64\%IOS_FILE%"
)

if not exist %PLAYCPPSDK_TARGET_DIR%\Mac mkdir %PLAYCPPSDK_TARGET_DIR%\Mac
if not exist %PLAYCPPSDK_TARGET_DIR%\Win64 mkdir %PLAYCPPSDK_TARGET_DIR%\Win64
if not exist %PLAYCPPSDK_TARGET_DIR%\Linux mkdir %PLAYCPPSDK_TARGET_DIR%\Linux
if not exist %PLAYCPPSDK_TARGET_DIR%\Android\arm64-v8a mkdir %PLAYCPPSDK_TARGET_DIR%\Android\arm64-v8a
if not exist %PLAYCPPSDK_TARGET_DIR%\Android\armeabi-v7a mkdir %PLAYCPPSDK_TARGET_DIR%\Android\armeabi-v7a
if not exist %PLAYCPPSDK_TARGET_DIR%\Android\x86_64 mkdir %PLAYCPPSDK_TARGET_DIR%\Android\x86_64
if not exist %PLAYCPPSDK_TARGET_DIR%\iOS\arm64 mkdir %PLAYCPPSDK_TARGET_DIR%\iOS\arm64

REM Check checksum
REM echo %WINDOWSHASH% | sha256sum -c  -

powershell -Command "Expand-Archive -LiteralPath %PLAYCPPSDK_CACHE_DIR%\Win64\%WINDOWS_FILE% -DestinationPath %PLAYCPPSDK_CACHE_DIR%\Win64"
copy %PLAYCPPSDK_CACHE_DIR%\Win64\sdk\lib\play_cpp_sdk.lib %PLAYCPPSDK_TARGET_DIR%\Win64
if exist %PLAYCPPSDK_CACHE_DIR%\Win64\sdk (
   rmdir %PLAYCPPSDK_CACHE_DIR%\Win64\sdk /s /q
)
tar xvf %PLAYCPPSDK_CACHE_DIR%\Mac\%MAC_FILE% -C %PLAYCPPSDK_TARGET_DIR%\Mac --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf %PLAYCPPSDK_CACHE_DIR%\Linux\%LINUX_FILE% -C %PLAYCPPSDK_TARGET_DIR%\Linux --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf %PLAYCPPSDK_CACHE_DIR%\Android\arm64-v8a\%ARM64_V8A_FILE% -C %PLAYCPPSDK_TARGET_DIR%\Android\arm64-v8a --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf %PLAYCPPSDK_CACHE_DIR%\Android\armeabi-v7a\%ARMEABI_V7A_FILE% -C %PLAYCPPSDK_TARGET_DIR%\Android\armeabi-v7a --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf %PLAYCPPSDK_CACHE_DIR%\Android\x86_64\%X86_64_FILE% -C %PLAYCPPSDK_TARGET_DIR%\Android\x86_64 --strip-components=2 sdk/lib/libplay_cpp_sdk.a
tar xvf %PLAYCPPSDK_CACHE_DIR%\iOS\arm64\%IOS_FILE% -C %PLAYCPPSDK_TARGET_DIR%\iOS\arm64 --strip-components=2 sdk/lib/libplay_cpp_sdk.a
