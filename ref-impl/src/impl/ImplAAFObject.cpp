/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include <assert.h>


ImplAAFObject::ImplAAFObject ()
{}


ImplAAFObject::~ImplAAFObject ()
{}


// Increment the object reference count.
size_t ImplAAFObject::AcquireReference()
{
  // Delegate to root class to increment the reference
  // count for this implementation object's COM or C++ API object.
  return (size_t)AcquireRef();
}

// Decrement the object reference count and delete the container.
size_t ImplAAFObject::ReleaseReference()
{
  // If there is only a single reference left it is the container's
  // reference to this implementation object. Tell the container to
  // Delete itself. The container's destructor will delete this
  // object.
  return (size_t)ReleaseRef();
}

// Just return the count. (this could be inline in the header...)
size_t ImplAAFObject::ReferenceCount()
{
  // Delegate to root class to return the reference
  // count for this implementation object's COM or C++ API object.
  return (size_t)RefCount();
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::SetGeneration (aafUID_t *  /*pGeneration*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetGeneration (aafUID_t *  /*pGeneration*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetStoredByteOrder (aafByteOrder_t *  /*pOrder*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetNativeByteOrder (aafByteOrder_t *  /*pOrder*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

//************
// Interfaces ivisible inside the toolkit, but not exposed through the API

// Gets the head object of the file containing this object.
// This function is used to maintain MOB and Definition tables in the
// head object.
AAFRESULT ImplAAFObject::MyHeadObject
	(ImplAAFHeader **header)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


extern "C" const aafClassID_t CLSID_AAFObject;

OMDEFINE_STORABLE(ImplAAFObject, CLSID_AAFObject);
