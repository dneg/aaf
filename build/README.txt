AAF SDK build system
====================
The AAF SDK build system relies on GNU make with all make directives stored
in either GNUmakefile files or *.mk files.

The sequence of make 'includes' is:

1 GNUmakefile                    contains all top-level targets
2  build/common.mk               get platform type
3   build/aafplatform.sh         shell script to probe for platform name
4   build/pdefs-<platform>.mk    platform specific library names and paths
5    build/pdefs-Unix.mk         set Unix command name variables eg RMDIR
6    build/cdefs-<compiler>.mk   compiler debug, opt and linker flags


The default target is 'ref-impl' which builds:
  aaflib   - Main library which client code must link to.  It exports
             AAFLoadLibrary AAFFileOpenNewModifyEx AAFFileOpenExistingRead etc

  aafiid   - Interface ID library which client code must also link to.
             Contain no functions, just read only data (class IDs named IID_*)

  com-api  - Core SDK dynamic library.  This is always loaded at runtime,
             usually via the file open APIs e.g. AAFFileOpenExistingRead

  plugins  - Optional plugin dynamic libraries, optionally loaded at runtime

    aafpgapi - implements plugin codecs (WAVE, AIFC, CDCI, PCM, JPEG)
    aafintp  - handles interpolating between two IAAFControlPoints
               in an IAAFVaryingValue

Variables
---------
  AAFBASE     - toplevel directory (1st variable set in each GNUmakefile)
                e.g. /tmp/AAF

  AAFTARGET   - "Debug", "Release" or "Debug-static". Default is "Debug" or
                override using make command-line e.g. make AAFTARGET=Release

  AAFBUILDDIR - arch-specific binary directory
                e.g. AAFi686LinuxSDK/g++


Dependency generation
---------------------
The -M flag of g++ and MIPSpro is used to generate a dependency list which
is stored in a .d file corresponding to the .cpp's filename.


Example structure of compiled output
------------------------------------
  AAFi686LinuxSDK/g++/aaflib/debug:
    libaaflib.a  aaflib.o aaflibsgi.o aaflibwin32.o

  AAFi686LinuxSDK/g++/aafiid/debug:
    libaafiid.a  aafiid.o

  AAFi686LinuxSDK/g++/OM/debug:
    libom.a  OMDataVector.o OMMappedFileRawStorage.o OMPropertyTable.o ...

  AAFi686LinuxSDK/g++/ref-impl/debug:
    libimpl.a  AAFInterpCOM.o ImplAAFAIFCDescriptor.o ImplAAFFile.o ...
