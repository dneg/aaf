//@doc
//@class    AAFScopeReference | Implementation class for AAFScopeReference
#ifndef __ImplAAFScopeReference_h__
#define __ImplAAFScopeReference_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


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
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFScopeReference)

  // Persistent Properties
  OMFixedSizeProperty<aafUInt32>	_relativeScope;
  OMFixedSizeProperty<aafUInt32>	_relativeSlot;
};

#endif // ! __ImplAAFScopeReference_h__


