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

class ImplAAFObject : public OMStorable, public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFObject ();
  virtual ~ImplAAFObject ();


  //****************
  // SetGeneration()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetGeneration
		// @parm [in] Generation ID to which this object is to be set
        (aafUID_t *  pGeneration);


  //****************
  // GetGeneration()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGeneration
		// @parm [out] Generation ID into which this object's generation is to be written
        (aafUID_t *  pGeneration);


  //***********************************************************
  // METHOD NAME: GetStoredByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetStoredByteOrder |
  // Returns the "Endian-ness" in which the current object was or will
  // be stored.  If this is a transient object (i.e., one which has
  // not been persisted) then it will return the native byte order of
  // the platform on which this is running.
  //
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetStoredByteOrder (
    // @parm [out] aafByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    aafByteOrder_t *  pOrder
  );



  //***********************************************************
  // METHOD NAME: GetNativeByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetNativeByteOrder |
  // Returns the native "Endian-ness" of the platform on which this is
  // running.
  //
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetNativeByteOrder (
    // @parm [out] aafByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    aafByteOrder_t *  pOrder
  );


public:
	// Interfaces ivisible inside the toolkit, but not exposed through the API
  
	// Gets the head object of the file containing this object.
	// This function is used to maintain MOB and Definition tables in the
	// head object.
	virtual AAFRESULT MyHeadObject
		(class ImplAAFHeader **header);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFObjectTest.cpp.
  static AAFRESULT test();

  OMDECLARE_STORABLE(ImplAAFObject)
};


#endif // ! __ImplAAFObject_h__

