include $(AAFBASE)/build/pdefs-PPCDarwin.mk

PLATFORM_CFLAGS = -arch i386
LD_DYN_LIB = $(COMPILER) -arch i386 -dynamiclib
