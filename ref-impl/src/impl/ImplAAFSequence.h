//@doc
//@class    AAFSequence | Implementation class for AAFSequence
#ifndef __ImplAAFSequence_h__
#define __ImplAAFSequence_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
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
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
		// @parm [in] Data Definition object
        (aafUID_t * pDatadef);


  //****************
  // AppendComponent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendComponent
		// @parm [in] Component to append to the sequence
        (ImplAAFComponent * pComponent);


  //****************
  // RemoveComponent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveComponent
		// @parm [in] Component to append to the sequence
        (ImplAAFComponent * pComponent);


  //****************
  // GetNumComponents()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumComponents
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

  // Interfaces visible inside the toolkit, but not exposed through the API
  AAFRESULT GetNthComponent(aafUInt32 index, ImplAAFComponent **ppComponent);
	virtual AAFRESULT ChangeContainedReferences(aafUID_t *from, aafUID_t *to);
  AAFRESULT
    SetNthComponent (aafUInt32 index, ImplAAFComponent* pComponent);

private:
	OMStrongReferenceVectorProperty<ImplAAFComponent> _components;

};

#endif // ! __ImplAAFSequence_h__

