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
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#if defined(_MAC) || defined(macintosh)
#include <initguid.h> // define all of the AAF guids.
#include "AAF.h"
#else
#include "AAF.h"
// TODO: This should not be here, I added them for now to get a good link.
const CLSID CLSID_AAFFile = { 0x9346ACD2, 0x2713, 0x11d2, { 0x80, 0x35, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

#endif

// Include the defintions for the AAF Stored Object identifiers.
#define INIT_AUID
#include "AAFStoredObjectIDs.h"


#define FAILURE (2)

// ============================================================================
// char to wide char conversion function prototypes
// ============================================================================
static void convert(wchar_t* wcName, size_t length, const char* name);
static void convert(char* cName, size_t length, const wchar_t* name);
static void convert(char* cName, size_t length, const char* name);
static void convert(wchar_t* wName, size_t length, const wchar_t* name);

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

// ============================================================================
// simple helper class to run the main application.
// ============================================================================
class AafOmf
{
public:
	AafOmf();
	~AafOmf();

public:
	void Usage( void );
	HRESULT GetUserInput(int argc, char* argv[]);
	HRESULT OpenInputFile( void );
	HRESULT OpenOutputFile( void );
	aafBool IsOMFFile(char* pFileName);
	HRESULT OMFFileOpen( char* pFileName );
	HRESULT	TOCFileCreate( void ); 
	void OMFFileClose( void );
	HRESULT AAFFileOpen( char* pFileName );
	HRESULT AAFDefinitionFileOpen( void );
	void AAFFileClose(void );
	HRESULT OMFFileRead( void );
	HRESULT ConvertOMFHeader( void );
	HRESULT ConvertOMFDataDefinitionObject( OMF2::omfObject_t obj);
	HRESULT ConvertOMFClassDictionaryObject( OMF2::omfObject_t obj);
	HRESULT ParseOMFMOBObject( OMF2::omfObject_t obj );
	HRESULT ConvertOMFMOBObject( OMF2::omfObject_t obj, IAAFMob* pMob );
	HRESULT ConvertOMFCompositionObject( OMF2::omfObject_t obj );
	HRESULT ConvertOMFMasterMob( OMF2::omfObject_t obj );
	HRESULT ConvertOMFSourceMob( OMF2::omfObject_t obj );
	HRESULT ConvertOMFMobSlots( OMF2::omfObject_t obj, IAAFMob* pMob );

	char*					pProgramName;

private:
	aafBool					bVerboseMode;
	aafBool					bCreateTOCFile;
	aafBool					bConvertAllObjects;
	aafBool					bOMFFileOpen;
	aafBool					bAAFFileOpen;
	aafBool					bLogFile;
	aafBool					bDefFile;
	char					sInFileName[256];
	char					sTOCFileName[256];
	char					sDefinitionFileName[256];
	char					sOutFileName[256];
	char					sLogFileName[256];

	// For Statistical summary
	aafInt32				nNumOMFObjects;
	aafInt32				nNumAAFObjects;
	aafInt32				nNumOMFProperties;
	aafInt32				nNumAAFProperties;
	aafInt32				nNumUndefinedOMFObjects;
	aafInt32				nNumUndefinedOMFProperties;

    OMF2::omfSessionHdl_t	OMFSession;
	OMF2::omfHdl_t			OMFFileHdl;
	OMF2::omfFileRev_t		OMFFileRev;

	IAAFFile*				pFile;
	IAAFHeader*				pHeader;
	IAAFDictionary*			pDictionary;
};

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

	if (IsOMFFile(sInFileName))
		rc = OMFFileOpen(sInFileName);

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
aafBool AafOmf::IsOMFFile (char *pFileName)
{
	aafBool		bReturn = AAFFalse;
	FILE*		pStream = NULL;
	int			result = 0;
	char		ReadBuffer[8];
	char		CompBuffer[8];


	pStream = fopen(pFileName, "r");
	CompBuffer[0] = (char)0xA4;
	CompBuffer[1] = 'C';
	CompBuffer[2] = 'M';
	CompBuffer[3] = (char)0xA5;
	CompBuffer[4] = 'H';
	CompBuffer[5] = 'd';
	CompBuffer[6] = 'r';
	CompBuffer[7] = 0x00;

	if (pStream != NULL)
	{
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
	
		fclose(pStream);
	}
	else
	{
		cout << "Cannot find input file: "<< pFileName<< endl;
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

	aafProductIdentification_t	ProductInfo;
	
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
					rc = pFile->Close();
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
					rc = pFile->OpenExistingModify(wcFileName, 0, &ProductInfo);
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
			rc = pHeader->GetDictionary(&pDictionary);
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

	
	rc = OMF2::omfiIteratorAlloc( OMFFileHdl, &OMFMobIter);
	if (AAFRESULT_SUCCESS == rc)
	{
		rc = OMF2::omfiGetNumMobs( OMFFileHdl, OMF2::kAllMob, &nOMFNumMobs);
		if (AAFRESULT_SUCCESS == rc)
		{
			cout <<"Found : "<<nOMFNumMobs<<" Mobs in the input file"<<endl;
			for (nOMFMobCount = 0; nOMFMobCount < nOMFNumMobs; nOMFMobCount++)
			{
				rc = OMF2::omfiGetNextMob(OMFMobIter, NULL, &OMFMob);
				if (AAFRESULT_SUCCESS == rc)
				{
					rc = ParseOMFMOBObject(OMFMob);
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
// ParseOMFMOBObject
//
//			This function is invoked by the ConvertMobCallBack routine for 
//			each OMF Mob object encountered. 
//
//	Algorithm:  Determine Mob kind
//				if Composition Mob
//					Convert Composition object
//				else if Master Mob
//					Convert Maste mob
//				...
//			
// Returns: AAFRESULT_SUCCESS if MOB object is converted succesfully
//
// ============================================================================
HRESULT AafOmf::ParseOMFMOBObject( OMF2::omfObject_t obj )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfProperty_t		idProp;
	OMF2::omfClassID_t		objClass;


	char					tmpClass[5];
	// Get OMF Mob information data


	idProp = OMF2::OMOOBJObjClass;
	rc = OMF2::omfsReadClassID(OMFFileHdl, obj, idProp, objClass );
	if (OMF2::OM_ERR_NONE == rc)
	{
		strncpy(tmpClass, objClass, 4);
		if (!strncmp(tmpClass, "CMOB", 4) )
			rc = ConvertOMFCompositionObject( obj );
		else if (!strncmp(tmpClass, "MMOB", 4) )
			ConvertOMFMasterMob( obj );
		else if ( !strncmp(tmpClass, "SMOB", 4) )
			ConvertOMFSourceMob( obj );
		else
		{
			tmpClass[4] = '\0';
			cout<<"Unrecognized Mob Class ID : "<<tmpClass<<endl;
		}
	}
	else
	{
		cout<<"Cannot read OMF Class ID"<< endl;
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
HRESULT AafOmf::ConvertOMFCompositionObject(OMF2::omfObject_t obj)
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfClassID_t		objClass;
	OMF2::omfDefaultFade_t	OMFDefaultFade;

	IAAFMob*				pMob = NULL;
	IAAFCompositionMob*		pCompMob = NULL;
	aafUID_t				CompMobID;
	aafDefaultFade_t		AAFDefaultFade;
	
	// Create a new Composition Mob
	rc = pDictionary->CreateInstance(&AUID_AAFCompositionMob, 
	 								 IID_IAAFCompositionMob,
									 (IUnknown **)&pCompMob);
	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		cout << "Created AAF composition MOB "<< endl;
	if (AAFRESULT_SUCCESS == rc )
	{
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



		rc = pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		if (AAFRESULT_SUCCESS == rc)
		{
			rc = ConvertOMFMOBObject(obj, pMob);
			if (AAFRESULT_SUCCESS == rc)
			{
				ConvertOMFMobSlots( obj, pMob);
				rc = pHeader->AppendMob(pMob);
			}
			pMob->Release();
		}

	}

	if (pCompMob)
		pCompMob->Release();

	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		cout << "Converted OMF Composition MOB to AAF"<< endl;
	return rc;
}
// ============================================================================
// ConvertOMFMobSlots
//
//			This function converts all the mob slots in the given OMF mob object 
//			
// Returns: AAFRESULT_SUCCESS if succesfully
//
// ============================================================================
HRESULT AafOmf::ConvertOMFMobSlots( OMF2::omfObject_t obj, IAAFMob* pMob )
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
	IAAFSequence*		pSequence = NULL;
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
				if (AAFRESULT_SUCCESS == rc)
				{
					rc = pDictionary->CreateInstance(&AUID_AAFSequence,
													 IID_IAAFSequence,
													 (IUnknown **)&pSequence);
					pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment);
					convert(wcTrackName, sizeof(wcTrackName), sTrackName);
					pMob->AppendNewSlot( pSegment, (aafSlotID_t)OMFTrackID, wcTrackName, &pMobSlot );
					if (bVerboseMode)
					{
						Indent(8);
						cout<<"Converted SlotID : "<<(int)OMFTrackID<<" Name : "<<sTrackName<<endl;
					} 
				}
			}
			
		}
	}

	OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);

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
HRESULT AafOmf::ConvertOMFMasterMob(OMF2::omfObject_t obj )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	IAAFMasterMob*			pMasterMob = NULL;
	IAAFMob*				pMob = NULL;
	
	// Create a Master Mob 
	rc = pDictionary->CreateInstance(&AUID_AAFMasterMob,
									 IID_IAAFMasterMob,
									 (IUnknown **)&pMasterMob);
	if (AAFRESULT_SUCCESS == rc)
	{
		rc = pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		if (AAFRESULT_SUCCESS == rc)
		{
			ConvertOMFMOBObject(obj, pMob );
			rc = pHeader->AppendMob(pMob );
			pMob->Release();
		}
	}
	
	if (pMasterMob)
		pMasterMob->Release();

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
HRESULT AafOmf::ConvertOMFSourceMob(OMF2::omfObject_t obj )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	IAAFSourceMob*			pSourceMob = NULL;
	IAAFMob*				pMob = NULL;
	
	// Create a Master Mob 
	rc = pDictionary->CreateInstance(&AUID_AAFSourceMob,
									 IID_IAAFSourceMob,
									 (IUnknown **)&pSourceMob);
	if (AAFRESULT_SUCCESS == rc)
	{
		rc = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
		if (AAFRESULT_SUCCESS == rc)
		{
			ConvertOMFMOBObject(obj, pMob );
			rc = pHeader->AppendMob(pMob );
			pMob->Release();
		}
	}
	
	if (pSourceMob)
		pSourceMob->Release();

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