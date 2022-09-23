UNAME := $(shell uname)
all: fetch android_key_gen download

android_key_gen:
ifeq ($(UNAME), Darwin)
	./android_key_gen.sh
endif

fetch:
	git submodule update --init --recursive

download:
	cd Plugins/play-unreal-plugin && make
