PACKAGE = Test
export PACKAGE

AAFBASE ?= ..


# Common definitions
include $(AAFBASE)/build/common.mk



.PHONY : all
all : com


.PHONY : com
com :
	cd com && $(MAKE)


.PHONY : clean
clean :
	cd com && $(MAKE) GENDEPS=0 $@


.PHONY : realclean
realclean :
	cd com && $(MAKE) GENDEPS=0 $@


