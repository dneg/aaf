// @doc INTERNAL
// @com This file implements the conversion of OMF files to AAF file format.
//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

// OMF Includes
#include "omPublic.h"
#include "omMedia.h"
#include "omEffect.h"
#include "omDefs.h"
#include "omUtils.h"

#include "AafOmf.h"
#define OMFI_ENABLE_SEMCHECK	1		// Move to project


#include "AAFTypes.h"
#include "AAFCodecDefs.h"

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
#include "Omf2Aaf.h"
#include "EffectTranslate.h"
#include "AAFDomainUtils.h"
#include "AAFException.h"
#include "OMFException.h"
#include "AutoRelease.h"
#include "AAFClassDefUIDs.h" 

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFDataDef> IAAFDataDefSP;
typedef IAAFSmartPointer<IAAFComponent> IAAFComponentSP;

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

const aafUID_t ProductID = { 0x3B4794A1, 0x299C, 0x11d3, { 0x80, 0xAE, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

extern AafOmfGlobals* gpGlobals;
// ============================================================================
// Constructor
// ============================================================================
Omf2Aaf::Omf2Aaf( AAFDomainUtils *aafDomainUtils, OMFDomainUtils *omfDomainUtils, EffectTranslate *effectTranslate  ) : 
pFile(NULL), pHeader(NULL), pDictionary(NULL), pAAF(aafDomainUtils), pOMF(omfDomainUtils), pEffectTranslate(effectTranslate)
{
	OMFSession = 0;
	OMFFileHdl = 0;
	OMFFileRev = kOmfRev1x;
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
		ret = omfsBeginSession(0, &OMFSession);
		bSessionStarted = kAAFTrue;
		ret = omfmInit(OMFSession);
		ret = omfsOpenFile((fileHandleType)pFileName, OMFSession, &OMFFileHdl);
		gpGlobals->bOMFFileOpen = kAAFTrue;
	}
	catch( ExceptionBase )
	{
		if (bSessionStarted)
		{
			omfsEndSession(OMFSession);
		}
		throw;
	}

	RegisterCodecProperties(gpGlobals, OMFSession);
	pOMF->RegisterOMFProperties(gpGlobals, OMFSession);

	ret = omfsFileGetRev(OMFFileHdl, &OMFFileRev);
	int revision = (kOmfRev2x == OMFFileRev) ? 2 : 1;
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

	aafWChar*	pwFileName = new wchar_t[strlen(pFileName)+1];
	mbstowcs(pwFileName, pFileName, strlen(pFileName)+1);

	// Get Identification from OMF Header 
	// We going to use the last identification entry in the
	// Identification array..
	omfObject_t		OMFHeadObject;
	OMFError = omfsGetHeadObject(OMFFileHdl, &OMFHeadObject);
	aafInt32 nOMFIdentifications = omfsLengthObjRefArray(OMFFileHdl, OMFHeadObject, OMHEADIdentList);
	if (nOMFIdentifications >= 1) 
	{
		omfProductVersion_t OMFVersion;
		omfObject_t		OMFIdent;
		char	text[256];

		OMFError = omfsGetNthObjRefArray(OMFFileHdl, OMFHeadObject, OMHEADIdentList,
										&OMFIdent, nOMFIdentifications);
		if(omfsReadString(OMFFileHdl, OMFIdent, OMIDNTCompanyName, text, sizeof(text)) != OM_ERR_NONE)
			strcpy(text, "<Not Specified>");
		pwCompanyName = new wchar_t[strlen(text)+1];
		mbstowcs(pwCompanyName, text, strlen(text)+1);
		ProductInfo.companyName = pwCompanyName;
		if(omfsReadString(OMFFileHdl, OMFIdent, OMIDNTProductName, text, sizeof(text)) != OM_ERR_NONE)
			strcpy(text, "<Not Specified>");
		pwProductName = new wchar_t[strlen(text)+1];
		mbstowcs(pwProductName, text, strlen(text)+1);
		ProductInfo.productName = pwProductName;
		if(omfsReadString(OMFFileHdl, OMFIdent, OMIDNTProductVersionString, text, sizeof(text)) != OM_ERR_NONE)
			strcpy(text, "<Not Specified>");
		pwProductVersionString =  new wchar_t[strlen(text)+1];
		mbstowcs(pwProductVersionString, text, strlen(text)+1);
		ProductInfo.productVersionString = pwProductVersionString;
		if (omfsReadProductVersionType(OMFFileHdl, OMFIdent, OMIDNTProductVersion, &OMFVersion) != OM_ERR_NONE)
		{
			OMFVersion.major = 0;
			OMFVersion.minor = 0;
			OMFVersion.tertiary = 0;
			OMFVersion.patchLevel = 0;
			OMFVersion.type = kVersionUnknown;
		}
		aafProductVersion_t v;
		v.major = OMFVersion.major;
		v.minor = OMFVersion.minor;
		v.tertiary = OMFVersion.tertiary;
		v.patchLevel = OMFVersion.type;
		v.type = (aafProductReleaseType_t)OMFVersion.type;
		ProductInfo.productVersion = &v;

		if(omfsReadString(OMFFileHdl, OMFIdent, OMIDNTPlatform, text, sizeof(text)) != OM_ERR_NONE)
			strcpy(text, "<Not Specified>");
		pwPlatform =  new wchar_t[strlen(text)+1];
		mbstowcs(pwPlatform, text, strlen(text)+1);
		ProductInfo.platform = pwPlatform;
		rc = AAFFileOpenNewModify(pwFileName, 0, &ProductInfo, &pFile);
		bAddExtraIdent = kAAFTrue;

		delete []  pwCompanyName;
		delete []  pwProductName;
		delete []  pwProductVersionString;
		delete [] pwPlatform;
	}
	else
	{
		aafProductVersion_t v;
		v.major = 1;
		v.minor = 0;
		v.tertiary = 0;
		v.patchLevel = 0;
		v.type = kAAFVersionDebug;
		ProductInfo.companyName = L"Company Name";
		ProductInfo.productName = L"AAF/OMF File Conversion";
		ProductInfo.productVersion = &v;
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
		aafProductVersion_t v;
		v.major = 1;
		v.minor = 0;
		v.tertiary = 0;
		v.patchLevel = 0;
		v.type = kAAFVersionDebug;
		pIdent->Initialize(L"Company Name",
						   L"OMF to AAF File Conversion",
						   L"<Unspecified version>",
						   ProductID);
		pIdent->SetProductVersion(v);
		rc = pHeader->AppendIdentification(pIdent);
	}
	pAAF->RegisterAAFProperties(pDictionary);
	gpGlobals->pLogger->Log(kLogInfo, "AAF File: %s Created succesfully\n", pFileName);
	pAAF->SetDictionary(pDictionary);

	delete [] pwFileName;

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
	omfIterHdl_t		OMFMobIter = NULL;
	omfMobObj_t		OMFMob;
	omfObject_t		OMFHeader, OMFObject;
	omfProperty_t		OMFPropertyID;

	// AAF Variables
	aafInt32				numMedia;
	
	CAAFBuiltinDefs defs (pDictionary);

	OMFError = omfiIteratorAlloc( OMFFileHdl, &OMFMobIter);
	OMFError = omfiGetNumMobs( OMFFileHdl, kAllMob, &nOMFNumMobs);
	gpGlobals->pLogger->Log( kLogInfo, "Found: %ld Mobs in the input file\n", nOMFNumMobs);
	for (nOMFMobCount = 0; nOMFMobCount < nOMFNumMobs; nOMFMobCount++)
	{
		OMFError = omfiGetNextMob(OMFMobIter, NULL, &OMFMob);
		IAAFMob*	pMob = NULL;
		AutoRelease <IAAFMob> pmob;
		gpGlobals->nNumOMFMobs++;
		omfErr_t err; // Dummy
		if (omfiIsACompositionMob(OMFFileHdl, OMFMob, &err ))
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
		else if (omfiIsAMasterMob(OMFFileHdl, OMFMob, &err) )
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
		else if ( omfiIsASourceMob(OMFFileHdl, OMFMob, &err) )
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
			omfClassID_t		objClass;
			char					id[5];
			OMFError = omfsReadClassID(OMFFileHdl, OMFMob, OMOOBJObjClass, objClass);
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

	OMFError = omfiIteratorDispose(OMFFileHdl, OMFMobIter);
	// Now that we have read all the metadata we can get on with
	// the actual essence (media) data

	OMFError = omfsGetHeadObject( OMFFileHdl, &OMFHeader );
	if (kOmfRev2x == OMFFileRev)
	{
		OMFPropertyID = OMHEADMediaData;
		numMedia = omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMFPropertyID); 
	}
	else
	{
		OMFPropertyID = OMMediaData;
		numMedia = omfsLengthObjIndex(OMFFileHdl, OMFHeader, OMFPropertyID);
	}

	gpGlobals->pLogger->Log( kLogInfo, "Found: %ld Essence data objects\n", numMedia);
	for (int k = 1;k <= numMedia; k++)
	{
		omfObjIndexElement_t	objIndex;
		omfUID_t				tmpMediaID;

		if (kOmfRev2x == OMFFileRev)
		{
			OMFError = omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMFPropertyID, &OMFObject, k);
			tmpMediaID.prefix = 0;
			tmpMediaID.major = 0;
			tmpMediaID.minor = 0;
		}
		else
		{
			OMFError = omfsGetNthObjIndex(OMFFileHdl, OMFHeader, OMFPropertyID, &objIndex, k);
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
	omfObject_t		OMFHeader, OMFObject;
	aafInt32				numDefs, numEntries;

	gpGlobals->pLogger->Log(kLogInfo, "Processing OMF Header...\n" );
	OMFError = omfsGetHeadObject( OMFFileHdl, &OMFHeader );
	IncIndentLevel();
	//From the OMF header we will extract all definition Objects and Class Dictionary
	// First we process the Data definitions
	numDefs = omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMHEADDefinitionObjects);
	gpGlobals->pLogger->Log( kLogInfo, "%sFound: %ld Data Definitions\n", 
	gpGlobals->indentLeader, numDefs);
	for (int i = 1;i <= numDefs;i++)
	{
		OMFError = omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMHEADDefinitionObjects, &OMFObject, i);
		// Process the given Data Definition object.
		 ConvertOMFDataDefinitionObject(OMFObject);
	}

	// Now we process the class dictionary
	if (kOmfRev2x == OMFFileRev)
	{
		numEntries = omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMHEADClassDictionary);
	}
	else
	{
		numEntries = omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMClassDictionary);
	}

	gpGlobals->pLogger->Log( kLogInfo, "Found: %ld Class Definitions\n", numEntries);
	for (int j = 1;j <= numEntries; j++)
	{
		if (kOmfRev2x == OMFFileRev)
			OMFError = omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMHEADClassDictionary, &OMFObject, j);
		else
			OMFError = omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMClassDictionary, &OMFObject, j);
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
void Omf2Aaf::ConvertOMFDataDefinitionObject( omfObject_t obj )
{
	IncIndentLevel();

	omfProperty_t		OMFPropertyID;
	if (kOmfRev2x == OMFFileRev)
	{
		OMFPropertyID = OMOOBJObjClass;
	}
	else
	{
		OMFPropertyID = OMObjID;
	}

	omfClassID_t		objClass;
	OMFCheck OMFError;
	OMFError = omfsReadClassID(OMFFileHdl, obj, OMFPropertyID, objClass);
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
void Omf2Aaf::ConvertOMFClassDictionaryObject( omfObject_t obj )
{
	OMFCheck rc;

	IncIndentLevel();

	omfProperty_t		OMFPropertyID;
	if (kOmfRev2x == OMFFileRev)
	{
		OMFPropertyID = OMCLSDClass;
	}
	else
	{
		OMFPropertyID = OMCLSDClassID;
	}

	char id[5];
	memset(id, 0, sizeof(id));
	rc = omfsReadClassID(OMFFileHdl, obj, OMFPropertyID, id);
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
void Omf2Aaf::ConvertOMFMediaDataObject( omfObject_t obj, omfUID_t inMediaID )
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	omfUID_t			mediaID;
	omfProperty_t		idProperty;
	omfProperty_t		OMFPropertyID;
	omfDDefObj_t		datakind;
	omfUniqueName_t	propName;

	IAAFEssenceData*		pEssenceData;
	AutoRelease<IAAFEssenceData> pEssence;
	IAAFMob*				pMob;
	AutoRelease<IAAFMob>	pmob;
	IAAFSourceMob*			pSourceMob;
	AutoRelease<IAAFSourceMob> pSource;
	aafBool					bConvertMedia = kAAFFalse;
	aafBool					bForceUseDataValue = kAAFFalse;

	CAAFBuiltinDefs defs (pDictionary);

	IncIndentLevel();
	if (kOmfRev2x == OMFFileRev)
	{
		OMFPropertyID = OMOOBJObjClass;
		OMFError = omfsReadUID(OMFFileHdl, obj, OMMDATMobID, &mediaID);
	}
	else
	{
		OMFPropertyID = OMObjID;
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
	OMFError = omfsReadClassID(OMFFileHdl, obj, OMFPropertyID, id);
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

		if (kOmfRev2x == OMFFileRev)
		{
			omfErr_t err;
			idProperty = OMTIFFData;
			omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, &err );
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
		if (kOmfRev2x == OMFFileRev)
		{
			omfErr_t err;
			idProperty = OMAIFCData;
			omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, &err );
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
		if (kOmfRev2x == OMFFileRev)
		{
			omfErr_t err;
			idProperty = OMWAVEData;
			omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, &err);
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
//		if (kOmfRev2x == OMFFileRev)
//		{
			omfErr_t err;
			idProperty = OMIDATImageData;
			omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, &err );
			OMFError = err;
//		}
//		else
//		{
//			strcat(propName, "IDAT");
//			strcat(propName, ":ImageData");
//		}
		bConvertMedia = kAAFTrue;
		bForceUseDataValue = kAAFTrue;
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
		omfPosition_t	OMFOffset;

		aafUInt32			numBytes;
		aafUInt32			nBlockSize;
		aafUInt32			numBytesRead;
		aafUInt32			numBytesWritten;
		

		// find out how big the data is 
		if (kOmfRev2x == OMFFileRev)
			numBytes = (aafUInt32)omfsLengthDataValue(OMFFileHdl, obj, idProperty);
		else
		{
			omfUniqueName_t	tmpPropName;
			omfIterHdl_t		propIter = NULL;
			omfProperty_t		tmpProp;
			omfType_t			tmpType;

			OMFError = omfiIteratorAlloc(OMFFileHdl, &propIter);
			omfErr_t err = omfiGetNextProperty(propIter, obj, &tmpProp, &tmpType);		
			while (OM_ERR_NONE == err && tmpProp)
			{
				OMFError = omfiGetPropertyName(OMFFileHdl, tmpProp, OMUNIQUENAME_SIZE, tmpPropName);
				if (strcmp(tmpPropName, propName) == 0)
				{
					idProperty = tmpProp;
					break;
				}
				err = omfiGetNextProperty(propIter, obj, &tmpProp, &tmpType);		
			}
			OMFError = omfiIteratorDispose(OMFFileHdl, propIter);

			if (kOmfRev2x == OMFFileRev || bForceUseDataValue)
				numBytes = (aafUInt32)omfsLengthDataValue(OMFFileHdl, obj, idProperty);
			else
				numBytes = (aafUInt32)omfsLengthVarLenBytes(OMFFileHdl, obj, idProperty);
		}
		if (numBytes > 0)
		{
			if (numBytes > (2 * 1048576))
			{
				nBlockSize = 2 * 1048576;		// only allocate 2 Meg
			}
			else
			{
				nBlockSize = numBytes;
			}
			pBuffer = new char[nBlockSize];
			OMFOffset = 0;
			do 
			{
				if( (numBytes - OMFOffset) < nBlockSize )
				{
					nBlockSize = (aafUInt32)(numBytes - OMFOffset);
				}
				if (kOmfRev2x == OMFFileRev)
				{
					numBytes = (aafUInt32)omfsLengthDataValue(OMFFileHdl, obj, idProperty);
					OMFError = omfsReadDataValue( OMFFileHdl, 
									 					obj,
														idProperty,
														datakind,
														(omfDataValue_t)pBuffer,
														OMFOffset,
														nBlockSize,
														&numBytesRead);
				}
				else
				{
					if(bForceUseDataValue)
					{
						OMFError = OMReadProp(OMFFileHdl, obj, idProperty, 0, kNeverSwab,
											OMDataValue, nBlockSize, pBuffer);
						numBytesRead = nBlockSize;
					}
					else
						OMFError = omfsReadVarLenBytes(OMFFileHdl,
														 obj,
														 idProperty,
														 OMFOffset,
														 nBlockSize,
														 pBuffer,
														 &numBytesRead);
				}

				// write the data
				rc = pEssenceData->SetPosition((aafPosition_t) OMFOffset);
				rc = pEssenceData->Write(numBytesRead, (aafDataBuffer_t)pBuffer, &numBytesWritten);

				// calculate next offset
				OMFOffset += numBytesRead;

			}while (numBytes > OMFOffset );

			Assert( numBytes == OMFOffset );
			if(pBuffer)
				delete [] (char *)pBuffer;
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
void Omf2Aaf::ConvertOMFDatakind( omfDDefObj_t datakind, 
								   aafUID_t * pDatakind)
{
	omfUniqueName_t	datakindName;
	OMFCheck rc;

	rc = omfiDatakindGetName(OMFFileHdl, datakind, 64, datakindName);

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
  (omfDDefObj_t datakind, 
   IAAFDataDef ** ppDataDef)
{
  aafUID_t defUid;
  ConvertOMFDatakind (datakind, &defUid);
  AAFCheck rc;
  rc = pDictionary->LookupDataDef (defUid, ppDataDef);
}

void Omf2Aaf::ConvertOMFDataDefType		// Used for parameter types Integer, Rational, etc...
  (omfDDefObj_t datakind, 
   IAAFTypeDef ** ppTypeDef)
{
	IAAFTypeDef				*pTypeDef = NULL; // initialize return value.
	omfUniqueName_t	datakindName;
	OMFCheck				OMFError;

	OMFError = omfiDatakindGetName(OMFFileHdl, datakind, 64, datakindName);
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
void Omf2Aaf::ConvertOMFMOBObject( omfObject_t obj, IAAFMob* pMob )
{

	IncIndentLevel();

	// Set mob Name
	char					sMobName[64];
	omfUID_t			OMFMobID;
	OMFCheck				OMFError;

	omfErr_t	testErr;
	testErr = omfiMobGetInfo(OMFFileHdl, obj, &OMFMobID, sizeof(sMobName), sMobName, NULL, NULL);
	char *src = (OM_ERR_NONE == testErr) ? sMobName : (char*)"<not named>";
	aafWChar*	pwMobName = new wchar_t[strlen(src)+1];
	mbstowcs(pwMobName, src, strlen(src)+1);
	AAFCheck aafCheck;
	aafCheck = pMob->SetName(pwMobName);
	gpGlobals->pLogger->Log( kLogInfo, "%sMob Name: %s\n", gpGlobals->indentLeader, src );

	// Convert OMF MobID into AAF AUID and set mob id
	aafMobID_t				AAFMobUID;
	aafCheck = aafMobIDFromMajorMinor(42, OMFMobID.major, OMFMobID.minor, 4, &AAFMobUID);
	aafCheck = pMob->SetMobID(AAFMobUID);

	// Set comments
	aafInt32				numComments;
	OMFCheck omfCheck;
	omfCheck = omfiMobGetNumComments(OMFFileHdl, obj, &numComments);
	if (numComments > 0)
	{
		gpGlobals->pLogger->Log( kLogInfo, "Processing %ld comments...\n",  (long) numComments );
		// Allocate the iterator
		omfIterHdl_t		OMFIterator;
		omfCheck = omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
		aafInt32	times;
		char		sCommentName[64];
		char		sCommentValue[256];
		for (times = 0; times < numComments; times++)
		{
			omfCheck = omfiMobGetNextComment(OMFIterator, obj, sizeof(sCommentName), sCommentName, sizeof(sCommentValue), sCommentValue);
			int nameLen = strlen( sCommentName );
			if( nameLen > 0 )
			{
				aafWChar *pwCommentName = new wchar_t[ nameLen + 1 ];
				mbstowcs(pwCommentName, sCommentName, nameLen + 1);
				int textLen = strlen(sCommentValue);
				aafWChar* pwCommentValue = new wchar_t[ textLen + 1 ];
				mbstowcs(pwCommentValue, sCommentValue, textLen + 1);
				aafCheck = pMob->AppendComment(pwCommentName, pwCommentValue);
				gpGlobals->pLogger->Log( kLogInfo, "Comment \"%s\" of length %ld was converted.\n", sCommentName, textLen );
				gpGlobals->pLogger->Log( kLogInfo, "Comment value = \"%s\".\n", sCommentValue );

				delete [] pwCommentName;
				delete [] pwCommentValue;
			}
			else
			{
				gpGlobals->pLogger->Log( kLogError, "Zero length comment name encountered. Comment not converted.\n" );
			}
		}

		// Release the iterator
		omfCheck = omfiIteratorDispose(OMFFileHdl, OMFIterator);
	}

	FinishUpMob(obj, pMob);
	DecIndentLevel();

	if(pwMobName)
		delete [] pwMobName;
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
void Omf2Aaf::ConvertOMFCompositionObject(omfObject_t obj, 
											IAAFCompositionMob* pCompMob)
{
	omfDefaultFade_t	OMFDefaultFade;
	aafDefaultFade_t		AAFDefaultFade;
	IAAFObject				*pElement = NULL;
	AAFCheck				hr;

	// get Composition mob information
	OMFCheck				OMFError;

	omfErr_t			testErr;
	testErr = omfiMobGetDefaultFade(OMFFileHdl, obj, &OMFDefaultFade);
	if (OM_ERR_NONE == testErr && OMFDefaultFade.valid)
	{
		gpGlobals->nNumOMFProperties++;
		AAFDefaultFade.fadeLength   = OMFDefaultFade.fadeLength;
		switch(OMFDefaultFade.fadeType)
		{
			case kFadeNone: 
				AAFDefaultFade.fadeType = kAAFFadeNone;
				break;
			case kFadeLinearAmp:
				AAFDefaultFade.fadeType = kAAFFadeLinearAmp;
				break;
			case kFadeLinearPower:
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
		AAFCheck rc;
		rc = pCompMob->SetDefaultFade(AAFDefaultFade.fadeLength,
								 AAFDefaultFade.fadeType,
								 AAFDefaultFade.fadeEditUnit);
		gpGlobals->nNumAAFProperties++;
	}
	aafUID_t	classID = kAAFClassID_CompositionMob;

	hr = pCompMob->QueryInterface(IID_IAAFObject, (void **)&pElement);
	AutoRelease<IAAFObject> arElement(pElement);
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
void Omf2Aaf::TraverseOMFMob( omfObject_t obj, IAAFMob* pMob )
{
	CAAFBuiltinDefs defs (pDictionary);

	aafInt32				numSlots;
	OMFCheck				OMFError;
	omfErr_t			testErr;

	if (OM_ERR_NONE != omfiMobGetNumSlots(OMFFileHdl, obj, &numSlots))
		numSlots = 0;

	aafInt32				numTracks;
	if (OM_ERR_NONE != omfiMobGetNumTracks(OMFFileHdl, obj, &numTracks))
		numTracks = 0;

	IncIndentLevel();
	gpGlobals->pLogger->Log( kLogInfo,"%sFound: %ld sub tracks\n", gpGlobals->indentLeader, numSlots);

	omfIterHdl_t		OMFIterator;
	OMFCheck		omfCheck;
	omfCheck = omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
	for (aafInt32 times = 0; times < numSlots; times++)
	{
		
		omfPosition_t		OMFOrigin;
		omfRational_t		OMFeditRate;
		omfTrackID_t		OMFTrackID;
		omfMSlotObj_t		OMFSlot;
		
		OMFError = omfiMobGetNextSlot(OMFIterator, obj, NULL, &OMFSlot);
		
		char					sTrackName[32];
		omfMSlotObj_t		OMFSegment;
		OMFError = omfiMobSlotGetInfo(OMFFileHdl, OMFSlot, &OMFeditRate, &OMFSegment);
		if (omfiMobSlotIsTrack(OMFFileHdl, OMFSlot, &testErr))
		{
			aafUInt32				physicalTrackNumber = 0;			
			OMFError = omfiTrackGetInfo(OMFFileHdl, obj, OMFSlot, &OMFeditRate, sizeof(sTrackName),
				sTrackName, &OMFOrigin, &OMFTrackID, &OMFSegment);
			if (kOmfRev2x == OMFFileRev)
				OMFError = omfiTrackGetPhysicalNum(OMFFileHdl, OMFSlot, (omfUInt32 *)&physicalTrackNumber);
			else
				physicalTrackNumber = OMFTrackID;

			IAAFComponent*			pComponent = NULL;
			ProcessOMFComponent(OMFSegment, &pComponent);
			if( pComponent )
			{
				AutoRelease<IAAFComponent> pcomp( pComponent );
				IAAFSegment* pSegment;
				AAFCheck rc;
				rc = pComponent->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
				AutoRelease<IAAFSegment> pseg( pSegment );
				IncIndentLevel();
				
				aafWChar* pwTrackName = new wchar_t[strlen(sTrackName)+1];
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

				delete [] pwTrackName;
			}
		}
		// At this point we have a consistent MOB - Lets save it
		// pFile->Save();  Commented out until dirty bit is used by OM !!!
	}
	
	omfCheck = omfiIteratorDispose(OMFFileHdl, OMFIterator);
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
void Omf2Aaf::ConvertOMFSelector( omfObject_t selector, IAAFSelector* pSelector )
{
	omfLength_t		OMFLength;
	omfDDefObj_t		OMFDatakind;
	omfSegObj_t		OMFSelected;
	OMFCheck		OMFError;

	OMFError = omfiSelectorGetInfo( OMFFileHdl, 
									selector,
									&OMFDatakind,
									&OMFLength,
									&OMFSelected);

	// aafUID_t				datadef ;
	IAAFDataDefSP pDataDef;
	ConvertOMFDataDef(OMFDatakind, &pDataDef);
	IAAFComponent*			pComponent;
	AAFCheck rc;
	rc = pSelector->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	AutoRelease< IAAFComponent > pcomp( pComponent );
	rc = pComponent->SetDataDef(pDataDef);
	rc = pComponent->SetLength(OMFLength);
	gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Selector object of length = %ld\n", gpGlobals->indentLeader, (int)OMFLength);
		
	omfNumSlots_t		numAlternates;
	if (kOmfRev2x == OMFFileRev)
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
			OMFCheck OMFError;
			OMFError = omfiSelectorGetNumAltSlots(OMFFileHdl,
											  selector,
											  &numAlternates);
			if (numAlternates > 0)
			{
				omfIterHdl_t		OMFIterator;
				OMFError = omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
				for (int i = 0; i <(int)numAlternates;i++)
				{
					omfSegObj_t		OMFAltSelected;
					OMFError = omfiSelectorGetNextAltSlot(OMFIterator,
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
				OMFError = omfiIteratorDispose(OMFFileHdl, OMFIterator);
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

		OMFError = omfiSelectorGetNumAltSlots(OMFFileHdl,
											  selector,
											  &numAlternates);
		if (numAlternates > 0)
		{

			omfIterHdl_t		OMFIterator;
			OMFError = omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
			for (int i = 0; i <(int)numAlternates;i++)
			{
				omfSegObj_t		OMFAltSelected;
				OMFError = omfiSelectorGetNextAltSlot(OMFIterator,
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
			OMFError = omfiIteratorDispose(OMFFileHdl, OMFIterator);
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
void Omf2Aaf::ProcessOMFComponent(omfObject_t OMFSegment, IAAFComponent** ppComponent)
{
	AAFCheck				rc;
	OMFCheck				check;
	OMFCheck				OMFError;
	omfErr_t			testErr;

	omfLength_t		OMFLength;
	omfDDefObj_t	OMFDatakind;
	IAAFDataDefSP pDataDef;
	IAAFTypeDef * pParameterDefType = 0;

	*ppComponent = NULL;

	CAAFBuiltinDefs defs (pDictionary);

	// First get sequence information
	IncIndentLevel();
	if (omfiIsASequence(OMFFileHdl, OMFSegment, &testErr) )
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

	else if (omfiIsASourceClip(OMFFileHdl, OMFSegment, &testErr) )
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
	else if (omfiIsATimecodeClip(OMFFileHdl, OMFSegment, &testErr) )
	{
		// Get Timecode information
		aafTimecode_t			timecode;
		omfTimecode_t		OMFTimecode;
		check = omfiTimecodeGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength, &OMFTimecode);
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
	else if (omfiIsAnEdgecodeClip(OMFFileHdl, OMFSegment, &testErr) )
	{
		// Get edgecode data
		omfEdgecode_t		OMFEdgecode;
		aafEdgecode_t			edgecode;
		check = omfiEdgecodeGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength, &OMFEdgecode);
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
	else if (omfiIsAFiller(OMFFileHdl, OMFSegment, &testErr) )
	{
		check = omfiFillerGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength);
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
	else if (omfiIsAnEffect(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFOperationGroup*		pEffect;
		rc = defs.cdOperationGroup()->
		  CreateInstance(IID_IAAFOperationGroup,
						 (IUnknown **) &pEffect);
		AutoRelease<IAAFOperationGroup> peff( pEffect );
		ConvertOMFEffects(OMFSegment, pEffect);
		rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (omfiIsATransition(OMFFileHdl, OMFSegment, &testErr) )
	{

		omfPosition_t		OMFCutPoint;
		omfEffObj_t		OMFEffect;
		check = omfiTransitionGetInfo(OMFFileHdl, 
										 OMFSegment,
										 &OMFDatakind, 
										 &OMFLength, 
										 &OMFCutPoint,
										 &OMFEffect);
		gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Transition of length: %ld\n ", gpGlobals->indentLeader, (int)OMFLength);
		ConvertOMFDataDef( OMFDatakind, &pDataDef);

		if (kOmfRev2x == OMFFileRev)
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
			omfUniqueName_t	EffectID1x;

			testErr = omfsReadString(OMFFileHdl, OMFSegment, OMCPNTEffectID, EffectID1x, OMUNIQUENAME_SIZE);
			if (OM_ERR_NONE == testErr)
			{
				if ( (strcmp(EffectID1x, "Blend:Dissolve") == 0) && 
					(omfiIsSoundKind(	OMFFileHdl,	OMFDatakind, kExactMatch, &testErr)))
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
									(aafUInt32)-1, kAAFFalse, 2, DDEF_Sound, &pEffectDef);
					AutoRelease<IAAFOperationDef> peffdef( pEffectDef );

					IAAFParameterDef*		pParameterDef;
					rc = pDictionary->LookupTypeDef(kAAFTypeID_Rational, &pParameterDefType);
					AutoRelease<IAAFTypeDef> arTypeDef(pParameterDefType);
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, pParameterDefType, 
												L"Level", 
												L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
												L" ",
												&pParameterDef);
					AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );

					rc = pEffectDef->AddParameterDef(pParameterDef);
					rc = pEffect->Initialize(pDataDef, (aafLength_t)OMFLength, pEffectDef);
					rc = pEffect->SetBypassOverride((aafUInt32)-1);
					rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
				}
				else if ( (strcmp(EffectID1x, "Blend:Dissolve") == 0) && 
						  (omfiIsPictureWithMatteKind( OMFFileHdl, OMFDatakind, kConvertTo, &testErr)))
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
									(aafUInt32)-1, kAAFFalse, 2, DDEF_PictureWithMatte, &pEffectDef);
					AutoRelease<IAAFOperationDef> peffdef( pEffectDef );

					IAAFParameterDef* pParameterDef;
					rc = pDictionary->LookupTypeDef(kAAFTypeID_Rational, &pParameterDefType);
					AutoRelease<IAAFTypeDef> arTypeDef(pParameterDefType);
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, pParameterDefType, 
												L"Level", 
												L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
												L" ",
												&pParameterDef);
					AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );

					rc = pEffectDef->AddParameterDef(pParameterDef);
					rc = pEffect->Initialize(pDataDef, (aafLength_t)OMFLength, pEffectDef);
					rc = pEffect->SetBypassOverride((aafUInt32)-1);
					rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
				}
				else if ( strncmp(EffectID1x, "Wipe:SMPTE:", 11) == 0)
				{
					// Get SMPTE Vide Wipe Info and 
					// Create a SMPTE Video Wipe Effect !!
					omfInt32	wipeNumber = 0;
					omfWipeArgs_t wipeControls;

					check = omfeSMPTEVideoWipeGetInfo(OMFFileHdl, OMFSegment, NULL, NULL, NULL, NULL, &wipeNumber, &wipeControls);
					gpGlobals->pLogger->Log( kLogInfo, "%sReplacing 1.x Transition with a SMPTE Video Wipe Effect!\n ", gpGlobals->indentLeader);

					IAAFOperationGroup* pEffect;
					rc = defs.cdOperationGroup()->
					  CreateInstance(IID_IAAFOperationGroup,
									 (IUnknown **) &pEffect);
					AutoRelease< IAAFOperationGroup > peff( pEffect );

					IAAFOperationDef* pEffectDef;
					GetAAFOperationDefinition("omfi:effect:SMPTEVideoWipe", NULL, 
									"SMPTE Video Wipe", "Combines two video streams according to SMPTE ",
									(aafUInt32)-1, kAAFFalse, 2, DDEF_Picture, &pEffectDef);
					AutoRelease<IAAFOperationDef> peffdef( pEffectDef );

					rc = pEffect->Initialize(pDataDef, (aafLength_t)OMFLength, pEffectDef);
					rc = pEffect->SetBypassOverride((aafUInt32)-1);

					// Port over Wipe number
					IAAFParameterDef* pParameterDef;
					rc = pDictionary->LookupTypeDef(kAAFTypeID_Int32, &pParameterDefType);
					{ // Use local block to narrow the scope of the AutoRelease object.
						AutoRelease<IAAFTypeDef> tdef( pParameterDefType );
						GetParameterDefinition((aafUID_t *)&kAAFParameterDefSMPTEWipeNumber, pParameterDefType, 
												L"Wipe Number", 
												L"SMPTE Wipe Number. No Default",
												L" ",
												&pParameterDef);
					}
					AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );
					pEffectDef->AddParameterDef(pParameterDef);

					IAAFConstantValue* pConstantValue = NULL;
					rc = defs.cdConstantValue()->
					  CreateInstance(IID_IAAFConstantValue,
									 (IUnknown **)&pConstantValue);
					AutoRelease< IAAFConstantValue > pconstval( pConstantValue );
					rc = pConstantValue->Initialize (pParameterDef, sizeof(wipeNumber), (unsigned char *)&wipeNumber);

					IAAFParameter*			pParameter;
					rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					AutoRelease<IAAFParameter>	pparam( pParameter );
					rc = pEffect->AddParameter(pParameter);

					/*********************************************************************/
					//Port over reverse flag
					IAAFParameterDef* pParameterDefRev;
					rc = pDictionary->LookupTypeDef(kAAFTypeID_Boolean, &pParameterDefType);
					{ // Use local block to narrow the scope of the AutoRelease object.
						AutoRelease<IAAFTypeDef> tdef( pParameterDefType );
						GetParameterDefinition((aafUID_t *)&kAAFParameterDefSMPTEReverse, pParameterDefType, 
												L"Reverse", 
												L"Reverse flag. Default FALSE.",
												L" ",
												&pParameterDefRev);
					}
					AutoRelease< IAAFParameterDef > pparamdefrev( pParameterDefRev );
					rc = pEffectDef->AddParameterDef(pParameterDefRev);

					IAAFConstantValue* pConstantValueRev = NULL;
					rc = defs.cdConstantValue()->
					  CreateInstance(IID_IAAFConstantValue,
									 (IUnknown **)&pConstantValueRev);
					AutoRelease< IAAFConstantValue > pconstvalrev( pConstantValueRev );
					unsigned char	reverse = wipeControls.reverse;
					rc = pConstantValueRev->Initialize (pParameterDefRev, sizeof(reverse), &reverse);

					IAAFParameter*			pParameterRev;
					rc = pConstantValueRev->QueryInterface(IID_IAAFParameter, (void **)&pParameterRev);
					AutoRelease<IAAFParameter>	pparamrev( pParameterRev );
					rc = pEffect->AddParameter(pParameterRev);
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
			else if (OM_ERR_PROP_NOT_PRESENT == testErr)
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
	else if (omfiIsANestedScope(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFNestedScope*		pNestedScope;
		rc = defs.cdNestedScope()->
		  CreateInstance(IID_IAAFNestedScope,
						 (IUnknown **) &pNestedScope);
		AutoRelease<IAAFNestedScope> pnest( pNestedScope );
		ConvertOMFNestedScope(OMFSegment, pNestedScope);
		rc = pNestedScope->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (omfiIsAScopeRef(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFScopeReference*		pScopeRef;
		rc = defs.cdScopeReference()->
		  CreateInstance(IID_IAAFScopeReference,
						 (IUnknown **) &pScopeRef);
		AutoRelease<IAAFScopeReference> pscope( pScopeRef );
		ConvertOMFScopeRef(OMFSegment, pScopeRef);
		rc = pScopeRef->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (omfiIsASelector(OMFFileHdl, OMFSegment, &testErr) )
	{
		IAAFSelector*			pSelector = NULL;
		rc = defs.cdSelector()->
		  CreateInstance(IID_IAAFSelector,
						 (IUnknown **) &pSelector);
		AutoRelease<IAAFSelector> psel( pSelector );
		ConvertOMFSelector(OMFSegment, pSelector);
		rc = pSelector->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (omfiIsAMediaGroup(OMFFileHdl, OMFSegment, &testErr) )
	{
		gpGlobals->pLogger->Log( kLogInfo, "%sProcessing Media Group. No action taken.\n ", gpGlobals->indentLeader);
	}
	else
	{
		char				classID[5];
		omfProperty_t idProp;

		if (kOmfRev2x == OMFFileRev)
			idProp = OMOOBJObjClass;
		else
			idProp = OMObjID;

		OMFError = omfsReadClassID(OMFFileHdl, OMFSegment, idProp, classID);
		// Some 1.x effects are detected here. They have each a class of their own !!!
		// For now e will create a 

//		if (strncmp(classID, "MASK", (size_t)4) == 0)
//		{
//			OMFError = omfeVideoFrameMaskGetInfo();
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
			check = omfiComponentGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength);
			ConvertOMFDataDef( OMFDatakind, &pDataDef);
			IAAFFiller*		pFiller;
			rc = defs.cdFiller()->
			  CreateInstance(IID_IAAFFiller,
							 (IUnknown **) &pFiller);
			AutoRelease< IAAFFiller > pfill( pFiller );
			rc = pFiller->Initialize(pDataDef, (aafLength_t)OMFLength);
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
void Omf2Aaf::ConvertOMFSequence(omfObject_t sequence, 
								   IAAFSequence* pSequence )
{
	// Get a pointer to a component interface
	IAAFComponent*			pComponent;
	AAFCheck rc;
	rc = pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	AutoRelease<IAAFComponent> pcomp( pComponent );

	// Get Sequence data kind 
	omfDDefObj_t		datakind = NULL;
	omfLength_t		sequLength = 0;
	OMFCheck		OMFError;
	OMFError = omfiSequenceGetInfo(OMFFileHdl, sequence, &datakind, &sequLength);
	IAAFDataDefSP pDataDef;
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
void Omf2Aaf::ConvertOMFSourceClip(omfObject_t sourceclip, 
									 IAAFSourceClip* pSourceClip )
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	omfDDefObj_t		datakind = NULL;
	omfLength_t		clipLength = 0;
	omfSourceRef_t	OMFSourceRef;
	omfFadeType_t		OMFFadeinType;
	omfFadeType_t		OMFFadeoutType;
	omfBool			fadeinPresent, fadeoutPresent;

	IAAFDataDefSP pDataDef;
	aafSourceRef_t			sourceRef;
	aafFadeType_t			fadeinType, fadeoutType;
	aafInt32				fadeinLen, fadeoutLen;


	OMFError = omfiSourceClipGetInfo(OMFFileHdl, sourceclip, &datakind, &clipLength, &OMFSourceRef);

	OMFError = omfiSourceClipGetFade(OMFFileHdl, sourceclip, 
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
void Omf2Aaf::ConvertOMFComponentProperties(omfObject_t component, 
											 IAAFComponent* pComponent )
{
	AAFCheck				rc;
	omfErr_t			OMFError;

	omfProperty_t		Property;
	omfUniqueName_t	propertyName;
	omfIterHdl_t		propertyIterator = NULL;
	omfType_t			propertyType = OMNoType;

	IncIndentLevel();
	omfiIteratorAlloc(OMFFileHdl, &propertyIterator);
	OMFError = omfiGetNextProperty(propertyIterator, component, &Property, &propertyType);
	while((OM_ERR_NONE == OMFError) &&Property)
	{
		switch (Property)
		{
			case OMOOBJObjClass:
			case OMCPNTLength:
			case OMCPNTDatakind:
			case OMSEQUComponents:
				gpGlobals->nNumOMFProperties++;
				gpGlobals->nNumAAFProperties++;
				break;
			case OMCPNTTrackKind:
			case OMCPNTEditRate:
			case OMCPNTName:
			case OMCPNTEffectID:
			case OMCPNTAttributes:
			default:
				gpGlobals->nNumOMFProperties++;
				OMFError = omfiGetPropertyName(OMFFileHdl, Property, 64, propertyName);
				gpGlobals->pLogger->Log(kLogWarn, "%sComponent Property NOT converted : %s\n", gpGlobals->indentLeader, propertyName);
				break;
		}
		OMFError = omfiGetNextProperty(propertyIterator, component, &Property, &propertyType);
	}
	OMFError = omfiIteratorDispose(OMFFileHdl, propertyIterator);
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
void Omf2Aaf::TraverseOMFSequence(omfObject_t sequence, IAAFSequence* pSequence )
{
	aafInt32				numComponents = 0;
	OMFCheck				OMFError;
	OMFError  = omfiSequenceGetNumCpnts(OMFFileHdl, sequence, &numComponents);
	if (numComponents > 0)
	{
		omfIterHdl_t		componentIterator;
		OMFError = omfiIteratorAlloc(OMFFileHdl, &componentIterator);
		for (aafInt32 cpntCount = 0; cpntCount < numComponents; cpntCount++)
		{   
			omfPosition_t		sequPos;
			omfCpntObj_t		sequComponent = NULL;
			OMFError = omfiSequenceGetNextCpnt(componentIterator, sequence, NULL, 
											   &sequPos, &sequComponent); 
			IAAFComponent*			pComponent;
			ProcessOMFComponent(sequComponent, &pComponent);
			if( pComponent )
			{
				AutoRelease<IAAFComponent> pcomp( pComponent );
				AAFCheck rc;
				rc = pSequence->AppendComponent(pComponent);
			}
		}
		OMFError = omfiIteratorDispose(OMFFileHdl, componentIterator);
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
void Omf2Aaf::ConvertOMFMasterMob(omfObject_t obj,
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
void Omf2Aaf::ConvertOMFLocator(omfObject_t obj,
									IAAFEssenceDescriptor* pEssenceDesc)
{
	CAAFBuiltinDefs defs (pDictionary);

	omfIterHdl_t		locatorIter;
	OMFCheck		omfCheck;
	omfErr_t			testErr;

	omfObject_t		OMFLocator;
	OMFCheck				OMFError;
	omfCheck = omfiIteratorAlloc(OMFFileHdl, &locatorIter);

	testErr = omfmMobGetNextLocator(locatorIter, obj, &OMFLocator);

	while((testErr == OM_ERR_NONE) &&(OMFLocator != NULL))
	{
		omfClassID_t		locType;
		char					locatorPath[128];
		omfCheck = omfmLocatorGetInfo(OMFFileHdl, OMFLocator, locType, 
			sizeof( locatorPath ), locatorPath);
		aafWChar *pwLocatorPath = new wchar_t[strlen(locatorPath)+1];
		mbstowcs(pwLocatorPath, locatorPath, strlen(locatorPath)+1);

		IAAFNetworkLocator*		pNetworkLocator;
		AAFCheck aafCheck;
		aafCheck = defs.cdNetworkLocator()->
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
		testErr = omfmMobGetNextLocator(locatorIter, obj, &OMFLocator);

		delete [] pwLocatorPath;
	}

	OMFError = omfiIteratorDispose(OMFFileHdl, locatorIter);
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
void Omf2Aaf::ConvertOMFCDCIDescriptorLocator(omfObject_t mediaDescriptor,
									IAAFCDCIDescriptor* pAAFDescriptor)
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	omfFrameLayout_t	frameLayout;
	omfPosition_t		zeroPos = 0;
	omfPosition_t		fourPos = 0;

	IAAFDigitalImageDescriptor*	pDigImageDesc = NULL;

	aafUInt32			Height = 0;
	aafUInt32			Width = 0;
	aafInt32			XOffset = 0;
	aafInt32			YOffset = 0;
	aafInt32			alphaTransparency = 0;
	aafInt32			alignmentFactor;

	aafFrameLayout_t	AAFFrameLayout;
	aafRational_t		aspectRatio;

	aafInt32			componentWidth = 0;
	aafUInt32			horizontalSubsampling = 0;
	aafColorSiting_t	colorSiting = kAAFCoSiting;
	aafUInt32			blackReferenceLevel = 0;
	aafUInt32			whiteReferenceLevel = 0;
	aafUInt32			colorRange = 0;
	aafInt16			paddingBits = 0;
	aafInt32			videoLineMap[2];
						
	rc = pAAFDescriptor->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&pDigImageDesc);

	// Get Digital Image properties and set them
	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMDIDDStoredHeight, &Height) != OM_ERR_NONE)
	{
		if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDStoredHeight, (omfInt32 *)&Height) != OM_ERR_NONE)
		{
			Height = 0;
		}
	}

	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMDIDDStoredWidth, &Width) != OM_ERR_NONE)
	{
		if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDStoredWidth, (omfInt32 *)&Width) != OM_ERR_NONE)
		{
			Width = 0;
		}
	}
	rc = pDigImageDesc->SetStoredView(Height, Width);

	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledHeight, &Height) != OM_ERR_NONE)
	{
		if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledHeight, (omfInt32 *)&Height) != OM_ERR_NONE)
		{
			Height = 0;
		}
	}
	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledWidth, &Width) != OM_ERR_NONE)
	{
		if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledWidth, (omfInt32 *)&Width) != OM_ERR_NONE)
		{
			Width = 0;
		}
	}
	if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledXOffset, &XOffset) != OM_ERR_NONE)
		XOffset = 0;
	if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledYOffset, &YOffset) != OM_ERR_NONE)
		YOffset = 0;
	rc = pDigImageDesc->SetSampledView(Height, Width, XOffset, YOffset);

	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayHeight, &Height) != OM_ERR_NONE)
	{
		if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayHeight, (omfInt32 *)&Height) != OM_ERR_NONE)
		{
			Height = 0;
		}
	}
	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayWidth, &Width) != OM_ERR_NONE)
	{
		if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayWidth, (omfInt32 *)&Width) != OM_ERR_NONE)
		{
			Width = 0;
		}
	}
	if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayXOffset, &XOffset) != OM_ERR_NONE)
		XOffset = 0;
	if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayYOffset, &YOffset) != OM_ERR_NONE)
		YOffset = 0;
	rc = pDigImageDesc->SetDisplayView(Height, Width, XOffset, YOffset);

	if (omfsReadLayoutType(OMFFileHdl, mediaDescriptor, OMDIDDFrameLayout, &frameLayout) != OM_ERR_NONE)
		AAFFrameLayout = (aafFrameLayout_t)0;
	else
	{
		if (frameLayout > 0)
			AAFFrameLayout = (aafFrameLayout_t)(frameLayout-1);
		else
			AAFFrameLayout = (aafFrameLayout_t)0;
	}
	rc = pDigImageDesc->SetFrameLayout(AAFFrameLayout);
	if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDAlphaTransparency, &alphaTransparency) != OM_ERR_NONE)
		alphaTransparency = 0;
	rc = pDigImageDesc->SetAlphaTransparency((aafAlphaTransparency_t)alphaTransparency);

//!!!	if (omfsReadRational(OMFFileHdl, mediaDescriptor, OMDIDDGamma, (omfRational_t *)&gamma) != OM_ERR_NONE)
//	{
//		gamma.numerator = 0;
//		gamma.denominator = 1;
//	}
//	rc = pDigImageDesc->SetGamma(gamma);

	if (omfsReadRational(OMFFileHdl, mediaDescriptor, OMDIDDImageAspectRatio, (omfRational_t *)&aspectRatio) != OM_ERR_NONE)
	{
		aspectRatio.numerator = 0;
		aspectRatio.denominator = 1;
	}
	rc = pDigImageDesc->SetImageAspectRatio(aspectRatio);

	if (omfsReadInt32(OMFFileHdl, mediaDescriptor, OMDIDDFieldAlignment, &alignmentFactor) != OM_ERR_NONE)
		alignmentFactor = 0;
	rc = pDigImageDesc->SetImageAlignmentFactor(alignmentFactor);

	if (OMReadProp(OMFFileHdl, mediaDescriptor, OMDIDDVideoLineMap, 
						   zeroPos, kSwabIfNeeded, OMInt32Array,
						   sizeof(aafInt32), &videoLineMap[0]) != OM_ERR_NONE)
		videoLineMap[0] = 0;
	omfsCvtInt32toPosition(sizeof(aafInt32), fourPos);
	if (OMReadProp(OMFFileHdl, mediaDescriptor, OMDIDDVideoLineMap, 
						   fourPos, kSwabIfNeeded, OMInt32Array,
						   sizeof(aafInt32), &videoLineMap[1]) != OM_ERR_NONE)
		videoLineMap[1] = 0;

	rc = pDigImageDesc->SetVideoLineMap( (sizeof(videoLineMap)/sizeof(aafInt32)), videoLineMap);

	//
	aafUID_t AAFCompress = kAAFNoCodec;
	aafUInt32 compLen = omfsLengthString(OMFFileHdl, mediaDescriptor, OMDIDDCompression);
	if( compLen > 0 )
	{
		char compress[ 64 ] = "";
		OMFError = omfsReadString(OMFFileHdl, mediaDescriptor, OMDIDDCompression, compress, compLen+1);
		if(strcmp(compress, "JFIF") == 0)
			AAFCompress = kAAFCodecJPEG;
	}
	rc = pDigImageDesc->SetCompression(AAFCompress);
	pDigImageDesc->Release();
	pDigImageDesc = NULL;

	// Next we read the values
	if (omfsReadInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIComponentWidth, &componentWidth) != OM_ERR_NONE)
		componentWidth = 0;
	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIHorizontalSubsampling, &horizontalSubsampling) != OM_ERR_NONE)
		horizontalSubsampling = 0;
	if (OMReadProp(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIHorizontalSubsampling, 
						  zeroPos, kSwabIfNeeded, OMColorSitingType,
						  sizeof(colorSiting), (void *)&(colorSiting)) != OM_ERR_NONE)
		memset(&colorSiting, 0, sizeof(aafColorSiting_t));
	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIBlackReferenceLevel, &blackReferenceLevel) != OM_ERR_NONE)
		blackReferenceLevel = 0;
	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIWhiteReferenceLevel, &whiteReferenceLevel) != OM_ERR_NONE)
		whiteReferenceLevel = 0;
	if (omfsReadUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIColorRange, &colorRange) != OM_ERR_NONE)
		colorRange = 0;
	if (OMReadProp(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIPaddingBits, 
						   zeroPos, kSwabIfNeeded, OMInt16,
						   sizeof(paddingBits), &paddingBits) != OM_ERR_NONE)
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
void Omf2Aaf::ConvertOMFSourceMob(omfObject_t obj,
									IAAFSourceMob* pSourceMob)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	omfErr_t			testErr;
	
	omfObject_t		mediaDescriptor;
	omfInt32			numLocators = 0;
	
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
	AutoRelease<IAAFObject> arElement(pElement);
	ConvertObjectProps(obj, classID, pElement);
	
	OMFError = omfmMobGetMediaDescription(OMFFileHdl, obj, &mediaDescriptor);
	
	if ( omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDFL, &testErr))
	{
		// File Source Mob
		if ( omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassTIFD, &testErr) )
		{
			// TIFF File Descriptor
			omfBool			IsContiguous, IsUniform;
			omfInt32			leadingLines, trailingLines;
			char					summary[1024];	//!!!Fixed size
			omfJPEGTableID_t	data;
			omfDDefObj_t		datakind;
			
			aafUInt32				bytesRead = 0;
			
			rc = defs.cdTIFFDescriptor()->
				CreateInstance(IID_IAAFTIFFDescriptor,
				(IUnknown **)&pTiffDesc);
			
			OMFError = omfsReadBoolean( OMFFileHdl,
				mediaDescriptor,
				OMTIFDIsContiguous, &IsContiguous);
			OMFError = omfsReadBoolean( OMFFileHdl,
				mediaDescriptor,
				OMTIFDIsUniform,
				&IsUniform);
			OMFError = omfsReadInt32(OMFFileHdl,
				mediaDescriptor,
				OMTIFDLeadingLines, 
				&leadingLines);
			OMFError = omfsReadInt32(OMFFileHdl,
				mediaDescriptor,
				OMTIFDTrailingLines, 
				&trailingLines);
			OMFError = omfsReadJPEGTableIDType( OMFFileHdl,
				mediaDescriptor,
				OMTIFDJPEGTableID, 
				&data);
			if (kOmfRev2x == OMFFileRev)
			{
				omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (omfErr_t *)&testErr);
				//Read only as much as there is...
				(void)omfsReadDataValue(OMFFileHdl,
					mediaDescriptor,
					OMTIFDSummary,
					datakind,
					summary,
					0,
					sizeof(summary),
					&bytesRead);
			}
			else
			{
				OMFError = omfsReadVarLenBytes(OMFFileHdl,
					mediaDescriptor,
					OMTIFDSummary,
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
		else if ( omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassWAVD, &testErr) )
		{
			// WAVE File Descriptor
			omfDDefObj_t		datakind;
			char					*summary;
			aafUInt32				bytesRead;
			aafUInt32				numBytes;
			
			rc = defs.cdWAVEDescriptor()->
				CreateInstance(IID_IAAFWAVEDescriptor,
				(IUnknown **)&pWAVEDesc);
			if (kOmfRev2x == OMFFileRev)
			{
				omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, &testErr);
				numBytes = (aafUInt32)omfsLengthDataValue(OMFFileHdl, mediaDescriptor, OMWAVDSummary);
				summary = new char[numBytes];
				OMFError = omfsReadDataValue(OMFFileHdl,
					mediaDescriptor,
					OMWAVDSummary,
					datakind,
					(omfDataValue_t)summary,
					0,
					numBytes,
					&bytesRead);
			}
			else
			{
				numBytes = (aafUInt32)omfsLengthVarLenBytes(OMFFileHdl,
					mediaDescriptor,
					OMWAVDSummary);
				summary = new char[numBytes];
				OMFError = omfsReadVarLenBytes(OMFFileHdl,
					mediaDescriptor,
					OMWAVDSummary,
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
		else if ( omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassAIFD, &testErr) )
		{
			// AIFC Audio File Descriptor
			char					summary[1024];		// Fixed size (see WAVE)
			omfDDefObj_t		datakind;
			
			aafUInt32				bytesRead;
			aafUInt32				numBytes;
			
			rc = defs.cdAIFCDescriptor()->
				CreateInstance(IID_IAAFAIFCDescriptor,
				(IUnknown **)&pAifcDesc);
			if (kOmfRev2x == OMFFileRev)
			{
				omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, &testErr);
				numBytes = (aafUInt32)omfsLengthDataValue(OMFFileHdl, mediaDescriptor, OMAIFDSummary);
				OMFError = omfsReadDataValue(OMFFileHdl,
					mediaDescriptor,
					OMAIFDSummary,
					datakind,
					(omfDataValue_t)summary,
					0,
					numBytes,
					&bytesRead);
			}
			else
			{
				OMFError = omfsReadVarLenBytes(OMFFileHdl,
					mediaDescriptor,
					OMAIFDSummary,
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
		else if ( omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassCDCI, &testErr) )
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
			omfClassID_t	objClass;
			char				id[5];
			char				summary[64];
			
			OMFError = omfsReadClassID(OMFFileHdl, mediaDescriptor, OMOOBJObjClass, objClass);
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
		
		if (kOmfRev2x == OMFFileRev)
			OMFError = omfsReadRational(OMFFileHdl, mediaDescriptor, OMMDFLSampleRate, (omfRational_t *)&sampleRate);
		else
			OMFError = omfsReadExactEditRate(OMFFileHdl, mediaDescriptor, OMMDFLSampleRate, (omfRational_t *)&sampleRate);
		rc = pFileDesc->SetSampleRate(sampleRate);
		
		OMFError = omfsReadLength(OMFFileHdl, mediaDescriptor, OMMDFLLength, (omfLength_t *)&length);
		rc = pFileDesc->SetLength(length);
		
		pFileDesc->Release();
		pFileDesc = NULL;
	}
	else
	{
		// Physical Source Mob
		if ( omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDTP, &testErr) ||
			omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDES, &testErr) )
		{
			// Tape Media descriptor
			omfTapeCaseType_t		formFactor;
			omfVideoSignalType_t	videoSignal;
			omfTapeFormatType_t	tapeFormat;
			omfLength_t			length;
			char						manufacturer[64], model[64];
			omfInt32				manuSize, modelSize;
			
			aafWChar*					pwManufacturer = NULL;
			aafWChar*					pwModel = NULL;
			
			rc = defs.cdTapeDescriptor()->
				CreateInstance(IID_IAAFTapeDescriptor,
				(IUnknown **)&pTapeDesc);
			manuSize = modelSize = 64;
			if (OM_ERR_NONE == omfmTapeMobGetDescriptor(OMFFileHdl, 
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
				formFactor = kTapeCaseNull;
				videoSignal = kVideoSignalNull;
				tapeFormat = kTapeFormatNull;
				length = 0;
			}
			pTapeDesc->SetTapeModel(pwModel);
			pTapeDesc->SetTapeManufacturer(pwManufacturer);
			pTapeDesc->SetTapeFormFactor((aafTapeCaseType_t)formFactor);
			pTapeDesc->SetSignalType((aafVideoSignalType_t)videoSignal);
			pTapeDesc->SetTapeFormat((aafTapeFormatType_t)tapeFormat);
			pTapeDesc->SetTapeLength((aafUInt32)length);
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
		else if ( omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDFM, &testErr))
		{
			// Film Media descriptor
			gpGlobals->pLogger->Log( kLogError, "%sFilm media descriptor NOT Implemented yet\n", gpGlobals->indentLeader);
		}
		else
		{
			omfClassID_t	objClass;
			omfClassID_t	objSuperClass;
			omfProperty_t	propertyID;
			omfBool		found;
			char				id[5];
			char				superID[5];
			
			if (kOmfRev2x == OMFFileRev)
			{
				propertyID = OMOOBJObjClass;
			}
			else
			{
				propertyID = OMObjID;
			}
			OMFError = omfsReadClassID(OMFFileHdl, mediaDescriptor, propertyID, objClass);
			strncpy(id, objClass, 4);
			id[4] = '\0';
			gpGlobals->pLogger->Log( kLogError,
				"%sCannot translate this Physical Media Descriptor: %s\n", gpGlobals->indentLeader, id);
			gpGlobals->nNumUndefinedOMFObjects++;
			// Here we could check for a Dictionary entry to translate this class !!
			// For now lets try to figure out what kind of descriptor can we do.
			rc = omfsClassFindSuperClass(OMFFileHdl, objClass, objSuperClass, &found);
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
		if(omfmMobGetNumLocators(OMFFileHdl, obj, &numLocators) == OM_ERR_NONE)
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
void Omf2Aaf::ConvertOMFConstValue(omfSegObj_t segment,
									  IAAFParameterDef* pParameterDef, IAAFConstantValue* pConstValue)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	
	omfDDefObj_t	cvDatakind;
	omfLength_t	cvLength;
	omfInt64		cvValueSize;
	omfInt64		cvBytesRead;
	void *				pcvBuffer = NULL;

	aafUInt32			valueSize;
	IAAFTypeDef*		pTypeDef = NULL;

	OMFError = omfiDataValueGetSize(OMFFileHdl, segment, &cvValueSize);
	omfsTruncInt64toUInt32(cvValueSize, &valueSize);
	if (valueSize > 0)
	{
		pcvBuffer = new char[valueSize];
		OMFError = omfiConstValueGetInfo(OMFFileHdl, segment, 
							&cvDatakind, &cvLength, cvValueSize, &cvBytesRead, pcvBuffer);
		gpGlobals->pLogger->Log( kLogInfo, 
			"%sProcessing Constant Value of length = %ld\n ", gpGlobals->indentLeader, (int)cvLength);

		// Constant value should be initialized with a parameter definition. The
		// parameter definition specifies the type. Someone may want to add validation
		// and or conversion code to make sure that the type returned from 
		// ConvertOMFDataDefType is the same type used in the parameter definition
		// (transdel 2000-MAR-07).
		rc = pConstValue->Initialize (pParameterDef, valueSize, (aafDataBuffer_t)pcvBuffer);

		ConvertOMFDataDefType(cvDatakind, &pTypeDef);
		pTypeDef->Release();
		pTypeDef = NULL;
	}
	if (pcvBuffer)
		delete [] (char*)pcvBuffer;
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
void Omf2Aaf::ConvertOMFVaryingValue(omfSegObj_t segment,
										IAAFParameterDef* pParameterDef, IAAFVaryingValue* pVaryingValue)
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	omfIterHdl_t		OMFIterator;
	omfInt32			numPoints;
	omfDDefObj_t		vvDatakind;
	omfCntlPtObj_t	control;
	omfDDefObj_t		cpDatakind;
	omfDDefObj_t		tmpDatakind;
	omfLength_t		vvLength;
	omfInterpKind_t	vvInterpolation;
	omfRational_t		time;
	omfEditHint_t		editHint;
	omfInt64			cpValueSize;
	omfUInt32			bytesRead = 0;
	omfPosition_t		offset;
	void *					pCPBuffer = NULL;

	IAAFControlPoint*		pControlPoint = NULL;
	IAAFTypeDef*			pTypeDef = NULL;
	IAAFInterpolationDef*	pInterp = NULL;

	aafRational_t			AAFCPTime;
	aafEditHint_t			AAFCPEditHint;
	aafUInt32				valueSize;
	omfErr_t			testErr;

	CAAFBuiltinDefs defs (pDictionary);

	IncIndentLevel();
	OMFError = omfiVaryValueGetInfo(OMFFileHdl, segment, &vvDatakind, &vvLength, &vvInterpolation);
	// tlk We do NOT know how to handle Interpolations yet !!!

	// Need to translate other interpolators!!!
	pInterp = pAAF->CreateInterpolationDefinition(pDictionary, LinearInterpolator);

	// The varying value must be initialized with a parameter definition and an
	// interpolation definition before any of its other interface methods are 
	// called. Someone may want to add validation and or conversion code to make 
	// sure that the type returned from ConvertOMFDataDefType is the same type 
	// used in the parameter definition (transdel 2000-MAR-07).
	rc = pVaryingValue->Initialize (pParameterDef, pInterp);
  
	ConvertOMFDataDefType(vvDatakind, &pTypeDef);
	pTypeDef->Release();
	pTypeDef = NULL;

	// Get number of Points 
	OMFError = omfiVaryValueGetNumPoints(OMFFileHdl, segment, &numPoints);

	gpGlobals->pLogger->Log( kLogInfo,
		"%sProcessing Varying Value of length = %ld and %ld Control Points\n ", gpGlobals->indentLeader, (int)vvLength, (int)numPoints);
	if (numPoints > 0)
	{
		omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
		for (int i = 0; i < numPoints;i++)
		{
			OMFError = omfiVaryValueGetNextPoint(OMFIterator, segment, NULL, &control);
			defs.cdControlPoint()->
				CreateInstance(IID_IAAFControlPoint,
				(IUnknown **)&pControlPoint);
			OMFError = omfiDataValueGetSize(OMFFileHdl, control, &cpValueSize);
			omfsTruncInt64toUInt32(cpValueSize, &valueSize);
			pCPBuffer = new char[valueSize];
			OMFError = omfiControlPtGetInfo(OMFFileHdl, control, &time, &editHint, &cpDatakind, 
				valueSize, (long *)&bytesRead, pCPBuffer);
			
			//!!! The above call is bad in the currently tested version of the OMF TK
			//!!! Get the information another way (next two function calls
			omfsReadObjRef(OMFFileHdl, control, OMCTLPDatakind, &cpDatakind);
			OMFError = omfsReadDataValue(OMFFileHdl, control,
				OMCTLPValue, cpDatakind, pCPBuffer,
				0, valueSize, &bytesRead);
			
			if (bytesRead != valueSize)
			{
				omfUniqueName_t	uniqueName;
				
				OMFError = omfsReadObjRef(OMFFileHdl, control, OMCTLPDatakind, &cpDatakind);
				OMFError = omfsReadUniqueName(OMFFileHdl, cpDatakind, OMDDEFDatakindID, uniqueName, sizeof(uniqueName));
				if (strcmp(uniqueName, "omfi:data:Rational") == 0 && (valueSize == 4))
				{
					omfiDatakindLookup(OMFFileHdl, (omfUniqueNamePtr_t)"omfi:data:Int32", &tmpDatakind, &testErr);
					cpDatakind = tmpDatakind;
				}
				offset = 0;
				OMFError = omfsReadDataValue(OMFFileHdl, control, OMCTLPValue, cpDatakind, pCPBuffer, offset,valueSize, &bytesRead);
			}
			AAFCPTime.numerator = time.numerator;
			AAFCPTime.denominator = time.denominator;
			rc = pControlPoint->Initialize (pVaryingValue, AAFCPTime, (aafUInt32)valueSize, (unsigned char *)pCPBuffer);
			AAFCPEditHint = (aafEditHint_t)editHint;
			rc = pControlPoint->SetEditHint(AAFCPEditHint);
			pVaryingValue->AddControlPoint(pControlPoint);
			pControlPoint->Release();
			pControlPoint = NULL;
			if (pCPBuffer)
				delete [] (char*)pCPBuffer;
			if (pControlPoint)
				pControlPoint->Release();
			pControlPoint = NULL;
		}
		omfiIteratorDispose(OMFFileHdl, OMFIterator);
	}
	
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
void Omf2Aaf::ConvertOMFNestedScope(omfSegObj_t segment,
									   IAAFNestedScope* pNestedScope)
{


	IncIndentLevel();

	omfDDefObj_t		nsDatakind;
	omfLength_t		nsLength;
	OMFCheck		OMFError;
	OMFError = omfiNestedScopeGetInfo(OMFFileHdl, segment, &nsDatakind, &nsLength);

	omfNumSlots_t		numSlots;
	OMFError = omfiNestedScopeGetNumSlots(OMFFileHdl, segment, &numSlots);
	gpGlobals->pLogger->Log( kLogInfo, 
			"%sProcessing Nested Scope of length = %ld and %ld slots\n ", 
			gpGlobals->indentLeader, (int)nsLength, (int) numSlots);

	// Set Nested Scope Component properties.
	IAAFDataDef * pDataDef = 0;
	ConvertOMFDataDef(nsDatakind, &pDataDef);
	AutoRelease<IAAFDataDef> pdatadef( pDataDef );
	IAAFComponent*			pSegmentComp;
	AAFCheck rc;
	rc = pNestedScope->QueryInterface(IID_IAAFComponent, (void **)&pSegmentComp);
	AutoRelease<IAAFComponent> pseg( pSegmentComp );

	rc = pSegmentComp->SetDataDef(pDataDef);
	rc = pSegmentComp->SetLength(nsLength);

	if (numSlots > 0)
	{
		omfIterHdl_t		OMFIterator;
		OMFError = omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
		for (int i = 0; i < numSlots;i++)
		{
			omfSegObj_t		scopeSlot;
			OMFError = omfiNestedScopeGetNextSlot(OMFIterator, segment, NULL, &scopeSlot);
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
		OMFError = omfiIteratorDispose(OMFFileHdl, OMFIterator);
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
void Omf2Aaf::ConvertOMFScopeRef(omfSegObj_t segment,
									IAAFScopeReference* pScopeRef)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	omfDDefObj_t		srDatakind;
	omfLength_t		srLength;
	omfUInt32			relScope;
	omfUInt32			relSlot;
	
	IAAFComponent*			pSegmentComp = NULL;
	IAAFDataDef * pDataDef = 0;
	
	IncIndentLevel();
	OMFError = omfiScopeRefGetInfo(OMFFileHdl, segment, &srDatakind, &srLength, &relScope, &relSlot);
	gpGlobals->pLogger->Log(kLogInfo,
		"%sProcessing Scope reference of length = %ld\n ", gpGlobals->indentLeader, (int)srLength);
	ConvertOMFDataDef(srDatakind, &pDataDef);
  AutoRelease<IAAFDataDef> arDataDef(pDataDef);
  rc = pScopeRef->QueryInterface(IID_IAAFComponent, (void **)&pSegmentComp);
  AutoRelease<IAAFComponent> arSegmentComp(pSegmentComp);
	rc = pSegmentComp->SetDataDef(pDataDef);
	rc = pSegmentComp->SetLength(srLength);
	
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
		OMFCheck check;
		check = omfsCloseFile(OMFFileHdl);
		OMFFileHdl = NULL;
		check = omfsEndSession(OMFSession);
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
void Omf2Aaf::ConvertOMFEffects(omfEffObj_t	effect,
								   IAAFOperationGroup*	pEffect)
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	omfDDefObj_t		effectDatakind;
	omfSegObj_t		inputSegmentA ;
	omfSegObj_t		inputSegmentB ;
	omfSegObj_t		levelSegment;
	omfSegObj_t		renderClip = NULL;
	omfLength_t		effectLength;
	omfDDefObj_t		effectDef;
	omfInt32			nameSize = 64, idSize = 64;
	omfUniqueName_t	effectID;
	omfUniqueName_t	MCEffectID;
	char					effectDefName[64];
	omfRational_t		speedRatio;
	omfUInt32			phaseOffset = 0;
	omfUInt32			mask = 0;
	omfFxFrameMaskDirection_t addOrDrop = kOmfFxFrameMaskDirNone;
	omfArgIDType_t	bypassOverride;
	omfInt32			wipeNumber;
	omfWipeArgs_t		wipeArgs;
	omfErr_t			testErr;

	IAAFOperationDef*		pEffectDef = NULL;
	IAAFComponent*			pEffectSegment = NULL;
	IAAFComponent*			pRenderSegment = NULL;
	IAAFSourceReference*	pSourceRef= NULL;
	IAAFSegment*			pSegment = NULL;
	IAAFParameter*			pParameter = NULL;
	IAAFParameterDef*		pParameterDef = NULL;
	IAAFTypeDef*			pParameterDefType = NULL;

	IAAFDataDefSP  effectDataDef;
	aafUID_t				effectDefAUID ;
//	aafUID_t				AAFEffectID;
//	aafInt32				keyFrameSlot, globalSlot;
	//	omfSegObj_t		segment;

	IncIndentLevel();

	CAAFBuiltinDefs defs (pDictionary);

	if (kOmfRev2x == OMFFileRev)
	{
		OMFError = omfiEffectGetInfo(OMFFileHdl, effect, &effectDatakind, &effectLength, &effectDef);

		gpGlobals->pLogger->Log( kLogInfo,
			"%sProcessing Effect of length = %ld\n ", gpGlobals->indentLeader, (unsigned int)effectLength);

		OMFError = omfiEffectDefGetInfo(OMFFileHdl, effectDef, idSize, effectID, nameSize, effectDefName,
									0, NULL, NULL, NULL);
		if (OM_ERR_PROP_NOT_PRESENT == omfiEffectGetBypassOverride(OMFFileHdl, effect, &bypassOverride))
			bypassOverride = 0;
		// Get the AAF Effect definition interface pointer !!
		if(omfsReadString(OMFFileHdl, effect, gpGlobals->pvtEffectIDProp,
									MCEffectID, sizeof(MCEffectID)) == OM_ERR_NONE)
			(void)pEffectTranslate->GetAAFEffectID(effectID, MCEffectID, &effectDefAUID);
		else
			(void)pEffectTranslate->GetAAFEffectID(effectID, NULL, &effectDefAUID);

		ConvertOMFDataDef(effectDatakind, &effectDataDef);
		ConvertOMFEffectDefinition(effectDef, effect, &pEffectDef);

		if (strcmp(effectID, "omfi:effect:VideoSpeedControl") == 0)
		{
			//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
			//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			rc = pDictionary->LookupTypeDef(kAAFTypeID_Rational, &pParameterDefType);
			{ // Use local block to narrow the scope of the AutoRelease object.
				AutoRelease<IAAFTypeDef> tdef( pParameterDefType );
				GetParameterDefinition((aafUID_t *)&kAAFParameterDefSpeedRatio, pParameterDefType, 
				L"Speed Ratio", 
				L"Defines the ratio of output length to input length. Range is -infinity to +infinity",
				L" ",
				&pParameterDef);
			}
			AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );

			pEffectDef->AddParameterDef(pParameterDef);
			OMFError = omfeVideoSpeedControlGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &speedRatio, &phaseOffset);
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
			OMFError = omfeVideoFrameMaskGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &mask, &addOrDrop, &phaseOffset);
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
			rc = pDictionary->LookupTypeDef(kAAFTypeID_Int32, &pParameterDefType);
			{ // Use local block to narrow the scope of the AutoRelease object.
				AutoRelease<IAAFTypeDef> tdef( pParameterDefType );
				GetParameterDefinition((aafUID_t *)&kAAFParameterDefPhaseOffset, pParameterDefType, 
										L"PhaseOffset", 
										L"Must be a constant Value. Default is 0",
										L" ",
										&pParameterDef);
			}
			AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );
			pEffectDef->AddParameterDef(pParameterDef);

			if(OM_ERR_NONE == omfeVideoRepeatGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &phaseOffset))
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
					AutoRelease<IAAFConstantValue> arCV(pConstantValue);
					rc = pConstantValue->Initialize (pParameterDef, sizeof(phaseOffset), (unsigned char *)&phaseOffset);
					rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					AutoRelease<IAAFParameter> arParam(pParameter);
					rc = pEffect->AddParameter(pParameter);
				}
			}
		}
		else if ((strcmp(effectID, "omfi:effect:VideoDissolve") == 0) ||
				 (strcmp(effectID, "omfi:effect:SimpleVideoDissolve") == 0) )
		{
//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			// Parameter definition always needs a type definition! This is
			// a required property. What is the appropriate type for this
			// "Level" definition? (transdel 2000-MAR-07)
			rc = pDictionary->LookupTypeDef(kAAFTypeID_Rational, &pParameterDefType);
			{ // Use local block to narrow the scope of the AutoRelease object.
				AutoRelease<IAAFTypeDef> tdef( pParameterDefType );
				GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, pParameterDefType, 
										L"Level", 
										L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
										L" ",
										&pParameterDef);
			}
			AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );
			rc = pEffectDef->AddParameterDef(pParameterDef);

			if(OM_ERR_NONE == omfeVideoDissolveGetInfo(OMFFileHdl, effect,
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
					if (omfiIsAConstValue(OMFFileHdl, levelSegment, &testErr))
					{
						IAAFConstantValue* pConstantValue = NULL;
						
						rc = defs.cdConstantValue()->
							CreateInstance(IID_IAAFConstantValue,
							(IUnknown **)&pConstantValue);
						AutoRelease<IAAFConstantValue> arCV(pConstantValue);
						ConvertOMFConstValue(levelSegment, pParameterDef, pConstantValue);
						rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					}
					else if (omfiIsAVaryValue(OMFFileHdl, levelSegment, &testErr))
					{
						IAAFVaryingValue* pVaryingValue = NULL;
						
						rc = defs.cdVaryingValue()->
							CreateInstance(IID_IAAFVaryingValue,
							(IUnknown **)&pVaryingValue);
						AutoRelease<IAAFVaryingValue> arVaryingValue(pVaryingValue);
						ConvertOMFVaryingValue(levelSegment, pParameterDef, pVaryingValue);
						rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					}
					if (pParameter)
					{
						AutoRelease<IAAFParameter> arParam(pParameter);
						rc = pEffect->AddParameter(pParameter);
					}
				}
			}
		}
		else if (strcmp(effectID, "omfi:effect:SMPTEVideoWipe") == 0)
		{
			//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
			//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			wipeNumber = 0;
			memset(&wipeArgs, 0, sizeof(wipeArgs));
			{
				rc = pDictionary->LookupTypeDef(kAAFTypeID_Int32, &pParameterDefType);
				{ // Use local block to narrow the scope of the AutoRelease object.
					AutoRelease<IAAFTypeDef> arTypeDef(pParameterDefType);
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefSMPTEWipeNumber, pParameterDefType, 
						L"Wipe Number", 
						L"SMPTE Wipe Number. No Default",
						L" ",
						&pParameterDef);
				}
				AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );
				rc = pEffectDef->AddParameterDef(pParameterDef);
				
				OMFError = omfeSMPTEVideoWipeGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &inputSegmentB, &levelSegment, &wipeNumber, &wipeArgs);
				if (wipeNumber)
				{
					IAAFConstantValue* pConstantValue = NULL;
					
					rc = defs.cdConstantValue()->
						CreateInstance(IID_IAAFConstantValue,
						(IUnknown **)&pConstantValue);
					AutoRelease<IAAFConstantValue> arCV(pConstantValue);
					rc = pConstantValue->Initialize (pParameterDef, sizeof(wipeNumber), (unsigned char *)&wipeNumber);
					rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					AutoRelease<IAAFParameter> arParam(pParameter);
					rc = pEffect->AddParameter(pParameter);
				}
			}
			
			{
				//
				IAAFConstantValue* pConstantValue = NULL;
				unsigned char	reverse = wipeArgs.reverse;

				rc = pDictionary->LookupTypeDef(kAAFTypeID_Boolean, &pParameterDefType);
				{ // Use local block to narrow the scope of the AutoRelease object.
					AutoRelease<IAAFTypeDef> arTypeDef(pParameterDefType);
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefSMPTEReverse, pParameterDefType, 
						L"Reverse", 
						L"SMPTE Reverse. Default FALSE",
						L" ",
						&pParameterDef);
				}
				AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );
				pEffectDef->AddParameterDef(pParameterDef);
				rc = defs.cdConstantValue()->
					CreateInstance(IID_IAAFConstantValue,
					(IUnknown **)&pConstantValue);
				AutoRelease<IAAFConstantValue> arCV(pConstantValue);
				rc = pConstantValue->Initialize (pParameterDef, sizeof(reverse), &reverse);
				rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
				AutoRelease<IAAFParameter> arParam(pParameter);
				rc = pEffect->AddParameter(pParameter);
			}
			
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
				// Need parameter definition before we create parameters.
				rc = pDictionary->LookupTypeDef(kAAFTypeID_Rational, &pParameterDefType);
				{	// Use local block to narrow the scope of the AutoRelease object.
					AutoRelease<IAAFTypeDef> arTypeDef(pParameterDefType);
					GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, pParameterDefType, 
						L"Level", 
						L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
						L" ",
						&pParameterDef);
				}
				AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );

				if (omfiIsAConstValue(OMFFileHdl, levelSegment, &testErr))
				{
					IAAFConstantValue* pConstantValue = NULL;
					
					rc = defs.cdConstantValue()->
						CreateInstance(IID_IAAFConstantValue,
						(IUnknown **)&pConstantValue);
					AutoRelease<IAAFConstantValue> arCV(pConstantValue);
					ConvertOMFConstValue(levelSegment, pParameterDef, pConstantValue);
					rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
				}
				else if (omfiIsAVaryValue(OMFFileHdl, levelSegment, &testErr))
				{
					IAAFVaryingValue* pVaryingValue = NULL;
					
					rc = defs.cdVaryingValue()->
						CreateInstance(IID_IAAFVaryingValue,
						(IUnknown **)&pVaryingValue);
					AutoRelease<IAAFVaryingValue> arVaryingValue(pVaryingValue);
					ConvertOMFVaryingValue(levelSegment, pParameterDef, pVaryingValue);
					rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
				}
				
				if (pParameter)
				{
					AutoRelease<IAAFParameter> arParam(pParameter);
					rc = pEffect->AddParameter(pParameter);
					rc = pEffectDef->AddParameterDef(pParameterDef); // Why isn't this called sooner like most of the othe blocks?
				}
			}
		}
		else if ((strcmp(effectID, "omfi:effect:MonoAudioDissolve") == 0) ||
				 (strcmp(effectID, "omfi:effect:SimpleMonoAudioDissolve") == 0))
		{
//			keyFrameSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_KEYFRAME_SLOT;
//			globalSlot = OMF2_EFFE_PUBLIC_WITH_AVID_PRIVATE_DATA_GLOBAL_SLOT;
			rc = pDictionary->LookupTypeDef(kAAFTypeID_Rational, &pParameterDefType);
			{	// Use local block to narrow the scope of the AutoRelease object.
				AutoRelease<IAAFTypeDef> arTypeDef(pParameterDefType);
				GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, pParameterDefType, 
										L"Level", 
										L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
										L" ",
										&pParameterDef);
			}
			AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );
			rc = pEffectDef->AddParameterDef(pParameterDef);

			if(OM_ERR_NONE == omfeMonoAudioDissolveGetInfo(OMFFileHdl, effect, &effectLength, &inputSegmentA, &inputSegmentB, &levelSegment))
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
					if (omfiIsAConstValue(OMFFileHdl, levelSegment, &testErr))
					{
						IAAFConstantValue* pConstantValue = NULL;
						
						rc = defs.cdConstantValue()->
							CreateInstance(IID_IAAFConstantValue,
							(IUnknown **)&pConstantValue);
						AutoRelease<IAAFConstantValue> arCV(pConstantValue);
						ConvertOMFConstValue(levelSegment, pParameterDef, pConstantValue);
						rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					}
					else if (omfiIsAVaryValue(OMFFileHdl, levelSegment, &testErr))
					{
						IAAFVaryingValue*	pVaryingValue = NULL;
						
						rc = defs.cdVaryingValue()->
							CreateInstance(IID_IAAFVaryingValue,
							(IUnknown **)&pVaryingValue);
						AutoRelease<IAAFVaryingValue> arVaryingValue(pVaryingValue);
						ConvertOMFVaryingValue(levelSegment, pParameterDef, pVaryingValue);
						rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
					}

					if (pParameter)
					{
						AutoRelease<IAAFParameter> arParam(pParameter);
						rc = pEffect->AddParameter(pParameter);
					}
				}
			}
			else
				OMFError = OM_ERR_NONE;

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
			omfIterHdl_t		OMFIterator;
			omfESlotObj_t		OMFEffectSlot;
			omfArgIDType_t	argID;
			omfSegObj_t		argValue;
			omfNumSlots_t		numSlots;

//			keyFrameSlot = OMF2_EFFE_ALLOTHERS_KEYFRAME_SLOT;
//			globalSlot = OMF2_EFFE_ALLOTHERS_GLOBAL_SLOT;
			gpGlobals->pLogger->Log( kLogInfo,
				"%sGeneric OMF Effect = %s\n ", gpGlobals->indentLeader, effectID);
			// this is just as default parameter definition !!!
			rc = pDictionary->LookupTypeDef(kAAFTypeID_Rational, &pParameterDefType);
			{ // Use local block to narrow the scope of the AutoRelease object.
				AutoRelease<IAAFTypeDef> tdef( pParameterDefType );
				GetParameterDefinition((aafUID_t *)&kAAFParameterDefLevel, pParameterDefType, 
										L"Level", 
										L"Level, equal to mix ratio of B/A. Range is 0 to 1. The formula  P = (Level*B)+((1-Level)*A)",
										L" ",
										&pParameterDef);
			}
			AutoRelease< IAAFParameterDef > pparamdef( pParameterDef );
			rc = pEffectDef->AddParameterDef(pParameterDef);
		
			OMFError = omfiEffectGetNumSlots(OMFFileHdl, effect, &numSlots);;
			if (numSlots > 0)
			{
				omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
				OMFError = omfiEffectGetNextSlot(OMFIterator, effect, NULL, &OMFEffectSlot);
				while(OMFEffectSlot != NULL)
				{
					OMFError = omfiEffectSlotGetInfo(OMFFileHdl, OMFEffectSlot, &argID, &argValue);
					if (argValue /* && (argID != keyFrameSlot) */)
					{
						if (omfiIsAConstValue(OMFFileHdl, argValue, &testErr))
						{
							IAAFConstantValue* pConstantValue = NULL;
							
							rc = defs.cdConstantValue()->
								CreateInstance(IID_IAAFConstantValue,
								(IUnknown **)&pConstantValue);
							AutoRelease<IAAFConstantValue> arCV(pConstantValue);
							ConvertOMFConstValue(argValue, pParameterDef, pConstantValue);
							rc = pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
							AutoRelease<IAAFParameter> arParam(pParameter);
							rc = pEffect->AddParameter(pParameter);
						}
						else if (omfiIsAVaryValue(OMFFileHdl, argValue, &testErr))
						{
							IAAFVaryingValue*	pVaryingValue = NULL;
							
							rc = defs.cdVaryingValue()->
								CreateInstance(IID_IAAFVaryingValue,
								(IUnknown **)&pVaryingValue);
							AutoRelease<IAAFVaryingValue> arVaryingValue(pVaryingValue);
							ConvertOMFVaryingValue(argValue, pParameterDef, pVaryingValue);
							rc = pVaryingValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter);
							AutoRelease<IAAFParameter> arParam(pParameter);
							rc = pEffect->AddParameter(pParameter);
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
					(void)omfiEffectGetNextSlot(OMFIterator, effect, NULL, &OMFEffectSlot);
				}
				OMFError = omfiIteratorDispose(OMFFileHdl, OMFIterator);
			}
			
		}

	}
	else
	{
		// Handle 1.x effects
		char				classID[5];

		OMFError = omfsReadClassID(OMFFileHdl, effect, OMObjID, classID);
		classID[4] = '\0';
		gpGlobals->pLogger->Log( kLogInfo,
			"%sOMF 1.x Effect = %s\n ", gpGlobals->indentLeader, classID);
	}


	pEffect->Initialize(effectDataDef, (aafLength_t)effectLength, pEffectDef);
	pEffect->SetBypassOverride(bypassOverride);
	
	if(omfiEffectGetFinalRender(OMFFileHdl, effect, &renderClip) == OM_ERR_NONE)
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
void Omf2Aaf::ConvertOMFEffectDefinition(omfDDefObj_t	effectDef,
											omfObject_t effect,
											IAAFOperationDef**	ppEffectDef)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	
	omfArgIDType_t	bypassOverride;
	omfBool			isTimeWarp;
	omfInt32			nameSize = 64, idSize = 64, descSize = 120;
	omfUniqueName_t	effectID, MCEffectID;
	char					effectName[64], descBuffer[120];
	
	aafUID_t				effectDataDef;
	aafInt32				numberInputs;
	
	
	OMFError = omfiEffectDefGetInfo(OMFFileHdl,
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
	
	if(omfsReadString(OMFFileHdl, effect, gpGlobals->pvtEffectIDProp,
								MCEffectID, sizeof(MCEffectID)) != OM_ERR_NONE)
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

	IAAFParameterDef*	ptmpParameterDef = NULL;

	// Verify that we did not got any NULL pointers
	if ( (pDefUID == NULL) || (pTypeDef == NULL) || // the type definition is a required property for parameter defintions!
//	if ( (pDefUID == NULL) ||  
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
		AutoRelease<IAAFParameterDef> arPropertyDef( ptmpParameterDef );
		rc = ptmpParameterDef->Initialize(*pDefUID, pwName, pwDesc, pTypeDef);
		rc = ptmpParameterDef->SetDisplayUnits(pwDisplayUnits);
		rc = pDictionary->RegisterParameterDef(ptmpParameterDef);
		ptmpParameterDef->AddRef (); // bump the refCount to 2 so auto release will reduce it back to one.
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
void Omf2Aaf::GetAAFOperationDefinition(omfUniqueName_t effectID,
										   omfUniqueName_t MCEffectID,
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
//!!!		(*ppEffectDef)->SetCategory(pwName);
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


void Omf2Aaf::ConvertObjectProps(omfObject_t pOMFObject, aafUID_t &classID, IAAFObject* pObj)
{
}		
