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
//#include <cstdlib>

#include "omPublic.h"
#include "omMedia.h"

// OMF Includes


#include "AafOmf.h"
#include "AutoRelease.h"
#include "AAFDomainUtils.h"
#include "OMFDomainUtils.h"
#if AVID_SPECIAL
#include "ConvertAvid.h"
#include "AAFDomainAvidUtils.h"
#include "OMFDomainAvidUtils.h"
#include "AvidEffectTranslate.h"
#else
#include "AAFDomainExtensions.h"
#include "OMFDomainExtensionUtils.h"
#include "EffectTranslate.h"
#endif
#include "Aaf2Omf.h"
#include "AAFCodecDefs.h"
#include "AAFClassDefUIDs.h"
#include "AAFException.h"
#include "OMFException.h"
//#include "omcAvJPED.h"

extern AafOmfGlobals* gpGlobals;

static aafBool	EqualAUID(const aafUID_t *uid1, const aafUID_t *uid2)
{
	return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? kAAFTrue : kAAFFalse);
}
// ============================================================================
// Constructor
// ============================================================================

Aaf2Omf::Aaf2Omf( AAFDomainUtils *aafDomainUtils, OMFDomainUtils *omfDomainUtils, EffectTranslate *effectTranslate ) : 
	pFile(NULL), pHeader(NULL), pDictionary(NULL), pAAF(aafDomainUtils), pOMF(omfDomainUtils), pEffectTranslate(effectTranslate)
{
}

// ============================================================================
// Destructor
// ============================================================================
Aaf2Omf::~Aaf2Omf()
{
	if (pHeader)
		pHeader->Release();
	if (pDictionary)
		pDictionary->Release();
	if (pFile)
		pFile->Release();

}
// ============================================================================
// ConvertFile
//			This function provides the control and logic to convert an AAF file
//			by calling all the functions inside this object
//
// ============================================================================
void Aaf2Omf::ConvertFile ()
{
	OpenInputFile();
	OpenOutputFile();
	pAAF->SetDictionary(pDictionary);
	AAFFileRead();

	if (gpGlobals->bOMFFileOpen)
	{
		CloseOutputFile();
		gpGlobals->bOMFFileOpen = kAAFFalse;
	}

	if (gpGlobals->bAAFFileOpen)
	{
		CloseInputFile();
		gpGlobals->bAAFFileOpen = kAAFFalse;
	}
}
// ============================================================================
// ConvertMobIDtoUID
//			This function reduces an MobID into an OMF UID.
//
// ============================================================================
void Aaf2Omf::ConvertMobIDtoUID(aafMobID_constptr pMobID, 
							   omfUID_t* pOMFMobID)
{
	
	struct SMPTELabel		// Change to match GUID to ensure correct byte swapping
	{
		aafUInt32	MobIDMajor;
		aafUInt16	MobIDMinorLow;
		aafUInt16	MobIDMinorHigh;
		aafUInt8	oid;
		aafUInt8	size;
		aafUInt8	ulcode;
		aafUInt8	SMPTE;
		aafUInt8	Registry;
		aafUInt8	unused;
		aafUInt8	MobIDPrefixLow;
		aafUInt8	MobIDPrefixHigh;
	};

	struct AMobID
	{
		aafUInt8 SMPTELabel[12];		// 12-bytes of label prefix
		aafUInt8 length;
		aafUInt8 instanceHigh;
		aafUInt8 instanceMid;
		aafUInt8 instanceLow;
		struct SMPTELabel	material;
	};
	union MobIDOverlay
	{
		aafMobID_t			mobID;
		struct AMobID	AMobID;
	};

	union MobIDOverlay aLabel;
	memcpy((void *)&aLabel.AMobID, pMobID, sizeof(aLabel.AMobID));

	pOMFMobID->prefix = (aLabel.AMobID.material.MobIDPrefixHigh << 8L) | 
		                 aLabel.AMobID.material.MobIDPrefixLow;
	pOMFMobID->major = aLabel.AMobID.material.MobIDMajor;
	pOMFMobID->minor = (aLabel.AMobID.material.MobIDMinorHigh << 16L) | 
		                aLabel.AMobID.material.MobIDMinorLow;
	
	gpGlobals->pLogger->Log( kLogInfo, "Converterting MobID to UID...\n" );
	gpGlobals->pLogger->Log( kLogInfo, "prefix: %ld\n", (long) pOMFMobID->prefix );
	gpGlobals->pLogger->Log( kLogInfo, "major: %ld\n", (long) pOMFMobID->major );
	gpGlobals->pLogger->Log( kLogInfo, "minor: %ld\n", (long) pOMFMobID->minor );
}
// ============================================================================
// OpenInputFile
//			This function determines an AAF file for read.
//			If the file does not exists or it is not an AAF file an
//			error code will be returned.
//
// ============================================================================
void Aaf2Omf::OpenInputFile ()
{
	AAFCheck					rc;

	// convert file name to Wide char
	aafWChar*	pwFileName = new wchar_t[strlen(gpGlobals->sInFileName)+1];
	mbstowcs(pwFileName, gpGlobals->sInFileName, strlen(gpGlobals->sInFileName)+1);

	try
	{
		rc =  AAFFileOpenExistingRead(pwFileName, 0, &pFile);
		rc =  pFile->GetHeader(&pHeader);
		rc =  pHeader->GetDictionary(&pDictionary);
		pAAF->RegisterAAFProperties(pDictionary);
	}
	catch( ExceptionBase )
	{
		throw;
	}
	gpGlobals->pLogger->Log( kLogInfo, "AAF file \"%s\" opened succesfully.\n", gpGlobals->sInFileName);
	gpGlobals->bAAFFileOpen = kAAFTrue;
	delete pwFileName;
}

// ============================================================================
// OpenOutputFile
//			This function creates the output file.
//
// ============================================================================
void Aaf2Omf::OpenOutputFile ()
{
	AAFCheck							rc;
	OMFCheck							OMFError;
	aafBool								bSessionStarted = kAAFFalse;
	
	omfProductIdentification_t	OMFProductInfo;
	char*								pszCompanyName = NULL;
	char*								pszProductName = NULL;
 	char*								pszProductVersionString = NULL;
 	char*								pszPlatform;
	char								src[] = "<Not Specified>";
	
	IAAFIdentification*					pIdent = NULL;
	aafWChar*							pwCompanyName = NULL;
	aafWChar*							pwProductName = NULL;
 	aafWChar*							pwProductVersionString = NULL;
 	aafWChar*							pwPlatform;
	aafUInt32							textSize = 0;
	aafProductVersion_t					productVersion;
	

	try
	{
		if (strlen(gpGlobals->sOutFileName) == 0)
		{
			char*	pExt;
			strcpy(gpGlobals->sOutFileName, gpGlobals->sInFileName);
			pExt= strrchr(gpGlobals->sOutFileName, '.');
			if( pExt != NULL )
				strcpy(pExt,".omf");
			else
				strcat( gpGlobals->sOutFileName, ".omf" );
		}
		
		if (gpGlobals->bDeleteOutput)
		{
			HRESULT	testRC;
			testRC = deleteFile(gpGlobals->sOutFileName);
			gpGlobals->pLogger->Log( kLogInfo, "Output file: %s will be %s\n", gpGlobals->sOutFileName, 
				testRC == AAFRESULT_SUCCESS ? "overwritten" : "created");
		}
		// Retrieve AAF file's last identification
		rc = pHeader->GetLastIdentification(&pIdent);
		AutoRelease<IAAFIdentification> ARIdent(  pIdent );
		pIdent->GetCompanyNameBufLen(&textSize);

		if (textSize > 0)
		{
			pwCompanyName = new wchar_t[textSize];
			pIdent->GetCompanyName(pwCompanyName, textSize);
			pszCompanyName = new char[textSize/sizeof(wchar_t)];
			wcstombs(pszCompanyName, pwCompanyName, textSize/sizeof(wchar_t));
		}
		else
		{
			pszCompanyName = new char[ sizeof( src ) ];
			strcpy(pszCompanyName, src);
		}
		
		pIdent->GetProductNameBufLen(&textSize);
		if (textSize > 0)
		{
			pwProductName = new wchar_t[textSize];
			pIdent->GetProductName(pwProductName, textSize);
			pszProductName = new char[textSize/sizeof(wchar_t)];
			wcstombs(pszProductName, pwProductName, textSize/sizeof(wchar_t));
		}
		else
		{
			pszProductName =  new char[ sizeof( src ) ];
			strcpy(pszProductName, src);
		}
		
		pIdent->GetProductVersionStringBufLen(&textSize);
		if (textSize > 0)
		{
			pwProductVersionString = new wchar_t[textSize];
			pIdent->GetProductVersionString(pwProductVersionString, textSize);
			pszProductVersionString = new char[textSize/sizeof(wchar_t)];
			wcstombs(pszProductVersionString, pwProductVersionString, textSize/sizeof(wchar_t));
		}
		else
		{
			pszProductVersionString = new char[sizeof( src ) ];
			strcpy(pszProductVersionString, src);
		}
		
		pIdent->GetPlatformBufLen(&textSize);
		if (textSize > 0)
		{
			pwPlatform = new wchar_t[textSize/sizeof(wchar_t)];
			pIdent->GetPlatform(pwPlatform, textSize);
			pszPlatform = new char[textSize/sizeof(wchar_t)];
			wcstombs(pszPlatform, pwPlatform, textSize/sizeof(wchar_t));
		}
		else
		{
			pszPlatform = new char[sizeof(src)];
			strcpy(pszPlatform, src);
		}
		
		
		OMFProductInfo.companyName = pszCompanyName;
		OMFProductInfo.productName = pszProductName;
		OMFProductInfo.productVersionString = pszProductVersionString;
		OMFProductInfo.platform = pszPlatform;
		
		/* rc = !!!*/ pIdent->GetProductVersion(&productVersion);
		OMFProductInfo.productVersion.major = productVersion.major;
		OMFProductInfo.productVersion.minor = productVersion.minor;
		OMFProductInfo.productVersion.tertiary = productVersion.tertiary;
		OMFProductInfo.productVersion.patchLevel = productVersion.patchLevel;
		OMFProductInfo.productID = 42; // Comes from OMF !!!
		OMFProductInfo.productVersion.type = (omfProductReleaseType_t)productVersion.type;
		
		OMFError = omfsBeginSession(&OMFProductInfo, &OMFSession);
		
		RegisterCodecProperties(gpGlobals, OMFSession);
		pOMF->RegisterOMFProperties(gpGlobals, OMFSession);
		
		bSessionStarted = kAAFTrue;
		OMFError = omfmInit(OMFSession);
		
		
		OMFError = omfsCreateFile((fileHandleType)gpGlobals->sOutFileName, OMFSession, kOmfRev2x, &OMFFileHdl);
		
		gpGlobals->bOMFFileOpen = kAAFTrue;
		// Clean up and exit 
	}
	catch( ExceptionBase  )
	{
		if( bSessionStarted == kAAFTrue )
		{
			omfsEndSession(OMFSession);
		}

		gpGlobals->pLogger->Log( kLogError, "File: %s could NOT be created\n", 
			gpGlobals->sOutFileName );
		throw;
	}
	
	gpGlobals->pLogger->Log( kLogInfo, "OMF file: %s created succesfully\n", 
			gpGlobals->sOutFileName);
	if(pszCompanyName)
		delete pszCompanyName;
	if(pszProductName)
		delete pszProductName;
	if(pszProductVersionString)
		delete pszProductVersionString;
	if(pszPlatform)
		delete pszPlatform;
	if(pwCompanyName)
		delete pwCompanyName;
	if(pwProductName)
		delete pwProductName;
	if(pwProductVersionString)
		delete pwProductVersionString;
	if(pwPlatform)
		delete pwPlatform;
}

// ============================================================================
// CloseOutputFile
//			This function closes the output file.
//
// ============================================================================
void Aaf2Omf::CloseOutputFile()
{
	OMFCheck	OMFError;

	OMFError = omfsCloseFile(OMFFileHdl);
	OMFError = omfsEndSession(OMFSession);
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
void Aaf2Omf::AAFFileRead()
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	omfMobObj_t		OMFMob = NULL;

	aafNumSlots_t			nAAFNumMobs;
	aafUInt32				numEssenceData = 0;
	IEnumAAFMobs*			pMobIter = NULL;
	IAAFMob*				pMob = NULL;
	IAAFCompositionMob*		pCompMob = NULL;
	IAAFMasterMob*			pMasterMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IEnumAAFEssenceData*	pEssenceDataIter = NULL;
	IAAFEssenceData*		pEssenceData = NULL;
	aafSearchCrit_t			criteria;

	rc = pHeader->CountMobs(kAAFAllMob, &nAAFNumMobs);
	if (gpGlobals->bVerboseMode)
	{
		printf(" Found: %ld Mobs in the input file\n", nAAFNumMobs);
	}
	criteria.searchTag = kAAFByMobKind;
	criteria.tags.mobKind = kAAFAllMob;
	rc = pHeader->GetMobs(&criteria, &pMobIter);
	while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
	{
		aafUInt32	nameLength = 0;
		aafMobID_t	MobID;
		char		szMobID[ sizeof( aafMobID_t ) * sizeof( wchar_t ) ];

		gpGlobals->nNumAAFMobs++;
		// Get Mob Info
		pMob->GetNameBufLen(&nameLength);
		wchar_t *pwMobName  = new wchar_t[nameLength/sizeof(wchar_t)];
		rc = pMob->GetName(pwMobName, nameLength);
		rc = pMob->GetMobID(&MobID);
		char *pszMobName = new char[nameLength/sizeof(wchar_t)];
		wcstombs(pszMobName, pwMobName, nameLength/sizeof(wchar_t));
		delete pwMobName;
		// Is this a composition Mob?
		if (SUCCEEDED(pMob->QueryInterface(IID_IAAFCompositionMob, (void **)&pCompMob)))
		{
			// Composition Mob
			ConvertCompositionMob(pCompMob, &OMFMob, pszMobName, &MobID);
			pCompMob->Release();
			pCompMob = NULL;
		}
		else
		{
			// Is it a Master Mob ?
			if (SUCCEEDED(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob)))
			{
				ConvertMasterMob(pMasterMob, &OMFMob, pszMobName, &MobID);
				pMasterMob->Release();
				pMasterMob = NULL;
			}
			else
			{
				// Is it a Source Mob
				if (SUCCEEDED(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob)))
				{
					ConvertSourceMob(pSourceMob, &OMFMob, pszMobName, &MobID);
					pSourceMob->Release();
					pSourceMob = NULL;
				}
				else
				{
					MobIDtoString(MobID, szMobID);
					printf("Unrecognized Mob kind encountered ID: %s\n", szMobID);
//					fprintf(stderr,"Unrecognized Mob kind encountered ID: %s\n", szMobID);
				}
			}
		}
		delete pszMobName;
		if (OMFMob)
		{
			TraverseMob(pMob, &OMFMob);
			// NOw add user comments 
			aafUInt32	numComments = 0;
			rc = pMob->CountComments(&numComments);
			if (numComments > 0)
			{
				gpGlobals->pLogger->Log( kLogInfo, "Processing %ld comments...\n",  (long) numComments );
				IEnumAAFTaggedValues*	pCommentIterator = NULL;
				HRESULT testRC = pMob->GetComments(&pCommentIterator);

				if( SUCCEEDED(testRC) )
				{
					IAAFTaggedValue*		pMobComment = NULL;
					while( SUCCEEDED(pCommentIterator->NextOne(&pMobComment)) )
					{
						aafUInt32	nameSize = 0, textSize = 0, bytesRead;
				
						pMobComment->GetNameBufLen(&nameSize);
						if( nameSize > 0 )
						{
							aafWChar *pwcName = new aafWChar [nameSize/sizeof(aafWChar)];
							pMobComment->GetName(pwcName, nameSize);
							char *pszCommName =  new char[nameSize/sizeof(aafWChar)];
							wcstombs(pszCommName, pwcName, nameSize/sizeof(aafWChar));
							pMobComment->GetValueBufLen((aafUInt32 *)&textSize);
							if( textSize > 0 )
							{
								aafWChar* pwcComment = new aafWChar[textSize/sizeof(aafWChar)];
								pMobComment->GetValue((aafUInt32)textSize, (aafDataBuffer_t)pwcComment, &bytesRead);
								char *pszComment =  new char[textSize/sizeof(aafWChar)];
								wcstombs(pszComment, pwcComment, textSize/sizeof(aafWChar));
								OMFError = omfiMobAppendComment(OMFFileHdl, OMFMob, pszCommName, pszComment);
								gpGlobals->pLogger->Log( kLogInfo, "Comment \"%s\" of length %ld was converted.\n", pszCommName, textSize );
								gpGlobals->pLogger->Log( kLogInfo, "Comment value = \"%s\".\n", pszComment);
								delete pwcComment;
								delete pszComment;
							}
							else
							{
								gpGlobals->pLogger->Log( kLogError, "Zero length comment value encountered. Comment not converted.\n" );
							}
							delete pwcName;
							delete pszCommName;
						}
						else
						{
							gpGlobals->pLogger->Log( kLogError, "Zero length comment name encountered. Comment not converted.\n" );
						}
						pMobComment->Release();
						pMobComment = NULL;
					}

					pCommentIterator->Release();
				}
			}
		}
		gpGlobals->nNumOMFMobs++;
		pMob->Release();
		pMob = NULL;
	}
	if (pMobIter)
		pMobIter->Release();

	// Now get the media data.
	rc = pHeader->CountEssenceData(&numEssenceData);
	if (numEssenceData > 0)
	{
		if (gpGlobals->bVerboseMode)
		{
			printf(" Found: %ld Essence data objects\n", numEssenceData);
		}
		rc = pHeader->EnumEssenceData(&pEssenceDataIter);
		while (SUCCEEDED(pEssenceDataIter->NextOne(&pEssenceData)))
		{
			ConvertEssenceDataObject(pEssenceData);
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
void Aaf2Omf::ConvertCompositionMob(IAAFCompositionMob* pCompMob,
									   omfMobObj_t* pOMFCompMob,
									   char* pMobName,
									   aafMobID_t* pMobID)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	
	omfUID_t			OMFMobID;
	omfDefaultFade_t	OMFFade;
	IAAFMob					*pMob = NULL;
	aafDefaultFade_t		DefaultFade;
	
	OMFError = omfiCompMobNew(OMFFileHdl, pMobName, (omfBool)kAAFFalse, pOMFCompMob);
	
	ConvertMobIDtoUID(pMobID, &OMFMobID);
	OMFError = omfiMobSetIdentity(OMFFileHdl, *pOMFCompMob, OMFMobID);
	
	if ((pCompMob->GetDefaultFade(&DefaultFade) == AAFRESULT_SUCCESS) && DefaultFade.valid)
	{
		OMFFade.fadeLength = DefaultFade.fadeLength;
		switch (DefaultFade.fadeType)
		{
		case kAAFFadeNone:
			OMFFade.fadeType = kFadeNone;
			break;
		case kAAFFadeLinearAmp:
			OMFFade.fadeType = kFadeLinearAmp;
			break;
		case kAAFFadeLinearPower:
			OMFFade.fadeType = kFadeLinearPower;
			break;
		}
		OMFFade.fadeEditUnit.numerator = DefaultFade.fadeEditUnit.numerator;
		OMFFade.fadeEditUnit.denominator = DefaultFade.fadeEditUnit.denominator;
		
		OMFError = omfiMobSetDefaultFade(OMFFileHdl,
			*pOMFCompMob,
			OMFFade.fadeLength, 
			OMFFade.fadeType ,
			OMFFade.fadeEditUnit);
		
	}
	
	rc = pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
	FinishUpMob(pMob, *pOMFCompMob);
	
	//	if (OMFError != OM_ERR_NONE)
	//		rc = AAFRESULT_INTERNAL_ERROR;
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
void Aaf2Omf::ConvertMasterMob(IAAFMasterMob* pMasterMob,
								  omfMobObj_t* pOMFMasterMob,
								  char* pMobName,
								  aafMobID_t* pMobID)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	IAAFMob					*pMob = NULL;
	
	omfUID_t		OMFMobID;
	
	OMFError = omfmMasterMobNew(OMFFileHdl, pMobName, (omfBool)kAAFTrue, pOMFMasterMob);
	
	ConvertMobIDtoUID(pMobID, &OMFMobID);
	OMFError = omfiMobSetIdentity(OMFFileHdl, *pOMFMasterMob, OMFMobID);
	
	if (gpGlobals->bVerboseMode)
		printf("Converted AAF Master MOB to OMF\n");
	
	rc = pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
	FinishUpMob(pMob, *pOMFMasterMob);
	
	//	if (OMFError != OM_ERR_NONE)
	//		rc = AAFRESULT_INTERNAL_ERROR;
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
void Aaf2Omf::ConvertSourceMob(IAAFSourceMob* pSourceMob,
								  omfMobObj_t* pOMFSourceMob,
								  char* pMobName,
								  aafMobID_t* pMobID)
{
	AAFCheck				rc;
	OMFCheck				OMFError;

	omfUID_t			OMFMobID;


	IAAFEssenceDescriptor*	pEssenceDesc = NULL;
	IAAFTapeDescriptor*		pTapeDesc = NULL;
	IAAFFileDescriptor*		pFileDesc = NULL;
	IAAFTIFFDescriptor*		pTiffDesc = NULL;
	IAAFWAVEDescriptor*		pWAVEDesc = NULL;
	IAAFAIFCDescriptor*		pAifcDesc = NULL;
	IAAFDigitalImageDescriptor* pDIDDDesc = NULL;
	IAAFCDCIDescriptor*		pCDCIDesc = NULL;
	IAAFObject*				pAAFObject = NULL;
	aafInt32				*lineMap = NULL;
	IAAFMob					*pMob = NULL;

	if (gpGlobals->bVerboseMode)
		printf("Converting AAF Source MOB to OMF\n");

	rc = pSourceMob->GetEssenceDescriptor(&pEssenceDesc);

	IncIndentLevel();

	ConvertMobIDtoUID(pMobID, &OMFMobID);
	if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFTapeDescriptor,(void **)&pTapeDesc)))
	{
		// This is Tape Descriptor 
		// Therefore the Source Mob is a Tape Mob
		aafWChar*			pwManufacturer = NULL;
		char*				pszManufacturer = NULL;
		aafWChar*			pwModel = NULL;
		char*				pszModel = NULL;
		aafUInt32			textSize = 0;
		aafTapeCaseType_t	formFactor = kAAFTapeCaseNull;
		aafTapeFormatType_t	tapeFormat = kAAFTapeFormatNull;
		aafUInt32			tapeLength = 0;
		aafVideoSignalType_t videoSignal = kAAFVideoSignalNull;
		
		OMFError = omfmTapeMobNew(OMFFileHdl, pMobName, pOMFSourceMob);
		
		OMFError = omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
		pTapeDesc->GetTapeFormFactor(&formFactor);
		pTapeDesc->GetSignalType(&videoSignal);
		pTapeDesc->GetTapeFormat(&tapeFormat);
		pTapeDesc->GetTapeLength(&tapeLength);
		pTapeDesc->GetTapeManufacturerBufLen(&textSize);
		if (textSize > 0)
		{
			pwManufacturer = new aafWChar[textSize/sizeof(aafWChar)];
			pTapeDesc->GetTapeManufacturer(pwManufacturer, textSize);
			pszManufacturer = new char[textSize/sizeof(aafWChar)];
			wcstombs(pszManufacturer, pwManufacturer, textSize/sizeof(aafWChar));
		}
		else
			pszManufacturer = NULL;
		pTapeDesc->GetTapeModelBufLen(&textSize);
		if (textSize > 0)
		{
			pwModel = new aafWChar[textSize/sizeof(wchar_t)];
			pTapeDesc->GetTapeModel(pwModel, textSize);
			pszModel = new char[textSize/sizeof(aafWChar)];
			wcstombs(pszModel, pwModel, textSize/sizeof(aafWChar));
		}
		else
			pszModel = NULL;
		
		OMFError = omfmTapeMobSetDescriptor(OMFFileHdl,
												*pOMFSourceMob,
												(omfTapeCaseType_t *)&formFactor,
												(omfVideoSignalType_t *)&videoSignal,
												(omfTapeFormatType_t *)&tapeFormat,
												(omfLength_t *)&tapeLength,
												pszManufacturer,
												pszModel);
		if (gpGlobals->bVerboseMode)
			printf("%sAdded a Tape Essence Descriptor to a Source MOB\n", gpGlobals->indentLeader);
		if (pwManufacturer)
			delete [] pwManufacturer;
		if (pszManufacturer)
			delete [] pszManufacturer;
		if (pwModel)
			delete [] pwModel;
		if (pszModel)
			delete [] pszModel;
		
		rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		FinishUpMob(pMob, *pOMFSourceMob);
		
		goto cleanup;
	}

	if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFFileDescriptor,(void **)&pFileDesc)))
	{
		// This is File Descriptor
		omfRational_t	rate;
		aafRational_t		sampleRate;
		aafLength_t		length;	

		pFileDesc->GetSampleRate(&sampleRate);
		pFileDesc->GetLength(&length);
		rate.numerator = sampleRate.numerator;
		rate.denominator = sampleRate.denominator;
		if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFTIFFDescriptor, (void **)&pTiffDesc)))
		{
			// It is a TIFF file descriptor
			omfDDefObj_t	datakind;
			omfObject_t	mediaDescriptor;
			aafInt32			leadingLines, trailingLines;
			aafJPEGTableID_t	JPEGTableID;
			aafDataValue_t		pSummary;
			aafUInt32			summarySize = 0;
			aafBool				IsUniform;
			aafBool				IsContiguous;

			// retrieve TIFF descriptor properties
			rc = pTiffDesc->GetSummaryBufferSize(&summarySize);
			pSummary = new aafUInt8[summarySize];
			rc = pTiffDesc->GetSummary(summarySize, pSummary);
			rc = pTiffDesc->GetTrailingLines(&trailingLines);
			rc = pTiffDesc->GetLeadingLines(&leadingLines);
			rc = pTiffDesc->GetIsUniform(&IsUniform);
			rc = pTiffDesc->GetIsContiguous(&IsContiguous);
			rc = pTiffDesc->GetJPEGTableID(&JPEGTableID);

			// Create a new OMF TIFF File Descriptor
			OMFError = omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_TIFF_VIDEO, pOMFSourceMob);
			OMFError = omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMFError = omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			OMFError = omfsWriteLength(OMFFileHdl, mediaDescriptor, OMMDFLLength, (omfLength_t)length); 
//!!!			if (OMFError)
//			{
//				char* pErrString = omfsGetErrorString(OMFError);
//				fprintf(stderr,"%sAn error occurred while adding TIFF Media descritptor, ERROR = %s\n",gpGlobals->indentLeader, pErrString);
//				goto cleanup;
//			}
			if (gpGlobals->bVerboseMode)
				printf("%sAdded a Tiff Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);

			// Set OMF TIFF File Descriptor properties
			omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (omfErr_t *)&rc);
			OMFError = omfsWriteBoolean( OMFFileHdl,
										mediaDescriptor,
										OMTIFDIsContiguous, 
										(omfBool)IsContiguous);
			OMFError = omfsWriteBoolean( OMFFileHdl,
										mediaDescriptor,
										OMTIFDIsUniform,
										(omfBool)IsUniform);
			OMFError = omfsWriteDataValue(OMFFileHdl,
										 mediaDescriptor,
										 OMTIFDSummary,
										 datakind,
										 (omfDataValue_t)pSummary,
										 (omfPosition_t)0,
										 summarySize);
			OMFError = omfsWriteJPEGTableIDType( OMFFileHdl,
								 				 mediaDescriptor,
												 OMTIFDJPEGTableID, 
												 (omfJPEGTableID_t)JPEGTableID);
			OMFError = omfsWriteInt32(OMFFileHdl,
									 mediaDescriptor,
									 OMTIFDLeadingLines, 
									 leadingLines);
			OMFError = omfsWriteInt32(OMFFileHdl,
									 mediaDescriptor,
									 OMTIFDTrailingLines, 
									 trailingLines);
//			if (OMFError)
//!!!			{
//				char* pErrString = omfsGetErrorString(OMFError);
//				fprintf(stderr,"%sAn error occurred while adding TIFF Media descritptor, ERROR = %s\n",gpGlobals->indentLeader, pErrString);
//				goto cleanup;
//			}
			delete [] pSummary;
			rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			FinishUpMob(pMob, *pOMFSourceMob);
			goto cleanup;
		}
		if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&pWAVEDesc)))
		{
			// It is a WAVE file descriptor
			omfDDefObj_t	datakind;
			omfObject_t	mediaDescriptor;
			aafDataValue_t		pSummary;
			aafUInt32			summarySize = 0;

			// retrieve WAVE descriptor properties
			rc = pWAVEDesc->GetSummaryBufferSize(&summarySize);
			pSummary = new aafUInt8[summarySize];
			rc = pWAVEDesc->GetSummary(summarySize, pSummary);

			//Create a new WAVE File Descriptor
			OMFError = omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_WAVE_AUDIO, pOMFSourceMob);
			OMFError = omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			OMFError = omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMFError = omfsWriteLength(OMFFileHdl, mediaDescriptor, OMMDFLLength, (omfLength_t)length); 
			omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (omfErr_t *)&rc);
			OMFError = omfsWriteDataValue(OMFFileHdl,
										 mediaDescriptor,
										 OMWAVDSummary,
										 datakind,
										 (omfDataValue_t)pSummary,
										 (omfPosition_t)0,
										 summarySize);
			if (gpGlobals->bVerboseMode)
				printf("%sAdded a Wave Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			FinishUpMob(pMob, *pOMFSourceMob);
			goto cleanup;
		}
		if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&pAifcDesc)))
		{
			// It is a AIFC file descriptor
			omfObject_t	mediaDescriptor;
			aafDataValue_t		pSummary;
			aafUInt32			summarySize = 0;

			// retrieve AIFC descriptor properties
			rc = pAifcDesc->GetSummaryBufferSize(&summarySize);
			pSummary = new aafUInt8[summarySize];
			rc = pAifcDesc->GetSummary(summarySize, pSummary);
			
			OMFError = omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_AIFC_AUDIO, pOMFSourceMob);
			OMFError = omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			OMFError = omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMFError = omfsWriteLength(OMFFileHdl, mediaDescriptor, OMMDFLLength, (omfLength_t)length); 
			if (gpGlobals->bVerboseMode)
				printf("%sAdded an AIFC Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			delete [] pSummary;
			rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			FinishUpMob(pMob, *pOMFSourceMob);
			goto cleanup;
		}
		if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&pCDCIDesc)))
		{
			aafUInt32				storedWidth, storedHeight, lineMapSize, n;
			aafUInt32				dispWidth, dispHeight, sampWidth, sampHeight, align;
			aafInt32				sampX, sampY, dispX, dispY;
			aafAlphaTransparency_t	alphaTrans;
			aafFrameLayout_t		layout;
			omfObject_t		mediaDescriptor;
			aafRational_t			aspect;
			omfRational_t		OMFAspect;
			aafUID_t				compressionID;
			char					*omfclassID;
			char					*omfCompression;

			// It is a CDCI file descriptor
			rc = pEssenceDesc->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&pDIDDDesc);//!!!
			rc = pDIDDDesc->GetCompression(&compressionID);
			if(memcmp(&compressionID, &kAAFCodecJPEG, sizeof(kAAFCodecJPEG)) == 0)
			{
				omfclassID = "CDCI";		// For now!!! (Get full toolkit w/all codecs later)
//				omfclassID = "JPED";
				omfCompression = "JFIF";
			}
			else
			{
				omfclassID = "CDCI";
				omfCompression = "";
			}

			OMFError = omfmFileMobNew(OMFFileHdl, pMobName, rate, omfclassID, pOMFSourceMob);
			OMFError = omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			OMFError = omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMFError = omfsWriteLength(OMFFileHdl, mediaDescriptor, OMMDFLLength, (omfLength_t)length); 
			
			// Get Digital Image properties and set them
			// Toolkit still expects signed
			pDIDDDesc->GetStoredView(&storedWidth, &storedHeight);
			OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMDIDDStoredHeight, storedHeight);
			OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMDIDDStoredWidth, storedWidth);

			if(pDIDDDesc->GetSampledView(&sampHeight, &sampWidth, &sampX, &sampY) == AAFRESULT_SUCCESS)
			{
				OMFError = omfsWriteUInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledHeight, sampHeight);
				OMFError = omfsWriteUInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledWidth, sampWidth);
				OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledXOffset, sampX);
				OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMDIDDSampledYOffset, sampY);
			}

			if(pDIDDDesc->GetDisplayView(&dispHeight, &dispWidth, &dispX, &dispY) == AAFRESULT_SUCCESS)
			{
				OMFError = omfsWriteUInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayHeight, dispHeight);
				OMFError = omfsWriteUInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayWidth, dispWidth);
				OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayXOffset, dispX);
				OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMDIDDDisplayYOffset, dispY);
			}
			rc = pDIDDDesc->GetFrameLayout(&layout);
			OMFError = omfsWriteLayoutType(OMFFileHdl, mediaDescriptor, OMDIDDFrameLayout, (omfFrameLayout_t)(layout+1));	// Toolkit used incorrect layout

//!!!			if(pDIDDDesc->GetGamma(&gamma) == AAFRESULT_SUCCESS)
//			{
//				OMFGamma.numerator = gamma.numerator;
//				OMFGamma.denominator = gamma.denominator;
//				OMFError = omfsWriteRational(OMFFileHdl, mediaDescriptor, OMDIDDGamma, OMFGamma);
//			}

			rc = pDIDDDesc->GetVideoLineMapSize(&lineMapSize);
			lineMap = new aafInt32[lineMapSize];
			rc = pDIDDDesc->GetVideoLineMap(lineMapSize, lineMap);
			// Possibly translate old line map #'s into new ones?!!!
			for(n = 0; n < lineMapSize; n++)
			{
				OMFError = OMWriteProp(OMFFileHdl, mediaDescriptor, OMDIDDVideoLineMap, 
				n*sizeof(aafInt32), OMInt32Array,
				sizeof(aafInt32), lineMap+n);
			}
			delete [] lineMap;
			lineMap = NULL;

			//
			OMFError = omfsWriteString(OMFFileHdl, mediaDescriptor, OMDIDDCompression, omfCompression);

			//
			if(pDIDDDesc->GetImageAspectRatio(&aspect) == AAFRESULT_SUCCESS)
			{
				OMFAspect.numerator = aspect.numerator;
				OMFAspect.denominator = aspect.denominator;
				OMFError = omfsWriteRational(OMFFileHdl, mediaDescriptor, OMDIDDImageAspectRatio, OMFAspect);
			}

			//
			if(pDIDDDesc->GetAlphaTransparency(&alphaTrans) == AAFRESULT_SUCCESS)
			{
				OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMDIDDAlphaTransparency, alphaTrans);
			}
			if(pDIDDDesc->GetImageAlignmentFactor(&align) == AAFRESULT_SUCCESS)
			{
				OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMDIDDFieldAlignment, align);
			}
			
			// retrieve CDCI descriptor properties
			aafInt32			componentWidth;
			aafUInt32			horizSubsample;
			aafColorSiting_t	cositing;

			// Find out which are optional & which must fail!!!
			rc = pCDCIDesc->GetComponentWidth(&componentWidth);
			OMFError = omfsWriteInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIComponentWidth, componentWidth); 

			rc = pCDCIDesc->GetHorizontalSubsampling(&horizSubsample);
			OMFError = omfsWriteUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIHorizontalSubsampling, horizSubsample); 

			rc = pCDCIDesc->GetColorSiting(&cositing);
			OMFError = OMWriteProp(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIColorSiting, 
						  0, OMColorSitingType,
						  sizeof(cositing), (void *)&(cositing));

			if (gpGlobals->bVerboseMode)
				printf("%sAdded a CDCI Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			FinishUpMob(pMob, *pOMFSourceMob);
			goto cleanup;
		}
		if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFObject, (void **)&pAAFObject)))
		{
			// Media descriptor not implemented or not recognized
			aafUID_t	ObjClass;
			char		szTempUID[64];

			// pAAFObject->GetObjectClass(&ObjClass);
			IAAFClassDef * classDef = 0;
			pAAFObject->GetDefinition(&classDef);
			IAAFDefObject * defObj = 0;
			classDef->QueryInterface(IID_IAAFDefObject, (void **)&defObj);
			defObj->GetAUID (&ObjClass);
			if (defObj)
			  {
				defObj->Release ();
				defObj = 0;
			  }
			if (classDef)
			  {
				classDef->Release ();
				classDef = 0;
			  }

			AUIDtoString(&ObjClass ,szTempUID);
			if (gpGlobals->bVerboseMode)
				printf("%sInvalid essence descripor type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
			fprintf(stderr,"%sInvalid essence descriptor type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
		}
		rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		FinishUpMob(pMob, *pOMFSourceMob);
		goto cleanup;
	}

cleanup:

	if (pOMFSourceMob)
		ConvertLocator(pEssenceDesc, pOMFSourceMob);
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
	if (pDIDDDesc)
		pDIDDDesc->Release();
	if(lineMap != NULL)
		delete [] lineMap;

	DecIndentLevel();
//	if (OMFError != OM_ERR_NONE)
//		rc = AAFRESULT_INTERNAL_ERROR;
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
void Aaf2Omf::TraverseMob(IAAFMob* pMob,
							 omfMobObj_t* pOMFMob)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	
	omfMSlotObj_t		OMFNewSlot = NULL;
	omfSegObj_t		OMFSegment = NULL;
	omfTrackID_t		OMFTrackID;
	omfRational_t		OMFeditRate;
	omfPosition_t		OMFOrigin;
	char*					pszTrackName = NULL;
	aafWChar*				pwTrackName = NULL;					

	IAAFComponent*			pComponent = NULL;
	IAAFMobSlot*			pSlot = NULL;
	IAAFTimelineMobSlot*	pTimelineMobSlot = NULL;
	IAAFSegment*			pSegment = NULL;
	IEnumAAFMobSlots*		pSlotIter = NULL;
	aafNumSlots_t			numSlots;
//	aafSearchCrit_t			criteria;
	aafUInt32				textSize;

	rc = pMob->CountSlots(&numSlots);

	rc = pMob->GetSlots(&pSlotIter);
	
	IncIndentLevel();
	if (gpGlobals->bVerboseMode)
	{
		printf("%sFound: %ld sub slots\n", gpGlobals->indentLeader, numSlots);
	}

	for (aafUInt32 times = 0; times< numSlots; times++)
	{
		if (AAFRESULT_SUCCESS == pSlotIter->NextOne(&pSlot))
		{
			//	Process Segment data
			pSlot->GetNameBufLen(&textSize);
			pwTrackName = new wchar_t[textSize/sizeof(wchar_t)];
			pSlot->GetName(pwTrackName, textSize);
			pwTrackName = new wchar_t[textSize/sizeof(wchar_t)];
			wcstombs(pszTrackName, pwTrackName, textSize/sizeof(wchar_t));
			pSlot->GetSlotID( (aafSlotID_t *)&OMFTrackID);
			if (SUCCEEDED(pSlot->QueryInterface(IID_IAAFTimelineMobSlot, (void **)&pTimelineMobSlot)))
			{
				pTimelineMobSlot->GetOrigin((aafPosition_t *)&OMFOrigin);
				pTimelineMobSlot->GetEditRate((aafRational_t *)&OMFeditRate);
				pTimelineMobSlot->Release();
				pTimelineMobSlot = NULL;
			}
			if (FAILED(pSlot->GetSegment(&pSegment)))
				break;
			pSegment->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
			ProcessComponent(pComponent, &OMFSegment);

			if(OMFSegment != NULL)
			{
				OMFError = omfiMobAppendNewTrack(OMFFileHdl,
					*pOMFMob,
					OMFeditRate,
					OMFSegment,
					OMFOrigin,
					OMFTrackID,
					pszTrackName, 
					&OMFNewSlot);
				if (gpGlobals->bVerboseMode)
				{
					printf("%sConverted SlotID: %d, Name: %s\n",gpGlobals->indentLeader, (int)OMFTrackID, pszTrackName);
				}
			}
			else
				printf("%sWARNING: Failed to convert SlotID: %d, Name: %s\n",gpGlobals->indentLeader, (int)OMFTrackID, pszTrackName);
				
			if (pszTrackName)
			{
				delete [] pszTrackName;
				pszTrackName = NULL;
			}
			if (pwTrackName)
			{
				delete [] pwTrackName;
				pwTrackName = NULL;
			}
			if (pSlot)
			{
				pSlot->Release();
				pSlot = NULL;
			}
			if (pComponent)
			{
				pComponent->Release();
				pComponent = NULL;
			}
			if (pSegment)
			{
				pSegment->Release();
				pSegment = NULL;
			}
		}
	}

	DecIndentLevel();
	pSlotIter->Release();
	pSlotIter = NULL;

//	if (OMFError != OM_ERR_NONE)
//		rc = AAFRESULT_INTERNAL_ERROR;
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
void Aaf2Omf::ProcessComponent(IAAFComponent* pComponent, 
								  omfObject_t* pOMFSegment)
{
	AAFCheck					rc;
	OMFCheck				OMFError;

	omfDDefObj_t		OMFDatakind;

	IAAFSequence*			pSequence = NULL;
	IAAFSourceClip*			pSourceClip = NULL;
	IAAFTimecode*			pTimecode = NULL;
	IAAFEdgecode*			pEdgecode = NULL;
	IAAFFiller*				pFiller = NULL;
	IAAFTransition*			pTransition = NULL;
	IAAFSelector*			pSelector = NULL;
	IAAFOperationGroup*		pEffect = NULL;
	IAAFScopeReference*		pScopeRef = NULL;
	IAAFDataDef*            pDataDef = 0;
	IAAFDefObject*          pDefObj = 0;
	IAAFNestedScope*		pNest = 0;
	IAAFEssenceGroup*		pGroup = 0;
	aafUID_t				datadef;
	aafLength_t				length;

	rc = AAFRESULT_SUCCESS;

	IncIndentLevel();
	

	rc = pComponent->GetDataDef(&pDataDef);

	rc = pDataDef->QueryInterface(IID_IAAFDefObject, (void **)&pDefObj);

	pDataDef->Release();
	pDataDef = 0;
	rc = pDefObj->GetAUID(&datadef);

	pDefObj->Release();
	pDefObj = 0;
	
	ConvertAAFDatadef(datadef, &OMFDatakind);

	rc = pComponent->GetLength(&length);
	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFSequence, (void **)&pSequence)))
	{
		// Component is a sequence
		OMFError = omfiSequenceNew(OMFFileHdl, OMFDatakind, pOMFSegment);
		if (gpGlobals->bVerboseMode)
			printf("%sProcessing Sequence\n", gpGlobals->indentLeader);
		TraverseSequence(pSequence, pOMFSegment);
		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFSourceClip, (void **)&pSourceClip)))
	{
		// component is a source clip
		aafSourceRef_t			ref;
		omfSourceRef_t	OMFSourceRef;
		aafFadeType_t			fadeInType, fadeOutType;
		aafLength_t				fadeInLen = 0, fadeOutLen = 0;
		aafBool					fadeInPresent = kAAFFalse, fadeOutPresent = kAAFFalse;
		
		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Source Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		// Get Source Clip properties
		if(pSourceClip->GetSourceReference(&ref) == AAFRESULT_SUCCESS)
		{
			pSourceClip->GetFade(&fadeInLen,
				&fadeInType,
				&fadeInPresent,
				&fadeOutLen,
				&fadeOutType,
				&fadeInPresent);
			
			if((fadeInPresent || fadeInPresent) && !EqualAUID(&datadef, &DDEF_Sound))
			{
				fadeInPresent = kAAFFalse;
				fadeOutPresent = kAAFFalse;
				printf("%sWARNING: Found fades on a non-audio Source Clip!\n ", gpGlobals->indentLeader);
			}
			
			ConvertMobIDtoUID(&ref.sourceID, &OMFSourceRef.sourceID);
			OMFSourceRef.sourceTrackID = (omfTrackID_t)ref.sourceSlotID;
			OMFSourceRef.startTime = (omfPosition_t)ref.startTime;
			// Create OMF Source Clip
			OMFError = omfiSourceClipNew(OMFFileHdl,
				OMFDatakind,
				(omfLength_t)length,
				OMFSourceRef,
				pOMFSegment);
			if ( (fadeInPresent && fadeInLen > 0) || (fadeOutPresent && fadeOutLen > 0) )
			{
				// Some 'magic' required to get types to match
				omfInt32 fadeInLen32 = (omfInt32) fadeInLen;
				omfInt32 fadeOutLen32 = (omfInt32) fadeOutLen;
				// Check that narrowing of data type didn't throw away
				// data
				if (((aafLength_t) fadeInLen32) != fadeInLen)
				{
					rc = AAFRESULT_INTERNAL_ERROR;
					goto cleanup;
				}
				if (((aafLength_t) fadeOutLen32) != fadeOutLen)
				{
					rc = AAFRESULT_INTERNAL_ERROR;
					goto cleanup;
				}
				OMFError = omfiSourceClipSetFade(OMFFileHdl, 
					*pOMFSegment,
					fadeInLen32,
					(omfFadeType_t)fadeInType,
					fadeOutLen32,
					(omfFadeType_t)fadeOutType);
			}
		}
		else
		{
			OMFSourceRef.sourceID.prefix = 0;
			OMFSourceRef.sourceID.major = 0;
			OMFSourceRef.sourceID.minor = 0;
			OMFSourceRef.sourceTrackID = 0;
			OMFSourceRef.startTime = 0;
			OMFError = omfiSourceClipNew(OMFFileHdl,
				OMFDatakind,
				(omfLength_t)0,
				OMFSourceRef,
				pOMFSegment);
		}

		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFTimecode, (void **)&pTimecode)))
	{
		// component is a Timecode
		omfTimecode_t	timecode;
		aafTimecode_t		AAFTimecode;

		pTimecode->GetTimecode(&AAFTimecode);

		// Some 'magic' required to get types to match; make sure
		// narrowing of type didn't throw away data
		timecode.startFrame = (omfFrameOffset_t) AAFTimecode.startFrame;
		if ((aafFrameOffset_t) timecode.startFrame != AAFTimecode.startFrame)
		{
		  rc = AAFRESULT_INTERNAL_ERROR;
		  goto cleanup;
		}

		timecode.drop = (omfDropType_t)AAFTimecode.drop;
		timecode.fps = AAFTimecode.fps;
		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Timecode Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
			IncIndentLevel();
			printf("%sstart Frame\t: %ld\n", gpGlobals->indentLeader, timecode.startFrame);
			if (timecode.drop == TRUE)
				printf("%sdrop\t\t: True\n", gpGlobals->indentLeader);
			else
				printf("%sdrop\t\t: False\n", gpGlobals->indentLeader);
			printf("%sFrames/second\t: %ld\n", gpGlobals->indentLeader, timecode.fps);     
			DecIndentLevel();				
		}

		OMFError = omfiTimecodeNew(OMFFileHdl, (omfLength_t)length, timecode, pOMFSegment);
		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFEdgecode, (void **)&pEdgecode)))
	{
		// component is an Edgecode
		aafEdgecode_t		edgecode;
		omfEdgecode_t	OMFEdgecode;

		pEdgecode->GetEdgecode(&edgecode);

		// Some 'magic' required to get types to match; make sure
		// narrowing of type didn't throw away data
		OMFEdgecode.startFrame = (omfFrameOffset_t) edgecode.startFrame;
		if ((aafFrameOffset_t) OMFEdgecode.startFrame != edgecode.startFrame)
		{
		  rc = AAFRESULT_INTERNAL_ERROR;
		  goto cleanup;
		}

		OMFEdgecode.filmKind = (omfFilmType_t)edgecode.filmKind;
		OMFEdgecode.codeFormat = (omfEdgeType_t)edgecode.codeFormat;
		memcpy(OMFEdgecode.header, edgecode.header, sizeof(edgecode.header));
		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Timecode Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
			IncIndentLevel();
			printf("%sstart Frame\t: %ld\n", gpGlobals->indentLeader, edgecode.startFrame);
			DecIndentLevel();				
		}
		OMFError = omfiEdgecodeNew(OMFFileHdl, (omfLength_t)length, OMFEdgecode, pOMFSegment);		
		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFFiller, (void **)&pFiller)))
	{
		// component is a filler
		OMFError = omfiFillerNew(OMFFileHdl, OMFDatakind, (omfLength_t)length, pOMFSegment);
		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Filler of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFOperationGroup, (void **)&pEffect)))
	{
//		//Component is an effect
		omfObject_t	nest = NULL;
		omfObject_t	effect = NULL;
		IAAFOperationDef	*pEffectDef = NULL;
		IAAFDefObject		*pDefObject = NULL;
		aafUID_t			effectDefAUID;

		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Effect of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		
		// Public effects don't get a surrounding NEST
		rc = pEffect->GetOperationDefinition(&pEffectDef);
		rc = pEffectDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject);
		pDefObject->GetAUID(&effectDefAUID);
		pEffectDef->Release();
		pDefObject->Release();

		if(pEffectTranslate->RequiresNestedScope(effectDefAUID))
		{
			OMFError = omfiNestedScopeNew(OMFFileHdl, OMFDatakind,
								(omfLength_t)length, &nest);
			ConvertEffects(pEffect, nest, &effect);
			OMFError = omfiNestedScopeAppendSlot(OMFFileHdl,nest,effect);
			*pOMFSegment = nest;
		}
		else
		{
			ConvertEffects(pEffect, NULL, pOMFSegment);
		}

		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFTransition, (void **)&pTransition)))
	{
		// component is a Transition
		omfEffObj_t	effect;

		aafPosition_t	cutPoint;

		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Transition of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		pTransition->GetCutPoint(&cutPoint);
		rc = pTransition->GetOperationGroup(&pEffect);
		// At this time (4/99) effects are not implemented therefore we 
		// will have to create an Effect from thin air.(hack it !!)
		ConvertEffects(pEffect, NULL, &effect);

		OMFError = omfiTransitionNew(OMFFileHdl,
									 OMFDatakind,
									 (omfLength_t)length,
									 (omfPosition_t)cutPoint,
									 effect,
									 pOMFSegment);
		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFSelector, (void **)&pSelector)))
	{
		// component is a selector
		IncIndentLevel();
		
		OMFError = omfiSelectorNew(OMFFileHdl,
								   OMFDatakind,
								   (omfLength_t)length,
								   pOMFSegment);
		ConvertSelector(pSelector, pOMFSegment);
		DecIndentLevel();
		goto cleanup;
	}
	
	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFNestedScope, (void **)&pNest)))
	{
		// component is a nested scope
		IncIndentLevel();
		
		OMFError = omfiNestedScopeNew(OMFFileHdl,
								   OMFDatakind,
								   (omfLength_t)length,
								   pOMFSegment);
		ConvertNestedScope(pNest, pOMFSegment);
		DecIndentLevel();
		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFEssenceGroup, (void **)&pGroup)))
	{
		// component is a nested scope
		IncIndentLevel();
		
		OMFError = omfiMediaGroupNew(OMFFileHdl,
								   OMFDatakind,
								   (omfLength_t)length,
								   pOMFSegment);
		ConvertEssenceGroup(pGroup, pOMFSegment);
		DecIndentLevel();
		goto cleanup;
	}

	if (SUCCEEDED(pComponent->QueryInterface(IID_IAAFScopeReference, (void **)&pScopeRef)))
	{
		// component is a source clip
		aafUInt32		relativeScope, relativeSlot;

		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Scope Reference of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		// Get Source Clip properties
        rc = pScopeRef->GetRelativeScope(&relativeScope);
        rc = pScopeRef->GetRelativeSlot(&relativeSlot);
		OMFError = omfiScopeRefNew(OMFFileHdl,
									 OMFDatakind,
									 (omfLength_t)length,
									 relativeScope,
									 relativeSlot,
									pOMFSegment);

		goto cleanup;
	}

	IAAFClassDef			*myClass;
	IAAFMetaDefinition		*classDef;
	IAAFObject				*pObj;
	aafUInt32				bufSize;
	wchar_t					*wbuf;
	char					*buf;

	pComponent->QueryInterface(IID_IAAFObject, (void **)&pObj);
	pObj->GetDefinition (&myClass);
	myClass->QueryInterface(IID_IAAFMetaDefinition, (void **)&classDef);

	classDef->GetNameBufLen(&bufSize);
	wbuf = new wchar_t[bufSize];
	buf = new char[bufSize/sizeof(wchar_t)];
	classDef->GetName (wbuf, bufSize);
	wcstombs(buf, wbuf, bufSize/sizeof(wchar_t));

	printf("%sInvalid component type found \"%s\", Translating into filler.\n", gpGlobals->indentLeader, buf);
	delete [] wbuf;
	delete [] buf;
	pObj->Release();
	myClass->Release();
	classDef->Release();

	// Translate the component into a filler
	OMFError = omfiFillerNew(OMFFileHdl, OMFDatakind, (omfLength_t)length, pOMFSegment);

cleanup:

	if (pScopeRef)
	  {
		pScopeRef->Release();
		pScopeRef = 0;
	  }
	
	if (pNest)
	  {
		pNest->Release();
		pNest = 0;
	  }

	if (pGroup)
	  {
		pGroup->Release();
		pGroup = 0;
	  }
	
	if (pSequence)
	  {
		pSequence->Release();
		pSequence = 0;
	  }
	
	if (pSourceClip)
	  {
		pSourceClip->Release();
		pSourceClip = 0;
	  }
	
	if (pTimecode)
	  {
		pTimecode->Release();
		pTimecode = 0;
	  }

	if (pFiller)
	  {
		pFiller->Release();
		pFiller = 0;
	  }
	
	if (pEdgecode)
	  {
		pEdgecode->Release();
		pEdgecode = 0;
	  }

	if (pTransition)
	  {
		pTransition->Release();
		pTransition = 0;
	  }

	if (pSelector)
	  {
		pSelector->Release();
		pSelector = 0;
	  }

	if (pDataDef)
	  {
		pDataDef->Release();
		pDataDef = 0;
	  }

	if (pDefObj)
	  {
		pDefObj->Release();
		pDefObj = 0;
	  }

	DecIndentLevel();
//	if (OMFError != OM_ERR_NONE)
//		rc = AAFRESULT_INTERNAL_ERROR;
}
// ============================================================================
// ConvertAAFDatadef
//
//			This function converts an AAF datadef into an OMF datakind. 
//			
// Returns: AAFRESULT_SUCCESS if datakind is converted succesfully
//
// ============================================================================
void Aaf2Omf::ConvertAAFDatadef(aafUID_t Datadef,
								   omfDDefObj_t* pDatakind)
{
	AAFCheck				rc;

	omfUniqueName_t	datakindName;
	omfBool			bFound;
	char					szAUID[64];

	if ( memcmp((char *)&Datadef, (char *)&DDEF_Picture, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Picture");
		bFound = omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&Datadef, (char *)&DDEF_Sound, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Sound");
		bFound = omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&Datadef, (char *)&DDEF_Timecode, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Timecode");
		bFound = omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&Datadef, (char *)&DDEF_Edgecode, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Edgecode");
		bFound = omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&Datadef, (char *)&DDEF_PictureWithMatte, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:PictureWithMatte");
		bFound = omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (omfErr_t *) &rc);
	}
	else
	{
		AUIDtoString(&Datadef, szAUID);
		printf("%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		fprintf(stderr,"%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		rc = AAFRESULT_INVALID_DATADEF;
	}

	if (!bFound)
		rc = AAFRESULT_INVALID_DATADEF;
}

// ============================================================================
// ConvertAAFTypeIDDatakind
//
//			This function converts an AAF type UID into an OMF datakind. 
//			
// Returns: AAFRESULT_SUCCESS if datakind is converted succesfully
//
// ============================================================================
void Aaf2Omf::ConvertAAFTypeIDDatakind(aafUID_t typeID, omfDDefObj_t* pDatakind)
{
	AAFCheck				rc;

	omfUniqueName_t	datakindName;
	omfBool			bFound;
	char					szAUID[64];

	if ( memcmp((char *)&typeID, (char *)&kAAFTypeID_Rational, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Rational");
		bFound = omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&typeID, (char *)&kAAFTypeID_Int32, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Int32");
		bFound = omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&typeID, (char *)&kAAFTypeID_Boolean, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Boolean");
		bFound = omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (omfErr_t *) &rc);
	}
	else
	{
		AUIDtoString(&typeID, szAUID);
		printf("%sInvalid Type Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		fprintf(stderr,"%sInvalid Type Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		rc = AAFRESULT_INVALID_DATADEF;
	}

	if (!bFound)
		rc = AAFRESULT_INVALID_DATADEF;
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
void Aaf2Omf::TraverseSequence(IAAFSequence* pSequence,
								  omfObject_t* pOMFSequence )
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	omfCpntObj_t		newComponent = NULL;

	IAAFComponent*			pComponent = NULL;
	IEnumAAFComponents*		pCompIter = NULL;
	aafUInt32				numComponents;
	aafUInt32				cpntCount;
	

	rc = pSequence->CountComponents(&numComponents);
	
	rc = pSequence->GetComponents(&pCompIter);
	for (cpntCount = 0; cpntCount < numComponents; cpntCount++)
	{
		rc = pCompIter->NextOne(&pComponent);
		ProcessComponent(pComponent, &newComponent);
		OMFError = omfiSequenceAppendCpnt(OMFFileHdl, *pOMFSequence, newComponent);
		pComponent->Release();
		pComponent = NULL;
	}
	pCompIter->Release();
	pCompIter = NULL;
//	if (OM_ERR_NONE != OMFError)
//		rc = AAFRESULT_INTERNAL_ERROR;

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
void Aaf2Omf::ConvertSelector(IAAFSelector* pSelector,
								 omfObject_t* pOMFSelector )
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	omfSegObj_t		OMFSelected = NULL;
	
	IAAFComponent*			pComponent = NULL;
	IAAFSegment*			pSegment = NULL;
	aafLength_t				length;
	aafInt32				numAlternates;
	
	IncIndentLevel();
	pSelector->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	pSelector->GetSelectedSegment(&pSegment);
	pSelector->GetNumAlternateSegments(&numAlternates);
	pComponent->GetLength(&length);
	
	if (gpGlobals->bVerboseMode)
		printf("%sProcessing Selector object of length = %ld\n", gpGlobals->indentLeader, length);
	
	pComponent->Release();
	pComponent = NULL;
	pSegment->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	ProcessComponent(pComponent, &OMFSelected );
	
	OMFError = omfiSelectorSetSelected(OMFFileHdl, *pOMFSelector, OMFSelected);
	if (numAlternates > 0)
	{
		omfSegObj_t	OMFAltSelected = NULL;
		
		IAAFComponent*		pAltComponent = NULL;
		IEnumAAFSegments*	pEnumAlternates = NULL;
		IAAFSegment*		pAltSegment = NULL;
		
		rc = pSelector->EnumAlternateSegments(&pEnumAlternates);
		for (int i = 0; i< numAlternates;i++)
		{
			pEnumAlternates->NextOne(&pAltSegment);
			pAltSegment->QueryInterface(IID_IAAFComponent, (void **)&pAltComponent);
			ProcessComponent(pAltComponent, &OMFAltSelected);
			OMFError = omfiSelectorAddAlt(OMFFileHdl, *pOMFSelector, OMFAltSelected);
			pAltComponent->Release();
			pAltSegment->Release();
		}
		
		pEnumAlternates->Release();
	}
	
	DecIndentLevel();
	if (pSegment)
		pSegment->Release();
	if (pComponent)
		pComponent->Release();
	
//	if (OM_ERR_NONE != OMFError)
//		rc = AAFRESULT_INTERNAL_ERROR;
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
void Aaf2Omf::ConvertNestedScope(IAAFNestedScope* pNest,
								 omfObject_t* pOMFNest )
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	omfSegObj_t		OMFSegment = NULL;

	IAAFComponent*			pComponent = NULL;
	IAAFSegment*			pSegment = NULL;
	IEnumAAFSegments*		pEnumSegments;
	aafLength_t				length;

	IncIndentLevel();

	pNest->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	pComponent->GetLength(&length);

	if (gpGlobals->bVerboseMode)
		printf("%sProcessing Nest object of length = %ld\n", gpGlobals->indentLeader, length);

	rc = pNest->GetSegments (&pEnumSegments);
	while(pEnumSegments->NextOne (&pSegment) == AAFRESULT_SUCCESS)
	{
		pSegment->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
		ProcessComponent(pComponent, &OMFSegment);
		OMFError = omfiNestedScopeAppendSlot(OMFFileHdl, *pOMFNest, OMFSegment);
		pSegment->Release();
		pSegment = NULL;
		pComponent->Release();
		pComponent = NULL;
	}
	DecIndentLevel();
	if (pSegment)
		pSegment->Release();
	if (pComponent)
		pComponent->Release();

//	if (OM_ERR_NONE != OMFError)
//		rc = AAFRESULT_INTERNAL_ERROR;
}

// ============================================================================
// ConvertEssenceGroup
//
//			This function converts an AAF EssenceGroup object and all the objects it
// contains or references.
//			
// Returns: AAFRESULT_SUCCESS if succesfully
//
// ============================================================================
void Aaf2Omf::ConvertEssenceGroup(IAAFEssenceGroup* pGroup,
								 omfObject_t* pOMFGroup )
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	omfSegObj_t		OMFSegment = NULL;

	IAAFComponent*			pComponent = NULL;
	IAAFSegment*			pSegment = NULL;
	IAAFSourceClip	*		pSourceClip;
	aafLength_t				length;
	aafUInt32				numChoices, n;

	IncIndentLevel();

	pGroup->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	pComponent->GetLength(&length);

	if (gpGlobals->bVerboseMode)
		printf("%sProcessing EssenceGroup object of length = %ld\n", gpGlobals->indentLeader, length);

	rc = pGroup->CountChoices (&numChoices);
	for(n = 0; n < numChoices; n++)
	{
		rc = pGroup->GetChoiceAt (n, &pSegment);
	  
	    pSegment->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
		ProcessComponent(pComponent, &OMFSegment);
		OMFError = omfiMediaGroupAddChoice(OMFFileHdl, *pOMFGroup, OMFSegment);
		pSegment->Release();
		pSegment = NULL;
		pComponent->Release();
		pComponent = NULL;
	}

	if(pGroup->GetStillFrame (&pSourceClip) == AAFRESULT_SUCCESS)
	{
		aafSourceRef_t			ref;
		
		if(pSourceClip->GetSourceReference(&ref) == AAFRESULT_SUCCESS)
		{
			rc = pSourceClip->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
			ProcessComponent(pComponent, &OMFSegment);
			OMFError = omfiMediaGroupSetStillFrame(OMFFileHdl, *pOMFGroup, OMFSegment);
			pComponent->Release();
			pComponent = NULL;
		}
	}

	DecIndentLevel();
	if (pSegment)
		pSegment->Release();
	if (pComponent)
		pComponent->Release();
	if (pSourceClip)
		pSourceClip->Release();

//	if (OM_ERR_NONE != OMFError)
//		rc = AAFRESULT_INTERNAL_ERROR;
}
// ============================================================================
// ConvertLocator
//
//			This function converts an AAF Filed descriptor to OMF
//			
// Returns: AAFRESULT_SUCCESS if succesfully
//
// ============================================================================
void Aaf2Omf::ConvertLocator(IAAFEssenceDescriptor* pEssenceDesc,
								omfMobObj_t*	pOMFSourceMob )
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	// omfClassID_t		locType;

	char*					pszLocatorPath = NULL;
	char*					pszName = NULL;

	IAAFLocator*			pLocator = NULL;
	IAAFTextLocator*		pTextLocator = NULL;
	IEnumAAFLocators*		pLocatorIter = NULL;		
	aafUInt32				numLocators = 0;
    aafWChar*				pwLocatorPath = NULL;
	aafWChar*				pwName = NULL;

	aafUInt32				pathSize = 0;
	aafUInt32				textSize = 0;

	rc = pEssenceDesc->CountLocators(&numLocators);
	if(numLocators > 0)
	{
		rc = pEssenceDesc->GetLocators(&pLocatorIter);
		while (SUCCEEDED(pLocatorIter->NextOne(&pLocator)))
		{
			if (SUCCEEDED(pLocator->QueryInterface(IID_IAAFTextLocator, (void **)&pTextLocator)))
			{
				rc = pTextLocator->GetNameBufLen(&textSize);
				pwName = new wchar_t[textSize/sizeof(wchar_t)];
				rc = pTextLocator->GetName(pwName, textSize);

				pszName = new char[textSize/sizeof(wchar_t)];
				wcstombs(pszName, pwName, textSize/sizeof(wchar_t));
				OMFError = omfmMobAddTextLocator(OMFFileHdl, *pOMFSourceMob, pszName);
				if (pwName)
					delete [] pwName;
				if (pszName)
					delete [] pszName;
			}
			else
			{
				pLocator->GetPathBufLen(&pathSize);
				pwLocatorPath = new wchar_t[pathSize/sizeof(wchar_t)];
				pLocator->GetPath(pwLocatorPath, pathSize);
				pszLocatorPath = new char[pathSize/sizeof(wchar_t)];
				wcstombs(pszLocatorPath, pwLocatorPath, pathSize/sizeof(wchar_t));
				OMFError = omfmMobAddNetworkLocator(OMFFileHdl, *pOMFSourceMob, kOmfiMedia, pszLocatorPath);
				if (pwLocatorPath)
				{
					delete [] pwLocatorPath;
					pwLocatorPath = NULL;
				}
				if (pszLocatorPath)
				{
					delete [] pszLocatorPath;
					pszLocatorPath = NULL;
				}
			}
		}
	}
	if (pTextLocator)
		pTextLocator->Release();
	if (pLocator)
		pLocator->Release();
	if (pLocatorIter)
		pLocatorIter->Release();
	if (pwLocatorPath)
		delete [] pwLocatorPath;
	if (pszLocatorPath)
		delete [] pszLocatorPath;

//	if (OMFError != OM_ERR_NONE)
//		rc = AAFRESULT_INTERNAL_ERROR;
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
void Aaf2Omf::ConvertEssenceDataObject( IAAFEssenceData* pEssenceData)
{
	AAFCheck				rc;
	OMFCheck				OMFError;
	
	omfObject_t		OMFSourceMob = NULL;
	omfObject_t		OMFHeader = NULL;
	omfObject_t		mediaData = NULL;
	omfUID_t			mediaID;
	omfProperty_t		idProperty;
	omfDDefObj_t		datakind;
	char					id[5];
	
	IAAFEssenceData*			pTIFFData = NULL;
	IAAFEssenceData*			pAIFCData = NULL;
	IAAFEssenceData*			pWAVEData = NULL;
	IAAFEssenceData*			pJPEGData = NULL;
	IAAFTIFFDescriptor*			pTIFFDesc = NULL;
	IAAFAIFCDescriptor*			pAIFCDesc = NULL;
	IAAFWAVEDescriptor*			pWAVEDesc = NULL;
	IAAFCDCIDescriptor*			pJPEGDesc = NULL;
	IAAFEssenceDescriptor		*pEssenceDesc = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFObject*				pAAFObject = NULL;
	aafMobID_t				mobID;
	
	// get the file mob id
	rc = pEssenceData->GetFileMobID(&mobID);

	// make sure it is a Source mob
	rc = pEssenceData->GetFileMob(&pSourceMob);
	
	if (FAILED(pSourceMob->GetEssenceDescriptor(&pEssenceDesc)))
	{
		pSourceMob->Release();
		return;
	}
	
	ConvertMobIDtoUID(&mobID, &mediaID);
	if (omfsFindSourceMobByID(OMFFileHdl, mediaID, &OMFSourceMob) != OM_ERR_NONE)
	{
		rc = AAFRESULT_INVALID_OBJ;
		pSourceMob->Release();
		return;
	}
	OMFError = omfsGetHeadObject( OMFFileHdl, &OMFHeader );
	
	// !!! Tomas, there is only one kind of essence data now, so this code will
	// need to find the EssenceDescriptor of the associated file mob (lookup he
	// dataID in the MOB index on the dictionary) and do a QI on the result
	// The code is something like:
	//
	if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFTIFFDescriptor, (void **)&pTIFFDesc)))
	{
		//Convert TIFF Essence data
		idProperty = OMTIFFData;
		omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (omfErr_t *)&rc);
		strcpy(id, "TIFF");
		OMFError = omfsObjectNew(OMFFileHdl, id, &mediaData);

		OMFError = omfsWriteUID(OMFFileHdl, mediaData, OMMDATMobID, mediaID);
//		if (OMFError != OM_ERR_NONE)
//		{
//			char* pErrString = omfsGetErrorString(OMFError);
//			goto cleanup;
//		}
		OMFError = omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMHEADMediaData, mediaData);
//		if (OMFError != OM_ERR_NONE)
//		{
//			char* pErrString = omfsGetErrorString(OMFError);
//			goto cleanup;
//		}
//		else
			goto CopyMedia;
	}
	
	if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc)))
	{
		//Convert AIFC Essence data
		idProperty = OMAIFCData;
		omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (omfErr_t *)&rc);
		strcpy(id, "AIFC");
		OMFError = omfsObjectNew(OMFFileHdl, id, &mediaData);
//		if (OMFError != OM_ERR_NONE)
//			goto cleanup;
		OMFError = omfsWriteUID(OMFFileHdl, mediaData, OMMDATMobID, mediaID);
//		if (OMFError != OM_ERR_NONE)
//		{
//			char* pErrString = omfsGetErrorString(OMFError);
//			goto cleanup;
//		}
		OMFError = omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMHEADMediaData, mediaData);
//		if (OMFError != OM_ERR_NONE)
//			goto cleanup;
//		else
			goto CopyMedia;
	}
	
	if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&pWAVEDesc)))
	{
		//Convert WAVE Essence data
		idProperty = OMWAVEData;
		omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (omfErr_t *)&rc);
		strcpy(id, "WAVE");
		OMFError = omfsObjectNew(OMFFileHdl, id, &mediaData);
//		if (OMFError != OM_ERR_NONE)
//			goto cleanup;
		OMFError = omfsWriteUID(OMFFileHdl, mediaData, OMMDATMobID, mediaID);
//		if (OMFError != OM_ERR_NONE)
//		{
//			char* pErrString = omfsGetErrorString(OMFError);
//			goto cleanup;
//		}
		OMFError = omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMHEADMediaData, mediaData);
//		if (OMFError != OM_ERR_NONE)
//			goto cleanup;
//		else
			goto CopyMedia;
	}
	
	//!!!Need to check "compression" flag to determine if really JPEG
	if (SUCCEEDED(pEssenceDesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&pJPEGDesc)))
	{
		//Convert JPEG Essence data
		idProperty = OMIDATImageData;
		omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (omfErr_t *)&rc);
		strcpy(id, "JPEG");
		OMFError = omfsObjectNew(OMFFileHdl, id, &mediaData);
//		if (OMFError != OM_ERR_NONE)
//			goto cleanup;
		OMFError = omfsWriteUID(OMFFileHdl, mediaData, OMMDATMobID, mediaID);
//		if (OMFError != OM_ERR_NONE)
//		{
//			char* pErrString = omfsGetErrorString(OMFError);
//			goto cleanup;
//		}
		OMFError = omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMHEADMediaData, mediaData);
//		if (OMFError != OM_ERR_NONE)
//			goto cleanup;
//		else
			goto CopyMedia;
	}
	
	// Media type not supported or invalid
	rc = pEssenceDesc->QueryInterface(IID_IAAFObject, (void **)&pAAFObject);
	
	{
		aafUID_t	ObjClass;
		char		szTempUID[64];
		
		// pAAFObject->GetObjectClass(&ObjClass);
		IAAFClassDef * classDef = 0;
		pAAFObject->GetDefinition(&classDef);
		IAAFDefObject * defObj = 0;
		pEssenceDesc->QueryInterface(IID_IAAFDefObject, (void **)&defObj);
		defObj->GetAUID (&ObjClass);
		if (defObj)
		{
			defObj->Release ();
			defObj = 0;
		}
		if (classDef)
		{
			classDef->Release ();
			classDef = 0;
		}
		
		AUIDtoString(&ObjClass ,szTempUID);
		if (gpGlobals->bVerboseMode)
			printf("%sInvalid essence data type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
		fprintf(stderr,"%sInvalid essence data type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
	}
	goto cleanup;
	
CopyMedia:
	if (mediaData)
	{
		void*			pBuffer = NULL;
		aafPosition_t	AAFOffset;
		
		aafLength_t		numBytes;
		aafUInt32		nBlockSize;
		aafUInt32		numBytesRead;
		
		rc = pEssenceData->GetSize(&numBytes);
		if (numBytes > 0)
		{
			if (numBytes > (2 * 1048576))
			{
				nBlockSize = 2 * 1048576;	// only allocate 2 Meg
			}
			else
			{
				// Some 'magic' required to get types to match; make sure
				// narrowing of type didn't throw away data
				nBlockSize = (aafUInt32) numBytes;
				if ((aafLength_t) nBlockSize != numBytes)
				{
					rc = AAFRESULT_INTERNAL_ERROR;
					goto cleanup;
				}
			}
			pBuffer = new char[nBlockSize];
			AAFOffset = 0;
			do 
			{
				rc = pEssenceData->SetPosition( AAFOffset );
				rc = pEssenceData->Read( nBlockSize, (unsigned char *)pBuffer, &numBytesRead);
				
				// write the media
				OMFError = omfsWriteDataValue(OMFFileHdl, 
					mediaData,
					idProperty,
					datakind, 
					(omfDataValue_t)pBuffer,
					(omfPosition_t)AAFOffset,
					numBytesRead);
				// calculate next offset
				AAFOffset += numBytesRead;
			}while (numBytes > AAFOffset );
			// Free the allocated buffer 
			delete [] (char*)pBuffer;
		}
	}
cleanup:
	if (pSourceMob)
		pSourceMob->Release();
	if (pEssenceDesc)
		pEssenceDesc->Release();
	
	if (pTIFFData)
		pTIFFData->Release();
	
	if (pAIFCData)
		pAIFCData->Release();
	
	if (pWAVEData)
		pWAVEData->Release();
	
	if (pJPEGData)
		pJPEGData->Release();
	
//	if (OMFError != OM_ERR_NONE)
//		rc = AAFRESULT_INTERNAL_ERROR;
}
// ============================================================================
// ConvertEffects
//
//			This function reads an OMF effect object, converts its properties
//			to AAF, updates the AAF Effect object and, if necessary creates the 
//			effect definition by Calling ConvertOMFEffectDefinition. 
//			
// Returns: AAFRESULT_SUCCESS if object is converted.
//
// ============================================================================
void Aaf2Omf::ConvertEffects(IAAFOperationGroup* pEffect,
								omfObject_t	 nest,
								omfEffObj_t*	pOMFEffect)
{
	AAFCheck				rc;
	OMFCheck				omStat;
	omfDDefObj_t		effectDatakind;
	// omfLength_t		effectLength;
	omfEDefObj_t		effectDef;
	// omfDDefObj_t		OMFdatakind;
	omfUniqueName_t	effectID;
	omfUniqueName_t	MCEffectID;
	omfErr_t			testErr;
	OMFCheck				OMFError;
	omfBool			bDefExists;
	omfObject_t		pOMFSegment, pOMFEffectSlot, pScopeReference;
	
	IAAFOperationDef*		pEffectDef = NULL;
	IAAFParameterDef*		pParameterDef = NULL;
	IAAFParameter*			pParameter = NULL;
	IAAFDefObject*			pDefObject = NULL;
	IAAFSegment*			pSegment = NULL;
	IAAFSourceReference*		pSourceRef= NULL;
	IAAFFiller*				pFiller = NULL;
	IAAFComponent*			pComponent = NULL;
	IAAFSourceClip*			pSourceClip = NULL;
	IAAFDataDef*			pDataDef = 0;
	IAAFDefObject* 			pDefObj = 0;
	
	aafUID_t				effectAUID;
	aafUID_t				effectDefAUID;
	aafLength_t				length;
	aafUInt32				byPass, bypassOverride, *byPassPtr;
	aafUInt32				textSize;
	aafBool					isATimeWarp;
	aafUInt32				n;
	aafUInt32				numSources,numParameters;
	aafWChar*				pwDesc = NULL;
	aafWChar*				pwName = NULL;
	
	char*					pszName = NULL;
	char*					pszDesc = NULL;
	
	IncIndentLevel();
	
	rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	
	pComponent->GetLength(&length);
	rc = pComponent->GetDataDef(&pDataDef);
	rc = pDataDef->QueryInterface(IID_IAAFDefObject, (void **)&pDefObj);
	pDataDef->Release ();
	pDataDef = 0;
	rc = pDefObj->GetAUID(&effectAUID);
	pDefObj->Release ();
	pDefObj = 0;
	ConvertAAFDatadef(effectAUID, &effectDatakind);
	pEffect->IsATimeWarp(&isATimeWarp);
	pEffect->CountSourceSegments(&numSources);
	pEffect->CountParameters(&numParameters);
	rc = pEffect->GetOperationDefinition(&pEffectDef);
	// pEffectDef->GetDataDefinitionID(&datadefAUID);
	rc = pEffectDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject);
	pDefObject->GetAUID(&effectDefAUID);
	pDefObject->GetNameBufLen(&textSize);
	pwName = new wchar_t[textSize/sizeof(wchar_t)];
	pDefObject->GetName(pwName, textSize);
	pszName = new char[textSize/sizeof(wchar_t)];
	wcstombs(pszName, pwName, textSize/sizeof(wchar_t));
	
	pDefObject->GetDescriptionBufLen(&textSize);
	pwDesc = new wchar_t[textSize/sizeof(wchar_t)];
	pDefObject->GetDescription(pwDesc, textSize);
	pszDesc = new char[textSize/sizeof(wchar_t)];
	wcstombs(pszDesc, pwDesc, textSize/sizeof(wchar_t));
	
	if(pEffectDef->GetBypass(&byPass) == AAFRESULT_SUCCESS)
		byPassPtr = &byPass;
	else
		byPassPtr = NULL;
	
	pEffectTranslate->GetEffectIDs(pEffect, effectID, MCEffectID);
	
	// At some point we should try to reconstruct the AVX private effect ID
	// from the AAF File
#if AVID_SPECIAL
	if(strcmp(MCEffectID, AVX_PLACEHOLDER_EFFECT) == 0)
	{
		IAAFConstantValue*		pConstantValue = NULL;


		strcpy(MCEffectID, "UnknownAVX Effect");
		strcpy(effectID, "omfi:effect:Unknown");
		
		if(pEffect->LookupParameter(kAAFParamID_AvidEffectID, &pParameter) == AAFRESULT_SUCCESS)
			rc = pParameter->QueryInterface(IID_IAAFConstantValue, (void **)&pConstantValue);

		aafUInt32			srcValueLen, lenRead;
		rc = pConstantValue->GetValueBufLen(&srcValueLen);
		//				Assert(srcValueLen <= sizeof(MCEffectID));
		rc = pConstantValue->GetValue(srcValueLen, (unsigned char*)MCEffectID, &lenRead);
		pConstantValue->Release();
		pConstantValue = NULL;
		pParameter->Release();
		pParameter = NULL;
	}
#endif
	
	bDefExists = omfiEffectDefLookup(OMFFileHdl, effectID, &effectDef, &testErr);
	if (testErr == OM_ERR_NONE && !bDefExists)
	{
		rc = omfiEffectDefNew(OMFFileHdl,
			effectID,
			pszName,
			pszDesc,
			(omfArgIDType_t *)byPassPtr,
			(omfBool)isATimeWarp,
			&effectDef);
	}
	
	omStat = omfiEffectNew(OMFFileHdl,
		effectDatakind,
		(omfLength_t)length,
		effectDef,
		pOMFEffect);
	
	
	if(MCEffectID[0] != '\0')
	{
		omStat = omfsWriteString(OMFFileHdl, (*pOMFEffect), gpGlobals->pvtEffectIDProp,
			MCEffectID);
	}
	
	if(SUCCEEDED(pEffect->GetRender(&pSourceRef)))
	{
		rc = pSourceRef->QueryInterface(IID_IAAFSourceClip, (void **) &pSourceClip);
		rc = pSourceRef->QueryInterface(IID_IAAFComponent, (void **) &pComponent);
		ProcessComponent(pComponent,&pOMFSegment);
		omStat = omfiEffectSetFinalRender(OMFFileHdl,(*pOMFEffect), pOMFSegment);
		pSourceClip->Release();
		pSourceClip = NULL;
		pComponent->Release();
		pComponent = NULL;
	}
	
	if(SUCCEEDED(pEffectDef->GetBypass(&bypassOverride)))
	{
		omStat = omfiEffectSetBypassOverride(OMFFileHdl,(*pOMFEffect),
			bypassOverride);	//!!! 1-1 mapping?
	}
	
	rc = pEffect->CountSourceSegments(&numSources);
	for(n = 0; n < numSources; n++)
	{
		rc = pEffect->GetInputSegmentAt(n, &pSegment);
		rc = pSegment->QueryInterface(IID_IAAFComponent,
												(void **) &pComponent);
		
		if(pComponent != NULL)
		{
			ProcessComponent(pComponent,&pOMFSegment);
			if(nest != NULL)
			{
				omStat = omfiNestedScopeAppendSlot(OMFFileHdl,nest, pOMFSegment);
				omStat = omfiScopeRefNew(OMFFileHdl,
					effectDatakind,
					(omfLength_t)length,
					0,
					1,		// This may not be a constant
					&pScopeReference);
				omStat = omfiEffectAddNewSlot(OMFFileHdl,(*pOMFEffect),
					-1*(n+1), pScopeReference, &pOMFEffectSlot);
			}
			else
			{
				omStat = omfiEffectAddNewSlot(OMFFileHdl,(*pOMFEffect),
					-1*(n+1), pOMFSegment, &pOMFEffectSlot);
			}
			pComponent->Release();
			pComponent = NULL;
		}
		//!!! Else error
		
		pSegment->Release();
	}
	
	// If the effect ID is known, map to a apecific OMF effect ID
	if(pEffect->LookupParameter(kAAFParameterDefLevel, &pParameter) == AAFRESULT_SUCCESS)
	{
		aafInt32	levelSlot;
#if AVID_SPECIAL
		if(pEffectTranslate->isPrivateEffect(effectDefAUID))
			levelSlot = OMF2_EFFE_ALLOTHERS_LEVEL_SLOT;
		else
#endif
			levelSlot = -3;
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect), levelSlot,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParameterDefSMPTEWipeNumber, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  1,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParameterDefSMPTEReverse, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  2,
			(omfLength_t)length);
	}
#if AVID_SPECIAL
	if(pEffect->LookupParameter(kAAFParamID_AvidUserParam, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  0,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidBounds, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  0,
			(omfLength_t)length);
	}
#endif
#if 0
	if(pEffect->LookupParameter(kAAFParameterDefAmplitude, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParameterDefPan, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	//		kAAFParameterDefSMPTEReverse
	//		kAAFParameterDefSMPTESoft
	//		kAAFParameterDefSMPTEBorder
	//		kAAFParameterDefSMPTEPosition
	//		kAAFParameterDefSMPTEModulator
	//		kAAFParameterDefSMPTEShadow
	//		kAAFParameterDefSMPTETumble
	//		kAAFParameterDefSMPTESpotlight
	//		kAAFParameterDefSMPTEReplicationH
	//		kAAFParameterDefSMPTEReplicationV
	//		kAAFParameterDefSMPTECheckerboard
	if(pEffect->LookupParameter(kAAFParameterDefPhaseOffset, &pParameter) == AAFRESULT_SUCCESS)
	{
		convertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParameterDefSpeedRatio, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidBorderWidth, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidBorderSoft, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidXPos, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidYPos, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidCrop, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidScale, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidSpillSupress, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
	if(pEffect->LookupParameter(kAAFParamID_AvidColor, &pParameter) == AAFRESULT_SUCCESS)
	{
		ConvertParameter(pParameter, (*pOMFEffect),  xxx,
			(omfLength_t)length);
	}
#endif
	
	DecIndentLevel();
	if (pwName)
	{
		delete [] pwName;
		pwName = 0;
	}
	
	if (pwDesc)
	{
		delete [] pwDesc;
		pwDesc = 0;
	}
	
	if (pszName)
	{
		delete [] pszName;
		pszName = 0;
	}
	
	if (pszDesc)
	{
		delete [] pszDesc;
		pszDesc = 0;
	}
	
	if (pEffectDef)
	{
		pEffectDef->Release();
		pEffectDef = 0;
	}
	
	if (pParameterDef)
	{
		pParameterDef->Release();
		pParameterDef = 0;
	}
	
	if (pParameter)
	{
		pParameter->Release();
		pParameter = 0;
	}
	
	if (pDefObject)
	{
		pDefObject->Release();
		pDefObject = 0;
	}
	
	//	if (pSegment)
	//	{
	//	  pSegment->Release();
	//	  pSegment = 0;
	//	}
	
	if (pSourceRef)
	{
		pSourceRef->Release();
		pSourceRef = 0;
	}
	
	if (pFiller)
	{
		pFiller->Release();
		pFiller = 0;
	}
	
	if (pComponent)
	{
		pComponent->Release();
		pComponent = 0;
	}
	
	if (pSourceClip)
	{
		pSourceClip->Release();
		pSourceClip = 0;
	}
	
	if (pDataDef)
	{
		pDataDef->Release ();
		pDataDef = 0;
	}
	
	if (pDefObj)
	{
		pDefObj->Release ();
		pDefObj = 0;
	}
}

void Aaf2Omf::ConvertParameter(	IAAFParameter*		pParm,
									aafUID_t			&effectDefID,
								  omfSegObj_t		pOMFEffect,
									omfInt32		slotNum,
									omfLength_t	effectLen)
{
	IAAFConstantValue	*pConstantValue = NULL;
	IAAFVaryingValue	*pVaryingValue = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	IAAFDefObject		*pDef = NULL;
	IAAFInterpolationDef	*pInterpDef = NULL;
	IAAFParameterDef	*pParmDef = NULL;
	IAAFControlPoint	*pPoint = NULL;
	IEnumAAFControlPoints *pointEnum;
	aafUID_t			typeDefID, interpDefID, paramDefID;
	AAFCheck			rc;
	OMFCheck			omStat;
	aafUInt32			srcValueLen, destValueLen, lenRead;
	aafDataBuffer_t		srcValue = NULL, destValue = NULL;
	omfInterpKind_t interpKind;
	omfObject_t	vval;
	aafRational_t		aafTime;
	omfRational_t	omfTime;
	aafEditHint_t		aafEditHint;
	omfEditHint_t	omfEditHint;
	omfSegObj_t	omfSeg;
	omfObject_t	pOMFEffectSlot;
	omfObject_t	pOMFDatakind;
	bool				didNew;

    rc = pParm->GetTypeDefinition(&pTypeDef);
	rc = pTypeDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef);
    rc = pDef->GetAUID(&typeDefID);
	pDef->Release();
	pDef = NULL;
	//
    rc = pParm->GetParameterDefinition(&pParmDef);
	rc = pParmDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef);
    rc = pDef->GetAUID(&paramDefID);
	pParmDef->Release();
	pParmDef = NULL;
	pDef->Release();
	pDef = NULL;

	//!!! This one parameter ID has no OMF equivilent.  make a routine which
	// knows the valid ones to let through.
#if AVID_SPECIAL
	if(memcmp(&paramDefID, &kAAFParamID_AvidSelected, sizeof(paramDefID)) != 0 &&
	   memcmp(&paramDefID, &kAAFParamID_AvidUserParam, sizeof(paramDefID)) != 0)
#endif
	{
		ConvertAAFTypeIDDatakind(typeDefID, &pOMFDatakind);
		
		if (SUCCEEDED(pParm->QueryInterface(IID_IAAFConstantValue, (void **)&pConstantValue)))
		{
			rc = pConstantValue->GetValueBufLen(&srcValueLen);
			srcValue = new unsigned char[srcValueLen];
			rc = pConstantValue->GetValue(srcValueLen, srcValue, &lenRead);
			ConvertValueBuf(typeDefID, srcValue, lenRead, &destValue, &destValueLen, &didNew);
			omStat = omfiConstValueNew(OMFFileHdl, pOMFDatakind, effectLen,
				destValueLen, destValue, &omfSeg);
			if(didNew)
				delete [] destValue;
			delete [] srcValue;
			destValue = NULL;
			srcValue = NULL;
			omStat = omfiEffectAddNewSlot(OMFFileHdl,pOMFEffect,
				slotNum, omfSeg, &pOMFEffectSlot);
		}
		else
		{
			rc = pParm->QueryInterface(IID_IAAFVaryingValue, (void **)&pVaryingValue);

			rc = pVaryingValue->GetInterpolationDefinition(&pInterpDef);
			rc = pInterpDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef);
			pInterpDef->Release();
			pInterpDef = NULL;
			rc = pDef->GetAUID(&interpDefID);
			pDef->Release();
			pDef = NULL;
			if(memcmp(&interpDefID, &LinearInterpolator, sizeof(interpDefID)) == 0)
			{
				interpKind = kLinearInterp;
			}
			else if(memcmp(&interpDefID, &ConstantInterpolator, sizeof(interpDefID)) == 0)
			{
				interpKind = kConstInterp;
			}
			// else error!!!
				
			omStat = omfiVaryValueNew(OMFFileHdl, pOMFDatakind, effectLen,
				interpKind, &vval);
			omfSeg = vval;
				
			rc = pVaryingValue->GetControlPoints(&pointEnum);
			while(pointEnum->NextOne(&pPoint) == AAFRESULT_SUCCESS)
			{
				HRESULT	testRC;
				rc = pPoint->GetTime(&aafTime);
				omfTime.numerator = aafTime.numerator;
				omfTime.denominator = aafTime.denominator;
				testRC = pPoint->GetEditHint(&aafEditHint);
				if(testRC == AAFRESULT_PROP_NOT_PRESENT)
					aafEditHint = kAAFNoEditHint;
				else
					rc = testRC;
				switch(aafEditHint)
				{
				case kAAFNoEditHint:
					omfEditHint = kNoEditHint;
					break;
				case kAAFProportional:
					omfEditHint = kProportional;
					break;
				case kAAFRelativeLeft:
					omfEditHint = kRelativeLeft;
					break;
				case kAAFRelativeRight:
					omfEditHint = kRelativeRight;
					break;
				case kAAFRelativeFixed:
					omfEditHint = kRelativeFixed;
					break;
					//!!!Handle default case
				}
				
				rc = pPoint->GetValueBufLen(&srcValueLen);
				srcValue = new unsigned char[srcValueLen];
				rc = pPoint->GetValue(srcValueLen, srcValue, &lenRead);
				ConvertValueBuf(typeDefID, srcValue, lenRead, &destValue, &destValueLen, &didNew);
				omStat = omfiVaryValueAddPoint(OMFFileHdl, vval, omfTime, omfEditHint, pOMFDatakind,
					destValueLen, destValue);
				if(didNew)
					delete [] destValue;
				delete [] srcValue;
				destValue = NULL;
				srcValue = NULL;
			}
			omStat = omfiEffectAddNewSlot(OMFFileHdl,pOMFEffect,
				slotNum, omfSeg, &pOMFEffectSlot);
		}
	}
	if(pConstantValue != NULL)
	{
		pConstantValue->Release();
		pConstantValue = NULL;
	}
	if(pInterpDef != NULL)
	{
		pInterpDef->Release();
		pInterpDef = NULL;
	}
	if(pPoint != NULL)
	{
		pPoint->Release();
		pPoint = NULL;
	}
	if((srcValue != NULL) && (srcValue != destValue))
	{
		delete [] srcValue;
		srcValue = NULL;
	}
	if(destValue != NULL)
	{
		delete [] destValue;
		destValue = NULL;
	}
}


omfObject_t Aaf2Omf::LocateSlot(omfEffObj_t pOMFEffect, aafInt32 kfSlotNum)
{
	omfNumSlots_t	numSlots, slotID;
	omfIterHdl_t	slotIter;
	omfESlotObj_t	effectSlot;
	omfObject_t	segment, result = NULL;
	OMFCheck	OMFError;

	OMFError = omfiEffectGetNumSlots(OMFFileHdl, pOMFEffect, &numSlots);
	OMFError = omfiIteratorAlloc(OMFFileHdl, &slotIter);
	while(omfiEffectGetNextSlot(slotIter, pOMFEffect, NULL, &effectSlot) == OM_ERR_NONE)
	{
		OMFError = omfiEffectSlotGetInfo(OMFFileHdl, effectSlot, &slotID, &segment);
		if(slotID == kfSlotNum)
		{
			result = segment;
		}
	}
	OMFError = omfiIteratorDispose(OMFFileHdl, slotIter);

	return(result);
}

void Aaf2Omf::ConvertValueBuf(aafUID_t &typeDefID,
								aafDataBuffer_t srcValue, aafUInt32 srcValueLen,
								 aafDataBuffer_t *destValue, aafUInt32 *destValueLen,
								 bool *didAllocateNew)
{
	*didAllocateNew = false;
	if(memcmp(&typeDefID, &kAAFTypeID_Rational, sizeof(typeDefID)) == 0)
	{
		*destValue = srcValue;
		*destValueLen = srcValueLen;
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_Int32, sizeof(typeDefID)) == 0)
	{
		*destValue = srcValue;
		*destValueLen = srcValueLen;
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_Boolean, sizeof(typeDefID)) == 0)
	{
		*destValue = srcValue;
		*destValueLen = srcValueLen;
	}
	else // Error!!!
	{
		*destValue = srcValue;
		*destValueLen = srcValueLen;
	}
}
// OTher idea: Upon hitting ANY of the Avid private params (or level) find a VVAL
// and assume that all VVALs containing AvidPrivate have identical times
void Aaf2Omf::ConvertObjectProps(IAAFObject* /*pObj*/, omfObject_t	/*pOMFObject*/)
{
}
