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
#include <wprintf.h>
#include <initguid.h> // define all of the AAF guids.
#include "AAF.h"
#else
#include "AAF.h"
// TODO: This should not be here, I added them for now to get a good link.
const CLSID CLSID_AAFSession = { 0xF0C10891, 0x3073, 0x11d2, { 0x80, 0x4A, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

const CLSID CLSID_AAFMob =            { 0xB1A21375, 0x1A7D, 0x11d2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };

const CLSID CLSID_AAFSegment =        { 0x7a2f0571, 0x1ba3, 0x11D2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };
#endif

static void     FatalErrorCode(HRESULT errcode, int line, char *file)
{
  wprintf(L"Error '%0x' returned at line %d in %s\n", errcode, line, file);
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

static void printIdentification(IAAFIdentification* pIdent)
{
    aafWChar companyName[500];
	check(pIdent->GetCompanyName(companyName, sizeof (companyName)));
	wprintf(L"CompanyName          = \"%s\"\n", companyName);

	aafWChar productName[500];
	check(pIdent->GetProductName(productName, sizeof (productName)));
	wprintf(L"ProductName          = \"%s\"\n", productName);

	aafWChar productVersionString[500];
	check(pIdent->GetProductVersionString(productVersionString,
										  sizeof (productVersionString)));
	wprintf(L"ProductVersionString = \"%s\"\n", productVersionString);

	aafWChar platform[500];
	check(pIdent->GetPlatform(platform, sizeof (platform)));
	wprintf(L"Platform             = \"%s\"\n", platform);
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

	wprintf(L"LastIdentification\n");
	printIdentification(pIdent);

	check(pHeader->GetNumMobs(kAllMob, &numMobs));
	printf("Number of Mobs = %ld\n", numMobs);

	IEnumAAFMobs *mobIter;
//!!!	aafSearchCrit_t		criteria;
//!!!	criteria.searchTag = kNoSearch;
    check(pHeader->EnumAAFAllMobs (NULL, &mobIter));
	for(n = 0; n < numMobs; n++)
	{
		IAAFMob			*aMob;
		aafWChar		name[500], slotName[500];
		aafNumSlots_t	numSlots;
		IEnumAAFMobSlots	*slotIter;
		IAAFMobSlot		*slot;
		aafUID_t		mobID;
		aafSlotID_t	trackID;

		check(mobIter->NextOne (&aMob));
		check(aMob->GetName (name));
		check(aMob->GetMobID (&mobID));
		wprintf(L"Mob %ld: (ID %ld) is named '%s'\n", n, mobID.Data1, name);
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
				wprintf(L"    Slot %ld: (ID %ld), is named '%s'\n",
							s, trackID, slotName);
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
	IAAFMobSlot		*newSlot;
	IAAFSegment		*seg;
	aafInt32		testSlot;
	aafWChar		*slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

	
	for(test = 0; test < 5; test++)
	{
 		check(CoCreateInstance(CLSID_AAFMob,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFMob, 
						   (void **)&pMob));
		newUID.Data1 = test;
		check(pMob->SetMobID(&newUID));
		check(pMob->SetName(names[test]));

		for(testSlot = 0; testSlot < 3; testSlot++)
		{
 			check(CoCreateInstance(CLSID_AAFSegment,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSegment, 
						   (void **)&seg));		//!!!Temp, abstract superclass
			check(pMob->AppendNewSlot (seg, testSlot+1, slotNames[testSlot], &newSlot));
//			seg->Release();
//			newSlot->Release();
		}
		check(pHeader->AppendMob(pMob));
//		pMob->Release();
	}
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
	aafWChar * pFileName = L"Foo.aaf";

  	wprintf(L"***Creating file %s\n", pFileName);
	CreateAAFFile(pFileName);
  	wprintf(L"***Re-opening file %s\n", pFileName);
	ReadAAFFile(pFileName);

	wprintf(L"Done\n");


	return(0);
}

