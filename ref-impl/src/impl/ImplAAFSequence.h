//@doc
//@class    AAFSequence | Implementation class for AAFSequence
#ifndef __ImplAAFSequence_h__
#define __ImplAAFSequence_h__


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

