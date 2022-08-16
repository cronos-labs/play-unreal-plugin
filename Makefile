UNAME := $(shell uname)
PLAYCPPSDK=v0.0.5-alpha
MACSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_Darwin_x86_64.tar.gz
MACCHEKSUMSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/checksums-Darwin_x86_64.txt
WINDOWSSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_Windows_x86_64.zip
WINDOWSCHECKSUMSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/checksums-Windows_x86_64.txt
LINUXSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_libc++_Linux_x86_64.tar.gz
LINUXCHECKSUMSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/checksums-libc++_Linux_x86_64.txt

all: rm prepare download checkhash uncompress

rm:
	rm -rf ./install

prepare:
	mkdir -p ./install/mac
	mkdir -p ./install/windows
	mkdir -p ./install/linux
	mkdir -p ./Source/ThirdParty/cronosplay/lib/Mac
	mkdir -p ./Source/ThirdParty/cronosplay/lib/Win64
	mkdir -p ./Source/ThirdParty/cronosplay/lib/Linux

download:
	cd install/mac && curl -O -L $(MACSRC)
	cd install/windows && curl -O -L $(WINDOWSSRC)
	cd install/linux && curl -O -L $(LINUXSRC)

checkhash:
ifeq ($(UNAME), Darwin)
	cd install/mac && curl -L $(MACCHEKSUMSRC) | shasum -a 256 -c -
	cd install/windows &&  curl -L $(WINDOWSCHECKSUMSRC) | shasum -a 256 -c -
	cd install/linux && curl -L $(LINUXCHECKSUMSRC) | shasum -a 256 -c -
endif

ifeq ($(UNAME), Linux)
	cd install/mac && curl -L $(MACCHEKSUMSRC) | sha256sum -c  -
	cd install/windows &&  curl -L $(WINDOWSCHECKSUMSRC) | sha256sum -c  -
	cd install/linux && curl -L $(LINUXCHECKSUMSRC) | sha256sum -c  -
endif


uncompress:
	cd install/mac && tar xvf play_cpp_sdk_Darwin_x86_64.tar.gz
	cd install/windows && unzip play_cpp_sdk_Windows_x86_64.zip
	cd install/linux && tar xvf play_cpp_sdk_libc++_Linux_x86_64.tar.gz
	cp install/mac/sdk/lib/libplay_cpp_sdk.a ./Source/ThirdParty/cronosplay/lib/Mac
	cp install/windows/sdk/lib/play_cpp_sdk.lib ./Source/ThirdParty/cronosplay/lib/Win64
	cp install/linux/sdk/lib/libplay_cpp_sdk.a ./Source/ThirdParty/cronosplay/lib/Linux

RunUAT:
	/home/ue4/UnrealEngine/Engine/Build/BatchFiles/RunUAT.sh \
	BuildPlugin \
	-Rocket \
	-TargetPlatforms=Linux \
	-Plugin=/home/ue4/play-unreal-plugin/CronosPlayUnreal.uplugin \
	-Package=/home/ue4/play-unreal-plugin/Output
