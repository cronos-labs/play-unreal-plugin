UNAME := $(shell uname)
PLAYCPPSDK=v0.0.4-alpha
MACSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_Darwin_x86_64.tar.gz
MACHASH="61e3eb7aa34a860a2822852fdcd3b9fb1d7809f741fd32c15328c1230fe142e6  play_cpp_sdk_Darwin_x86_64.tar.gz"
WINDOWSSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_Windows_x86_64.zip
WINDOWSHASH="56797bc1f67ea4e00a0cffcc8fe048b58703fd913d44ec81a68402f238e363f8 *play_cpp_sdk_Windows_x86_64.zip"

all: rm prepare download checkhash uncompress

rm:
	rm -rf ./install/

prepare:
	mkdir -p ./install/mac
	mkdir -p ./install/windows
	mkdir -p ./Source/ThirdParty/cronosplay/lib/Mac
	mkdir -p ./Source/ThirdParty/cronosplay/lib/Win64

download:
	cd install/mac && curl -O -L $(MACSRC)
	cd install/windows && curl -O -L $(WINDOWSSRC)

checkhash:
ifeq ($(UNAME), Darwin)
	cd install/mac && echo $(MACHASH) | shasum -a 256 -c -
	cd install/windows &&  echo $(WINDOWSHASH) | shasum -a 256 -c -
endif

ifeq ($(UNAME), Linux)
	cd install/mac && echo $(MACHASH) | sha256sum -c  -
	cd install/windows &&  echo $(WINDOWSHASH) | sha256sum -c  -
endif


uncompress:
	cd install/mac && tar xvf play_cpp_sdk_Darwin_x86_64.tar.gz
	cd install/windows && unzip play_cpp_sdk_Windows_x86_64.zip
	cp install/mac/sdk/lib/* ./Source/ThirdParty/cronosplay/lib/Mac
	cp install/windows/sdk/lib/* ./Source/ThirdParty/cronosplay/lib/Win64
