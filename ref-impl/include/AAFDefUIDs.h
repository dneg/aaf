const aafUID_t DDEF_Video = { 0x6F3C8CE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t DDEF_Audio = { 0x78E1EBE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t DDEF_Timecode = { 0x7F275E81, 0x77E5, 0x11D2, { 0x80, 0x7F, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

const aafUID_t NilMOBID = { 0xF15D2C20, 0x6D22, 0x11d2, { 0x80, 0x7D, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

const aafUID_t kAAFFmtEnd = { 0x6AEFC4F1, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
		 
const aafUID_t kAAFPixelFormat = { 0x6AEFC4F2, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expPixelFormat */
const aafUID_t kAAFFrameLayout = { 0x6AEFC4F3, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } }; 		/* operand.expFrameLayout */
const aafUID_t kAAFFieldDominance = { 0x6AEFC4F4, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expFieldDom */
const aafUID_t kAAFStoredRect = { 0x6AEFC4F5, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } }; 		/* operand.expRect */
const aafUID_t kAAFDisplayRect = { 0x6AEFC4F6, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };  		/* operand.expRect */
const aafUID_t kAAFSampledRect = { 0x6AEFC4F7, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };  		/* operand.expRect */
const aafUID_t kAAFPixelSize = { 0x6AEFC4F8, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } }; 			/* operand.expInt16 */
const aafUID_t kAAFAspectRatio = { 0x6AEFC4F9, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expRational */
const aafUID_t kAAFAlphaTransparency = { 0x6AEFC4FA, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };  /* operand.expInt32 */
const aafUID_t kAAFGamma = { 0x6AEFC4FC, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };				/* operand.expRational */
const aafUID_t kAAFImageAlignmentFactor = { 0x6AEFC4FD, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } }; /* operand.expInt32 */
const aafUID_t kAAFVideoLineMap = { 0x6AEFC4FE, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expLineMap */
const aafUID_t kAAFWillTransferLines = { 0x6AEFC4FF, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };	/* operand.expBoolean */
const aafUID_t kAAFIsCompressed = { 0x6AEFC500, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expBoolean */
const aafUID_t kAAFLineLength = { 0x6AEFC501, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };			/* operand.expInt32 */
		/* the following group is exclusive to RGBA codec */
const aafUID_t kAAFRGBCompLayout = { 0x6AEFC502, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expCompArray */
const aafUID_t kAAFRGBCompSizes = { 0x6AEFC503, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		/* operand.expCompSizeArray */
const aafUID_t kAAFRGBPalette = { 0x6AEFC504, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };			/* operand.expPointer */
const aafUID_t kAAFRGBPaletteLayout = { 0x6AEFC505, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };	/* operand.expCompArray */
const aafUID_t kAAFRGBPaletteSizes = { 0x6AEFC506, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };	/* operand.expCompSizeArray */	
			/* the following group is exclusive to CDCI codec */
const aafUID_t kAAFCDCICompWidth = { 0x6AEFC507, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };	    /* operand.expInt32 */
const aafUID_t kAAFCDCIHorizSubsampling = { 0x6AEFC508, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };/* operand.expUInt32 */
const aafUID_t kAAFCDCIColorSiting = { 0x6AEFC509, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };    /* operand.expColorSiting */
const aafUID_t kAAFCDCIBlackLevel = { 0x6AEFC50A, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };     /* operand.expUInt32 */
const aafUID_t kAAFCDCIWhiteLevel = { 0x6AEFC50B, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };     /* operand.expUInt32 */
const aafUID_t kAAFCDCIColorRange = { 0x6AEFC50C, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };     /* operand.expUInt32 */
const aafUID_t kAAFCDCIPadBits = { 0x6AEFC50D, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };	    /* operand.expInt16 */
			
const aafUID_t kAAFSampleSize = { 0x6AEFC50E, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } }; 
const aafUID_t kAAFSampleRate = { 0x6AEFC50F, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } }; 
const aafUID_t kAAFSampleFormat = { 0x6AEFC510, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } }; 
const aafUID_t kAAFNumChannels = { 0x6AEFC511, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafUID_t kAAFExpLong = { 0x6AEFC512, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };		// TomR:Added to support all options in the union
const aafUID_t kAAFExpRational = { 0x6AEFC513, 0x9041, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } }; 	// TomR:Added to support all options in the union
