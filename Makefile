UNAME := $(shell uname)
PWD = $(shell pwd)

# Set the play cpp sdk version
PLAYCPPSDK=v0.0.13-alpha
# Set NDK versions (to see what NDK_VERSION is available, please check play-cpp-sdk release page)
NDK_VERSION=21.4.7075529
# Set names of play cpp sdk library files
MAC_FILE=play_cpp_sdk_Darwin_x86_64.tar.gz
WINDOWS_FILE=play_cpp_sdk_Windows_x86_64.zip
LINUX_FILE=play_cpp_sdk_libc++_Linux_x86_64.tar.gz
ARM64_V8A_FILE=play_cpp_sdk_aarch64-linux-android-$(NDK_VERSION).tar.gz
ARMEABI_V7A_FILE=play_cpp_sdk_armv7-linux-androideabi-$(NDK_VERSION).tar.gz
X86_64_FILE=play_cpp_sdk_x86_64-linux-android-$(NDK_VERSION).tar.gz
IOS_FILE=play_cpp_sdk_aarch64-apple-ios.tar.gz
# Set names of checksum files
MAC_CHECKSUM_FILE=checksums-Darwin_x86_64.txt
WINDOWS_CHECKSUM_FILE=checksums-Windows_x86_64.txt
LINUX_CHECKSUM_FILE=checksums-libc++_Linux_x86_64.txt
ANDROID_ARM64_V8A_CHECKSUM_FILE=checksums-aarch64-linux-android-$(NDK_VERSION).txt
ANDROID_ARMEABI_V7A_CHECKSUM_FILE=checksums-armv7-linux-androideabi-$(NDK_VERSION).txt
ANDROID_X86_64_CHECKSUM_FILE=checksums-x86_64-linux-android-$(NDK_VERSION).txt
IOS_CHECKSUM_FILE=checksums-aarch64-apple-ios.txt

MAC_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(MAC_FILE)
MAC_CHEKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(MAC_CHECKSUM_FILE)
WINDOWS_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(WINDOWS_FILE)
WINDOWS_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(WINDOWS_CHECKSUM_FILE)
LINUX_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(LINUX_FILE)
LINUX_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(LINUX_CHECKSUM_FILE)
ANDROID_ARM64_V8A_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(ARM64_V8A_FILE)
ANDROID_ARM64_V8A_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(ANDROID_ARM64_V8A_CHECKSUM_FILE)
ANDROID_ARMEABI_V7A_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(ARMEABI_V7A_FILE)
ANDROID_ARMEABI_V7A_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(ANDROID_ARMEABI_V7A_CHECKSUM_FILE)
ANDROID_X86_64_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(X86_64_FILE)
ANDROID_X86_64_CHECKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(ANDROID_X86_64_CHECKSUM_FILE)
IOS_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(IOS_FILE)
IOS_CHEKSUM_SRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/$(IOS_CHECKSUM_FILE)


all: rm download checkhash uncompress

rm:
	rm -rf ./install

prepare:
	mkdir -p ./install/mac
	mkdir -p ./install/windows
	mkdir -p ./install/linux
	mkdir -p ./install/android/arm64-v8a
	mkdir -p ./install/android/armeabi-v7a
	mkdir -p ./install/android/x86_64
	mkdir -p ./install/ios
	mkdir -p ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Mac
	mkdir -p ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Win64
	mkdir -p ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Linux
	mkdir -p ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Android/arm64-v8a
	mkdir -p ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Android/armeabi-v7a
	mkdir -p ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Android/x86_64
	mkdir -p ./Source/ThirdParty/PlayCppSdkLibrary/Lib/iOS/arm64

download: prepare
	cd install/mac && curl -s -O -L $(MAC_SRC)
	cd install/windows && curl -s -O -L $(WINDOWS_SRC)
	cd install/linux && curl -s -O -L $(LINUX_SRC)
	cd install/android/arm64-v8a && curl -s -O -L $(ANDROID_ARM64_V8A_SRC)
	cd install/android/armeabi-v7a && curl -s -O -L $(ANDROID_ARMEABI_V7A_SRC)
	cd install/android/x86_64 && curl -s -O -L $(ANDROID_X86_64_SRC)
	cd install/ios && curl -s -O -L $(IOS_SRC)

checkhash:
ifeq ($(UNAME), Darwin)
	cd install/mac && curl -s -L $(MAC_CHEKSUM_SRC) | shasum -a 256 -c -
	cd install/windows &&  curl -s -L $(WINDOWS_CHECKSUM_SRC) | shasum -a 256 -c -
	cd install/linux && curl -s -L $(LINUX_CHECKSUM_SRC) | shasum -a 256 -c -
	cd install/android/arm64-v8a && curl -s -L $(ANDROID_ARM64_V8A_CHECKSUM_SRC) | shasum -a 256 -c -
	cd install/android/armeabi-v7a && curl -s -L $(ANDROID_ARMEABI_V7A_CHECKSUM_SRC) | shasum -a 256 -c -
	cd install/android/x86_64 && curl -s -L $(ANDROID_X86_64_CHECKSUM_SRC) | shasum -a 256 -c -
	cd install/ios && curl -s -L $(IOS_CHEKSUM_SRC) | shasum -a 256 -c -
else
	cd install/mac && curl -s -L $(MAC_CHEKSUM_SRC) | sha256sum -c  -
	cd install/windows &&  curl -s -L $(WINDOWS_CHECKSUM_SRC) | sha256sum -c  -
	cd install/linux && curl -s -L $(LINUX_CHECKSUM_SRC) | sha256sum -c  -
	cd install/android/arm64-v8a && curl -s -L $(ANDROID_ARM64_V8A_CHECKSUM_SRC) | sha256sum -c  -
	cd install/android/armeabi-v7a && curl -s -L $(ANDROID_ARMEABI_V7A_CHECKSUM_SRC) | sha256sum -c  -
	cd install/android/x86_64 && curl -s -L $(ANDROID_X86_64_CHECKSUM_SRC) | sha256sum -c  -
	cd install/ios && curl -s -L $(IOS_CHEKSUM_SRC) | sha256sum -c  -
endif


uncompress: prepare
	tar xvf install/mac/$(MAC_FILE) -C ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Mac --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	unzip -j install/windows/$(WINDOWS_FILE) sdk/lib/play_cpp_sdk.lib -d ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Win64
	tar xvf install/linux/$(LINUX_FILE) -C ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Linux --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	tar xvf install/android/arm64-v8a/$(ARM64_V8A_FILE) -C ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Android/arm64-v8a --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	tar xvf install/android/armeabi-v7a/$(ARMEABI_V7A_FILE) -C ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Android/armeabi-v7a --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	tar xvf install/android/x86_64/$(X86_64_FILE) -C ./Source/ThirdParty/PlayCppSdkLibrary/Lib/Android/x86_64 --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	tar xvf install/ios/$(IOS_FILE) -C ./Source/ThirdParty/PlayCppSdkLibrary/Lib/iOS/arm64 --strip-components=2 sdk/lib/libplay_cpp_sdk.a


# FIXME Build Android/IOS with RunUAT.sh
RunUAT:
ifeq ($(UNAME), Darwin)
	/Users/Shared/Epic\ Games/UE_4.27/Engine/Build/BatchFiles/RunUAT.sh \
	BuildPlugin \
	-Rocket \
	-TargetPlatforms=Mac \
	-Plugin=$(PWD)/CronosPlayUnreal.uplugin \
	-Package=$(PWD)/Output
else ifeq ($(UNAME), Linux)
	/home/ue4/UnrealEngine/Engine/Build/BatchFiles/RunUAT.sh \
	BuildPlugin \
	-Rocket \
	-TargetPlatforms=Linux \
	-Plugin=$(PWD)/CronosPlayUnreal.uplugin \
	-Package=$(PWD)/Output
endif
