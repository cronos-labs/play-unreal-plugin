UNAME := $(shell uname)
PLAYCPPSDK=v0.0.3-alpha
MACSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_Darwin_x86_64.tar.gz
MACHASH="95eb546124063e223927bfdb7be658e171f8eb0dd2c0ef75c4b95454d0c49b6e  play_cpp_sdk_Darwin_x86_64.tar.gz"
WINDOWSSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_Windows_x86_64.zip
WINDOWSHASH="9ad8bedaaa96e8a5898126d40da527a4180b8c7cfe8ab7a2d6d01445ad476efd *play_cpp_sdk_Windows_x86_64.zip"

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
