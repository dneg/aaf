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

#endif

static void     FatalErrorCode(HRESULT errcode, int line, char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
  // we don't need to exit on failure
  //exit(1); 
}

static HRESULT moduleErrorTmp = S_OK;/* note usage in macro */
#define check(a)  \
{  moduleErrorTmp = a; \
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
  hr = CoCreateInstance(CLSID_AAFSession,
                        NULL, 
                        CLSCTX_INPROC_SERVER, 
                        IID_IAAFSession, 
                        (void **)&pSession);
  */
  hr = CoCreateInstance(CLSID_AAFFile,
                        NULL, 
                        CLSCTX_INPROC_SERVER, 
                        IID_IAAFFile, 
                        (void **)&pFile);
  check(hr); // display error message
  if (SUCCEEDED(hr))
  {
    aafProductIdentification_t	ProductInfo;


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
    if (SUCCEEDED(hr))
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
          IAAFIdentification *    pIdent = NULL;

          hr = pHeader->GetLastIdentification(&pIdent);
          check(hr); // display error message
          if (SUCCEEDED(hr))
          {
            fprintf(stdout, "LastIdentification\n");
            printIdentification(pIdent);

            pIdent->Release();
            pIdent = NULL;

            aafNumSlots_t n;
            hr = pHeader->GetNumMobs(kAllMob, &n);
            check(hr);
            printf("Number of Mobs       = %d\n", n);
          }
          pHeader->Release();
          pHeader = NULL;
        }
        
        hr = pFile->Close();
        check(hr);

        pFile->Release();
        pFile = NULL;
      }
    }

    // pSession->EndSession(); // obsolete!

    // pSession->Release();
    // pSession = NULL;
  }
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

int main(int argumentCount, char* argumentVector[])
{
  if (argumentCount != 2) {
    fprintf(stderr, "Error : wrong number of arguments\n");
    return(1);
  }

  char* inputFileName = argumentVector[1];

  wchar_t wInputFileName[256];
  convert(wInputFileName, 256, inputFileName);

  CComInitialize comInit;

  ReadAAFFile(wInputFileName);

  fprintf(stdout, "Done\n");

  return(0);
}

