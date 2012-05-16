include $(AAFBASE)/build/pdefs-PPCDarwin.mk

PLATFORM_CFLAGS = -arch x86_64
PLATFORMLIBS = -arch x86_64
LD_DYN_LIB = $(COMPILER) -arch x86_64 -dynamiclib
