//@doc
//@class    EnumAAFStorablePropVals | Implementation class for EnumAAFStorablePropVals
#ifndef __ImplEnumAAFStorablePropVals_h__
#define __ImplEnumAAFStorablePropVals_h__


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








class ImplAAFRefContainerValue;
class ImplAAFTypeDefObjectRef;
class OMReferenceContainerIterator;

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif


class ImplEnumAAFStorablePropVals : public ImplEnumAAFPropertyValues
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFStorablePropVals ();

protected:
  virtual ~ImplEnumAAFStorablePropVals ();

public:
  
  //****************
  // Initialize()
  //
  AAFRESULT STDMETHODCALLTYPE
    Initialize
      (ImplAAFRefContainerValue* pContainerValue,
       OMReferenceContainerIterator* containerIterator);
  
  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next Property Value
        (ImplAAFPropertyValue ** ppPropertyValue);


  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of objects requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive Property Values
         ImplAAFPropertyValue ** ppPropertyValues,

         // @parm [out,ref] number of actual Property Values fetched into ppPropertyValues
	// array
         aafUInt32 *  pFetched);


  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
        // @parm [in] Number of elements to skip
        (aafUInt32  count);


  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();



  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
        // @parm [out,retval] new enumeration
        (ImplEnumAAFPropertyValues ** ppEnum);
        
        
protected:
  ImplAAFRefContainerValue* _containerValue;
  ImplAAFTypeDefObjectRef* _elementType;
	OMReferenceContainerIterator* _containerIterator;
};

#endif // ! __ImplEnumAAFStorablePropVals_h__


