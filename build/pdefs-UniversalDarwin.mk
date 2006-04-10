include $(AAFBASE)/build/pdefs-PPCDarwin.mk

PLATFORM_CFLAGS = -arch ppc -arch i386
LD_DYN_LIB = $(COMPILER) -arch ppc -arch i386 -dynamiclib
