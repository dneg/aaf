#include "ImplAAFObjectCreation.h"
#include "AAF.h"
#include "ImplAAFRoot.h"

#include <string.h>

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
}
