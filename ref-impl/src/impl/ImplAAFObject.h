//@doc
//@class    AAFObject | Implementation class for AAFObject
#ifndef __ImplAAFObject_h__
#define __ImplAAFObject_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





//
// Forward declaration
//
class AAFObject;

#include "AAFTypes.h"
#include "OMStorable.h"
#include "ImplAAFRoot.h"

#include <assert.h>

class ImplAAFObject : public OMStorable, public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFObject ();
  virtual ~ImplAAFObject ();


// AxD_XMETHOD1(itsFile,
// 			[out],objOut,AAFFile,aFile, ,
// 			Returns the AAFFile containing the object in persistent
//   // store\, or NULL for transient objects.)

  //****************
  // isForeignByteOrder()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    isForeignByteOrder
        (aafBool*  isForeign);  //@parm [out] 

  //@comm This routine is optimized in the common case (objects byte order == file byte order).	
  //@comm Replaces ompvtIsForeignByteOrder

  //****************
  // Delete()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Delete ();

  //@comm Replaces omfsObjectDelete

  //****************
  // IsTypeOf()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsTypeOf
        (aafUID_t      aClass,   //@parm [in] AUID corresponding to a class
		 aafBool*  Result);  //@parm [out] 

  // tjb - this is temporary and should be removed
  const OMClassId& classId(void) const
  {
    // This function must be overidden by subclasses. It is pure
    // virtual on OMStorable. It is defined here since it is not yet
    // overridden in the automatically generated Impl classes.
    //
    assert(!"This code should not be reached.");
    return nullOMClassId;
  }
 
public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFObjectTest.cpp.
  static AAFRESULT test();

};

#endif // ! __ImplAAFObject_h__

