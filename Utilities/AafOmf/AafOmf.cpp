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
const CLSID CLSID_AAFComponent = { 0x38e6f8a0, 0x2a2c, 0x11d2, { 0x84, 0x11, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
const CLSID CLSID_AAFCompositionMob = { 0x91920d64, 0x2a2e, 0x11D2, { 0xbf, 0xA3, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFFile = { 0x9346ACD2, 0x2713, 0x11d2, { 0x80, 0x35, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const CLSID CLSID_AAFFiller = { 0x38e6f8a8, 0x2a2c, 0x11d2, { 0x84, 0x11, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
const CLSID CLSID_AAFHeader = { 0xB1A21383, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFIdentification = { 0xB1A2137F, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFLocator = { 0xe58a8565, 0x2a3e, 0x11D2, { 0xbf, 0xA4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFMasterMob = { 0xb1a21373, 0x1a7d, 0x11d2, { 0xbf, 0x78, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
const CLSID CLSID_AAFEssenceDescriptor = { 0x94659784, 0x2a3c, 0x11D2, { 0xbf, 0xA4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFFileDescriptor = { 0xe58a8562, 0x2a3e, 0x11D2, { 0xbf, 0xa4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFTapeDescriptor = { 0xe58a856a, 0x2a3e, 0x11D2, { 0xbf, 0xa4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFMob = { 0xB1A21375, 0x1A7D, 0x11d2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFMobSlot = { 0xB1A21377, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFNetworkLocator = { 0x2c1097b1, 0x69d6, 0x11d2, { 0x84, 0x1b, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
const CLSID CLSID_AAFObject = { 0xB1A213AC, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFSegment = { 0x7a2f0571, 0x1ba3, 0x11D2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFSequence = { 0x66e855f1, 0x2bc2, 0x11d2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
const CLSID CLSID_AAFSession = { 0xF0C10891, 0x3073, 0x11d2, { 0x80, 0x4A, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const CLSID CLSID_AAFSourceClip = { 0x38e6f8a5, 0x2a2c, 0x11d2, { 0x84, 0x11, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
const CLSID CLSID_AAFSourceMob = { 0xB1A2137D, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFSourceReference = { 0x92B88034, 0x2B2B, 0x11d2, { 0xBF, 0x7E, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFTimecode = { 0x92B88036, 0x2B2B, 0x11d2, { 0xBF, 0x7E, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_AAFTimelineMobSlot = { 0xB1A21379, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFComponents = { 0xB1A21395, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFIdentifications = { 0xB1A21385, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFLocators = { 0xB1A2139F, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFMobSlots = { 0xB1A21389, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };
const CLSID CLSID_EnumAAFMobs = { 0xB1A21387, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };

#endif

#define SUCCESS (1)
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
	HRESULT GetUserInput(int argc, char* argv[], char* pInfile, char* pOutFile);
	void SetOMFFileName(char* pFileName)
	{
		strcpy(sOMFFileName, pFileName);
	}
	void SetAAFFileName(char * pFileName)
	{
		convert(wcAAFFileName, sizeof(wcAAFFileName), pFileName);
	}
	aafBool IsOMFFile(char* pFileName);
	HRESULT OMFFileOpen( void );
	HRESULT	TOCFileCreate( void ); 
	void OMFFileClose( void );
	HRESULT AAFFileOpen( void );
	HRESULT AAFDefinitionFileOpen( void );
	void AAFFileClose(void );
	HRESULT OMFFileRead( void );
	HRESULT ConvertOMFHeader( void );
	HRESULT ConvertOMFMOBObject( OMF2::omfObject_t obj );
	HRESULT ConvertOMFCompositionObject( OMF2::omfObject_t obj, IAAFCompositionMob* pCompMob );
	HRESULT ConvertOMFMasterMob( OMF2::omfObject_t obj, IAAFMasterMob* pMasterMob );

	char*					pProgramName;

private:
	aafBool					bVerboseMode;
	aafBool					bCreateTOCFile;
	aafBool					bOMFFileOpen;
	aafBool					bAAFFileOpen;
	char					sOMFFileName[256];
	char					sTOCFileName[256];
	char					sDefinitionFileName[256];
	aafWChar				wcAAFFileName[256];
	char					sLogFileName[256];
	aafInt32				nMobIdPrefix;

	// For Statistical summary
	aafInt32				nNumOMFObjects;
	aafInt32				nNumAAFObjects;
	aafInt32				nNumOMFProperties;
	aafInt32				nNumAAFProperties;

    OMF2::omfSessionHdl_t	OMFSession;
	OMF2::omfHdl_t			OMFFileHdl;
	OMF2::omfFileRev_t		OMFFileRev;

	IAAFFile*				pFile;
	IAAFHeader*				pHeader;
};

// ============================================================================
// Global Variables
// ============================================================================

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
// Constructor
// ============================================================================
AafOmf::AafOmf() : pFile(NULL), pHeader(NULL), bOMFFileOpen(AAFFalse), bAAFFileOpen(AAFFalse),
	nNumOMFObjects(0), nNumAAFObjects(0), nNumOMFProperties(0), nNumAAFProperties(0)
{
	memset(sOMFFileName, 0, sizeof(sOMFFileName));
	memset(wcAAFFileName, 0, sizeof(wcAAFFileName));
	memset(sLogFileName, 0, sizeof(sLogFileName));

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
	cout << "=================================================================================" << endl;
	cout << pProgramName<<" : OMFI to AAF file conversion Version 0.01.00"<< endl;
	cout << "Usage: "<< endl;
	cout << pProgramName<< " [-v] [-p logfile] <infile> [-d deffile] [-t tocfile] [outfile]"<< endl;
	cout << endl;
	cout << "    -v         = Verbose - give progress report (optional)" << endl;
	cout << "    -p logfile = Log file name(optional)" << endl;
	cout << "    infile     = input file name (required)"<< endl;
	cout << "    -d deffile = Definition file (optional)" << endl;
	cout << "    -t tocfile = Dump OMFI Table of contents (optional)" << endl;
	cout << "    outfile    = output file name (optional)" << endl;
	cout << "=================================================================================" << endl;
}

// ============================================================================
// GetUserInput
//
//		Parse program arguments and extract names of OMF and AAF files.
//
// ============================================================================
HRESULT AafOmf::GetUserInput(int argc, char* argv[], char* pInFile, char* pOutFile)
{
	HRESULT			rc = AAFRESULT_SUCCESS;
	aafInt32		nFileCount = 0;

	pProgramName = argv[0];
	
	for (int i = 1; i < argc; i++)
	{
		char*	pNextArgument = argv[i];
		char	c = pNextArgument[0];

		if ((c == '-') && (strlen(pNextArgument) >= 2))
		{
			char flag = pNextArgument[1];
			switch( flag )
			{
				case 'm':
//					bIgnoreErrors = AAFTrue;
					break;
				case 'v':
					bVerboseMode = AAFTrue;
					break;
				case 'd':
//					bOverride = AAFTrue;
					break;
				case 't':
//					bConvertAAFFile = AAFTrue;
					break;
				default:
					rc = -1;
					break;
			}
		}
		else
		{
			switch(nFileCount)
			{
				case 0:
					strcpy(pInFile, argv[i]);
					nFileCount++;
					break;
				case 1:
					strcpy(pOutFile, argv[i]);
					nFileCount++;
					break;
				case 2:
				default:
					strcpy(sLogFileName, argv[i]);
					break;
			}
		}
	}
	if (argc < 3)
		rc = -1;

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


	return bReturn;
}
// ============================================================================
// OMFFileOpen
//
//		Open an OMF File
//
// ============================================================================
HRESULT AafOmf::OMFFileOpen()
{
	HRESULT				rc = AAFRESULT_SUCCESS;
	aafBool				bSessionStarted = AAFFalse;

	if (OMF2::OM_ERR_NONE == OMF2::omfsBeginSession(0, &OMFSession))
	{
		bSessionStarted = AAFTrue;
		if (OMF2::OM_ERR_NONE == OMF2::omfmInit(OMFSession))
		{
			rc = OMF2::omfsOpenFile((OMF2::fileHandleType)sOMFFileName, OMFSession, &OMFFileHdl);

		}
		else
			rc = -1;
	}
	else 
	{
		if (bSessionStarted)
			OMF2::omfsEndSession(OMFSession);

		rc = -1;
	}

	OMF2::omfsFileGetRev(OMFFileHdl, &OMFFileRev);
	if (OMF2::kOmfRev2x != OMFFileRev)
	{
		rc = -1 ;
		cout << "OMFI File Revision 1.0 is NOT supported"<<endl<<endl;
		OMF2::omfsCloseFile(OMFFileHdl);
		OMF2::omfsEndSession(OMFSession);
	}
	else
	{
		if (bVerboseMode)
		{
			cout << "OMF File : "<< sOMFFileName<<" opened succesfully"<< endl;
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
HRESULT AafOmf::AAFFileOpen( )
{
	HRESULT					rc = AAFRESULT_SUCCESS;

	OMF2::omfObject_t		OMFHeadObject, OMFIdent;
	char					text[256];
	char					sFileName[256];
	aafInt32				nOMFIdentifications;
	aafWChar				CompanyName[256];
	aafWChar				ProductName[256];
    aafWChar				ProductVersionString[256];
    aafWChar				Platform[256];

	aafProductIdentification_t	ProductInfo;

//	if (bOverride)
	{
		// Delete the file if exists.
		deleteFile(wcAAFFileName);
	}
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
					convert(sFileName, 256, wcAAFFileName);
					rc = pFile->OpenNewModify(wcAAFFileName, 0, &ProductInfo);
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
					rc = pFile->OpenExistingModify(wcAAFFileName, 0, &ProductInfo);
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
				rc = pFile->OpenNewModify(wcAAFFileName, 0, &ProductInfo);
				convert(sFileName, 256, wcAAFFileName);
			}
		}
		bAAFFileOpen = AAFTrue;
		rc = pFile->GetHeader(&pHeader);
	}
	else
		rc = AAFRESULT_INTERNAL_ERROR;

	if (bVerboseMode)
		cout << "AAF File : "<< sFileName<<" opened succesfully"<< endl;

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
			for (nOMFMobCount = 0; nOMFMobCount < nOMFNumMobs; nOMFMobCount++)
			{
				rc = OMF2::omfiGetNextMob(OMFMobIter, NULL, &OMFMob);
				if (AAFRESULT_SUCCESS == rc)
				{
					rc = ConvertOMFMOBObject(OMFMob);
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
	OMF2::omfClassID_t		objClass;

	char					id[5];
	aafInt32				numDefs, numEntries;


	rc = OMF2::omfsGetHeadObject( OMFFileHdl, &OMFHeader );
	if (OMF2::OM_ERR_NONE == rc)
	{
		//From the OMF header we will extract all definition Objects and Class Dictionary
		// First we process the Data definitions
		numDefs = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADDefinitionObjects);
		for (int i = 1;i <numDefs;i++)
		{
			rc = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADDefinitionObjects, &OMFObject, i);
			if (OMF2::OM_ERR_NONE == rc)
			{
				// Process the given Data Definition object.
				rc = OMF2::omfsReadClassID(OMFFileHdl, OMFObject, OMF2::OMOOBJObjClass, objClass);
			}
		}
		// Now we process the class dictionary
		numEntries = OMF2::omfsLengthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADClassDictionary);
		for (int j = 1;j <numEntries; j++)
		{
			rc = OMF2::omfsGetNthObjRefArray(OMFFileHdl, OMFHeader, OMF2::OMHEADClassDictionary, &OMFObject, i);
			if (OMF2::OM_ERR_NONE == rc)
			{
				// Process the given Class Dictionary object.
				rc = OMF2::omfsReadClassID(OMFFileHdl, OMFObject, OMF2::OMCLSDClassID, id);
			}
		}
	}
	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		cout << "Converted OMF Header values to AAF"<< endl;

	return rc;
}
// ============================================================================
// ConvertOMFMOBObject
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
HRESULT AafOmf::ConvertOMFMOBObject( OMF2::omfObject_t obj )
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	char					sMobName[32];
	aafWChar				wcMobName[32];
	aafInt32				numComments;
	aafInt32				times;

	OMF2::omfClassID_t		objClass;
	OMF2::omfUID_t			mobID;
	OMF2::omfProperty_t		idProp;
	OMF2::omfIterHdl_t		OMFIterator;

	IAAFCompositionMob*		pCompMob = NULL;
	IAAFMasterMob*			pMasterMob = NULL;
	IAAFMob*				pMob = NULL;

	char					tmpClass[5];

	// Get OMF Mob information data

	rc = OMF2::omfiMobGetInfo(OMFFileHdl, obj, &mobID, 32, sMobName, NULL, NULL);
	if (AAFRESULT_SUCCESS == rc)
		convert(wcMobName, 32, sMobName);
	else
		convert(wcMobName, 32, "Name NOT provided");

	rc = OMF2::omfiMobGetNumComments(OMFFileHdl, obj, &numComments);
	if (AAFRESULT_SUCCESS != rc)
		numComments = 0;

	idProp = OMF2::OMOOBJObjClass;
	rc = OMF2::omfsReadClassID(OMFFileHdl, obj, idProp, objClass );
	if (OMF2::OM_ERR_NONE == rc)
	{
		strncpy(tmpClass, objClass, 4);
		if (!strncmp(tmpClass, "CMOB", 4) )
		{
			// Create the new Composition Mob
			rc = CoCreateInstance(CLSID_AAFCompositionMob, 
			 					  NULL,
								  CLSCTX_INPROC_SERVER,
								  IID_IAAFCompositionMob,
								  (void **)&pCompMob);
			if (AAFRESULT_SUCCESS == rc)
			{
				ConvertOMFCompositionObject( obj, pCompMob);
				rc = pCompMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
				pCompMob->Release();
			}
		}
		else if (!strncmp(tmpClass, "MMOB", 4) )
		{
			// Create a Master Mob 
			rc = CoCreateInstance(CLSID_AAFMasterMob, 
			 					  NULL,
								  CLSCTX_INPROC_SERVER,
								  IID_IAAFMasterMob,
								  (void **)&pMasterMob);
			if (AAFRESULT_SUCCESS == rc)
			{
				ConvertOMFMasterMob( obj, pMasterMob);
				rc = pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
			}

		}
		else
		{
		}
		// Here we update all the MOB related fields
		if (AAFRESULT_SUCCESS == rc)
		{
			pMob->SetName(wcMobName);
			// 
			// pMob-SetUID(); ??
			// Set comments
			if (numComments > 0)
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
			rc = pHeader->AppendMob(pMob);
			pMob->Release();
		}
	}
	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		cout << "Converted OMF MOB generic data to AAF"<< endl;
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
	aafInt32				numSlots;
	aafInt32				numTracks;
	aafInt32				times;

	OMF2::omfClassID_t		objClass;
	OMF2::omfDefaultFade_t	OMFDefaultFade;
	OMF2::omfIterHdl_t		OMFIterator;

	IAAFMob*				pMob = NULL;
	aafUID_t				CompMobID;
	aafDefaultFade_t		AAFDefaultFade;
	

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


	rc = OMF2::omfiMobGetNumSlots(OMFFileHdl, obj, &numSlots);
	if (AAFRESULT_SUCCESS != rc)
		numSlots = 0;

	rc = OMF2::omfiMobGetNumTracks(OMFFileHdl, obj, &numTracks);
	if (AAFRESULT_SUCCESS != rc)
		numTracks = 0;

	// Set Slots
	if (numSlots >0)
	{
		OMF2::omfiIteratorAlloc(OMFFileHdl, &OMFIterator);
				
		for (times = 0; times < numSlots; times++)
		{
			OMF2::omfMSlotObj_t	OMFSlot;
			OMF2::omfMSlotObj_t	OMFSegment;
			OMF2::omfTrackID_t	OMFTrackID;
			OMF2::omfRational_t	OMFeditRate;
			OMF2::omfPosition_t	OMFOrigin;
			OMF2::omfErr_t		OMFError;
			char				sTrackName[32];
			aafWChar			wcTrackName[32];					

			IAAFMobSlot*		pMobSlot = NULL;

			rc = OMF2::omfiMobGetNextSlot(OMFIterator, obj, NULL, &OMFSlot);
			if (AAFRESULT_SUCCESS == rc)
			{
				rc = OMF2::omfiMobSlotGetInfo(OMFFileHdl, OMFSlot, &OMFeditRate, &OMFSegment);
				if (OMF2::omfiMobSlotIsTrack(OMFFileHdl, OMFSlot, &OMFError))
				{
					rc = OMF2::omfiTrackGetInfo(OMFFileHdl, obj, OMFSlot, &OMFeditRate, sizeof(sTrackName),
									sTrackName, &OMFOrigin, &OMFTrackID, &OMFSegment);
						
//							rc = CoCreateInstance(CLSID_AAFMobSlot,
//												  NULL,
//												  CLSCTX_INPROC_SERVER,
//												  IID_IAAFMOBSLOT,
//												  (void **)pMobSlot);
//							pMob->AppendSlot( pMobSlot );
				}
				
			}
		}

		OMF2::omfiIteratorDispose(OMFFileHdl, OMFIterator);
	}

	if (pMob)
		pMob->Release();

	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		cout << "Converted OMF Composition MOB to AAF"<< endl;
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
											 IAAFMasterMob* pMasterMob)
{
	HRESULT					rc = AAFRESULT_SUCCESS;
	
	if (AAFRESULT_SUCCESS == rc && bVerboseMode)
		cout << "Converted OMF Master MOB to AAF"<< endl;
	return rc;
}

void AafOmf::OMFFileClose()
{
	OMF2::omfsCloseFile(OMFFileHdl);
	OMF2::omfsEndSession(OMFSession);
}

void AafOmf::AAFFileClose( )
{
	if (pHeader)
		pHeader->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
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
	char			sInFile[256];
	char			sOutFile[256];

	HRESULT			rc;



	if (AAFRESULT_SUCCESS == theApp.GetUserInput(argc, argv, sInFile, sOutFile) )
	{
		cout << theApp.pProgramName << ": Version 0.01.00" << endl;
		if (!theApp.IsOMFFile (sInFile))
		{
			theApp.SetAAFFileName ( sInFile);
			theApp.SetOMFFileName ( sOutFile);
		}
		else
		{
			theApp.SetAAFFileName ( sOutFile);
			theApp.SetOMFFileName ( sInFile);
		}
		if (AAFRESULT_SUCCESS == theApp.OMFFileOpen() )
		{
			if (AAFRESULT_SUCCESS == theApp.AAFFileOpen() )
			{
					rc = theApp.ConvertOMFHeader( );
					if (AAFRESULT_SUCCESS == rc)
						rc = theApp.OMFFileRead();
					else
						fatalError("Main", "Could not convert OMF Header Object");
			}
			else
			{
				fatalError ("Main","Could not open AAF File ");
			}
		}
		else
		{
			fatalError ("Main","Could not open OMF File");
		}
	}
	else
	{
		theApp.Usage();
	}
	return(0);
}