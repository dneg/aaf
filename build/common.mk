
#----------------------------------------------------------
# Determine the platform we're building on and include
# platfrom specific definitions.
#----------------------------------------------------------
ifndef AAFPLATFORM
    AAFPLATFORM = $(shell $(AAFBASE)/build/aafplatform)
endif

include $(AAFBASE)/build/pdefs-$(AAFPLATFORM).mk


#----------------------------------------------------------
# AAFSDK is the directory where all the binaries will 
# be placed. It's located in AAF toolkit directory and has
# two components: platform name generated above and 
# compiler name defined in the platfrom specific 
# definitions file.
#----------------------------------------------------------
AAFSDK = $(AAFBASE)/AAF$(AAFPLATFORM)SDK/$(COMPILER)


#----------------------------------------------------------
# Define AAFTARGET and related variables.
# There are two supported build types: Debug (default) and 
# Release. To change default setting user can specify 
# AAFTARGET as parameter to make command:
#
#	make AAFTARGET=Release
#
#----------------------------------------------------------
# Define default target build
ifndef AAFTARGET
    AAFTARGET = Debug
endif

# Define target build subdirectory.
ifeq ($(AAFTARGET), Debug)
    AAFTARGETDIR = debug
else
    AAFTARGETDIR = .
endif

# Update DBG_FLAGS depending on build target.
ifeq ($(AAFTARGET), Debug)
    ifneq ($(findstring -D_DEBUG, $(DBG_FLAGS)), -D_DEBUG)
	DBG_FLAGS += -D_DEBUG
    endif
else
ifeq ($(AAFTARGET), Release)
    ifneq ($(findstring -DNDEBUG, $(DBG_FLAGS)), -DNDEBUG)
	DBG_FLAGS = -DNDEBUG
    endif
endif
endif


#----------------------------------------------------------
# Put compiler specific flags in CFLAGS
# CFLAGS components are defined in platform-specific 
# definitions file pdefs-$(AAFPLATFORM).mk
#----------------------------------------------------------
CFLAGS = $(DBG_FLAGS) $(ADD_CFLAGS) $(PLATFORM_CFLAGS) $(BYTE_ORDER)

# U_OPTS should be defined as whether use_unicode or 
# no_unicode in platform-specific .def file
ifeq ($(U_OPTS), use_unicode)
    CFLAGS += -D_UNICODE=1
endif


#----------------------------------------------------------
# Dynamic library search path. Optional.
# It allows the libraries to be found quickly.
# By default it depends on AAFBASE value. But user can 
# set this variable to point to the directory where AAF 
# libraries are installed. For example:
#
#	make RPATH=/usr/local/lib/aaf
#
#----------------------------------------------------------
#RPATH ?= $(LIBDIR)  <-- this doesnt work
ifeq ($(origin RPATH), undefined)
    RPATH = $(LIBDIR)
endif


#----------------------------------------------------------
# Dependencies generation flag.
# If it's not 0 make will generate dependency files.
# By default dependencies generation is turned on. To
# change this behaviour use:
#
#       make GENDEPS=0
#
#----------------------------------------------------------
ifndef GENDEPS
    GENDEPS = 1
endif


PACKDIR := $(AAFSDK)/$(PACKAGE)
OBJDIR := $(PACKDIR)/$(AAFTARGETDIR)
LIBDIR := $(AAFSDK)/lib/$(AAFTARGETDIR)
BINDIR := $(AAFSDK)/bin/$(AAFTARGETDIR)

CPP_EXTENSION ?= cpp

HEADERS = $(wildcard, *.{hxx,h})
CXXFILES = $(wildcard *.$(CPP_EXTENSION))
CFILES = $(wildcard *.c)
CXXFILES2 = 


SOURCES = $(CXXFILES) $(CFILES)
CXXOBJS1 = $(SOURCES:%.$(CPP_EXTENSION)=$(OBJDIR)/%$(OBJ))
CXXOBJS = $(CXXOBJS1:%.c=$(OBJDIR)/%$(OBJ))
CXXOBJS2A = $(CXXFILES2:%.$(CPP_EXTENSION)=$(OBJDIR)/%$(OBJ))
CXXOBJS2 = $(CXXOBJS2A:%.c=$(OBJDIR)/%$(OBJ))

DEPS_TMP = $(SOURCES:%.$(CPP_EXTENSION)=$(OBJDIR)/%.d)
DEPS = $(DEPS_TMP:%.c=$(OBJDIR)/%.d)


