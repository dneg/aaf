// @doc INTERNAL
// @com This file implements the conversion of OMF files to AAF file format.
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/
/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

// AAF Utilities Infra-structure 
#include "UtlConsole.h"
// OMF Includes


#include "AafOmf.h"

#include "Aaf2Omf.h"

extern AafOmfGlobals* gpGlobals;
// ============================================================================
// Constructor
// ============================================================================
Aaf2Omf::Aaf2Omf() : pFile(NULL), pHeader(NULL), pDictionary(NULL)
{
}
// ============================================================================
// Destructor
// ============================================================================
Aaf2Omf::~Aaf2Omf()
{
}
// ============================================================================
// ConvertFile
//			This function provides the control and logic to convert an AAF file
//			by calling all the functions inside this object
//
// ============================================================================
HRESULT Aaf2Omf::ConvertFile ()
{
	HRESULT		rc = AAFRESULT_SUCCESS;

	rc = OpenInputFile();
	if (SUCCEEDED(rc))
	{
		rc = OpenOutputFile();
		if (SUCCEEDED(rc))
		{
//			rc = ConvertAAFHeader();
//			if (SUCCEEDED(rc))
			{
				rc = AAFFileRead();
			}
		}
	}

	if (gpGlobals->bOMFFileOpen)
	{
		CloseOutputFile();
		gpGlobals->bOMFFileOpen = AAFFalse;
	}

	if (gpGlobals->bAAFFileOpen)
	{
		CloseInputFile();
		gpGlobals->bAAFFileOpen = AAFFalse;
	}

	return (rc);
}
// ============================================================================
// ConvertAUIDtoUID
//			This function reduces an AUID into an OMF UID.
//
// ============================================================================
void Aaf2Omf::ConvertAUIDtoUID(aafUID_t* pMobID, 
							   OMF2::omfUID_t* pOMFMobID)
{
	struct SMPTELabel
	{
		aafUInt32	MobIDMajor;
		aafUInt32	MobIDMinor;
		aafUInt8	oid;
		aafUInt8	size;
		aafUInt8	ulcode;
		aafUInt8	SMPTE;
		aafUInt8	Registry;
		aafUInt8	unused;
		aafUInt16	MobIDPrefix;
	};

	union label
	{
		aafUID_t			auid;
		struct SMPTELabel	smpte;
	};

	union label aLabel;
	memcpy((void *)&aLabel.auid, pMobID, sizeof(aafUID_t));

	pOMFMobID->prefix = aLabel.smpte.MobIDPrefix;
	pOMFMobID->major = aLabel.smpte.MobIDMajor;
	pOMFMobID->minor = aLabel.smpte.MobIDMinor;
}
// ============================================================================
// OpenInputFile
//			This function determines an AAF file for read.
//			If the file does not exists or it is not an AAF file an
//			error code will be returned.
//
// ============================================================================
HRESULT Aaf2Omf::OpenInputFile ()
{
	HRESULT						rc = AAFRESULT_SUCCESS;
	aafWChar*					pwFileName = NULL;
	IAAFIdentification*			pIdent = NULL;

	// convert file name to Wide char
	UTLStrAToStrW(gpGlobals->sInFileName, &pwFileName);


	rc = AAFFileOpenExistingRead(pwFileName, 0, &pFile);
	if (FAILED(rc))
	{
		UTLMemoryFree(pwFileName);
		return rc;
	}

	rc = pFile->GetHeader(&pHeader);
	if (FAILED(rc))
	{
		UTLMemoryFree(pwFileName);
		return rc;
	}

	rc = pHeader->GetDictionary(&pDictionary);
	if (FAILED(rc))
	{
		UTLMemoryFree(pwFileName);
		return rc;
	}

	gpGlobals->bAAFFileOpen = AAFTrue;
	UTLMemoryFree(pwFileName);

	return rc;
}
// ============================================================================
// OpenOutputFile
//			This function creates the output file.
//
// ============================================================================
HRESULT Aaf2Omf::OpenOutputFile ()
{
	HRESULT								rc = AAFRESULT_SUCCESS;
	aafBool								bOverride = AAFFalse;
	aafBool								bSessionStarted = AAFFalse;

	OMF2::omfProductIdentification_t	OMFProductInfo;
	char*								pszCompanyName = NULL;
	char*								pszProductName = NULL;
    char*								pszProductVersionString = NULL;
    char*								pszPlatform;
	
	IAAFIdentification*					pIdent = NULL;
	aafWChar*							pwCompanyName = NULL;
	aafWChar*							pwProductName = NULL;
    aafWChar*							pwProductVersionString = NULL;
    aafWChar*							pwPlatform;
	aafUInt32							textSize = 0;
	aafProductVersion_t					productVersion;

	if (strlen(gpGlobals->sOutFileName) == 0)
	{
		char*	pExt;
		bOverride = AAFTrue;
		strcpy(gpGlobals->sOutFileName, gpGlobals->sInFileName);
		pExt= strrchr(gpGlobals->sOutFileName, '.');
		strcpy(pExt,".omf");
	}
	
	if (bOverride)
	{
		UTLstdprintf("Previous file: %s will be overwritten\n", gpGlobals->sOutFileName);
		deleteFile(gpGlobals->sOutFileName);
	}
	// Retrieve AAF file's last identification
	rc = pHeader->GetLastIdentification(&pIdent);
	pIdent->GetCompanyNameBufLen(&textSize);
	if (textSize > 0)
	{
		UTLMemoryAlloc(textSize, (void**)&pwCompanyName);
		pIdent->GetCompanyName(pwCompanyName, textSize);
		UTLStrWToStrA(pwCompanyName, &pszCompanyName);
	}
	else
		UTLStrWToStrA(L"<Not Specified>", &pszCompanyName);

	pIdent->GetProductNameBufLen(&textSize);
	if (textSize > 0)
	{
		UTLMemoryAlloc(textSize, (void**)&pwProductName);
		pIdent->GetProductName(pwProductName, textSize);
		UTLStrWToStrA(pwProductName, &pszProductName);
	}
	else
		UTLStrWToStrA(L"<Not Specified>", &pszProductName);

	pIdent->GetProductVersionStringBufLen(&textSize);
	if (textSize > 0)
	{
		UTLMemoryAlloc(textSize, (void**)&pwProductVersionString);
		pIdent->GetProductVersionString(pwProductVersionString, textSize);
		UTLStrWToStrA(pwProductVersionString, &pszProductVersionString);
	}
	else
		UTLStrWToStrA(L"<Not Specified>", &pszProductVersionString);

	pIdent->GetPlatformBufLen(&textSize);
	if (textSize > 0)
	{
		UTLMemoryAlloc(textSize, (void**)&pwPlatform);
		pIdent->GetPlatform(pwPlatform, textSize);
		UTLStrWToStrA(pwPlatform, &pszPlatform);
	}
	else
		UTLStrWToStrA(L"<Not Specified>", &pszPlatform);


	OMFProductInfo.companyName = pszCompanyName;
	OMFProductInfo.productName = pszProductName;
	OMFProductInfo.productVersionString = pszProductVersionString;
	OMFProductInfo.platform = pszPlatform;

	rc = pIdent->GetProductVersion(&productVersion);
	OMFProductInfo.productVersion.major = productVersion.major;
	OMFProductInfo.productVersion.minor = productVersion.minor;
	OMFProductInfo.productVersion.tertiary = productVersion.tertiary;
	OMFProductInfo.productVersion.patchLevel = productVersion.patchLevel;
	OMFProductInfo.productID = 42; // Comes from OMF !!!
	OMFProductInfo.productVersion.type = (OMF2::omfProductReleaseType_t)productVersion.type;

	rc = OMF2::omfsBeginSession(&OMFProductInfo, &OMFSession);
	if (rc != OMF2::OM_ERR_NONE )
	{
		rc = AAFRESULT_BADOPEN;
		goto Cleanup;
	}
	
	bSessionStarted = AAFTrue;
	rc = OMF2::omfmInit(OMFSession);
	if (rc != OMF2::OM_ERR_NONE )
	{
		OMF2::omfsEndSession(OMFSession);
		rc = AAFRESULT_BAD_SESSION;
		goto Cleanup;
	}

	rc = OMF2::omfsCreateFile((OMF2::fileHandleType)gpGlobals->sOutFileName, OMFSession, OMF2::kOmfRev2x, &OMFFileHdl);
	if (rc != OMF2::OM_ERR_NONE )
	{
		rc = AAFRESULT_BADOPEN;
		goto Cleanup;
	}
	

	gpGlobals->bOMFFileOpen = AAFTrue;
	// Clean up and exit 
Cleanup:
	if (pIdent)
		pIdent->Release();

	if (pwCompanyName)
		UTLMemoryFree(pwCompanyName);

	if (pszCompanyName)
		UTLMemoryFree(pszCompanyName);

	if (pwProductName)
		UTLMemoryFree(pwProductName);

	if (pszProductName)
		UTLMemoryFree(pszProductName);

	if (pwPlatform)
		UTLMemoryFree(pwPlatform);

	if (pszPlatform)
		UTLMemoryFree(pszPlatform);

	if (pwProductVersionString)
		UTLMemoryFree(pwProductVersionString);
	
	if (pszProductVersionString)
		UTLMemoryFree(pszProductVersionString);
	
	return AAFRESULT_SUCCESS;
}

// ============================================================================
// CloseOutputFile
//			This function closes the output file.
//
// ============================================================================
void Aaf2Omf::CloseOutputFile()
{
	HRESULT	rc;
	rc = OMF2::omfsCloseFile(OMFFileHdl);
	rc = OMF2::omfsEndSession(OMFSession);
}

// ============================================================================
// CloseInputFile
//			This function closes the input file.
//
// ============================================================================
void Aaf2Omf::CloseInputFile( )
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
		pFile->Close();
		pFile->Release();
		pFile = NULL;
	}
}
// ============================================================================
// AAFFileRead
//
//		Here is where all the real work is done.  
//
// ============================================================================
HRESULT Aaf2Omf::AAFFileRead()
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfMobObj_t		OMFMob = NULL;

	aafInt32				nAAFNumMobs;
	aafUInt32				numEssenceData = 0;
	IEnumAAFMobs*			pMobIter = NULL;
	IAAFMob*				pMob = NULL;
	IAAFCompositionMob*		pCompMob = NULL;
	IAAFMasterMob*			pMasterMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IEnumAAFEssenceData*	pEssenceDataIter = NULL;
	IAAFEssenceData*		pEssenceData = NULL;
	aafSearchCrit_t			criteria;

	rc = pHeader->GetNumMobs(kAllMob, &nAAFNumMobs);
	if (gpGlobals->bVerboseMode)
	{
		UTLstdprintf(" Found: %ld Mobs in the input file\n", nAAFNumMobs);
	}
	criteria.searchTag = kByMobKind;
	criteria.tags.mobKind = kAllMob;
	rc = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
	while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
	{
		aafInt32	nameLength = 0;
		aafWChar*	pwMobName = NULL;
		aafUID_t	MobID;
		char		szMobID[64];
		char*		pszMobName = NULL;

		gpGlobals->nNumAAFMobs++;
		// Get Mob Info
		pMob->GetNameBufLen(&nameLength);
		UTLMemoryAlloc(nameLength, (void**)&pwMobName);
		rc = pMob->GetName(pwMobName, nameLength);
		rc = pMob->GetMobID(&MobID);
		UTLStrWToStrA(pwMobName, &pszMobName);
		// Is this a composition Mob?
		rc = pMob->QueryInterface(IID_IAAFCompositionMob, (void **)&pCompMob);
		if (SUCCEEDED(rc))
		{
			// Composition Mob
			rc = ConvertCompositionMob(pCompMob, &OMFMob, pszMobName, &MobID);
			pCompMob->Release();
			pCompMob = NULL;
		}
		else
		{
			// Is it a Master Mob ?
			rc = pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob);
			if (SUCCEEDED(rc))
			{
				rc = ConvertMasterMob(pMasterMob, &OMFMob, pszMobName, &MobID);
				pMasterMob->Release();
				pMasterMob = NULL;
			}
			else
			{
				// Is it a Source Mob
				rc = pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob);
				if (SUCCEEDED(rc))
				{
					rc = ConvertSourceMob(pSourceMob, &OMFMob, pszMobName, &MobID);
					pSourceMob->Release();
					pSourceMob = NULL;
				}
				else
				{
					AUIDtoString(&MobID, szMobID);
					UTLstdprintf("Unrecognized Mob kind encountered ID: %s\n", szMobID);
					UTLerrprintf("Unrecognized Mob kind encountered ID: %s\n", szMobID);
				}
			}
		}
		if (OMFMob)
		{
			rc = TraverseMob(pMob, &OMFMob);
			if (SUCCEEDED(rc))
			{
				// NOw add user comments 
				aafUInt32	numComments = 0;

				rc = pMob->GetNumComments(&numComments);
				if (SUCCEEDED(rc) && (numComments > 0))
				{
					IEnumAAFMobComments*	pCommentIterator = NULL;
					aafMobComment_t*		pMobComment = NULL;
					rc = pMob->EnumAAFAllMobComments(&pCommentIterator);
					while ( (SUCCEEDED(rc)) && (SUCCEEDED(pCommentIterator->NextOne(pMobComment))))
					{
						char*	pszComment;
						char*	pszCommName;

						UTLStrWToStrA(pMobComment->category, &pszCommName);
						UTLStrWToStrA(pMobComment->comment, &pszComment);
						rc = OMF2::omfiMobAppendComment(OMFFileHdl, OMFMob, pszCommName, pszComment);
						UTLMemoryFree(pszCommName);
						UTLMemoryFree(pszComment);
					}
					pCommentIterator->Release();
				}
			}
		}
		UTLMemoryFree(pwMobName);
		UTLMemoryFree(pszMobName);
		gpGlobals->nNumOMFMobs++;
		pMob->Release();
		pMob = NULL;
	}
	if (pMobIter)
		pMobIter->Release();

	// Now get the media data.
	rc = pHeader->GetNumEssenceData(&numEssenceData);
	if (SUCCEEDED(rc) && numEssenceData > 0)
	{
		if (gpGlobals->bVerboseMode)
		{
			UTLstdprintf(" Found: %ld Essence data objects\n", numEssenceData);
		}
		rc = pHeader->EnumEssenceData(&pEssenceDataIter);
		while (SUCCEEDED(rc) && SUCCEEDED(pEssenceDataIter->NextOne(&pEssenceData)))
		{
			rc = ConvertEssenceDataObject(pEssenceData);
		}
		if (pEssenceDataIter)
		{
			pEssenceDataIter->Release();
			pEssenceDataIter = NULL;
		}
		if (pEssenceData)
		{
			pEssenceData->Release();
			pEssenceData = NULL;
		}
	}
	if (pMasterMob)
		pMasterMob->Release();
	if (pSourceMob)
		pSourceMob->Release();
	if (pCompMob)
		pCompMob->Release();
	return rc;
}
// ============================================================================
// ConvertCompositionMob
//
//			This function extracts all the properties of an AAF Composition MOB,
//			Creates an OMF Composition mob object, sets its properties and 
//			appends it to the OMF header. 
//
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ConvertCompositionMob(IAAFCompositionMob* pCompMob,
									   OMF2::omfMobObj_t* pOMFCompMob,
									   char* pMobName,
									   aafUID_t* pMobID)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfUID_t			OMFMobID;
	OMF2::omfDefaultFade_t	OMFFade;

	aafDefaultFade_t		DefaultFade;

	rc = OMF2::omfiCompMobNew(OMFFileHdl, pMobName, (OMF2::omfBool)AAFFalse, pOMFCompMob);
	if (OMF2::OM_ERR_NONE == rc)
	{
		ConvertAUIDtoUID(pMobID, &OMFMobID);
		rc = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFCompMob, OMFMobID);
		if (OMF2::OM_ERR_NONE == rc)
		{
			pCompMob->GetDefaultFade(&DefaultFade);
			if (DefaultFade.valid)
			{
				OMFFade.fadeLength = DefaultFade.fadeLength;
				switch (DefaultFade.fadeType)
				{
					case kFadeNone:
						OMFFade.fadeType = OMF2::kFadeNone;
						break;
					case kFadeLinearAmp:
						OMFFade.fadeType = OMF2::kFadeLinearAmp;
						break;
					case kFadeLinearPower:
						OMFFade.fadeType = OMF2::kFadeLinearPower;
						break;
				}
				OMFFade.fadeEditUnit.numerator = DefaultFade.fadeEditUnit.numerator;
				OMFFade.fadeEditUnit.denominator = DefaultFade.fadeEditUnit.denominator;

				rc = OMF2::omfiMobSetDefaultFade(OMFFileHdl,
												 *pOMFCompMob,
												 OMFFade.fadeLength, 
												 OMFFade.fadeType ,
												 OMFFade.fadeEditUnit);
			}
		}
	}

	return rc;
}
// ============================================================================
// ConvertMasterMob
//
//			This function extracts all the properties of an AAF Master MOB,
//			Creates an OMF Master mob object, sets its properties and 
//			appends it to the OMF header. 
//
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ConvertMasterMob(IAAFMasterMob* pMasterMob,
								  OMF2::omfMobObj_t* pOMFMasterMob,
								  char* pMobName,
								  aafUID_t* pMobID)
{
	HRESULT				rc = AAFRESULT_SUCCESS;
	OMF2::omfUID_t		OMFMobID;

	rc = OMF2::omfmMasterMobNew(OMFFileHdl, pMobName, (OMF2::omfBool)AAFTrue, pOMFMasterMob);
	if (OMF2::OM_ERR_NONE == rc)
	{
		ConvertAUIDtoUID(pMobID, &OMFMobID);
		rc = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFMasterMob, OMFMobID);
		if (OMF2::OM_ERR_NONE == rc)
		{
			if (gpGlobals->bVerboseMode)
				UTLstdprintf("Converted AAF Master MOB to OMF\n");
		}
	}
	return rc;
}

// ============================================================================
// ConvertSourceMob
//
//			This function extracts all the properties of an AAF Source MOB,
//			Creates an OMF Source mob object, sets its properties and 
//			appends it to the OMF header. 
//
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ConvertSourceMob(IAAFSourceMob* pSourceMob,
								  OMF2::omfMobObj_t* pOMFSourceMob,
								  char* pMobName,
								  aafUID_t* pMobID)
{
	HRESULT				rc = AAFRESULT_SUCCESS;
	OMF2::omfUID_t		OMFMobID;


	IAAFEssenceDescriptor*	pEssenceDesc = NULL;
	IAAFTapeDescriptor*		pTapeDesc = NULL;
	IAAFFileDescriptor*		pFileDesc = NULL;
	IAAFTIFFDescriptor*		pTiffDesc = NULL;
	IAAFWAVEDescriptor*		pWAVEDesc = NULL;
	IAAFAIFCDescriptor*		pAifcDesc = NULL;
	IAAFCDCIDescriptor*		pCDCIDesc = NULL;
	IAAFObject*				pAAFObject = NULL;

	if (gpGlobals->bVerboseMode)
		UTLstdprintf("Converting AAF Source MOB to OMF\n");

	rc = pSourceMob->GetEssenceDescriptor(&pEssenceDesc);
	if (FAILED(rc))
		return rc;

	IncIndentLevel();

	ConvertAUIDtoUID(pMobID, &OMFMobID);
	rc = pEssenceDesc->QueryInterface(IID_IAAFTapeDescriptor,(void **)&pTapeDesc);
	if (SUCCEEDED(rc))
	{
		// This is Tape Descriptor 
		// Therefore the Source Mob is a Tape Mob
		aafWChar*			pwManufacturer = NULL;
		char*				pszManufacturer = NULL;
		aafWChar*			pwModel = NULL;
		char*				pszModel = NULL;
		aafInt32			textSize;
		aafTapeCaseType_t	formFactor;
		aafTapeFormatType_t	tapeFormat;
		aafLength_t			tapeLength;
		aafVideoSignalType_t videoSignal;

		rc = OMF2::omfmTapeMobNew(OMFFileHdl, pMobName, pOMFSourceMob);
		if (OMF2::OM_ERR_NONE == rc)
		{
			rc = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			pTapeDesc->GetTapeFormFactor(&formFactor);
			pTapeDesc->GetSignalType(&videoSignal);
			pTapeDesc->GetTapeFormat(&tapeFormat);
			pTapeDesc->GetTapeLength(&tapeLength);
			pTapeDesc->GetTapeManBufLen(&textSize);
			if (textSize > 0)
			{
				UTLMemoryAlloc(textSize, (void**)&pwManufacturer);
				pTapeDesc->GetTapeManufacturer(pwManufacturer, textSize);
				UTLStrWToStrA(pwManufacturer, &pszManufacturer);
			}
			else
				pszManufacturer = NULL;
			pTapeDesc->GetTapeModelBufLen(&textSize);
			if (textSize > 0)
			{
				UTLMemoryAlloc(textSize, (void**)&pwModel);
				pTapeDesc->GetTapeModel(pwModel, textSize);
				UTLStrWToStrA(pwModel, &pszModel);
			}
			else
				pszModel = NULL;

			rc = OMF2::omfmTapeMobSetDescriptor(OMFFileHdl,
												*pOMFSourceMob,
												(OMF2::omfTapeCaseType_t *)&formFactor,
												(OMF2::omfVideoSignalType_t *)&videoSignal,
												(OMF2::omfTapeFormatType_t *)&tapeFormat,
												(OMF2::omfLength_t *)&tapeLength,
												pszManufacturer,
												pszModel);
			if (gpGlobals->bVerboseMode)
				UTLstdprintf("%sAdded a Tape Essence Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			if (pwManufacturer)
				UTLMemoryFree(pwManufacturer);
			if (pszManufacturer)
				UTLMemoryFree(pszManufacturer);
			if (pwModel)
				UTLMemoryFree(pwModel);
			if (pszModel)
				UTLMemoryFree(pszModel);
		goto Cleanup;
		}
	}

	rc = pEssenceDesc->QueryInterface(IID_IAAFFileDescriptor,(void **)&pFileDesc);
	if (SUCCEEDED(rc))
	{
		// This is File Descriptor
		OMF2::omfRational_t	rate;
		aafRational_t		sampleRate;
		aafLength_t		length;	

		pFileDesc->GetSampleRate(&sampleRate);
		pFileDesc->GetLength(&length);
		rate.numerator = sampleRate.numerator;
		rate.denominator = sampleRate.denominator;
		rc = pEssenceDesc->QueryInterface(IID_IAAFTIFFDescriptor, (void **)&pTiffDesc);
		if (SUCCEEDED(rc))
		{
			// It is a TIFF file descriptor
			OMF2::omfDDefObj_t	datakind;
			OMF2::omfObject_t	mediaDescriptor;
			aafInt32			leadingLines, trailingLines;
			aafJPEGTableID_t	JPEGTableID;
			aafDataValue_t		pSummary;
			aafUInt32			summarySize = 0;
			aafBool				IsUniform;
			aafBool				IsContiguous;

			// retrieve TIFF descriptor properties
			rc = pTiffDesc->GetSummaryBufferSize(&summarySize);
			UTLMemoryAlloc(summarySize, (void **)&pSummary);
			rc = pTiffDesc->GetSummary(summarySize, pSummary);
			rc = pTiffDesc->GetTrailingLines(&trailingLines);
			rc = pTiffDesc->GetLeadingLines(&leadingLines);
			rc = pTiffDesc->GetIsUniform(&IsUniform);
			rc = pTiffDesc->GetIsContiguous(&IsContiguous);
			rc = pTiffDesc->GetJPEGTableID(&JPEGTableID);

			// Create a new OMF TIFF File Descriptor
			rc = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_TIFF_VIDEO, pOMFSourceMob);
			rc = OMF2::omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			rc = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			rc = OMF2::omfsWriteLength(OMFFileHdl, mediaDescriptor, OMF2::OMMDFLLength, (OMF2::omfLength_t)length); 
			if (rc)
			{
				char* pErrString = OMF2::omfsGetErrorString((OMF2::omfErr_t)rc);
				goto Cleanup;
			}
			if (gpGlobals->bVerboseMode)
				UTLstdprintf("%sAdded a Tiff Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);

			// Set OMF TIFF File Descriptor properties
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&rc);
			rc = OMF2::omfsWriteBoolean( OMFFileHdl,
										mediaDescriptor,
										OMF2::OMTIFDIsContiguous, 
										(OMF2::omfBool)IsContiguous);
			rc = OMF2::omfsWriteBoolean( OMFFileHdl,
										mediaDescriptor,
										OMF2::OMTIFDIsUniform,
										(OMF2::omfBool)IsUniform);
			rc = OMF2::omfsWriteDataValue(OMFFileHdl,
										 mediaDescriptor,
										 OMF2::OMTIFDSummary,
										 datakind,
										 (OMF2::omfDataValue_t)pSummary,
										 (OMF2::omfPosition_t)0,
										 summarySize);
			rc = OMF2::omfsWriteJPEGTableIDType( OMFFileHdl,
								 				 mediaDescriptor,
												 OMF2::OMTIFDJPEGTableID, 
												 (OMF2::omfJPEGTableID_t)JPEGTableID);
			rc = OMF2::omfsWriteInt32(OMFFileHdl,
									 mediaDescriptor,
									 OMF2::OMTIFDLeadingLines, 
									 leadingLines);
			rc = OMF2::omfsWriteInt32(OMFFileHdl,
									 mediaDescriptor,
									 OMF2::OMTIFDTrailingLines, 
									 trailingLines);
			UTLMemoryFree(pSummary);
			goto Cleanup;
		}
		rc = pEssenceDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&pWAVEDesc);
		if (SUCCEEDED(rc))
		{
			// It is a WAVE file descriptor
			OMF2::omfDDefObj_t	datakind;
			OMF2::omfObject_t	mediaDescriptor;
			aafDataValue_t*		pSummary;
			aafUInt32			summarySize = 0;

			// retrieve WAVE descriptor properties
			rc = pWAVEDesc->GetSummaryBufferSize(&summarySize);
			UTLMemoryAlloc(summarySize, (void **)&pSummary);
			rc = pWAVEDesc->GetSummary(summarySize, *pSummary);

			//Create a new WAVE File Descriptor
			rc = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_WAVE_AUDIO, pOMFSourceMob);
			rc = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			rc = OMF2::omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (OMF2::omfErr_t *)&rc);
			rc = OMF2::omfsWriteDataValue(OMFFileHdl,
										 mediaDescriptor,
										 OMF2::OMWAVDSummary,
										 datakind,
										 (OMF2::omfDataValue_t)pSummary,
										 (OMF2::omfPosition_t)0,
										 summarySize);
			if (gpGlobals->bVerboseMode)
				UTLstdprintf("%sAdded a Wave Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			goto Cleanup;
		}
		rc = pEssenceDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&pAifcDesc);
		if (SUCCEEDED(rc))
		{
			// It is a AIFC file descriptor
			OMF2::omfObject_t	mediaDescriptor;
			aafDataValue_t		pSummary;
			aafUInt32			summarySize = 0;

			// retrieve AIFC descriptor properties
			rc = pAifcDesc->GetSummaryBufferSize(&summarySize);
			UTLMemoryAlloc(summarySize, (void **)&pSummary);
			rc = pAifcDesc->GetSummary(summarySize, pSummary);
			
			rc = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_AIFC_AUDIO, pOMFSourceMob);
			rc = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			rc = OMF2::omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			rc = OMF2::omfsWriteLength(OMFFileHdl, mediaDescriptor, OMF2::OMMDFLLength, (OMF2::omfLength_t)length); 
			if (gpGlobals->bVerboseMode)
				UTLstdprintf("%sAdded an AIFC Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			UTLMemoryFree(pSummary);
			goto Cleanup;
		}
		rc = pEssenceDesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&pCDCIDesc);
		if (SUCCEEDED(rc))
		{
			// It is a CDCI file descriptor
			rc = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_CDCI_VIDEO, pOMFSourceMob);
			rc = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			if (gpGlobals->bVerboseMode)
				UTLstdprintf("%sAdded a CDCI Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			goto Cleanup;
		}
		rc = pEssenceDesc->QueryInterface(IID_IAAFObject, (void **)&pAAFObject);
		if (SUCCEEDED(rc))
		{
			// Media descriptor not implemented or not recognized
			aafUID_t	ObjClass;
			char		szTempUID[64];

			pAAFObject->GetObjectClass(&ObjClass);
			AUIDtoString(&ObjClass ,szTempUID);
			if (gpGlobals->bVerboseMode)
				UTLstdprintf("%sInvalid essence descripor type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
			UTLerrprintf("%sInvalid essence descriptor type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
		}
		goto Cleanup;
	}

Cleanup:

	if (pOMFSourceMob)
		rc = ConvertLocator(pEssenceDesc, pOMFSourceMob);
	if (pTapeDesc)
		pTapeDesc->Release();

	if (pFileDesc)
		pFileDesc->Release();

	if (pTiffDesc)
		pTiffDesc->Release();

	if (pWAVEDesc)
		pWAVEDesc->Release();

	if (pAifcDesc)
		pAifcDesc->Release();

	if (pCDCIDesc)
		pCDCIDesc->Release();

	DecIndentLevel();
	return rc;
}
// ============================================================================
// TraverseMob
//
//			This function inspects the given AAF Mopb for components and recursively
//			visits and convert every component within the MOB. 
//			The objects found are converted to OMF and attached to the OMF object.
//
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::TraverseMob(IAAFMob* pMob,
							 OMF2::omfMobObj_t* pOMFMob)
{
	HRESULT				rc = AAFRESULT_SUCCESS;
	
	OMF2::omfMSlotObj_t		OMFNewSlot;
	OMF2::omfSegObj_t		OMFSegment;
	OMF2::omfTrackID_t		OMFTrackID;
	OMF2::omfRational_t		OMFeditRate;
	OMF2::omfPosition_t		OMFOrigin;
	char*					pszTrackName = NULL;
	aafWChar*				pwTrackName = NULL;					

	IAAFComponent*			pComponent = NULL;
	IAAFMobSlot*			pSlot = NULL;
	IAAFTimelineMobSlot*	pTimelineMobSlot = NULL;
	IAAFSegment*			pSegment = NULL;
	IEnumAAFMobSlots*		pSlotIter = NULL;
	aafNumSlots_t			numSlots;
	aafSearchCrit_t			criteria;
	aafInt32				textSize;

	rc = pMob->GetNumSlots(&numSlots);
	if (FAILED(rc))
		return rc;

	rc = pMob->EnumAAFAllMobSlots(&pSlotIter);
	if (FAILED(rc))
		return rc;
	
	IncIndentLevel();
	if (gpGlobals->bVerboseMode)
	{
		UTLstdprintf("%sFound: %ld sub slots\n", gpGlobals->indentLeader, numSlots);
	}

	for (aafInt32 times = 0; times< numSlots; times++)
	{
		rc = pSlotIter->NextOne(&pSlot);
		if (AAFRESULT_SUCCESS == rc)
		{
			//	Process Segment data
			pSlot->GetNameBufLen(&textSize);
			UTLMemoryAlloc(textSize, (void**)&pwTrackName);
			pSlot->GetName(pwTrackName, textSize);
			UTLStrWToStrA(pwTrackName, &pszTrackName);
			pSlot->GetSlotID( (aafSlotID_t *)&OMFTrackID);
			rc = pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void **)&pTimelineMobSlot);
			if (SUCCEEDED(rc))
			{
				pTimelineMobSlot->GetOrigin((aafPosition_t *)&OMFOrigin);
				pTimelineMobSlot->GetEditRate((aafRational_t *)&OMFeditRate);
				pTimelineMobSlot->Release();
				pTimelineMobSlot = NULL;
			}
			rc = pSlot->GetSegment(&pSegment);
			if (FAILED(rc))
				break;
			pSegment->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
			ProcessComponent(pComponent, &OMFSegment);

			rc = OMF2::omfiMobAppendNewTrack(OMFFileHdl,
											 *pOMFMob,
											 OMFeditRate,
											 OMFSegment,
											 OMFOrigin,
											 OMFTrackID,
											 pszTrackName, 
											 &OMFNewSlot);
			if (rc == OMF2::OM_ERR_NONE )
			{
				if (gpGlobals->bVerboseMode)
				{
					UTLstdprintf("%sConverted SlotID: %d, Name: %s\n",gpGlobals->indentLeader, (int)OMFTrackID, pszTrackName);
				}
			}
			if (pszTrackName)
			{
				UTLMemoryFree(pszTrackName);
				pszTrackName = NULL;
			}
			if (pwTrackName)
			{
				UTLMemoryFree(pwTrackName);
				pwTrackName = NULL;
			}
		}
	}
	DecIndentLevel();
	pSlotIter->Release();
	pSlotIter = NULL;

	return rc;
}

// ============================================================================
// ProcessComponent
//
//			This function will :
//				1. Identify type of AAF Component
//				2. Create the equivalent OMF object
//				3. Convert the AAF datakind to OMF datadef of the object
//				4. Traverse the component (if needed) into its objects
//				5. return the OMF segment 
//
//	INPUTS:		pComponent	Pointer to a AAF Component Interface pointer
//				pOMFSegment	Pointer an OMF object to be created.
//
//	OUTPUTS:	pOMFSegment	new OMF Segment	
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ProcessComponent(IAAFComponent* pComponent, 
								  OMF2::omfObject_t* pOMFSegment)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfDDefObj_t		OMFDatakind;

	IAAFSequence*			pSequence = NULL;
	IAAFSourceClip*			pSourceClip = NULL;
	IAAFTimecode*			pTimecode = NULL;
	IAAFEdgecode*			pEdgecode = NULL;
	IAAFFiller*				pFiller = NULL;
	IAAFTransition*			pTransition = NULL;
	IAAFSelector*			pSelector = NULL;
	IAAFGroup*				pEffect = NULL;
	IAAFEffectDef*			pEffectDef = NULL;
	IAAFParameterDef*		pParameterDef = NULL;
	IAAFDefObject*			pDefObject = NULL;

	aafUID_t				datadef;
	aafLength_t				length;

	IncIndentLevel();
	
	rc = pComponent->GetDataDef(&datadef);
	if (FAILED(rc))
		return rc;
	
	rc = ConvertAAFDatadef(datadef, &OMFDatakind);
	if (FAILED(rc))
		return rc;
	rc = pComponent->GetLength(&length);
	rc = pComponent->QueryInterface(IID_IAAFSequence, (void **)&pSequence);
	if (SUCCEEDED(rc))
	{
		// Component is a sequence
		rc = OMF2::omfiSequenceNew(OMFFileHdl, OMFDatakind, pOMFSegment);
		if (gpGlobals->bVerboseMode)
			UTLstdprintf("%sProcessing Sequence\n", gpGlobals->indentLeader);
		TraverseSequence(pSequence, pOMFSegment);
		goto Cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFSourceClip, (void **)&pSourceClip);
	if (SUCCEEDED(rc))
	{
		// component is a source clip
		aafSourceRef_t			ref;
		OMF2::omfSourceRef_t	OMFSourceRef;
		aafFadeType_t			fadeInType, fadeOutType;
		aafLength_t				fadeInLen, fadeOutLen;
		aafBool					fadeInPresent, fadeOutPresent;

		if (gpGlobals->bVerboseMode)
		{
			UTLstdprintf("%sProcessing Source Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		// Get Source Clip properties
		pSourceClip->GetSourceReference(&ref);
		pSourceClip->GetFade(&fadeInLen,
							 &fadeInType,
							 &fadeInPresent,
							 &fadeOutLen,
							 &fadeOutType,
							 &fadeOutPresent);
		ConvertAUIDtoUID(&ref.sourceID, &OMFSourceRef.sourceID);
		OMFSourceRef.sourceTrackID = (OMF2::omfTrackID_t)ref.sourceSlotID;
		OMFSourceRef.startTime = (OMF2::omfPosition_t)ref.startTime;
		// Create OMF Source Clip
		rc = OMF2::omfiSourceClipNew(OMFFileHdl,
									 OMFDatakind,
									 (OMF2::omfLength_t)length,
									 OMFSourceRef,
									 pOMFSegment);
		if (OMF2::OM_ERR_NONE== rc)
		{
			if (fadeInPresent || fadeOutPresent)
				rc = OMF2::omfiSourceClipSetFade(OMFFileHdl, 
												 *pOMFSegment,
												 fadeInLen,
												 (OMF2::omfFadeType_t)fadeInType,
												 fadeOutLen,
												 (OMF2::omfFadeType_t)fadeOutType);
		}
		goto Cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFTimecode, (void **)&pTimecode);
	if (SUCCEEDED(rc))
	{
		// component is a Timecode
		OMF2::omfTimecode_t	timecode;
		aafTimecode_t		AAFTimecode;

		pTimecode->GetTimecode(&AAFTimecode);
		timecode.startFrame = AAFTimecode.startFrame;
		timecode.drop = (OMF2::omfDropType_t)AAFTimecode.drop;
		timecode.fps = AAFTimecode.fps;
		if (gpGlobals->bVerboseMode)
		{
			UTLstdprintf("%sProcessing Timecode Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
			IncIndentLevel();
			UTLstdprintf("%sstart Frame\t: %ld\n", gpGlobals->indentLeader, timecode.startFrame);
			if (timecode.drop == AAFTrue)
				UTLstdprintf("%sdrop\t\t: True\n", gpGlobals->indentLeader);
			else
				UTLstdprintf("%sdrop\t\t: False\n", gpGlobals->indentLeader);
			UTLstdprintf("%sFrames/second\t: %ld\n", gpGlobals->indentLeader, timecode.fps);     
			DecIndentLevel();				
		}

		rc = OMF2::omfiTimecodeNew(OMFFileHdl, (OMF2::omfLength_t)length, timecode, pOMFSegment);
		goto Cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFEdgecode, (void **)&pEdgecode);
	if (SUCCEEDED(rc))
	{
		// component is an Edgecode
		aafEdgecode_t		edgecode;
		OMF2::omfEdgecode_t	OMFEdgecode;

		pEdgecode->GetEdgecode(&edgecode);
		OMFEdgecode.startFrame = edgecode.startFrame;
		OMFEdgecode.filmKind = (OMF2::omfFilmType_t)edgecode.filmKind;
		OMFEdgecode.codeFormat = (OMF2::omfEdgeType_t)edgecode.codeFormat;
		memcpy(OMFEdgecode.header, edgecode.header, sizeof(edgecode.header));
		if (gpGlobals->bVerboseMode)
		{
			UTLstdprintf("%sProcessing Timecode Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
			IncIndentLevel();
			UTLstdprintf("%sstart Frame\t: %ld\n", gpGlobals->indentLeader, edgecode.startFrame);
			DecIndentLevel();				
		}
		rc = OMF2::omfiEdgecodeNew(OMFFileHdl, (OMF2::omfLength_t)length, OMFEdgecode, pOMFSegment);		
		goto Cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFFiller, (void **)&pFiller);
	if (SUCCEEDED(rc))
	{
		// component is a filler
		rc = OMF2::omfiFillerNew(OMFFileHdl, OMFDatakind, (OMF2::omfLength_t)length, pOMFSegment);
		if (gpGlobals->bVerboseMode)
		{
			UTLstdprintf("%sProcessing Filler of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		goto Cleanup;
	}
	rc = pComponent->QueryInterface(IID_IAAFTransition, (void **)&pTransition);
	if (SUCCEEDED(rc))
	{
		// component is a Transition
		OMF2::omfEffObj_t	effect;
		OMF2::omfEDefObj_t	effectDef;
		OMF2::omfUniqueName_t effectID;

		aafPosition_t	cutPoint;

		if (gpGlobals->bVerboseMode)
		{
			UTLstdprintf("%sProcessing Transition of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		pTransition->GetCutPoint(&cutPoint);
		rc = pTransition->GetEffect(&pEffect);
		rc = pEffect->GetEffectDefinition(&pEffectDef);
		// At this time (4/99) effects are not implemented therefore we 
		// will have to create an Effect from thin air.(hack it !!)
		strcpy(effectID, "omfi:effect:VideoDissolve");
		rc = OMF2::omfiEffectDefNew(OMFFileHdl,
									effectID,
									NULL,
									NULL,
									NULL,
									(OMF2::omfBool)AAFFalse,
									&effectDef);

		rc = OMF2::omfiEffectNew(OMFFileHdl,
								 OMFDatakind,
								 (OMF2::omfLength_t)length,
								 effectDef,
								 &effect);

		rc = OMF2::omfiTransitionNew(OMFFileHdl,
									 OMFDatakind,
									 (OMF2::omfLength_t)length,
									 (OMF2::omfPosition_t)cutPoint,
									 effect,
									 pOMFSegment);
		goto Cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFSelector, (void **)&pSelector);
	if (SUCCEEDED(rc))
	{
		// component is a selector
		IncIndentLevel();
		
		rc = OMF2::omfiSelectorNew(OMFFileHdl,
								   OMFDatakind,
								   (OMF2::omfLength_t)length,
								   pOMFSegment);
		rc = ConvertSelector(pSelector, pOMFSegment);
		DecIndentLevel();
		goto Cleanup;
	}

	char szTempUID[64];
	AUIDtoString(&datadef ,szTempUID);
	if (gpGlobals->bVerboseMode)
		UTLstdprintf("%sInvalid component type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
	UTLerrprintf("%sInvalid component type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);

Cleanup:

	if (pSequence)
		pSequence->Release();
	
	if (pSourceClip)
		pSourceClip->Release();
	
	if (pTimecode)
		pTimecode->Release();

	if (pFiller)
		pFiller->Release();
	
	if (pEdgecode)
		pEdgecode->Release();

	if (pTransition)
		pTransition->Release();

	if (pSelector)
		pSelector->Release();

	DecIndentLevel();
	return rc;
}

// ============================================================================
// ConvertAAFDatadef
//
//			This function converts an AAF datadef into an OMF datakind. 
//			
// Returns: AAFRESULT_SUCCESS if datakind is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ConvertAAFDatadef(aafUID_t Datadef,
								   OMF2::omfDDefObj_t* pDatakind)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfUniqueName_t	datakindName;
	OMF2::omfBool			bFound;
	char					szAUID[64];

	if ( memcmp((char *)&Datadef, (char *)&DDEF_Video, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Picture");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&Datadef, (char *)&DDEF_Audio, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Sound");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&Datadef, (char *)&DDEF_Timecode, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Timecode");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&Datadef, (char *)&DDEF_Edgecode, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Edgecode");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
	}
	else
	{
		AUIDtoString(&Datadef, szAUID);
		UTLstdprintf("%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		UTLerrprintf("%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		rc = AAFRESULT_INVALID_DATADEF;
	}

	if (!bFound)
		rc = AAFRESULT_INVALID_DATADEF;
	return rc;
}
// ============================================================================
// TraverseSequence
//
//			This function reads all components the of an AAF Sequence,
//			creates/sets the equivalent OMF objects 
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::TraverseSequence(IAAFSequence* pSequence,
								  OMF2::omfObject_t* pOMFSequence )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfCpntObj_t		newComponent;

	IAAFComponent*			pComponent = NULL;
	IEnumAAFComponents*		pCompIter = NULL;
	aafInt32				numComponents;
	aafInt32				cpntCount;
	

	rc = pSequence->GetNumComponents(&numComponents);
	if (FAILED(rc))
		return rc;
	
	rc = pSequence->EnumComponents(&pCompIter);
	for (cpntCount = 0; cpntCount < numComponents; cpntCount++)
	{
		rc = pCompIter->NextOne(&pComponent);
		if (SUCCEEDED(rc))
		{
			rc = ProcessComponent(pComponent, &newComponent);
			if (SUCCEEDED(rc))
			{
				rc = OMF2::omfiSequenceAppendCpnt(OMFFileHdl, *pOMFSequence, newComponent);
			}
		}
		pComponent->Release();
		pComponent = NULL;
	}
	pCompIter->Release();
	pCompIter = NULL;
	return rc;
}
// ============================================================================
// ConvertSelector
//
//			This function converts an AAF Selector object and all the objects it
// contains or references.
//			
// Returns: AAFRESULT_SUCCESS if succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ConvertSelector(IAAFSelector* pSelector,
								 OMF2::omfObject_t* pOMFSelector )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfSegObj_t		OMFSelected;

	IAAFComponent*			pComponent = NULL;
	IAAFSegment*			pSegment = NULL;
	aafLength_t				length;
	aafInt32				numAlternates;

	pSelector->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	pSelector->GetSelectedSegment(&pSegment);
	pSelector->GetNumAlternateSegments(&numAlternates);
	pComponent->GetLength(&length);

	if (gpGlobals->bVerboseMode)
		UTLstdprintf("%sProcessing Selector object of length = %ld\n", gpGlobals->indentLeader, length);

	pComponent->Release();
	pComponent = NULL;
	pSegment->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	rc = ProcessComponent(pComponent, &OMFSelected );
	if (SUCCEEDED(rc))
	{
		rc = OMF2::omfiSelectorSetSelected(OMFFileHdl, *pOMFSelector, OMFSelected);
		if (numAlternates > 0)
		{
			OMF2::omfSegObj_t	OMFAltSelected;

			IAAFComponent*		pAltComponent = NULL;
			IEnumAAFSegments*	pEnumAlternates = NULL;
			IAAFSegment*		pAltSegment = NULL;
	
			rc = pSelector->EnumAlternateSegments(&pEnumAlternates);
			IncIndentLevel();
			for (int i = 0; i< numAlternates;i++)
			{
				pEnumAlternates->NextOne(&pAltSegment);
				pAltSegment->QueryInterface(IID_IAAFComponent, (void **)&pAltComponent);
				rc = ProcessComponent(pAltComponent, &OMFAltSelected);
				if (SUCCEEDED(rc))
				{
					rc = OMF2::omfiSelectorAddAlt(OMFFileHdl, *pOMFSelector, OMFAltSelected);
				}
				pAltComponent->Release();
				pAltSegment->Release();
			}

			pEnumAlternates->Release();
		}
	}
	if (pSegment)
		pSegment->Release();
	return rc;
}

// ============================================================================
// ConvertLocator
//
//			This function converts an AAF Filed descriptor to OMF
//			
// Returns: AAFRESULT_SUCCESS if succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ConvertLocator(IAAFEssenceDescriptor* pEssenceDesc,
								OMF2::omfMobObj_t*	pOMFSourceMob )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfClassID_t		locType;
	char*					pszLocatorPath = NULL;

	IAAFLocator*			pLocator = NULL;
	IEnumAAFLocators*		pLocatorIter = NULL;		
	aafInt32				numLocators = 0;
    aafWChar*				pwLocatorPath = NULL;
	aafInt32				pathSize = 0;

	rc = pEssenceDesc->GetNumLocators(&numLocators);
	if (SUCCEEDED(rc) && numLocators > 0)
	{
		rc = pEssenceDesc->EnumAAFAllLocators(&pLocatorIter);
		while (SUCCEEDED(rc) && SUCCEEDED(pLocatorIter->NextOne(&pLocator)))
		{
			pLocator->GetPathBufLen(&pathSize);
			UTLMemoryAlloc(pathSize, (void **)&pwLocatorPath);
			pLocator->GetPath(pwLocatorPath, pathSize);
			UTLStrWToStrA(pwLocatorPath, &pszLocatorPath);
			rc = OMF2::omfmMobAddNetworkLocator(OMFFileHdl, *pOMFSourceMob, OMF2::kOmfiMedia, pszLocatorPath);
			if (rc != OMF2::OM_ERR_NONE)
				break;
			if (pwLocatorPath)
			{
				UTLMemoryFree(pwLocatorPath);
				pwLocatorPath = NULL;
			}
			if (pszLocatorPath)
			{
				UTLMemoryFree(pszLocatorPath);
				pszLocatorPath = NULL;
			}
		}
	}
	if (pLocator)
		pLocator->Release();
	if (pLocatorIter)
		pLocatorIter->Release();
	if (pwLocatorPath)
		UTLMemoryFree(pwLocatorPath);
	if (pszLocatorPath)
		UTLMemoryFree(pszLocatorPath);
	return rc;
	
}
// ============================================================================
// ConvertEssenceDataObject
//
//			This function is called by the AAFFILERead module for each 
//			media data object found in the header
//			
// Returns: AAFRESULT_SUCCESS if Header object is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ConvertEssenceDataObject( IAAFEssenceData* pEssenceData)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfObject_t		OMFSourceMob = NULL;
	OMF2::omfObject_t		OMFHeader = NULL;
	OMF2::omfObject_t		mediaData = NULL;
	OMF2::omfUID_t			mediaID;
	OMF2::omfProperty_t		idProperty;
	OMF2::omfDDefObj_t		datakind;
	char					id[5];

	IAAFTIFFData*			pTIFFData = NULL;
	IAAFAIFCData*			pAIFCData = NULL;
	IAAFWAVEData*			pWAVEData = NULL;
	IAAFJPEGData*			pJPEGData = NULL;
	IAAFMob*				pMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFEssenceDescriptor*	pEssenceDesc = NULL;
	IAAFObject*				pAAFObject = NULL;
	aafUID_t				mobID;
	aafBool					bConvertMedia = AAFFalse;

	// get the file mob id
	rc = pEssenceData->GetFileMobID(&mobID);
	if (FAILED(rc))
		return rc;
	// make sure it is a Source mob
	rc = pEssenceData->GetFileMob(&pSourceMob);
	if (FAILED(rc))
		return rc;

	rc = pSourceMob->GetEssenceDescriptor(&pEssenceDesc);
	if (FAILED(rc))
	{
		pSourceMob->Release();
		return rc;
	}

	ConvertAUIDtoUID(&mobID, &mediaID);
	rc = OMF2::omfsFindSourceMobByID(OMFFileHdl, mediaID, &OMFSourceMob);
	if (rc != OMF2::OM_ERR_NONE)
	{
		rc = AAFRESULT_INVALID_OBJ;
		pSourceMob->Release();
		return rc;
	}
	rc = OMF2::omfsGetHeadObject( OMFFileHdl, &OMFHeader );

	rc = pEssenceData->QueryInterface(IID_IAAFTIFFData, (void **)&pTIFFData);
	if (SUCCEEDED(rc))
	{
		//Convert TIFF Essence data
		idProperty = OMF2::OMTIFFData;
		OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&rc);
		strcpy(id, "TIFF");
		rc = OMF2::omfsObjectNew(OMFFileHdl, id, &mediaData);
		if (rc != OMF2::OM_ERR_NONE)
			goto Cleanup;
		rc = OMF2::omfsWriteUID(OMFFileHdl, mediaData, OMF2::OMMDATMobID, mediaID);
		if (rc)
		{
			char* pErrString = OMF2::omfsGetErrorString((OMF2::omfErr_t)rc);
			goto Cleanup;
		}
		rc = OMF2::omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, mediaData);
		if (rc != OMF2::OM_ERR_NONE)
			goto Cleanup;
		else
			goto CopyMedia;
	}

	rc = pEssenceData->QueryInterface(IID_IAAFAIFCData, (void **)&pAIFCData);
	if (SUCCEEDED(rc))
	{
		//Convert AIFC Essence data
		idProperty = OMF2::OMAIFCData;
		OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (OMF2::omfErr_t *)&rc);
		strcpy(id, "AIFC");
		rc = OMF2::omfsObjectNew(OMFFileHdl, id, &mediaData);
		if (rc != OMF2::OM_ERR_NONE)
			goto Cleanup;
		rc = OMF2::omfsWriteUID(OMFFileHdl, mediaData, OMF2::OMMDATMobID, mediaID);
		if (rc)
		{
			char* pErrString = OMF2::omfsGetErrorString((OMF2::omfErr_t)rc);
			goto Cleanup;
		}
		rc = OMF2::omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, mediaData);
		if (rc != OMF2::OM_ERR_NONE)
			goto Cleanup;
		else
			goto CopyMedia;
	}

	rc = pEssenceData->QueryInterface(IID_IAAFWAVEData, (void **)&pWAVEData);
	if (SUCCEEDED(rc))
	{
		//Convert WAVE Essence data
		idProperty = OMF2::OMWAVEData;
		OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (OMF2::omfErr_t *)&rc);
		strcpy(id, "WAVE");
		rc = OMF2::omfsObjectNew(OMFFileHdl, id, &mediaData);
		if (rc != OMF2::OM_ERR_NONE)
			goto Cleanup;
		rc = OMF2::omfsWriteUID(OMFFileHdl, mediaData, OMF2::OMMDATMobID, mediaID);
		if (rc)
		{
			char* pErrString = OMF2::omfsGetErrorString((OMF2::omfErr_t)rc);
			goto Cleanup;
		}
		rc = OMF2::omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, mediaData);
		if (rc != OMF2::OM_ERR_NONE)
			goto Cleanup;
		else
			goto CopyMedia;
	}

	rc = pEssenceData->QueryInterface(IID_IAAFJPEGData, (void **)&pJPEGData);
	if (SUCCEEDED(rc))
	{
		//Convert JPEG Essence data
		idProperty = OMF2::OMIDATImageData;
		OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&rc);
		strcpy(id, "JPEG");
		rc = OMF2::omfsObjectNew(OMFFileHdl, id, &mediaData);
		if (rc != OMF2::OM_ERR_NONE)
			goto Cleanup;
		rc = OMF2::omfsWriteUID(OMFFileHdl, mediaData, OMF2::OMMDATMobID, mediaID);
		if (rc)
		{
			char* pErrString = OMF2::omfsGetErrorString((OMF2::omfErr_t)rc);
			goto Cleanup;
		}
		rc = OMF2::omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, mediaData);
		if (rc != OMF2::OM_ERR_NONE)
			goto Cleanup;
		else
			goto CopyMedia;
	}
	// Media type not supported or invalid
	rc = pEssenceData->QueryInterface(IID_IAAFObject, (void **)&pAAFObject);
	if (SUCCEEDED(rc))
	{
		aafUID_t	ObjClass;
		char		szTempUID[64];

		pAAFObject->GetObjectClass(&ObjClass);
		AUIDtoString(&ObjClass ,szTempUID);
		if (gpGlobals->bVerboseMode)
			UTLstdprintf("%sInvalid essence data type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
		UTLerrprintf("%sInvalid essence data type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
	}
	goto Cleanup;

CopyMedia:
	if (mediaData)
	{
		void*			pBuffer = NULL;
		aafPosition_t	AAFOffset;

		aafLength_t		numBytes;
		aafUInt32		nBlockSize;
		aafUInt32		numBytesRead;
		aafBool			bMore = AAFFalse;

		rc = pEssenceData->GetSize(&numBytes);
		if (numBytes > 0)
		{
			if (numBytes > (2 * 1048576))
			{
				nBlockSize = 2 * 1048576;	// only allocate 2 Meg
				bMore = AAFTrue;			// you going to need more than one read/write
			}
			else
			{
				nBlockSize = numBytes;
			}
			rc = UTLMemoryAlloc(nBlockSize, &pBuffer);
			AAFOffset = 0;
			do 
			{
				rc = pEssenceData->SetPosition( AAFOffset );
				rc = pEssenceData->Read( nBlockSize, (unsigned char *)pBuffer, &numBytesRead);

				// write the media
				rc = OMF2::omfsWriteDataValue(OMFFileHdl, 
									 		  mediaData,
											  idProperty,
											  datakind, 
											  (OMF2::omfDataValue_t)pBuffer,
											  (OMF2::omfPosition_t)AAFOffset,
											  numBytesRead);
				// calculate next offset
				AAFOffset += numBytesRead;
			}while (numBytes > AAFOffset );
			// Free the allocated buffer 
			UTLMemoryFree(pBuffer);
		}
	}
Cleanup:
	if (pSourceMob)
		pSourceMob->Release();

	if (pTIFFData)
		pTIFFData->Release();
		
	if (pAIFCData)
		pAIFCData->Release();

	if (pWAVEData)
		pWAVEData->Release();

	if (pJPEGData)
		pJPEGData->Release();

	return rc;

}