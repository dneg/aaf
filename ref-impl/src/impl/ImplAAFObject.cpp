
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


// AxD_XMETHOD1(itsFile,
// 			[out],objOut,AAFFile,aFile, ,
// 			Returns the AAFFile containing the object in persistent
//   // store\, or NULL for transient objects.)

AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::isForeignByteOrder (aafBool*  /*isForeign*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  //@comm This routine is optimized in the common case (objects byte order == file byte order).	
  //@comm Replaces ompvtIsForeignByteOrder

AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::Delete ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  //@comm Replaces omfsObjectDelete

AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::IsTypeOf (aafUID_t      /*aClass*/,
                           aafBool*  /*Result*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}
 

