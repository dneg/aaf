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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

// OMF Includes


#include "AafOmf.h"

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
#include "AAFDomainUtils.h"
#include "aafCodecdefs.h"
#include "aafclassdefuids.h"
//#include "omcAvJPED.h"

static void     LogError(HRESULT errcode, int line, char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define checkAAF(a)  \
{ moduleErrorTmp = a; \
	if (!SUCCEEDED(moduleErrorTmp)) \
	{ \
	    LogError(moduleErrorTmp, __LINE__, __FILE__);\
		goto cleanup; \
	} \
}

#define checkOMF(a)  \
{ moduleErrorTmp = a; \
	if (moduleErrorTmp != OMF2::OM_ERR_NONE) \
	{ \
	    LogError(moduleErrorTmp, __LINE__, __FILE__);\
		goto cleanup; \
	} \
}
#define checkFatal(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     exit(1);\
}

extern AafOmfGlobals* gpGlobals;
// ============================================================================
// Constructor
// ============================================================================
Aaf2Omf::Aaf2Omf() : pFile(NULL), pHeader(NULL), pDictionary(NULL)
{
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
	pwFileName = new wchar_t[strlen(gpGlobals->sInFileName)+1];
	mbstowcs(pwFileName, gpGlobals->sInFileName, strlen(gpGlobals->sInFileName)+1);


	rc = AAFFileOpenExistingRead(pwFileName, 0, &pFile);
	if (FAILED(rc))
	{
		delete [] pwFileName;
		return rc;
	}

	rc = pFile->GetHeader(&pHeader);
	if (FAILED(rc))
	{
		delete [] pwFileName;
		return rc;
	}

	rc = pHeader->GetDictionary(&pDictionary);
	if (FAILED(rc))
	{
		delete [] pwFileName;
		return rc;
	}
	if (gpGlobals->bVerboseMode)
	{
		printf("AAF File: %s opened succesfully\n", gpGlobals->sInFileName);
//		printf("          File Revision %s \n", szFileVersion);
	}

	pAAF->RegisterAAFProperties(pDictionary);

	gpGlobals->bAAFFileOpen = AAFTrue;
	delete [] pwFileName;

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
	OMF2::omfErr_t						OMFError = OMF2::OM_ERR_NONE;
	aafBool								bSessionStarted = AAFFalse;

	OMF2::omfProductIdentification_t	OMFProductInfo;
	char*								pszCompanyName = NULL;
	char*								pszProductName = NULL;
    char*								pszProductVersionString = NULL;
    char*								pszPlatform;
	char*								src;
	
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
		strcpy(gpGlobals->sOutFileName, gpGlobals->sInFileName);
		pExt= strrchr(gpGlobals->sOutFileName, '.');
		strcpy(pExt,".omf");
	}
	
	if (gpGlobals->bDeleteOutput)
	{
		rc = deleteFile(gpGlobals->sOutFileName);
		if (rc == AAFRESULT_SUCCESS)
			printf("Output file: %s will be overwritten\n", gpGlobals->sOutFileName);
		else
			printf("Output file: %s will be created\n", gpGlobals->sOutFileName);
	}
	// Retrieve AAF file's last identification
	rc = pHeader->GetLastIdentification(&pIdent);
	pIdent->GetCompanyNameBufLen(&textSize);
	if (textSize > 0)
	{
		pwCompanyName = (wchar_t *)new wchar_t[textSize];
		pIdent->GetCompanyName(pwCompanyName, textSize);
		pszCompanyName = (char *)new char[textSize/sizeof(wchar_t)];
		wcstombs(pszCompanyName, pwCompanyName, textSize/sizeof(wchar_t));
	}
	else
	{
		src = "<Not Specified>";
		pszCompanyName = (char *)new char[strlen(src)+1];
		strcpy(pszCompanyName, src);
	}

	pIdent->GetProductNameBufLen(&textSize);
	if (textSize > 0)
	{
		pwProductName = (wchar_t *)new wchar_t[textSize];
		pIdent->GetProductName(pwProductName, textSize);
		pszProductName = (char *)new char[textSize/sizeof(wchar_t)];
		wcstombs(pszProductName, pwProductName, textSize/sizeof(wchar_t));
	}
	else
	{
		src = "<Not Specified>";
		pszProductName = (char *)new char[strlen(src)+1];
		strcpy(pszProductName, src);
	}

	pIdent->GetProductVersionStringBufLen(&textSize);
	if (textSize > 0)
	{
		pwProductVersionString = (wchar_t *)new wchar_t[textSize];
		pIdent->GetProductVersionString(pwProductVersionString, textSize);
		pszProductVersionString = (char *)new char[textSize/sizeof(wchar_t)];
		wcstombs(pszProductVersionString, pwProductVersionString, textSize/sizeof(wchar_t));
	}
	else
	{
		src = "<Not Specified>";
		pszProductVersionString = (char *)new char[strlen(src)+1];
		strcpy(pszProductVersionString, src);
	}

	pIdent->GetPlatformBufLen(&textSize);
	if (textSize > 0)
	{
		pwPlatform = (wchar_t *)new wchar_t[textSize/sizeof(wchar_t)];
		pIdent->GetPlatform(pwPlatform, textSize);
		pszPlatform = (char *)new char[textSize/sizeof(wchar_t)];
		wcstombs(pszPlatform, pwPlatform, textSize/sizeof(wchar_t));
	}
	else
	{
		src = "<Not Specified>";
		pszPlatform = (char *)new char[strlen(src)+1];
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
	OMFProductInfo.productVersion.type = (OMF2::omfProductReleaseType_t)productVersion.type;

	OMFError = OMF2::omfsBeginSession(&OMFProductInfo, &OMFSession);
	if (OMFError != OMF2::OM_ERR_NONE )
	{
		rc = AAFRESULT_BADOPEN;
		goto cleanup;
	}

	RegisterCodecProperties(gpGlobals, OMFSession);
	pOMF->RegisterOMFProperties(gpGlobals, OMFSession);
	
	bSessionStarted = AAFTrue;
	OMFError = OMF2::omfmInit(OMFSession);
	if (OMFError != OMF2::OM_ERR_NONE )
	{
		OMF2::omfsEndSession(OMFSession);
		rc = AAFRESULT_BAD_SESSION;
		goto cleanup;
	}


//	OMFError = omfmRegisterCodec(OMFSession, OMF2::omfCodecAvJPED, OMF2::kOMFRegisterLinked);

	OMFError = OMF2::omfsCreateFile((OMF2::fileHandleType)gpGlobals->sOutFileName, OMFSession, OMF2::kOmfRev2x, &OMFFileHdl);
	if (OMFError != OMF2::OM_ERR_NONE )
	{
		rc = AAFRESULT_BADOPEN;
		goto cleanup;
	}
	

	gpGlobals->bOMFFileOpen = AAFTrue;
	// Clean up and exit 
cleanup:
	if (pIdent)
		pIdent->Release();

	if (pwCompanyName)
		delete [] pwCompanyName;

	if (pszCompanyName)
		delete [] pszCompanyName;

	if (pwProductName)
		delete [] pwProductName;

	if (pszProductName)
		delete [] pszProductName;

	if (pwPlatform)
		delete [] pwPlatform;

	if (pszPlatform)
		delete [] pszPlatform;

	if (pwProductVersionString)
		delete [] pwProductVersionString;
	
	if (pszProductVersionString)
		delete [] pszProductVersionString;

	if (gpGlobals->bVerboseMode && SUCCEEDED(rc))
	{
		printf("OMF file: %s created succesfully\n", gpGlobals->sOutFileName);
	}
	else
	{
		printf("File: %s could NOT be created\n", gpGlobals->sOutFileName);
	}


	return rc;
}

// ============================================================================
// CloseOutputFile
//			This function closes the output file.
//
// ============================================================================
void Aaf2Omf::CloseOutputFile()
{
	OMF2::omfErr_t	OMFError = OMF2::OM_ERR_NONE;
	OMFError = OMF2::omfsCloseFile(OMFFileHdl);
	OMFError = OMF2::omfsEndSession(OMFSession);
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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;

	OMF2::omfMobObj_t		OMFMob = NULL;

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

	rc = pHeader->GetNumMobs(kAllMob, &nAAFNumMobs);
	if (gpGlobals->bVerboseMode)
	{
		printf(" Found: %ld Mobs in the input file\n", nAAFNumMobs);
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
		pwMobName = new wchar_t[nameLength/sizeof(wchar_t)];
		rc = pMob->GetName(pwMobName, nameLength);
		rc = pMob->GetMobID(&MobID);
		pszMobName = new char[nameLength/sizeof(wchar_t)];
		wcstombs(pszMobName, pwMobName, nameLength/sizeof(wchar_t));
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
					printf("Unrecognized Mob kind encountered ID: %s\n", szMobID);
//					fprintf(stderr,"Unrecognized Mob kind encountered ID: %s\n", szMobID);
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
					IEnumAAFTaggedValues*	pCommentIterator = NULL;
					IAAFTaggedValue*		pMobComment = NULL;
					rc = pMob->EnumAAFAllMobComments(&pCommentIterator);
					while ( (SUCCEEDED(rc)) && (SUCCEEDED(pCommentIterator->NextOne(&pMobComment))))
					{
						char*		pszComment;
						char*		pszCommName;
						aafWChar*	pwcComment;
						aafWChar*	pwcName;
						aafInt32	textSize;
						aafUInt32	bytesRead;

						pMobComment->GetNameBufLen(&textSize);
						pwcName = new aafWChar [textSize/sizeof(aafWChar)];
						pMobComment->GetName(pwcName, textSize);
						pszCommName =  new char[textSize/sizeof(aafWChar)];
						wcstombs(pszCommName, pwcName, textSize/sizeof(aafWChar));

						pMobComment->GetValueBufLen((aafUInt32 *)&textSize);
						pwcComment = new aafWChar[textSize/sizeof(aafWChar)];
						pMobComment->GetValue((aafUInt32)textSize, (aafDataBuffer_t)pwcComment, &bytesRead);
						pszComment =  new char[textSize/sizeof(aafWChar)];
						wcstombs(pszComment, pwcComment, textSize/sizeof(aafWChar));

						OMFError = OMF2::omfiMobAppendComment(OMFFileHdl, OMFMob, pszCommName, pszComment);
						delete [] pszCommName;
						pszCommName = NULL;
						delete [] pszComment;
						pszComment = NULL;
						delete [] pwcName;
						pwcName = NULL;
						delete [] pwcComment;
						pwcComment = NULL;
						pMobComment->Release();
						pMobComment = NULL;
					}
					pCommentIterator->Release();
				}
			}
		}
		delete [] pwMobName;
		delete [] pszMobName;
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
			printf(" Found: %ld Essence data objects\n", numEssenceData);
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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;

	OMF2::omfUID_t			OMFMobID;
	OMF2::omfDefaultFade_t	OMFFade;
	IAAFMob					*pMob = NULL;
	aafDefaultFade_t		DefaultFade;

	OMFError = OMF2::omfiCompMobNew(OMFFileHdl, pMobName, (OMF2::omfBool)AAFFalse, pOMFCompMob);
	if (OMF2::OM_ERR_NONE == OMFError)
	{
		ConvertAUIDtoUID(pMobID, &OMFMobID);
		OMFError = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFCompMob, OMFMobID);
		if (OMF2::OM_ERR_NONE == OMFError)
		{
			rc = pCompMob->GetDefaultFade(&DefaultFade);
			if ((rc == AAFRESULT_SUCCESS) && DefaultFade.valid)
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

				OMFError = OMF2::omfiMobSetDefaultFade(OMFFileHdl,
												 *pOMFCompMob,
												 OMFFade.fadeLength, 
												 OMFFade.fadeType ,
												 OMFFade.fadeEditUnit);

			}

			rc = pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			FinishUpMob(pMob, *pOMFCompMob);
		}
	}

	if (OMFError != OMF2::OM_ERR_NONE)
		rc = AAFRESULT_INTERNAL_ERROR;

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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;
	IAAFMob					*pMob = NULL;

	OMF2::omfUID_t		OMFMobID;

	OMFError = OMF2::omfmMasterMobNew(OMFFileHdl, pMobName, (OMF2::omfBool)AAFTrue, pOMFMasterMob);
	if (OMF2::OM_ERR_NONE == OMFError)
	{
		ConvertAUIDtoUID(pMobID, &OMFMobID);
		OMFError = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFMasterMob, OMFMobID);
		if (OMF2::OM_ERR_NONE == OMFError)
		{
			if (gpGlobals->bVerboseMode)
				printf("Converted AAF Master MOB to OMF\n");
		}
	}
			
	rc = pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
	FinishUpMob(pMob, *pOMFMasterMob);

	if (OMFError != OMF2::OM_ERR_NONE)
		rc = AAFRESULT_INTERNAL_ERROR;

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
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;

	OMF2::omfUID_t			OMFMobID;


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

	rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
	FinishUpMob(pMob, *pOMFSourceMob);

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

		OMFError = OMF2::omfmTapeMobNew(OMFFileHdl, pMobName, pOMFSourceMob);
		if (OMF2::OM_ERR_NONE == OMFError)
		{
			OMFError = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			pTapeDesc->GetTapeFormFactor(&formFactor);
			pTapeDesc->GetSignalType(&videoSignal);
			pTapeDesc->GetTapeFormat(&tapeFormat);
			pTapeDesc->GetTapeLength(&tapeLength);
			pTapeDesc->GetTapeManBufLen(&textSize);
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

			OMFError = OMF2::omfmTapeMobSetDescriptor(OMFFileHdl,
												*pOMFSourceMob,
												(OMF2::omfTapeCaseType_t *)&formFactor,
												(OMF2::omfVideoSignalType_t *)&videoSignal,
												(OMF2::omfTapeFormatType_t *)&tapeFormat,
												(OMF2::omfLength_t *)&tapeLength,
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
		goto cleanup;
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
			pSummary = new aafUInt8[summarySize];
			rc = pTiffDesc->GetSummary(summarySize, pSummary);
			rc = pTiffDesc->GetTrailingLines(&trailingLines);
			rc = pTiffDesc->GetLeadingLines(&leadingLines);
			rc = pTiffDesc->GetIsUniform(&IsUniform);
			rc = pTiffDesc->GetIsContiguous(&IsContiguous);
			rc = pTiffDesc->GetJPEGTableID(&JPEGTableID);

			// Create a new OMF TIFF File Descriptor
			OMFError = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_TIFF_VIDEO, pOMFSourceMob);
			OMFError = OMF2::omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMFError = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			OMFError = OMF2::omfsWriteLength(OMFFileHdl, mediaDescriptor, OMF2::OMMDFLLength, (OMF2::omfLength_t)length); 
			if (OMFError)
			{
				char* pErrString = OMF2::omfsGetErrorString(OMFError);
				fprintf(stderr,"%sAn error occurred while adding TIFF Media descritptor, ERROR = %s\n",gpGlobals->indentLeader, pErrString);
				goto cleanup;
			}
			if (gpGlobals->bVerboseMode)
				printf("%sAdded a Tiff Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);

			// Set OMF TIFF File Descriptor properties
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&rc);
			OMFError = OMF2::omfsWriteBoolean( OMFFileHdl,
										mediaDescriptor,
										OMF2::OMTIFDIsContiguous, 
										(OMF2::omfBool)IsContiguous);
			OMFError = OMF2::omfsWriteBoolean( OMFFileHdl,
										mediaDescriptor,
										OMF2::OMTIFDIsUniform,
										(OMF2::omfBool)IsUniform);
			OMFError = OMF2::omfsWriteDataValue(OMFFileHdl,
										 mediaDescriptor,
										 OMF2::OMTIFDSummary,
										 datakind,
										 (OMF2::omfDataValue_t)pSummary,
										 (OMF2::omfPosition_t)0,
										 summarySize);
			OMFError = OMF2::omfsWriteJPEGTableIDType( OMFFileHdl,
								 				 mediaDescriptor,
												 OMF2::OMTIFDJPEGTableID, 
												 (OMF2::omfJPEGTableID_t)JPEGTableID);
			OMFError = OMF2::omfsWriteInt32(OMFFileHdl,
									 mediaDescriptor,
									 OMF2::OMTIFDLeadingLines, 
									 leadingLines);
			OMFError = OMF2::omfsWriteInt32(OMFFileHdl,
									 mediaDescriptor,
									 OMF2::OMTIFDTrailingLines, 
									 trailingLines);
			if (OMFError)
			{
				char* pErrString = OMF2::omfsGetErrorString(OMFError);
				fprintf(stderr,"%sAn error occurred while adding TIFF Media descritptor, ERROR = %s\n",gpGlobals->indentLeader, pErrString);
				goto cleanup;
			}
			delete [] pSummary;
			goto cleanup;
		}
		rc = pEssenceDesc->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&pWAVEDesc);
		if (SUCCEEDED(rc))
		{
			// It is a WAVE file descriptor
			OMF2::omfDDefObj_t	datakind;
			OMF2::omfObject_t	mediaDescriptor;
			aafDataValue_t		pSummary;
			aafUInt32			summarySize = 0;

			// retrieve WAVE descriptor properties
			rc = pWAVEDesc->GetSummaryBufferSize(&summarySize);
			pSummary = new aafUInt8[summarySize];
			rc = pWAVEDesc->GetSummary(summarySize, pSummary);

			//Create a new WAVE File Descriptor
			OMFError = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_WAVE_AUDIO, pOMFSourceMob);
			OMFError = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			OMFError = OMF2::omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMFError = OMF2::omfsWriteLength(OMFFileHdl, mediaDescriptor, OMF2::OMMDFLLength, (OMF2::omfLength_t)length); 
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (OMF2::omfErr_t *)&rc);
			OMFError = OMF2::omfsWriteDataValue(OMFFileHdl,
										 mediaDescriptor,
										 OMF2::OMWAVDSummary,
										 datakind,
										 (OMF2::omfDataValue_t)pSummary,
										 (OMF2::omfPosition_t)0,
										 summarySize);
			if (gpGlobals->bVerboseMode)
				printf("%sAdded a Wave Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			goto cleanup;
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
			pSummary = new aafUInt8[summarySize];
			rc = pAifcDesc->GetSummary(summarySize, pSummary);
			
			OMFError = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_AIFC_AUDIO, pOMFSourceMob);
			OMFError = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			OMFError = OMF2::omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMFError = OMF2::omfsWriteLength(OMFFileHdl, mediaDescriptor, OMF2::OMMDFLLength, (OMF2::omfLength_t)length); 
			if (gpGlobals->bVerboseMode)
				printf("%sAdded an AIFC Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			delete [] pSummary;
			goto cleanup;
		}
		rc = pEssenceDesc->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&pCDCIDesc);
		if (SUCCEEDED(rc))
		{
			aafUInt32				storedWidth, storedHeight, lineMapSize, n;
			aafUInt32				dispWidth, dispHeight, sampWidth, sampHeight;
			aafInt32				sampX, sampY, dispX, dispY, align;
			aafAlphaTransparency_t	alphaTrans;
			aafFrameLayout_t		layout;
			OMF2::omfObject_t		mediaDescriptor;
			aafRational_t			gamma, aspect;
			OMF2::omfRational_t		OMFGamma, OMFAspect;
			aafUID_t				compressionID;
			char					*omfclassID;
			char					*omfCompression;

			// It is a CDCI file descriptor
			rc = pEssenceDesc->QueryInterface(IID_IAAFDigitalImageDescriptor, (void **)&pDIDDDesc);//!!!
			(void)pDIDDDesc->GetCompression(&compressionID);
			if(memcmp(&compressionID, &CodecJPEG, sizeof(CodecJPEG)) == 0)
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

			OMFError = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, omfclassID, pOMFSourceMob);
			OMFError = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
			OMFError = OMF2::omfmMobGetMediaDescription(OMFFileHdl, *pOMFSourceMob, &mediaDescriptor);
			OMFError = OMF2::omfsWriteLength(OMFFileHdl, mediaDescriptor, OMF2::OMMDFLLength, (OMF2::omfLength_t)length); 
			
			// Get Digital Image properties and set them
			// Toolkit still expects signed
			pDIDDDesc->GetStoredView(&storedWidth, &storedHeight);
			OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDStoredHeight, storedHeight);
			OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDStoredWidth, storedWidth);

			if(pDIDDDesc->GetSampledView(&sampHeight, &sampWidth, &sampX, &sampY) == AAFRESULT_SUCCESS)
			{
				OMFError = OMF2::omfsWriteUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledHeight, sampHeight);
				OMFError = OMF2::omfsWriteUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledWidth, sampWidth);
				OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledXOffset, sampX);
				OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDSampledYOffset, sampY);
			}

			if(pDIDDDesc->GetDisplayView(&dispHeight, &dispWidth, &dispX, &dispY) == AAFRESULT_SUCCESS)
			{
				OMFError = OMF2::omfsWriteUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayHeight, dispHeight);
				OMFError = OMF2::omfsWriteUInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayWidth, dispWidth);
				OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayXOffset, dispX);
				OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDDisplayYOffset, dispY);
			}
			(void)pDIDDDesc->GetFrameLayout(&layout);
			OMFError = OMF2::omfsWriteLayoutType(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDFrameLayout, (OMF2::omfFrameLayout_t)(layout+1));	// Toolkit used incorrect layout

			if(pDIDDDesc->GetGamma(&gamma) == AAFRESULT_SUCCESS)
			{
				OMFGamma.numerator = gamma.numerator;
				OMFGamma.denominator = gamma.denominator;
				OMFError = OMF2::omfsWriteRational(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDGamma, OMFGamma);
			}

			(void)pDIDDDesc->GetVideoLineMapSize(&lineMapSize);
			lineMap = new aafInt32[lineMapSize];
			(void)pDIDDDesc->GetVideoLineMap(lineMapSize, lineMap);
			// Possibly translate old line map #'s into new ones?!!!
			for(n = 0; n < lineMapSize; n++)
			{
				OMFError = OMF2::OMWriteProp(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDVideoLineMap, 
				n*sizeof(aafInt32), OMF2::OMInt32Array,
				sizeof(aafInt32), lineMap+n);
			}
			delete [] lineMap;
			lineMap = NULL;

			//
			OMFError = OMF2::omfsWriteString(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDCompression, omfCompression);

			//
			if(pDIDDDesc->GetImageAspectRatio(&aspect) == AAFRESULT_SUCCESS)
			{
				OMFAspect.numerator = aspect.numerator;
				OMFAspect.denominator = aspect.denominator;
				OMFError = OMF2::omfsWriteRational(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDImageAspectRatio, OMFAspect);
			}

			//
			if(pDIDDDesc->GetAlphaTransparency(&alphaTrans) == AAFRESULT_SUCCESS)
			{
				OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDAlphaTransparency, alphaTrans);
			}
			if(pDIDDDesc->GetImageAlignmentFactor(&align) == AAFRESULT_SUCCESS)
			{
				OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, OMF2::OMDIDDFieldAlignment, align);
			}
			
			// retrieve CDCI descriptor properties
			aafInt32			componentWidth;
			aafUInt32			horizSubsample;
			aafColorSiting_t	cositing;

			// Find out which are optional & which must fail!!!
			(void)pCDCIDesc->GetComponentWidth(&componentWidth);
			OMFError = OMF2::omfsWriteInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIComponentWidth, componentWidth); 

			(void)pCDCIDesc->GetHorizontalSubsampling(&horizSubsample);
			OMFError = OMF2::omfsWriteUInt32(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIHorizontalSubsampling, horizSubsample); 

			(void)pCDCIDesc->GetColorSiting(&cositing);
			OMFError = OMF2::OMWriteProp(OMFFileHdl, mediaDescriptor, gpGlobals->omCDCIColorSiting, 
						  0, OMF2::OMColorSitingType,
						  sizeof(cositing), (void *)&(cositing));

			if (gpGlobals->bVerboseMode)
				printf("%sAdded a CDCI Media Descriptor to a Source MOB\n", gpGlobals->indentLeader);
			goto cleanup;
		}
		rc = pEssenceDesc->QueryInterface(IID_IAAFObject, (void **)&pAAFObject);
		if (SUCCEEDED(rc))
		{
			// Media descriptor not implemented or not recognized
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
				printf("%sInvalid essence descripor type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
			fprintf(stderr,"%sInvalid essence descriptor type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
		}
		goto cleanup;
	}

cleanup:

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
	if (pDIDDDesc)
		pDIDDDesc->Release();
	if(lineMap != NULL)
		delete [] lineMap;

	DecIndentLevel();
	if (OMFError != OMF2::OM_ERR_NONE)
		rc = AAFRESULT_INTERNAL_ERROR;
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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;
	
	OMF2::omfMSlotObj_t		OMFNewSlot = NULL;
	OMF2::omfSegObj_t		OMFSegment = NULL;
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
//	aafSearchCrit_t			criteria;
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
		printf("%sFound: %ld sub slots\n", gpGlobals->indentLeader, numSlots);
	}

	for (aafUInt32 times = 0; times< numSlots; times++)
	{
		rc = pSlotIter->NextOne(&pSlot);
		if (AAFRESULT_SUCCESS == rc)
		{
			//	Process Segment data
			pSlot->GetNameBufLen(&textSize);
			pwTrackName = new wchar_t[textSize/sizeof(wchar_t)];
			pSlot->GetName(pwTrackName, textSize);
			pwTrackName = new wchar_t[textSize/sizeof(wchar_t)];
			wcstombs(pszTrackName, pwTrackName, textSize/sizeof(wchar_t));
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
			rc = ProcessComponent(pComponent, &OMFSegment);

			OMFError = OMF2::omfiMobAppendNewTrack(OMFFileHdl,
											 *pOMFMob,
											 OMFeditRate,
											 OMFSegment,
											 OMFOrigin,
											 OMFTrackID,
											 pszTrackName, 
											 &OMFNewSlot);
			if (OMFError == OMF2::OM_ERR_NONE )
			{
				if (gpGlobals->bVerboseMode)
				{
					printf("%sConverted SlotID: %d, Name: %s\n",gpGlobals->indentLeader, (int)OMFTrackID, pszTrackName);
				}
			}
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

	if (OMFError != OMF2::OM_ERR_NONE)
		rc = AAFRESULT_INTERNAL_ERROR;

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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;

	OMF2::omfDDefObj_t		OMFDatakind;

	IAAFSequence*			pSequence = NULL;
	IAAFSourceClip*			pSourceClip = NULL;
	IAAFTimecode*			pTimecode = NULL;
	IAAFEdgecode*			pEdgecode = NULL;
	IAAFFiller*				pFiller = NULL;
	IAAFTransition*			pTransition = NULL;
	IAAFSelector*			pSelector = NULL;
	IAAFOperationGroup*		pEffect = NULL;
	IAAFScopeReference*		pScopeRef = NULL;

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
		OMFError = OMF2::omfiSequenceNew(OMFFileHdl, OMFDatakind, pOMFSegment);
		if (gpGlobals->bVerboseMode)
			printf("%sProcessing Sequence\n", gpGlobals->indentLeader);
		TraverseSequence(pSequence, pOMFSegment);
		goto cleanup;
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
			printf("%sProcessing Source Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
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
		OMFError = OMF2::omfiSourceClipNew(OMFFileHdl,
									 OMFDatakind,
									 (OMF2::omfLength_t)length,
									 OMFSourceRef,
									 pOMFSegment);
		if (OMF2::OM_ERR_NONE == OMFError)
		{
			if (fadeInPresent || fadeOutPresent)
			  {
				// Some 'magic' required to get types to match
				OMF2::omfInt32 fadeInLen32 = (OMF2::omfInt32) fadeInLen;
				OMF2::omfInt32 fadeOutLen32 = (OMF2::omfInt32) fadeOutLen;
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
				OMFError = OMF2::omfiSourceClipSetFade(OMFFileHdl, 
													   *pOMFSegment,
													   fadeInLen32,
													   (OMF2::omfFadeType_t)fadeInType,
													   fadeOutLen32,
													   (OMF2::omfFadeType_t)fadeOutType);
			  }
		}
		else
			rc = AAFRESULT_INTERNAL_ERROR;

		goto cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFTimecode, (void **)&pTimecode);
	if (SUCCEEDED(rc))
	{
		// component is a Timecode
		OMF2::omfTimecode_t	timecode;
		aafTimecode_t		AAFTimecode;

		pTimecode->GetTimecode(&AAFTimecode);

		// Some 'magic' required to get types to match; make sure
		// narrowing of type didn't throw away data
		timecode.startFrame = (OMF2::omfFrameOffset_t) AAFTimecode.startFrame;
		if ((aafFrameOffset_t) timecode.startFrame != AAFTimecode.startFrame)
		{
		  rc = AAFRESULT_INTERNAL_ERROR;
		  goto cleanup;
		}

		timecode.drop = (OMF2::omfDropType_t)AAFTimecode.drop;
		timecode.fps = AAFTimecode.fps;
		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Timecode Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
			IncIndentLevel();
			printf("%sstart Frame\t: %ld\n", gpGlobals->indentLeader, timecode.startFrame);
			if (timecode.drop == AAFTrue)
				printf("%sdrop\t\t: True\n", gpGlobals->indentLeader);
			else
				printf("%sdrop\t\t: False\n", gpGlobals->indentLeader);
			printf("%sFrames/second\t: %ld\n", gpGlobals->indentLeader, timecode.fps);     
			DecIndentLevel();				
		}

		OMFError = OMF2::omfiTimecodeNew(OMFFileHdl, (OMF2::omfLength_t)length, timecode, pOMFSegment);
		goto cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFEdgecode, (void **)&pEdgecode);
	if (SUCCEEDED(rc))
	{
		// component is an Edgecode
		aafEdgecode_t		edgecode;
		OMF2::omfEdgecode_t	OMFEdgecode;

		pEdgecode->GetEdgecode(&edgecode);

		// Some 'magic' required to get types to match; make sure
		// narrowing of type didn't throw away data
		OMFEdgecode.startFrame = (OMF2::omfFrameOffset_t) edgecode.startFrame;
		if ((aafFrameOffset_t) OMFEdgecode.startFrame != edgecode.startFrame)
		{
		  rc = AAFRESULT_INTERNAL_ERROR;
		  goto cleanup;
		}

		OMFEdgecode.filmKind = (OMF2::omfFilmType_t)edgecode.filmKind;
		OMFEdgecode.codeFormat = (OMF2::omfEdgeType_t)edgecode.codeFormat;
		memcpy(OMFEdgecode.header, edgecode.header, sizeof(edgecode.header));
		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Timecode Clip of length: %ld\n ", gpGlobals->indentLeader, (int)length);
			IncIndentLevel();
			printf("%sstart Frame\t: %ld\n", gpGlobals->indentLeader, edgecode.startFrame);
			DecIndentLevel();				
		}
		OMFError = OMF2::omfiEdgecodeNew(OMFFileHdl, (OMF2::omfLength_t)length, OMFEdgecode, pOMFSegment);		
		goto cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFFiller, (void **)&pFiller);
	if (SUCCEEDED(rc))
	{
		// component is a filler
		OMFError = OMF2::omfiFillerNew(OMFFileHdl, OMFDatakind, (OMF2::omfLength_t)length, pOMFSegment);
		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Filler of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		goto cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFOperationGroup, (void **)&pEffect);
	if (SUCCEEDED(rc))
	{
		//Component is an effect
		OMF2::omfEffObj_t	effect = NULL;

		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Effect of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		rc = ConvertEffects(pEffect, &effect);

		goto cleanup;
	}
	rc = pComponent->QueryInterface(IID_IAAFTransition, (void **)&pTransition);
	if (SUCCEEDED(rc))
	{
		// component is a Transition
		OMF2::omfEffObj_t	effect;

		aafPosition_t	cutPoint;

		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Transition of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		pTransition->GetCutPoint(&cutPoint);
		rc = pTransition->GetOperationGroup(&pEffect);
		// At this time (4/99) effects are not implemented therefore we 
		// will have to create an Effect from thin air.(hack it !!)
		rc = ConvertEffects(pEffect, &effect);

		OMFError = OMF2::omfiTransitionNew(OMFFileHdl,
									 OMFDatakind,
									 (OMF2::omfLength_t)length,
									 (OMF2::omfPosition_t)cutPoint,
									 effect,
									 pOMFSegment);
		goto cleanup;
	}

	rc = pComponent->QueryInterface(IID_IAAFSelector, (void **)&pSelector);
	if (SUCCEEDED(rc))
	{
		// component is a selector
		IncIndentLevel();
		
		OMFError = OMF2::omfiSelectorNew(OMFFileHdl,
								   OMFDatakind,
								   (OMF2::omfLength_t)length,
								   pOMFSegment);
		rc = ConvertSelector(pSelector, pOMFSegment);
		DecIndentLevel();
		goto cleanup;
	}
	
	rc = pComponent->QueryInterface(IID_IAAFScopeReference, (void **)&pScopeRef);
	if (SUCCEEDED(rc))
	{
		// component is a source clip
		aafUInt32		relativeScope, relativeSlot;

		if (gpGlobals->bVerboseMode)
		{
			printf("%sProcessing Scope Reference of length: %ld\n ", gpGlobals->indentLeader, (int)length);
		}
		// Get Source Clip properties
        checkAAF(pScopeRef->GetRelativeScope(&relativeScope));
        checkAAF(pScopeRef->GetRelativeSlot(&relativeSlot));
		checkOMF(OMF2::omfiScopeRefNew(OMFFileHdl,
									 OMFDatakind,
									 (OMF2::omfLength_t)length,
									 relativeScope,
									 relativeSlot,
									pOMFSegment));

		goto cleanup;
	}

	char szTempUID[64];
	AUIDtoString(&datadef ,szTempUID);
	if (gpGlobals->bVerboseMode)
		printf("%sInvalid component type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);
	fprintf(stderr,"%sInvalid component type found, datadef : %s\n", gpGlobals->indentLeader, szTempUID);

cleanup:

	if (pScopeRef)
		pScopeRef->Release();
	
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
	if (OMFError != OMF2::OM_ERR_NONE)
		rc = AAFRESULT_INTERNAL_ERROR;
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

	if ( memcmp((char *)&Datadef, (char *)&DDEF_Picture, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Picture");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&Datadef, (char *)&DDEF_Sound, sizeof(aafUID_t)) == 0 )
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
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectPictureWithMate, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:PictureWithMatte");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
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
	return rc;
}

// ============================================================================
// ConvertAAFTypeIDDatakind
//
//			This function converts an AAF type UID into an OMF datakind. 
//			
// Returns: AAFRESULT_SUCCESS if datakind is converted succesfully
//
// ============================================================================
HRESULT Aaf2Omf::ConvertAAFTypeIDDatakind(aafUID_t typeID, OMF2::omfDDefObj_t* pDatakind)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfUniqueName_t	datakindName;
	OMF2::omfBool			bFound;
	char					szAUID[64];

	if ( memcmp((char *)&typeID, (char *)&kAAFTypeID_Rational, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Rational");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&typeID, (char *)&kAAFTypeID_Int32, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Int32");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
	}
	else if ( memcmp((char *)&typeID, (char *)&kAAFTypeID_Boolean, sizeof(aafUID_t)) == 0 )
	{
		strcpy(datakindName, "omfi:data:Boolean");
		bFound = OMF2::omfiDatakindLookup(OMFFileHdl, datakindName, pDatakind, (OMF2::omfErr_t *) &rc);
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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;
	OMF2::omfCpntObj_t		newComponent = NULL;

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
				OMFError = OMF2::omfiSequenceAppendCpnt(OMFFileHdl, *pOMFSequence, newComponent);
			}

		}
		pComponent->Release();
		pComponent = NULL;
	}
	pCompIter->Release();
	pCompIter = NULL;
	if (OMF2::OM_ERR_NONE != OMFError)
		rc = AAFRESULT_INTERNAL_ERROR;

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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;
	OMF2::omfSegObj_t		OMFSelected = NULL;

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
	rc = ProcessComponent(pComponent, &OMFSelected );
	if (SUCCEEDED(rc))
	{
		OMFError = OMF2::omfiSelectorSetSelected(OMFFileHdl, *pOMFSelector, OMFSelected);
		if (numAlternates > 0)
		{
			OMF2::omfSegObj_t	OMFAltSelected = NULL;

			IAAFComponent*		pAltComponent = NULL;
			IEnumAAFSegments*	pEnumAlternates = NULL;
			IAAFSegment*		pAltSegment = NULL;
	
			rc = pSelector->EnumAlternateSegments(&pEnumAlternates);
			for (int i = 0; i< numAlternates;i++)
			{
				pEnumAlternates->NextOne(&pAltSegment);
				pAltSegment->QueryInterface(IID_IAAFComponent, (void **)&pAltComponent);
				rc = ProcessComponent(pAltComponent, &OMFAltSelected);
				if (SUCCEEDED(rc))
				{
					OMFError = OMF2::omfiSelectorAddAlt(OMFFileHdl, *pOMFSelector, OMFAltSelected);
				}
				pAltComponent->Release();
				pAltSegment->Release();
			}

			pEnumAlternates->Release();
		}
	}

	DecIndentLevel();
	if (pSegment)
		pSegment->Release();
	if (pComponent)
		pComponent->Release();

	if (OMF2::OM_ERR_NONE != OMFError)
		rc = AAFRESULT_INTERNAL_ERROR;
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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;
	// OMF2::omfClassID_t		locType;

	char*					pszLocatorPath = NULL;
	char*					pszName = NULL;

	IAAFLocator*			pLocator = NULL;
	IAAFTextLocator*		pTextLocator = NULL;
	IEnumAAFLocators*		pLocatorIter = NULL;		
	aafInt32				numLocators = 0;
    aafWChar*				pwLocatorPath = NULL;
	aafWChar*				pwName = NULL;

	aafInt32				pathSize = 0;
	aafUInt32				textSize = 0;

	rc = pEssenceDesc->GetNumLocators(&numLocators);
	if (SUCCEEDED(rc) && numLocators > 0)
	{
		rc = pEssenceDesc->EnumAAFAllLocators(&pLocatorIter);
		while (SUCCEEDED(rc) && SUCCEEDED(pLocatorIter->NextOne(&pLocator)))
		{
			rc = pLocator->QueryInterface(IID_IAAFTextLocator, (void **)&pTextLocator);
			if (SUCCEEDED(rc))
			{
				rc = pTextLocator->GetNameBufLen((aafInt32 *)&textSize);
				pwName = new wchar_t[textSize/sizeof(wchar_t)];
				rc = pTextLocator->GetName(pwName, textSize);
				if (SUCCEEDED(rc))
				{
					pszName = new char[textSize/sizeof(wchar_t)];
					wcstombs(pszName, pwName, textSize/sizeof(wchar_t));
					OMFError = OMF2::omfmMobAddTextLocator(OMFFileHdl, *pOMFSourceMob, pszName);
					if (pwName)
						delete [] pwName;
					if (pszName)
						delete [] pszName;
				}
			}
			else
			{
				pLocator->GetPathBufLen(&pathSize);
				pwLocatorPath = new wchar_t[pathSize/sizeof(wchar_t)];
				pLocator->GetPath(pwLocatorPath, pathSize);
				pszLocatorPath = new char[pathSize/sizeof(wchar_t)];
				wcstombs(pszLocatorPath, pwLocatorPath, pathSize/sizeof(wchar_t));
				OMFError = OMF2::omfmMobAddNetworkLocator(OMFFileHdl, *pOMFSourceMob, OMF2::kOmfiMedia, pszLocatorPath);
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

	if (OMFError != OMF2::OM_ERR_NONE)
		rc = AAFRESULT_INTERNAL_ERROR;

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
	OMF2::omfErr_t			OMFError = OMF2::OM_ERR_NONE;

	OMF2::omfObject_t		OMFSourceMob = NULL;
	OMF2::omfObject_t		OMFHeader = NULL;
	OMF2::omfObject_t		mediaData = NULL;
	OMF2::omfUID_t			mediaID;
	OMF2::omfProperty_t		idProperty;
	OMF2::omfDDefObj_t		datakind;
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
	IAAFMob*				pMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
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
	OMFError = OMF2::omfsFindSourceMobByID(OMFFileHdl, mediaID, &OMFSourceMob);
	if (OMFError != OMF2::OM_ERR_NONE)
	{
		rc = AAFRESULT_INVALID_OBJ;
		pSourceMob->Release();
		return rc;
	}
	OMFError = OMF2::omfsGetHeadObject( OMFFileHdl, &OMFHeader );

	// !!! Tomas, there is only one kind of essence data now, so this code will
	// need to find the EssenceDescriptor of the associated file mob (lookup he
	// dataID in the MOB index on the dictionary) and do a QI on the result
	// The code is something like:
	//
	rc = pEssenceDesc->QueryInterface(IID_IAAFTIFFDescriptor, (void **)&pTIFFDesc);
	if (SUCCEEDED(rc))
	{
		//Convert TIFF Essence data
		idProperty = OMF2::OMTIFFData;
		OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&rc);
		strcpy(id, "TIFF");
		OMFError = OMF2::omfsObjectNew(OMFFileHdl, id, &mediaData);
		if (OMFError != OMF2::OM_ERR_NONE)
			goto cleanup;
		OMFError = OMF2::omfsWriteUID(OMFFileHdl, mediaData, OMF2::OMMDATMobID, mediaID);
		if (OMFError != OMF2::OM_ERR_NONE)
		{
			char* pErrString = OMF2::omfsGetErrorString(OMFError);
			goto cleanup;
		}
		OMFError = OMF2::omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, mediaData);
		if (OMFError != OMF2::OM_ERR_NONE)
		{
			char* pErrString = OMF2::omfsGetErrorString(OMFError);
			goto cleanup;
		}
		else
			goto CopyMedia;
	}

	rc = pEssenceDesc->QueryInterface(IID_IAAFAIFCDescriptor, (void **)&pAIFCDesc);
	if (SUCCEEDED(rc))
	{
		//Convert AIFC Essence data
		idProperty = OMF2::OMAIFCData;
		OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (OMF2::omfErr_t *)&rc);
		strcpy(id, "AIFC");
		OMFError = OMF2::omfsObjectNew(OMFFileHdl, id, &mediaData);
		if (OMFError != OMF2::OM_ERR_NONE)
			goto cleanup;
		OMFError = OMF2::omfsWriteUID(OMFFileHdl, mediaData, OMF2::OMMDATMobID, mediaID);
		if (OMFError != OMF2::OM_ERR_NONE)
		{
			char* pErrString = OMF2::omfsGetErrorString(OMFError);
			goto cleanup;
		}
		OMFError = OMF2::omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, mediaData);
		if (OMFError != OMF2::OM_ERR_NONE)
			goto cleanup;
		else
			goto CopyMedia;
	}

	rc = pEssenceData->QueryInterface(IID_IAAFWAVEDescriptor, (void **)&pWAVEDesc);
	if (SUCCEEDED(rc))
	{
		//Convert WAVE Essence data
		idProperty = OMF2::OMWAVEData;
		OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (OMF2::omfErr_t *)&rc);
		strcpy(id, "WAVE");
		OMFError = OMF2::omfsObjectNew(OMFFileHdl, id, &mediaData);
		if (OMFError != OMF2::OM_ERR_NONE)
			goto cleanup;
		OMFError = OMF2::omfsWriteUID(OMFFileHdl, mediaData, OMF2::OMMDATMobID, mediaID);
		if (OMFError != OMF2::OM_ERR_NONE)
		{
			char* pErrString = OMF2::omfsGetErrorString(OMFError);
			goto cleanup;
		}
		OMFError = OMF2::omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, mediaData);
		if (OMFError != OMF2::OM_ERR_NONE)
			goto cleanup;
		else
			goto CopyMedia;
	}

	//!!!Need to check "compression" flag to determine if really JPEG
	rc = pEssenceData->QueryInterface(IID_IAAFCDCIDescriptor, (void **)&pJPEGDesc);
	if (SUCCEEDED(rc))
	{
		//Convert JPEG Essence data
		idProperty = OMF2::OMIDATImageData;
		OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&rc);
		strcpy(id, "JPEG");
		OMFError = OMF2::omfsObjectNew(OMFFileHdl, id, &mediaData);
		if (OMFError != OMF2::OM_ERR_NONE)
			goto cleanup;
		OMFError = OMF2::omfsWriteUID(OMFFileHdl, mediaData, OMF2::OMMDATMobID, mediaID);
		if (OMFError != OMF2::OM_ERR_NONE)
		{
			char* pErrString = OMF2::omfsGetErrorString(OMFError);
			goto cleanup;
		}
		OMFError = OMF2::omfsAppendObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, mediaData);
		if (OMFError != OMF2::OM_ERR_NONE)
			goto cleanup;
		else
			goto CopyMedia;
	}
	
	// Media type not supported or invalid
	rc = pEssenceData->QueryInterface(IID_IAAFObject, (void **)&pAAFObject);
	if (SUCCEEDED(rc))
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
				OMFError = OMF2::omfsWriteDataValue(OMFFileHdl, 
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
			delete [] pBuffer;
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

	if (OMFError != OMF2::OM_ERR_NONE)
		rc = AAFRESULT_INTERNAL_ERROR;

	return rc;

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
HRESULT Aaf2Omf::ConvertEffects(IAAFOperationGroup* pEffect,
								OMF2::omfEffObj_t*	pOMFEffect)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfDDefObj_t		effectDatakind;
	// OMF2::omfLength_t		effectLength;
	OMF2::omfEDefObj_t		effectDef;
	// OMF2::omfDDefObj_t		OMFdatakind;
	OMF2::omfUniqueName_t	effectID;
	OMF2::omfUniqueName_t	MCEffectID;
	OMF2::omfErr_t			OMFError;
	OMF2::omfBool			bDefExists;
	OMF2::omfObject_t		pOMFSegment, pOMFEffectSlot;

	IAAFOperationDef*		pEffectDef = NULL;
	IAAFParameterDef*		pParameterDef = NULL;
	IAAFParameter*			pParameter = NULL;
	IAAFDefObject*			pDefObject = NULL;
	IAAFSegment*			pSegment = NULL;
	IAAFSourceReference*	pSourceRef= NULL;
	IAAFFiller*				pFiller = NULL;
	IAAFComponent*			pComponent = NULL;
	IAAFSourceClip*			pSourceClip = NULL;

	aafUID_t				effectAUID;
	aafUID_t				effectDefAUID;
	aafUID_t				datadefAUID;
	aafLength_t				length;
	aafUInt32				byPass, bypassOverride, *byPassPtr;
	aafUInt32				textSize;
	aafBool					isATimeWarp;
	aafInt32				numSources;
	aafInt32				numParameters;
	aafWChar*				pwDesc = NULL;
	aafWChar*				pwName = NULL;

	char*					pszName = NULL;
	char*					pszDesc = NULL;
	char*					stdName = NULL;

	IncIndentLevel();

	rc = pEffect->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	if (SUCCEEDED(rc))
	{
		pComponent->GetLength(&length);
		pComponent->GetDataDef(&effectAUID);
		ConvertAAFDatadef(effectAUID, &effectDatakind);
		pEffect->IsATimeWarp(&isATimeWarp);
		pEffect->GetNumSourceSegments(&numSources);
		pEffect->GetNumParameters(&numParameters);
		rc = pEffect->GetOperationDefinition(&pEffectDef);
		pEffectDef->GetDataDefinitionID(&datadefAUID);
		checkAAF(pEffectDef->QueryInterface(IID_IAAFDefObject, (void **) &pDefObject));
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
		bDefExists = OMF2::omfiEffectDefLookup(OMFFileHdl, effectID, &effectDef, &OMFError);
		if (OMFError == OMF2::OM_ERR_NONE && !bDefExists)
		{
			rc = OMF2::omfiEffectDefNew(OMFFileHdl,
										effectID,
										pszName,
										pszDesc,
										(OMF2::omfArgIDType_t *)byPassPtr,
										(OMF2::omfBool)isATimeWarp,
										&effectDef);
		}

		checkOMF(OMF2::omfiEffectNew(OMFFileHdl,
								 effectDatakind,
								 (OMF2::omfLength_t)length,
								 effectDef,
								 pOMFEffect));

		
		if(MCEffectID[0] != '\0')
		{
			checkOMF(OMF2::omfsWriteString(OMFFileHdl, (*pOMFEffect), gpGlobals->pvtEffectIDProp,
											MCEffectID));
		}

        if(SUCCEEDED(pEffect->GetRender(&pSourceRef)))
		{
			rc = pSourceRef->QueryInterface(IID_IAAFSourceClip, (void **) &pSourceClip);
			if (SUCCEEDED(rc))
			{
				checkAAF(pSourceRef->QueryInterface(IID_IAAFComponent, (void **) &pComponent));
				checkAAF(ProcessComponent(pComponent,&pOMFSegment));
				checkOMF(OMF2::omfiEffectSetFinalRender(OMFFileHdl,(*pOMFEffect), pOMFSegment));
				pSourceClip->Release();
				pSourceClip = NULL;
				pComponent->Release();
				pComponent = NULL;
			}
			// !!!else error
		}
                
        if(SUCCEEDED(pEffectDef->GetBypass(&bypassOverride)))
		{
			checkOMF(OMF2::omfiEffectSetBypassOverride(OMFFileHdl,(*pOMFEffect),
							bypassOverride));	//!!! 1-1 mapping?
		}
        
		aafInt32	n, numSources;
		checkAAF(pEffect->GetNumSourceSegments(&numSources));
		for(n = 0; n < numSources; n++)
		{
			checkAAF(pEffect->GetIndexedInputSegment(n, &pSegment));
			checkAAF(pSourceRef->QueryInterface(IID_IAAFSourceClip,
												(void **) &pComponent));
			checkAAF(ProcessComponent(pComponent,&pOMFSegment));
			checkOMF(OMF2::omfiEffectAddNewSlot(OMFFileHdl,(*pOMFEffect),
									-1*(n+1), pOMFSegment, &pOMFEffectSlot));
			pSegment->Release();
			pComponent->Release();
		}
       
		// If the effect ID is known, map to a apecific OMF effect ID
		if(pEffect->GetParameterByArgID(kAAFParameterDefLevel, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect), -3,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParameterDefSMPTEWipeNumber, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  1,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParameterDefSMPTEReverse, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  2,
										(OMF2::omfLength_t)length));
		}
#if 0
		if(pEffect->GetParameterByArgID(kAAFParameterDefAmplitude, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParameterDefPan, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
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
		if(pEffect->GetParameterByArgID(kAAFParameterDefPhaseOffset, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParameterDefSpeedRatio, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidBorderWidth, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidBorderSoft, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidXPos, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidYPos, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidCrop, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, effectDefAUID, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidScale, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidSpillSupress, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidBounds, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidColor, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidUserParam, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
#endif
	}
cleanup:
	DecIndentLevel();
	if (pwName)
		delete [] pwName;
	if (pwDesc)
		delete [] pwDesc;
	if (pszName)
		delete [] pszName;
	if (pszDesc)
		delete [] pszDesc;

	if (pEffectDef)
		pEffectDef->Release();
	if (pParameterDef)
		pParameterDef->Release();
	if (pParameter)
		pParameter->Release();
	if (pDefObject)
		pDefObject->Release();
	if (pSegment)
		pSegment->Release();
	if (pSourceRef)
		pSourceRef->Release();
	if (pFiller)
		pFiller->Release();
	if (pComponent)
		pComponent->Release();
	if (pSourceClip)
		pSourceClip->Release();

	return rc;
}

HRESULT Aaf2Omf::ConvertParameter(	IAAFParameter*		pParm,
									aafUID_t			&effectDefID,
								  OMF2::omfSegObj_t		pOMFEffect,
									OMF2::omfInt32		slotNum,
									OMF2::omfLength_t	effectLen)
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
	AAFRESULT			rc;
	aafUInt32			srcValueLen, destValueLen, lenRead;
	aafDataBuffer_t		srcValue = NULL, destValue = NULL;
	OMF2::omfInterpKind_t interpKind;
	OMF2::omfObject_t	vval, kfVVAL = NULL;
	aafRational_t		aafTime;
	OMF2::omfRational_t	omfTime;
	aafEditHint_t		aafEditHint;
	OMF2::omfEditHint_t	omfEditHint;
	OMF2::omfSegObj_t	omfSeg;
	OMF2::omfObject_t	pOMFEffectSlot;
	OMF2::omfObject_t	pOMFDatakind;
	bool				didNew;

	moduleErrorTmp = AAFRESULT_SUCCESS;
    checkAAF(pParm->GetTypeDefinition(&pTypeDef));
	checkAAF(pTypeDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef));
    checkAAF(pDef->GetAUID(&typeDefID));
	pDef->Release();
	pDef = NULL;
	//
    checkAAF(pParm->GetParameterDefinition(&pParmDef));
	checkAAF(pParmDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef));
    checkAAF(pDef->GetAUID(&paramDefID));
	pParmDef->Release();
	pParmDef = NULL;
	pDef->Release();
	pDef = NULL;

	//!!! This one parameter ID has no OMF equivilent.  make a routine which
	// knows the valid ones to let through.
#if AVID_SPECIAL
	if(memcmp(&paramDefID, &kAAFParamID_AvidSelected, sizeof(paramDefID)) != 0)
#endif
	{
		checkAAF(ConvertAAFTypeIDDatakind(typeDefID, &pOMFDatakind));
		
		rc = pParm->QueryInterface(IID_IAAFConstantValue, (void **)&pConstantValue);
		if (SUCCEEDED(rc))
		{
			checkAAF(pConstantValue->GetValueBufLen(&srcValueLen));
			srcValue = new unsigned char[srcValueLen];
			checkAAF(pConstantValue->GetValue(srcValueLen, srcValue, &lenRead));
			ConvertValueBuf(typeDefID, srcValue, lenRead, &destValue, &destValueLen, &didNew);
			checkOMF(OMF2::omfiConstValueNew(OMFFileHdl, pOMFDatakind, effectLen,
				destValueLen, destValue, &omfSeg));
			if(didNew)
				delete [] destValue;
			delete [] srcValue;
			destValue = NULL;
			srcValue = NULL;
			checkOMF(OMF2::omfiEffectAddNewSlot(OMFFileHdl,pOMFEffect,
				slotNum, omfSeg, &pOMFEffectSlot));
		}
		else
		{
			rc = pParm->QueryInterface(IID_IAAFVaryingValue, (void **)&pVaryingValue);
			if (SUCCEEDED(rc))
			{
				checkAAF(pVaryingValue->GetInterpolationDefinition(&pInterpDef));
				checkAAF(pInterpDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef));
				pInterpDef->Release();
				pInterpDef = NULL;
				checkAAF(pDef->GetAUID(&interpDefID));
				pDef->Release();
				pDef = NULL;
				if(memcmp(&interpDefID, &LinearInterpolator, sizeof(interpDefID)) == 0)
				{
					interpKind = OMF2::kLinearInterp;
				}
				else if(memcmp(&interpDefID, &ConstantInterpolator, sizeof(interpDefID)) == 0)
				{
					interpKind = OMF2::kConstInterp;
				}
				// else error!!!
				
				checkOMF(OMF2::omfiVaryValueNew(OMFFileHdl, pOMFDatakind, effectLen,
					interpKind, &vval));
				omfSeg = vval;
				
				checkAAF(pVaryingValue->GetControlPoints(&pointEnum));
				while(pointEnum->NextOne(&pPoint) == AAFRESULT_SUCCESS)
				{
					checkAAF(pPoint->GetTime(&aafTime));
					omfTime.numerator = aafTime.numerator;
					omfTime.denominator = aafTime.denominator;
					rc = pPoint->GetEditHint(&aafEditHint);
					if(rc == AAFRESULT_PROP_NOT_PRESENT)
						aafEditHint = kNoEditHint;
					else
						checkAAF(rc);
					switch(aafEditHint)
					{
					case kNoEditHint:
						omfEditHint = OMF2::kNoEditHint;
						break;
					case kProportional:
						omfEditHint = OMF2::kProportional;
						break;
					case kRelativeLeft:
						omfEditHint = OMF2::kRelativeLeft;
						break;
					case kRelativeRight:
						omfEditHint = OMF2::kRelativeRight;
						break;
					case kRelativeFixed:
						omfEditHint = OMF2::kRelativeFixed;
						break;
						//!!!Handle default case
					}
					
					checkAAF(pPoint->GetValueBufLen(&srcValueLen));
					srcValue = new unsigned char[srcValueLen];
					checkAAF(pPoint->GetValue(srcValueLen, srcValue, &lenRead));
					ConvertValueBuf(typeDefID, srcValue, lenRead, &destValue, &destValueLen, &didNew);
					checkOMF(OMF2::omfiVaryValueAddPoint(OMFFileHdl, vval, omfTime, omfEditHint, pOMFDatakind,
						destValueLen, destValue));
					if(didNew)
						delete [] destValue;
					delete [] srcValue;
					destValue = NULL;
					srcValue = NULL;
				}
				checkOMF(OMF2::omfiEffectAddNewSlot(OMFFileHdl,pOMFEffect,
					slotNum, omfSeg, &pOMFEffectSlot));
			}
			//!!!Else error
		}
	}
cleanup:
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
	return(moduleErrorTmp);
}


OMF2::omfObject_t Aaf2Omf::LocateSlot(OMF2::omfEffObj_t pOMFEffect, aafInt32 kfSlotNum)
{
	OMF2::omfNumSlots_t	numSlots, slotID;
	OMF2::omfIterHdl_t	slotIter;
	OMF2::omfESlotObj_t	effectSlot;
	OMF2::omfObject_t	segment, result = NULL;

	(void)OMF2::omfiEffectGetNumSlots(OMFFileHdl, pOMFEffect, &numSlots);
	(void)OMF2::omfiIteratorAlloc(OMFFileHdl, &slotIter);
	while(OMF2::omfiEffectGetNextSlot(slotIter, pOMFEffect, NULL, &effectSlot) == OMF2::OM_ERR_NONE)
	{
		(void)OMF2::omfiEffectSlotGetInfo(OMFFileHdl, effectSlot, &slotID, &segment);
		if(slotID == kfSlotNum)
		{
			result = segment;
		}
	}
	(void)OMF2::omfiIteratorDispose(OMFFileHdl, slotIter);

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
