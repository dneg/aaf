###############################################################################
#
# README for Scalability testscript usage
#
##############################################################################

NOTE: Requires a Bourne shell to run the scripts
      E.g. Bash under Linux or MSYS under MS Windows

	  For MS Windows testers MSYS is recommended
	      http://www.mingw.org/msys.shtml

1. To Create the test data directory containing ELI files and raw DV files

   Modify the variable BASEDIR in the script to the directory where you want
   the test data to be created (default is $HOME).

   This script depends upon a raw DV file named 1frame.dv being present.

     sh ./createtest.sh <max number of DV sources in file>

   E.g. 

     sh ./createtest.sh 4000
   
   This script will create the directory $BASEDIR/aaftestdata and populate it
   with a number of .eli files, plus a dv/ subdirectory containing the raw DV
   files referenced by the .eli files.


2. To run the tests

   Ensure that PATH and LD_LIBRARY_PATH variables are set correctly on U*nix
   so that the executables InfoDumper, eli2aaf, and libcom-api.so are
   found correctly.  For MS Windows, PATH must be set correctly for the
   executables and AAFCOAPI.dll.

   sh <datadir>/runaaftest.sh [options]

     where <datadir> is the directory in which the test data was created by
     createtest.sh
	 
	 [options] can be one or more of
	   read      - read tests
	   write     - writing tests
	   4k        - run all tests with 4kB sectors (default is 512 byte)
	   bigfile   - run a bigfile test (requires 'stadium_house.dv')
	   <n>       - integer for max number of cuts to be tested

   The variable AAF_TEST_DIR in this script needs to be set to the directory
   where the AAF files are created/read from.

   For testing purposes, I found it simpler to copy the libraries and 
   executables needed for testing from the development tree into a separate 
   directory.  I then set AAF_TEST_DIR in this script to the directory to 
   where I copied the libraries and executables.

   The test results are displayed on stdout and can be redirected to a
   file.


   To create and read 512 sector files

       sh <datadir>/runaaftest.sh read write 512b <n>

         where <datadir> should be set to the directory where the test data
		 has been stored

         <n> - max number of DV sources to be used

	   E.g. 

	   sh /home/joe/aaftestdata/runaaftest.sh read write 512b 4000

       sh /c/aaftestdata/runaaftest.sh read write 512b 4000
	       (/c means drive C: under MSYS)

   To create and read 4k sector files

       sh <datadir>/runaaftest.sh read write <n>

         where <datadir> should be set to the directory where the test data
		 has been stored

         <n> - max number of DV sources to be used.

	   E.g. 

	   sh /home/joe/aaftestdata/runaaftest.sh read write 4000

       sh /c/aaftestdata/runaaftest.sh read write 4000
