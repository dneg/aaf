//@doc
//@class    AAFTaggedValue | Implementation class for AAFTaggedValue
#ifndef __ImplAAFTaggedValue_h__
#define __ImplAAFTaggedValue_h__


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

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMWideStringProperty.h"
#include "OMVariableSizeProperty.h"

class ImplAAFTypeDef;

class ImplAAFTaggedValue : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTaggedValue ();

protected:
  virtual ~ImplAAFTaggedValue ();

public:


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in, string] User defined name of this tagged value object
         const aafCharacter * pName,

         // @parm [in] Type Definition of the following value data
         ImplAAFTypeDef * pType,
         
         // @parm [in] Size of preallocated buffer
         aafUInt32  valueSize,

         // @parm [in, size_is(valueSize)] buffer containing value
         aafDataBuffer_t  pValue);

  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (// @parm [in,string] Tagged value Name
         wchar_t *  pName,

         // @parm [in] length of the buffer to hold the Tagged value's Name
         aafInt32  bufSize);

  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        // @parm [out] Name buffer length
        (aafUInt32 *  pLen);

  //****************
  // GetTypeDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinition
        // @parm [out] Type definition object
        (ImplAAFTypeDef ** ppTypeDef);
  // 
  // Succeeds if all of the following are true:
  // - the ppTypeDef pointer is valid.
  // 
  // If this method fails nothing will be written to *ppTypeDef.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect\, it will return the first one
  // encountered in the order given below:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  \(This is the only code indicating success.\)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize\(\) called on it.
  //
  // AAFRESULT_PROP_NOT_PRESENT
  //   - This property does not exist in the file.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppTypeDef arg is NULL.)

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
        // @parm [out] Pointer to an variable used to return the length
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



public:

	// persistent data
	OMWideStringProperty				_name;
	OMVariableSizeProperty<aafUInt8>	_value;

  bool _initialized;
  ImplAAFTypeDef * _cachedTypeDef;
};

#endif // ! __ImplAAFTaggedValue_h__


