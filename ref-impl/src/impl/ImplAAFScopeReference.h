//@doc
//@class    AAFScopeReference | Implementation class for AAFScopeReference
#ifndef __ImplAAFScopeReference_h__
#define __ImplAAFScopeReference_h__


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


