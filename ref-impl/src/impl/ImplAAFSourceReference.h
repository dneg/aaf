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
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/





#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include "ImplAAFObject.h"
#include "OMProperty.h"


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
        (aafSlotID_t *  pMobSlotID);

  //****************
  // SetSourceMobSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceMobSlotID
		// @parm [in] Source Mob ID to set
        (aafSlotID_t   mobSlotID);

public:
	//SDK-private

	virtual AAFRESULT ChangeContainedReferences(aafUID_t *from, aafUID_t *to);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFSourceReference)

private:
	OMFixedSizeProperty<aafUID_t>	_sourceID;
	OMFixedSizeProperty<aafInt32>	_sourceMobSlotId;

};

#endif // ! __ImplAAFSourceReference_h__

