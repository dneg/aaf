//@doc
//@class    AAFSourceReference | Implementation class for AAFSourceReference
#ifndef __ImplAAFSourceReference_h__
#define __ImplAAFSourceReference_h__

#include "OMStorable.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include "ImplAAFObject.h"
#include "OMProperty.h"

// Persistent Property IDs

const int PID_SOURCEREFERENCE_SOURCEID		= 1;
const int PID_SOURCEREFERENCE_SRCMOBSLOTID	= 2;

class ImplAAFSourceReference : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSourceReference ();
  virtual ~ImplAAFSourceReference ();



  //****************
  // GetSourceID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceID
		// @parm [retval][out] Place to put source ID
        (aafUID_t *  pSourceID);

  //****************
  // SetSourceID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceID
		// @parm [in] Source ID to set
        (aafUID_t   sourceID);

  //****************
  // GetSourceMobSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceMobSlotID
		// @parm [retval][out] Place to put source mob slot ID
        (aafTrackID_t *  pMobSlotID);

  //****************
  // SetSourceMobSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceMobSlotID
		// @parm [in] Source Mob ID to set
        (aafTrackID_t   mobSlotID);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(AAFSourceReference);

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFSourceReferenceTest.cpp.
  static AAFRESULT test();
private:
	OMFixedSizeProperty<aafUID_t>	_sourceID;
	OMFixedSizeProperty<aafInt32>	_sourceMobSlotId;

};

#endif // ! __ImplAAFSourceReference_h__

