// @doc INTERNAL
// @com This file implements the conversion of OMF files to AAF file format.
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
 * prior written permission of Avid Technology, Inc.
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>
#include <memory>

// OMF Includes
namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
#include "omEffect.h"
#include "omDefs.h"
#include "omUtils.h"
}
#include "AafOmf.h"
#define OMFI_ENABLE_SEMCHECK	1		// Move to project


#include "AAFTypes.h"
#include "aafcodecdefs.h"

#include "CAAFBuiltinDefs.h"

//#include "AAFUtils.h"
//AAFRESULT aafMobIDNew(aafUID_t *mobID);
//AAFRESULT aafMobIDFromMajorMinor(
//        aafUInt32	major,
//		aafUInt32	minor,
//		aafUID_t *mobID);     /* OUT - Newly created Mob ID */

//#include "aafCvt.h"

#include "AAFDomainUtils.h"
#include "OMFDomainUtils.h"
#if AVID_SPECIAL
#include "ConvertAvid.h"
#include "AAFDomainAvidUtils.h"
#include "OMFDomainAvidUtils.h"
#include "AvidEffectTranslate.h"
#else
#include "Extensions.h"
#include "AAFDomainExtensions.h"
#include "OMFDomainExtensionUtils.h"
#endif
#include "omf2aaf.h"
#include "EffectTranslate.h"
#include "AAFDomainUtils.h"
#include "AAFException.h"
#include "OMFException.h"
#include "AutoRelease.h"
#include "AAFClassDefUIDs.h" 

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFDataDef> IAAFDataDefSP;

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

const aafUID_t ProductID = { 0x3B4794A1, 0x299C, 0x11d3, { 0x80, 0xAE, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

extern AafOmfGlobals* gpGlobals;
// ============================================================================
// Constructor
// ============================================================================
Omf2Aaf::Omf2Aaf() : pFile(NULL), pHeader(NULL), pDictionary(NULL)
{
	OMFSession = 0;
	OMFFileHdl = 0;
	OMFFileRev = OMF2::kOmfRev1x;
	pAAF = new AAFDomainExtensionUtils;
	pOMF = new OMFDomainExtensionUtils;
#if AVID_SPECIAL
	pEffectTranslate = new AvidEffectTranslate;
#else
	pEffectTranslate = new EffectTranslate;
#endif
}

// ============================================================================
// Destructor
// ============================================================================
Omf2Aaf::~Omf2Aaf()
{
	if (pHeader)
		pHeader->Release();
	if (pDictionary)
		pDictionary->Release();
	if (pFile)
	{
		pFile->Save();
		pFile->Close();
		pFile->Release();
	}
}
// ============================================================================
// ConvertFile
//			This function provides the control and logic to convert an OMF file
//			by calling all the functions inside this object
//
// ============================================================================
void Omf2Aaf::ConvertFile ()
{
	OMFFileOpen( gpGlobals->sInFileName );
	OpenOutputFile();
	ConvertOMFHeader();
	OMFFileRead();
	OMFFileClose();
	AAFFileClose();
}

// ============================================================================
// OpenOutputFile
//			This function creates the output file.
//
// ============================================================================
void Omf2Aaf::OpenOutputFile ()
{
	if (strlen(gpGlobals->sOutFileName) == 0)
	{
		char*	pExt;
		strcpy(gpGlobals->sOutFileName, gpGlobals->sInFileName);
		pExt= strrchr(gpGlobals->sOutFileName, '.');
		strcpy(pExt,".aaf");
	}
	
	if (gpGlobals->bDeleteOutput)
	{
		HRESULT rc = deleteFile(gpGlobals->sOutFileName);
		gpGlobals->pLogger->Log(kLogWarn, 
			"Output file: %s will be %s\n", gpGlobals->sOutFileName, 
			rc == AAFRESULT_SUCCESS ? "overwritten" : "created");
	}

	AAFFileOpen(gpGlobals->sOutFileName);
	return;
}
// ============================================================================
// OMFFileOpen
//
//		Open an OMF File
//
// ============================================================================
void Omf2Aaf::OMFFileOpen(char * pFileName)
{
	aafBool				bSessionStarted = kAAFFalse;
	OMFCheck			ret;
	gpGlobals->pLogger->Log( kLogInfo,"Opening OMF file \"%s\"\n", pFileName);
	try
	{
		ret = OMF2::omfsBeginSession(0, &OMFSession);
		bSessionStarted = kAAFTrue;
		ret = OMF2::omfmInit(OMFSession);
		ret = OMF2::omfsOpenFile((OMF2::fileHandleType)pFileName, OMFSession, &OMFFileHdl);
		gpGlobals->bOMFFileOpen = kAAFTrue;
	}
	catch( ExceptionBase )
	{
		if (bSessionStarted)
		{
			OMF2::omfsEndSession(OMFSession);
		}
		throw;
	}

	RegisterCodecProperties(gpGlobals, OMFSession);
	pOMF->RegisterOMFProperties(gpGlobals, OMFSession);

	ret = OMF2::omfsFileGetRev(OMFFileHdl, &OMFFileRev);
	int revision = (OMF2::kOmfRev2x == OMFFileRev) ? 2 : 1;
	gpGlobals->pLogger->Log( kLogInfo, 
		"OMF file \"%s\" opened succesfully.\nFile Revision %d.0\n", 
		pFileName, revision);

	return;
}

// ============================================================================
// AAFFileOpen
//
//		Open an AAF File
//
// ============================================================================
void Omf2Aaf::AAFFileOpen( char* pFileName)
{
	AAFCheck				rc ;
	OMFCheck				OMFError;
	aafWChar*				pwCompanyName;
	aafWChar*				pwProductName;
    aafWChar*				pwProductVersionString;
    aafWChar*				pwPlatform;
	aafBool					bAddExtraIdent = kAAFFalse;
	aafProductIdentification_t	ProductInfo;

	std::auto_ptr<wchar_t> pwFile( new wchar_t[strlen(pFileName)+1] );
	aafWChar*	pwFileName = pwFile.get();
	mbstowcs(pwFileName, pFileName, strlen(pFileName)+1);

	// Get Identification from OMF Header 
	// We going to use the last identification entry in the
	// Identification array..
	OMF2::omfObject_t		OMFHeadObject;
	OMFError = OMF2::omfsGetHeadObject(OMFFileHdl, &OMFHeadObject);
	aafInt32 nOMFIdentifications = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeadObject, OMF2::OMHEADIdentList);
	if (nOMFIdentifications >= 1) 
	{
		OMF2::omfProductVersion_t OMFVersion;
		OMF2::omfObject_t		OMFIdent;
		char	text[256];

		OMFError = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeadObject, OMF2::OMHEADIdentList,
										&OMFIdent, nOMFIdentifications);
		if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTCompanyName, text, sizeof(text)) != OMF2::OM_ERR_NONE)
			strcpy(text, "<Not Specified>");
		std::auto_ptr<wchar_t> pwcompany( new wchar_t[strlen(text)+1] );
		pwCompanyName = pwcompany.get();
		mbstowcs(pwCompanyName, text, strlen(text)+1);
		ProductInfo.companyName = pwCompanyName;
		if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTProductName, text, sizeof(text)) != OMF2::OM_ERR_NONE)
			strcpy(text, "<Not Specified>");
		std::auto_ptr<wchar_t> pwproduct( new wchar_t[strlen(text)+1] );
		pwProductName = pwproduct.get();
		mbstowcs(pwProductName, text, strlen(text)+1);
		ProductInfo.productName = pwProductName;
		if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTProductVersionString, text, sizeof(text)) != OMF2::OM_ERR_NONE)
			strcpy(text, "<Not Specified>");
		std::auto_ptr<wchar_t> pwversion( new wchar_t[strlen(text)+1] );
		pwProductVersionString =  pwversion.get();
		mbstowcs(pwProductVersionString, text, strlen(text)+1);
		ProductInfo.productVersionString = pwProductVersionString;
		if (OMF2::omfsReadProductVersionType(OMFFileHdl, OMFIdent, OMF2::OMIDNTProductVersion, &OMFVersion) != OMF2::OM_ERR_NONE)
		{
			OMFVersion.major = 0;
			OMFVersion.minor = 0;
			OMFVersion.tertiary = 0;
			OMFVersion.patchLevel = 0;
			OMFVersion.type = OMF2::kVersionUnknown;
		}
		ProductInfo.productVersion.major = OMFVersion.major;
		ProductInfo.productVersion.minor = OMFVersion.minor;
		ProductInfo.productVersion.tertiary = OMFVersion.tertiary;
		ProductInfo.productVersion.patchLevel = OMFVersion.type;
		ProductInfo.productVersion.type = (aafProductReleaseType_t)OMFVersion.type;

		if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTPlatform, text, sizeof(text)) != OMF2::OM_ERR_NONE)
			strcpy(text, "<Not Specified>");
		std::auto_ptr<wchar_t> pwplatform( new wchar_t[strlen(text)+1] );
		pwPlatform = pwplatform.get();
		mbstowcs(pwPlatform, text, strlen(text)+1);
		ProductInfo.platform = pwPlatform;
		rc = AAFFileOpenNewModify(pwFileName, 0, &ProductInfo, &pFile);
		bAddExtraIdent = kAAFTrue;
	}
	else
	{
		ProductInfo.companyName = L"Company Name";
		ProductInfo.productName = L"AAF/OMF File Conversion";
		ProductInfo.productVersion.major = 1;
		ProductInfo.productVersion.minor = 0;
		ProductInfo.productVersion.tertiary = 0;
		ProductInfo.productVersion.patchLevel = 0;
		ProductInfo.productVersion.type = kAAFVersionDebug;
		ProductInfo.productVersionString = NULL;
		ProductInfo.productID = ProductID;
		ProductInfo.platform = NULL;
		rc = AAFFileOpenNewModify(pwFileName, 0, &ProductInfo, &pFile);
	}

	gpGlobals->bAAFFileOpen = kAAFTrue;
	rc = pFile->GetHeader(&pHeader);
	rc = pHeader->GetDictionary(&pDictionary);
	CAAFBuiltinDefs defs (pDictionary);

	if (bAddExtraIdent)
	{
		// Create a new Identification
		IAAFIdentification*		pIdent = NULL;
		rc = defs.cdIdentification()->
		  CreateInstance(IID_IAAFIdentification,
						 (IUnknown **)&pIdent);
		AutoRelease<IAAFIdentification> pident( pIdent );
		ProductInfo.companyName = L"Company Name";
		ProductInfo.productName = L"OMF to AAF File Conversion";
		ProductInfo.productVersion.major = 1;
		ProductInfo.productVersion.minor = 0;
		ProductInfo.productVersion.tertiary = 0;
		ProductInfo.productVersion.patchLevel = 0;
		ProductInfo.productVersion.type = kAAFVersionDebug;
		ProductInfo.productVersionString = NULL;
		ProductInfo.productID = ProductID ;
		ProductInfo.platform = NULL;
		pIdent->SetCompanyName(ProductInfo.companyName);
		pIdent->SetProductName(ProductInfo.productName);
		pIdent->SetProductVersion(&ProductInfo.productVersion);
		rc = pHeader->AppendIdentification(pIdent);
	}
	pAAF->RegisterAAFProperties(pDictionary);
	gpGlobals->pLogger->Log(kLogInfo, "AAF File: %s Created succesfully\n", pFileName);
	pAAF->SetDictionary(pDictionary);

	// auto_ptrs will clean up all allocated memory upon return;
	return;
}

// ============================================================================
// OMFFileRead
//
//		Here is where all the real work is done.  We recursively read all 
//		Objects inside the OMF file.
//
// ============================================================================
void Omf2Aaf::OMFFileRead()
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	aafInt32				nOMFNumMobs;
	aafInt32				nOMFMobCount;

	// OMF Variables
	OMF2::omfIterHdl_t		OMFMobIter = NULL;
	OMF2::omfMobObj_t		OMFMob;
	OMF2::omfObject_t		OMFHeader, OMFObject;
	OMF2::omfProperty_t		OMFPropertyID;

	// AAF Variables
	aafInt32				numMedia;
	
	CAAFBuiltinDefs defs (pDictionary);

	OMFError = OMF2::omfiIteratorAlloc( OMFFileHdl, &OMFMobIter);
	OMFError = OMF2::omfiGetNumMobs( OMFFileHdl, OMF2::kAllMob, &nOMFNumMobs);
	gpGlobals->pLogger->Log( kLogInfo, "Found: %ld Mobs in the input file\n", nOMFNumMobs);
	for (nOMFMobCount = 0; nOMFMobCount < nOMFNumMobs; nOMFMobCount++)
	{
		OMFError = OMF2::omfiGetNextMob(OMFMobIter, NULL, &OMFMob);
		IAAFMob*	pMob = NULL;
		AutoRelease <IAAFMob> pmob;
		gpGlobals->nNumOMFMobs++;
		OMF2::omfErr_t err; // Dummy
		if (OMF2::omfiIsACompositionMob(OMFFileHdl, OMFMob, &err ))
		{
			// Create a new Composition Mob
			IAAFCompositionMob*		pCompMob;
			rc = defs.cdCompositionMob()->
			  CreateInstance(IID_IAAFCompositionMob,
							 (IUnknown **)&pCompMob);
			AutoRelease<IAAFCompositionMob> ar(pCompMob);
			gpGlobals->pLogger->Log( kLogInfo, "Created AAF Composition Mob\n");
			ConvertOMFCompositionObject( OMFMob, pCompMob );
			rc = pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			pmob = pMob;
		}
		else if (OMF2::omfiIsAMasterMob(OMFFileHdl, OMFMob, &err) )
		{
			// Create a Master Mob 
			IAAFMasterMob*			pMasterMob;
			rc = defs.cdMasterMob()->
			  CreateInstance(IID_IAAFMasterMob,
							 (IUnknown **)&pMasterMob);
			AutoRelease<IAAFMasterMob> ar(pMasterMob);
			gpGlobals->pLogger->Log( kLogInfo, "Created AAF Master Mob\n");
			ConvertOMFMasterMob(OMFMob, pMasterMob );
			rc = pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			pmob = pMob;
		}
		else if ( OMF2::omfiIsASourceMob(OMFFileHdl, OMFMob, &err) )
		{
			IAAFSourceMob*			pSourceMob;
			rc = defs.cdSourceMob()->
			  CreateInstance(IID_IAAFSourceMob,
							 (IUnknown **)&pSourceMob);
			AutoRelease<IAAFSourceMob> ar(pSourceMob);
			gpGlobals->pLogger->Log( kLogInfo, "Created AAF Source Mob\n");
			ConvertOMFSourceMob( OMFMob, pSourceMob );
			rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			pmob = pMob;
		}
		else
		{
			OMF2::omfClassID_t		objClass;
			char					id[5];
			OMFError = OMF2::omfsReadClassID(OMFFileHdl, OMFMob, OMF2::OMOOBJObjClass, objClass);
			strncpy(id, objClass, 4);
			id[4] = '\0';
			gpGlobals->pLogger->Log( kLogWarn, "Unrecognized Mob Class ID: %s\n",id);
		}

		if (pMob)
		{
			gpGlobals->pLogger->Log( kLogInfo, "Converting basic MOB data...\n" );
			ConvertOMFMOBObject( OMFMob, pMob);
			gpGlobals->pLogger->Log( kLogInfo, "Traversing MOB...\n" );
			TraverseOMFMob( OMFMob, pMob);
			gpGlobals->pLogger->Log( kLogInfo, "Appending MOB to file...\n" );
			rc = pHeader->AddMob(pMob);
			gpGlobals->nNumAAFMobs++;
		}
	}

	OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, OMFMobIter);
	// Now that we have read all the metadata we can get on with
	// the actual essence (media) data

	OMFError = OMF2::omfsGetHeadObject( OMFFileHdl, &OMFHeader );
	if (OMF2::kOmfRev2x == OMFFileRev)
	{
		OMFPropertyID = OMF2::OMHEADMediaData;
		numMedia = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMFPropertyID); 
	}
	else
	{
		OMFPropertyID = OMF2::OMMediaData;
		numMedia = OMF2::omfsLengthObjIndex(OMFFileHdl, OMFHeader, OMFPropertyID);
	}

	gpGlobals->pLogger->Log( kLogInfo, "Found: %ld Essence data objects\n", numMedia);
	for (int k = 1;k <= numMedia; k++)
	{
		OMF2::omfObjIndexElement_t	objIndex;
		OMF2::omfUID_t				tmpMediaID;

		if (OMF2::kOmfRev2x == OMFFileRev)
		{
			OMFError = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMFPropertyID, &OMFObject, k);
			tmpMediaID.prefix = 0;
			tmpMediaID.major = 0;
			tmpMediaID.minor = 0;
		}
		else
		{
			OMFError = OMF2::omfsGetNthObjIndex(OMFFileHdl, OMFHeader, OMFPropertyID, &objIndex, k);
			OMFObject = objIndex.Mob;
			tmpMediaID = objIndex.ID;

		}
		// Process the given Class Dictionary object.
		ConvertOMFMediaDataObject(OMFObject, tmpMediaID);
	}

	return;
}
// ============================================================================
// ConvertOMFHeader
//
//			This function is invoked by the main module and is responsible for
//			reading an OMF header, copy the relevant information to the aaf 
//			header, and convert all OMF dictionary and data definitions into 
//			the AAF Class dictionary.
//
//			
// Returns: None. Raises an exception if Header object is not converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFHeader( void )
{
	AAFCheck rc;
	OMFCheck OMFError;
	OMF2::omfObject_t		OMFHeader, OMFObject;
	aafInt32				numDefs, numEntries;

	gpGlobals->pLogger->Log(kLogInfo, "Processing OMF Header...\n" );
	OMFError = OMF2::omfsGetHeadObject( OMFFileHdl, &OMFHeader );
	IncIndentLevel();
	//From the OMF header we will extract all definition Objects and Class Dictionary
	// First we process the Data definitions
	numDefs = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADDefinitionObjects);
	gpGlobals->pLogger->Log( kLogInfo, "%sFound: %ld Data Definitions\n", 
	gpGlobals->indentLeader, numDefs);
	for (int i = 1;i <= numDefs;i++)
	{
		OMFError = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADDefinitionObjects, &OMFObject, i);
		// Process the given Data Definition object.
		 ConvertOMFDataDefinitionObject(OMFObject);
	}

	// Now we process the class dictionary
	if (OMF2::kOmfRev2x == OMFFileRev)
	{
		numEntries = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADClassDictionary);
	}
	else
	{
		numEntries = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMClassDictionary);
	}

	gpGlobals->pLogger->Log( kLogInfo, "Found: %ld Class Definitions\n", numEntries);
	for (int j = 1;j <= numEntries; j++)
	{
		OMFError = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADClassDictionary, &OMFObject, j);
		// Process the given Class Dictionary object.
		ConvertOMFClassDictionaryObject(OMFObject);
	}
	gpGlobals->pLogger->Log( kLogInfo, "Converted OMF Header values to AAF\n");
	DecIndentLevel();
	return;
}
// ============================================================================
// ConvertOMFDataDefinitionObject
//
//			This function is called by the ConvertOMFHeader module for each 
//			Data definition found in the header, all relevant data from the
//			OMF DDEF is extracted and converted into the AAF Class dictionary.
//			
// Returns: AAFRESULT_SUCCESS if Header object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFDataDefinitionObject( OMF2::omfObject_t obj )
{
	IncIndentLevel();

	OMF2::omfProperty_t		OMFPropertyID;
	if (OMF2::kOmfRev2x == OMFFileRev)
	{
		OMFPropertyID = OMF2::OMOOBJObjClass;
	}
	else
	{
		OMFPropertyID = OMF2::OMObjID;
	}

	OMF2::omfClassID_t		objClass;
	OMFCheck OMFError = OMF2::omfsReadClassID(OMFFileHdl, obj, OMFPropertyID, objClass);
	char id[ 5 ];
	strncpy(id, objClass, 4);
	id[4] = '\0';
	gpGlobals->pLogger->Log( kLogInfo, "%sProcessing: %s Data Definition\n", gpGlobals->indentLeader, id );

//	if (strcmp(id, "EDEF") == 0)
//	{
//		IAAFOperationDef*	pEffectDef = NULL;
//		rc = ConvertOMFEffectDefinition(obj, &pEffectDef);
//		pEffectDef->Release();
//	}

	DecIndentLevel();
	return;
}
// ============================================================================
// ConvertOMFClassDictionaryObject
//
//			This function is called by the ConvertOMFHeader module for each 
//			Class Dictionary object found in the header, all relevant data from the
//			OMF Class dictionary is extracted and converted into the AAF Class
//			dictionary.
//			
// Returns: AAFRESULT_SUCCESS if Header object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFClassDictionaryObject( OMF2::omfObject_t obj )
{
	IncIndentLevel();

	OMF2::omfProperty_t		OMFPropertyID;
	if (OMF2::kOmfRev2x == OMFFileRev)
	{
		OMFPropertyID = OMF2::OMCLSDClass;
	}
	else
	{
		OMFPropertyID = OMF2::OMCLSDClassID;
	}

	char id[5];
	memset(id, 0, sizeof(id));
	OMFCheck rc = OMF2::omfsReadClassID(OMFFileHdl, obj, OMFPropertyID, id);
	gpGlobals->pLogger->Log( kLogInfo, "%sProcessing: %s Class Definition\n", gpGlobals->indentLeader, id);

	DecIndentLevel();
	return;
}

// ============================================================================
// ConvertOMFMediaDataObject
//
//			This function is called by the ConvertOMFHeader module for each 
//			media data object found in the header
//			
// Returns: AAFRESULT_SUCCESS if Header object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFMediaDataObject( OMF2::omfObject_t obj, OMF2::omfUID_t inMediaID )
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	OMF2::omfUID_t			mediaID;
	OMF2::omfProperty_t		idProperty;
	OMF2::omfProperty_t		OMFPropertyID;
	OMF2::omfDDefObj_t		datakind;
	OMF2::omfUniqueName_t	propName;

	IAAFEssenceData*		pEssenceData;
	AutoRelease<IAAFEssenceData> pEssence;
	IAAFMob*				pMob;
	AutoRelease<IAAFMob>	pmob;
	IAAFSourceMob*			pSourceMob;
	AutoRelease<IAAFSourceMob> pSource;
	aafBool					bConvertMedia = kAAFFalse;

	CAAFBuiltinDefs defs (pDictionary);

	IncIndentLevel();
	if (OMF2::kOmfRev2x == OMFFileRev)
	{
		OMFPropertyID = OMF2::OMOOBJObjClass;
		OMFError = OMF2::omfsReadUID(OMFFileHdl, obj, OMF2::OMMDATMobID, &mediaID);
	}
	else
	{
		OMFPropertyID = OMF2::OMObjID;
		mediaID = inMediaID;
	}

	aafMobID_t	mobID;
	rc = aafMobIDFromMajorMinor(42, mediaID.major, mediaID.minor, 4, &mobID);
	rc = pHeader->LookupMob(mobID, &pMob);
	pmob = pMob;
	rc = pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob);
	pSource = pSourceMob;
	char	id[5];
	memset(id, 0, sizeof(id));
	OMFError = OMF2::omfsReadClassID(OMFFileHdl, obj, OMFPropertyID, id);
	gpGlobals->pLogger->Log( kLogInfo, "%sProcessing: %s Media Data\n", gpGlobals->indentLeader, id);
	strcpy(propName, "OMFI:");
	if (strncmp(id, "TIFF", 4) == 0)
	{
		// handle TIFF media data
		IAAFEssenceData*	pTIFFData = NULL;
		rc = defs.cdEssenceData()->
		  CreateInstance(IID_IAAFEssenceData,
						 (IUnknown **)&pTIFFData);
		AutoRelease<IAAFEssenceData> tiffdata(pTIFFData);
		rc = pTIFFData->QueryInterface(IID_IAAFEssenceData, (void **)&pEssenceData);
		pEssence = pEssenceData;
		rc = pEssenceData->SetFileMob(pSourceMob);
		rc = pHeader->AddEssenceData(pEssenceData);

		if (OMF2::kOmfRev2x == OMFFileRev)
		{
			OMF2::omfErr_t err;
			idProperty = OMF2::OMTIFFData;
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, &err );
			OMFError = err;
		}
		else
		{
			strncat(propName, id, (size_t)4);
			strcat(propName, ":Data");
		}

		bConvertMedia = kAAFTrue;
	}
	else if (strncmp(id, "AIFC", 4) == 0)
	{
		// Handle Audio (Wave data)
		IAAFEssenceData*	pAIFCData = NULL;
		rc = defs.cdEssenceData()->
		  CreateInstance(IID_IAAFEssenceData,
						 (IUnknown **)&pAIFCData);
		AutoRelease<IAAFEssenceData> aifcdata(pAIFCData);
		rc = pAIFCData->QueryInterface(IID_IAAFEssenceData, (void **)&pEssenceData);
		pEssence = pEssenceData;
		rc = pEssenceData->SetFileMob(pSourceMob);
		rc = pHeader->AddEssenceData(pEssenceData);
		if (OMF2::kOmfRev2x == OMFFileRev)
		{
			OMF2::omfErr_t err;
			idProperty = OMF2::OMAIFCData;
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, &err );
			OMFError = err;
		}
		else
		{
			strncat(propName, id, (size_t)4);
			strcat(propName, ":Data");
		}
		bConvertMedia = kAAFTrue;
	}
	else if (strncmp(id, "WAVE", 4) == 0 )
	{
		// Handle Audio (Wave data)
		IAAFEssenceData*	pWAVEData = NULL;
		rc = defs.cdEssenceData()->
		  CreateInstance(IID_IAAFEssenceData,
						 (IUnknown **)&pWAVEData);
		AutoRelease<IAAFEssenceData> wavedata(pWAVEData);
		rc = pWAVEData->QueryInterface(IID_IAAFEssenceData, (void **)&pEssenceData);
		pEssence = pEssenceData;
		rc = pEssenceData->SetFileMob(pSourceMob);
		rc = pHeader->AddEssenceData(pEssenceData);
		if (OMF2::kOmfRev2x == OMFFileRev)
		{
			OMF2::omfErr_t err;
			idProperty = OMF2::OMWAVEData;
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, &err);
			OMFError = err;
		}
		else
		{
			strncat(propName, id, (size_t)4);
			strcat(propName, ":Data");
		}
		bConvertMedia = kAAFTrue;
	}
	else if (strncmp(id, "JPEG", 4) == 0)
	{
		IAAFEssenceData*	pJPEGData = NULL;

		rc = defs.cdEssenceData()->
		  CreateInstance(IID_IAAFEssenceData,
						 (IUnknown **)&pJPEGData);
		AutoRelease<IAAFEssenceData> jpegdata(pJPEGData);
		rc = pJPEGData->QueryInterface(IID_IAAFEssenceData, (void **)&pEssenceData);
		pEssence = pEssenceData;
		rc = pEssenceData->SetFileMob(pSourceMob);
		rc = pHeader->AddEssenceData(pEssenceData);
		if (OMF2::kOmfRev2x == OMFFileRev)
		{
			OMF2::omfErr_t err;
			idProperty = OMF2::OMIDATImageData;
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, &err );
			OMFError = err;
		}
		else
		{
			strcat(propName, "IDAT");
			strcat(propName, ":Data");
		}
		bConvertMedia = kAAFTrue;
	}
	else
	{
		// rest of media (essence) NOT implemented)
		IncIndentLevel();
		gpGlobals->pLogger->Log( kLogWarn,
			"%sThis conversion is not implemented yet !!\n", gpGlobals->indentLeader);
		DecIndentLevel();
	}
	if (bConvertMedia)
	{
		void*				pBuffer = NULL;
		OMF2::omfPosition_t	OMFOffset;

		aafUInt32			numBytes;
		aafUInt32			nBlockSize;
		aafUInt32			numBytesRead;
		aafUInt32			numBytesWritten;
		aafBool				bMore = kAAFFalse;
		

		// find out how big the data is 
		if (OMF2::kOmfRev2x == OMFFileRev)
			numBytes = (aafUInt32)OMF2::omfsLengthDataValue(OMFFileHdl, obj, idProperty);
		else
		{
			OMF2::omfUniqueName_t	tmpPropName;
			OMF2::omfIterHdl_t		propIter = NULL;
			OMF2::omfProperty_t		tmpProp;
			OMF2::omfType_t			tmpType;

			OMFError = OMF2::omfiIteratorAlloc(OMFFileHdl, &propIter);
			OMF2::omfErr_t err = OMF2::omfiGetNextProperty(propIter, obj, &tmpProp, &tmpType);		
			while (OMF2::OM_ERR_NONE == err && tmpProp)
			{
				OMFError = OMF2::omfiGetPropertyName(OMFFileHdl, tmpProp, OMUNIQUENAME_SIZE, tmpPropName);
				if (strcmp(tmpPropName, propName) == 0)
				{
					idProperty = tmpProp;
					break;
				}
				err = OMF2::omfiGetNextProperty(propIter, obj, &tmpProp, &tmpType);		
			}
			OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, propIter);

			numBytes = (aafUInt32)OMF2::omfsLengthVarLenBytes(OMFFileHdl, obj, idProperty);
		}
		if (numBytes > 0)
		{
			if (numBytes > (2 * 1048576))
			{
				nBlockSize = 2 * 1048576;		// only allocate 2 Meg
				bMore = kAAFTrue;			// you going to need more than one read/write
			}
			else
			{
				nBlockSize = numBytes;
			}
			std::auto_ptr<char> buf( new char[nBlockSize] ); // For auto delete..
			pBuffer = buf.get();
			OMFOffset = 0;
			do 
			{
				if( (numBytes - OMFOffset) < nBlockSize )
				{
					nBlockSize = (numBytes - OMFOffset);
				}
				if (OMF2::kOmfRev2x == OMFFileRev)
					OMFError = OMF2::omfsReadDataValue( OMFFileHdl, 
									 					obj,
														idProperty,
														datakind,
														(OMF2::omfDataValue_t)pBuffer,
														OMFOffset,
														nBlockSize,
														&numBytesRead);
				else
					OMFError = OMF2::omfsReadVarLenBytes(OMFFileHdl,
														 obj,
														 idProperty,
														 OMFOffset,
														 nBlockSize,
														 pBuffer,
														 &numBytesRead);

				// write the data
				rc = pEssenceData->SetPosition((aafPosition_t) OMFOffset);
				rc = pEssenceData->Write(numBytesRead, (aafDataBuffer_t)pBuffer, &numBytesWritten);

				// calculate next offset
				OMFOffset += numBytesRead;

			}while (numBytes > OMFOffset );

			Assert( numBytes == OMFOffset );
			// auto_ptr will Free the allocated buffer here when it goes
			// out of scope.
		}
	}

	DecIndentLevel();
	return;
}

// ============================================================================
// ConvertOMFDatakind
//
//			This function converts an OMF datakind into an AAF datadef. 
//			
// Returns: An Exception if datakind is not converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFDatakind( OMF2::omfDDefObj_t datakind, 
								   aafUID_t * pDatakind)
{
	OMF2::omfUniqueName_t	datakindName;
	OMFCheck rc = OMF2::omfiDatakindGetName(OMFFileHdl, datakind, 64, datakindName);

	if (strncmp("omfi:data:Picture", datakindName, strlen(datakindName))== 0)
		*pDatakind = DDEF_Picture;
	else if (strncmp("omfi:data:Sound", datakindName, strlen(datakindName)) == 0)
		*pDatakind = DDEF_Sound;
	else if (strncmp("omfi:data:StereoSound", datakindName, strlen(datakindName)) == 0)
		*pDatakind = DDEF_Sound;
	else if(strncmp("omfi:data:Timecode", datakindName, strlen(datakindName)) == 0)
		*pDatakind = DDEF_Timecode;
	else if(strncmp("omfi:data:Edgecode", datakindName, strlen(datakindName)) == 0)
		*pDatakind = DDEF_Edgecode;
	else if(strncmp("omfi:data:PictureWithMatte", datakindName, strlen(datakindName)) == 0)
		*pDatakind = DDEF_PictureWithMatte;
	else
	{
		gpGlobals->pLogger->Log( kLogWarn, "Unknown DataDef: %s Found in sequence\n", datakindName);
		*pDatakind = DDEF_Unknown;
	}

	return;
}

void Omf2Aaf::ConvertOMFDataDef			// Used for essence types audio, video, smell, etc...
  (OMF2::omfDDefObj_t datakind, 
   IAAFDataDef ** ppDataDef)
{
  aafUID_t defUid;
  ConvertOMFDatakind (datakind, &defUid);
  AAFCheck rc = pDictionary->LookupDataDef (defUid, ppDataDef);
}

void Omf2Aaf::ConvertOMFDataDefType		// Used for parameter types Integer, Rational, etc...
  (OMF2::omfDDefObj_t datakind, 
   IAAFTypeDef ** ppTypeDef)
{
	IAAFTypeDef				*pTypeDef;
	OMF2::omfUniqueName_t	datakindName;
	OMFCheck				OMFError;

	OMFError = OMF2::omfiDatakindGetName(OMFFileHdl, datakind, 64, datakindName);
	if (strcmp(datakindName, "omfi:data:Rational") == 0)
	{
		pDictionary->LookupTypeDef(kAAFTypeID_Rational, &pTypeDef);
	}
	// Else error!!!

	*ppTypeDef = pTypeDef;
}
// ============================================================================
// ConvertOMFMOBObject
//
//			This function is converts all the mob basic data (name, MobId, etc.) 
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFMOBObject( OMF2::omfObject_t obj, IAAFMob* pMob )
{

	IncIndentLevel();

	// Set mob Name
	char					sMobName[64];
	OMF2::omfUID_t			OMFMobID;
	OMFCheck				OMFError;

	OMF2::omfErr_t	testErr;
	testErr = OMF2::omfiMobGetInfo(OMFFileHdl, obj, &OMFMobID, sizeof(sMobName), sMobName, NULL, NULL);
	char *src = (OMF2::OM_ERR_NONE == testErr) ? sMobName : "<not named>";
	std::auto_ptr<wchar_t> pwmobname( new wchar_t[strlen(src)+1] );
	aafWChar*	pwMobName = pwmobname.get();
	mbstowcs(pwMobName, src, strlen(src)+1);
	AAFCheck aafCheck = pMob->SetName(pwMobName);
	gpGlobals->pLogger->Log( kLogInfo, "%sMob Name: %s\n", gpGlobals->indentLeader, src );

	// Convert OMF MobID into AAF AUID and set mob id
	aafMobID_t				AAFMobUID;
	aafCheck = aafMobIDFromMajorMinor(42, OMFMobID.major, OMFMobID.minor, 4, &AAFMobUID);
	aafCheck = pMob->SetMobID(AAFMobUID);

	// Set comments
	aafInt32				numComments;
	OMFCheck omfCheck = OMF2::omfiMobGetNumComments(OMFFileHdl, obj, &numComments);
	if (numComments > 0)
	{
		// Allocate the iterator
		OMF2::omfIterHdl_t		OMFIterator;
		omfCheck = OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
		aafInt32	times;
		char		sCommentName[64];
		char		sCommentValue[256];
		for (times = 0; times < numComments; times++)
		{
			omfCheck = OMF2::omfiMobGetNextComment(OMFIterator, obj, sizeof(sCommentName), sCommentName, sizeof(sCommentValue), sCommentValue);
			std::auto_ptr<wchar_t>pwcomment( new wchar_t[strlen(sCommentName)+1] );
			aafWChar *pwCommentName = pwcomment.get();
			mbstowcs(pwCommentName, sCommentName, strlen(sCommentName)+1);
			std::auto_ptr<wchar_t>pwcommentval( new wchar_t[strlen(sCommentValue)+1] );
			aafWChar* pwCommentValue = pwcommentval.get();
			mbstowcs(pwCommentValue, sCommentValue, strlen(sCommentValue)+1);
			aafCheck = pMob->AppendComment(pwCommentName, pwCommentValue);
		}

		// Release the iterator
		omfCheck = OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
	}

	((ExtendedOmf2Aaf*)this)->FinishUpMob(obj, pMob);
	DecIndentLevel();

	// auto_ptrs will delete [] allocated pointers here...
	// AutoRelease's will release resources here.
	return;
}

// ============================================================================
// ConvertOMFCompositionObject
//
//			This function extracts all the properties of an OMF Composition MOB,
//			Creates an AAF Composition mob object, sets its properties and 
//			appends it to the AAF header. 
//
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFCompositionObject(OMF2::omfObject_t obj, 
											IAAFCompositionMob* pCompMob)
{
	OMF2::omfDefaultFade_t	OMFDefaultFade;
	aafDefaultFade_t		AAFDefaultFade;
	IAAFObject				*pElement = NULL;
	AAFCheck				hr;

	// get Composition mob information
	OMFCheck				OMFError;

	OMF2::omfErr_t			testErr;
	testErr = OMF2::omfiMobGetDefaultFade(OMFFileHdl, obj, &OMFDefaultFade);
	if (OMF2::OM_ERR_NONE == testErr && OMFDefaultFade.valid)
	{
		gpGlobals->nNumOMFProperties++;
		AAFDefaultFade.fadeLength   = OMFDefaultFade.fadeLength;
		switch(OMFDefaultFade.fadeType)
		{
			case OMF2::kFadeNone: 
				AAFDefaultFade.fadeType = kAAFFadeNone;
				break;
			case OMF2::kFadeLinearAmp:
				AAFDefaultFade.fadeType = kAAFFadeLinearAmp;
				break;
			case OMF2::kFadeLinearPower:
				AAFDefaultFade.fadeType = kAAFFadeLinearPower;
				break;
		}
		AAFDefaultFade.fadeEditUnit.numerator = OMFDefaultFade.fadeEditUnit.numerator;
		AAFDefaultFade.fadeEditUnit.denominator = OMFDefaultFade.fadeEditUnit.denominator;
	}
	else
	{
		AAFDefaultFade.fadeType = kAAFFadeNone;
		AAFDefaultFade.fadeEditUnit.numerator = 0;
		AAFDefaultFade.fadeEditUnit.denominator = 1;
		AAFDefaultFade.fadeLength = 0;
		AAFDefaultFade.valid = kAAFFalse;
	}

	// Set default fade values
	if (AAFDefaultFade.valid)
	{
		AAFCheck rc = pCompMob->SetDefaultFade(AAFDefaultFade.fadeLength,
								 AAFDefaultFade.fadeType,
								 AAFDefaultFade.fadeEditUnit);
		gpGlobals->nNumAAFProperties++;
	}
	aafUID_t	classID = kAAFClassID_CompositionMob;

	hr = pCompMob->QueryInterface(IID_IAAFObject, (void **)&pElement);
	ConvertObjectProps(obj, classID, pElement);

	gpGlobals->pLogger->Log( kLogInfo, "Converted OMF Composition MOB to AAF\n");

	return;
}
// ============================================================================
// TraverseOMFMob
//
//			This function converts all the mob slots in the given OMF mob object 
//			
// Returns: AAFRESULT_SUCCESS if succesfully
//
// ============================================================================
void Omf2Aaf::TraverseOMFMob( OMF2::omfObject_t obj, IAAFMob* pMob )
{
	CAAFBuiltinDefs defs (pDictionary);

	aafInt32				numSlots;
	OMFCheck				OMFError;
	OMF2::omfErr_t			testErr;

	if (OMF2::OM_ERR_NONE != OMF2::omfiMobGetNumSlots(OMFFileHdl, obj, &numSlots))
		numSlots = 0;

	aafInt32				numTracks;
	if (OMF2::OM_ERR_NONE != OMF2::omfiMobGetNumTracks(OMFFileHdl, obj, &numTracks))
		numTracks = 0;

	IncIndentLevel();
	gpGlobals->pLogger->Log( kLogInfo,"%sFound: %ld sub tracks\n", gpGlobals->indentLeader, numSlots);

	OMF2::omfIterHdl_t		OMFIterator;
	OMFCheck omfCheck = OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
	for (aafInt32 times = 0; times < numSlots; times++)
	{
		
		OMF2::omfPosition_t		OMFOrigin;
		OMF2::omfRational_t		OMFeditRate;
		OMF2::omfTrackID_t		OMFTrackID;
		OMF2::omfMSlotObj_t		OMFSlot;
		
		OMFError = OMF2::omfiMobGetNextSlot(OMFIterator, obj, NULL, &OMFSlot);
		
		char					sTrackName[32];
		OMF2::omfMSlotObj_t		OMFSegment;
		OMFError = OMF2::omfiMobSlotGetInfo(OMFFileHdl, OMFSlot, &OMFeditRate, &OMFSegment);
		if (OMF2::omfiMobSlotIsTrack(OMFFileHdl, OMFSlot, &testErr))
		{
			aafUInt32				physicalTrackNumber = 0;			
			OMFError = OMF2::omfiTrackGetInfo(OMFFileHdl, obj, OMFSlot, &OMFeditRate, sizeof(sTrackName),
				sTrackName, &OMFOrigin, &OMFTrackID, &OMFSegment);
			OMFError = OMF2::omfiTrackGetPhysicalNum(OMFFileHdl, OMFSlot, (OMF2::omfUInt32 *)&physicalTrackNumber);
			
			IAAFComponent*			pComponent = NULL;
			ProcessOMFComponent(OMFSegment, &pComponent);
			if( pComponent )
			{
				AutoRelease<IAAFComponent> pcomp( pComponent );
				IAAFSegment* pSegment;
				AAFCheck rc = pComponent->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
				AutoRelease<IAAFSegment> pseg( pSegment );
				IncIndentLevel();
				
				std::auto_ptr <wchar_t> pwtrack( new wchar_t[strlen(sTrackName)+1] );
				aafWChar* pwTrackName = pwtrack.get();
				mbstowcs(pwTrackName, sTrackName, strlen(sTrackName)+1);
				
				// OMF ONLY created timeline mob slots 
				// so that is what we going to do here
				IAAFTimelineMobSlot*	pTimelineMobSlot;
				rc = defs.cdTimelineMobSlot()->
					CreateInstance(IID_IAAFTimelineMobSlot,
					(IUnknown **)&pTimelineMobSlot);
				AutoRelease<IAAFTimelineMobSlot> ptimeline( pTimelineMobSlot );
				
				IAAFMobSlot*	pMobSlot;
				rc = pTimelineMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot);
				AutoRelease<IAAFMobSlot> pmobslot( pMobSlot );
				
				aafRational_t aafRat;
				memcpy (&aafRat, &OMFeditRate, sizeof (aafRational_t));
				rc = pTimelineMobSlot->SetEditRate(aafRat);
				rc = pTimelineMobSlot->SetOrigin((aafPosition_t)OMFOrigin);
				rc = pMobSlot->SetName(pwTrackName);
				rc = pMobSlot->SetSlotID((aafSlotID_t)OMFTrackID);
				rc = pMobSlot->SetSegment(pSegment);
				rc = pMobSlot->SetPhysicalNum(physicalTrackNumber);
				rc = pMob->AppendSlot( pMobSlot );
				gpGlobals->pLogger->Log( kLogInfo, "%sConverted SlotID: %d, Name: %s\n",gpGlobals->indentLeader, (int)OMFTrackID, sTrackName);
				DecIndentLevel();
			}
		}
		// At this point we have a consistent MOB - Lets save it
		// pFile->Save();  Commented out until dirty bit is used by OM !!!
	}
	
	omfCheck = OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
	DecIndentLevel();

	return;
}
// ============================================================================
// ConvertOMFSelector
//
//			This function converts an OMF Selector object and all the objects it
// contains or references.
//			
// Returns: AAFRESULT_SUCCESS if succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFSelector( OMF2::omfObject_t selector, IAAFSelector* pSelector )
{
	OMF2::omfLength_t		OMFLength;
	OMF2::omfDDefObj_t		OMFDatakind;
	OMF2::omfSegObj_t		OMFSelected;
	OMFCheck OMFError = OMF2::omfiSelectorGetInfo( OMFFileHdl, 
									selector,
									&OMFDatakind,
									&OMFLength,
									&OMFSelected);

	// aafUID_t				datadef ;
	IAAFDataDefSP pDataDef;
	ConvertOMFDataDef(OMFDatakind, &pDataDef);
	IAAFComponent*			pComponent;
	AAFCheck rc = pSelector->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	AutoRelease< IAAFComponent > pcomp( pComponent );
	rc = pComponent->SetDataDef(pDataDef);
	rc = pComponent->SetLength(OMFLength);
	gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Selector object of length = %ld\n", gpGlobals->indentLeader, (int)OMFLength);
		
	OMF2::omfNumSlots_t		numAlternates;
	if (OMF2::kOmfRev2x == OMFFileRev)
	{
		IAAFComponent*			pComponent;
		ProcessOMFComponent(OMFSelected, &pComponent);
		if( pComponent )
		{
			AutoRelease< IAAFComponent > pcomp( pComponent );
			IAAFSegment*			pSegment;
			rc = pComponent->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
			AutoRelease< IAAFSegment > pseg( pSegment );
			rc = pSelector->SetSelectedSegment(pSegment);
			OMFCheck OMFError = OMF2::omfiSelectorGetNumAltSlots(OMFFileHdl,
											  selector,
											  &numAlternates);
			if (numAlternates > 0)
			{
				OMF2::omfIterHdl_t		OMFIterator;
				OMFError = OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
				for (int i = 0; i <(int)numAlternates;i++)
				{
					OMF2::omfSegObj_t		OMFAltSelected;
					OMFError = OMF2::omfiSelectorGetNextAltSlot(OMFIterator,
													  selector,
													  NULL, 
													  &OMFAltSelected);
					IAAFComponent*			pAltComponent;
					ProcessOMFComponent(OMFAltSelected, &pAltComponent);
					if( pAltComponent )
					{
						AutoRelease<IAAFComponent> paltcomp( pAltComponent );

						IAAFSegment*			pAltSegment;
						rc = pAltComponent->QueryInterface(IID_IAAFSegment, (void **)&pAltSegment);
						AutoRelease<IAAFSegment> paltseg( pAltSegment );

						rc = pSelector->AppendAlternateSegment(pAltSegment);
					}
				}
				OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
			}
		}
	}
	else
	{
		aafBool					bDoneSelected = kAAFFalse;
		if (OMFSelected)
		{
			IAAFComponent*			pComponent;
			ProcessOMFComponent(OMFSelected, &pComponent);
			if( pComponent )
			{
				AutoRelease< IAAFComponent > pcomp( pComponent );
				IAAFSegment*			pSegment;
				rc = pComponent->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
				AutoRelease<IAAFSegment> pseg( pSegment );
				rc = pSelector->SetSelectedSegment(pSegment);
				bDoneSelected = kAAFTrue;
			}
		}

		OMFError = OMF2::omfiSelectorGetNumAltSlots(OMFFileHdl,
											  selector,
											  &numAlternates);
		if (numAlternates > 0)
		{

			OMF2::omfIterHdl_t		OMFIterator;
			OMFError = OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
			for (int i = 0; i <(int)numAlternates;i++)
			{
				OMF2::omfSegObj_t		OMFAltSelected;
				OMFError = OMF2::omfiSelectorGetNextAltSlot(OMFIterator,
														  selector,
														  NULL, 
														  &OMFAltSelected);
				IAAFComponent*			pAltComponent;
				ProcessOMFComponent(OMFAltSelected, &pAltComponent);
				if( pAltComponent )
				{
					AutoRelease<IAAFComponent> paltcomp( pAltComponent );
					if (i == 0 && !bDoneSelected)
					{
						IAAFSegment*	pSegment;
						rc = pAltComponent->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
						AutoRelease<IAAFSegment> pseg( pSegment );
						rc = pSelector->SetSelectedSegment(pSegment);
					}
					else
					{
						IAAFSegment*	pAltSegment;
						rc = pAltComponent->QueryInterface(IID_IAAFSegment, (void **)&pAltSegment);
						AutoRelease<IAAFSegment> paltseg( pAltSegment );
						rc = pSelector->AppendAlternateSegment(pAltSegment);
					}
				}
			}
			OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
		}
	}

	return;
}
// ============================================================================
// ProcessOMFComponent
//
//			This function will :
//				1. Identify type of OMF Component
//				2. Create the equivalent AAF object
//				3. Convert the OMF datakind to AAF datadef of the object
//				4. Traverse the component (if needed) into its objects
//				5. return the AAF segment 
//
//	INPUTS:		OMFSegment	OMF object to be processed.
//				pSegment	Pointer to a AAF Segment Interface pointer
//
//	OUTPUTS:	pSegment	new AAF Segment	 or NULL if zero lenght filler found.
//			
// Returns: An exception on error.
//
// ============================================================================
void Omf2Aaf::ProcessOMFComponent(OMF2::omfObject_t OMFSegment, IAAFComponent** ppComponent)
{
	AAFCheck				rc;
	OMFCheck				check;
	OMFCheck				OMFError;
	OMF2::omfErr_t			testErr;

	OMF2::omfLength_t		OMFLength;
	OMF2::omfDDefObj_t	OMFDatakind;
	IAAFDataDef * pDataDef = 0;

	*ppComponent = NULL;

	CAAFBuiltinDefs defs (pDictionary);

	// First get sequence information
	IncIndentLevel();
	if (OMF2::omfiIsASequence(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFSequence*	pSequence;
		rc = defs.cdSequence()->
		  CreateInstance(IID_IAAFSequence,
						 (IUnknown **)&pSequence);
		AutoRelease<IAAFSequence> pseq( pSequence );
		rc = pSequence->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Sequence\n", gpGlobals->indentLeader);
		ConvertOMFSequence(OMFSegment, pSequence);
		TraverseOMFSequence(OMFSegment, pSequence);
	}

	else if (OMF2::omfiIsASourceClip(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFSourceClip*			pSourceClip;
		rc = defs.cdSourceClip()->
		  CreateInstance(IID_IAAFSourceClip,
						 (IUnknown **)&pSourceClip);
		AutoRelease<IAAFSourceClip> psrc( pSourceClip );
		ConvertOMFSourceClip(OMFSegment, pSourceClip);
		rc = pSourceClip->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		gpGlobals->pLogger->Log( kLogInfo, "%sProcessing SourceClip\n", gpGlobals->indentLeader);
	}
	else if (OMF2::omfiIsATimecodeClip(OMFFileHdl, OMFSegment, &testErr) )
	{
		// Get Timecode information
		aafTimecode_t			timecode;
		OMF2::omfTimecode_t		OMFTimecode;
		check = OMF2::omfiTimecodeGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength, &OMFTimecode);
		timecode.startFrame = OMFTimecode.startFrame;
		timecode.drop = (aafDropType_t)OMFTimecode.drop;
		timecode.fps  = OMFTimecode.fps;
		gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Timecode\n", gpGlobals->indentLeader);
		IncIndentLevel();
		gpGlobals->pLogger->Log( kLogInfo, "%slength\t\t: %ld\n", gpGlobals->indentLeader, (int)OMFLength);
		gpGlobals->pLogger->Log( kLogInfo, "%sstart Frame\t: %ld\n", gpGlobals->indentLeader, timecode.startFrame);
		gpGlobals->pLogger->Log( kLogInfo, "%sdrop\t\t: %s\n", gpGlobals->indentLeader, 
			timecode.drop == kAAFTrue ? "True" : "False" );
		gpGlobals->pLogger->Log( kLogInfo, "%sFrames/second\t: %ld\n", gpGlobals->indentLeader, timecode.fps);     
		DecIndentLevel();	
		
		IAAFTimecode*	pTimecode;
		rc = defs.cdTimecode()->
		  CreateInstance(IID_IAAFTimecode,
						 (IUnknown **)&pTimecode);
		AutoRelease<IAAFTimecode> ptime( pTimecode );
		rc = pTimecode->Initialize((aafLength_t)OMFLength, &timecode);
		rc = pTimecode->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (OMF2::omfiIsAnEdgecodeClip(OMFFileHdl, OMFSegment, &testErr) )
	{
		// Get edgecode data
		OMF2::omfEdgecode_t		OMFEdgecode;
		aafEdgecode_t			edgecode;
		check = OMF2::omfiEdgecodeGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength, &OMFEdgecode);
		edgecode.startFrame = OMFEdgecode.startFrame;
		edgecode.filmKind = (aafFilmType_t)OMFEdgecode.filmKind;
		edgecode.codeFormat = (aafEdgeType_t)OMFEdgecode.codeFormat;
		for (int ii=0;ii<sizeof(edgecode.header);ii++)
			edgecode.header[ii] = OMFEdgecode.header[ii];

		gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Edgecode\n ", gpGlobals->indentLeader);
		IncIndentLevel();
		gpGlobals->pLogger->Log( kLogInfo, "%slength\t\t: %ld\n", gpGlobals->indentLeader, (int)OMFLength);
		gpGlobals->pLogger->Log( kLogInfo, "%sstart Frame\t: %ld\n", gpGlobals->indentLeader, edgecode.startFrame);
		DecIndentLevel();				
		ConvertOMFDataDef(OMFDatakind, &pDataDef);

		IAAFEdgecode*	pEdgecode;
		rc = defs.cdEdgecode()->
		  CreateInstance(IID_IAAFEdgecode,
						 (IUnknown **)&pEdgecode);
		AutoRelease<IAAFEdgecode> pedge( pEdgecode );
		rc = pEdgecode->Initialize((aafLength_t)OMFLength, edgecode);
		rc = pEdgecode->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		IAAFComponent* pTempComp = *ppComponent;

		rc = pTempComp->SetDataDef(pDataDef);
	}
	else if (OMF2::omfiIsAFiller(OMFFileHdl, OMFSegment, &testErr) )
	{
		check = OMF2::omfiFillerGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength);
		if (OMFLength > 0 || gpGlobals->bConvertAllObjects)
		{
			ConvertOMFDataDef( OMFDatakind, &pDataDef);
			gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Filler of length: %ld\n ", gpGlobals->indentLeader, (int)OMFLength);
			IAAFFiller*		pFiller;
			rc = defs.cdFiller()->
			  CreateInstance(IID_IAAFFiller,
							 (IUnknown **) &pFiller);
			AutoRelease<IAAFFiller> pfill( pFiller );

			rc = pFiller->Initialize(pDataDef, (aafLength_t)OMFLength);

			rc = pFiller->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		}
		else
		{
			gpGlobals->pLogger->Log( kLogInfo, "%sZero length Filler ignored !!\n", gpGlobals->indentLeader);
		}
	}
	else if (OMF2::omfiIsAnEffect(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFOperationGroup*		pEffect;
		rc = defs.cdOperationGroup()->
		  CreateInstance(IID_IAAFOperationGroup,
						 (IUnknown **) &pEffect);
		AutoRelease<IAAFOperationGroup> peff( pEffect );
		ConvertOMFEffects(OMFSegment, pEffect);
		rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (OMF2::omfiIsATransition(OMFFileHdl, OMFSegment, &testErr) )
	{

		OMF2::omfPosition_t		OMFCutPoint;
		OMF2::omfEffObj_t		OMFEffect;
		check = OMF2::omfiTransitionGetInfo(OMFFileHdl, 
										 OMFSegment,
										 &OMFDatakind, 
										 &OMFLength, 
										 &OMFCutPoint,
										 &OMFEffect);
		gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Transition of length: %ld\n ", gpGlobals->indentLeader, (int)OMFLength);
		ConvertOMFDataDef( OMFDatakind, &pDataDef);

		if (OMF2::kOmfRev2x == OMFFileRev)
		{
			IAAFTransition*			pTransition;
			rc = defs.cdTransition()->
			  CreateInstance(IID_IAAFTransition,
							 (IUnknown **)&pTransition);
			AutoRelease<IAAFTransition> ptrans( pTransition );

			IAAFOperationGroup*		pEffect;
			rc = defs.cdOperationGroup()->
			  CreateInstance(IID_IAAFOperationGroup,
							 (IUnknown **) &pEffect);
			AutoRelease<IAAFOperationGroup> peff( pEffect );

			IncIndentLevel();
			ConvertOMFEffects(OMFEffect, pEffect);

			pTransition->Initialize(pDataDef, (aafLength_t)OMFLength, (aafPosition_t)OMFCutPoint, pEffect);
			rc = pTransition->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
			DecIndentLevel();
		}
		else
		{
			// NOTE - 1x Transitions are really effects so we process them
			// accordingly.
			OMF2::omfUniqueName_t	EffectID1x;

			testErr = OMF2::omfsReadString(OMFFileHdl, OMFSegment, OMF2::OMCPNTEffectID, EffectID1x, OMUNIQUENAME_SIZE);
			if (OMF2::OM_ERR_NONE == testErr)
			{
				if ( (strcmp(EffectID1x, "Blend:Dissolve") == 0) && 
					(OMF2::omfiIsSoundKind(	OMFFileHdl,	OMFDatakind, OMF2::kExactMatch, &testErr)))
				{
					// Create a Mono Audio Dissolve Effect
					gpGlobals->pLogger->Log( kLogInfo, "%sReplacing 1.x Transition with a Audio Dissolve Effect!\n ", gpGlobals->indentLeader);

					IAAFOperationGroup*		pEffect;
					rc = defs.cdOperationGroup()->
					  CreateInstance(IID_IAAFOperationGroup,
									 (IUnknown **) &pEffect);
					AutoRelease< IAAFOperationGroup > peff( pEffect );

					IAAFOperationDef*		pEffectDef;
					GetAAFOperationDefinition("omfi::effectSimpleMonoAudioDissolve", NULL, "Simple Mono Audio Dissolve", "Combines two mono audio streams",
									-1, kAAFFalse, 2, DDEF_Sound, &pEffectDef);
					AutoRelease<IAAFOperationDef> peffdef( pEffectDef );

					IAAFParameterDef*		pParameterDef;
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, NULL, 
												L"Level", 
												L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
												L" ",
												&pParameterDef);
					AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );

					rc = pEffectDef->AddParameterDef(pParameterDef);
					rc = pEffect->Initialize(pDataDef, (aafLength_t)OMFLength, pEffectDef);
					rc = pEffect->SetBypassOverride(-1);
					rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
				}
				else if ( (strcmp(EffectID1x, "Blend:Dissolve") == 0) && 
						  (OMF2::omfiIsPictureWithMatteKind( OMFFileHdl, OMFDatakind, OMF2::kConvertTo, &testErr)))
				{
					// Create a Video Dissolve Effect
					gpGlobals->pLogger->Log( kLogInfo,"%sReplacing 1.x Transition with a Video Dissolve Effect!\n ", gpGlobals->indentLeader);
					IAAFOperationGroup*	pEffect;
					rc = defs.cdOperationGroup()->
					  CreateInstance(IID_IAAFOperationGroup,
									 (IUnknown **) &pEffect);
					AutoRelease< IAAFOperationGroup > peff( pEffect );

					IAAFOperationDef* pEffectDef;
					GetAAFOperationDefinition("omfi::effectSimpleVideoDissolve", NULL, 
									"Simple Video Dissolve", "Combines two video streams",
									-1, kAAFFalse, 2, DDEF_PictureWithMatte, &pEffectDef);
					AutoRelease<IAAFOperationDef> peffdef( pEffectDef );

					IAAFParameterDef* pParameterDef;
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, NULL, 
												L"Level", 
												L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
												L" ",
												&pParameterDef);
					AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );

					rc = pEffectDef->AddParameterDef(pParameterDef);
					rc = pEffect->Initialize(pDataDef, (aafLength_t)OMFLength, pEffectDef);
					rc = pEffect->SetBypassOverride(-1);
					rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
				}
				else if ( strncmp(EffectID1x, "Wipe:SMPTE:", 11) == 0)
				{
					// Get SMPTE Vide Wipe Info and 
					// Create a SMPTE Video Wipe Effect !!
					OMF2::omfInt32	wipeNumber = 0;
					OMF2::omfWipeArgs_t wipeControls;

					check = OMF2::omfeSMPTEVideoWipeGetInfo(OMFFileHdl, OMFSegment, NULL, NULL, NULL, NULL, &wipeNumber, &wipeControls);
					gpGlobals->pLogger->Log( kLogInfo, "%sReplacing 1.x Transition with a SMPTE Video Wipe Effect!\n ", gpGlobals->indentLeader);

					IAAFOperationGroup* pEffect;
					rc = defs.cdOperationGroup()->
					  CreateInstance(IID_IAAFOperationGroup,
									 (IUnknown **) &pEffect);
					AutoRelease< IAAFOperationGroup > peff( pEffect );

					IAAFOperationDef* pEffectDef;
					GetAAFOperationDefinition("omfi:effect:SMPTEVideoWipe", NULL, 
									"SMPTE Video Wipe", "Combines two video streams according to SMPTE ",
									-1, kAAFFalse, 2, DDEF_Picture, &pEffectDef);
					AutoRelease<IAAFOperationDef> peffdef( pEffectDef );

					rc = pEffect->Initialize(pDataDef, (aafLength_t)OMFLength, pEffectDef);
					rc = pEffect->SetBypassOverride(-1);

					// Port over Wipe number
					IAAFParameterDef* pParameterDef;
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefSMPTEWipeNumber, NULL, 
												L"Wipe Number", 
												L"SMPTE Wipe Number. No Default",
												L" ",
												&pParameterDef);
					AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );
					pEffectDef->AddParameterDef(pParameterDef);

					IAAFConstantValue* pConstantValue = NULL;
					rc = defs.cdConstantValue()->
					  CreateInstance(IID_IAAFConstantValue,
									 (IUnknown **)&pConstantValue);
					AutoRelease< IAAFConstantValue > pconstval( pConstantValue );
					rc = pConstantValue->SetValue(sizeof(wipeNumber), (unsigned char *)&wipeNumber);

					IAAFParameter*			pParameter;
					rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					AutoRelease<IAAFParameter>	pparam( pParameter );
					rc = pEffect->AddParameter(pParameter);
					rc = pParameter->SetParameterDefinition(pParameterDef);

					IAAFTypeDef*			typeDef;
					rc = pDictionary->LookupTypeDef(kAAFTypeID_Int32, &typeDef);
					AutoRelease<IAAFTypeDef> tdef( typeDef );
					rc = pParameter->SetTypeDefinition(typeDef);

					/*********************************************************************/
					//Port over reverse flag
					IAAFParameterDef* pParameterDefRev;
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefSMPTEReverse, NULL, 
												L"Reverse", 
												L"Reverse flag. Default FALSE.",
												L" ",
												&pParameterDefRev);
					AutoRelease< IAAFParameterDef > pparamdefrev( pParameterDefRev );
					rc = pEffectDef->AddParameterDef(pParameterDefRev);

					IAAFConstantValue* pConstantValueRev = NULL;
					rc = defs.cdConstantValue()->
					  CreateInstance(IID_IAAFConstantValue,
									 (IUnknown **)&pConstantValueRev);
					AutoRelease< IAAFConstantValue > pconstvalrev( pConstantValueRev );
					unsigned char	reverse = wipeControls.reverse;
					rc = pConstantValueRev->SetValue(sizeof(reverse), &reverse);

					IAAFParameter*			pParameterRev;
					rc = pConstantValueRev->QueryInterface(IID_IAAFParameter, (void **)&pParameterRev);
					AutoRelease<IAAFParameter>	pparamrev( pParameterRev );
					rc = pEffect->AddParameter(pParameterRev);
					rc = pParameterRev->SetParameterDefinition(pParameterDefRev);

					IAAFTypeDef*			typeDefRev;
					rc = pDictionary->LookupTypeDef(kAAFTypeID_Boolean, &typeDefRev);
					AutoRelease<IAAFTypeDef> tdefrev( typeDefRev );
					rc = pParameter->SetTypeDefinition(typeDefRev);
					/*********************************************************************/

					// !!! Port over the rest of the wipeControls later
					rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
				}
				else
				{
					// Unknown transition - convert it to a filler
					gpGlobals->pLogger->Log( kLogWarn, "Converting UNKNOWN 1.x Transition to a Filler !! \n ");
					IAAFFiller*		pFiller;
					rc = defs.cdFiller()->
					  CreateInstance(IID_IAAFFiller,
									 (IUnknown **) &pFiller);
					AutoRelease<IAAFFiller> pfill( pFiller );

					rc = pFiller->Initialize(pDataDef, (aafLength_t)OMFLength);
					rc = pFiller->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
				}
			}
			else if (OMF2::OM_ERR_PROP_NOT_PRESENT == testErr)
			{
				// Transition has no Effect ID - cannot convert !
				gpGlobals->pLogger->Log( kLogWarn, "Converting UNKNOWN 1.x Transition to a Filler.\n ");
				IAAFFiller*		pFiller;
				rc = defs.cdFiller()->
				  CreateInstance(IID_IAAFFiller,
								 (IUnknown **) &pFiller);
				AutoRelease<IAAFFiller> pfill( pFiller );
				rc = pFiller->Initialize(pDataDef, (aafLength_t)OMFLength);
				rc = pFiller->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
			}
			else
			{
				OMFError = testErr;
			}
		}
	}
	else if (OMF2::omfiIsANestedScope(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFNestedScope*		pNestedScope;
		rc = defs.cdNestedScope()->
		  CreateInstance(IID_IAAFNestedScope,
						 (IUnknown **) &pNestedScope);
		AutoRelease<IAAFNestedScope> pnest( pNestedScope );
		ConvertOMFNestedScope(OMFSegment, pNestedScope);
		rc = pNestedScope->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (OMF2::omfiIsAScopeRef(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFScopeReference*		pScopeRef;
		rc = defs.cdScopeReference()->
		  CreateInstance(IID_IAAFScopeReference,
						 (IUnknown **) &pScopeRef);
		AutoRelease<IAAFScopeReference> pscope( pScopeRef );
		ConvertOMFScopeRef(OMFSegment, pScopeRef);
		rc = pScopeRef->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (OMF2::omfiIsASelector(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFSelector*			pSelector = NULL;
		rc = defs.cdSelector()->
		  CreateInstance(IID_IAAFSelector,
						 (IUnknown **) &pSelector);
		AutoRelease<IAAFSelector> psel( pSelector );
		ConvertOMFSelector(OMFSegment, pSelector);
		rc = pSelector->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (OMF2::omfiIsAMediaGroup(OMFFileHdl, OMFSegment, &testErr) )
	{
		gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Media Group. No action taken.\n ", gpGlobals->indentLeader);
	}
	else
	{
		char				classID[5];
		OMF2::omfProperty_t idProp;

		if (OMF2::kOmfRev2x == OMFFileRev)
			idProp = OMF2::OMOOBJObjClass;
		else
			idProp = OMF2::OMObjID;

		OMFError = OMF2::omfsReadClassID(OMFFileHdl, OMFSegment, idProp, classID);
		// Some 1.x effects are detected here. They have each a class of their own !!!
		// For now e will create a 

//		if (strncmp(classID, "MASK", (size_t)4) == 0)
//		{
//			OMFError = OMF2::omfeVideoFrameMaskGetInfo();
//		}
//		else if (strncmp(classID, "REPT", (size_t)4) == 0)
//		{
//		}
//		else if (strncmp(classID, "SPED", (size_t)4) == 0)
//		{
//		}
//		else if (strncmp(classID, "PVOL", (size_t)4) == 0)
//		{
//		}
//		else if (strncmp(classID, "TKFX", (size_t)4) == 0)
//		{
//		}
//		else if (strncmp(classID, "TNFX", (size_t)4) == 0)
//		{
//		}
//		else
//		{
			gpGlobals->nNumUndefinedOMFObjects++;
			classID[4] = '\0';
			gpGlobals->pLogger->Log( kLogWarn, "%sUNKNOWN OBJECT : %s being replaced by a Filler \n", gpGlobals->indentLeader, classID);
			check = OMF2::omfiComponentGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength);
			ConvertOMFDataDef( OMFDatakind, &pDataDef);
			IAAFFiller*		pFiller;
			rc = defs.cdFiller()->
			  CreateInstance(IID_IAAFFiller,
							 (IUnknown **) &pFiller);
			rc = pFiller->Initialize(pDataDef, (aafLength_t)OMFLength);
			AutoRelease< IAAFFiller > pfill( pFiller );
			rc = pFiller->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
//		}
	}

	if( *ppComponent == NULL )
	{
			gpGlobals->pLogger->Log( kLogWarn, 
				"%sProcessOMFComponent returning NULL component.\n",  gpGlobals->indentLeader );
	}

	DecIndentLevel();
}
// ============================================================================
// ConvertOMFSequence
//
//			This function converts all the properties of an OMF Sequence, into 
//			their AAF  equivalent.
//			
// Returns: AAFRESULT_SUCCESS 
//
// ============================================================================
void Omf2Aaf::ConvertOMFSequence(OMF2::omfObject_t sequence, 
								   IAAFSequence* pSequence )
{
	// Get a pointer to a component interface
	IAAFComponent*			pComponent;
	AAFCheck rc = pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	AutoRelease<IAAFComponent> pcomp( pComponent );

	// Get Sequence data kind 
	OMF2::omfDDefObj_t		datakind = NULL;
	OMF2::omfLength_t		sequLength = 0;
	OMFCheck OMFError = OMF2::omfiSequenceGetInfo(OMFFileHdl, sequence, &datakind, &sequLength);
	IAAFDataDef * pDataDef = 0;
	ConvertOMFDataDef(datakind, &pDataDef);
	rc = pComponent->SetDataDef(pDataDef);
	pComponent->SetLength(sequLength);

		// Next - get all properties
//		rc = ConvertOMFComponentProperties(sequence, pComponent);
}
// ============================================================================
// ConvertOMFSourceClip
//
//			This function converts all the properties of an OMF Source Clip into 
//			their AAF  equivalent.
//			
// Returns: AAFRESULT_SUCCESS 
//
// ============================================================================
void Omf2Aaf::ConvertOMFSourceClip(OMF2::omfObject_t sourceclip, 
									 IAAFSourceClip* pSourceClip )
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	OMF2::omfDDefObj_t		datakind = NULL;
	OMF2::omfLength_t		clipLength = 0;
	OMF2::omfSourceRef_t	OMFSourceRef;
	OMF2::omfFadeType_t		OMFFadeinType;
	OMF2::omfFadeType_t		OMFFadeoutType;
	OMF2::omfBool			fadeinPresent, fadeoutPresent;

	IAAFDataDef * pDataDef = 0;
	aafSourceRef_t			sourceRef;
	aafFadeType_t			fadeinType, fadeoutType;
	aafInt32				fadeinLen, fadeoutLen;


	OMFError = OMF2::omfiSourceClipGetInfo(OMFFileHdl, sourceclip, &datakind, &clipLength, &OMFSourceRef);

	OMFError = OMF2::omfiSourceClipGetFade(OMFFileHdl, sourceclip, 
									 &fadeinLen, &OMFFadeinType, &fadeinPresent,
									 &fadeoutLen, &OMFFadeoutType, &fadeoutPresent);

	ConvertOMFDataDef(datakind, &pDataDef);
		rc = aafMobIDFromMajorMinor(42,
									OMFSourceRef.sourceID.major,
									OMFSourceRef.sourceID.minor,
									4,
									&sourceRef.sourceID);
		sourceRef.sourceSlotID = OMFSourceRef.sourceTrackID;
		sourceRef.startTime    = OMFSourceRef.startTime;
		fadeinType = (aafFadeType_t)OMFFadeinType;
		fadeoutType = (aafFadeType_t)OMFFadeoutType;
		rc = pSourceClip->Initialize(pDataDef, clipLength, sourceRef);
		if (fadeinPresent || fadeoutPresent)
			rc = pSourceClip->SetFade(fadeinLen, fadeinType, fadeoutLen, fadeoutType);
}
// ============================================================================
// ConvertOMFComponentProperties
//
//			This function converts all the properties of an OMF Component, into 
//			their AAF  equivalent.
//			
// Returns: AAFRESULT_SUCCESS 
//
// ============================================================================
void Omf2Aaf::ConvertOMFComponentProperties(OMF2::omfObject_t component, 
											 IAAFComponent* pComponent )
{
	AAFCheck				rc;
	OMF2::omfErr_t			OMFError;

	OMF2::omfProperty_t		Property;
	OMF2::omfUniqueName_t	propertyName;
	OMF2::omfIterHdl_t		propertyIterator = NULL;
	OMF2::omfType_t			propertyType = OMF2::OMNoType;
	OMF2::omfDDefObj_t		datakind = NULL;

	IncIndentLevel();
	OMF2::omfiIteratorAlloc(OMFFileHdl, &propertyIterator);
	OMFError = OMF2::omfiGetNextProperty(propertyIterator, component, &Property, &propertyType);
	while((OMF2::OM_ERR_NONE == OMFError) &&Property)
	{
		switch (Property)
		{
			case OMF2::OMOOBJObjClass:
			case OMF2::OMCPNTLength:
			case OMF2::OMCPNTDatakind:
			case OMF2::OMSEQUComponents:
				gpGlobals->nNumOMFProperties++;
				gpGlobals->nNumAAFProperties++;
				break;
			case OMF2::OMCPNTTrackKind:
			case OMF2::OMCPNTEditRate:
			case OMF2::OMCPNTName:
			case OMF2::OMCPNTEffectID:
			case OMF2::OMCPNTAttributes:
			default:
				gpGlobals->nNumOMFProperties++;
				OMFError = OMF2::omfiGetPropertyName(OMFFileHdl, Property, 64, propertyName);
				gpGlobals->pLogger->Log(kLogWarn, "%sComponent Property NOT converted : %s\n", gpGlobals->indentLeader, propertyName);
				break;
		}
		OMFError = OMF2::omfiGetNextProperty(propertyIterator, component, &Property, &propertyType);
	}
	OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, propertyIterator);
	DecIndentLevel();
}
// ============================================================================
// TraverseOMFSequence
//
//			This function reads all components the of an OMF Sequence,
//			sets the equivalent AAF properties 
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::TraverseOMFSequence(OMF2::omfObject_t sequence, IAAFSequence* pSequence )
{
	aafInt32				numComponents = 0;
	OMFCheck OMFError = OMF2::omfiSequenceGetNumCpnts(OMFFileHdl, sequence, &numComponents);
	if (numComponents > 0)
	{
		OMF2::omfIterHdl_t		componentIterator;
		OMFError = OMF2::omfiIteratorAlloc(OMFFileHdl, &componentIterator);
		for (aafInt32 cpntCount = 0; cpntCount < numComponents; cpntCount++)
		{   
			OMF2::omfPosition_t		sequPos;
			OMF2::omfCpntObj_t		sequComponent = NULL;
			OMFError = OMF2::omfiSequenceGetNextCpnt(componentIterator, sequence, NULL, 
											   &sequPos, &sequComponent); 
			IAAFComponent*			pComponent;
			ProcessOMFComponent(sequComponent, &pComponent);
			if( pComponent )
			{
				AutoRelease<IAAFComponent> pcomp( pComponent );
				AAFCheck rc = pSequence->AppendComponent(pComponent);
			}
		}
		OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, componentIterator);
	}
}
// ============================================================================
// ConvertOMFMasterMob
//
//			This function extracts all the properties of an OMF Master MOB,
//			sets the equivalent AAF properties and 
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFMasterMob(OMF2::omfObject_t obj,
									IAAFMasterMob* pMasterMob )
{
	AAFCheck				rc;
	
	gpGlobals->pLogger->Log( kLogInfo, "Converting OMF Master MOB to AAF\n");
}
// ============================================================================
// ConvertOMFLocator
//
//			This function extracts all the properties of an OMF Locator,
//			sets the equivalent AAF properties
//			
// Returns: An exception if  not  converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFLocator(OMF2::omfObject_t obj,
									IAAFEssenceDescriptor* pEssenceDesc)
{
	CAAFBuiltinDefs defs (pDictionary);

	OMF2::omfIterHdl_t		locatorIter;
	OMFCheck omfCheck = OMF2::omfiIteratorAlloc(OMFFileHdl, &locatorIter);
	OMF2::omfErr_t			testErr;

	OMF2::omfObject_t		OMFLocator;
	OMFCheck				OMFError;
	testErr = OMF2::omfmMobGetNextLocator(locatorIter, obj, &OMFLocator);

	while((testErr == OMF2::OM_ERR_NONE) &&(OMFLocator != NULL))
	{
		OMF2::omfClassID_t		locType;
		char					locatorPath[128];
		omfCheck = OMF2::omfmLocatorGetInfo(OMFFileHdl, OMFLocator, locType, 
			sizeof( locatorPath ), locatorPath);
		std::auto_ptr<wchar_t> plocpath(  new wchar_t[strlen(locatorPath)+1] );
		aafWChar *pwLocatorPath = plocpath.get();
		mbstowcs(pwLocatorPath, locatorPath, strlen(locatorPath)+1);

		IAAFNetworkLocator*		pNetworkLocator;
		AAFCheck aafCheck = defs.cdNetworkLocator()->
		  CreateInstance(IID_IAAFNetworkLocator,
						 (IUnknown **)&pNetworkLocator);
		AutoRelease<IAAFNetworkLocator> pnetloc( pNetworkLocator );

		IAAFLocator*			pLocator;
		aafCheck = pNetworkLocator->QueryInterface(IID_IAAFLocator, (void **)&pLocator);
		AutoRelease<IAAFLocator> ploc( pLocator );

		aafCheck = pLocator->SetPath(pwLocatorPath);
		aafCheck = pEssenceDesc->AppendLocator(pLocator);
		gpGlobals->pLogger->Log( kLogInfo, 
				"%sAdded a Network locator to the Essence Descriptor\n", gpGlobals->indentLeader);
		testErr = OMF2::omfmMobGetNextLocator(locatorIter, obj, &OMFLocator);
	}

	OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, locatorIter);
}
// ============================================================================
// ConvertOMFCDCIDescriptorLocator
//
//			This function extracts all the properties of an CDCI Image Descriptor,
//			sets the equivalent AAF properties
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFCDCIDescriptorLocator(OMF2::omfObject_t mediaDescriptor,
									IAAFCDCIDescriptor* pAAFDescriptor)
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	OMF2::omfFrameLayout_t	frameLayout;
	OMF2::omfPosition_t		zeroPos = 0;
	OMF2::omfPosition_t		fourPos = 0;

	IAAFDigitalImageDescriptor*	pDigImageDesc = NULL;

	aafUInt32			Height = 0;
	aafUInt32			Width = 0;
	aafInt32			XOffset = 0;
	aafInt32			YOffset = 0;
	aafInt32			alphaTransparency = 0;
	aafInt32			alignmentFactor;

	aafFrameLayout_t	AAFFrameLayout;
	aafRational_t		gamma;
	aafRational_t		aspectRatio;

	aafInt32			componentWidth = 0;
	aafUInt32			horizontalSubsampling = 0;
	aafColorSiting_t	colorSiting = kAAFCoSiting;
	aafUInt32			blackReferenceLevel = 0;
	aafUInt32			whiteReferenceLevel = 0;
	aafUInt32			colorRange = 0;
	aafInt16			paddingBits = 0;
	aafInt32			videoLineMap[2];
	aafUInt32			compLen;
	char				*compress = NULL;
	aafUID_t			AAFCompress;
						
	rc = pAAFDescriptor->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&pDigImageDesc);

	// Get Digital Image properties and set them
	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDStoredHeight, &Height) != OMF2::OM_ERR_NONE)
	{
		if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDStoredHeight, (OMF2::omfInt32 *)&Height) != OMF2::OM_ERR_NONE)
		{
			Height = 0;
		}
	}

	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDStoredWidth, &Width) != OMF2::OM_ERR_NONE)
	{
		if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDStoredWidth, (OMF2::omfInt32 *)&Width) != OMF2::OM_ERR_NONE)
		{
			Width = 0;
		}
	}
	rc = pDigImageDesc->SetStoredView(Height, Width);

	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledHeight, &Height) != OMF2::OM_ERR_NONE)
	{
		if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledHeight, (OMF2::omfInt32 *)&Height) != OMF2::OM_ERR_NONE)
		{
			Height = 0;
		}
	}
	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledWidth, &Width) != OMF2::OM_ERR_NONE)
	{
		if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledWidth, (OMF2::omfInt32 *)&Width) != OMF2::OM_ERR_NONE)
		{
			Width = 0;
		}
	}
	if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledXOffset, &XOffset) != OMF2::OM_ERR_NONE)
		XOffset = 0;
	if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledYOffset, &YOffset) != OMF2::OM_ERR_NONE)
		YOffset = 0;
	rc = pDigImageDesc->SetSampledView(Height, Width, XOffset, YOffset);

	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayHeight, &Height) != OMF2::OM_ERR_NONE)
	{
		if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayHeight, (OMF2::omfInt32 *)&Height) != OMF2::OM_ERR_NONE)
		{
			Height = 0;
		}
	}
	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayWidth, &Width) != OMF2::OM_ERR_NONE)
	{
		if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayWidth, (OMF2::omfInt32 *)&Width) != OMF2::OM_ERR_NONE)
		{
			Width = 0;
		}
	}
	if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayXOffset, &XOffset) != OMF2::OM_ERR_NONE)
		XOffset = 0;
	if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayYOffset, &YOffset) != OMF2::OM_ERR_NONE)
		YOffset = 0;
	rc = pDigImageDesc->SetDisplayView(Height, Width, XOffset, YOffset);

	if (OMF2::omfsReadLayoutType(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDFrameLayout, &frameLayout) != OMF2::OM_ERR_NONE)
		AAFFrameLayout = (aafFrameLayout_t)0;
	else
	{
		if (frameLayout > 0)
			AAFFrameLayout = (aafFrameLayout_t)(frameLayout-1);
		else
			AAFFrameLayout = (aafFrameLayout_t)0;
	}
	rc = pDigImageDesc->SetFrameLayout(AAFFrameLayout);
	if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDAlphaTransparency, &alphaTransparency) != OMF2::OM_ERR_NONE)
		alphaTransparency = 0;
	rc = pDigImageDesc->SetAlphaTransparency((aafAlphaTransparency_t)alphaTransparency);

	if (OMF2::omfsReadRational(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDGamma, (OMF2::omfRational_t *)&gamma) != OMF2::OM_ERR_NONE)
	{
		gamma.numerator = 0;
		gamma.denominator = 1;
	}
	rc = pDigImageDesc->SetGamma(gamma);

	if (OMF2::omfsReadRational(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDImageAspectRatio, (OMF2::omfRational_t *)&aspectRatio) != OMF2::OM_ERR_NONE)
	{
		aspectRatio.numerator = 0;
		aspectRatio.denominator = 1;
	}
	rc = pDigImageDesc->SetImageAspectRatio(aspectRatio);

	if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDFieldAlignment, &alignmentFactor) != OMF2::OM_ERR_NONE)
		alignmentFactor = 0;
	rc = pDigImageDesc->SetImageAlignmentFactor(alignmentFactor);

	if (OMF2::OMReadProp(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDVideoLineMap, 
						   zeroPos, OMF2::kSwabIfNeeded, OMF2::OMInt32Array,
						   sizeof(aafInt32), &videoLineMap[0]) != OMF2::OM_ERR_NONE)
		videoLineMap[0] = 0;
	omfsCvtInt32toPosition(sizeof(aafInt32), fourPos);
	if (OMF2::OMReadProp(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDVideoLineMap, 
						   fourPos, OMF2::kSwabIfNeeded, OMF2::OMInt32Array,
						   sizeof(aafInt32), &videoLineMap[1]) != OMF2::OM_ERR_NONE)
		videoLineMap[1] = 0;

	rc = pDigImageDesc->SetVideoLineMap( (sizeof(videoLineMap)/sizeof(aafInt32)), videoLineMap);

	//
	compLen = OMF2::omfsLengthString(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDCompression);
	compress = new char[compLen+1];			
	OMFError = OMF2::omfsReadString(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDCompression, compress,
						compLen+1);
	if(strcmp(compress, "JFIF") == 0)
		AAFCompress = CodecJPEG;
	else
		AAFCompress = NoCodec;
	rc = pDigImageDesc->SetCompression(AAFCompress);
	delete [] compress;
	compress = NULL;
	pDigImageDesc->Release();
	pDigImageDesc = NULL;

	// Next we read the values
	if (OMF2::omfsReadInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIComponentWidth, &componentWidth) != OMF2::OM_ERR_NONE)
		componentWidth = 0;
	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIHorizontalSubsampling, &horizontalSubsampling) != OMF2::OM_ERR_NONE)
		horizontalSubsampling = 0;
	if (OMF2::OMReadProp(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIHorizontalSubsampling, 
						  zeroPos, OMF2::kSwabIfNeeded, OMF2::OMColorSitingType,
						  sizeof(colorSiting), (void *)&(colorSiting)) != OMF2::OM_ERR_NONE)
		memset(&colorSiting, 0, sizeof(aafColorSiting_t));
	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIBlackReferenceLevel, &blackReferenceLevel) != OMF2::OM_ERR_NONE)
		blackReferenceLevel = 0;
	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIWhiteReferenceLevel, &whiteReferenceLevel) != OMF2::OM_ERR_NONE)
		whiteReferenceLevel = 0;
	if (OMF2::omfsReadUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIColorRange, &colorRange) != OMF2::OM_ERR_NONE)
		colorRange = 0;
	if (OMF2::OMReadProp(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIPaddingBits, 
						   zeroPos, OMF2::kSwabIfNeeded, OMF2::OMInt16,
						   sizeof(paddingBits), &paddingBits) != OMF2::OM_ERR_NONE)
		paddingBits = 0;
	// Now set the EssenceDescriptor
	rc = pAAFDescriptor->SetComponentWidth(componentWidth);
	rc = pAAFDescriptor->SetHorizontalSubsampling(horizontalSubsampling);
	rc = pAAFDescriptor->SetColorSiting(colorSiting);
	rc = pAAFDescriptor->SetBlackReferenceLevel(blackReferenceLevel);
	rc = pAAFDescriptor->SetWhiteReferenceLevel(whiteReferenceLevel);
	rc = pAAFDescriptor->SetColorRange(colorRange);
	rc = pAAFDescriptor->SetPaddingBits(paddingBits);
}
// ============================================================================
// ConvertOMFSourceMob
//
//			This function extracts all the properties of an OMF Source MOB,
//			sets the equivalent AAF properties and 
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFSourceMob(OMF2::omfObject_t obj,
									IAAFSourceMob* pSourceMob)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	OMF2::omfErr_t			testErr;
	
	OMF2::omfObject_t		mediaDescriptor;
	OMF2::omfInt32			numLocators = 0;
	
	IAAFEssenceDescriptor*	pEssenceDesc = NULL;
	IAAFTapeDescriptor*		pTapeDesc = NULL;
	IAAFFileDescriptor*		pFileDesc = NULL;
	IAAFTIFFDescriptor*		pTiffDesc = NULL;
	IAAFWAVEDescriptor*		pWAVEDesc = NULL;
	IAAFAIFCDescriptor*		pAifcDesc = NULL;
	IAAFCDCIDescriptor*		pCDCIDesc = NULL;
	IAAFObject*				pElement = NULL;
	
	aafLength_t				length = 0;
	aafRational_t			sampleRate;
	
	CAAFBuiltinDefs defs (pDictionary);
	
	gpGlobals->pLogger->Log( kLogInfo, "Converting OMF Source MOB to AAF\n");
	
	
	IncIndentLevel();
	aafUID_t	classID = kAAFClassID_SourceMob;
	
	rc = pSourceMob->QueryInterface(IID_IAAFObject, (void **)&pElement);
	ConvertObjectProps(obj, classID, pElement);
	
	OMFError = OMF2::omfmMobGetMediaDescription(OMFFileHdl, obj, &mediaDescriptor);
	
	if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDFL, &testErr))
	{
		// File Source Mob
		if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassTIFD, &testErr) )
		{
			// TIFF File Descriptor
			OMF2::omfBool			IsContiguous, IsUniform;
			OMF2::omfInt32			leadingLines, trailingLines;
			char					summary[1024];	//!!!Fixed size
			OMF2::omfJPEGTableID_t	data;
			OMF2::omfDDefObj_t		datakind;
			
			aafUInt32				bytesRead = 0;
			
			rc = defs.cdTIFFDescriptor()->
				CreateInstance(IID_IAAFTIFFDescriptor,
				(IUnknown **)&pTiffDesc);
			
			OMFError = OMF2::omfsReadBoolean( OMFFileHdl,
				mediaDescriptor,
				OMF2::OMTIFDIsContiguous, &IsContiguous);
			OMFError = OMF2::omfsReadBoolean( OMFFileHdl,
				mediaDescriptor,
				OMF2::OMTIFDIsUniform,
				&IsUniform);
			OMFError = OMF2::omfsReadInt32(OMFFileHdl,
				mediaDescriptor,
				OMF2::OMTIFDLeadingLines, 
				&leadingLines);
			OMFError = OMF2::omfsReadInt32(OMFFileHdl,
				mediaDescriptor,
				OMF2::OMTIFDTrailingLines, 
				&trailingLines);
			OMFError = OMF2::omfsReadJPEGTableIDType( OMFFileHdl,
				mediaDescriptor,
				OMF2::OMTIFDJPEGTableID, 
				&data);
			if (OMF2::kOmfRev2x == OMFFileRev)
			{
				OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&testErr);
				//Read only as much as there is...
				(void)OMF2::omfsReadDataValue(OMFFileHdl,
					mediaDescriptor,
					OMF2::OMTIFDSummary,
					datakind,
					summary,
					0,
					sizeof(summary),
					&bytesRead);
			}
			else
			{
				OMFError = OMF2::omfsReadVarLenBytes(OMFFileHdl,
					mediaDescriptor,
					OMF2::OMTIFDSummary,
					0,
					sizeof(summary),
					summary,
					&bytesRead);
			}
			
			rc = pTiffDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
			rc = pTiffDesc->SetIsUniform((aafBool)IsUniform);
			rc = pTiffDesc->SetIsContiguous((aafBool)IsContiguous);
			rc = pTiffDesc->SetLeadingLines((aafInt32)leadingLines);
			rc = pTiffDesc->SetTrailingLines((aafInt32)trailingLines);
			rc = pTiffDesc->SetJPEGTableID((aafJPEGTableID_t)data);
			if (bytesRead > 0)
				pTiffDesc->SetSummary((aafUInt32)bytesRead, (aafDataValue_t) summary);
			rc = pSourceMob->SetEssenceDescriptor(pEssenceDesc);
			gpGlobals->pLogger->Log( kLogInfo, 
				"%sAdded a TIFF Essence Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			if (pTiffDesc)
			{
				pTiffDesc->Release();
				pTiffDesc = NULL;
			}
		}
		else if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassWAVD, &testErr) )
		{
			// WAVE File Descriptor
			OMF2::omfDDefObj_t		datakind;
			char					*summary;
			aafUInt32				bytesRead;
			aafUInt32				numBytes;
			
			rc = defs.cdWAVEDescriptor()->
				CreateInstance(IID_IAAFWAVEDescriptor,
				(IUnknown **)&pWAVEDesc);
			if (OMF2::kOmfRev2x == OMFFileRev)
			{
				OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, &testErr);
				numBytes = (aafUInt32)OMF2::omfsLengthDataValue(OMFFileHdl, mediaDescriptor, OMF2::OMWAVDSummary);
				summary = new char[numBytes];
				OMFError = OMF2::omfsReadDataValue(OMFFileHdl,
					mediaDescriptor,
					OMF2::OMWAVDSummary,
					datakind,
					(OMF2::omfDataValue_t)summary,
					0,
					numBytes,
					&bytesRead);
			}
			else
			{
				numBytes = (aafUInt32)OMF2::omfsLengthVarLenBytes(OMFFileHdl,
					mediaDescriptor,
					OMF2::OMWAVDSummary);
				summary = new char[numBytes];
				OMFError = OMF2::omfsReadVarLenBytes(OMFFileHdl,
					mediaDescriptor,
					OMF2::OMWAVDSummary,
					0,
					sizeof(summary),
					summary,
					&bytesRead);
			}
			rc = pWAVEDesc->SetSummary(bytesRead, (aafDataValue_t)summary);
			delete [] summary;
			rc = pWAVEDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
			pSourceMob->SetEssenceDescriptor(pEssenceDesc);
			gpGlobals->pLogger->Log( kLogInfo,
				"%sAdded a Wave Essence Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			if (pWAVEDesc)
			{
				pWAVEDesc->Release();
				pWAVEDesc = NULL;
			}
		}
		else if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassAIFD, &testErr) )
		{
			// AIFC Audio File Descriptor
			char					summary[1024];		// Fixed size (see WAVE)
			OMF2::omfDDefObj_t		datakind;
			
			aafUInt32				bytesRead;
			aafUInt32				numBytes;
			
			rc = defs.cdAIFCDescriptor()->
				CreateInstance(IID_IAAFAIFCDescriptor,
				(IUnknown **)&pAifcDesc);
			if (OMF2::kOmfRev2x == OMFFileRev)
			{
				OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, &testErr);
				numBytes = (aafUInt32)OMF2::omfsLengthDataValue(OMFFileHdl, mediaDescriptor, OMF2::OMAIFDSummary);
				OMFError = OMF2::omfsReadDataValue(OMFFileHdl,
					mediaDescriptor,
					OMF2::OMAIFDSummary,
					datakind,
					(OMF2::omfDataValue_t)summary,
					0,
					numBytes,
					&bytesRead);
			}
			else
			{
				OMFError = OMF2::omfsReadVarLenBytes(OMFFileHdl,
					mediaDescriptor,
					OMF2::OMAIFDSummary,
					0,
					sizeof(summary),
					summary,
					&bytesRead);
			}
			rc = pAifcDesc->SetSummary(bytesRead, (aafDataValue_t) summary);
			
			rc = pAifcDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
			pSourceMob->SetEssenceDescriptor(pEssenceDesc);
			gpGlobals->pLogger->Log( kLogInfo, 
				"%sAdded a AIFC Essence Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			if (pAifcDesc)
			{
				pAifcDesc->Release();
				pAifcDesc = NULL;
			}
		}
		else if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassCDCI, &testErr) )
		{
			rc = defs.cdCDCIDescriptor()->
				CreateInstance(IID_IAAFCDCIDescriptor,
				(IUnknown **)&pCDCIDesc);
			ConvertOMFCDCIDescriptorLocator(mediaDescriptor, pCDCIDesc);
			rc = pCDCIDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
			pSourceMob->SetEssenceDescriptor(pEssenceDesc);
			if (pCDCIDesc)
			{
				pCDCIDesc->Release();
				pCDCIDesc = NULL;
			}
		}
		else
		{
			// Generic or not recognizable media descriptor !!!
			OMF2::omfClassID_t	objClass;
			char				id[5];
			char				summary[64];
			
			OMFError = OMF2::omfsReadClassID(OMFFileHdl, mediaDescriptor, OMF2::OMOOBJObjClass, objClass);
			strncpy(id, objClass, 4);
			id[4] = '\0';
			gpGlobals->pLogger->Log( kLogError,
				"%sCannot translate this Media File Descriptor: %s\n", gpGlobals->indentLeader, id);
			gpGlobals->nNumUndefinedOMFObjects++;
			// as a cop-out we generate a Wave descriptor and continue 
			rc = defs.cdWAVEDescriptor()->
				CreateInstance(IID_IAAFWAVEDescriptor,
				(IUnknown **)&pWAVEDesc);
			memset(summary, 0, sizeof(summary));
			rc = pWAVEDesc->SetSummary(sizeof(summary), (aafDataValue_t)summary);
			rc = pWAVEDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
			pSourceMob->SetEssenceDescriptor(pEssenceDesc);
			if (pWAVEDesc)
			{
				pWAVEDesc->Release();
				pWAVEDesc = NULL;
			}
		}
		// Retrieve and set generic File Descriptor properties.
		rc = pEssenceDesc->QueryInterface(IID_IAAFFileDescriptor, (void **) &pFileDesc);
		
		OMFError = OMF2::omfsReadRational(OMFFileHdl, mediaDescriptor, OMF2::OMMDFLSampleRate, (OMF2::omfRational_t *)&sampleRate);
		rc = pFileDesc->SetSampleRate(sampleRate);
		
		OMFError = OMF2::omfsReadLength(OMFFileHdl, mediaDescriptor, OMF2::OMMDFLLength, (OMF2::omfLength_t *)&length);
		rc = pFileDesc->SetLength(length);
		
		pFileDesc->Release();
		pFileDesc = NULL;
	}
	else
	{
		// Physical Source Mob
		if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDTP, &testErr) ||
			OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDES, &testErr) )
		{
			// Tape Media descriptor
			OMF2::omfTapeCaseType_t		formFactor;
			OMF2::omfVideoSignalType_t	videoSignal;
			OMF2::omfTapeFormatType_t	tapeFormat;
			OMF2::omfLength_t			length;
			char						manufacturer[64], model[64];
			OMF2::omfInt32				manuSize, modelSize;
			
			aafWChar*					pwManufacturer = NULL;
			aafWChar*					pwModel = NULL;
			
			rc = defs.cdTapeDescriptor()->
				CreateInstance(IID_IAAFTapeDescriptor,
				(IUnknown **)&pTapeDesc);
			manuSize = modelSize = 64;
			if (OMF2::OM_ERR_NONE == OMF2::omfmTapeMobGetDescriptor(OMFFileHdl, 
				obj,
				&formFactor,
				&videoSignal,
				&tapeFormat,
				&length,
				manuSize, manufacturer,
				modelSize, model))
			{
				if (strlen(manufacturer) > 0)
				{
					pwManufacturer = new wchar_t[strlen(manufacturer)+1];
					mbstowcs(pwManufacturer, manufacturer, strlen(manufacturer)+1);
				}
				if (strlen(model) > 0)
				{
					pwModel = new wchar_t[strlen(model)+1];
					mbstowcs(pwModel, model, strlen(model)+1);
				}
			}
			else
			{
				char	*src = "Not provided";
				
				pwManufacturer = new wchar_t[strlen(src)+1];
				mbstowcs(pwManufacturer, model, strlen(src)+1);
				pwModel = new wchar_t[strlen(src)+1];
				mbstowcs(pwModel, model, strlen(src)+1);
				formFactor = OMF2::kTapeCaseNull;
				videoSignal = OMF2::kVideoSignalNull;
				tapeFormat = OMF2::kTapeFormatNull;
				length = 0;
			}
			pTapeDesc->SetTapeModel(pwModel);
			pTapeDesc->SetTapeManufacturer(pwManufacturer);
			pTapeDesc->SetTapeFormFactor((aafTapeCaseType_t)formFactor);
			pTapeDesc->SetSignalType((aafVideoSignalType_t)videoSignal);
			pTapeDesc->SetTapeFormat((aafTapeFormatType_t)tapeFormat);
			pTapeDesc->SetTapeLength((aafLength_t)length);
			rc = pTapeDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
			pSourceMob->SetEssenceDescriptor(pEssenceDesc);
			gpGlobals->pLogger->Log(kLogInfo,
				"%sAdded a Tape Essence Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			if (pTapeDesc)
			{
				pTapeDesc->Release();
				pTapeDesc = NULL;
			}
			if (pwManufacturer)
				delete [] pwManufacturer;
			if (pwModel)
				delete [] pwModel;
		}
		else if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDFM, &testErr))
		{
			// Film Media descriptor
			gpGlobals->pLogger->Log( kLogError, "%sFilm media descriptor NOT Implemented yet\n", gpGlobals->indentLeader);
		}
		else
		{
			OMF2::omfClassID_t	objClass;
			OMF2::omfClassID_t	objSuperClass;
			OMF2::omfProperty_t	propertyID;
			OMF2::omfBool		found;
			char				id[5];
			char				superID[5];
			
			if (OMF2::kOmfRev2x == OMFFileRev)
			{
				propertyID = OMF2::OMOOBJObjClass;
			}
			else
			{
				propertyID = OMF2::OMObjID;
			}
			OMFError = OMF2::omfsReadClassID(OMFFileHdl, mediaDescriptor, propertyID, objClass);
			strncpy(id, objClass, 4);
			id[4] = '\0';
			gpGlobals->pLogger->Log( kLogError,
				"%sCannot translate this Physical Media Descriptor: %s\n", gpGlobals->indentLeader, id);
			gpGlobals->nNumUndefinedOMFObjects++;
			// Here we could check for a Dictionary entry to translate this class !!
			// For now lets try to figure out what kind of descriptor can we do.
			rc = OMF2::omfsClassFindSuperClass(OMFFileHdl, objClass, objSuperClass, &found);
			if (found)
			{
				// we found the class 
				strncpy(superID, objSuperClass, 4);
				superID[4] = '\0';
				gpGlobals->pLogger->Log( kLogInfo, "%s %s ---> %s\n", gpGlobals->indentLeader, id, superID);
			}
		}
		}
		// Retrieve any locator info from the Source mob
		if(OMF2::omfmMobGetNumLocators(OMFFileHdl, obj, &numLocators) == OM_ERR_NONE)
		{
			if (numLocators > 0)
			{
				ConvertOMFLocator(obj, pEssenceDesc);
			}
		}
		
		if (pEssenceDesc)
			pEssenceDesc->Release();
		
		
		DecIndentLevel();
}
// ============================================================================
// ConvertOMFConstValue
//
//			This function extracts all the properties of an OMF Constant Value 
//			segment, and converts it into an AAF Constant Value object.
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFConstValue(OMF2::omfSegObj_t segment,
									  IAAFConstantValue* pConstValue)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	
	OMF2::omfDDefObj_t	cvDatakind;
	OMF2::omfLength_t	cvLength;
	OMF2::omfInt64		cvValueSize;
	OMF2::omfInt64		cvBytesRead;
	void *				pcvBuffer = NULL;

	aafUInt32			valueSize;
	IAAFParameter*		pParameter;
	IAAFTypeDef*		pTypeDef = NULL;

	OMFError = OMF2::omfiDataValueGetSize(OMFFileHdl, segment, &cvValueSize);
	OMF2::omfsTruncInt64toUInt32(cvValueSize, &valueSize);
	if (valueSize > 0)
	{
		pcvBuffer = new char[valueSize];
		rc = pConstValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
		OMFError = OMF2::omfiConstValueGetInfo(OMFFileHdl, segment, 
							&cvDatakind, &cvLength, cvValueSize, &cvBytesRead, pcvBuffer);
		gpGlobals->pLogger->Log( kLogInfo, 
			"%sProcessing Constant Value of length = %ld\n ", gpGlobals->indentLeader, (int)cvLength);
		rc = pConstValue->SetValue(valueSize, (unsigned char *)pcvBuffer);
		
		ConvertOMFDataDefType(cvDatakind, &pTypeDef);
		pParameter->SetTypeDefinition(pTypeDef);
		pTypeDef->Release();
		pTypeDef = NULL;
	}
	if (pcvBuffer)
		delete [] pcvBuffer;
	if (pParameter)
		pParameter->Release();
}
// ============================================================================
// ConvertOMFVaryingValue
//
//			This function extracts all the properties of an OMF Varying Value 
//			segment, and converts it into an AAF Constant Value object.
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFVaryingValue(OMF2::omfSegObj_t segment,
										IAAFVaryingValue* pVaryingValue)
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	OMF2::omfIterHdl_t		OMFIterator;
	OMF2::omfInt32			numPoints;
	OMF2::omfDDefObj_t		vvDatakind;
	OMF2::omfCntlPtObj_t	control;
	OMF2::omfDDefObj_t		cpDatakind;
	OMF2::omfDDefObj_t		tmpDatakind;
	OMF2::omfLength_t		vvLength;
	OMF2::omfInterpKind_t	vvInterpolation;
	OMF2::omfRational_t		time;
	OMF2::omfEditHint_t		editHint;
	OMF2::omfInt64			cpValueSize;
	OMF2::omfUInt32			bytesRead = 0;
	OMF2::omfPosition_t		offset;
	void *					pCPBuffer = NULL;

	IAAFControlPoint*		pControlPoint = NULL;
	IAAFTypeDef*			pTypeDef = NULL;
	IAAFInterpolationDef*	pInterp = NULL;
	IAAFParameter*			pParm = NULL;

	aafRational_t			AAFCPTime;
	aafEditHint_t			AAFCPEditHint;
	aafUInt32				valueSize;
	OMF2::omfErr_t			testErr;

	CAAFBuiltinDefs defs (pDictionary);

	IncIndentLevel();
	OMFError = OMF2::omfiVaryValueGetInfo(OMFFileHdl, segment, &vvDatakind, &vvLength, &vvInterpolation);
	// tlk We do NOT know how to handle Interpolations yet !!!

	ConvertOMFDataDefType(vvDatakind, &pTypeDef);
	rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParm);
	pParm->SetTypeDefinition(pTypeDef);
	pParm->Release();
	pParm = NULL;
	pTypeDef->Release();
	pTypeDef = NULL;

	// Get number of Points 
	OMFError = OMF2::omfiVaryValueGetNumPoints(OMFFileHdl, segment, &numPoints);

	gpGlobals->pLogger->Log( kLogInfo,
		"%sProcessing Varying Value of length = %ld and %ld Control Points\n ", gpGlobals->indentLeader, (int)vvLength, (int)numPoints);
	if (numPoints > 0)
	{
		OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
		for (int i = 0; i < numPoints;i++)
		{
			OMFError = OMF2::omfiVaryValueGetNextPoint(OMFIterator, segment, NULL, &control);
			defs.cdControlPoint()->
				CreateInstance(IID_IAAFControlPoint,
				(IUnknown **)&pControlPoint);
			OMFError = OMF2::omfiDataValueGetSize(OMFFileHdl, control, &cpValueSize);
			OMF2::omfsTruncInt64toUInt32(cpValueSize, &valueSize);
			pCPBuffer = new char[valueSize];
			OMFError = OMF2::omfiControlPtGetInfo(OMFFileHdl, control, &time, &editHint, &cpDatakind, 
				valueSize, (long *)&bytesRead, pCPBuffer);
			
			//!!! The above call is bad in the currently tested version of the OMF TK
			//!!! Get the information another way (next two function calls
			OMF2::omfsReadObjRef(OMFFileHdl, control, OMF2::OMCTLPDatakind, &cpDatakind);
			OMFError = OMF2::omfsReadDataValue(OMFFileHdl, control,
				OMF2::OMCTLPValue, cpDatakind, pCPBuffer,
				0, valueSize, &bytesRead);
			
			AAFCPTime.numerator = time.numerator;
			AAFCPTime.denominator = time.denominator;
			AAFCPEditHint = (aafEditHint_t)editHint;
			pControlPoint->SetTime(AAFCPTime);
			pControlPoint->SetEditHint(AAFCPEditHint);
			if (bytesRead != valueSize)
			{
				OMF2::omfUniqueName_t	uniqueName;
				
				OMFError = OMF2::omfsReadObjRef(OMFFileHdl, control, OMF2::OMCTLPDatakind, &cpDatakind);
				OMFError = OMF2::omfsReadUniqueName(OMFFileHdl, cpDatakind, OMF2::OMDDEFDatakindID, uniqueName, sizeof(uniqueName));
				if (strcmp(uniqueName, "omfi:data:Rational") == 0 && (valueSize == 4))
				{
					OMF2::omfiDatakindLookup(OMFFileHdl, (OMF2::omfUniqueNamePtr_t)"omfi:data:Int32", &tmpDatakind, &testErr);
					cpDatakind = tmpDatakind;
				}
				offset = 0;
				OMFError = OMF2::omfsReadDataValue(OMFFileHdl, control, OMF2::OMCTLPValue, cpDatakind, pCPBuffer, offset,valueSize, &bytesRead);
			}
			pControlPoint->SetValue((aafUInt32)valueSize, (unsigned char *)pCPBuffer);
			pControlPoint->SetTypeDefinition(pTypeDef);
			pVaryingValue->AddControlPoint(pControlPoint);
			pControlPoint->Release();
			pControlPoint = NULL;
			if (pCPBuffer)
				delete [] pCPBuffer;
			if (pControlPoint)
				pControlPoint->Release();
			pControlPoint = NULL;
		}
		OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
	}
	
	// Need to translate other interpolators!!!
	pInterp = pAAF->CreateInterpolationDefinition(pDictionary, LinearInterpolator);
	pVaryingValue->SetInterpolationDefinition(pInterp);

	DecIndentLevel();
}

// ============================================================================
// ConvertOMFNestedScope
//
//			This function extracts all the properties of an OMF Nested scope
//			segment, and converts it into an AAF Nested scope object.
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFNestedScope(OMF2::omfSegObj_t segment,
									   IAAFNestedScope* pNestedScope)
{


	IncIndentLevel();

	OMF2::omfDDefObj_t		nsDatakind;
	OMF2::omfLength_t		nsLength;
	OMFCheck OMFError = OMF2::omfiNestedScopeGetInfo(OMFFileHdl, segment, &nsDatakind, &nsLength);

	OMF2::omfNumSlots_t		numSlots;
	OMFError = OMF2::omfiNestedScopeGetNumSlots(OMFFileHdl, segment, &numSlots);
	gpGlobals->pLogger->Log( kLogInfo, 
			"%sProcessing Nested Scope of length = %ld and %ld slots\n ", 
			gpGlobals->indentLeader, (int)nsLength, (int) numSlots);

	// Set Nested Scope Component properties.
	IAAFDataDef * pDataDef = 0;
	ConvertOMFDataDef(nsDatakind, &pDataDef);
	IAAFComponent*			pSegmentComp;
	AAFCheck rc = pNestedScope->QueryInterface(IID_IAAFComponent, (void **)&pSegmentComp);
	AutoRelease<IAAFComponent> pseg( pSegmentComp );

	rc = pSegmentComp->SetDataDef(pDataDef);
	rc = pSegmentComp->SetLength(nsLength);

	if (numSlots > 0)
	{
		OMF2::omfIterHdl_t		OMFIterator;
		OMFError = OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
		for (int i = 0; i < numSlots;i++)
		{
			OMF2::omfSegObj_t		scopeSlot;
			OMFError = OMF2::omfiNestedScopeGetNextSlot(OMFIterator, segment, NULL, &scopeSlot);
			IAAFComponent*	pSlotComp;
			ProcessOMFComponent(scopeSlot, &pSlotComp);
			if( pSlotComp )
			{
				AutoRelease<IAAFComponent> pslot( pSlotComp );
				IAAFSegment*	pSegment;
				rc = pSlotComp->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
				AutoRelease<IAAFSegment> pseg( pSegment );
				rc = pNestedScope->AppendSegment(pSegment);
			}
		}
		OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
	}

	DecIndentLevel();
}
// ============================================================================
// ConvertOMFScopeRef
//
//			This function extracts all the properties of an OMF Scope reference
//			segment, and converts it into an AAF ScopeReference object.
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
void Omf2Aaf::ConvertOMFScopeRef(OMF2::omfSegObj_t segment,
									IAAFScopeReference* pScopeRef)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	OMF2::omfDDefObj_t		srDatakind;
	OMF2::omfLength_t		srLength;
	OMF2::omfUInt32			relScope;
	OMF2::omfUInt32			relSlot;
	
	IAAFComponent*			pSegmentComp = NULL;
	IAAFDataDef * pDataDef = 0;
	
	IncIndentLevel();
	OMFError = OMF2::omfiScopeRefGetInfo(OMFFileHdl, segment, &srDatakind, &srLength, &relScope, &relSlot);
	gpGlobals->pLogger->Log(kLogInfo,
		"%sProcessing Scope reference of length = %ld\n ", gpGlobals->indentLeader, (int)srLength);
	ConvertOMFDataDef(srDatakind, &pDataDef);
	pScopeRef->QueryInterface(IID_IAAFComponent, (void **)&pSegmentComp);
	pSegmentComp->SetDataDef(pDataDef);
	pSegmentComp->SetLength(srLength);
	pSegmentComp->Release();
	pSegmentComp = NULL;
	
	rc = pScopeRef->Create((aafUInt32)relScope, (aafUInt32)relSlot);
	
	
	DecIndentLevel();
}
// ============================================================================
// OMFFileClose
//
//			This function closes the input OMF file
//			
// Returns: None
//
// ============================================================================
void Omf2Aaf::OMFFileClose()
{
	if( gpGlobals->bOMFFileOpen == kAAFTrue )
	{
		OMFCheck check = OMF2::omfsCloseFile(OMFFileHdl);
		OMFFileHdl = NULL;
		check = OMF2::omfsEndSession(OMFSession);
		OMFSession = NULL;
	}
}

// ============================================================================
// AAFFileClose
//
//			This function closes the output AAF file
//			
// Returns: None
//
// ============================================================================
void Omf2Aaf::AAFFileClose( )
{
	if (pDictionary)
	{
		pDictionary->Release();
		pDictionary = NULL;
	}

	if (pHeader)
	{
		pHeader->Release();
		pHeader = NULL;
	}
	
	if (pFile)
	{
		pFile->Save();
		pFile->Close();
		pFile->Release();
		pFile = NULL;
	}
}

// ============================================================================
// ConvertOMFEffects
//
//			This function reads an OMF effect object, converts its properties
//			to AAF, updates the AAF Effect object and, if necessary creates the 
//			effect definition by Calling ConvertOMFEffectDefinition. 
//			
// Returns: AAFRESULT_SUCCESS if object is converted.
//
// ============================================================================
void Omf2Aaf::ConvertOMFEffects(OMF2::omfEffObj_t	effect,
								   IAAFOperationGroup*	pEffect)
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	OMF2::omfDDefObj_t		effectDatakind;
	OMF2::omfSegObj_t		effectSegment = NULL;
	OMF2::omfSegObj_t		inputSegmentA ;
	OMF2::omfSegObj_t		inputSegmentB ;
	OMF2::omfSegObj_t		levelSegment;
	OMF2::omfSegObj_t		renderClip = NULL;
	OMF2::omfESlotObj_t		effectSlot = NULL;
	OMF2::omfLength_t		effectLength;
	OMF2::omfDDefObj_t		effectDef;
	OMF2::omfInt32			nameSize = 64, idSize = 64;
	OMF2::omfUniqueName_t	effectID;
	OMF2::omfUniqueName_t	MCEffectID;
	char					effectDefName[64];
	OMF2::omfRational_t		speedRatio;
	OMF2::omfUInt32			phaseOffset = 0;
	OMF2::omfUInt32			mask = 0;
	OMF2::omfFxFrameMaskDirection_t addOrDrop = OMF2::kOmfFxFrameMaskDirNone;
	OMF2::omfArgIDType_t	bypassOverride;
	OMF2::omfInt32			wipeNumber;
	OMF2::omfWipeArgs_t		wipeArgs;
	OMF2::omfErr_t			testErr;

	IAAFOperationDef*		pEffectDef = NULL;
	IAAFComponent*			pEffectSegment = NULL;
	IAAFComponent*			pRenderSegment = NULL;
	IAAFSourceReference*	pSourceRef= NULL;
	IAAFSegment*			pSegment = NULL;
	IAAFParameter*			pParameter = NULL;
	IAAFParameterDef*		pParameterDef = NULL;
	IAAFTypeDef*			typeDef = NULL;

	IAAFDataDefSP  effectDataDef;
	aafUID_t				effectDefAUID ;
//	aafUID_t				AAFEffectID;
//	aafInt32				keyFrameSlot, globalSlot;
	//	OMF2::omfSegObj_t		segment;

	IncIndentLevel();

	CAAFBuiltinDefs defs (pDictionary);

	if (OMF2::kOmfRev2x == OMFFileRev)
	{
		OMFError = OMF2::omfiEffectGetInfo(OMFFileHdl, effect, &effectDatakind, &effectLength, &effectDef);

		gpGlobals->pLogger->Log( kLogInfo,
			"%sProcessing Effect of length = %ld\n ", gpGlobals->indentLeader, (unsigned int)effectLength);

		OMFError = OMF2::omfiEffectDefGetInfo(OMFFileHdl, effectDef, idSize, effectID, nameSize, effectDefName,
									0, NULL, NULL, NULL);
		if (OMF2::OM_ERR_PROP_NOT_PRESENT == OMF2::omfiEffectGetBypassOverride(OMFFileHdl, effect, &bypassOverride))
			bypassOverride = 0;
		// Get the AAF Effect definition interface pointer !!
		if(OMF2::omfsReadString(OMFFileHdl, effect, gpGlobals->pvtEffectIDProp,
									MCEffectID, sizeof(MCEffectID)) == OMF2::OM_ERR_NONE)
			(void)pEffectTranslate->GetAAFEffectID(effectID, MCEffectID, &effectDefAUID);
		else
			(void)pEffectTranslate->GetAAFEffectID(effectID, NULL, &effectDefAUID);

		ConvertOMFDataDef(effectDatakind, &effectDataDef);
		ConvertOMFEffectDefinition(effectDef, effect, &pEffectDef);

		if (strcmp(effectID, "omfi:effect:VideoSpeedControl") == 0)
		{
			//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
			//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			GetParameterDefinition((aafUID_t *)&kAAFParameterDefSpeedRatio, NULL, 
				L"Speed Ratio", 
				L"Defines the ratio of output length to input length. Range is -infinity to +infinity",
				L" ",
				&pParameterDef);
			pEffectDef->AddParameterDef(pParameterDef);
			OMFError = OMF2::omfeVideoSpeedControlGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &speedRatio, &phaseOffset);
			if (inputSegmentA)
			{
				ProcessOMFComponent(inputSegmentA, &pEffectSegment);
				if( pEffectSegment )
				{
					pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
					pEffect->AppendInputSegment(pSegment);
					pSegment->Release();
					pEffectSegment->Release();
					pSegment = NULL;
					pEffectSegment = NULL;
				}
			}
		}
		else if (strcmp(effectID, "omfi:effect:VideoFrameMask") == 0)
		{
			//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
			//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			OMFError = OMF2::omfeVideoFrameMaskGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &mask, &addOrDrop, &phaseOffset);
			if (inputSegmentA)
			{
				ProcessOMFComponent(inputSegmentA, &pEffectSegment);
				if( pEffectSegment )
				{
					pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
					pEffect->AppendInputSegment(pSegment);
					pSegment->Release();
					pEffectSegment->Release();
					pSegment = NULL;
					pEffectSegment = NULL;
				}
			}
		}
		else if (strcmp(effectID, "omfi:effect:VideoRepeat") == 0)
		{
//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			GetParameterDefinition((aafUID_t *)&kAAFParameterDefPhaseOffset, NULL, 
										L"PhaseOffset", 
										L"Must be a constant Value. Default is 0",
										L" ",
										&pParameterDef);
			pEffectDef->AddParameterDef(pParameterDef);

			if(OMF2::OM_ERR_NONE == OMF2::omfeVideoRepeatGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &phaseOffset))
			{
				if (inputSegmentA)
				{
					ProcessOMFComponent(inputSegmentA, &pEffectSegment);
					if( pEffectSegment )
					{
						pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
						pEffect->AppendInputSegment(pSegment);
						pSegment->Release();
						pEffectSegment->Release();
						pSegment = NULL;
						pEffectSegment = NULL;
					}
				}
				if (phaseOffset > 0)
				{
					IAAFConstantValue* pConstantValue = NULL;
		
					rc = defs.cdConstantValue()->
						CreateInstance(IID_IAAFConstantValue,
						(IUnknown **)&pConstantValue);
					pConstantValue->SetValue(sizeof(phaseOffset), (unsigned char *)&phaseOffset);
					rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					pConstantValue->Release();
					pConstantValue = NULL;
					if (pParameter)
					{
						pParameter->SetParameterDefinition(pParameterDef);

						pDictionary->LookupTypeDef(kAAFTypeID_Int32, &typeDef);
						pParameter->SetTypeDefinition(typeDef);
						typeDef->Release();
						typeDef = NULL;

						pEffect->AddParameter(pParameter);
						pParameter->Release();
						pParameter = NULL;
					}
				}
			}
			pParameterDef->Release();
			pParameterDef = NULL;
		}
		else if ((strcmp(effectID, "omfi:effect:VideoDissolve") == 0) ||
				 (strcmp(effectID, "omfi:effect:SimpleVideoDissolve") == 0) )
		{
//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, NULL, 
										L"Level", 
										L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
										L" ",
										&pParameterDef);
			pEffectDef->AddParameterDef(pParameterDef);

			if(OMF2::OM_ERR_NONE == OMF2::omfeVideoDissolveGetInfo(OMFFileHdl, effect,
				&effectLength, &inputSegmentA, &inputSegmentB, &levelSegment))
			{
				if (inputSegmentA)
				{
					ProcessOMFComponent(inputSegmentA, &pEffectSegment);
					if( pEffectSegment )
					{
						pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
						pEffect->AppendInputSegment(pSegment);
						pSegment->Release();
						pEffectSegment->Release();
						pSegment = NULL;
						pEffectSegment = NULL;
					}
				}
				if (inputSegmentB)
				{
					ProcessOMFComponent(inputSegmentB, &pEffectSegment);
					if( pEffectSegment )
					{
						pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
						pEffect->AppendInputSegment(pSegment);
						pSegment->Release();
						pEffectSegment->Release();
						pSegment = NULL;
						pEffectSegment = NULL;
					}
				}
				if (levelSegment)
				{
					if (OMF2::omfiIsAConstValue(OMFFileHdl, levelSegment, &testErr))
					{
						IAAFConstantValue* pConstantValue = NULL;
						
						rc = defs.cdConstantValue()->
							CreateInstance(IID_IAAFConstantValue,
							(IUnknown **)&pConstantValue);
						ConvertOMFConstValue(levelSegment, pConstantValue);
						rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
						pConstantValue->Release();
						pConstantValue = NULL;
					}
					else if (OMF2::omfiIsAVaryValue(OMFFileHdl, levelSegment, &testErr))
					{
						IAAFVaryingValue* pVaryingValue = NULL;
						
						rc = defs.cdVaryingValue()->
							CreateInstance(IID_IAAFVaryingValue,
							(IUnknown **)&pVaryingValue);
						ConvertOMFVaryingValue(levelSegment, pVaryingValue);
						rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
						pVaryingValue->Release();
						pVaryingValue = NULL;
					}
					if (pParameter)
					{
						pParameter->SetParameterDefinition(pParameterDef);

						pDictionary->LookupTypeDef(kAAFTypeID_Rational, &typeDef);
						pParameter->SetTypeDefinition(typeDef);
						typeDef->Release();
						typeDef = NULL;
						
						pEffect->AddParameter(pParameter);
						pParameter->Release();
						pParameter = NULL;
					}
				}
			}
			pParameterDef->Release();
			pParameterDef = NULL;
		}
		else if (strcmp(effectID, "omfi:effect:SMPTEVideoWipe") == 0)
		{
			//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
			//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			wipeNumber = 0;
			memset(&wipeArgs, 0, sizeof(wipeArgs));
			GetParameterDefinition((aafUID_t *)&kAAFParameterDefSMPTEWipeNumber, NULL, 
				L"Wipe Number", 
				L"SMPTE Wipe Number. No Default",
				L" ",
				&pParameterDef);
			pEffectDef->AddParameterDef(pParameterDef);
			
			OMFError = OMF2::omfeSMPTEVideoWipeGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &inputSegmentB, &levelSegment, &wipeNumber, &wipeArgs);
			if (wipeNumber)
			{
				IAAFConstantValue* pConstantValue = NULL;
				
				rc = defs.cdConstantValue()->
					CreateInstance(IID_IAAFConstantValue,
					(IUnknown **)&pConstantValue);
				pConstantValue->SetValue(sizeof(wipeNumber), (unsigned char *)&wipeNumber);
				rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
				pEffect->AddParameter(pParameter);
				pParameter->SetParameterDefinition(pParameterDef);
				pDictionary->LookupTypeDef(kAAFTypeID_Int32, &typeDef);
				pParameter->SetTypeDefinition(typeDef);
				typeDef->Release();
				typeDef = NULL;
				
				pConstantValue->Release();
				pConstantValue = NULL;
				
				
				pParameter->Release();
				pParameter = NULL;
			}
			pParameterDef->Release();
			pParameterDef = NULL;
			
			//
			IAAFConstantValue* pConstantValue = NULL;
			unsigned char	reverse = wipeArgs.reverse;
			GetParameterDefinition((aafUID_t *)&kAAFParameterDefSMPTEReverse, NULL, 
				L"Reverse", 
				L"SMPTE Reverse. Default FALSE",
				L" ",
				&pParameterDef);
			pEffectDef->AddParameterDef(pParameterDef);
			rc = defs.cdConstantValue()->
				CreateInstance(IID_IAAFConstantValue,
				(IUnknown **)&pConstantValue);
			pConstantValue->SetValue(sizeof(reverse), &reverse);
			rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
			pEffect->AddParameter(pParameter);
			pParameter->SetParameterDefinition(pParameterDef);
			pDictionary->LookupTypeDef(kAAFTypeID_Boolean, &typeDef);
			pParameter->SetTypeDefinition(typeDef);
			typeDef->Release();
			typeDef = NULL;
			
			pConstantValue->Release();
			pConstantValue = NULL;
			
			pParameter->Release();
			pParameter = NULL;
			pParameterDef->Release();
			pParameterDef = NULL;
			
			//
			// port other wipeArgs over
			
			//
			if (inputSegmentA)
			{
				ProcessOMFComponent(inputSegmentA, &pEffectSegment);
				if( pEffectSegment )
				{
					pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
					pEffect->AppendInputSegment(pSegment);
					pSegment->Release();
					pEffectSegment->Release();
					pSegment = NULL;
					pEffectSegment = NULL;
				}
			}
			if (inputSegmentB)
			{
				ProcessOMFComponent(inputSegmentB, &pEffectSegment);
				if( pEffectSegment )
				{
					pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
					pEffect->AppendInputSegment(pSegment);
					pSegment->Release();
					pEffectSegment->Release();
					pSegment = NULL;
					pEffectSegment = NULL;
				}
			}
			if (levelSegment)
			{
				if (OMF2::omfiIsAConstValue(OMFFileHdl, levelSegment, &testErr))
				{
					IAAFConstantValue* pConstantValue = NULL;
					
					rc = defs.cdConstantValue()->
						CreateInstance(IID_IAAFConstantValue,
						(IUnknown **)&pConstantValue);
					ConvertOMFConstValue(levelSegment, pConstantValue);
					rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					pConstantValue->Release();
					pConstantValue = NULL;
				}
				else if (OMF2::omfiIsAVaryValue(OMFFileHdl, levelSegment, &testErr))
				{
					IAAFVaryingValue* pVaryingValue = NULL;
					
					rc = defs.cdVaryingValue()->
						CreateInstance(IID_IAAFVaryingValue,
						(IUnknown **)&pVaryingValue);
					ConvertOMFVaryingValue(levelSegment, pVaryingValue);
					rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					pVaryingValue->Release();
					pVaryingValue = NULL;
				}
				GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, NULL, 
					L"Level", 
					L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
					L" ",
					&pParameterDef);
				pParameter->SetParameterDefinition(pParameterDef);
				
				pDictionary->LookupTypeDef(kAAFTypeID_Rational, &typeDef);
				pParameter->SetTypeDefinition(typeDef);
				typeDef->Release();
				typeDef = NULL;
				
				pEffect->AddParameter(pParameter);
				pEffectDef->AddParameterDef(pParameterDef);
				pParameterDef->Release();
				pParameterDef = NULL;
				pParameter->Release();
				pParameter = NULL;
			}
		}
		else if ((strcmp(effectID, "omfi:effect:MonoAudioDissolve") == 0) ||
				 (strcmp(effectID, "omfi:effect:SimpleMonoAudioDissolve") == 0))
		{
//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, NULL, 
										L"Level", 
										L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
										L" ",
										&pParameterDef);
			pEffectDef->AddParameterDef(pParameterDef);

			if(OMF2::OM_ERR_NONE == OMF2::omfeMonoAudioDissolveGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &inputSegmentB, &levelSegment))
			{
				if (inputSegmentA)
				{
					ProcessOMFComponent(inputSegmentA, &pEffectSegment);
					if( pEffectSegment )
					{
						pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
						pEffect->AppendInputSegment(pSegment);
						pSegment->Release();
						pEffectSegment->Release();
						pSegment = NULL;
						pEffectSegment = NULL;
					}
				}
				if (inputSegmentB)
				{
					ProcessOMFComponent(inputSegmentB, &pEffectSegment);
					if( pEffectSegment )
					{
						pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
						pEffect->AppendInputSegment(pSegment);
						pSegment->Release();
						pEffectSegment->Release();
						pSegment = NULL;
						pEffectSegment = NULL;
					}
				}
				if (levelSegment)
				{
					if (OMF2::omfiIsAConstValue(OMFFileHdl, levelSegment, &testErr))
					{
						IAAFConstantValue* pConstantValue = NULL;
						
						rc = defs.cdConstantValue()->
							CreateInstance(IID_IAAFConstantValue,
							(IUnknown **)&pConstantValue);
						ConvertOMFConstValue(levelSegment, pConstantValue);
						rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
						pConstantValue->Release();
						pConstantValue = NULL;
					}
					else if (OMF2::omfiIsAVaryValue(OMFFileHdl, levelSegment, &testErr))
					{
						IAAFVaryingValue*	pVaryingValue = NULL;
						
						rc = defs.cdVaryingValue()->
							CreateInstance(IID_IAAFVaryingValue,
							(IUnknown **)&pVaryingValue);
						ConvertOMFVaryingValue(levelSegment, pVaryingValue);
						rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
						pVaryingValue->Release();
						pVaryingValue = NULL;
					}
					pParameter->SetParameterDefinition(pParameterDef);

					pDictionary->LookupTypeDef(kAAFTypeID_Rational, &typeDef);
					pParameter->SetTypeDefinition(typeDef);
					typeDef->Release();
					typeDef = NULL;
					
					pEffect->AddParameter(pParameter);
					pParameter->Release();
					pParameter = NULL;
				}
			}
			else
				OMFError = OMF2::OM_ERR_NONE;

			pParameterDef->Release();
			pParameterDef = NULL;
		}
		else if (strcmp(effectID, "omfi:effect:MonoAudioGain") == 0)
//		else if(0 /*processUnknownEffects*/)	// Add a flag to this routine, false when called from a subclass
		{
			// --------------------------------------------------------------
			//			Generic OMF 2.x Effect processing !!! 
			// 
			//	Each Argument represents a parameter and should be checked 
			//  to see if it is Constant value or a Varying value
			//  Then it should be added to the effect parameter list.
			//	Interpretation of each arguments depends on the type of effect
			// ---------------------------------------------------------------
			OMF2::omfIterHdl_t		OMFIterator;
			OMF2::omfESlotObj_t		OMFEffectSlot;
			OMF2::omfArgIDType_t	argID;
			OMF2::omfSegObj_t		argValue;
			OMF2::omfNumSlots_t		numSlots;

//			keyFrameSlot = OMF2_EFFE_ALLOTHERS_KEYFRAME_SLOT;
//			globalSlot = OMF2_EFFE_ALLOTHERS_GLOBAL_SLOT;
			gpGlobals->pLogger->Log( kLogInfo,
				"%sGeneric OMF Effect = %s\n ", gpGlobals->indentLeader, effectID);
			// this is just as default parameter definition !!!
			GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, NULL, 
										L"Level", 
										L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
										L" ",
										&pParameterDef);

			pEffectDef->AddParameterDef(pParameterDef);
		
			OMFError = OMF2::omfiEffectGetNumSlots(OMFFileHdl, effect, &numSlots);;
			if (numSlots > 0)
			{
				OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
				OMFError = OMF2::omfiEffectGetNextSlot(OMFIterator, effect, NULL, &OMFEffectSlot);
				while(OMFEffectSlot != NULL)
				{
					OMFError = OMF2::omfiEffectSlotGetInfo(OMFFileHdl, OMFEffectSlot, &argID, &argValue);
					if (argValue /* && (argID != keyFrameSlot) */)
					{
						if (OMF2::omfiIsAConstValue(OMFFileHdl, argValue, &testErr))
						{
							IAAFConstantValue* pConstantValue = NULL;
							
							rc = defs.cdConstantValue()->
								CreateInstance(IID_IAAFConstantValue,
								(IUnknown **)&pConstantValue);
							ConvertOMFConstValue(argValue, pConstantValue);
							rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
							pConstantValue->Release();
							pConstantValue = NULL;
							if (pParameter)
							{
								pParameter->SetParameterDefinition(pParameterDef);
								
								//!!! Set parameter type (see other parameter)
								pEffect->AddParameter(pParameter);
								pParameter->Release();
								pParameter = NULL;
							}
						}
						else if (OMF2::omfiIsAVaryValue(OMFFileHdl, argValue, &testErr))
						{
							IAAFVaryingValue*	pVaryingValue = NULL;
							
							rc = defs.cdVaryingValue()->
								CreateInstance(IID_IAAFVaryingValue,
								(IUnknown **)&pVaryingValue);
							ConvertOMFVaryingValue(argValue, pVaryingValue);
							rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
							pVaryingValue->Release();
							pVaryingValue = NULL;
							if (pParameter)
							{
								pParameter->SetParameterDefinition(pParameterDef);
								//!!! Set parameter type (see other parameter)
								pEffect->AddParameter(pParameter);
								pParameter->Release();
								pParameter = NULL;
							}
						}
						else
						{
							ProcessOMFComponent(argValue, &pEffectSegment);
							if( pEffectSegment )
							{
								pEffectSegment->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
								pEffect->AppendInputSegment(pSegment);
								pSegment->Release();
								pEffectSegment->Release();
								pSegment = NULL;
								pEffectSegment = NULL;
							}
						}
					}
					(void)OMF2::omfiEffectGetNextSlot(OMFIterator, effect, NULL, &OMFEffectSlot);
				}
				OMFError = OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
			}
			if (pParameterDef)
				pParameterDef->Release();
			
		}

	}
	else
	{
		// Handle 1.x effects
		char				classID[5];

		OMFError = OMF2::omfsReadClassID(OMFFileHdl, effect, OMF2::OMObjID, classID);
		classID[4] = '\0';
		gpGlobals->pLogger->Log( kLogInfo,
			"%sOMF 1.x Effect = %s\n ", gpGlobals->indentLeader, classID);
	}


	pEffect->Initialize(effectDataDef, (aafLength_t)effectLength, pEffectDef);
	pEffect->SetBypassOverride(bypassOverride);
	
	if(OMF2::omfiEffectGetFinalRender(OMFFileHdl, effect, &renderClip) == OM_ERR_NONE)
	{
		ProcessOMFComponent(renderClip, &pRenderSegment);
		if (pRenderSegment)
		{
			pRenderSegment->QueryInterface(IID_IAAFSourceReference, (void **)&pSourceRef);
			pEffect->SetRender(pSourceRef);
			pSourceRef->Release();
		}
	}	 
	
	if (pEffectDef)
		pEffectDef->Release();

	if (pEffectSegment)
		pEffectSegment->Release();

	if (pRenderSegment)
		pRenderSegment->Release();
	
	DecIndentLevel();
}
// ============================================================================
// ConvertOMFEffectDefinition
//
//			This function Converts OMF effect definitions into AAF Operation 
//			definition object. 
//			
// Returns: None
//
// ============================================================================
void Omf2Aaf::ConvertOMFEffectDefinition(OMF2::omfDDefObj_t	effectDef,
											OMF2::omfObject_t effect,
											IAAFOperationDef**	ppEffectDef)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	
	OMF2::omfArgIDType_t	bypassOverride;
	OMF2::omfBool			isTimeWarp;
	OMF2::omfInt32			nameSize = 64, idSize = 64, descSize = 120;
	OMF2::omfUniqueName_t	effectID, MCEffectID;
	char					effectName[64], descBuffer[120];
	
	aafUID_t				effectDataDef;
	aafInt32				numberInputs;
	
	
	OMFError = OMF2::omfiEffectDefGetInfo(OMFFileHdl,
		effectDef,
		idSize, effectID,
		nameSize, effectName,
		descSize, descBuffer,
		&bypassOverride,
		&isTimeWarp);
	//
	// Number of Inputs and data def depends on the type of effect 
	//
	
	if ((strcmp(effectID, "omfi:effect:SimpleMonoAudioDissolve") == 0) ||
		(strcmp(effectID, "omfi:effect:MonoAudioDissolve") == 0) )
	{
		numberInputs = 2;
		effectDataDef = DDEF_Sound;
	}
	else if (strcmp(effectID, "omfi:effect:MonoAudioGain") == 0)
	{
		numberInputs = 1;
		effectDataDef = DDEF_Sound;
	}
	else if (strcmp(effectID, "omfi:effect:MonoAudioMixdown") == 0)
	{
		numberInputs = -1;
		effectDataDef = DDEF_Sound;
	}
	else if (strcmp(effectID, "omfi:effect:MonoAudioPan") == 0)
	{
		numberInputs = 1;
		effectDataDef = DDEF_Sound;
	}
	else if (strcmp(effectID, "omfi:effect:SMPTEVideoWipe") == 0)
	{	
		numberInputs = 2;
		effectDataDef = DDEF_Picture;
	}
	else if ((strcmp(effectID, "omfi:effect:SimpleVideoDissolve") == 0) ||
		(strcmp(effectID, "omfi:effect:VideoDissolve") == 0) )
	{
		numberInputs = 2;
		effectDataDef = DDEF_PictureWithMatte;
	}
	else if (strcmp(effectID, "omfi:effect:VideoFadeToBlack") == 0)
	{
		numberInputs = 1;
		effectDataDef = DDEF_PictureWithMatte;
	}
	else if (strcmp(effectID, "omfi:effect:VideoFrameMask") == 0)
	{
		numberInputs = 1;
		effectDataDef = DDEF_PictureWithMatte;
	}
	else if (strcmp(effectID, "omfi:effect:VideoRepeat") == 0)
	{
		numberInputs = 1;
		effectDataDef = DDEF_PictureWithMatte;
	}
	else if (strcmp(effectID, "omfi:effect:VideoSpeedControl") == 0)
	{
		numberInputs = 1;
		effectDataDef = DDEF_PictureWithMatte;
	}
	else
	{
		numberInputs = -1;
		effectDataDef = DDEF_Picture;
	}
	
	if(OMF2::omfsReadString(OMFFileHdl, effect, gpGlobals->pvtEffectIDProp,
								MCEffectID, sizeof(MCEffectID)) != OMF2::OM_ERR_NONE)
								MCEffectID[0] ='\0';
	GetAAFOperationDefinition( effectID,
		MCEffectID,
		effectName, 
		descBuffer,
		(aafUInt32)bypassOverride,
		(aafBool)isTimeWarp, 
		numberInputs,
		effectDataDef,
		ppEffectDef);
	
}
// ============================================================================
// GetParameterDefinition
//
//			This function Creates, if necessary, a Parameter Definition object.
//			Sets its property and returns its to the caller.  
//	
//			It is the responsibility of the calling function to release the 
//			interface when finished.
//
// Returns: 
//
// ============================================================================
void Omf2Aaf::GetParameterDefinition(aafUID_t* pDefUID, 
										IAAFTypeDef* pTypeDef,
										aafWChar* pwName, 
										aafWChar* pwDesc, 
										aafWChar* pwDisplayUnits,
										IAAFParameterDef** ppParameterDef)
{
	AAFCheck				rc;

//	IAAFTypeDef*		ptmpTypeDef = NULL;
	IAAFParameterDef*	ptmpParameterDef = NULL;

	// Verify that we did not got any NULL pointers
//	if ( (pDefUID == NULL) || (pTypeDef == NULL) || 
	if ( (pDefUID == NULL) ||  
		 (pwName == NULL) || (pwDesc == NULL) ||
		 (pwDisplayUnits == NULL) || (ppParameterDef == NULL))
		 rc = AAFRESULT_NULL_PARAM;

	CAAFBuiltinDefs defs (pDictionary);

	// First verify if the Parameter Def exists already !
	if (FAILED(pDictionary->LookupParameterDef(*pDefUID, &ptmpParameterDef)))
	{
		// Create a new Parameter definition
		defs.cdParameterDef()->
		  CreateInstance(IID_IAAFParameterDef,
						 (IUnknown **) &ptmpParameterDef);
		ptmpParameterDef->Initialize(*pDefUID, pwName, pwDesc);
		ptmpParameterDef->SetDisplayUnits(pwDisplayUnits);
//		ptmpParameterDef->SetTypeDef(pTypeDef);
		rc = pDictionary->RegisterParameterDef(ptmpParameterDef);
	}

	*ppParameterDef = ptmpParameterDef;
}

// ============================================================================
// GetAAFOperationDefinition
//
//			This function creates or retrieves from the Dictionary the Operation 
//			definition object.  The dictionary is searched to see if it has 
//			already been created and registered. If so a pointer to the 
//			Operation Definition Interface is returned in the ppEffectDef 
//			argument. If the Definition does not exists it is created. 
//			
// Returns: None
//
// ============================================================================
void Omf2Aaf::GetAAFOperationDefinition(OMF2::omfUniqueName_t effectID,
										   OMF2::omfUniqueName_t MCEffectID,
										   char* defName, 
										   char* defDescription, 
										   aafUInt32 bypassOverride, 
										   aafBool isTimeWarp,
										   aafInt32 numberInputs,
										   aafUID_t defDataDef,
										   IAAFOperationDef** ppEffectDef)
{
	AAFCheck				rc;

    aafWChar*			pwCategory = NULL;
	aafWChar*			pwDesc = NULL;
	aafWChar*			pwName = NULL;
	aafUID_t			effectDefAUID ;
	char				*src = "Not Specified";

	pwCategory = new wchar_t[strlen(src)+1];
	mbstowcs(pwCategory, src, strlen(src)+1);
	pwDesc = new wchar_t[strlen(defDescription)+1];
	mbstowcs(pwDesc, defDescription, strlen(defDescription)+1);
	pwName = new wchar_t[strlen(defName)+1];
	mbstowcs(pwName, defName, strlen(defName)+1);

	CAAFBuiltinDefs defs (pDictionary);

	(void)pEffectTranslate->GetAAFEffectID(effectID, MCEffectID, &effectDefAUID);

	// Look in the dictionary to find if the effect Definition exists
	// if it exists use it.
	if (FAILED(pDictionary->LookupOperationDef(effectDefAUID, ppEffectDef)))
	{
		defs.cdOperationDef()->
		  CreateInstance(IID_IAAFOperationDef,
						 (IUnknown **) ppEffectDef);
		(*ppEffectDef)->Initialize(effectDefAUID, pwName, pwDesc);
		pDictionary->RegisterOperationDef(*ppEffectDef);
		(*ppEffectDef)->SetIsTimeWarp((aafBool)isTimeWarp);
		(*ppEffectDef)->SetCategory(pwName);
		(*ppEffectDef)->SetBypass((aafUInt32 )bypassOverride);
		// Set degradeTo to itself for now because we do not have optional properties !!!
// 		(*ppEffectDef)->AppendDegradeToOperations(*ppEffectDef);
		(*ppEffectDef)->SetNumberInputs(numberInputs);
		IAAFDataDefSP pDataDef;
		pDictionary->LookupDataDef (defDataDef, &pDataDef);
		(*ppEffectDef)->SetDataDef(pDataDef); 

		rc = AAFRESULT_SUCCESS;
	}

	if (pwName)
		delete [] pwName;
	if (pwDesc)
		delete [] pwDesc;
	if (pwCategory)
		delete [] pwCategory;
}


void Omf2Aaf::ConvertObjectProps(OMF2::omfObject_t pOMFObject, aafUID_t &classID, IAAFObject* pObj)
{
}		
