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

// Include the AAF interface declarations.
#include "AAF.h"


// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

#include "CAAFBuiltinDefs.h"


static void     FatalErrorCode(HRESULT errcode, int line, char *file)
{
  printf("\nError '%0x' returned at line %d in %s\n", errcode, line, file);
  exit(1);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */
#define check(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     FatalErrorCode(moduleErrorTmp, __LINE__, __FILE__);\
}

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "\nASSERT: %s\n\n", msg); exit(1);}


static void convert(wchar_t* wcName, size_t length, const char* name)
{
  assert((name && *name), "Valid input name");
  assert(wcName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");
  
  size_t status = mbstowcs(wcName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, "\nError : Failed to convert'%s' to a wide character string.\n\n", name);
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
    fprintf(stderr, "\nError : Conversion failed.\n\n");
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
    fprintf(stderr, "\nError : Failed to copy '%s'.\n\n", name);
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
    fprintf(stderr, "\nError : Failed to copy '%s'.\n\n", name);
    exit(1);  
  }
}

// The maximum number of characters in the formated CLSID.
// (as returned by StringFromGUID2).
const size_t MAX_CLSID_BUFFER = 40;

static void formatMobID(char *cBuffer, size_t length, aafMobID_t *pMobID)
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
    fprintf(stderr, "\nError : formatMobID failed.\n\n");
    exit(1);  
  }
}

#if defined(_MAC) || defined(macintosh)
// For some reason the CoCreateGuid() function is not implemented in the 
// Microsoft Component Library...so we define something that should be
// fairly unique on the mac.

#include <Events.h>
#include <time.h>

STDAPI CoCreateGuid(GUID  *pguid)
{
  // {1994bd00-69de-11d2-b6bc-fcab70ff7331}
  static GUID sTemplate = 
    { 0x1994bd00, 0x69de, 0x11d2, { 0xb6, 0xbc, 0xfc, 0xab, 0x70, 0xff, 0x73, 0x31 } };

  static bool sInitializedTemplate = false;
  
  if (NULL == pguid)
    return E_INVALIDARG;
    
  if (!sInitializedTemplate)
  {
    time_t timer = time(NULL);
    UInt32 ticks = TickCount();
   
    sTemplate.Data1 += timer + ticks;
    
    sInitializedTemplate = true;
  }
  
  // Just bump the first member of the guid to emulate GUIDGEN behavior.
  ++sTemplate.Data1;
  
  *pguid = sTemplate;
  
  return S_OK;
}

#endif

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
  HRESULT hr = S_OK;
  IAAFFile * pFile = NULL;


  hr = AAFFileOpenExistingRead(pFileName, 0, &pFile);
  if (SUCCEEDED(hr))
  {
    IAAFHeader * pHeader = NULL;


    hr = pFile->GetHeader(&pHeader);
    check(hr); // display error message
    if (SUCCEEDED(hr))
    {
      IAAFDictionary * pDictionary = NULL;

      hr = pHeader->GetDictionary(&pDictionary);
      check(hr); // display error message
      if (SUCCEEDED(hr))
      {
        IAAFIdentification *pIdent = NULL;
        aafNumSlots_t numMobs = 0;


        hr = pHeader->GetLastIdentification(&pIdent);
        check(hr); // display error message
        if (SUCCEEDED(hr))
        {
          printf("LastIdentification\n");
          printIdentification(pIdent);

          pIdent->Release();
          pIdent = NULL;
        }

        hr = pHeader->CountMobs(kAllMob, &numMobs);
        check(hr); // display error message
        if (FAILED(hr))
          numMobs = 0;
        printf("Number of Mobs = %ld\n", numMobs);


        IEnumAAFMobs      *mobIter = NULL;

        if (SUCCEEDED(hr))
        {
          //!!!  aafSearchCrit_t    criteria;
          //!!!  criteria.searchTag = kNoSearch;
          hr = pHeader->GetMobs (NULL, &mobIter);
          check(hr); // display error message
        }
        if (SUCCEEDED(hr)) // EnumAAFAllMobs && GetNumMobs SUCCEEDED
        {
          aafUInt32 n = 0;


          for(n = 0; n < numMobs; n++)
          {
            // Buffer for string version of each object's class id.
            IAAFMob *aMob = NULL;
            aafWChar name[500], slotName[500];
            char chName[1000], chMobID[MAX_CLSID_BUFFER];
            aafNumSlots_t  numSlots;
            aafMobID_t    mobID = {0};
            aafSlotID_t    trackID;
            aafRational_t  rate;

            hr = mobIter->NextOne (&aMob);
            check(hr); // display error message
            if (SUCCEEDED(hr))
            {
              IAAFSourceMob      *smob = NULL;

              hr = aMob->GetName (name, sizeof(slotName));
              check(hr); // display error message
              if (SUCCEEDED(hr))
                convert(chName, sizeof(chName), name);
              else
                strcpy(chName, "<mob name not available>");

              hr = aMob->GetMobID (&mobID);
              check(hr); // display error message
              if (SUCCEEDED(hr))
                formatMobID(chMobID, MAX_CLSID_BUFFER, &mobID);
              else
                strcpy(chMobID, "<mob id not available>");
            
              printf("Mob %ld: (ID %s) is named '%s'\n", n, chMobID, chName);

              hr = aMob->CountSlots (&numSlots);
              check(hr); // display error message
              if (FAILED(hr))
                numSlots = 0;
              printf("Found %ld slots\n", numSlots);

              hr = aMob->QueryInterface (IID_IAAFSourceMob, (void **)&smob);
              check(hr); // display error message
              if(SUCCEEDED(hr))
              {
                IAAFEssenceDescriptor  *essenceDesc = NULL;
                aafUInt32 numLocators;


                hr = smob->GetEssenceDescriptor(&essenceDesc);
                check(hr); // display error message
                if (SUCCEEDED(hr))
                {
                  IAAFFileDescriptor *fileDesc = NULL;


                  hr = essenceDesc->QueryInterface (IID_IAAFFileDescriptor, (void **)&fileDesc);
                  check(hr); // display error message
                  if(SUCCEEDED(hr))
                  {
                    hr = fileDesc->GetSampleRate(&rate);
                    check(hr); // display error message
                    if(SUCCEEDED(hr))
                    {
                      printf("    It is a file source mob of sample rate %ld/%ld.\n",
                             rate.numerator, rate.denominator);
                    }
                    // cleanup references...
                    fileDesc->Release();
                    fileDesc = NULL;
                  }
                  else
                    printf("    It is a source mob, but not a file source mob\n");

                  numLocators = -1;
                  hr = essenceDesc->CountLocators(&numLocators);
                  check(hr); // display error message
                  if(SUCCEEDED(hr))
                  {
                    assert ((numLocators >= 0), "numLocators written");
                    printf ("    It has %d locator%s attached.\n",
                            numLocators,
                            numLocators==1 ? "" : "s");
                  }

                  // cleanup references...
                  essenceDesc->Release();
                  essenceDesc = NULL;
                }

                smob->Release();
                smob = NULL;
              }

              if(numSlots != 0)
              {
                IEnumAAFMobSlots  *slotIter = NULL;


                hr = aMob->GetSlots(&slotIter);
                check(hr); // display error message
                if(SUCCEEDED(hr))
                {
                  IAAFMobSlot    *slot = NULL;
                  aafUInt32 s;


                  for(s = 0; s < numSlots; s++)
                  {
                    hr = slotIter->NextOne (&slot);
                    check(hr); // display error message
                    if(SUCCEEDED(hr))
                    {
                      check(slot->GetName (slotName, sizeof(slotName)));
                      check(slot->GetSlotID(&trackID));
                      convert(chName, sizeof(chName), slotName);
                      printf("    Slot %ld: (ID %ld), is named '%s'\n",
                            s, trackID, chName);
    
                      // cleanup references
                      slot->Release();
                      slot = NULL;
                    } // NextOne
                  } // for

                  // cleanup references
                  slotIter->Release();
                  slotIter = NULL;
                } // SUCCEEDED(EnumAAFAllMobSlots)
              } // if(numSlots != 0)

              // cleanup references
              aMob->Release();
              aMob = NULL;
            }
          }


          // cleanup references
          mobIter->Release();
          mobIter = NULL;
        } // EnumAAFAllMobs && GetNumMobs SUCCEEDED
        
        pDictionary->Release();
        pDictionary = NULL;
      }

      pHeader->Release();
      pHeader = NULL;
    }

    check(pFile->Close());

    pFile->Release();
    pFile = NULL;

  }
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static void CreateAAFFile(aafWChar * pFileName)
{
  IAAFFile *          pFile = NULL;
  IAAFHeader *        pHeader = NULL;
  IAAFDictionary *pDictionary = NULL;
  aafProductIdentification_t  ProductInfo;
  aafMobID_t          newMobID;
  
  // delete any previous test file before continuing...
  char chFileName[1000];
  convert(chFileName, sizeof(chFileName), pFileName);
  remove(chFileName);

  // Create a new file...
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"Make AVR Example";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = NIL_UID;
  ProductInfo.platform = NULL;
  
  check(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
  check(pFile->GetHeader(&pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  check(pHeader->GetDictionary(&pDictionary));
  CAAFBuiltinDefs defs (pDictionary);
   
//Make the first mob
  IAAFMob            *pMob = NULL;
  long  test;
  aafWChar    *names[5] = { L"FOOBAR1", L"FOOBAR2", L"FOOBAR3", L"FOOBAR4", L"FOOBAR5" };
  aafRational_t  editRate = { 2997, 100 };
  IAAFTimelineMobSlot    *newSlot = NULL;
  IAAFSegment    *seg = NULL;
  IAAFSourceClip  *sclp = NULL;
  IAAFSourceMob  *smob = NULL;
  aafInt32    testSlot;
  aafWChar    *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };
  IAAFFileDescriptor  *fileDesc = NULL;
  IAAFEssenceDescriptor *essenceDesc = NULL;
  aafRational_t  audioRate = { 44100, 1 };
  IAAFLocator    *pLocator = NULL;

  for(test = 0; test < 5; test++)
  {
     // Create a source Mob with a FileDescriptor attached
    check(defs.cdSourceMob()->
		  CreateInstance(IID_IAAFSourceMob, 
						 (IUnknown **)&smob));
    check(smob->QueryInterface (IID_IAAFMob, (void **)&pMob));
    check(CoCreateGuid((GUID *)&newMobID)); // hack: we need a utility function.
    //newMobID.Data1 = test;
    check(pMob->SetMobID(newMobID));
    check(pMob->SetName(names[test]));

    check(defs.cdFileDescriptor()->
		  CreateInstance(IID_IAAFFileDescriptor, 
						 (IUnknown **)&fileDesc));
    check(fileDesc->SetSampleRate(audioRate));
    check(fileDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&essenceDesc));

    {
      HRESULT stat;
      stat = defs.cdNetworkLocator()->
		CreateInstance(IID_IAAFLocator, 
					   (IUnknown **)&pLocator);
      check (stat);
    }
    check(fileDesc->SetSampleRate(audioRate));

    check(essenceDesc->AppendLocator (pLocator));
    check(smob->SetEssenceDescriptor(essenceDesc));

    // Add some slots
    for(testSlot = 0; testSlot < 3; testSlot++)
    {
       check(defs.cdSourceClip()->
			 CreateInstance(IID_IAAFSourceClip, 
							(IUnknown **)&sclp));
      check(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));
      check(pMob->AppendNewTimelineSlot
			(editRate,
			 seg,
			 testSlot+1,
			 slotNames[testSlot],
			 0,
			 &newSlot));
      
      // Cleanup references...
      newSlot->Release();
      newSlot = NULL;
      
      seg->Release();
      seg = NULL;
      
      sclp->Release();
      sclp = NULL;
    }
    
    // Add the newly created and initialized Mob to the end of the mob index.
    check(pHeader->AddMob(pMob));
    
    
    // Cleanup references...
    pLocator->Release();
    pLocator = NULL;

    essenceDesc->Release();
    essenceDesc = NULL;

    fileDesc->Release();
    fileDesc = NULL;
    
    pMob->Release();
    pMob = NULL;
    
    smob->Release();
    smob = NULL;
  }
  
  // Cleanup
  pDictionary->Release();
  pDictionary = NULL;

  pHeader->Release();
  pHeader = NULL;
  check(pFile->Save());
  check(pFile->Close());
  if (pFile)
    pFile->Release();

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

// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
  CAAFInitialize(const char *dllname = NULL)
  {
  	printf("Attempting to load the AAF dll...\n");
    check(AAFLoad(dllname));
    printf("DONE\n");
  }

  ~CAAFInitialize()
  {
    AAFUnload();
  }
};


main()
{
  CComInitialize comInit;
  CAAFInitialize aafInit;

  aafWChar * pwFileName = L"Foo.aaf";
  const char * pFileName = "Foo.aaf";

  printf("***Creating file %s\n", pFileName);
  CreateAAFFile(pwFileName);
  printf("***Re-opening file %s\n", pFileName);
  ReadAAFFile(pwFileName);
  
  printf("Done\n");
  return(0);
}

