

type 'REG2' {
	longint = 1;			// version #
	longint = endofdata/8;	// size of data
	longint = 0;			// reserved, must be zero
	// FLAGS
		integer = 0;
		byte = 0;
		bitstring[6] = 0;
		boolean			callDllUnReg=1,	dontDllUnReg=0;				
		boolean			callDllReg=1, dontDllReg=0;
		align long;				
	longint = 0x45524943;
	longint = regKeys/8;
	longint = (regKeysEnd-regKeys)/8;
	longint = unregKeys/8;
	longint = (unregKeysEnd-unregKeys)/8;
	longint = 0;		// reserved
	longint = 0;		// reserved

  verifyKey:
	cstring;				// verify key
  verifyKeyEnd:

  FragName:
	cstring;				// verify key
  FragNameEnd:

	integer = $$CountOf (regArray);
  regKeys:
	array regArray {
		cstring;			// reg keys
	};
	align word;
  regKeysEnd:

	integer = $$CountOf (unregArray);
  unregKeys:
	array unregArray {
		cstring;			// unreg keys
	};
	align word;
  unregKeysEnd:

  nextArea:					// for future exp
  endofdata:				//
};


// #define OLE2_TMPL_OFF if you do not want
// this tmpl resource in your app/dll
#ifndef OLE2_TMPL_OFF

type 'TMPL' {
	array {
		pstring;												/* field name			*/
		literal longint;										/* field type			*/
	};
};

resource 'TMPL' (1, "REG2") {
	{
	"dwVersion",	'HLNG',
	"dwSizeofRsrc",	'HLNG',
	"(private)", 	'FLNG',
	// FLAGS
		"fill",		'FWRD',
		"fill",		'FBYT',
		"  (off)", 	'BBIT',
		"  (off)", 	'BBIT',
		"  (off)", 	'BBIT',
		"  (off)", 	'BBIT',
		"  (off)", 	'BBIT',
		"  (off)", 	'BBIT',
		"fUnregApi",'BBIT',
		"fRegApi", 	'BBIT',
	"(private)", 	'FLNG',
	"dwRegOffset", 	'HLNG',
	"dwRegSize", 	'HLNG',
	"dwUnregOffs",	'HLNG',
	"dwUnregSize",	'HLNG',
	"(private)", 	'FLNG',
	"(private)", 	'FLNG',
	"szVerifyKey",	'CSTR',
	"szFragName",	'CSTR',
	// REG KEYS
		"wNumKeyReg",	'OCNT',
		"REGISTER", 	'LSTC',
		"      szKey",	'CSTR',
		"REGISTER", 	'LSTE',
		"align", 		'AWRD',
	// UNREG KEYS
		"wNumKeyUnreg", 'OCNT',
		"UNREGISTR",	'LSTC',
		"      szKey",	'CSTR',
		"UNREGISTR", 	'LSTE',
		"align", 		'AWRD',
	};
};
#endif // TMPL

