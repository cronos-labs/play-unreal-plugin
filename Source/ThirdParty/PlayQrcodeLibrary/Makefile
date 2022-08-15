all:
	arch -x86_64 gcc -c Source/qrcodegen.cpp -I ./Include -std=c++11
	arch -x86_64 ar rcs ./Lib/Mac/libqrcodegen.a qrcodegen.o

linux:
	clang -c Source/qrcodegen.cpp -I ./Include -std=c++11
	ar rcs ./Lib/Linux/libqrcodegen.a qrcodegen.o

