//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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

// Initializes the given Impl object's container by calling
// the container's initialization method.
AAFRESULT InitializeImplExtension(ImplAAFRoot * pObj, aafUID_constref defID);

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
