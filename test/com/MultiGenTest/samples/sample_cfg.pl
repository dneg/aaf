#!/usr/bin/perl

# First, a few variables used to create entries in the CFG array.
# Your mileage may vary.

$LinuxBaseDir = "/home/aaftest";   # This is the local directory on the Linux server.
$IrixBaseDir  = "/mnt/aaftest";    # /home/aaftest on the Linux server accessed via NFS.
$Win2KBaseDir = "g:";              # /home/aaftest on the Linux server accessed via Samba

$LinuxAAFDir = "~jpt/aaf/V101_RC2/AAF/AAFi686LinuxSDK/g++";
$IrixAAFDir  = "~jpt/AAF/V101_RC2/AAF/AAFMipsIrixSDK/MIPSpro";
$Win2KAAFDir = "c:/cygwin/home/jpt/AAF/V101_RC2/AAF/AAFWinSDK";

$SharedTestDirPath = "testfiles";

#
# START HERE
#

# The CFG array is an associative array that stores all the
# configuration required by MultiGenPerl.pl.

%CFG = (

  # A list of platform names.  These are symbolic names, they do not,
  # for example, have to be machine names or network addresses.  Per
  # platform variations, such as compiler versions, could also be
  # expressed in the this list.  For example, we could use Linux296
  # and Linux303 to independentally verify Linux builds that use
  # different versions of gcc.

  Platforms => [ Win2K, Linux, Irix ],

  # A list of COM library versions.  Again these are just symbolic
  # names.  Every platform in the Platforms array must have a version
  # of the com library in the Versions list.

  Versions => [ V101, V1 ],

  # A list of the test that should be executed.  They will be executed
  # in order.  Each entry in this list must have an identically named
  # entry in the CFG array (find them below).

  Tests => [ TestLoad,
	     AddMobs,
	     ModifyMobs,
	     ModifyComments,
	     CreateEssence,
	     ModifyEssence,
	   ],

  # Use this test vector run the tests mentioned in ../ReadMe.html.
  #Tests => [ TestLoad, TestOne, TestTwo ],

  # A full path the COM library is required for each platform and
  # library version.  An CFG entry for each version in the Versions
  # list accomplishes this.  Each version entry maps the platform to a
  # full path to the library.

  V1   => { Win2K => "c:/cygwin/home/jpt/AAF/V1/AAFWinRC1src/AAF/AAFWinSDK/bin/AAFCOAPI.dll",
	    Linux => "~jpt/aaf/V1/AAF/AAFi686LinuxSDK/g++/bin/libcom-api.so",
	    Irix  => "~jpt/AAF/V1/AAF/AAFMipsIrixSDK/MIPSpro/bin/libcom-api.so",
	  },

  V101 => {  Win2K => "${Win2KAAFDir}/bin/AAFCOAPI.dll",
 	     Linux => "${LinuxAAFDir}/bin/libcom-api.so",
	     Irix  => "${IrixAAFDir}/bin/libcom-api.so",
	  },

  PlugInLibPath => {

      V1   => { Win2K => "c:/cygwin/home/jpt/AAF/V1/AAFWinRC1src/AAF/AAFWinSDK/bin/aafext/AAFPGAPI.DLL",
		Linux => "~jpt/aaf/V1/AAF/AAFi686LinuxSDK/g++/bin/libaafpgapi.so",
		Irix  => "~jpt/AAF/V1/AAF/AAFMipsIrixSDK/MIPSpro/bin/libaafpgapi.so",
	    },

      V101 => { Win2K => "${Win2KAAFDir}/bin/aafext/AAFPGAPI.DLL",
		Linux => "${LinuxAAFDir}/bin/libaafpgapi.so",
		Irix  => "${IrixAAFDir}/bin/libaafpgapi.so",
	    },
  },

  # Test are executed using the MultiGenTest program.
  # MultiGenTestPath maps versions and platform names to a full path
  # to the MultiGenTest executable.

  MultiGenTestPath =>
	{ V101 => { Win2K => "${Win2KAAFDir}/Release/Test/MultiGenTest.exe",
	            Linux => "${LinuxAAFDir}/bin/debug/MultiGenTest",
	            Irix  => "${IrixAAFDir}/bin/debug/MultiGenTest",
		  },

	  V1   => { Win2K => "c:/cygwin/home/jpt/AAF/V1/AAFWinRC1src/AAF/AAFWinSDK/Debug/Test/MultiGenTest.exe",
	            Linux => "~jpt/aaf/V1/AAF/AAFi686LinuxSDK/g++/bin/MultiGenTest",
	            Irix  => "~jpt/AAF/V1/AAF/AAFMipsIrixSDK/MIPSpro/bin/MultiGenTest",
		  },
        },

  # SharedTestDirPath maps the platform name to a full path the shared
  # directory where .aaf files should be stored.
  #
  # The .aaf files created by the MultiGenTest program must located in
  # a common directory accessible by all platforms.  Despite being
  # common, the path name could vary per platform. 

  SharedTestDirPath =>
	{ Win2K => "${Win2KBaseDir}/${SharedTestDirPath}",
	  Linux => "${LinuxBaseDir}/${SharedTestDirPath}",
	  Irix  => "${IrixBaseDir}/${SharedTestDirPath}",
        },

  # A remote shell command is required to execute the MultiGenTest
  # program on each platform.  This can by anything.  Typically it
  # will simply be ssh or rsh.  The command is expressed as an array
  # of arguments not as a single string.  The machine on which
  # MultiGenTest.pl is executed (i.e. the local machine) does not
  # require a remote shell, hence, it simply has a empty entry.

  SysExecArgs => 
	{ Win2K => [],
	  Linux => [ ssh, "jpt\@192.168.1.101" ],
	  Irix  => [ rsh, "jpt\@192.168.1.104" ],
	},


  # Test exclusion parameters:
  #
  # V1 of the AAF COM library does not support modifying files.  The
  # same is true for V1.0.1 of the COM library which supports modification
  # only if the platforms that created and modified the file are of
  # identical byte order.  This is handled by simplying excluding
  # tests that are expected to fail.
  #
  # The following CFG entries allow these exclusion rules to be turned
  # on/off.
  #
  # These are CFG entries are a bit ad-hoc at the moment.  A better
  # approach might to let the tests fail, then check if the failure
  # was expected.  How could that be generically expressed?
  #
  # Versions  - array of library versions that do not support modify.
  # ByteOrder - exclude modify tests if the byte ordering of the 
  #             creator and modifier do not match.
    
  NoModifySupport => { Versions  => { V1 => "true" },
		       ByteOrder => "false"
		     },

  ByteOrder => { Win2K => "little",
		 Linux => "little",
		 Irix  => "big"
		 },

  # Script control policy parameters:
  #
  # Parameters that affect program flow control.
  #
  # If ExitOnTestFailure is true, MultiGenTest.pl will exit
  # immediately if a test fails.

  ExitOnTestFailure => "false",

  # The test script must copy .aaf files before executing modify
  # tests.  The copy command, and the host it should be executed
  # on must be specified.
  #
  # In this case, the files are all stored on a Linux server, while
  # the MultiGenTest.pl script is run on a Win2K machine.  It is
  # assumed that executing the copy directly on the server will be the
  # fastest way to copy a file, hence, the copy command is executed on
  # that machine.

  CopyHost    => "Linux",
  CopyCommand => [ "/bin/cp", "-f" ],

  # Finally, there must be one CFG entry for each test in the Tests
  # vector.  Each test entry is an array with the following syntax:
  #
  # Index 0 is the command option: "load" | "create" | "modify"
  #
  # "load": will form and execute a MultiGenTest command that simply
  # loads and unloads the COM library.
  #
  # "create": will form and execute a MultiGenTest command that
  # creates a new file and runs the command stored at index 1.  The
  # resulting .aaf file is verifying using the command at index 3.
  #
  # "modify": will form and execute a MultiGenTest command that
  # modifies an existing .aaf file.  Index 3 identifies the test that
  # created the file to be modified.  Index 1 is the MultiGenTest
  # command that modifies the file.  Index 2 is the MultiGenTest
  # command that verifies the result.
  #
  # In this example, TestOne creates a file and runs the AddMasterMobs
  # test to add two master mobs named A and B.  The verify command
  # checks that these mobs are found in the file.  TestTwo opens the
  # file created by TestOne, and adds two more master mobs named C and
  # D. The TestTwo verify command checks that all four mobs are found
  # in the file.

  TestLoad => [ "load" ],

  TestOne => [ "create",
	       [ "AddMasterMobs A B" ],
	       [ "FindMasterMobsExclusive A B" ],
	     ],

  TestTwo => [ "modify",
	       [ "AddMasterMobs C D" ],
	       [ "FindMasterMobsExclusive A B C D" ],
	       "TestOne"
	     ],

  AddMobs =>

	[ "create",
	  
	  ["MasterMobAdd A",
	   "MasterMobAdd B",
	   "MasterMobAdd C",
	   "MasterMobAdd D",
	   "AppendComment A CategoryOne   CommentOne",
	   "AppendComment A CategoryTwo   CommentTwo",
	   "AppendComment A CategoryThree CommentThree",
	  ],
	  
	  ["FindMasterMob A",
	   "FindMasterMob B",
	   "FindMasterMob C",
	   "FindMasterMob D",
	   "CountMasterMobs 4",
	   "FindComment A CategoryOne   CommentOne",
	   "FindComment A CategoryTwo   CommentTwo",
	   "FindComment A CategoryThree CommentThree",
	   "CountComments A 3",
	  ],
      ],

  ModifyMobs =>

	[ "modify",

	  [ "MasterMobAdd E",
	    "RenameMob A G",
	    "MasterMobAdd F",
	    "RenameMob D H",
	    "RemoveMob B",
	    "RemoveMob C",
	  ],
	  
	  [ "FindMasterMob E",
	    "FindMasterMob F",
	    "FindMasterMob G",
	    "FindMasterMob H",
	    "CountMasterMobs 4",
	  ],
	  
	  "AddMobs"
       ],

  ModifyComments =>

       [  "modify",

	  [ "AppendComment A CategoryOne   CommentOneChanged",
	    "AppendComment A CategoryFour  CommentFour",
	    "RemoveComment A CategoryTwo   CommentTwo",
	    "RemoveComment A CategoryThree CommentThree",
	    "AppendComment B CategoryOne   CommentOne",
	  ],

	  [ "FindComment A CategoryOne  CommentOneChanged",
	    "FindComment A CategoryFour CommentFour",
	    "CountComments A 2",
	    "FindComment B CategoryOne  CommentOne",
	    "CountComments B 1",
	  ],

	  "AddMobs",
       ],

  CreateEssence =>

       [ "create",

	 [
	    "MasterMobAdd A",
	    "EssenceCreate A 1 picture cdci uncompressed    3",
	    "EssenceCreate A 2   sound wave uncompressed 3000",
	    "MasterMobAdd B",
	    "EssenceCreate B 1 picture cdci uncompressed    4",
	    "EssenceCreate B 2   sound wave uncompressed 4000",
	 ],

	 [
	    "SlotDataDef A 1 picture",
	    "SlotDataDef A 2   sound",
	    "SlotDataDef B 1 picture",
	    "SlotDataDef B 2   sound",
	    "CountSamples A 1 uncompressed    3",
	    "CountSamples A 2 uncompressed 3000",
	    "CountSamples B 1 uncompressed    4",
	    "CountSamples B 2 uncompressed 4000",
	 ],

       ],

  ModifyEssence =>

	[ "modify",

	  [
	     "EssenceAppend A 1 uncompressed    3",
	     "EssenceAppend A 2 uncompressed 3000",
	     "EssenceAppend B 1 uncompressed    4",
	     "EssenceAppend B 2 uncompressed 4000"
	  ],
	  
	  [
	    "CountSamples A 1 uncompressed 7", 
	    "CountSamples A 2 uncompressed 7000", 
	    "CountSamples B 1 uncompressed 8", 
	    "CountSamples B 2 uncompressed 8000", 
	  ],

	  "CreateEssence"

	],
);
