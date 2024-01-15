CC:=clang
CFLAGS:=

OFILES:=
CFILES:=

build: build/obj build-src

	cd build/obj && ar rcs libjson.a *.o && mv libjson.a ../

build-src: src/
	cd src && make
	cd src && cp *.h ../build/include 
	cd lib/DynamicString && make && cp dynamic_string.o ../../build/obj

setup:
	mkdir build
	mkdir build/obj
	mkdir build/include
	mkdir lib
	cd lib && mkdir DynVec && cd DynVec && wget https://gist.githubusercontent.com/JohannesThoren/5135ea3a6491945ffdecbb511f121360/raw/7dd07a1ed304fb8ef9f34d48fb0862713a82fd62/vector.h
	cd lib && git clone https://github.com/JohannesThoren/DynamicString.git
	cd lib/DynamicString && make && mv dynamic_string.o ../../build/obj
	cd build && mkdir lib && mkdir lib/DynamicString
	cp lib/DynamicString/dynamic_string.h build/lib/DynamicString/

#variables for setup

SRC_MAKE="CC:=clang\nCFLAGS:=\n*.o: *.c\n\t$(CC) -c *.c\n\tmv *.o ../build/obj"
