//@doc
//@class    AAFSequence | Implementation class for AAFSequence
#ifndef __ImplAAFSequence_h__
#define __ImplAAFSequence_h__

#include "OMStorable.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

class ImplAAFTimecode;

class ImplAAFComponent;

class ImplAAFSegment;

class ImplAAFDataDef;

class ImplEnumAAFComponents;





#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


const int PID_SEQUENCE_COMPONENTS	= 30;

class ImplAAFSequence : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSequence ();
  virtual ~ImplAAFSequence ();


  //****************
  // SetInitialValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetInitialValue
		// @parm [in] Data Definition object
        (aafUID_t * pDatadef);


  //****************
  // AppendCpnt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendCpnt
		// @parm [in] Component to append to the sequence
        (ImplAAFComponent * pComponent);


  //****************
  // RemoveCpnt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveCpnt
		// @parm [in] Component to append to the sequence
        (ImplAAFComponent * pComponent);


  //****************
  // GetNumCpnts()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumCpnts
		// @parm [out] Number of components
        (aafInt32 *  pNumCpnts);


  //****************
  // EnumComponents()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumComponents
		// @parm [out, retval] Component Enumeration
        (ImplEnumAAFComponents ** ppEnum);


  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC (/*[in]*/ aafPosition_t *  pOffset,
	  /*[out]*/ aafTimecode_t *  pTimecode);

  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset (/*[in]*/ aafTimecode_t *  pTimecode,
	  /*[in]*/ aafRational_t *  pEditRate,
	  /*[out]*/ aafFrameOffset_t *  pOffset);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFSequence)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFSequenceTest.cpp.
  static AAFRESULT test();

  // Interfaces visible inside the toolkit, but not exposed through the API
  AAFRESULT GetNthComponent(aafUInt32 index, ImplAAFComponent **ppComponent);

private:
	OMStrongReferenceVectorProperty<ImplAAFComponent> _components;

};

#endif // ! __ImplAAFSequence_h__

