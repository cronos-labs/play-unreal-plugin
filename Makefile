PLAYCPPSDK=v0.0.2-alpha
MACSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_Darwin_x86_64.tar.gz
MACHASH="5b5167a2a375b0a443c8156088456ff159410bc37a6d8e0f876041f867621ca3  play_cpp_sdk_Darwin_x86_64.tar.gz"
WINDOWSSRC=https://github.com/cronos-labs/play-cpp-sdk/releases/download/$(PLAYCPPSDK)/play_cpp_sdk_Windows_x86_64.zip
WINDOWSHASH="43624bcb713b901986ce0d526f7f718bfff4a27c29d413130cf4a4469d0cac68 play_cpp_sdk_Windows_x86_64.zip"

all: prepare download checkhash uncompress

prepare:
	mkdir -p ./install/mac
	mkdir -p ./install/windows
	mkdir -p ./ThirdParty/cronosplay/lib/Mac
	mkdir -p ./ThirdParty/cronosplay/lib/Win64

download:
	cd install/mac && wget $(MACSRC)
	cd install/windows && wget $(WINDOWSSRC)

checkhash:
	cd install/mac && echo $(MACHASH) | sha256sum -c  -
	cd install/windows &&  echo $(WINDOWSHASH) | sha256sum -c  -

uncompress:
	cd install/mac && tar xvf play_cpp_sdk_Darwin_x86_64.tar.gz
	cd install/windows && unzip play_cpp_sdk_Windows_x86_64.zip
	cp install/mac/sdk/lib/* ./ThirdParty/cronosplay/lib/Mac
	cp install/windows/sdk/lib/* ./ThirdParty/cronosplay/lib/Win64



