REM Set the play cpp sdk version
set PLAYCPPSDK=v0.0.27-alpha

REM Set the play-cpp-sdk cache path
set PLAYCPPSDK_CACHE_DIR=C:\play-cpp-sdk\%PLAYCPPSDK%

REM Set the play-cpp-sdk target path
set PLAYCPPSDK_TARGET_DIR=Source\ThirdParty\PlayCppSdkLibrary\Lib

REM Set NDK versions (to see what NDK_VERSION is available, please check play-cpp-sdk release page)
set UE4_27_NDK_VERSION=23.0.7599858
set UE5_0_NDK_VERSION=23.0.7599858
set UE5_1_NDK_VERSION=25.1.8937393

set TARGET=%1
if "%TARGET%"=="4.27" (
  set NDK_VERSION=%UE4_27_NDK_VERSION%
) else if "%TARGET%"=="5.0" (
  set NDK_VERSION=%UE5_0_NDK_VERSION%
) else if "%TARGET%"=="5.1" (
  set NDK_VERSION=%UE5_1_NDK_VERSION%
) else if "%TARGET%"=="" (
  set NDK_VERSION=%UE5_1_NDK_VERSION%
) else (
  echo "UE %TARGET% is not supported."
)

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
set IOS_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/%PLAYCPPSDK%/%IOS_CHECKSUM_FILE%

REM rmdir %PLAYCPPSDK_CACHE_DIR%\ /s /q
setlocal EnableDelayedExpansion

set CACHE_OS_LIST=Mac Win64 Linux Android\arm64-v8a Android\armeabi-v7a Android\x86_64 iOS\arm64
for %%i in (%CACHE_OS_LIST%) do (
    if "%%i"=="Mac" (
        set CACHE_FILE=%MAC_FILE%
        set CACHE_SRC=%MAC_SRC%
    ) else if "%%i"=="Win64" (
        set CACHE_FILE=%WINDOWS_FILE%
        set CACHE_SRC=%WINDOWS_SRC%
    ) else if "%%i"=="Linux" (
        set CACHE_FILE=%Linux_FILE%
        set CACHE_SRC=%Linux_SRC%
    ) else if "%%i"=="Android\arm64-v8a" (
        set CACHE_FILE=%ARM64_V8A_FILE%
        set CACHE_SRC=%ANDROID_ARM64_V8A_SRC%
    ) else if "%%i"=="Android\armeabi-v7a" (
        set CACHE_FILE=%ARMEABI_V7A_FILE%
        set CACHE_SRC=%ANDROID_ARMEABI_V7A_SRC%
    ) else if "%%i"=="Android\x86_64" (
        set CACHE_FILE=%X86_64_FILE%
        set CACHE_SRC=%ANDROID_X86_64_SRC%
    ) else if "%%i"=="iOS\arm64" (
        set CACHE_FILE=%IOS_FILE%
        set CACHE_SRC=%IOS_SRC%
    )

    set CACHE_DIR=%PLAYCPPSDK_CACHE_DIR%\%%i\!CACHE_FILE!
    if exist !CACHE_DIR! (
        echo "%CACHE_DIR% exists"
    ) else (
        mkdir %PLAYCPPSDK_CACHE_DIR%\%%i
        powershell -Command "Invoke-WebRequest !CACHE_SRC! -OutFile !CACHE_DIR!"
    )
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
