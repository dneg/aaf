#ifndef IMPLAAFOBJECTCREATION_H
#define IMPLAAFOBJECTCREATION_H

#include "AAFTypes.h"

class ImplAAFRoot;

// Creates and returns an Impl object based on the given class ID.
// Will create the appropriate kind of API class and attach it.
//
// Implementation note:  This function does the following:
// 1) Create the API class.  (The API class will create the Impl class
//    and attach it.)
// 2) Ask the newly created API class for its contained Impl class.
// 3) Return that Impl class.
//
ImplAAFRoot * CreateImpl (const aafClassID_t & rClassID);

// Deletes the given Impl object.  Will make sure the associated API
// class is also deleted.
//
// Implementation note:  This function does the following:
// 1) Ask the given Impl object for its containing API object.
// 2) Delete that containing API object.  (The API class will
//    automatically delete the impl class.)
//
void DeleteImpl (ImplAAFRoot *& pObj);

// Increases the reference count of the API container object.
aafUInt32 AcquireImplReference(ImplAAFRoot *pObj);

// Decreases the reference count of the API container object by one.
// If the count goes to zero then the given Impl object will be deleted
// with the corresponding API container object.
aafUInt32 ReleaseImplReference(ImplAAFRoot *pObj);

// Return the reference count of the given implementation object
// container's reference count.
aafUInt32 ReferenceImplCount(ImplAAFRoot *pObj);

#endif
