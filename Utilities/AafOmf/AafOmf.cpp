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

// OMF Includes
namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

#include "AAFTypes.h"

//#include "AAFUtils.h"
AAFRESULT aafMobIDNew(aafUID_t *mobID);
AAFRESULT aafMobIDFromMajorMinor(
        aafUInt32	major,
		aafUInt32	minor,
		aafUID_t *mobID);     /* OUT - Newly created Mob ID */

//#include "aafCvt.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

// AAF Utilities Infra-structure 
#include "UtlConsole.h"


#include "AafOmf.h"


// ============================================================================
// Global Variables and functions
// ============================================================================

// ============================================================================
// Delete a file given its  name
// ============================================================================
void deleteFile(char* fileName)
{

	int result = remove(fileName);
	//.. ignore error which typically happens if the file doesn;t exist

}

char* baseName(char* fullName)
{
	char* result;
#if defined(WIN32)
	const int delimiter = '\\';
#elif defined(_MAC) || defined(macintosh)
	const int delimiter = ':';
#else
	const in delimiter = '/';
#endif
	result = strrchr(fullName, delimiter);
	if (result == 0)
	{
		result = fullName;
	}
	else if (strlen(result) == 0) 
	{
		result = fullName;
	}
	else 
	{
		result++;
	}

	return result;
}

// ============================================================================
// Constructor
// ============================================================================
AafOmf::AafOmf() : pFile(NULL), pHeader(NULL), pDictionary(NULL),
	bOMFFileOpen(AAFFalse), bAAFFileOpen(AAFFalse),
	bCreateTOCFile(AAFFalse), bConvertAllObjects(AAFFalse), bLogFile(AAFFalse), bDefFile(AAFFalse), 
	nNumOMFObjects(0), nNumAAFObjects(0), nNumOMFProperties(0), nNumAAFProperties(0),
	nNumUndefinedOMFObjects(0), nNumUndefinedOMFProperties(0),numIndents(0)
{
	memset(sInFileName, 0, sizeof(sInFileName));
	memset(sOutFileName, 0, sizeof(sOutFileName));
	memset(sLogFileName, 0, sizeof(sLogFileName));
	memset(sDefinitionFileName, 0, sizeof(sDefinitionFileName));
	memset(indentLeader, 0, sizeof(indentLeader));
}

// ============================================================================
// Destructor
// ============================================================================
AafOmf::~AafOmf()
{
	UTLExitFileIO();
	if (bOMFFileOpen)
	{
		OMFFileClose();
		bOMFFileOpen = AAFFalse;
	}

	if (bAAFFileOpen)
	{
		AAFFileClose();
		bAAFFileOpen = AAFFalse;
	}
}

// ============================================================================
// Usage
// 
//		Displays short help text with the program arguments.
//
// ============================================================================
void AafOmf::Usage( void )
{
	UTLstdprintf("\n*******************\n\n");
	UTLstdprintf("%s : OMFI to AAF file conversion Version 0.01.00\n\n",pProgramName);
	UTLstdprintf("Usage: \n");
	UTLstdprintf("%s [-v] [-s] [-p logfile] [-d deffile] [-t tocfile] <infile> [outfile]\n\n", pProgramName);
	UTLstdprintf("-v         = Verbose - give progress report (optional)\n" );
	UTLstdprintf("-s         = Straight conversion. Do NOT discard unnecessary objects (optional)\n");
	UTLstdprintf("-p logfile = Log file name(optional)\n");
	UTLstdprintf("-d deffile = Definition file (optional)\n");
	UTLstdprintf("-t tocfile = Dump OMFI Table of contents (optional)\n");
	UTLstdprintf("infile     = input file name (required)\n");
	UTLstdprintf("outfile    = output file name (optional)\n");
	UTLstdprintf("\n*******************\n\n");
}
/******************** IncIndentLevel *******************
	Increases the incrementation used for readability

	Inputs:		none
				  
	Outputs:	none

	Returns:	none

	Errors:		none
*********************************************************/
void AafOmf::IncIndentLevel( void )
{
	numIndents++;
	if (numIndents <= MAX_INDENT)
		indentLeader[numIndents-1] = '\t';
}

/******************** DecIndentLevel *******************
	Lessens the incrementation used for readability

	Inputs:		none
			  
	Outputs:	none

	Returns:	none

	Errors:		none
*********************************************************/
void AafOmf::DecIndentLevel(void)
{
	numIndents--;
	if (numIndents >= 0 && numIndents < MAX_INDENT)
		indentLeader[numIndents] = 0;

}

// ============================================================================
// GetUserInput
//
//		Parse program arguments and extract names of OMF and AAF files.
//
// ============================================================================
HRESULT AafOmf::GetUserInput(int argc, char* argv[])
{
	HRESULT			rc = AAFRESULT_SUCCESS;
	aafInt32		nFileCount = 0;

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			char*	pNextArgument = argv[i];
			char	c = pNextArgument[0];
			char*	pFileName;

			if ((c == '-') && (strlen(pNextArgument) >= 2))
			{
				char flag = pNextArgument[1];
				switch( flag )
				{
					case 'v':
						bVerboseMode = AAFTrue;
						break;
					case 's':
						bConvertAllObjects = AAFTrue;
						break;
					case 'p':
						if ((i + 1 < argc)&& (*argv[i+1] != '-'))
						{
							i++;	// Consume the value
							pFileName = argv[i];
							if (strlen(pFileName))
							{
								bLogFile = AAFTrue;
								strcpy(sLogFileName, pFileName);
							}
						}
						break;
					case 'd':
						if ((i + 1 < argc)&& (*argv[i+1] != '-'))
						{
							i++;	// Consume the value
							pFileName = argv[i];
							if (strlen(pFileName))
							{
								bDefFile = AAFTrue;
								strcpy(sDefinitionFileName, pFileName);
							}
						}
						break;
					case 't':
						if ((i + 1 < argc)&& (*argv[i+1] != '-'))
						{
							i++;	// Consume the value
							pFileName = argv[i];
							if (strlen(pFileName))
							{
								bCreateTOCFile = AAFTrue;
								strcpy(sTOCFileName, pFileName);
							}
						}
						break;
					default:
						rc = AAFRESULT_BAD_FLAGS;
						break;
				}
			}
			else
			{
				nFileCount += 1;
				if (nFileCount == 1)
					strcpy(sInFileName, argv[i]);
				else
					strcpy(sOutFileName, argv[i]);
			}
		}
	}
	else
		rc = AAFRESULT_BAD_FLAGS;

	return rc;
}
// ============================================================================
// OpenInputFile
//			This function determines the input file type and opens the file.
//
// ============================================================================
HRESULT AafOmf::OpenInputFile ()
{
	HRESULT		rc = AAFRESULT_SUCCESS;
	FILE*		pStream = NULL;

	// verify if file exists
	if (strlen(sInFileName) > 0)
	{
		pStream = fopen(sInFileName, "r");
		if (pStream != NULL)
		{
			if (IsOMFFile(pStream))
			{
				fclose(pStream);
				rc = OMFFileOpen(sInFileName);
			}
			else
			{
				fclose(pStream);
				UTLerrprintf("File : %s is not a valid OMF file!!!\n",sInFileName);
				rc = OMF2::OM_ERR_NOTOMFIFILE;
			}
		}
		else
		{
			UTLerrprintf("Unable to Open %s\n", sInFileName);
			rc = AAFRESULT_FILE_NOT_FOUND;
		}
	}
	else
		rc = AAFRESULT_NULL_PARAM;

	return rc;
}

// ============================================================================
// OpenOutputFile
//			This function creates the output file.
//
// ============================================================================
HRESULT AafOmf::OpenOutputFile ()
{
	HRESULT		rc = AAFRESULT_SUCCESS;
	aafBool		bOverride = AAFFalse;

	if (strlen(sOutFileName) == 0)
	{
		char*	pExt;
		bOverride = AAFTrue;
		strcpy(sOutFileName, sInFileName);
		pExt= strrchr(sOutFileName, '.');
		strcpy(pExt,".aaf");
	}
	
	if (bOverride)
	{
		UTLstdprintf("Previous file: %s will be overwritten\n",sOutFileName);
		deleteFile(sOutFileName);
	}

	rc = AAFFileOpen(sOutFileName);

	return rc;
}
// ============================================================================
// IsOMFFile
//			This function returns AAFTrue if the given file is an OMF file.
//
// ============================================================================
aafBool AafOmf::IsOMFFile (FILE* pStream)
{
	aafBool		bReturn = AAFFalse;
	int			result = 0;
	char		ReadBuffer[8];
	char		CompBuffer[8];


	CompBuffer[0] = (char)0xA4;
	CompBuffer[1] = 'C';
	CompBuffer[2] = 'M';
	CompBuffer[3] = (char)0xA5;
	CompBuffer[4] = 'H';
	CompBuffer[5] = 'd';
	CompBuffer[6] = 'r';
	CompBuffer[7] = 0x00;

	result = fseek(pStream, -24, SEEK_END);
	if (result == 0)
	{
		result = fread(ReadBuffer, sizeof( char ), sizeof(ReadBuffer), pStream);
		if (result > 0)
		{
			if (memcmp(CompBuffer, ReadBuffer, strlen(CompBuffer)) == 0)
				bReturn = AAFTrue;
		}
	}
	
	return bReturn;
}
// ============================================================================
// OMFFileOpen
//
//		Open an OMF File
//
// ============================================================================
HRESULT AafOmf::OMFFileOpen(char * pFileName)
{
	HRESULT				rc = AAFRESULT_SUCCESS;
	aafBool				bSessionStarted = AAFFalse;

	if (OMF2::OM_ERR_NONE == OMF2::omfsBeginSession(0, &OMFSession))
	{
		bSessionStarted = AAFTrue;
		if (OMF2::OM_ERR_NONE == OMF2::omfmInit(OMFSession))
		{
			rc = OMF2::omfsOpenFile((OMF2::fileHandleType)pFileName, OMFSession, &OMFFileHdl);

		}
		else
			rc = AAFRESULT_BADOPEN;
	}
	else 
	{
		if (bSessionStarted)
			OMF2::omfsEndSession(OMFSession);

		rc = AAFRESULT_BAD_SESSION;
	}

	OMF2::omfsFileGetRev(OMFFileHdl, &OMFFileRev);
	if (OMF2::kOmfRev2x != OMFFileRev)
	{
		rc = AAFRESULT_FILEREV_NOT_SUPP ;
		UTLerrprintf("OMFI File Revision 1.0 is NOT supported\n\n");
		OMF2::omfsCloseFile(OMFFileHdl);
		OMF2::omfsEndSession(OMFSession);
	}
	else
	{
		bOMFFileOpen = AAFTrue;
		if (bVerboseMode)
			UTLstdprintf("OMF File: %s opened succesfully\n", pFileName);
	}
	return rc;
}

// ============================================================================
// AAFFileOpen
//
//		Open an AAF File
//
// ============================================================================
HRESULT AafOmf::AAFFileOpen( char* pFileName)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfObject_t		OMFHeadObject, OMFIdent;
	char					text[256];
	aafWChar*				pwFileName = NULL;
	aafInt32				nOMFIdentifications;
	aafWChar*				pwCompanyName = NULL;
	aafWChar*				pwProductName;
    aafWChar*				pwProductVersionString = NULL;
    aafWChar*				pwPlatform;
	aafBool					bAddExtraIdent = AAFFalse;

	aafProductIdentification_t	ProductInfo;
	IAAFIdentification*		pIdent = NULL;

	
	UTLStrAToStrW(pFileName, &pwFileName);

	rc = CoCreateInstance(CLSID_AAFFile, NULL, CLSCTX_INPROC_SERVER, IID_IAAFFile, (void **)&pFile);
	if (AAFRESULT_SUCCESS == rc)
	{
		rc = pFile->Initialize();
		if (AAFRESULT_SUCCESS == rc)
		{
			// Get Identification from OMF Header 
			// We going to use the last identification entry in the
			// Identification array.. 
			OMF2::omfsGetHeadObject(OMFFileHdl, &OMFHeadObject);
			nOMFIdentifications = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeadObject, OMF2::OMHEADIdentList);
			if (nOMFIdentifications >= 1) 
			{
				rc = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeadObject, OMF2::OMHEADIdentList,
											&OMFIdent, nOMFIdentifications);
				if (AAFRESULT_SUCCESS == rc)
				{
					if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTCompanyName, text, sizeof(text)) != OMF2::OM_ERR_NONE)
						strcpy(text, "<Not Specified>");
					UTLStrAToStrW(text, &pwCompanyName);
					ProductInfo.companyName = pwCompanyName;
					if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTProductName, text, sizeof(text)) != OMF2::OM_ERR_NONE)
						strcpy(text, "<Not Specified>");
					UTLStrAToStrW(text, &pwProductName);
					ProductInfo.productName = pwProductName;
					if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTProductVersionString, text, sizeof(text)) != OMF2::OM_ERR_NONE)
						strcpy(text, "<Not Specified>");
					UTLStrAToStrW(text, &pwProductVersionString);
					ProductInfo.productVersionString = pwProductVersionString;
					if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTPlatform, text, sizeof(text)) != OMF2::OM_ERR_NONE)
						strcpy(text, "<Not Specified>");
					UTLStrAToStrW(text, &pwPlatform);
					ProductInfo.platform = pwPlatform;
					rc = pFile->OpenNewModify(pwFileName, 0, &ProductInfo);
					bAddExtraIdent = AAFTrue;
				}
			}
			else
			{
				ProductInfo.companyName = L"Company Name";
				ProductInfo.productName = L"OMF to AAF File Conversion";
				ProductInfo.productVersion.major = 1;
				ProductInfo.productVersion.minor = 0;
				ProductInfo.productVersion.tertiary = 0;
				ProductInfo.productVersion.patchLevel = 0;
				ProductInfo.productVersion.type = kVersionUnknown;
				ProductInfo.productVersionString = NULL;
				ProductInfo.productID = -1;
				ProductInfo.platform = NULL;
				rc = pFile->OpenNewModify(pwFileName, 0, &ProductInfo);
			}
		}
		bAAFFileOpen = AAFTrue;
		rc = pFile->GetHeader(&pHeader);
		if (AAFRESULT_SUCCESS == rc)
		{
			rc = pHeader->GetDictionary(&pDictionary);
			if (AAFRESULT_SUCCESS == rc && bAddExtraIdent)
			{
				// Create a new Composition Mob
				rc = pDictionary->CreateInstance(&AUID_AAFIdentification, 
	 											 IID_IAAFIdentification,
												 (IUnknown **)&pIdent);
				if (AAFRESULT_SUCCESS == rc)
				{
					ProductInfo.companyName = L"Company Name";
					ProductInfo.productName = L"OMF to AAF File Conversion";
					ProductInfo.productVersion.major = 1;
					ProductInfo.productVersion.minor = 0;
					ProductInfo.productVersion.tertiary = 0;
					ProductInfo.productVersion.patchLevel = 0;
					ProductInfo.productVersion.type = kVersionUnknown;
					ProductInfo.productVersionString = NULL;
					ProductInfo.productID = -1;
					ProductInfo.platform = NULL;
					pIdent->SetCompanyName(ProductInfo.companyName);
					pIdent->SetProductName(ProductInfo.productName);
					pIdent->SetProductVersion(&ProductInfo.productVersion);
					rc = pHeader->AppendIdentification(pIdent);
				}
				
			}
		}
	}
	else
		rc = AAFRESULT_INTERNAL_ERROR;

	if (bVerboseMode)
		UTLstdprintf("AAF File: %s Created succesfully\n", pFileName);

	// Clean up all allocated memory and return
	if(pwFileName)
		UTLMemoryFree(pwFileName);

	if(pwCompanyName)
		UTLMemoryFree(pwCompanyName);

	if(pwProductName)
		UTLMemoryFree(pwProductName);

	if(pwProductVersionString)
		UTLMemoryFree(pwProductVersionString);

	if(pwPlatform)
		UTLMemoryFree(pwPlatform);

	return rc;
}

// ============================================================================
// OMFFileRead
//
//		Here is where all the real work is done.  We recursively read all 
//		Objects inside the OMF file using omfiMobMatchAndExecute function.
//		For each object found the ConvertMob callback is invoked.
//		The  parsing and conversion of OMF objects into AAF objects takes place
//		inside the above mentioned callback. 
//
// ============================================================================
HRESULT AafOmf::OMFFileRead()
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	aafInt32				nOMFNumMobs;
	aafInt32				nOMFMobCount;

	// OMF Variables
	OMF2::omfIterHdl_t		OMFMobIter = NULL;
	OMF2::omfMobObj_t		OMFMob;
	OMF2::omfObject_t		OMFHeader, OMFObject;

	// AAF Variables
	IAAFMob*				pMob = NULL;
	IAAFCompositionMob*		pCompMob = NULL;
	IAAFMasterMob*			pMasterMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;

	aafInt32				numMedia;
	
	rc = OMF2::omfiIteratorAlloc( OMFFileHdl, &OMFMobIter);
	if (AAFRESULT_SUCCESS == rc)
	{
		rc = OMF2::omfiGetNumMobs( OMFFileHdl, OMF2::kAllMob, &nOMFNumMobs);
		if (SUCCEEDED(rc))
		{
			if (bVerboseMode)
			{
				UTLstdprintf("Found: %ld Mobs in the input file\n", nOMFNumMobs);
			}
			for (nOMFMobCount = 0; nOMFMobCount < nOMFNumMobs; nOMFMobCount++)
			{
				rc = OMF2::omfiGetNextMob(OMFMobIter, NULL, &OMFMob);
				if (SUCCEEDED(rc) )
				{
					pMob = NULL;
					if (OMF2::omfiIsACompositionMob(OMFFileHdl, OMFMob, (OMF2::omfErr_t *) &rc))
					{
						// Create a new Composition Mob
						rc = pDictionary->CreateInstance(&AUID_AAFCompositionMob, 
	 													 IID_IAAFCompositionMob,
														 (IUnknown **)&pCompMob);
						if (SUCCEEDED(rc))
						{
							if (bVerboseMode)
								UTLstdprintf( "Created AAF Composition Mob\n");
							rc = ConvertOMFCompositionObject( OMFMob, pCompMob );
							pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
							pCompMob->Release();
							pCompMob = NULL;
						}
					}
					else if (OMF2::omfiIsAMasterMob(OMFFileHdl, OMFMob, (OMF2::omfErr_t *) &rc) )
					{
						// Create a Master Mob 
						rc = pDictionary->CreateInstance(&AUID_AAFMasterMob,
														 IID_IAAFMasterMob,
														 (IUnknown **)&pMasterMob);
						if (SUCCEEDED(rc))
						{
							if (bVerboseMode)
								UTLstdprintf("Created AAF Master Mob\n");
							rc = ConvertOMFMasterMob(OMFMob, pMasterMob );
							pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
							pMasterMob->Release();
							pMasterMob = NULL;
						}
					}
					else if ( OMF2::omfiIsASourceMob(OMFFileHdl, OMFMob, (OMF2::omfErr_t *)&rc) )
					{
						rc = pDictionary->CreateInstance(&AUID_AAFSourceMob,
														 IID_IAAFSourceMob,
														 (IUnknown **)&pSourceMob);
						if (SUCCEEDED(rc))
						{
							if (bVerboseMode)
								UTLstdprintf("Created AAF Source Mob\n");
							rc = ConvertOMFSourceMob( OMFMob, pSourceMob );
							pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
							pSourceMob->Release();
							pSourceMob = NULL;
						}
					}
					else
					{
						OMF2::omfClassID_t		objClass;
						char					id[5];

						rc = OMF2::omfsReadClassID(OMFFileHdl, OMFMob, OMF2::OMOOBJObjClass, objClass);
						if (OMF2::OM_ERR_NONE == rc && bVerboseMode)
						{
							strncpy(id, objClass, 4);
							id[4] = '\0';
							UTLstdprintf("Unrecognized Mob Class ID: %s\n",id);
						}
					}
					if (pMob)
					{
						rc = ConvertOMFMOBObject( OMFMob, pMob);
						if (rc != AAFRESULT_SUCCESS)
							UTLerrprintf("Error convert basic MOB data\n");
						rc = TraverseOMFMob( OMFMob, pMob);
						if (rc != AAFRESULT_SUCCESS)
							UTLerrprintf("Error Traversing MOB\n ");
						rc = pHeader->AppendMob(pMob);
						if (rc != AAFRESULT_SUCCESS)
							UTLerrprintf("Error appending MOB to the file\n");
						pMob->Release();
						pMob = NULL;
					}
				}
			}
		}
	}
	OMF2::omfiIteratorDispose(OMFFileHdl, OMFMobIter);
	// Now that we have read all the metadata we can get on with
	// the actual essence (media) data

	rc = OMF2::omfsGetHeadObject( OMFFileHdl, &OMFHeader );
	numMedia = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData); 
	if (bVerboseMode)
	{
		UTLstdprintf("Found: %ld Media data objects\n", numMedia);
	}
	for (int k = 0;k < numMedia; k++)
	{
		rc = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADMediaData, &OMFObject, k+1);
		if (OMF2::OM_ERR_NONE == rc)
		{
			// Process the given Class Dictionary object.
			rc = ConvertOMFMediaDataObject(OMFObject);
		}
	}

	return rc;
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
// Returns: AAFRESULT_SUCCESS if Header object is converted succesfully
//
// ============================================================================
HRESULT AafOmf::ConvertOMFHeader( void )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfObject_t		OMFHeader, OMFObject;

	aafInt32				numDefs, numEntries;


	rc = OMF2::omfsGetHeadObject( OMFFileHdl, &OMFHeader );
	if (bVerboseMode)
	{
		UTLstdprintf("Processing OMF Header\n");
	}
	IncIndentLevel();
	if (OMF2::OM_ERR_NONE == rc)
	{
		//From the OMF header we will extract all definition Objects and Class Dictionary
		// First we process the Data definitions
		numDefs = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADDefinitionObjects);
		if (bVerboseMode)
		{
			UTLstdprintf("%sFound: %ld Data Definitions\n", indentLeader, numDefs);
		}
		for (int i = 1;i <= numDefs;i++)
		{
			rc = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADDefinitionObjects, &OMFObject, i);
			if (OMF2::OM_ERR_NONE == rc)
			{
				// Process the given Data Definition object.
				rc = ConvertOMFDataDefinitionObject(OMFObject);
			}
		}
		// Now we process the class dictionary
		numEntries = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADClassDictionary);
		if (bVerboseMode)
		{
			UTLstdprintf("Found: %ld Class Definitions\n", numEntries);
		}
		for (int j = 0;j < numEntries; j++)
		{
			rc = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADClassDictionary, &OMFObject, j+1);
			if (OMF2::OM_ERR_NONE == rc)
			{
				// Process the given Class Dictionary object.
				rc = ConvertOMFClassDictionaryObject(OMFObject);
			}
		}
	}
	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		UTLstdprintf("Converted OMF Header values to AAF\n");

	DecIndentLevel();
	return rc;
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
HRESULT AafOmf::ConvertOMFDataDefinitionObject( OMF2::omfObject_t obj )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfClassID_t		objClass;
	char					id[5];

	IncIndentLevel();
	rc = OMF2::omfsReadClassID(OMFFileHdl, obj, OMF2::OMOOBJObjClass, objClass);
	if (OMF2::OM_ERR_NONE == rc && bVerboseMode)
	{
		strncpy(id, objClass, 4);
		id[4] = '\0';
		UTLstdprintf("%sProcessing: %s Data Definition\n", indentLeader, id );
	}

	DecIndentLevel();
	return rc;
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
HRESULT AafOmf::ConvertOMFClassDictionaryObject( OMF2::omfObject_t obj )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	char					id[5];

	IncIndentLevel();
	memset(id, 0, sizeof(id));
	rc = OMF2::omfsReadClassID(OMFFileHdl, obj, OMF2::OMCLSDClass, id);
	if ( OMF2::OM_ERR_PROP_NOT_PRESENT == rc )
		UTLerrprintf("*** Invalid Class Id ***\n");
	else if (bVerboseMode)
		UTLstdprintf("%sProcessing: %s Class Definition\n", indentLeader, id);

	DecIndentLevel();
	return rc;
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
HRESULT AafOmf::ConvertOMFMediaDataObject( OMF2::omfObject_t obj )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfUID_t			mediaID;
	OMF2::omfProperty_t		idProperty;
	OMF2::omfDDefObj_t		datakind;
	char					id[5];

	IAAFEssenceData*		pEssenceData = NULL;
	IAAFMob*				pMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	aafUID_t				mobID;
	aafBool					bConvertMedia = AAFFalse;

	IncIndentLevel();
	memset(id, 0, sizeof(id));
	rc = OMF2::omfsReadUID(OMFFileHdl, obj, OMF2::OMMDATMobID, &mediaID);
	rc = aafMobIDFromMajorMinor(mediaID.major, mediaID.minor, &mobID);
	rc = pHeader->LookupMob(&mobID, &pMob);
	if (FAILED(rc))
		return rc;

	rc = pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob);
	rc = OMF2::omfsReadClassID(OMFFileHdl, obj, OMF2::OMOOBJObjClass, id);
	if (SUCCEEDED(rc))
	{
		if (bVerboseMode)
			UTLstdprintf("%sProcessing: %s Media Data\n", indentLeader, id);
		if (strncmp(id, "TIFF", 4) == 0)
		{
			// handle TIFF media data
			IAAFTIFFData*	pTIFFData = NULL;
			rc = pDictionary->CreateInstance(&AUID_AAFTIFFData,
											 IID_IAAFTIFFData,
											 (IUnknown **)&pTIFFData);
			rc = pTIFFData->QueryInterface(IID_IAAFEssenceData, (void **)&pEssenceData);
			rc = pEssenceData->SetFileMob(pSourceMob);
			rc = pHeader->AppendEssenceData(pEssenceData);
			idProperty = OMF2::OMTIFFData;
			bConvertMedia = AAFTrue;
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&rc);

		}
		else if (strncmp(id, "AIFC", 4) == 0)
		{
			// Handle Audio (Wave data)
			IAAFWAVEData*	pWAVEData = NULL;
			rc = pDictionary->CreateInstance(&AUID_AAFWAVEData,
											 IID_IAAFWAVEData,
											 (IUnknown **)&pWAVEData);
			rc = pWAVEData->QueryInterface(IID_IAAFEssenceData, (void **)&pEssenceData);
			rc = pEssenceData->SetFileMob(pSourceMob);
			rc = pHeader->AppendEssenceData(pEssenceData);
			idProperty = OMF2::OMAIFCData;
			OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Sound", &datakind, (OMF2::omfErr_t *)&rc);
			bConvertMedia = AAFTrue;
		}
		else
		{
			// rest of media (essence) NOT implemented)
			IncIndentLevel();
			UTLstdprintf("%sThis conversion is not implemented yet !!\n", indentLeader);
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
			aafBool				bMore = AAFFalse;
			

			// find out how big the data is 
			numBytes = (aafUInt32)OMF2::omfsLengthDataValue(OMFFileHdl, obj, idProperty);
			if (numBytes > (2 * 1048576))
			{
				nBlockSize = 2 * 1048576;		// only allocate 2 Meg
				bMore = AAFTrue;			// you going to need more than one read/write
			}
			else
			{
				nBlockSize = numBytes;
			}
			rc = UTLMemoryAlloc(nBlockSize, &pBuffer);
			OMFOffset = 0;
			do 
			{
				rc = OMF2::omfsReadDataValue(OMFFileHdl, 
											 obj,
											 idProperty,
											 datakind,
											 (OMF2::omfDataValue_t)pBuffer,
											 OMFOffset,
											 nBlockSize,
											 &numBytesRead);

				// write the data
				rc = pEssenceData->SetPosition((aafPosition_t) OMFOffset);
				rc = pEssenceData->Write(numBytesRead, (aafDataBuffer_t)pBuffer, &numBytesWritten);

				// calculate next offset
				OMFOffset += numBytesRead;

			}while (numBytes > OMFOffset );
			// Free the allocated buffer 
			UTLMemoryFree(pBuffer);
		}
	}
	if (pEssenceData)
	{
		pEssenceData->Release();
		pEssenceData = NULL;
	}
	DecIndentLevel();
	pMob->Release();
	pSourceMob->Release();
	return rc;
}
// ============================================================================
// ConvertOMFDatakind
//
//			This function converts an OMF datakind into an AAF datadef. 
//			
// Returns: AAFRESULT_SUCCESS if datakind is converted succesfully
//
// ============================================================================
HRESULT AafOmf::ConvertOMFDatakind( OMF2::omfDDefObj_t datakind, 
								   aafUID_t* pDatadef)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfUniqueName_t	datakindName;
	
	rc = OMF2::omfiDatakindGetName(OMFFileHdl, datakind, 64, datakindName);
	if (strncmp("omfi:data:Picture", datakindName, 17)== 0)
		*pDatadef = DDEF_Video;
	else if (strncmp("omfi:data:Sound", datakindName, 15) == 0)
		*pDatadef = DDEF_Audio;
	else if(strncmp("omfi:data:Timecode", datakindName, 18) == 0)
		*pDatadef = DDEF_Timecode;
	else
	{
		UTLstdprintf("Invalid DataDef :%s Found in sequence\n", datakindName);
		UTLerrprintf("Invalid DataDef :%s Found in sequence\n", datakindName);
		rc = AAFRESULT_INVALID_DATADEF;
	}

	return rc;
}
// ============================================================================
// ConvertOMFMOBObject
//
//			This function is converts all the mob basic data (name, MobId, etc.) 
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT AafOmf::ConvertOMFMOBObject( OMF2::omfObject_t obj, IAAFMob* pMob )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	char					sMobName[32];
	aafWChar*				pwMobName = NULL;
	aafInt32				numComments;
	aafInt32				times;

	OMF2::omfUID_t			OMFMobID;
	OMF2::omfIterHdl_t		OMFIterator;

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

	IncIndentLevel();
	rc = OMF2::omfiMobGetInfo(OMFFileHdl, obj, &OMFMobID, 32, sMobName, NULL, NULL);
	if (AAFRESULT_SUCCESS == rc)
		UTLStrAToStrW(sMobName, &pwMobName);
	else
		UTLStrAToStrW("Name NOT provided", &pwMobName);

	if (bVerboseMode)
		UTLstdprintf("%sMob Name: %s", indentLeader, sMobName);

	// Set Name
	pMob->SetName(pwMobName);

	// Convert OMF MobID into AAF AUID
	aLabel.smpte.MobIDMajor = OMFMobID.major;
	aLabel.smpte.MobIDMinor = OMFMobID.minor;
		
	aLabel.smpte.oid = 0x06;
	aLabel.smpte.size = 0x0E;
	aLabel.smpte.ulcode = 0x2B;
	aLabel.smpte.SMPTE = 0x34;
	aLabel.smpte.Registry = 0x02;
	aLabel.smpte.unused = 0;
	aLabel.smpte.MobIDPrefix = 42;		// Means its an OMF Uid

	// Set MobID
	pMob->SetMobID(&aLabel.auid);

	// Set comments
	rc = OMF2::omfiMobGetNumComments(OMFFileHdl, obj, &numComments);
	if (AAFRESULT_SUCCESS == rc && numComments > 0)
	{
		// Allocate the iterator
		OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
		for (times = 0; times < numComments; times++)
		{
			char		sCommentName[64];
			aafWChar*	pwCommentName = NULL;
			char		sCommentValue[256];
			aafWChar*	pwCommentValue = NULL;

			rc = OMF2::omfiMobGetNextComment(OMFIterator, obj, sizeof(sCommentName), sCommentName, sizeof(sCommentValue), sCommentValue);
			if (AAFRESULT_SUCCESS == rc)
			{
				UTLStrAToStrW(sCommentName, &pwCommentName);
				UTLStrAToStrW(sCommentValue, &pwCommentValue);
				pMob->AppendComment(pwCommentName, pwCommentValue);
				if (pwCommentName)
					UTLMemoryFree(pwCommentName);
				if (pwCommentValue)
					UTLMemoryFree(pwCommentValue);
			}
		}

		// Release the iterator
		OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
	}

	if (pwMobName)
		UTLMemoryFree(pwMobName);

	DecIndentLevel();
	return rc;
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
HRESULT AafOmf::ConvertOMFCompositionObject(OMF2::omfObject_t obj, 
											IAAFCompositionMob* pCompMob)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfDefaultFade_t	OMFDefaultFade;

	aafDefaultFade_t		AAFDefaultFade;
	
	// get Composition mob information
	rc = OMF2::omfiMobGetDefaultFade(OMFFileHdl, obj, &OMFDefaultFade);
	if (AAFRESULT_SUCCESS == rc && OMFDefaultFade.valid)
	{
		AAFDefaultFade.fadeLength   = OMFDefaultFade.fadeLength;
		switch(OMFDefaultFade.fadeType)
		{
			case OMF2::kFadeNone: 
				AAFDefaultFade.fadeType = kFadeNone;
				break;
			case OMF2::kFadeLinearAmp:
				AAFDefaultFade.fadeType = kFadeLinearAmp;
				break;
			case OMF2::kFadeLinearPower:
				AAFDefaultFade.fadeType = kFadeLinearPower;
				break;
		}
		AAFDefaultFade.fadeEditUnit.numerator = OMFDefaultFade.fadeEditUnit.numerator;
		AAFDefaultFade.fadeEditUnit.denominator = OMFDefaultFade.fadeEditUnit.denominator;
	}

	// Set default fade values
	if (OMFDefaultFade.valid)
		pCompMob->SetDefaultFade(AAFDefaultFade.fadeLength,
								 AAFDefaultFade.fadeType,
								 AAFDefaultFade.fadeEditUnit);

	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		UTLstdprintf("Converted OMF Composition MOB to AAF\n");

	return rc;
}
// ============================================================================
// TraverseOMFMob
//
//			This function converts all the mob slots in the given OMF mob object 
//			
// Returns: AAFRESULT_SUCCESS if succesfully
//
// ============================================================================
HRESULT AafOmf::TraverseOMFMob( OMF2::omfObject_t obj, IAAFMob* pMob )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	aafInt32				times;
	aafInt32				numSlots;
	aafInt32				numTracks;

	OMF2::omfIterHdl_t		OMFIterator;
	OMF2::omfMSlotObj_t		OMFSlot;
	OMF2::omfMSlotObj_t		OMFSegment;
	OMF2::omfTrackID_t		OMFTrackID;
	OMF2::omfRational_t		OMFeditRate;
	OMF2::omfPosition_t		OMFOrigin;
	OMF2::omfErr_t			OMFError;
	char					sTrackName[32];
	aafWChar*				pwTrackName = NULL;					

	IAAFMobSlot*			pMobSlot = NULL;
	IAAFSegment*			pSegment = NULL;
	IAAFComponent*			pComponent = NULL;
	IAAFTimelineMobSlot*	pTimelineMobSlot = NULL;

	rc = OMF2::omfiMobGetNumSlots(OMFFileHdl, obj, &numSlots);
	if (AAFRESULT_SUCCESS != rc)
		numSlots = 0;

	rc = OMF2::omfiMobGetNumTracks(OMFFileHdl, obj, &numTracks);
	if (AAFRESULT_SUCCESS != rc)
		numTracks = 0;

	IncIndentLevel();
	if (bVerboseMode)
	{
		UTLstdprintf("%sFound: %ld sub tracks\n", indentLeader, numSlots);
	}
	OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
	for (times = 0; times < numSlots; times++)
	{
		rc = OMF2::omfiMobGetNextSlot(OMFIterator, obj, NULL, &OMFSlot);
		if (AAFRESULT_SUCCESS == rc)
		{
			rc = OMF2::omfiMobSlotGetInfo(OMFFileHdl, OMFSlot, &OMFeditRate, &OMFSegment);
			if (OMF2::omfiMobSlotIsTrack(OMFFileHdl, OMFSlot, &OMFError))
			{
				rc = OMF2::omfiTrackGetInfo(OMFFileHdl, obj, OMFSlot, &OMFeditRate, sizeof(sTrackName),
								sTrackName, &OMFOrigin, &OMFTrackID, &OMFSegment);
				if (AAFRESULT_SUCCESS == rc)
				{
					ProcessOMFComponent(OMFSegment, &pComponent);
					rc = pComponent->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
					if (pSegment)
					{
						IncIndentLevel();
						UTLStrAToStrW(sTrackName, &pwTrackName);
						pMob->AppendNewSlot( pSegment, (aafSlotID_t)OMFTrackID, pwTrackName, &pMobSlot );
						if (bVerboseMode)
						{
							UTLstdprintf("%sConverted SlotID: %d, Name: %s\n",indentLeader, (int)OMFTrackID, sTrackName);
						}
						DecIndentLevel();
					}
					if (pSegment)
					{
						pSegment->Release();
						pSegment = NULL;
					}
				}
			}
			
		}
	}

	OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
	DecIndentLevel();
	if (pwTrackName)
		UTLMemoryFree(pwTrackName);

	return rc;
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
HRESULT AafOmf::ConvertOMFSelector( OMF2::omfObject_t selector, IAAFSelector* pSelector )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfLength_t		OMFLength;
	OMF2::omfDDefObj_t		OMFDatakind;
	OMF2::omfSegObj_t		OMFSelected;
	OMF2::omfNumSlots_t		numAlternates;

	IAAFComponent*			pComponent = NULL;
	IAAFSegment*			pSegment = NULL;
	aafUID_t				datadef ;

	rc = OMF2::omfiSelectorGetInfo( OMFFileHdl, 
									selector,
									&OMFDatakind,
									&OMFLength,
									&OMFSelected);

	if (SUCCEEDED(rc))
	{        
		rc = ConvertOMFDatakind(OMFDatakind, &datadef);
		pSelector->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
		pComponent->SetDataDef(&datadef);
		pComponent->SetLength((aafLength_t *)&OMFLength);
		pComponent->Release();
		pComponent = NULL;
		if (bVerboseMode)
			UTLstdprintf("%sProcessing Selector object of length = %ld\n", indentLeader, (int)OMFLength);
		rc = ProcessOMFComponent(OMFSelected, &pComponent);
		if (SUCCEEDED(rc))
		{
			rc = pComponent->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
			rc = pSelector->SetSelectedSegment(pSegment);
			rc = OMF2::omfiSelectorGetNumAltSlots(OMFFileHdl,
												  selector,
												  &numAlternates);
			if (SUCCEEDED(rc) && numAlternates > 0)
			{
				OMF2::omfIterHdl_t		OMFIterator;
				int						i;
				OMF2::omfSegObj_t		OMFAltSelected;

				IAAFComponent*			pAltComponent = NULL;
				IAAFSegment*			pAltSegment = NULL;

				OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
				IncIndentLevel();
				for (i = 0; i <(int)numAlternates;i++)
				{
					rc = OMF2::omfiSelectorGetNextAltSlot(OMFIterator,
														  selector,
														  NULL, 
														  &OMFAltSelected);
					rc = ProcessOMFComponent(OMFAltSelected, &pAltComponent);
					if (SUCCEEDED(rc))
					{
						rc = pAltComponent->QueryInterface(IID_IAAFSegment, (void **)&pAltSegment);
						rc = pSelector->AppendAlternateSegment(pAltSegment);
						pAltSegment->Release();
					}
					pAltComponent->Release();
				}
				DecIndentLevel();
				OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
			}
			pSegment->Release();
			pComponent->Release();
		}
	}
	return rc;
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
//	OUTPUTS:	pSegment	new AAF Segment	
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT AafOmf::ProcessOMFComponent(OMF2::omfObject_t OMFSegment, IAAFComponent** ppComponent)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfEdgecode_t		OMFEdgecode;
	OMF2::omfLength_t		OMFLength;
	OMF2::omfDDefObj_t		OMFDatakind;
	OMF2::omfTimecode_t		OMFTimecode;
	OMF2::omfEffObj_t		OMFEffect;
	OMF2::omfPosition_t		OMFCutPoint;

	IAAFSequence*			pSequence = NULL;
	IAAFSourceClip*			pSourceClip = NULL;
	IAAFTimecode*			pTimecode = NULL;
	IAAFEdgecode*			pEdgecode = NULL;
	IAAFFiller*				pFiller = NULL;
	IAAFTransition*			pTransition = NULL;
	IAAFSelector*			pSelector = NULL;
	IAAFGroup*				pEffect = NULL;
	aafEdgecode_t			edgecode;
	aafTimecode_t			timecode;
	aafUID_t				datadef ;


	// First get sequence information
	IncIndentLevel();
	if (OMF2::omfiIsASequence(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		rc = pDictionary->CreateInstance(&AUID_AAFSequence,
										 IID_IAAFSequence,
										 (IUnknown **)&pSequence);
		pSequence->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		if (bVerboseMode)
			UTLstdprintf("%sProcessing Sequence\n", indentLeader);

		ConvertOMFSequence(OMFSegment, pSequence);
		TraverseOMFSequence(OMFSegment, pSequence);
		pSequence->Release();
	}

	else if (OMF2::omfiIsASourceClip(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		rc = pDictionary->CreateInstance(&AUID_AAFSourceClip,
										 IID_IAAFSourceClip,
										 (IUnknown **)&pSourceClip);
		rc = ConvertOMFSourceClip(OMFSegment, pSourceClip);
		pSourceClip->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		if (bVerboseMode)
			UTLstdprintf("%sProcessing SourceClip\n", indentLeader);
	}
	else if (OMF2::omfiIsATimecodeClip(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		// Get Timecode information
		OMF2::omfiTimecodeGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength, &OMFTimecode);
		timecode.startFrame = OMFTimecode.startFrame;
		timecode.drop = (aafDropType_t)OMFTimecode.drop;
		timecode.fps  = OMFTimecode.fps;
		if (bVerboseMode)
		{
			UTLstdprintf("%sProcessing Timecode\n", indentLeader);
			IncIndentLevel();
			UTLstdprintf("%slength\t\t: %ld\n", indentLeader, (int)OMFLength);
			UTLstdprintf("%sstart Frame\t: %ld\n", indentLeader, timecode.startFrame);
			if (timecode.drop == AAFTrue)
				UTLstdprintf("%sdrop\t\t: True\n", indentLeader);
			else
				UTLstdprintf("%sdrop\t\t: False\n", indentLeader);
			UTLstdprintf("%sFrames/second\t: %ld\n", indentLeader, timecode.fps);     
			DecIndentLevel();				
		}

		rc = pDictionary->CreateInstance(&AUID_AAFTimecode,
										 IID_IAAFTimecode,
										 (IUnknown **)&pTimecode);

		pTimecode->Initialize((aafLength_t)OMFLength, &timecode);
		pTimecode->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (OMF2::omfiIsAnEdgecodeClip(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		// Get edgecode data
		OMF2::omfiEdgecodeGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength, &OMFEdgecode);
		edgecode.startFrame = OMFEdgecode.startFrame;
		edgecode.filmKind = (aafFilmType_t)OMFEdgecode.filmKind;
		edgecode.codeFormat = (aafEdgeType_t)OMFEdgecode.codeFormat;
		for (int ii=0;ii<sizeof(edgecode.header);ii++)
			edgecode.header[ii] = OMFEdgecode.header[ii];

		if (bVerboseMode)
		{
			UTLstdprintf("%sProcessing Edgecode\n ", indentLeader);
			IncIndentLevel();
			UTLstdprintf("%slength\t\t: %ld\n", indentLeader, (int)OMFLength);
			UTLstdprintf("%sstart Frame\t: %ld\n", indentLeader, edgecode.startFrame);
			DecIndentLevel();				
		}
		rc = pDictionary->CreateInstance(&AUID_AAFEdgecode,
										 IID_IAAFEdgecode,
										 (IUnknown **)&pEdgecode);

		pEdgecode->Create((aafLength_t)OMFLength, edgecode);
		pEdgecode->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
	}
	else if (OMF2::omfiIsAFiller(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		rc = OMF2::omfiFillerGetInfo(OMFFileHdl, OMFSegment, &OMFDatakind, &OMFLength);
		rc = ConvertOMFDatakind( OMFDatakind, &datadef);
		if (SUCCEEDED(rc))
		{
			if (bVerboseMode)
			{
				UTLstdprintf("%sProcessing Filler of length: %ld\n ", indentLeader, (int)OMFLength);
			}
			rc = pDictionary->CreateInstance(&AUID_AAFFiller,
											  IID_IAAFFiller,
											  (IUnknown **) &pFiller);
			rc = pFiller->Initialize( &datadef, (aafLength_t)OMFLength);
			rc = pFiller->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		}
	}
	else if (OMF2::omfiIsAnEffect(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			UTLstdprintf("%sProcessing Effect\n ", indentLeader);
		}
	}
	else if (OMF2::omfiIsATransition(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		rc = OMF2::omfiTransitionGetInfo(OMFFileHdl, 
										 OMFSegment,
										 &OMFDatakind, 
										 &OMFLength, 
										 &OMFCutPoint,
										 &OMFEffect);
		rc = ConvertOMFDatakind( OMFDatakind, &datadef);
		if (SUCCEEDED(rc))
		{
			if (bVerboseMode)
			{
				UTLstdprintf("%sProcessing Transition of length: %ld\n ", indentLeader, (int)OMFLength);
			}
			rc = pDictionary->CreateInstance(&AUID_AAFTransition,
											  IID_IAAFTransition,
											  (IUnknown **) &pTransition);

			rc = pDictionary->CreateInstance(&AUID_AAFGroup,
											  IID_IAAFGroup,
											  (IUnknown **) &pEffect);

			rc = pTransition->Create(&datadef, (aafLength_t)OMFLength, (aafPosition_t)OMFCutPoint, pEffect);
			rc = pTransition->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		}
	}
	else if (OMF2::omfiIsAConstValue(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			UTLstdprintf("%sProcessing Constant Value\n ", indentLeader);
		}
	}
	else if (OMF2::omfiIsAVaryValue(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			UTLstdprintf("%sProcessing Varying Value\n ", indentLeader);
		}
	}
	else if (OMF2::omfiIsANestedScope(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			UTLstdprintf("%sProcessing Nested Scope\n ", indentLeader);
		}
	}
	else if (OMF2::omfiIsAScopeRef(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			UTLstdprintf("%sProcessing Scope Reference\n ", indentLeader);
		}
	}
	else if (OMF2::omfiIsASelector(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		rc = pDictionary->CreateInstance(&AUID_AAFSelector,
										  IID_IAAFSelector,
										  (IUnknown **) &pSelector);
		IncIndentLevel();
		rc = ConvertOMFSelector(OMFSegment, pSelector);
		rc = pSelector->QueryInterface(IID_IAAFComponent, (void **)ppComponent);
		pSelector->Release();
		DecIndentLevel();
	}
	else if (OMF2::omfiIsAMediaGroup(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			UTLstdprintf("%sProcessing Media Group\n ", indentLeader);
		}
	}
	else
	{
		char	classID[5];

		rc = OMF2::omfsReadClassID(OMFFileHdl, OMFSegment, OMF2::OMOOBJObjClass,classID);
		classID[4] = '\0';
		if (bVerboseMode)
		{
			UTLerrprintf("%s*** UNKNOWN OBJECT : %s\n", indentLeader, classID);
		}
	}

	DecIndentLevel();
	return rc;
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
HRESULT AafOmf::ConvertOMFSequence(OMF2::omfObject_t sequence, 
								   IAAFSequence* pSequence )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfDDefObj_t		datakind = NULL;
	OMF2::omfLength_t		sequLength = 0;

	IAAFComponent*			pComponent = NULL;
	aafUID_t				datadef;
	
	aafBool					bValid = AAFFalse;

	// Get a pointer to a component interface
	rc = pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	if (SUCCEEDED(rc))
	{
		// Get Sequence data kind 
		rc = OMF2::omfiSequenceGetInfo(OMFFileHdl, sequence, &datakind, &sequLength);
		rc = ConvertOMFDatakind(datakind, &datadef);
		if (SUCCEEDED(rc))
			pComponent->SetDataDef(&datadef);
		pComponent->SetLength((aafLength_t *)&sequLength);
		// Next - get all properties
		rc = ConvertOMFComponentProperties(sequence, pComponent);
		pComponent->Release();
	}

	return rc;
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
HRESULT AafOmf::ConvertOMFSourceClip(OMF2::omfObject_t sourceclip, 
									 IAAFSourceClip* pSourceClip )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfDDefObj_t		datakind = NULL;
	OMF2::omfLength_t		clipLength = 0;
	OMF2::omfSourceRef_t	OMFSourceRef;
	OMF2::omfFadeType_t		OMFFadeinType;
	OMF2::omfFadeType_t		OMFFadeoutType;
	OMF2::omfBool			fadeinPresent, fadeoutPresent;

	IAAFComponent*			pComponent = NULL;
	aafUID_t				datadef;
	aafSourceRef_t			sourceRef;
	aafFadeType_t			fadeinType, fadeoutType;
	aafInt32				fadeinLen, fadeoutLen;


	rc = OMF2::omfiSourceClipGetInfo(OMFFileHdl, sourceclip, &datakind, &clipLength, &OMFSourceRef);
	if (FAILED(rc))
		return rc;
	rc = OMF2::omfiSourceClipGetFade(OMFFileHdl, sourceclip, 
									 &fadeinLen, &OMFFadeinType, &fadeinPresent,
									 &fadeoutLen, &OMFFadeoutType, &fadeoutPresent);
	if (SUCCEEDED(rc))
	{
		ConvertOMFDatakind(datakind, &datadef);
		rc = aafMobIDFromMajorMinor(OMFSourceRef.sourceID.major,
									OMFSourceRef.sourceID.minor,
									&sourceRef.sourceID);
		sourceRef.sourceSlotID = OMFSourceRef.sourceTrackID;
		sourceRef.startTime    = OMFSourceRef.startTime;
		fadeinType = (aafFadeType_t)OMFFadeinType;
		fadeoutType = (aafFadeType_t)OMFFadeoutType;
		rc = pSourceClip->Initialize(&datadef, (aafLength_t *)&clipLength, sourceRef);
		if (fadeinPresent || fadeoutPresent)
			rc = pSourceClip->SetFade(fadeinLen, fadeinType, fadeoutLen, fadeoutType);
	}	
	return rc;
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
HRESULT AafOmf::ConvertOMFComponentProperties(OMF2::omfObject_t component, 
											 IAAFComponent* pComponent )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfProperty_t		Property;
	OMF2::omfUniqueName_t	propertyName;
	OMF2::omfIterHdl_t		propertyIterator = NULL;
	OMF2::omfType_t			propertyType = OMF2::OMNoType;
	OMF2::omfDDefObj_t		datakind = NULL;

	OMF2::omfiIteratorAlloc(OMFFileHdl, &propertyIterator);
	OMF2::omfiGetNextProperty(propertyIterator, component, &Property, &propertyType);
	while((OMF2::OM_ERR_NONE == rc) &&Property)
	{
		switch (Property)
		{
			case OMF2::OMCPNTLength:
			case OMF2::OMCPNTDatakind:
			case OMF2::OMCPNTTrackKind:
			case OMF2::OMCPNTEditRate:
			case OMF2::OMCPNTName:
			case OMF2::OMCPNTEffectID:
			case OMF2::OMCPNTAttributes:
			case OMF2::OMOOBJObjClass:
			default:
				OMF2::omfiGetPropertyName(OMFFileHdl, Property, 64, propertyName);
				UTLstdprintf("%sComponent Property NOT converted : %s\n", indentLeader, propertyName);
				break;
		}
		rc = OMF2::omfiGetNextProperty(propertyIterator, component, &Property, &propertyType);
	}
	rc = OMF2::omfiIteratorDispose(OMFFileHdl, propertyIterator);

	return rc;
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
HRESULT AafOmf::TraverseOMFSequence(OMF2::omfObject_t sequence, IAAFSequence* pSequence )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfIterHdl_t		componentIterator = NULL;
	OMF2::omfPosition_t		sequPos;
	OMF2::omfCpntObj_t		sequComponent;

	IAAFComponent*			pComponent = NULL;

	aafInt32				numComponents = 0;
	aafInt32				cpntCount;

	OMF2::omfiIteratorAlloc(OMFFileHdl, &componentIterator);
	rc = OMF2::omfiSequenceGetNumCpnts(OMFFileHdl, sequence, &numComponents);
	if (SUCCEEDED(rc))
	{
		for (cpntCount = 0; cpntCount < numComponents; cpntCount++)
		{   
			rc = OMF2::omfiSequenceGetNextCpnt(componentIterator, sequence, NULL, 
				                               &sequPos, &sequComponent); 
			if (SUCCEEDED(rc))
			{
				rc = ProcessOMFComponent(sequComponent, &pComponent);
				if (pComponent)
				{
					rc = pSequence->AppendComponent(pComponent);
					pComponent->Release();
					pComponent = NULL;
				}
			}
		}
	}

	if (pComponent)
		pComponent->Release();

	DecIndentLevel();
	OMF2::omfiIteratorDispose(OMFFileHdl, componentIterator);
	return rc;
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
HRESULT AafOmf::ConvertOMFMasterMob(OMF2::omfObject_t obj,
									IAAFMasterMob* pMasterMob )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	
	if (bVerboseMode)
		UTLstdprintf("Converting OMF Master MOB to AAF\n");
	return rc;
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
HRESULT AafOmf::ConvertOMFSourceMob(OMF2::omfObject_t obj,
									IAAFSourceMob* pSourceMob)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfObject_t		mediaDescriptor;

	IAAFEssenceDescriptor*	pEssenceDesc = NULL;
	IAAFTapeDescriptor*		pTapeDesc = NULL;
	IAAFFileDescriptor*		pFileDesc = NULL;
	IAAFTIFFDescriptor*		pTiffDesc = NULL;	
	IAAFAIFCDescriptor*		pAifcDesc = NULL;
	IAAFUnixLocator*		pUnixLocator = NULL;
	IAAFLocator*			pLocator = NULL;

	if (bVerboseMode)
		UTLstdprintf("Converting OMF Source MOB to AAF\n");

	IncIndentLevel();
	rc = OMF2::omfmMobGetMediaDescription(OMFFileHdl, obj, &mediaDescriptor);
	if (SUCCEEDED(rc))
	{
		if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassMDTP, (OMF2::omfErr_t *)&rc))
		{
			OMF2::omfTapeCaseType_t		formFactor;
			OMF2::omfVideoSignalType_t	videoSignal;
			OMF2::omfTapeFormatType_t	tapeFormat;
			OMF2::omfLength_t			length;
			char						manufacturer[64], model[64];
			OMF2::omfInt32				manuSize, modelSize;

			aafWChar*					pwManufacturer = NULL;
			aafWChar*					pwModel = NULL;

			rc = pDictionary->CreateInstance(&AUID_AAFTapeDescriptor,
											 IID_IAAFTapeDescriptor,
											 (IUnknown **)&pTapeDesc);
			if (SUCCEEDED( rc) )
			{
				manuSize = modelSize = 64;
				rc = OMF2::omfmTapeMobGetDescriptor(OMFFileHdl, 
													obj,
													&formFactor,
													&videoSignal,
													&tapeFormat,
													&length,
													manuSize, manufacturer,
													modelSize, model);
				if (SUCCEEDED(rc))
				{
					if (strlen(manufacturer) > 0)
					{
						UTLStrAToStrW(manufacturer, &pwManufacturer);
						pTapeDesc->SetTapeManufacturer(pwManufacturer);
						UTLMemoryFree(pwManufacturer);
					}
					if (strlen(model) > 0)
					{
						UTLStrAToStrW(model, &pwModel);
						pTapeDesc->SetTapeModel(pwModel);
						UTLMemoryFree(pwModel);
					}
					pTapeDesc->SetTapeFormFactor((aafTapeCaseType_t)formFactor);
					pTapeDesc->SetSignalType((aafVideoSignalType_t)videoSignal);
					pTapeDesc->SetTapeFormat((aafTapeFormatType_t)tapeFormat);
					pTapeDesc->SetTapeLength((aafLength_t)length);
				}
				rc = pTapeDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
				pSourceMob->SetEssenceDescriptor(pEssenceDesc);
				if (bVerboseMode)
					UTLstdprintf("%sAdded a Tape Essence Descriptor to a Source MoB\n", indentLeader);
				if (pTapeDesc)
				{
					pTapeDesc->Release();
					pTapeDesc = NULL;
				}
			}
		}
		else if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassTIFD, (OMF2::omfErr_t *)&rc) )
		{
			OMF2::omfBool			IsContiguous, IsUniform;
			OMF2::omfInt32			leadingLines, trailingLines;
			char					summary[1024];
			OMF2::omfJPEGTableID_t	data;
			OMF2::omfDDefObj_t		datakind;
			
			aafUInt32				bytesRead;

			rc = pDictionary->CreateInstance(&AUID_AAFTIFFDescriptor,
											 IID_IAAFTIFFDescriptor,
											 (IUnknown **)&pTiffDesc);
			if (SUCCEEDED( rc) )
			{

				rc = OMF2::omfsReadBoolean( OMFFileHdl,
											mediaDescriptor,
											OMF2::OMTIFDIsContiguous, &IsContiguous);
				rc = OMF2::omfsReadBoolean( OMFFileHdl,
											mediaDescriptor,
											OMF2::OMTIFDIsUniform,
											&IsUniform);
				rc = OMF2::omfsReadInt32(OMFFileHdl,
										 mediaDescriptor,
										 OMF2::OMTIFDLeadingLines, 
										 &leadingLines);
				rc = OMF2::omfsReadInt32(OMFFileHdl,
										 mediaDescriptor,
										 OMF2::OMTIFDTrailingLines, 
										 &trailingLines);
				OMF2::omfiDatakindLookup(OMFFileHdl, "omfi:data:Picture", &datakind, (OMF2::omfErr_t *)&rc);
				rc = OMF2::omfsReadDataValue(OMFFileHdl,
											 mediaDescriptor,
											 OMF2::OMTIFDSummary,
											 datakind,
											 summary,
											 0,
											 sizeof(summary),
											 &bytesRead);
				rc = OMF2::omfsReadJPEGTableIDType( OMFFileHdl,
													mediaDescriptor,
													OMF2::OMTIFDJPEGTableID, 
													&data);

				rc = pTiffDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
				pTiffDesc->SetIsUniform((aafBool)IsUniform);
				pTiffDesc->SetIsContiguous((aafBool)IsContiguous);
				pTiffDesc->SetLeadingLines((aafInt32)leadingLines);
				pTiffDesc->SetTrailingLines((aafInt32)trailingLines);
				pTiffDesc->SetJPEGTableID((aafJPEGTableID_t)data);
				pTiffDesc->SetSummary((aafUInt32)bytesRead, (aafDataValue_t) summary);
				pSourceMob->SetEssenceDescriptor(pEssenceDesc);
				if (bVerboseMode)
					UTLstdprintf("%sAdded a TIFF Essence Descriptor to a Source MoB\n", indentLeader);
				if (pTiffDesc)
				{
					pTiffDesc->Release();
					pTiffDesc = NULL;
				}
			}
		}
		else if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassAIFD, (OMF2::omfErr_t *)&rc) )
		{
			rc = pDictionary->CreateInstance(&AUID_AAFAIFCDescriptor,
											 IID_IAAFAIFCDescriptor,
											 (IUnknown **)&pAifcDesc);
			if (SUCCEEDED( rc) )
			{
				rc = pAifcDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
				pSourceMob->SetEssenceDescriptor(pEssenceDesc);
				if (bVerboseMode)
					UTLstdprintf("%sAdded a AIFC Essence Descriptor to a Source MoB\n", indentLeader);
				if (pAifcDesc)
				{
					pAifcDesc->Release();
					pAifcDesc = NULL;
				}
			}
		}
		else if ( OMF2::omfsIsTypeOf(OMFFileHdl, mediaDescriptor, OMClassUNXL, (OMF2::omfErr_t *)&rc) )
		{
			rc = pDictionary->CreateInstance(&AUID_AAFFileDescriptor,
											 IID_IAAFFileDescriptor,
											 (IUnknown **)&pFileDesc);
			if (SUCCEEDED( rc) )
			{
				rc = pFileDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDesc);
				rc = pDictionary->CreateInstance(&AUID_AAFUnixLocator,
												 IID_IAAFUnixLocator,
												 (IUnknown **)&pUnixLocator);
				rc = pUnixLocator->QueryInterface(IID_IAAFLocator, (void **)&pLocator);
				pLocator->SetPath(L"fake.aaf");
				pEssenceDesc->AppendLocator(pLocator);
				pSourceMob->SetEssenceDescriptor(pEssenceDesc);
				if (bVerboseMode)
					UTLstdprintf("%sAdded a File Descriptor with a UNIX locator, to a Source MoB\n", indentLeader);
				if (pFileDesc)
				{
					pFileDesc->Release();
					pFileDesc = NULL;
				}
				if (pUnixLocator)
				{
					pUnixLocator->Release();
					pUnixLocator = NULL;
				}
			}
		}

	}
	if (pEssenceDesc)
		pEssenceDesc->Release();

	if (pLocator)
		pLocator->Release();

	DecIndentLevel();
	return rc;
}

void AafOmf::OMFFileClose()
{
	OMF2::omfsCloseFile(OMFFileHdl);
	OMF2::omfsEndSession(OMFSession);
}

void AafOmf::AAFFileClose( )
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
// MAIN Module 
//
//		This is the application's main controlling routine.  
//
// ============================================================================
int main(int argc, char *argv[])
{
	CComInitialize	comInit;
	AafOmf			theApp;

	HRESULT			rc;



	theApp.pProgramName = baseName(argv[0]);
	rc = UTLInitFileIO();
	if (FAILED(rc))
		return rc;

	UTLstdprintf("%s: Version 0.01.00\n", theApp.pProgramName);
	if (AAFRESULT_SUCCESS == theApp.GetUserInput(argc, argv) )
	{
		if (AAFRESULT_SUCCESS == theApp.OpenInputFile() )
		{
			if (AAFRESULT_SUCCESS == theApp.OpenOutputFile() )
			{
					rc = theApp.ConvertOMFHeader( );
					if (AAFRESULT_SUCCESS == rc)
						rc = theApp.OMFFileRead();
					else
						UTLerrprintf("Could not convert OMF Header Object");
			}
		}
	}
	else
	{
		theApp.Usage();
	}

	return(rc);
}