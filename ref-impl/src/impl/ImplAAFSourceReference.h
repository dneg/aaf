//@doc
//@class    AAFSourceReference | Implementation class for AAFSourceReference
#ifndef __ImplAAFSourceReference_h__
#define __ImplAAFSourceReference_h__

#include "OMStorable.h"

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

private:
	OMFixedSizeProperty<aafUID_t>	_sourceID;
	OMFixedSizeProperty<aafInt32>	_sourceMobSlotId;

};

#endif // ! __ImplAAFSourceReference_h__

