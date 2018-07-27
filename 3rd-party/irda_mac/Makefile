LIB_DEPS=irlap.c irlap1.c irlap2.c irlmp.c irlmp1.c irlmp2.c irttp1.c irttp2.c irttp2_accept.c moschip.c


all: lib

lib:
	gcc -dynamiclib $(LIB_DEPS) -arch i386 -arch x86_64 -framework IOKit -framework CoreFoundation -o libirda.dylib
clean:
	rm -fr *.dylib *.o
