//@doc
//@class    AAFSelector | Implementation class for AAFSelector
#ifndef __ImplAAFSelector_h__
#define __ImplAAFSelector_h__


/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


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

private:
	OMStrongReferenceProperty<ImplAAFSegment>		_selected;
	OMStrongReferenceVectorProperty<ImplAAFSegment>	_alternates;
};

#endif // ! __ImplAAFSelector_h__


