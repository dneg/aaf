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
// const CLSID CLSID_AAFSession = { 0xF0C10891, 0x3073, 0x11d2, { 0x80, 0x4A, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

const CLSID CLSID_AAFFile = { 0x9346ACD2, 0x2713, 0x11d2, { 0x80, 0x35, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };

const CLSID CLSID_AAFMob =            { 0xB1A21375, 0x1A7D, 0x11d2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };

const CLSID CLSID_AAFSegment =        { 0x7a2f0571, 0x1ba3, 0x11D2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };

const CLSID CLSID_AAFSourceClip = { 0x38e6f8a5, 0x2a2c, 0x11d2, { 0x84, 0x11, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };

const CLSID CLSID_AAFSourceMob = { 0xB1A2137D, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };

const CLSID CLSID_AAFFileDescriptor = { 0xe58a8562, 0x2a3e, 0x11D2, { 0xbf, 0xa4, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };

const CLSID CLSID_AAFNetworkLocator = { 0x2c1097b1, 0x69d6, 0x11d2, { 0x84, 0x1b, 0x00, 0x60, 0x08, 0x32, 0xac, 0xb8 } };
#endif

static void     FatalErrorCode(HRESULT errcode, int line, char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
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
  // IAAFSession * pSession = NULL;
  IAAFFile * pFile = NULL;

  /*
  CoCreateInstance(CLSID_AAFSession,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFSession, 
               (void **)&pSession);
  */
  CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile);
  check(hr); // display error message
  if (SUCCEEDED(hr))
  {
    aafProductIdentification_t  ProductInfo;


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

    // hr = pSession->SetDefaultIdentification(&ProductInfo);
    // check(hr); // display error message
    // if (SUCCEEDED(hr))
    {
      // IAAFFile * pFile = NULL;

      // hr = pSession->OpenReadFile(pFileName, &pFile);
      hr = pFile->Initialize();
      check(hr); // display error message
      hr = pFile->OpenExistingRead(pFileName, 0);
      check(hr); // display error message
      if (SUCCEEDED(hr))
      {
        IAAFHeader * pHeader = NULL;


        hr = pFile->GetHeader(&pHeader);
        check(hr); // display error message
        if (SUCCEEDED(hr))
        {
          IAAFIdentification *pIdent = NULL;
          aafInt32 numMobs = 0;


          hr = pHeader->GetLastIdentification(&pIdent);
          check(hr); // display error message
          if (SUCCEEDED(hr))
          {
            printf("LastIdentification\n");
            printIdentification(pIdent);

            pIdent->Release();
            pIdent = NULL;
          }

          hr = pHeader->GetNumMobs(kAllMob, &numMobs);
          check(hr); // display error message
          if (FAILED(hr))
            numMobs = 0;
          printf("Number of Mobs = %ld\n", numMobs);


          IEnumAAFMobs      *mobIter = NULL;

          if (SUCCEEDED(hr))
          {
            //!!!  aafSearchCrit_t    criteria;
            //!!!  criteria.searchTag = kNoSearch;
            hr = pHeader->EnumAAFAllMobs (NULL, &mobIter);
            check(hr); // display error message
          }
          if (SUCCEEDED(hr)) // EnumAAFAllMobs && GetNumMobs SUCCEEDED
          {
            aafInt32 n = 0;


            for(n = 0; n < numMobs; n++)
            {
              // Buffer for string version of each object's class id.
              IAAFMob *aMob = NULL;
              aafWChar name[500], slotName[500];
              char chName[1000], chMobID[MAX_CLSID_BUFFER];
              aafNumSlots_t  numSlots;
              aafUID_t    mobID = {0};
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

                hr = aMob->GetNumSlots (&numSlots);
                check(hr); // display error message
                if (FAILED(hr))
                  numSlots = 0;
                printf("Found %ld slots\n", numSlots);

                hr = aMob->QueryInterface (IID_IAAFSourceMob, (void **)&smob);
                check(hr); // display error message
                if(SUCCEEDED(hr))
                {
                  IAAFEssenceDescriptor  *essenceDesc = NULL;
                  aafInt32 numLocators;


                  hr = smob->GetEssenceDescription(&essenceDesc);
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
                    hr = essenceDesc->GetNumLocators(&numLocators);
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


                  hr = aMob->EnumAAFAllMobSlots(&slotIter);
                  check(hr); // display error message
                  if(SUCCEEDED(hr))
                  {
                    IAAFMobSlot    *slot = NULL;
                    aafInt32 s;


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

          pHeader->Release();
          pHeader = NULL;
        }

        check(pFile->Close());

        pFile->Release();
        pFile = NULL;
      }
    }

    // check(pSession->EndSession());

    // pSession->Release();
    // pSession = NULL;
  }
}

static void CreateAAFFile(aafWChar * pFileName)
{
  // IAAFSession *        pSession = NULL;
  IAAFFile *          pFile = NULL;
  IAAFHeader *        pHeader = NULL;
  aafProductIdentification_t  ProductInfo;
  aafUID_t          newUID;

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

  /*
  check(CoCreateInstance(CLSID_AAFSession,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFSession, 
               (void **)&pSession));
  */
  check(CoCreateInstance(CLSID_AAFFile,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFFile, 
               (void **)&pFile));
    
  // check(pSession->SetDefaultIdentification(&ProductInfo));

  // check(pSession->CreateFile(pFileName, kAAFRev1, &pFile));
  check(pFile->Initialize());
  check(pFile->OpenNewModify(pFileName, 0, &ProductInfo));
  
  check(pFile->GetHeader(&pHeader));
   
//Make the first mob
  IAAFMob            *pMob = NULL;
  long  test;
  aafWChar    *names[5] = { L"FOOBAR1", L"FOOBAR2", L"FOOBAR3", L"FOOBAR4", L"FOOBAR5" };
  aafRational_t  editRate = { 2997, 100 };
  IAAFMobSlot    *newSlot = NULL;
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

    {
      HRESULT stat;
      stat = CoCreateInstance(CLSID_AAFNetworkLocator,
                  NULL, 
                  CLSCTX_INPROC_SERVER, 
                  IID_IAAFLocator, 
                  (void **)&pLocator);
      check (stat);
    }
    check(fileDesc->SetSampleRate(&audioRate));

    check(essenceDesc->AppendLocator (pLocator));
    check(smob->SetEssenceDescription(essenceDesc));

    // Add some slots
    for(testSlot = 0; testSlot < 3; testSlot++)
    {
       check(CoCreateInstance(CLSID_AAFSourceClip,
               NULL, 
               CLSCTX_INPROC_SERVER, 
               IID_IAAFSourceClip, 
               (void **)&sclp));    //!!!Temp, abstract superclass
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
  pHeader->Release();
  pHeader = NULL;
  
  check(pFile->Close());
  if (pFile)
    pFile->Release();

  // check(pSession->EndSession());
  
  // if (pSession)
  //   pSession->Release();
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

