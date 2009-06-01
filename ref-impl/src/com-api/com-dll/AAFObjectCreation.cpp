//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFObjectCreation.h"
#include "AAF.h"
#include "AAFPrivate.h"
#include "ImplAAFObject.h"
#include "ImplAAFPluginManager.h"

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

// Initializes the given Impl object's container by calling
// the container's initialization method.

static AAFRESULT InitializePluginFromDefinitionProc(
  aafUID_constref /* defID */, 
  REFCLSID clsid, 
  IAAFRoot * pRoot)
{
   assert(NULL != pRoot);
   return pRoot->InitializeExtension(clsid);
}

AAFRESULT InitializeImplExtension(ImplAAFRoot * pObj, aafUID_constref defID)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  IAAFRoot * pRoot = static_cast<IAAFRoot *>(pObj->GetContainer());
  assert(pRoot);

  // If the plugin manager has an associated code clsid then attempt
  // to initialize the extension object.
	// NOTE: Need an iterator interface for plugins...
	//
  ImplAAFPluginManager *pPluginMgr = ImplAAFPluginManager::GetPluginManager();
  if (NULL != pPluginMgr)
  {
    result = pPluginMgr->ForEachPluginWithDefinitionDo(
      defID, 
      (AAFDOTOPLUGINWITHDEFINTIONPROC)InitializePluginFromDefinitionProc, 
      pRoot);

    pPluginMgr->ReleaseReference();
  }
  else
  { // GetPluginManager failed!
    result = AAFRESULT_INVALID_OBJ;
  }
  
  return result;
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
