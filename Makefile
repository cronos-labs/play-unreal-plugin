UNAME := $(shell uname)
all: android_key_gen download

android_key_gen:
ifeq ($(UNAME), Darwin)
	./android_key_gen.sh
endif

download:
	cd Plugins/play-unreal-plugin && make
