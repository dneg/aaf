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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

#include "ImplAAFObjectCreation.h"
#include "AAF.h"
#include "AAFRoot.h"
#include "ImplAAFRoot.h"
#include "ImplAAFContentStorage.h"

#include <string.h>
#include <assert.h>


// Creates and returns an Impl object based on the given class ID.
// Will create the appropriate kind of API class and attach it.
//
// Implementation note:  This function does the following:
// 1) Create the API class.  (The API class will create the Impl class
//    and attach it.)
// 2) Ask the newly created API class for its contained Impl class.
// 3) Return that Impl class.
//
ImplAAFRoot * CreateImpl (const aafClassID_t & rClassID)
{
	IAAFRoot	*pIAAFRoot;
	ImplAAFRoot	*implRoot;
	HRESULT		hr;
	CLSID           classID;

	// Cast (by bitwise copy) from aafClassID_t to CLSID.
	//
	memcpy(&classID, &rClassID, sizeof(CLSID));

	// The reference implementation must be "self-contained". We do
	// not want any user supplied classes to be created and used
	// instead on one our built-in classes.
	//
	// The simplest change is to just simulate a call to 
	// CoCreateInstance:
	//
	// This code is invoked within the current module so we
	// should just be able to call the DllGetClassObject entry point
	// instead of calling CoCreateInstance and searching the 
	// registry.
	IClassFactory *pFactory = NULL;
	hr = DllGetClassObject(classID, IID_IClassFactory, (void **)&pFactory);
	if (SUCCEEDED(hr))
	{
		hr = pFactory->CreateInstance(NULL, IID_IAAFRoot, (void **)&pIAAFRoot);
		pFactory->Release();
	}

	if (SUCCEEDED(hr))
		pIAAFRoot->GetImplRep((void **)&implRoot);
	else
		implRoot = NULL;

	return (implRoot);
}

// Deletes the given Impl object.  Will make sure the associated API
// class is also deleted.
//
// Implementation note:  This function does the following:
// 1) Ask the given Impl object for its containing API object.
// 2) Delete that containing API object.  (The API class will
//    automatically delete the impl class.)
//
void DeleteImpl (ImplAAFRoot *& pObj)
{
  ReleaseImplReference(pObj);
  pObj = NULL;
}


// Increases the reference count of the API container object.
aafUInt32 AcquireImplReference(ImplAAFRoot *pObj)
{
  IAAFRoot * pRoot = static_cast<IAAFRoot *>(pObj->GetContainer());
  assert(pRoot);
  return pRoot->AddRef();
}

// Decreases the reference count of the API container object by one.
// If the count goes to zero then the given Impl object will be deleted
// with the corresponding API container object.
aafUInt32 ReleaseImplReference(ImplAAFRoot *pObj)
{
  IAAFRoot * pRoot = static_cast<IAAFRoot *>(pObj->GetContainer());
  assert(pRoot);
  return pRoot->Release();
}

// Return the reference count of the given implementation object
// container's reference count.
aafUInt32 ReferenceImplCount(ImplAAFRoot *pObj)
{
  IAAFRoot * pRoot = static_cast<IAAFRoot *>(pObj->GetContainer());
  assert(pRoot);
  pRoot->AddRef();
  return pRoot->Release();
}
