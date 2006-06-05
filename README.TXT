                         AAF SDK version 1.1 Release
                         http://aaf.sourceforge.net/

Getting Started
----------------
(a) Using Microsoft Visual C++ (Win32)
    - Open AAFWinSDK/AAFWinSDK.sln

    - Build the "Everything" project
      (a Debug or Release build type can be selected)

    - Client application builds should point their library & include paths to:
        AAFWinSDK/include/
        AAFWinSDK/lib/

(b) Using GNU make
    - To build the library, tests and all the examples and utilities run
        make everything

    - To run the module tests
        make check

    - To build a release version (instead of the default debug version)
        make AAFTARGET=Release <target>

    - To build a static library and statically linked examples and utilities
        make AAFTARGET=Debug-static <target>

    - To build the SDK to use the libgsf Structured Storage library
        make LIBGSF_PATH=/usr/local <target>
      (the prefix of your libgsf and glib libraries & includes may differ)

    - The "install" target is the minimum needed to build and install the
      library and header files for client development.  Client application
      builds should point their library and include paths to the following
      (the AAF*SDK/ directory will vary by platform):
        AAFi686LinuxSDK/g++/include/              # all builds
        AAFi686LinuxSDK/g++/lib/debug/            # Debug libs
        AAFi686LinuxSDK/g++/lib/                  # Release libs
      
Examples
---------
To run an example the SDK dynamic library (AAFCOAPI.dll for WIN32,
libcom-api.so for other platforms) must be in your platform's
dynamic library search path.

(a) Under Win32 your PATH must contain AAFWinSDK\bin
    - Create sample AAF files by running ComModAAF.exe
        AAFWinSDK/Debug/Test/ComModAAF.exe

    - Use the InfoDumper executable to inspect an AAF file
        AAFWinSDK/Debug/Examples/Com/InfoDumper.exe EssenceAccessCDCI_DV.aaf

(b) Using GNU make
    - run "make check" to have sample .aaf files created under
        test/com/ComModTestAAF

    - run InfoDumper to inspect an AAF file
        env LD_LIBRARY_PATH=AAFi686LinuxSDK/g++/bin/debug AAFi686LinuxSDK/g++/bin/debug/InfoDumper test/com/ComModTestAAF/EssenceAccessCDCI_DV.aaf
        (the platform specific paths <AAF*SDK> will vary by platform)

Further information
--------------------
API documentation and the AAF SDK FAQ are available from the AAF SDK home page
  http://aaf.sourceforge.net/

If you a have query please use the AAF SDK Forums
  http://sourceforge.net/forum/?group_id=24405