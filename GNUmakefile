
.PHONY : all
all : ss-impl ref-impl examples devutils utilities test

.PHONY : ss-impl
ss-impl :
	cd ss-impl/ref && $(MAKE)

.PHONY : ref-impl
ref-impl :
	cd ref-impl && $(MAKE)

.PHONY : examples
examples :
	cd examples && $(MAKE) 

.PHONY : devutils
devutils :
	cd DevUtils && $(MAKE)

.PHONY : utilities
utilities :
	cd Utilities && $(MAKE)

.PHONY : test
test :
	cd test && $(MAKE)

.PHONY : check
check : ss-impl ref-impl
	cd ss-impl/ref && $(MAKE) check
	cd test/com/ComModTestAAF && $(MAKE) check

.PHONY : clean
clean :
	cd ss-impl/ref && $(MAKE) clean
	cd ref-impl && $(MAKE) clean
