#ifndef _AAF_OMF_
#define _AAF_OMF_	1
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
#include "StreamLogger.h"

//#ifndef _INCLUDED_OMF2_
//#define _INCLUDED_OMF2_
//namespace OMF2
//{
//#include "omPublic.h"
//#include "omMedia.h"
//}
//#endif

const int MAX_INDENT = 8;
const unsigned	kLogError = 0; 	// Error level.
const unsigned	kLogWarn 	= 1;  // Warning level.
const unsigned	kLogInfo  = 2; 	// Informational level.

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
	StreamLogger*		pLogger;

	// MC Private Properties
	omfProperty_t		pvtEffectIDProp;
	omfProperty_t		pvtAppCode;
	omfProperty_t		pvtAttributes;
	omfProperty_t		pvtDataAttribute;

	// Codec Properties
	omfProperty_t		omCDCIComponentWidth;
	omfProperty_t		omCDCIHorizontalSubsampling;
	omfProperty_t		omCDCIColorSiting;
	omfProperty_t		omCDCIBlackReferenceLevel;
	omfProperty_t		omCDCIWhiteReferenceLevel;
	omfProperty_t		omCDCIColorRange;
	omfProperty_t		omCDCIPaddingBits;
	~_AafOmfGlobals( void )		{ delete pLogger; }
} AafOmfGlobals;


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


int deleteFile( char* fileName );
void IncIndentLevel( void );
void DecIndentLevel( void );
void AUIDtoString(aafUID_t *uid, char *buf);
void MobIDtoString(aafMobID_constref uid, char *buf);
AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	prefix,
        aafUInt32	major,
		aafUInt32	minor,
		aafUInt8	UMIDType,
		aafMobID_t *mobID);     /* OUT - Newly created Mob ID */
void RegisterCodecProperties(AafOmfGlobals *globals, omfSessionHdl_t OMFSession);
HRESULT InitGlobalVars( void );
HRESULT IsOMFFile (char * pFileName );

extern AafOmfGlobals*	gpGlobals;


#define COMMON_ERR_BASE		(AAFRESULT)0xE0000000
#define AAF2OMF_ERR_BASE	(AAFRESULT)0xE0001000
#define OMF2AAF_ERR_BASE	(AAFRESULT)0xE0006000
#define AAFRESULT_FILE_NOT_OMF (OMF2AAF_ERR_BASE + 1)
#endif
