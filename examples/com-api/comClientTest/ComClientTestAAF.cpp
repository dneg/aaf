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

#include "aafErr.h"		// TODO: this file needs to be moved to a public include directory...
#include "AAFTypes.h"
#include "AAFUtils.h"	// TODO: this file needs to be moved to a public include directory...
#include "AAF.h"

// TODO: these should not be here, I added them for now to get a good link
const CLSID CLSID_AAFSession = { 0xF0C10891, 0x3073, 0x11d2, { 0x80, 0x4A, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
const aafProductVersion_t AAFToolkitVersion = {2, 1, 0, 1, kVersionBeta};

static void     FatalErrorCode(aafErr_t errcode, int line, char *file)
{
// TODO: Fix this later. aafGetErrorString unresolved at link
//  printf("Error '%s' returned at line %d in %s\n",
//	 aafGetErrorString(errcode), line, file);
  exit(1);
}

static void     FatalError(char *message)
{
  printf(message);
  exit(1);
}

static aafErr_t moduleErrorTmp = OM_ERR_NONE;/* note usage in macro */
#define check(a)  \
{ moduleErrorTmp = a; \
  if (moduleErrorTmp != OM_ERR_NONE) \
     FatalErrorCode(moduleErrorTmp, __LINE__, __FILE__);\
}

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}

void ReadAAFFile(unsigned char * pFileName, aafProductIdentification_t *pProductInfo)
{
	IAAFSession *	pISessionPtr = NULL;
	IAAFFile *	pIFilePtr = NULL;
	HRESULT		hr;
	aafFileRev_t FileRev;
	  
	hr = CoCreateInstance(CLSID_AAFSession,
						  NULL, 
						  CLSCTX_INPROC_SERVER, 
						  IID_IAAFSession, 
						  (void **)&pISessionPtr);
	check(hr);
	  
	hr = pISessionPtr->SetDefaultIdentification(pProductInfo);
	check(hr);

	hr = pISessionPtr->OpenReadFile(pFileName, &pIFilePtr);
	check(hr);
  
	hr = pIFilePtr->GetRevision(&FileRev);
	check(hr);

	if (FileRev == kAAFRev1)
		fprintf(stdout, "Successfullly read file revision.\n");
	else
		fprintf(stdout, "Failed to read file revision.\n");

	check(pIFilePtr->Close());

	check(pISessionPtr->EndSession());

	if (pIFilePtr) pIFilePtr->Release();
	if (pISessionPtr) pISessionPtr->Release();
}

void CreateAAFFile(unsigned char * pFileName, aafProductIdentification_t *pProductInfo)
{
	IAAFSession *	pISessionPtr = NULL;
	IAAFFile *	pIFilePtr = NULL;
	HRESULT		hr;

	hr = CoCreateInstance(CLSID_AAFSession,
						  NULL, 
						  CLSCTX_INPROC_SERVER, 
						  IID_IAAFSession, 
						  (void **)&pISessionPtr);
	check(hr);
    
	hr = pISessionPtr->SetDefaultIdentification(pProductInfo);
	check(hr);

	hr = pISessionPtr->CreateFile(pFileName, kAAFRev1, &pIFilePtr);
	check(hr);
  
	check(pIFilePtr->Close());

	check(pISessionPtr->EndSession());

	if (pIFilePtr) pIFilePtr->Release();
	if (pISessionPtr) pISessionPtr->Release();
}

main()
{
	aafProductIdentification_t ProductInfo;

	  
	CoInitialize(NULL);

	ProductInfo.companyName = (unsigned char *)"AAF Developers Desk";
	ProductInfo.productName = (unsigned char *)"Make AVR Example";
	ProductInfo.productVersion = AAFToolkitVersion;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	CreateAAFFile((unsigned char *)"Foo.aaf", &ProductInfo);
	ReadAAFFile((unsigned char *)"Foo.aaf", &ProductInfo);

	fprintf(stdout, "Done\n");

	CoUninitialize();

	return(0);
}

