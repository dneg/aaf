//@doc
//@class    AAFSelector | Implementation class for AAFSelector
#ifndef __ImplAAFSelector_h__
#define __ImplAAFSelector_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


class ImplAAFSegment;

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFSegment> ImplEnumAAFSegments;

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include "OMStrongRefProperty.h"
#include "OMStrongRefVectorProperty.h"

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
  // RemoveAlternateSegment()
  //
	virtual AAFRESULT STDMETHODCALLTYPE
    RemoveAlternateSegment
		(ImplAAFSegment* pSegment);

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
	virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);

private:
	OMStrongReferenceProperty<ImplAAFSegment>		_selected;
	OMStrongReferenceVectorProperty<ImplAAFSegment>	_alternates;
};

#endif // ! __ImplAAFSelector_h__


