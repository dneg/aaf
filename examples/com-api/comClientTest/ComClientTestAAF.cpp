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

#include "AAFTypes.h"

#if defined(_MAC) || defined(macintosh)
#include <initguid.h> // define all of the AAF guids.
#include "AAF.h"
#else
#include "AAF.h"
// TODO: This should not be here, I added them for now to get a good link.
const CLSID CLSID_AAFSession = { 0xF0C10891, 0x3073, 0x11d2, { 0x80, 0x4A, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

const CLSID CLSID_AAFMob =            { 0xB1A21375, 0x1A7D, 0x11d2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };

const CLSID CLSID_AAFSegment =        { 0x7a2f0571, 0x1ba3, 0x11D2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };

const CLSID CLSID_AAFSourceClip = { 0x38e6f8a5, 0x2a2c, 0x11d2, { 0x84, 0x11, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };

const CLSID CLSID_AAFSourceMob = { 0xB1A2137D, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };

const CLSID CLSID_AAFFileDescriptor = { 0xe58a8562, 0x2a3e, 0x11D2, { 0xbf, 0xa4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
#endif

static void     FatalErrorCode(HRESULT errcode, int line, char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
  exit(1);
}

static void     FatalError(char *message)
{
  printf(message);
  exit(1);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define check(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     FatalErrorCode(moduleErrorTmp, __LINE__, __FILE__);\
}

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}


static void convert(wchar_t* wcName, size_t length, const char* name)
{
  assert((name && *name), "Valid input name");
  assert(wcName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");
  
  size_t status = mbstowcs(wcName, name, length);
  if (status == (size_t)-1) {
	fprintf(stderr, "Error : Failed to convert'%s' to a wide character string.\n\n", name);
    exit(1);  
  }
}

static void convert(char* cName, size_t length, const wchar_t* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
	fprintf(stderr, ": Error : Conversion failed.\n\n");
    exit(1);  
  }
}

static void convert(char* cName, size_t length, const char* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t sourceLength = strlen(name);
  if (sourceLength < length - 1) {
    strncpy(cName, name, length);
  } else {
	fprintf(stderr, "Error : Failed to copy '%s'.\n\n", name);
    exit(1);  
  }
}

static void convert(wchar_t* wName, size_t length, const wchar_t* name)
{
  assert((name && *name), "Valid input name");
  assert(wName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t sourceLength = 0;
  while (*name)
	  ++sourceLength;
  if (sourceLength < length - 1) {
	// Copy the string if there is enough room in the destinition buffer.
	while (*wName++ = *name++)
		;
  } else {
	fprintf(stderr, "Error : Failed to copy '%s'.\n\n", name);
    exit(1);  
  }
}

// The maximum number of characters in the formated CLSID.
// (as returned by StringFromGUID2).
const size_t MAX_CLSID_BUFFER = 40;

static void formatMobID(char *cBuffer, size_t length, aafUID_t *pMobID)
{
  assert(pMobID, "Valid input mobID");
  assert(cBuffer != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  int bytesCopied = 0;
  OLECHAR wCLSID[MAX_CLSID_BUFFER];

  bytesCopied = StringFromGUID2(*((GUID *)pMobID), wCLSID, MAX_CLSID_BUFFER);
  if (0 < bytesCopied) {
    convert(cBuffer, length, wCLSID);
  } else {
	fprintf(stderr, "Error : formatMobID failed.\n\n");
    exit(1);  
  }
}

static void printIdentification(IAAFIdentification* pIdent)
{
    aafWChar wchName[500];
    char chName[1000];
    
    
	check(pIdent->GetCompanyName(wchName, sizeof (wchName)));
	convert(chName, sizeof(chName), wchName);
	printf("CompanyName          = \"%s\"\n", chName);

	check(pIdent->GetProductName(wchName, sizeof (wchName)));
	convert(chName, sizeof(chName), wchName);
	printf("ProductName          = \"%s\"\n", chName);

	check(pIdent->GetProductVersionString(wchName, sizeof (wchName)));
	convert(chName, sizeof(chName), wchName);
	printf("ProductVersionString = \"%s\"\n", chName);

	check(pIdent->GetPlatform(wchName, sizeof (wchName)));
	convert(chName, sizeof(chName), wchName);
	printf("Platform             = \"%s\"\n", chName);
}

static void ReadAAFFile(aafWChar * pFileName)
{
	IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	IAAFIdentification *		pIdent;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t	numMobs, n, s;

	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"Make AVR Example. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;
	  
	check(CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)&pSession));
	  
	check(pSession->SetDefaultIdentification(&ProductInfo));

	check(pSession->OpenReadFile(pFileName, &pFile));
  
	check(pFile->GetHeader(&pHeader));

	check(pHeader->GetLastIdentification(&pIdent));

	printf("LastIdentification\n");
	printIdentification(pIdent);

	check(pHeader->GetNumMobs(kAllMob, &numMobs));
	printf("Number of Mobs = %ld\n", numMobs);

	IEnumAAFMobs *mobIter;
	IAAFSourceMob	*smob;
	IAAFEssenceDescriptor	*essenceDesc;
	IAAFFileDescriptor		*fileDesc;

//!!!	aafSearchCrit_t		criteria;
//!!!	criteria.searchTag = kNoSearch;
    check(pHeader->EnumAAFAllMobs (NULL, &mobIter));
	for(n = 0; n < numMobs; n++)
	{
		// Buffer for string version of each object's class id.
		IAAFMob			*aMob = NULL;
		aafWChar		name[500], slotName[500];
    	char chName[1000], chMobID[MAX_CLSID_BUFFER];
		aafNumSlots_t	numSlots;
		IEnumAAFMobSlots	*slotIter = NULL;
		IAAFMobSlot		*slot = NULL;
		aafUID_t		mobID;
		aafSlotID_t		trackID;
		aafRational_t	rate;

		check(mobIter->NextOne (&aMob));
		check(aMob->GetName (name));
		check(aMob->GetMobID (&mobID));
		formatMobID(chMobID, MAX_CLSID_BUFFER, &mobID);
		convert(chName, sizeof(chName), name);
		printf("Mob %ld: (ID %s) is named '%s'\n", n, chMobID, chName);
	    check(aMob->GetNumSlots (&numSlots));
		printf("Found %ld slots\n", numSlots);
	    if(SUCCEEDED(aMob->QueryInterface (IID_IAAFSourceMob, (void **)&smob)))
		{
			check(smob->GetEssenceDescription(&essenceDesc));
			if(SUCCEEDED(essenceDesc->QueryInterface (IID_IAAFFileDescriptor, (void **)&fileDesc)))
			{
				check(fileDesc->GetSampleRate(&rate));
				printf("    It is a file source mob of sample rate %ld/%ld.\n",
						rate.numerator, rate.denominator);
			}
			else
				printf("    It is a source mob, but not a file source mob\n");
		}
		check(aMob->GetNumSlots (&numSlots));
		printf("Found %ld slots\n", numSlots);
		if(numSlots != 0)
		{
			check(aMob->EnumAAFAllMobSlots(&slotIter));
			for(s = 0; s < numSlots; s++)
			{
				check(slotIter->NextOne (&slot));
				check(slot->GetName (slotName));
				check(slot->GetSlotID(&trackID));
				convert(chName, sizeof(chName), slotName);
				printf("    Slot %ld: (ID %ld), is named '%s'\n",
							s, trackID, chName);
			}
		}
	}

	//!!! Problem deleting, let it leak -- 	delete mobIter;
	check(pFile->Close());

	check(pSession->EndSession());

	if (pHeader) pHeader->Release();
	if (pFile) pFile->Release();
	if (pSession) pSession->Release();
}

static void CreateAAFFile(aafWChar * pFileName)
{
	IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"Make AVR Example";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	check(CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)&pSession));
    
	check(pSession->SetDefaultIdentification(&ProductInfo));

	check(pSession->CreateFile(pFileName, kAAFRev1, &pFile));
  
  	check(pFile->GetHeader(&pHeader));
 	
//Make the first mob
	IAAFMob						*pMob;
	long	test;
	aafWChar		*names[5] = { L"FOOBAR1", L"FOOBAR2", L"FOOBAR3", L"FOOBAR4", L"FOOBAR5" };
	aafRational_t	editRate = { 2997, 100 };
	IAAFMobSlot		*newSlot = NULL;
	IAAFSegment		*seg = NULL;
	IAAFSourceClip	*sclp = NULL;
	IAAFSourceMob	*smob = NULL;
	aafInt32		testSlot;
	aafWChar		*slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };
	IAAFFileDescriptor	*fileDesc = NULL;
	IAAFEssenceDescriptor *essenceDesc = NULL;
	aafRational_t	audioRate = { 44100, 1 };

	for(test = 0; test < 5; test++)
	{
 		// Create a source Mob with a FileDescriptor attached
		check(CoCreateInstance(CLSID_AAFSourceMob,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceMob, 
						   (void **)&smob));
		check(smob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		check(CoCreateGuid((GUID *)&newUID)); // hack: we need a utility function.
		//newUID.Data1 = test;
		check(pMob->SetMobID(&newUID));
		check(pMob->SetName(names[test]));

 		check(CoCreateInstance(CLSID_AAFFileDescriptor,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFileDescriptor, 
						   (void **)&fileDesc));
		check(fileDesc->SetSampleRate(&audioRate));
		check(fileDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&essenceDesc));
		check(smob->SetEssenceDescription(essenceDesc));

		// Add some slots
		for(testSlot = 0; testSlot < 3; testSlot++)
		{
 			check(CoCreateInstance(CLSID_AAFSourceClip,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSourceClip, 
						   (void **)&sclp));		//!!!Temp, abstract superclass
			check(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));
			check(pMob->AppendNewSlot (seg, testSlot+1, slotNames[testSlot], &newSlot));
			
			// Cleanup references...
			newSlot->Release();
			newSlot = NULL;
			
			seg->Release();
			seg = NULL;
			
			sclp->Release();
			sclp = NULL;
		}
		
		// Add the newly created and initialized Mob to the end of the mob index.
		check(pHeader->AppendMob(pMob));
		
		
		// Cleanup references...
		fileDesc->Release();
		fileDesc = NULL;
		
		pMob->Release();
		pMob = NULL;
		
		smob->Release();
		smob = NULL;
	}
	
	// Cleanup
	pHeader->Release();
	pHeader = NULL;
	
	check(pFile->Close());

	check(pSession->EndSession());

	if (pFile) pFile->Release();
	if (pSession) pSession->Release();
}

// simple helper class to initialize and cleanup COM library.
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

main()
{
	CComInitialize comInit;
	aafWChar * pwFileName = L"Foo.aaf";
	const char * pFileName = "Foo.aaf";

  	printf("***Creating file %s\n", pFileName);
	CreateAAFFile(pwFileName);
  	printf("***Re-opening file %s\n", pFileName);
	ReadAAFFile(pwFileName);

	printf("Done\n");


	return(0);
}

