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
} AafOmfGlobals;

int deleteFile( char* fileName );
void IncIndentLevel( void );
void DecIndentLevel( void );
void AUIDtoString(aafUID_t *uid, char *buf);
AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	major,
		aafUInt32	minor,
		aafUID_t *mobID);     /* OUT - Newly created Mob ID */


// Media composer effect IDs
const aafUID_t kEffBlendPIPUUID =		{0xD94E75C2,0x6ADF,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffBlendSuperUUID =	{0xD94E75C4,0x6ADF,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffBlendFadeUpUUID =	{0xD94E75C7,0x6ADF,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffBlendDipUUID =		{0xD94E75C8,0x6ADF,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffXRotate = {0x783B4810,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffYRotate = {0x783B4811,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffZRotate = {0x783B4812,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeZoom = {0x783B4813,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeHorz = {0x783B4814,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeVert = {0x783B4815,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeBottomCenter = {0x783B4816,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeRightCenter = {0x783B4817,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeTopCenter = {0x783B4818,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeLeftCenter = {0x783B4818,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeTop = {0x783B481A,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeBottom = {0x783B481B,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeLeft = {0x783B481C,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeRight = {0x783B481D,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeBottomLeft = {0x783B481E,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeTopRight = {0x783B481F,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeBottomRight = {0x783B4820,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kEffSqueezeTopLeft = {0x783B4821,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};

// Media composer parameter IDs
const aafUID_t kAAFParamID_AvidBorderWidth =	{0x8BC42735,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidBorderSoft =	{0x8BC42736,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidXPos =	{0x8BC42737,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidYPos =	{0x8BC42738,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidCrop =	{0x8BC42739,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidScale =	{0x8BC4273A,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidSpillSupress =	{0x8BC4273B,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidBounds =	{0x8BC4273C,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidColor =	{0x8BC4273D,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidUserParam =	{0x8BC4273E,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFParamID_AvidEffectName = {0x783B480F,0x7CF6,0x11d3,{0x80,0xD5,0x00,0x60,0x08,0x14,0x3E,0x6F}};

// Media Composer type ID's
const aafUID_t kAAFTypeID_AvidPosition =	{0x8BC4272E,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFTypeID_AvidCrop =	{0x8BC4272F,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFTypeID_AvidScale =	{0x8BC42730,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFTypeID_AvidSpillSupress =	{0x8BC42731,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFTypeID_AvidBounds =	{0x8BC42732,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFTypeID_AvidEffColor =	{0x8BC42733,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};
const aafUID_t kAAFTypeID_AvidEffUserParam =	{0x8BC42734,0x6BAB,0x11d3,{0x80,0xCF,0x00,0x60,0x08,0x14,0x3E,0x6F}};

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

