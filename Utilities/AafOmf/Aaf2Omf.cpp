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

#include "Aaf2Omf.h"

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

	OMFError = OMF2::omfsRegisterDynamicProp(OMFSession, OMF2::kOmfTstRevEither,
		"AvidPrivateEffectID", OMClassEFFE, OMF2::OMUniqueName,
		OMF2::kPropOptional, &privateEffectProp);
	
	bSessionStarted = AAFTrue;
	OMFError = OMF2::omfmInit(OMFSession);
	if (OMFError != OMF2::OM_ERR_NONE )
	{
		OMF2::omfsEndSession(OMFSession);
		rc = AAFRESULT_BAD_SESSION;
		goto cleanup;
	}

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
						pwcName = new wchar_t [textSize/sizeof(wchar_t)];
						pMobComment->GetName(pwcName, textSize);
						pszCommName =  new char[textSize/sizeof(wchar_t)];
						wcstombs(pszCommName, pwcName, textSize/sizeof(wchar_t));

						pMobComment->GetValueBufLen((aafUInt32 *)&textSize);
						pwcComment = new wchar_t (textSize/sizeof(wchar_t));
						pMobComment->GetValue((aafUInt32)textSize, (aafDataBuffer_t)pwcComment, &bytesRead);
						pszComment =  new char[textSize/sizeof(wchar_t)];
						wcstombs(pszComment, pwcComment, textSize/sizeof(wchar_t));

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
	IAAFCDCIDescriptor*		pCDCIDesc = NULL;
	IAAFObject*				pAAFObject = NULL;

	if (gpGlobals->bVerboseMode)
		printf("Converting AAF Source MOB to OMF\n");

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
				pwManufacturer = new wchar_t[textSize/sizeof(wchar_t)];
				pTapeDesc->GetTapeManufacturer(pwManufacturer, textSize);
				pszManufacturer = new char[textSize/sizeof(wchar_t)];
				wcstombs(pszManufacturer, pwManufacturer, textSize/sizeof(wchar_t));
			}
			else
				pszManufacturer = NULL;
			pTapeDesc->GetTapeModelBufLen(&textSize);
			if (textSize > 0)
			{
				pwModel = new wchar_t[textSize/sizeof(wchar_t)];
				pTapeDesc->GetTapeModel(pwModel, textSize);
				pszModel = new char[textSize/sizeof(wchar_t)];
				wcstombs(pszModel, pwModel, textSize/sizeof(wchar_t));
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
			// It is a CDCI file descriptor
			OMFError = OMF2::omfmFileMobNew(OMFFileHdl, pMobName, rate, CODEC_CDCI_VIDEO, pOMFSourceMob);
			OMFError = OMF2::omfiMobSetIdentity(OMFFileHdl, *pOMFSourceMob, OMFMobID);
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
// GetEffectIDsFromAUID
//
//			This function converts an AAF effectID into an OMF unique name. 
//			
// Returns: AAFRESULT_SUCCESS if datakind is converted succesfully
//			
// NOTE :	the buffer passed to this routine should be big enough to accomodate
//			the whole OMF name.!!
// ============================================================================
HRESULT Aaf2Omf::GetEffectIDsFromAUID(aafUID_t Datadef,
									   OMF2::omfUniqueNamePtr_t effectID,
									   OMF2::omfUniqueNamePtr_t MCEffectID)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	// OMF2::omfBool			bFound;
	char					szAUID[OMUNIQUENAME_SIZE];

	MCEffectID[0] = '\0';
	if ( memcmp((char *)&Datadef, (char *)&kAAFEffectVideoDissolve, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:SimpleVideoDissolve");
		strcpy(MCEffectID, "BLEND_DISSOLVE");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectVideoFadeToBlack, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:VideoFadeToBlack");
		strcpy(MCEffectID, "BLEND_FADE_DOWN");
	}
//!!! Wipe will be more complicated to figure out
//	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectSMPTEVideoWipe, sizeof(aafUID_t)) == 0 )
//	{
//		strcpy(effectID, "omfi:effect:SMPTEVideoWipe");
//		strcpy(MCEffectID, xxxx);
//	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectVideoSpeedControl, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:VideoSpeedControl");
		strcpy(MCEffectID, "EFF_TIMEWARP.MOTION_CTL");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectVideoRepeat, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:VideoRepeat");
		strcpy(MCEffectID, "EFF_TIMEWARP.REPEAT");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectVideoFrameToMask, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:VideoFrameMask");
		strcpy(MCEffectID, "EFF_TIMEWARP.CAPTURE_MASK");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectMonoAudioDissolve, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:SimpleMonoAudioDissolve");
		strcpy(MCEffectID, "BLEND_AUDIO_DISSOLVE");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectMonoAudioPan, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:MonoAudioPan");
		strcpy(MCEffectID, "Audio Pan Volume");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectMonoAudioGain, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:MonoAudioGain");
		strcpy(MCEffectID, "Audio Pan Volume");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectStereoAudioDissolve, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:StereoAudioDissolve");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kAAFEffectStereoAudioGain, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "omfi:effect:StereoAudioGain");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffBlendPIPUUID, sizeof(aafUID_t)) == 0 )
	{
		strcpy(MCEffectID, "EFF_BLEND_PIP");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffBlendSuperUUID, sizeof(aafUID_t)) == 0 )
	{
		strcpy(MCEffectID, "BLEND_SUPER");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffBlendFadeUpUUID, sizeof(aafUID_t)) == 0 )
	{
		strcpy(MCEffectID, "BLEND_FADE_UP");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffBlendDipUUID, sizeof(aafUID_t)) == 0 )
	{
		strcpy(MCEffectID, "BLEND_DIP");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffXRotate, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_ROTATE_XROTATE");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffYRotate, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_ROTATE_YROTATE");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffZRotate, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_ROTATE_ZROTATE");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeZoom, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_ZOOM");

	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeHorz, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_HORZ");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeVert, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_VERT");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeBottomCenter, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_BOTTOMCENTER");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeRightCenter, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_RIGHTCENTER");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeTopCenter, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_TOPCENTER");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeLeftCenter, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_LEFTCENTER");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeTop, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZETOP");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeBottom, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZEBOTTOM");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeLeft, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZELEFT");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeRight, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZERIGHT");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeBottomLeft, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_BOTTOMLEFT");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeTopRight, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_TOPRIGHT");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeBottomRight, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_BOTTOMRIGHT");
	}
	else if ( memcmp((char *)&Datadef, (char *)&kEffSqueezeTopLeft, sizeof(aafUID_t)) == 0 )
	{
		strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
		strcpy(MCEffectID, "EFF_BLEND_SQUEEZE_TOPLEFT");
	}
	else
	{
		AUIDtoString(&Datadef, szAUID);
		printf("%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		fprintf(stderr,"%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		rc = AAFRESULT_INVALID_DATADEF;
	}

#if 0
#define EFF_EMPTY_EFFECT				"EFF_EMPTY_EFFECT"	//	This is the ID for the effect 
															//	that is returned when the requested 
															//	effect is not available
#define EFF_AUDIO_PANVOL				
#define EFF_AUDIO_EQMB					"EFF_AUDIO_EQMB"	// Multiband equalization (ADM-style)
#define EFF_AUDIO_AS_PLUG_IN			"EFF_AUDIO_AS_PLUG_IN"	// Digi Audio Suite Plug-In
#define EFF_TIME_WARP					"EFF_TIMEWARP"
#define EFF_MOTION_STROBE				"EFF_TIMEWARP.STROBE"
#define EFF_AUDIO_WARP                  "EFF_TIMEWARP.AUDIO_TIME_WARP"
#define EFF_BASIC_PLUGIN				"EFF_BASIC_PLUGIN"
#define EFF_PLUGIN_FILTER				"EFF_PLUGIN_FILTER"
#define EFF_PLUGIN_TRANSITION			"EFF_PLUGIN_TRANSITION"        
#define EFF_PIP							"EFF_PICTURE_IN_PICTURE"
#define EFF_BLEND_VDISSOLVE				"BLEND_DISSOLVE"
#define EFF_BLEND_WIPE_RBOX				"EFF_BLEND_WIPE_RBOX"
#define EFF_BLEND_WIPE_TBOX				"EFF_BLEND_WIPE_TBOX"
#define EFF_BLEND_WIPE_LBOX				"EFF_BLEND_WIPE_LBOX"
#define EFF_BLEND_WIPE_BBOX				"EFF_BLEND_WIPE_BBOX"
#define EFF_BLEND_WIPE_LR				"EFF_BLEND_WIPE_LR"
#define EFF_BLEND_WIPE_LL				"EFF_BLEND_WIPE_LL"
#define EFF_BLEND_WIPE_UR				"EFF_BLEND_WIPE_UR"
#define EFF_BLEND_WIPE_UL				"EFF_BLEND_WIPE_UL"
#define EFF_BLEND_WIPE_VOPEN			"EFF_BLEND_WIPE_VOPEN"
#define EFF_BLEND_WIPE_HOPEN			"EFF_BLEND_WIPE_HOPEN"
#define EFF_BLEND_WIPE_VERT				"EFF_BLEND_WIPE_VERT"
#define EFF_BLEND_WIPE_HORZ				"EFF_BLEND_WIPE_HORZ"
#define EFF_BLEND_WIPE_LRLDIAG			"EFF_BLEND_WIPE_LRLDIAG"
#define EFF_BLEND_WIPE_LLRDIAG			"EFF_BLEND_WIPE_LLRDIAG"
#define EFF_BLEND_WIPE_URLDIAG			"EFF_BLEND_WIPE_URLDIAG"
#define EFF_BLEND_WIPE_ULRDIAG			"EFF_BLEND_WIPE_ULRDIAG"
#define EFF_BLEND_WIPE_SPIRAL			"EFF_BLEND_WIPE_SPIRAL"
#define EFF_BLEND_WIPE_GRID				"EFF_BLEND_WIPE_GRID"
#define EFF_BLEND_WIPE_1ROW				"EFF_BLEND_WIPE_1ROW"
#define EFF_BLEND_WIPE_SPECKLE			"EFF_BLEND_WIPE_SPECKLE"
#define EFF_BLEND_WIPE_ZIG_ZAG			"EFF_BLEND_WIPE_ZIG_ZAG"
#define EFF_BLEND_WIPE_VOPEN_SAW		"EFF_BLEND_WIPE_VOPEN_SAW"
#define EFF_BLEND_WIPE_HOPEN_SAW		"EFF_BLEND_WIPE_HOPEN_SAW"
#define EFF_BLEND_WIPE_VSAW				"EFF_BLEND_WIPE_VSAW"
#define EFF_BLEND_WIPE_HSAW				"EFF_BLEND_WIPE_HSAW"
#define EFF_BLEND_WIPE_4CORNER			"EFF_BLEND_WIPE_4CORNER"
#define EFF_BLEND_WIPE_BOX				"EFF_BLEND_WIPE_BOX"
#define EFF_BLEND_WIPE_CIRCLE			"EFF_BLEND_WIPE_CIRCLE"
#define EFF_BLEND_WIPE_RADAR			"EFF_BLEND_WIPE_RADAR"
#define EFF_BLEND_WIPE_DIAMOND			"EFF_BLEND_WIPE_DIAMOND"
#define EFF_BLEND_WIPE_ELLIPSE			"EFF_BLEND_WIPE_ELLIPSE"
#define EFF_BLEND_WIPE_2ROW				"EFF_BLEND_WIPE_2ROW"		// horizontal bands
#define EFF_BLEND_WIPE_HBLIND			"EFF_BLEND_WIPE_HBLIND"
#define EFF_BLEND_WIPE_VBLIND			"EFF_BLEND_WIPE_VBLIND"
#define EFF_BLEND_MASK_166				"EFF_BLEND_MASK_166"
#define EFF_BLEND_MASK_185				"EFF_BLEND_MASK_185"
#define EFF_BLEND_MASK_235				"EFF_BLEND_MASK_235"
#define EFF_BLEND_MASK_177				"EFF_BLEND_MASK_177"		// 16x9 Mask
#define EFF_BLEND_VDISSOLVE_LIN			"BLEND_DISSOLVE_LIN"		// film fade
#define EFF_BLEND_VDISSOLVE_LOG			"BLEND_DISSOLVE_LOG"		// film dissolve
#define EFF_BLOW_UP						"EFF_BLOW_UP"
#define EFF_BLEND_MASK					"EFF_BLEND_MASK"
#define EFF_COLORCORRECT				"EFF_COLORCORRECT"			// Color Effect
#define EFF_RGB_COLOR_CORRECTION		"EFF_RGB_COLOR_CORRECTION"	// Chameleon Color Correction
#define EFF_BLEND_FLIP_VERT				"EFF_BLEND_FLIP_VERT"		// flip
#define EFF_BLEND_FLIP_BOTH				"EFF_BLEND_FLIP_BOTH"		// flip/flop
#define EFF_BLEND_FLIP_HORZ				"EFF_BLEND_FLIP_HORZ"		// flop
#define EFF_BLEND_MASK_IMAGE			"EFF_BLEND_MASK_IMAGE"
#define EFF_PAINT						"EFF_PAINT"
#define EFF_PAINT_MOSAIC				"EFF_PAINT_MOSAIC"
#define EFF_PAINT_SPOT_COLOR				"EFF_PAINT_SPOT_COLOR"
#define EFF_PAINT_MPEG_OUTLINE				"EFF_PAINT_MPEG_OUTLINE"
#define EFF_PAINT_SCRATCH               "EFF_PAINT_SCRATCH"
#define EFF_BLEND_RESIZE				"EFF_BLEND_RESIZE"
#define EFF_PAN_SCAN					"EFF_PAN_SCAN"
#define EFF_SUBMASTER					"EFF_SUBMASTER"
#define EFF_ANIMATTE					"EFF_ANIMATTE"
#define EFF_BLEND_LUMAKEY				"EFF_BLEND_LUMAKEY"
#define EFF_BLEND_MATTE_KEY				"EFF_BLEND_MATTE_KEY"			// DSF - the software Matte Key
#define EFF_BLEND_YUV_CHROMAKEY			"EFF_BLEND_YUV_CHROMAKEY"		// real chroma key - 5.2 and beyond
#define EFF_BLEND_SIMPLE_LUMAKEY		"EFF_BLEND_SIMPLE_LUMAKEY"
#define EFF_BLEND_SIMPLE_CHROMAKEY		"EFF_BLEND_SIMPLE_CHROMAKEY"	// for debug only
#define EFF_BLEND_CHROMAKEY				"EFF_BLEND_CHROMAKEY"			// pre 5.2
#define EFF_BLEND_TOUCAN_CHROMAKEY		"EFF_BLEND_TOUCAN_CHROMAKEY"	// enhanced chromakey plugin
#define EFF_2DMATTE_KEY					"EFF_2DMATTE"					// real time static matte
#define EFF_BLEND_CONCEAL_BOTTOMLEFT	"EFF_BLEND_CONCEAL_BOTTOMLEFT"
#define EFF_BLEND_CONCEAL_BOTTOMRIGHT	"EFF_BLEND_CONCEAL_BOTTOMRIGHT"
#define EFF_BLEND_CONCEAL_TOPRIGHT		"EFF_BLEND_CONCEAL_TOPRIGHT"
#define EFF_BLEND_CONCEAL_TOPLEFT		"EFF_BLEND_CONCEAL_TOPLEFT"
#define EFF_BLEND_CONCEAL_UP			"EFF_BLEND_CONCEAL_UP"
#define EFF_BLEND_CONCEAL_DOWN			"EFF_BLEND_CONCEAL_DOWN"
#define EFF_BLEND_CONCEAL_LEFT			"EFF_BLEND_CONCEAL_LEFT"
#define EFF_BLEND_CONCEAL_RIGHT			"EFF_BLEND_CONCEAL_RIGHT"
#define EFF_BLEND_LCONCEAL_BOTTOMLEFT	"EFF_BLEND_LCONCEAL_BOTTOMLEFT"
#define EFF_BLEND_LCONCEAL_BOTTOMRIGHT	"EFF_BLEND_LCONCEAL_BOTTOMRIGHT"
#define EFF_BLEND_LCONCEAL_TOPRIGHT		"EFF_BLEND_LCONCEAL_TOPRIGHT"
#define EFF_BLEND_LCONCEAL_TOPLEFT		"EFF_BLEND_LCONCEAL_TOPLEFT"
#define EFF_BLEND_PEEL_BOTTOMLEFT		"EFF_BLEND_PEEL_BOTTOMLEFT"
#define EFF_BLEND_PEEL_BOTTOMRIGHT		"EFF_BLEND_PEEL_BOTTOMRIGHT"
#define EFF_BLEND_PEEL_TOPRIGHT			"EFF_BLEND_PEEL_TOPRIGHT"
#define EFF_BLEND_PEEL_TOPLEFT			"EFF_BLEND_PEEL_TOPLEFT"
#define EFF_BLEND_PEEL_TOP				"EFF_BLEND_PEEL_TOP"
#define EFF_BLEND_PEEL_BOTTOM			"EFF_BLEND_PEEL_BOTTOM"
#define EFF_BLEND_PEEL_LEFT				"EFF_BLEND_PEEL_LEFT"
#define EFF_BLEND_PEEL_RIGHT			"EFF_BLEND_PEEL_RIGHT"
#define EFF_BLEND_PUSH_BOTTOMLEFT		"EFF_BLEND_PUSH_BOTTOMLEFT"
#define EFF_BLEND_PUSH_BOTTOMRIGHT		"EFF_BLEND_PUSH_BOTTOMRIGHT"
#define EFF_BLEND_PUSH_TOPRIGHT			"EFF_BLEND_PUSH_TOPRIGHT"
#define EFF_BLEND_PUSH_TOPLEFT			"EFF_BLEND_PUSH_TOPLEFT"
#define EFF_BLEND_PUSH_TOP				"EFF_BLEND_PUSH_TOP"
#define EFF_BLEND_PUSH_BOTTOM			"EFF_BLEND_PUSH_BOTTOM"
#define EFF_BLEND_PUSH_LEFT				"EFF_BLEND_PUSH_LEFT"
#define EFF_BLEND_PUSH_RIGHT			"EFF_BLEND_PUSH_RIGHT"
#define EFF_SBLEND						"EFF_SBLEND"
#define EFF_3DPIP						"EFF_3DPIP"						// for MCX
#define EFF_3DSHAPE_PAGE_CURL			"EFF_3DSHAPE_PAGE_CURL"			// for MCX
#define EFF_3DSHAPE_QUAD_CURL			"EFF_3DSHAPE_QUAD_CURL"			// for MCX
#define EFF_3DSHAPE_ARROW				"EFF_3DSHAPE_ARROW"				// for MCX
#define EFF_3DSHAPE_SLATS				"EFF_3DSHAPE_SLATS"				// for MCX
#define EFF_3DSHAPE_MULTI_WAVE			"EFF_3DSHAPE_MULTI_WAVE"		// for MCX
#define EFF_3DSHAPE_CENTER_BURST		"EFF_3DSHAPE_CENTER_BURST"		// for MCX
#define EFF_3DSHAPE_SINE_WAVE			"EFF_3DSHAPE_SINE_WAVE"			// for MCX
#define EFF_3DSHAPE_BALL				"EFF_3DSHAPE_BALL"				// for MCX
#define EFF_3DSHAPE_PAGE_FOLD			"EFF_3DSHAPE_PAGE_FOLD"			// for MCX
#define EFF_3DSHAPE_BUMPS				"EFF_3DSHAPE_BUMPS"				// for MCX
#define EFF_3DTITLE						"EFF_3DTITLE"
#define EFF_3DMATTE_KEY					"EFF_3DMATTE"
#define EFF_ROLLING_TITLE				"EFF_ROLLING_TITLE"
#define EFF_ROLLING_2DMATTE				"EFF_ROLLING_2DMATTE"
#define EFF_CRAWLING_TITLE				"EFF_CRAWLING_TITLE"
#define EFF_CRAWLING_2DMATTE			"EFF_CRAWLING_2DMATTE"
#define EFF_BLEND_TITLE					"EFF_BLEND_GRAPHIC"
#define BLEND_DISSOLVE_LIN				"BLEND_DISSOLVE_LIN"		// film fade
#define BLEND_DISSOLVE_LOG				"BLEND_DISSOLVE_LOG"		// film dissolve
#define EFF_TEST_ONE_TRACK				"EFF_TEST_ONE_TRACK"
#define EFF_TEST_TWO_TRACK				"EFF_TEST_TWO_TRACK"
#define EFF_TEST_THREE_TRACK			"EFF_TEST_THREE_TRACK"
#endif

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

		GetEffectIDsFromAUID(effectDefAUID, effectID, MCEffectID);
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
			checkOMF(OMF2::omfsWriteUniqueName(OMFFileHdl, (*pOMFEffect), privateEffectProp,
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
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect), -3,
										(OMF2::omfLength_t)length));
		}
#if 0
		if(pEffect->GetParameterByArgID(kAAFParameterDefAmplitude, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParameterDefPan, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParameterDefSMPTEWipeNumber, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
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
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParameterDefSpeedRatio, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidBorderWidth, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidBorderSoft, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidXPos, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidYPos, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
										(OMF2::omfLength_t)length));
		}
		if(pEffect->GetParameterByArgID(kAAFParamID_AvidCrop, &pParameter) == AAFRESULT_SUCCESS)
		{
			checkAAF(ConvertParameter(pParameter, (*pOMFEffect),  xxx,
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
								  OMF2::omfSegObj_t		pOMFEffect,
									OMF2::omfInt32		slotNum,
									OMF2::omfLength_t	effectLen)
{
	IAAFConstantValue	*pConstantValue = NULL;
	IAAFVaryingValue	*pVaryingValue = NULL;
	IAAFTypeDef			*pTypeDef = NULL;
	IAAFDefObject		*pDef = NULL;
	IAAFInterpolationDef	*pInterpDef = NULL;
	IAAFControlPoint	*pPoint = NULL;
	IEnumAAFControlPoints *pointEnum;
	aafUID_t			typeDefID, interpDefID;
	AAFRESULT			rc;
	aafUInt32			srcValueLen, destValueLen, lenRead;
	aafDataBuffer_t		srcValue = NULL, destValue = NULL;
	OMF2::omfInterpKind_t interpKind;
	OMF2::omfObject_t	vval;
	aafRational_t		aafTime;
	OMF2::omfRational_t	omfTime;
	aafEditHint_t		aafEditHint;
	OMF2::omfEditHint_t	omfEditHint;
	OMF2::omfSegObj_t	omfSeg;
	OMF2::omfObject_t	pOMFEffectSlot;
	OMF2::omfObject_t	pOMFDatakind;
	
	moduleErrorTmp = AAFRESULT_SUCCESS;
    checkAAF(pParm->GetTypeDefinition(&pTypeDef));
	checkAAF(pTypeDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef));
    checkAAF(pDef->GetAUID(&typeDefID));
	pDef->Release();
	pDef = NULL;
	checkAAF(ConvertAAFTypeIDDatakind(typeDefID, &pOMFDatakind));

	rc = pParm->QueryInterface(IID_IAAFConstantValue, (void **)&pConstantValue);
	if (SUCCEEDED(rc))
    {
        checkAAF(pConstantValue->GetValueBufLen(&srcValueLen));
		srcValue = new unsigned char[srcValueLen];
        checkAAF(pConstantValue->GetValue(srcValueLen, srcValue, &lenRead));
		if(memcpy(&typeDefID, &kAAFTypeID_Rational, sizeof(typeDefID)) == 0)
		{
			destValue = srcValue;
			destValueLen = srcValueLen;
		}
		else // Error!!!
		{
			destValue = srcValue;
			destValueLen = srcValueLen;
		}
		checkOMF(OMF2::omfiConstValueNew(OMFFileHdl, pOMFDatakind, effectLen,
									destValueLen, destValue, &omfSeg));
		if((destValue != NULL) && (srcValue != destValue))
			delete [] destValue;
		delete [] srcValue;
		destValue = NULL;
		srcValue = NULL;
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

				if(memcmp(&typeDefID, &kAAFTypeID_AvidPosition, sizeof(typeDefID)) == 0 ||
				   memcmp(&typeDefID, &kAAFTypeID_AvidCrop, sizeof(typeDefID)) == 0 ||
				   memcmp(&typeDefID, &kAAFTypeID_AvidScale, sizeof(typeDefID)) == 0 ||
				   memcmp(&typeDefID, &kAAFTypeID_AvidSpillSupress, sizeof(typeDefID)) == 0 ||
				   memcmp(&typeDefID, &kAAFTypeID_AvidBounds, sizeof(typeDefID)) == 0 ||
				   memcmp(&typeDefID, &kAAFTypeID_AvidEffColor, sizeof(typeDefID)) == 0 ||
				   memcmp(&typeDefID, &kAAFTypeID_AvidEffUserParam, sizeof(typeDefID)) == 0)
				{
					checkAAF(UpdateKeyFrameVVAL(pPoint, vval, omfTime,
									sizeof(OMFIPvtKFInfo_t),
									omfEditHint,
									pOMFDatakind));
				}
				else
				{
					checkAAF(pPoint->GetValueBufLen(&srcValueLen));
					srcValue = new unsigned char[srcValueLen];
					checkAAF(pPoint->GetValue(srcValueLen, srcValue, &lenRead));
					destValue = NULL;
					destValueLen = 0;
					if(memcmp(&typeDefID, &kAAFTypeID_Rational, sizeof(typeDefID)) == 0)
					{
						destValue = srcValue;
						destValueLen = srcValueLen;
					}
					else // Error!!!
					{
						destValue = srcValue;
						destValueLen = srcValueLen;
					}

					checkOMF(OMF2::omfiVaryValueAddPoint(OMFFileHdl, vval, omfTime, omfEditHint, pOMFDatakind,
										destValueLen, destValue));
					if((destValue != NULL) && (srcValue != destValue))
						delete [] destValue;
					delete [] srcValue;
					destValue = NULL;
					srcValue = NULL;
				}
			}
		}
		//!!!Else error

		checkOMF(OMF2::omfiEffectAddNewSlot(OMFFileHdl,pOMFEffect,
									slotNum, omfSeg, &pOMFEffectSlot));
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

HRESULT Aaf2Omf::UpdateKeyFrameVVAL(IAAFControlPoint*		controlPoint,
									OMF2::omfSegObj_t		vval,
									OMF2::omfRational_t		time,
									aafInt32				destValueLen,
									OMF2::omfEditHint_t		editHint,
									OMF2::omfDDefObj_t		dataKind)
{
	// N^2 problem, we need to optimize this somehow
	OMF2::omfInt32		numPoints, n, bytesRead;
	OMF2::omfIterHdl_t	hdl;
	OMF2::omfCntlPtObj_t ctlp;
	bool				found = false;
	OMF2::omfRational_t	ctlpTime;
	aafDataBuffer_t		value;

	moduleErrorTmp = AAFRESULT_SUCCESS;
	checkOMF(OMF2::omfiVaryValueGetNumPoints(OMFFileHdl, vval, &numPoints));
	checkOMF(OMF2::omfiIteratorAlloc(OMFFileHdl, &hdl));
	value = new unsigned char[destValueLen];
	for(n = 1; (n <= numPoints) && !found; n++)
	{
		checkOMF(OMF2::omfiVaryValueGetNextPoint(hdl, vval, NULL, &ctlp));
		checkOMF(OMF2::omfsReadRational(OMFFileHdl, vval, OMF2::OMCTLPTime, &ctlpTime));
		if(ctlpTime.numerator == time.numerator && 
		   ctlpTime.denominator == time.denominator)
		{
			found = true;
			// Assert NON-NULL!!!
			checkOMF(OMF2::omfiControlPtGetInfo(OMFFileHdl, ctlp, &ctlpTime,
												NULL, NULL, destValueLen,
												&bytesRead, value));
		}
	}
	checkOMF(OMF2::omfiIteratorDispose(OMFFileHdl, hdl));

	// Use the AAF Private definition kind to fill in any new fields
	// Not Yet DONE!!!

	// Either create the value, or replace the existing value
	if(found)
	{
		checkOMF(OMF2::omfsWriteDataValue(OMFFileHdl, ctlp, OMF2::OMCTLPValue,
				dataKind, value, 0, destValueLen));
	}
	else
	{
		checkOMF(OMF2::omfiVaryValueAddPoint(OMFFileHdl, vval, time, editHint,
											dataKind, destValueLen, value));
	}
cleanup:
	return(moduleErrorTmp);
}


// OTher idea: Upon hitting ANY of the Avid private params (or level) find a VVAL
// and assume that all VVALs containing AvidPrivate have identical times
