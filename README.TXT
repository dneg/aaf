                         AAF SDK version 1.1 Release
                         http://aaf.sourceforge.net/

Getting Started
----------------
(a) Using Microsoft Visual C++ (Win32)
    - Open AAFWinSDK/AAFWinSDK.sln

    - Build the "Everything" project
      (a Debug or Release build type can be selected)

(b) Using GNU make
    - To build the library, tests and all the examples and utilities run
        make everything

    - To run the module tests
        make check

    - To build a release version (instead of the default debug version)
        make AAFTARGET=Release

    - To build a static library and statically linked examples and utilities
        make AAFTARGET=Debug-static

    - To build the SDK to use the libgsf Structured Storage library
        make LIBGSF_PATH=/usr/local
      (the prefix of your libgsf and glib libraries & includes may differ)

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
        (the platform specific paths may differ on your platform)

Further information
----=---------------
API documentation and the AAF SDK FAQ are available from the AAF SDK home page
  http://aaf.sourceforge.net/

If you a have query please use the AAF SDK Forums
  http://sourceforge.net/forum/?group_id=24405
