//@doc
//@class    AAFSelector | Implementation class for AAFSelector
#ifndef __ImplAAFSelector_h__
#define __ImplAAFSelector_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


class ImplAAFSegment;

class ImplEnumAAFSegments;



#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFSelector : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSelector ();

protected:
  virtual ~ImplAAFSelector ();

public:


  //****************
  // GetSelectedSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSelectedSegment
        // @parm [out] Returned the selected Segment object
        (ImplAAFSegment ** ppSelSegment);

  //****************
  // SetSelectedSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSelectedSegment
        // @parm [in] A Segment object
        (ImplAAFSegment * pSelSegment);

  //****************
  // AppendAlternateSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendAlternateSegment
        // @parm [in] Segment to append to the Alternate list of segments
        (ImplAAFSegment * pSegment);


  //****************
  // GetNumAlternateSegments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumAlternateSegments
        // @parm [out] Number of Alternate Segments
        (aafInt32 *  pNumSegments);

  //****************
  // EnumAlternateSegments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumAlternateSegments
        // @parm [out, retval] Segment Enumeration
        (ImplEnumAAFSegments ** ppEnum);

  AAFRESULT GetNthSegment(aafUInt32 index, ImplAAFSegment** ppSegment);
public:
	//SDK-private calls
	virtual AAFRESULT GetMinimumBounds(aafPosition_t rootPos, aafLength_t rootLen,
										ImplAAFMob *mob, ImplAAFMobSlot *track,
										aafMediaCriteria_t *mediaCrit,
										aafPosition_t currentObjPos,
										aafOperationChoice_t *effectChoice,
										ImplAAFComponent	*prevObject,
										ImplAAFComponent *nextObject,
										ImplAAFScopeStack *scopeStack,
										aafPosition_t *diffPos, aafLength_t *minLength,
										ImplAAFOperationGroup **groupObject, aafInt32	*nestDepth,
										ImplAAFComponent **found, aafBool *foundTransition);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFSelector)

private:
	OMStrongReferenceProperty<ImplAAFSegment>		_selected;
	OMStrongReferenceVectorProperty<ImplAAFSegment>	_alternates;
};

#endif // ! __ImplAAFSelector_h__


