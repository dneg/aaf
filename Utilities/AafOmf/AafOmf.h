/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

/************************************************************************
 *
 * Omf2Aaf.h Describes structures and prototypes for the OMF to AAF 
 *          conversion utility.
 *
 ************************************************************************/

  
#if defined(_MAC) || defined(macintosh)
#include <initguid.h> // define all of the AAF guids.
#include "AAF.h"
#else
#include "AAF.h"

#endif

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFOperationDefs.h"
#include "AAFParameterDefs.h"
#include "AAFInterpolatorDefs.h"
#include "AAFTypeDefUIDs.h"

const int MAX_INDENT = 8;

// ============================================================================
// simple helper class to initialize and cleanup COM library.
// ============================================================================
struct CComInitialize
{
	CComInitialize()
	{
		CoInitialize(NULL);
	}

	~CComInitialize()
	{
		CoUninitialize();
	}
};

typedef struct _AafOmfGlobals
{
	aafInt16		numIndents;
	char			indentLeader[MAX_INDENT+1];	

	aafBool			bVerboseMode;
	aafBool			bCreateTOCFile;
	aafBool			bConvertAllObjects;
	aafBool			bOMFFileOpen;
	aafBool			bAAFFileOpen;
	aafBool			bLogFile;
	aafBool			bDefFile;
	aafBool			bDeleteOutput;
	aafBool			bConvertAAFFile;

	// For Statistical summary
	aafInt32		nNumOMFMobs;
	aafInt32		nNumAAFMobs;
	aafInt32		nNumOMFObjects;
	aafInt32		nNumAAFObjects;
	aafInt32		nNumOMFProperties;
	aafInt32		nNumAAFProperties;
	aafInt32		nNumUndefinedOMFObjects;
	aafInt32		nNumUndefinedOMFProperties;
	aafInt32		nNumUndefinedAAFObjects;
	aafInt32		nNumUndefinedAAFProperties;

	char			sInFileName[256];
	char			sTOCFileName[256];
	char			sDefinitionFileName[256];
	char			sOutFileName[256];
	char			sLogFileName[256];
	char*			pProgramName;

	// MC Private Properties
	OMF2::omfProperty_t		pvtEffectIDProp;
	OMF2::omfProperty_t		pvtAppCode;

	// Codec Properties
	OMF2::omfProperty_t		omCDCIComponentWidth;
	OMF2::omfProperty_t		omCDCIHorizontalSubsampling;
	OMF2::omfProperty_t		omCDCIColorSiting;
	OMF2::omfProperty_t		omCDCIBlackReferenceLevel;
	OMF2::omfProperty_t		omCDCIWhiteReferenceLevel;
	OMF2::omfProperty_t		omCDCIColorRange;
	OMF2::omfProperty_t		omCDCIPaddingBits;
} AafOmfGlobals;

int deleteFile( char* fileName );
void IncIndentLevel( void );
void DecIndentLevel( void );
void AUIDtoString(aafUID_t *uid, char *buf);
AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	major,
		aafUInt32	minor,
		aafUID_t *mobID);     /* OUT - Newly created Mob ID */
void RegisterCodecProperties(AafOmfGlobals *globals, OMF2::omfSessionHdl_t OMFSession);
void RegisterOMFMCPrivate(AafOmfGlobals *globals, OMF2::omfSessionHdl_t OMFSession);
void RegisterAAFMCPrivate(IAAFDictionary * dict);
HRESULT SetIntegerPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, aafUID_t* pPropID, const aafUID_t* pIntTypeID,
							   aafMemPtr_t pValue, aafUInt32 ValueSize, IAAFDictionary *dict);

HRESULT GetIntegerPropFromObject(IAAFObject* pObj, const aafUID_t* pClassID, aafUID_t* pPropID,
								 const aafUID_t* pIntTypeID, aafMemPtr_t pValue, aafUInt32 ValueSize, IAAFDictionary *dict);

const aafUID_t AUID_PropertyMobAppCode = { 0x96c46992, 0x4f62, 0x11d3, { 0xa0, 0x22, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb } };

#define MAX_EFFECT_COLORS		16

typedef	unsigned long	AvFixed30;
typedef	unsigned long	AvFixed16;

typedef struct
	{
	unsigned char 		hue;
	unsigned char 		sat;
	unsigned char 		lum;
	unsigned char		dep;
	} hsl8Color_t, **hsl8ColorHdl;

typedef	struct
	{
	AvFixed16	top;
	AvFixed16	left;
	AvFixed16	bottom;
	AvFixed16	right;
	unsigned char		Lvl2Xscale;
	unsigned char		Lvl2Yscale;
	unsigned char		Lvl2Xpos;
	unsigned char		Lvl2Ypos;
	} OMFIPvtFixedRect;

typedef struct
	{
	long		cookie;
	long		revision;
	long	    selected;			// Boolean would have struct alignment problems x-platform
	AvFixed30	percentTime;
	AvFixed30	level;
	AvFixed16	posX;
	AvFixed16	XFloor;
	AvFixed16	XCeiling;
	AvFixed16	posY;
	AvFixed16	YFloor;
	AvFixed16	YCeiling;
	AvFixed16	Xscale;
	AvFixed16	Yscale;

	AvFixed16	cropLeft;
	AvFixed16	cropRight;
	AvFixed16	cropTop;
	AvFixed16	cropBottom;
	
	OMFIPvtFixedRect	Box;
	long				borderWidth;
	long				borderSoft;
	long				nColors;
	
	short				secondGain;
	short				spillGain;
	
	short				secondSoft;
	short				spillSoft;

	char				enableKeyFlags;
	char				pad1;

	hsl8Color_t 		colors[MAX_EFFECT_COLORS];

// userParamSize must ALWAYS be the last field of this structure.  It doesn't have to
// be the last one written to the domain, but it must be the last one here.  userParamSize
// must always remain a long because people are using it to determine the position of the
// userParams which is glommed on the end.  POC.

	long				userParamSize;
// the userParams are just glommed onto the end of this structure at this position.  POC.

	} OMFIPvtKFInfo_t;

typedef struct
	{
	long				cookie;
	long				rev;
	long				kfCurrent;
	long				kfSmooth;
	short				colorItem;
	short				quality;
	unsigned char		isReversed;
	unsigned char		ScalesDetached;
	} OMFIPvtGlobalInfo_t;

