UNAME := $(shell uname)
PWD = $(shell pwd)

# Set the play cpp sdk version
PLAYCPPSDK=v0.0.14-alpha
# Set the play-cpp-sdk cache path
PLAYCPPSDK_CACHE_DIR=./install/$(PLAYCPPSDK)
# Set the play-cpp-sdk target path
PLAYCPPSDK_TARGET_DIR=./Source/ThirdParty/PlayCppSdkLibrary/Lib
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


all: download checkhash uncompress

download:
ifneq (,$(wildcard $(PLAYCPPSDK_CACHE_DIR)))
	echo "$(PLAYCPPSDK_CACHE_DIR) exists"
else
	mkdir -p $(PLAYCPPSDK_CACHE_DIR)/Mac
	mkdir -p $(PLAYCPPSDK_CACHE_DIR)/Win64
	mkdir -p $(PLAYCPPSDK_CACHE_DIR)/Linux
	mkdir -p $(PLAYCPPSDK_CACHE_DIR)/Android/arm64-v8a
	mkdir -p $(PLAYCPPSDK_CACHE_DIR)/Android/armeabi-v7a
	mkdir -p $(PLAYCPPSDK_CACHE_DIR)/Android/x86_64
	mkdir -p $(PLAYCPPSDK_CACHE_DIR)/iOS/arm64

	cd $(PLAYCPPSDK_CACHE_DIR)/Mac && curl -s -O -L $(MAC_SRC)
	cd $(PLAYCPPSDK_CACHE_DIR)/Win64 && curl -s -O -L $(WINDOWS_SRC)
	cd $(PLAYCPPSDK_CACHE_DIR)/Linux && curl -s -O -L $(LINUX_SRC)
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/arm64-v8a && curl -s -O -L $(ANDROID_ARM64_V8A_SRC)
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/armeabi-v7a && curl -s -O -L $(ANDROID_ARMEABI_V7A_SRC)
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/x86_64 && curl -s -O -L $(ANDROID_X86_64_SRC)
	cd $(PLAYCPPSDK_CACHE_DIR)/iOS/arm64 && curl -s -O -L $(IOS_SRC)
endif

checkhash:
ifeq ($(UNAME), Darwin)
	cd $(PLAYCPPSDK_CACHE_DIR)/Mac && curl -s -L $(MAC_CHEKSUM_SRC) | shasum -a 256 -c -
	cd $(PLAYCPPSDK_CACHE_DIR)/Win64 &&  curl -s -L $(WINDOWS_CHECKSUM_SRC) | shasum -a 256 -c -
	cd $(PLAYCPPSDK_CACHE_DIR)/Linux && curl -s -L $(LINUX_CHECKSUM_SRC) | shasum -a 256 -c -
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/arm64-v8a && curl -s -L $(ANDROID_ARM64_V8A_CHECKSUM_SRC) | shasum -a 256 -c -
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/armeabi-v7a && curl -s -L $(ANDROID_ARMEABI_V7A_CHECKSUM_SRC) | shasum -a 256 -c -
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/x86_64 && curl -s -L $(ANDROID_X86_64_CHECKSUM_SRC) | shasum -a 256 -c -
	cd $(PLAYCPPSDK_CACHE_DIR)/iOS/arm64 && curl -s -L $(IOS_CHEKSUM_SRC) | shasum -a 256 -c -
else
	cd $(PLAYCPPSDK_CACHE_DIR)/Mac && curl -s -L $(MAC_CHEKSUM_SRC) | sha256sum -c  -
	cd $(PLAYCPPSDK_CACHE_DIR)/Win64 &&  curl -s -L $(WINDOWS_CHECKSUM_SRC) | sha256sum -c  -
	cd $(PLAYCPPSDK_CACHE_DIR)/Linux && curl -s -L $(LINUX_CHECKSUM_SRC) | sha256sum -c  -
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/arm64-v8a && curl -s -L $(ANDROID_ARM64_V8A_CHECKSUM_SRC) | sha256sum -c  -
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/armeabi-v7a && curl -s -L $(ANDROID_ARMEABI_V7A_CHECKSUM_SRC) | sha256sum -c  -
	cd $(PLAYCPPSDK_CACHE_DIR)/Android/x86_64 && curl -s -L $(ANDROID_X86_64_CHECKSUM_SRC) | sha256sum -c  -
	cd $(PLAYCPPSDK_CACHE_DIR)/iOS/arm64 && curl -s -L $(IOS_CHEKSUM_SRC) | sha256sum -c  -
endif

prepare:
	mkdir -p $(PLAYCPPSDK_TARGET_DIR)/Mac
	mkdir -p $(PLAYCPPSDK_TARGET_DIR)/Win64
	mkdir -p $(PLAYCPPSDK_TARGET_DIR)/Linux
	mkdir -p $(PLAYCPPSDK_TARGET_DIR)/Android/arm64-v8a
	mkdir -p $(PLAYCPPSDK_TARGET_DIR)/Android/armeabi-v7a
	mkdir -p $(PLAYCPPSDK_TARGET_DIR)/Android/x86_64
	mkdir -p $(PLAYCPPSDK_TARGET_DIR)/iOS/arm64

uncompress: prepare
	tar xvf $(PLAYCPPSDK_CACHE_DIR)/Mac/$(MAC_FILE) -C $(PLAYCPPSDK_TARGET_DIR)/Mac --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	unzip -j $(PLAYCPPSDK_CACHE_DIR)/Win64/$(WINDOWS_FILE) sdk/lib/play_cpp_sdk.lib -d $(PLAYCPPSDK_TARGET_DIR)/Win64
	tar xvf $(PLAYCPPSDK_CACHE_DIR)/Linux/$(LINUX_FILE) -C $(PLAYCPPSDK_TARGET_DIR)/Linux --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	tar xvf $(PLAYCPPSDK_CACHE_DIR)/Android/arm64-v8a/$(ARM64_V8A_FILE) -C $(PLAYCPPSDK_TARGET_DIR)/Android/arm64-v8a --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	tar xvf $(PLAYCPPSDK_CACHE_DIR)/Android/armeabi-v7a/$(ARMEABI_V7A_FILE) -C $(PLAYCPPSDK_TARGET_DIR)/Android/armeabi-v7a --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	tar xvf $(PLAYCPPSDK_CACHE_DIR)/Android/x86_64/$(X86_64_FILE) -C $(PLAYCPPSDK_TARGET_DIR)/Android/x86_64 --strip-components=2 sdk/lib/libplay_cpp_sdk.a
	tar xvf $(PLAYCPPSDK_CACHE_DIR)/iOS/arm64/$(IOS_FILE) -C $(PLAYCPPSDK_TARGET_DIR)/iOS/arm64 --strip-components=2 sdk/lib/libplay_cpp_sdk.a


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

cmake:
ifeq ($(UNAME), Darwin)
	/Users/Shared/Epic\ Games/UE_5.1/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh \
    -project=$(PWD)/CronosPlayUnreal.uplugin -game -CMakefile
else ifeq ($(UNAME), Linux)
	/home/ue4/UnrealEngine/Engine/Build/BatchFiles/Linux/GenerateProjectFiles.sh \
    -project=$(PWD)/CronosPlayUnreal.uplugin -game -CMakefile
endif

compile_commands: cmake
	mkdir -p build
	cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

clean:
	rm CMakeLists.txt
	rm -rf build
