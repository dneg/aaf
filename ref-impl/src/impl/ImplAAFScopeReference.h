//@doc
//@class    AAFScopeReference | Implementation class for AAFScopeReference
#ifndef __ImplAAFScopeReference_h__
#define __ImplAAFScopeReference_h__


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
 * @class AAFScopeReference | AAFScopeReference refers to a section in the 
 *			specified AAFNestedScope slo tor AAFMobSLot.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/




#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFScopeReference : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFScopeReference ();

protected:
  virtual ~ImplAAFScopeReference ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (
		 // @parm [in] Data definition for referenced slot
		 ImplAAFDataDef *pDataDef,

		 // @parm [in] Number of nested scopes to pass to find the Nested Scope slot
         aafUInt32  RelativeScope,

         // @parm [in] Number of slots that preced the slot containing the Scope Reference
         aafUInt32  RelativeSlot);


  //****************
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
        (// @parm [in] Number of nested scopes to pass to find the Nested Scope slot
         aafUInt32  RelativeScope,

         // @parm [in] Number of slots that preced the slot containing the Scope Reference
         aafUInt32  RelativeSlot);

  //****************
  // GetRelativeScope()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRelativeScope
        // @parm [retval][out] Pointer to a Relative Scope
        (aafUInt32 *  pnRelativeScope);

  //****************
  // GetRelativeSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRelativeSlot
        // @parm [retval][out] Pointer to a Relative Slot
        (aafUInt32 *  pnRelativeSlot);



public:

  // Persistent Properties
  OMFixedSizeProperty<aafUInt32>	_relativeScope;
  OMFixedSizeProperty<aafUInt32>	_relativeSlot;
};

#endif // ! __ImplAAFScopeReference_h__


