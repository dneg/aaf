
ifndef AAFPLATFORM
    AAFPLATFORM = $(shell $(AAFBASE)/build/aafplatform)
endif


include $(AAFBASE)/build/pdefs-$(AAFPLATFORM).mk

AAFSDK = $(AAFBASE)/AAF$(AAFPLATFORM)SDK/$(COMPILER)


# Put compiler specific flags in CFLAGS
CFLAGS = $(DBG_FLAGS) $(ADD_CFLAGS) $(PLATFORM_CFLAGS) $(BYTE_ORDER)

# U_OPTS should be defined as whether use_unicode or 
# no_unicode in platform-specific .def file
ifeq ( $(U_OPTS), use_unicode )
    CFLAGS += -D_UNICODE=1
endif


CPP_EXTENSION ?= cpp


# Dynamic library search path. Optional.
# It allows the libraries to be found quickly.
# By default it depends on AAFBASE value. But user can set this variable
# to point to the directory where AAF libraries are installed. For example:
#
#	make RPATH=/usr/local/lib/aaf
#
#RPATH ?= $(LIBDIR)  <-- this doesnt work
ifeq ($(origin RPATH), undefined)
    RPATH = $(LIBDIR)
endif


PACKDIR := $(AAFSDK)/$(PACKAGE)
OBJDIR := $(PACKDIR)/obj
LIBDIR := $(AAFSDK)/lib
BINDIR := $(PACKDIR)/bin


HEADERS = $(wildcard, *.{hxx,h})
CXXFILES = $(wildcard *.$(CPP_EXTENSION))
CFILES = $(wildcard *.c)
CXXFILES2 = 


SOURCES = $(CXXFILES) $(CFILES)
CXXOBJS1 = $(SOURCES:%.$(CPP_EXTENSION)=$(OBJDIR)/%$(OBJ))
CXXOBJS = $(CXXOBJS1:%.c=$(OBJDIR)/%$(OBJ))
CXXOBJS2A = $(CXXFILES2:%.$(CPP_EXTENSION)=$(OBJDIR)/%$(OBJ))
CXXOBJS2 = $(CXXOBJS2A:%.c=$(OBJDIR)/%$(OBJ))
#CXXOBJS2 = $(CXXFILES2:%.$(CPP_EXTENSION)=$(OBJDIR)/%$(OBJ))

