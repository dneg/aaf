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
struct IAAFObject;
class AAFObject;

#include "AAFTypes.h"
#if OM_PRESENT
#include "OMStorable.h"

class ImplAAFObject : public OMStorable
#else
class ImplAAFObject
#endif
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
 

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFObjectTest.cpp.
  static AAFRESULT test();

  void InitContainer (void * pContainer);

  void * GetContainer ();

private:

  void * _pContainer;

};

#endif // ! __ImplAAFObject_h__

