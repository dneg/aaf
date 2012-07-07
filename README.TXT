                        AAF SDK version 1.1.6 Release
                         http://aaf.sourceforge.net/

Getting Started
----------------
(a) Using Microsoft Visual C++ (Win32)
    - Open AAFWinSDK/vs<version>/AAFWinSDK.sln
      (where Visual C++ version can be 9 or 10)

    - Build the "Everything" project
      (Select Debug or Release configuration and Win32 or x86_64 platform.)

    - VS9 client application builds should point their library & include paths to:
        AAFWinSDK/vs9/include/
        AAFWinSDK/vs9/lib/
      and running client applications require AAFCOAPI.dll so PATH must include:
        AAFWinSDK/vs<version>/bin/
      Clean the solution before changing the platfrom between Win32 and x86_64.

    - VS10 client application builds should point their library & include paths to:
        include: AAFWinSDK/vs10/include/
        library: AAFWinSDK/vs10/{Win32,x86_64}/{Debug,Release}/RefImpl
      and running client applications require AAFCOAPI.dll so PATH must include:
        AAFWinSDK/vs10/{Win32,x86_64}/{Debug,Release}/RefImpl

(b) Using GNU make
    - To build the library, tests and all the examples and utilities run
        make everything

    - To run the module tests
        make check

    - For MacOS X, you can optionally build universal binaries using
	    make AAFPLATFORM=UniversalDarwin <target>

    - For MacOS X 10.6 64 bit, you can build 32 or 64 bit binaries using
	    make AAFPLATFORM=i386Darwin <target>
	    or
	    make AAFPLATFORM=x86_64Darwin <target>

	    Note that AAFPLATFORM=i386Darwin is used by default on OSX
	    if no platform is specified on the make command line.
	    
    - To build a release version (instead of the default debug version)
        make AAFTARGET=Release <target>

    - To build a static library and statically linked examples and utilities
        make AAFTARGET=Debug-static <target>

    - To build the SDK to use the libgsf Structured Storage library
        make LIBGSF_PATH=/usr/local <target>
      (the prefix of your libgsf and glib libraries & includes may differ)

    - The "install" target is the minimum needed to build and install the
      library and header files for client development.  Client application
      builds should point their compiler library and include paths to the
	  following (each platform has its own AAF*SDK/ directory):
        AAFi686LinuxSDK/g++/include/              # all builds
        AAFi686LinuxSDK/g++/lib/debug/            # Debug build
        AAFi686LinuxSDK/g++/lib/                  # Release build
      To run client applications, libcom-api.so is required so add the
	  bin directory to your LD_LIBRARY_PATH:
	    AAFi686LinuxSDK/g++/bin/debug/            # Debug build
	    AAFi686LinuxSDK/g++/bin/                  # Release build
      
Examples
---------
To run an example the SDK dynamic library (AAFCOAPI.dll for WIN32,
libcom-api.so for other platforms) must be in your platform's
dynamic library search path.

(a) Under Win32 your PATH must contain AAFWinSDK\bin
    - Create sample AAF files by running ComModAAF.exe
        vs9: vs9/AAFWinSDK/Debug/Test/ComModAAF.exe
        vs10: vs10/AAFWinSDK/{Win32,x86_64}/Debug/Test/ComModAAF.exe

    - Use the InfoDumper executable to inspect an AAF file
        vs9: vs9/AAFWinSDK/Debug/Examples/Com/InfoDumper.exe EssenceAccessCDCI_DV.aaf
        vs10: vs10/AAFWinSDK/{Win32,x86_64}/Debug/Examples/Com/InfoDumper.exe EssenceAccessCDCI_DV.aaf

(b) Using GNU make
    - On a unix platform run "make check" to have sample .aaf files created under
        test/com/ComModTestAAF

    - run InfoDumper to inspect an AAF file
        env LD_LIBRARY_PATH=AAFi686LinuxSDK/g++/bin/debug AAFi686LinuxSDK/g++/bin/debug/InfoDumper test/com/ComModTestAAF/EssenceAccessCDCI_DV.aaf
        (the platform specific paths <AAF*SDK> will vary by platform)

Documentation
--------------------
API documentation and the AAF SDK FAQ are available from the AAF SDK home page
  http://aaf.sourceforge.net/

The API documentation can also be built from the SDK source:
  cd ref-impl/doc/com-api
  make doc
See ref-impl/doc/com-api/README.txt for full details.

Further information
--------------------
If you a have query please use the AAF SDK Forums
  http://sourceforge.net/forum/?group_id=24405
