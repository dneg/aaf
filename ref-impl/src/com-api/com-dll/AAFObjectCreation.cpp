#include "ImplAAFObjectCreation.h"
#include "AAF.h"
#include "ImplAAFRoot.h"
#include "ImplAAFContentStorage.h"

#include <string.h>
#include <assert.h>

//JeffB: We currently have one object which exists in the file, but not in the API
//extern "C" const aafClassID_t
//CLSID_AAFContentStorage = { 0x54D4C481, 0x5F8B, 0x11d2, { 0x80, 0x73, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F} };

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

	hr = CoCreateInstance(classID,
				NULL, 
				CLSCTX_INPROC_SERVER, 
				IID_IAAFRoot, 
				(void **)&pIAAFRoot);

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
