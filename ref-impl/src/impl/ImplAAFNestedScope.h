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

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

class ImplEnumAAFSegments;






#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


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
  // RemoveSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveSegment
        // @parm [in] Pointer to segment to be added
        (ImplAAFSegment * pSegment);

  //****************
  // GetSlots()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSlots
        // @parm [retval][out] Slots - segment list  enumeration
        (ImplEnumAAFSegments ** ppEnum);

public:
	// SDK-internal

	virtual AAFRESULT ChangeContainedReferences(aafUID_t *from, aafUID_t *to);


private:

  // Persistent Properties	
  OMStrongReferenceVectorProperty<ImplAAFSegment>		_slots;
};

#endif // ! __ImplAAFNestedScope_h__


