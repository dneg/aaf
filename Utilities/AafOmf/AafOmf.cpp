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

#include "AafOmf.h"


// ============================================================================
// Global Variables and functions
// ============================================================================

void Indent(int numSpaces)
{
	for (int i = 0; i < numSpaces;i++)
	{
		cout<<" ";
	}
}
void reportAssertionFailure(char* name,
                            char* expressionString,
                            char* fileName,
                            size_t lineNumber)
{
  cerr << "Assertion \"" << name << "\" failed." << endl;
  cerr << "The failure occurred at line "
       << lineNumber << " in file \"" << fileName << "\"." << endl;
  cerr << "The condition \"" << expressionString << "\" was violated." << endl;

  exit(FAILURE);
}

// Assertions are on unless they are explicitly turned off.
//
#if !defined(NODEBUG)
#define ASSERT(expression, name)  \
  (expression) \
    ? (void)0  \
    : reportAssertionFailure(name, #expression,  __FILE__, __LINE__)

#else
#define ASSERT(expression, expression)
#endif

void fatalError(char* routineName, char* message)
{
  cerr << ": Fatal error in routine \"" << routineName << "\". "
       << message << endl;

  exit(FAILURE);
}
// ============================================================================
// Wide char ot char and vice versa conversion overloaded functions
// ============================================================================
static void convert(wchar_t* wcName, size_t length, const char* name)
{
  ASSERT((name && *name), "Valid input name");
  ASSERT(wcName != 0, "Valid output buffer");
  ASSERT(length > 0, "Valid output buffer size");
  
  size_t status = mbstowcs(wcName, name, length);
  if (status == (size_t)-1) {
    fatalError("convert", "Conversion failed.");
  }
}

static void convert(char* cName, size_t length, const wchar_t* name)
{
  ASSERT((name && *name), "Valid input name");
  ASSERT(cName != 0, "Valid output buffer");
  ASSERT(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fatalError("convert", "Conversion failed.");
  }
}

static void convert(char* cName, size_t length, const char* name)
{
  ASSERT((name && *name), "Valid input name");
  ASSERT(cName != 0, "Valid output buffer");
  ASSERT(length > 0, "Valid output buffer size");

  size_t sourceLength = strlen(name);
  if (sourceLength < length - 1) {
    strncpy(cName, name, length);
  } else {
    fatalError("convert", "Conversion failed.");
  }
}

static void convert(wchar_t* wName, size_t length, const wchar_t* name)
{
  ASSERT((name && *name), "Valid input name");
  ASSERT(wName != 0, "Valid output buffer");
  ASSERT(length > 0, "Valid output buffer size");

  size_t sourceLength = 0;
  while (*name)
    ++sourceLength;
  if (sourceLength < length - 1) {
    // Copy the string if there is enough room in the destinition buffer.
    while (*wName++ = *name++)
      ;
  } else {
    fatalError("convert", "Conversion failed.");
  }
}

// ============================================================================
// Delete a file given its wide char name
// ============================================================================
void deleteFile(const wchar_t* fileName)
{
	char cName[256];

	convert(cName, 256, fileName);
	int result = remove(cName);
	//.. ignore error which typically happens if the file doesn;t exist

}
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
	nNumUndefinedOMFObjects(0), nNumUndefinedOMFProperties(0)
{
	memset(sInFileName, 0, sizeof(sInFileName));
	memset(sOutFileName, 0, sizeof(sOutFileName));
	memset(sLogFileName, 0, sizeof(sLogFileName));
	memset(sDefinitionFileName, 0, sizeof(sDefinitionFileName));

}

// ============================================================================
// Destructor
// ============================================================================
AafOmf::~AafOmf()
{
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
	cout << "================================================================================" << endl;
	cout << pProgramName<<" : OMFI to AAF file conversion Version 0.01.00"<< endl;
	cout << "Usage: "<< endl;
	cout << pProgramName<< " [-v] [-s] [-p logfile] [-d deffile] [-t tocfile] <infile> [outfile]"<< endl;
	cout << endl;
	cout << "-v         = Verbose - give progress report (optional)" << endl;
	cout <<	"-s         = Straight conversion. Do NOT discard unnecessary objects (optional)"<<endl;
	cout << "-p logfile = Log file name(optional)" << endl;
	cout << "-d deffile = Definition file (optional)" << endl;
	cout << "-t tocfile = Dump OMFI Table of contents (optional)" << endl;
	cout << "infile     = input file name (required)"<< endl;
	cout << "outfile    = output file name (optional)" << endl;
	cout << "================================================================================" << endl;
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
				cout <<"File : "<<sInFileName<<" is not a valid OMF file"<<endl;
				cout << "Program will exit !"<<endl;
				rc = OMF2::OM_ERR_NOTOMFIFILE;
			}
		}
		else
		{
			cout << "Cannot find input file: "<< sInFileName<< endl;
			cout << "Program will exit !"<<endl;
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
		cout <<"Previous file : "<<sOutFileName<<" will be overwritten"<<endl;
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
		cout << "OMFI File Revision 1.0 is NOT supported"<<endl<<endl;
		OMF2::omfsCloseFile(OMFFileHdl);
		OMF2::omfsEndSession(OMFSession);
	}
	else
	{
		if (bVerboseMode)
		{
			cout << "OMF File : "<< pFileName<<" opened succesfully"<< endl;
			bOMFFileOpen = AAFTrue;
		}
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
	aafWChar				wcFileName[256];
	aafInt32				nOMFIdentifications;
	aafWChar				CompanyName[256];
	aafWChar				ProductName[256];
    aafWChar				ProductVersionString[256];
    aafWChar				Platform[256];
	aafBool					bAddExtraIdent = AAFFalse;

	aafProductIdentification_t	ProductInfo;
	IAAFIdentification*		pIdent = NULL;

	
	convert(wcFileName, sizeof(wcFileName), pFileName);
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
					convert(CompanyName, sizeof(CompanyName), text);
					ProductInfo.companyName = CompanyName;
					if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTProductName, text, sizeof(text)) != OMF2::OM_ERR_NONE)
						strcpy(text, "<Not Specified>");
					convert(ProductName, sizeof(ProductName), text);
					ProductInfo.productName = ProductName;
					if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTProductVersionString, text, sizeof(text)) != OMF2::OM_ERR_NONE)
						strcpy(text, "<Not Specified>");
					convert(ProductVersionString, sizeof(ProductVersionString), text);
					ProductInfo.productVersionString = ProductVersionString;
					if(OMF2::omfsReadString(OMFFileHdl, OMFIdent, OMF2::OMIDNTPlatform, text, sizeof(text)) != OMF2::OM_ERR_NONE)
						strcpy(text, "<Not Specified>");
					convert(Platform, sizeof(Platform), text);
					ProductInfo.platform = Platform;
					rc = pFile->OpenNewModify(wcFileName, 0, &ProductInfo);
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
				rc = pFile->OpenNewModify(wcFileName, 0, &ProductInfo);
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
		cout << "AAF File : "<< pFileName<<" Created succesfully"<< endl;

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

	// AAF Variables
	IAAFMob*				pMob = NULL;
	IAAFCompositionMob*		pCompMob = NULL;
	IAAFMasterMob*			pMasterMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	
	rc = OMF2::omfiIteratorAlloc( OMFFileHdl, &OMFMobIter);
	if (AAFRESULT_SUCCESS == rc)
	{
		rc = OMF2::omfiGetNumMobs( OMFFileHdl, OMF2::kAllMob, &nOMFNumMobs);
		if (AAFRESULT_SUCCESS == rc)
		{
			if (bVerboseMode)
			{
				cout <<"Found : "<<nOMFNumMobs<<" Mobs in the input file"<<endl;
			}
			for (nOMFMobCount = 0; nOMFMobCount < nOMFNumMobs; nOMFMobCount++)
			{
				rc = OMF2::omfiGetNextMob(OMFMobIter, NULL, &OMFMob);
				if (AAFRESULT_SUCCESS == rc)
				{
					pMob = NULL;
					if (OMF2::omfiIsACompositionMob(OMFFileHdl, OMFMob, (OMF2::omfErr_t *) &rc))
					{
						// Create a new Composition Mob
						rc = pDictionary->CreateInstance(&AUID_AAFCompositionMob, 
	 													 IID_IAAFCompositionMob,
														 (IUnknown **)&pCompMob);
						if (AAFRESULT_SUCCESS == rc)
						{
							if (bVerboseMode)
								cout <<"Created AAF Composition Mob"<<endl;
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
						if (AAFRESULT_SUCCESS == rc)
						{
							if (bVerboseMode)
								cout <<"Created AAF Master Mob"<<endl;
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
						if (AAFRESULT_SUCCESS == rc)
						{
							if (bVerboseMode)
								cout <<"Created AAF Source Mob"<<endl;
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
							cout<<"Unrecognized Mob Class ID : "<<id<<endl;
						}
					}
					if (pMob)
					{
						rc = ConvertOMFMOBObject( OMFMob, pMob);
						if (rc != AAFRESULT_SUCCESS)
							cout<<"Error convert basic MOB data"<<endl;
						rc = TraverseOMFMob( OMFMob, pMob);
						if (rc != AAFRESULT_SUCCESS)
							cout<<"Error Traversing MOB "<<endl;
						rc = pHeader->AppendMob(pMob);
						if (rc != AAFRESULT_SUCCESS)
							cout<<"Error appending MOB to the file"<<endl;
						pMob->Release();
						pMob = NULL;
					}
				}
			}
		}
	}

	OMF2::omfiIteratorDispose(OMFFileHdl, OMFMobIter);
			
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
		cout<<"Processing OMF Header"<<endl;
	if (OMF2::OM_ERR_NONE == rc)
	{
		//From the OMF header we will extract all definition Objects and Class Dictionary
		// First we process the Data definitions
		numDefs = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADDefinitionObjects);
		if (bVerboseMode)
		{
			Indent(4);
			cout<<"Found : "<<numDefs<< " Data Definitions"<<endl;
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
			Indent(4);
			cout<<"Found : "<<numEntries<< " Class Definitions"<<endl;
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
		cout << "Converted OMF Header values to AAF"<< endl;

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

	rc = OMF2::omfsReadClassID(OMFFileHdl, obj, OMF2::OMOOBJObjClass, objClass);
	if (OMF2::OM_ERR_NONE == rc && bVerboseMode)
	{
		strncpy(id, objClass, 4);
		id[4] = '\0';
		Indent(8);
		cout<<"Processing : "<<id <<" "<<endl;
	}

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

	memset(id, 0, sizeof(id));
	rc = OMF2::omfsReadClassID(OMFFileHdl, obj, OMF2::OMCLSDClass, id);
	if ( OMF2::OM_ERR_PROP_NOT_PRESENT == rc )
		cout << "*** Invalid Class Id ***"<<endl;
	else if (bVerboseMode)
	{
		Indent(8);
		cout<<"Processing : "<<id <<" Class Definition"<<endl;
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
	aafWChar				wcMobName[32];
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

	rc = OMF2::omfiMobGetInfo(OMFFileHdl, obj, &OMFMobID, 32, sMobName, NULL, NULL);
	if (AAFRESULT_SUCCESS == rc)
		convert(wcMobName, 32, sMobName);
	else
		convert(wcMobName, 32, "Name NOT provided");


	if (bVerboseMode)
	{
		Indent(8);
		cout<<"Mob Name : "<<sMobName<<endl;
	}
	// Set Name
	pMob->SetName(wcMobName);

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
			aafWChar	wcCommentName[64];
			char		sCommentValue[256];
			aafWChar	wcCommentValue[256];

			rc = OMF2::omfiMobGetNextComment(OMFIterator, obj, sizeof(sCommentName), sCommentName, sizeof(sCommentValue), sCommentValue);
			if (AAFRESULT_SUCCESS == rc)
			{
				convert(wcCommentName, sizeof(sCommentName), sCommentName);
				convert(wcCommentValue, sizeof(sCommentValue), sCommentValue);
				pMob->AppendComment(wcCommentName, wcCommentValue);
			}
		}

		// Release the iterator
		OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
	}

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
		cout << "Converted OMF Composition MOB to AAF"<< endl;

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
	aafWChar				wcTrackName[32];					

	IAAFMobSlot*		pMobSlot = NULL;
	IAAFSegment*		pSegment = NULL;
	IAAFTimelineMobSlot* pTimelineMobSlot = NULL;

	rc = OMF2::omfiMobGetNumSlots(OMFFileHdl, obj, &numSlots);
	if (AAFRESULT_SUCCESS != rc)
		numSlots = 0;

	rc = OMF2::omfiMobGetNumTracks(OMFFileHdl, obj, &numTracks);
	if (AAFRESULT_SUCCESS != rc)
		numTracks = 0;

	if (bVerboseMode)
	{
		Indent(4);
		cout<<"Found : "<<numSlots<<" sub tracks"<<endl;
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
				if (strlen(sTrackName) == 0)
					strcpy(sTrackName, "unknown track name");
				if (AAFRESULT_SUCCESS == rc)
				{
					ProcessOMFComponent(OMFSegment, &pSegment);
					if (pSegment)
					{
						convert(wcTrackName, sizeof(wcTrackName), sTrackName);
						pMob->AppendNewSlot( pSegment, (aafSlotID_t)OMFTrackID, wcTrackName, &pMobSlot );
						if (bVerboseMode)
						{
							Indent(8);
							cout<<"Converted SlotID : "<<(int)OMFTrackID<<" Name : "<<sTrackName<<endl;
						}
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
HRESULT AafOmf::ProcessOMFComponent(OMF2::omfObject_t OMFSegment, IAAFSegment** ppSegment)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfEdgecode_t		OMFEdgecode;
	OMF2::omfLength_t		OMFLength;
	OMF2::omfDDefObj_t		OMFDatakind;
	OMF2::omfTimecode_t		OMFTimecode;

	IAAFSequence*			pSequence = NULL;
	IAAFSourceClip*			pSourceClip = NULL;
	IAAFTimecode*			pTimecode = NULL;
	IAAFEdgecode*			pEdgecode = NULL;
	aafEdgecode_t			edgecode;
	aafTimecode_t			timecode;

	// First get sequence information
	if (OMF2::omfiIsASequence(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		rc = pDictionary->CreateInstance(&AUID_AAFSequence,
										 IID_IAAFSequence,
										 (IUnknown **)&pSequence);
		pSequence->QueryInterface(IID_IAAFSegment, (void **)ppSegment);
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Sequence"<< endl;
		}
		ConvertOMFSequence(OMFSegment, pSequence);
		TraverseOMFComponent(OMFSegment);
		pSequence->Release();
	}

	else if (OMF2::omfiIsASourceClip(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		rc = pDictionary->CreateInstance(&AUID_AAFSourceClip,
										 IID_IAAFSourceClip,
										 (IUnknown **)&pSourceClip);
		pSourceClip->QueryInterface(IID_IAAFSegment, (void **)ppSegment);
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing SourceClip"<< endl;
		}
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
			Indent(4);
			cout <<"Found Timecode"<< endl;
			Indent(4);
			cout <<"      length        : "<<(int)OMFLength<<endl;
			Indent(4);
			cout <<"      start Frame   : "<< timecode.startFrame<<endl;
			Indent(4);
			if (timecode.drop == AAFTrue)
				cout <<"      drop          : True"<<endl;
			else
				cout <<"      drop          : False"<<endl;
			Indent(4);
			cout <<"      Frames/second : "<< timecode.fps<<endl;     
				
		}

		rc = pDictionary->CreateInstance(&AUID_AAFTimecode,
										 IID_IAAFTimecode,
										 (IUnknown **)&pTimecode);

		pTimecode->Initialize((aafLength_t)OMFLength, &timecode);

		pTimecode->QueryInterface(IID_IAAFSegment, (void **)ppSegment);
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
			Indent(4);
			cout <<"Found Edgecode "<< endl;
			Indent(4);
			cout <<"      length        : "<<(int)OMFLength<<endl;
			Indent(4);
			cout <<"      start Frame   : "<< edgecode.startFrame<<endl;
		}
		rc = pDictionary->CreateInstance(&AUID_AAFEdgecode,
										 IID_IAAFEdgecode,
										 (IUnknown **)&pEdgecode);

		pEdgecode->Create((aafLength_t)OMFLength, edgecode);
		pEdgecode->QueryInterface(IID_IAAFSegment, (void **)ppSegment);
	}
	else if (OMF2::omfiIsAFiller(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Filler"<< endl;
		}
	}
	else if (OMF2::omfiIsAnEffect(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Effect"<< endl;
		}
	}
	else if (OMF2::omfiIsATransition(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Transition"<< endl;
		}
	}
	else if (OMF2::omfiIsAConstValue(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Constant Value"<< endl;
		}
	}
	else if (OMF2::omfiIsAVaryValue(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Varying Value"<< endl;
		}
	}
	else if (OMF2::omfiIsANestedScope(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Nested Scope"<< endl;
		}
	}
	else if (OMF2::omfiIsAScopeRef(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Scope Reference"<< endl;
		}
	}
	else if (OMF2::omfiIsASelector(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Selector"<< endl;
		}
	}
	else if (OMF2::omfiIsAMediaGroup(OMFFileHdl, OMFSegment, (OMF2::omfErr_t *)&rc) )
	{
		if (bVerboseMode)
		{
			Indent(4);
			cout <<"Processing Media Group"<< endl;
		}
	}
	else
	{
		char	classID[5];

		rc = OMF2::omfsReadClassID(OMFFileHdl, OMFSegment, OMF2::OMOOBJObjClass,classID);
		classID[4] = '\0';
		if (bVerboseMode)
		{
			Indent(8);
			cout <<"UNKNOWN OBJECT : "<<classID<<endl;
		}
	}

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
	OMF2::omfUniqueName_t	datakindName;
	OMF2::omfLength_t		sequLength = 0;

	IAAFComponent*			pComponent = NULL;
	aafUID_t				datadef;
	
	aafBool					bValid = AAFFalse;

	// Get a pointer to a component interface
	rc = pSequence->QueryInterface(IID_IAAFComponent, (void **)&pComponent);
	if (AAFRESULT_SUCCESS == rc)
	{
		// Get Sequence data kind 
		rc = OMF2::omfiSequenceGetInfo(OMFFileHdl, sequence, &datakind, &sequLength);
		rc = OMF2::omfiDatakindGetName(OMFFileHdl, datakind, 64, datakindName);
		if (strncmp("omfi:data:Picture", datakindName, 17)== 0)
		{
			bValid = AAFTrue;
			datadef = DDEF_Video;
		}
		else if (strncmp("omfi:data:Sound", datakindName, 15) == 0)
		{
			bValid = AAFTrue;
			datadef = DDEF_Audio;
		}
		else if(strncmp("omfi:data:Timecode", datakindName, 18) == 0)
		{
			bValid = AAFTrue;
			datadef = DDEF_Timecode;
		}
		else
		{
			bValid = AAFFalse;
			cout<<"Invalid DataDef : "<<datakindName<<" Found in sequence"<<endl;
		}
		if (bValid)
			pComponent->SetDataDef(&datadef);
		// Next - get all properties
		rc = ConvertOMFComponentProperties(sequence, pComponent);
		pComponent->Release();
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
			case OMF2::OMCPNTDatakind:
				break;
			case OMF2::OMCPNTTrackKind:
			case OMF2::OMCPNTEditRate:
			case OMF2::OMCPNTName:
			case OMF2::OMCPNTLength:
			case OMF2::OMCPNTEffectID:
			case OMF2::OMCPNTAttributes:
			case OMF2::OMOOBJObjClass:
				break;
			default:

				OMF2::omfiGetPropertyName(OMFFileHdl, Property, 64, propertyName);
				cout<<"Component Property NOT converted : "<<propertyName<<endl;
				break;
		}
		rc = OMF2::omfiGetNextProperty(propertyIterator, component, &Property, &propertyType);
	}
	rc = OMF2::omfiIteratorDispose(OMFFileHdl, propertyIterator);

	return rc;
}
// ============================================================================
// TraverseOMFComponent
//
//			This function reads all components the of an OMF Sequence,
//			sets the equivalent AAF properties 
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT AafOmf::TraverseOMFComponent(OMF2::omfObject_t component )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	OMF2::omfIterHdl_t		componentIterator = NULL;
	OMF2::omfPosition_t		sequPos;
	OMF2::omfCpntObj_t		sequComponent;

	aafInt32				numComponents = 0;
	aafInt32				cpntCount;

	OMF2::omfiIteratorAlloc(OMFFileHdl, &componentIterator);
	rc = OMF2::omfiSequenceGetNumCpnts(OMFFileHdl, component, &numComponents);
	if (OMF2::OM_ERR_NONE == rc)
	{
		for (cpntCount = 0; cpntCount < numComponents; cpntCount++)
		{   
			rc = OMF2::omfiSequenceGetNextCpnt(componentIterator, component, NULL, 
				                               &sequPos, &sequComponent); 
		}
	}
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
	
	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		cout << "Converted OMF Master MOB to AAF"<< endl;
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

	
	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		cout << "Converted OMF Source MOB to AAF"<< endl;
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
	cout << theApp.pProgramName << ": Version 0.01.00" << endl;
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
						fatalError("Main", "Could not convert OMF Header Object");
			}
		}
	}
	else
	{
		theApp.Usage();
	}
	return(0);
}