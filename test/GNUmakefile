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
	cd com && $(MAKE) clean
	$(RMDIR) $(OBJDIR)/*

