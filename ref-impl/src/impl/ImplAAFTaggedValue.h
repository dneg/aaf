//@doc
//@class    AAFTaggedValue | Implementation class for AAFTaggedValue
#ifndef __ImplAAFTaggedValue_h__
#define __ImplAAFTaggedValue_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


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
         wchar_t *  pName,

         // @parm [in] Type definition of this tagged value object
         aafUID_t * pDatadef);

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
        (aafInt32 *  pLen);

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
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTaggedValue)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTaggedValueTest.cpp.
  static AAFRESULT test();

	// persistent data
	OMWideStringProperty				_name;
	OMFixedSizeProperty<aafUID_t>		_type;
	OMVariableSizeProperty<aafUInt8>	_value;

};

#endif // ! __ImplAAFTaggedValue_h__


