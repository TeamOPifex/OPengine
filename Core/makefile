####################################
## OPengine_Core makefile         ##
## Author: Kirk Roerig            ##
## Date 9-21-2012                 ##
####################################

LIBSRC = Core MathHelpers
LIBOBJ = $(addsuffix .o,$(LIBSRC))
OUT = OPengine_Core

TESTSRC = test
TESTOUT = Test

all:
	make LINUX32

LINUX32:
	rm -rf Target.h
	echo "#ifndef OPIFEX_LINUX32\n#define OPIFEX_LINUX32\n#endif" >> Target.h
	make LINUX
LINUX64:
	rm -rf Target.h
	echo "#define OPIFEX_LINUX64" >> Target.h
	make LINUX
LINUX:
	#build the static lib
	for file in $(LIBSRC); do \
		gcc -std=c99 -c $$file.c -L. -lm -lc -o $$file.o;\
	done
	ar rcs lib$(OUT).a $(LIBOBJ)
test:
	#gcc -std=c99 -static $(TESTSRC).c -L -l$(OUT) -o $(TESTOUT)
	gcc -std=c99 -lm -lc $(TESTSRC).c lib$(OUT).a -o $(TESTOUT) 
clean:
	rm -rf $(LIBOBJ) lib$(OUT).a

