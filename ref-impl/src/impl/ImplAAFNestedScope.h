//@doc
//@class    AAFNestedScope | Implementation class for AAFNestedScope
#ifndef __ImplAAFNestedScope_h__
#define __ImplAAFNestedScope_h__


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


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

/*************************************************************************
 * 
 * @class AAFNestedScope | an AAFNestedScope object defines a scope that 
 *			contains an ordered set of AAFSegments and produces the value
 *			specified by the last AAFSegement in the ordered set.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/

#include "ImplAAFSegment.h"

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFSegment> ImplEnumAAFSegments;

#include "OMStrongRefVectorProperty.h"

class ImplAAFNestedScope : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFNestedScope ();

protected:
  virtual ~ImplAAFNestedScope ();

public:


  //****************
  // AppendSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendSegment
        // @parm [in] Pointer to segment to be added
        (ImplAAFSegment * pSegment);

  //****************
  // PrependSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependSegment
        // @parm [in] Pointer to segment to be added
        (ImplAAFSegment * pSegment);

  //****************
  // InsertSegmentAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertSegmentAt
        // @parm [in] index where segment is to be inserted
        (aafUInt32 index,
        // @parm [in] Pointer to segment to be added
		 ImplAAFSegment * pSegment);

  //****************
  // CountSegments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountSegments
        // @parm [out\, retval] number of segments
        (aafUInt32 * pResult);

  //****************
  // RemoveSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveSegmentAt
        // @parm [in] index of segment to be removed
        (aafUInt32 index);

  //****************
  // GetSegmentAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSegmentAt
        // @parm [in] index of segment to retrieve
        (aafUInt32 index,
        // @parm [out, retval] retrieved segment
		 ImplAAFSegment ** ppSegment);

  //****************
  // GetSegments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSegments
        // @parm [retval][out] Slots - segment list  enumeration
        (ImplEnumAAFSegments ** ppEnum);

public:
	// SDK-internal

	virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);


private:

  // Persistent Properties	
  OMStrongReferenceVectorProperty<ImplAAFSegment>		_slots;
};

#endif // ! __ImplAAFNestedScope_h__


