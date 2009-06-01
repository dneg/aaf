//@doc
//@class    AAFConstantValue | Implementation class for AAFConstantValue
#ifndef __ImplAAFConstantValue_h__
#define __ImplAAFConstantValue_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


class ImplAAFDataDef;
class ImplAAFParameter;


#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif
#include "OMVariableSizeProperty.h"

class ImplAAFTypeDef;

class ImplAAFConstantValue : public ImplAAFParameter
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFConstantValue ();


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] // Parameter definition for this object (this determines the type of the constant value)
         ImplAAFParameterDef * pParameterDef,
         
         // @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [in, size_is(valueSize)] buffer containing value
         aafDataBuffer_t  pValue);


protected:
  virtual ~ImplAAFConstantValue ();

public:


	
/****/
  //****************
  // GetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValue
        (// @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [out, size_is(valueSize),length_is(*bytesRead)] Preallocated buffer to hold value
         aafDataBuffer_t  pValue,

         // @parm [out] Number of actual bytes read
         aafUInt32*  bytesRead);


  //****************
  // GetValueBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValueBufLen
        // @parm [out] Mob Name
        (aafUInt32 *  pLen);

/****/
  //****************
  // SetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetValue
        (// @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [in, size_is(valueSize)] buffer containing value
         aafDataBuffer_t  pValue);

  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef);

private:
  OMVariableSizeProperty<aafUInt8>	_value;

  ImplAAFTypeDef * _cachedTypeDef;
};

#endif // ! __ImplAAFConstantValue_h__


