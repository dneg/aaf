
.PHONY : all
all : ref-impl examples devutils utilities test

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
check : ref-impl
	cd test/com/ComModTestAAF && $(MAKE) check

.PHONY : clean
clean :
	cd ref-impl && $(MAKE) clean
	cd DevUtils && $(MAKE) clean
	cd Utilities && $(MAKE) clean
	cd examples && $(MAKE) clean
	cd test && $(MAKE) clean

