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
	if (FAILED(rc))
		return rc;

	rc = OpenOutputFile();
	if (FAILED(rc))
		return rc;
/*
	rc = ConvertOMFHeader();
	if (FAILED(rc))
		return rc;
*/
	
	rc = AAFFileRead();
	return (rc);
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
//	aafProductIdentification_t	ProductInfo;
	IAAFIdentification*			pIdent = NULL;

	// convert file name to Wide char
	UTLStrAToStrW(gpGlobals->sInFileName, &pwFileName);


	rc = AAFFileOpenExistingRead(pwFileName, 0, &pFile);
	if (FAILED(rc))
		return rc;

	rc = pFile->GetHeader(&pHeader);
	if (FAILED(rc))
		return rc;

	gpGlobals->bAAFFileOpen = AAFTrue;
	return rc;
}
// ============================================================================
// OpenOutputFile
//			This function creates the output file.
//
// ============================================================================
HRESULT Aaf2Omf::OpenOutputFile ()
{
	HRESULT		rc = AAFRESULT_SUCCESS;
	aafBool		bOverride = AAFFalse;
	aafBool		bSessionStarted = AAFFalse;

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

	rc = OMF2::omfsBeginSession(0, &OMFSession);
	if (rc != OMF2::OM_ERR_NONE )
		return rc;
	
	bSessionStarted = AAFTrue;
	rc = OMF2::omfmInit(OMFSession);
	if (rc != OMF2::OM_ERR_NONE )
	{
		OMF2::omfsEndSession(OMFSession);
		return AAFRESULT_BAD_SESSION;
	}

	rc = OMF2::omfsCreateFile((OMF2::fileHandleType)gpGlobals->sOutFileName, OMFSession, OMF2::kOmfRev2x, &OMFFileHdl);
	if (rc != OMF2::OM_ERR_NONE )
		return AAFRESULT_BADOPEN;

	gpGlobals->bOMFFileOpen = AAFTrue;
	return AAFRESULT_SUCCESS;
}

// ============================================================================
// CloseOutputFile
//			This function closes the output file.
//
// ============================================================================
void Aaf2Omf::CloseOutputFile()
{
	OMF2::omfsCloseFile(OMFFileHdl);
	OMF2::omfsEndSession(OMFSession);
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
	aafInt32				nAAFNumMobs;
	aafInt32				nAAFMobCount;
	IEnumAAFMobs*			pMobIter = NULL;
	IAAFMob*				pMob = NULL;
	IAAFCompositionMob*		pCompMob = NULL;
	aafSearchCrit_t			criteria;

	rc = pHeader->GetNumMobs(kAllMob, &nAAFNumMobs);
	criteria.searchTag = kByMobKind;
	criteria.tags.mobKind = kAllMob;
	rc = pHeader->EnumAAFAllMobs(&criteria, &pMobIter);
	while (pMobIter && (pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS))
	{
		pMob->Release();
		pMob = NULL;
	}

	return rc;
}
