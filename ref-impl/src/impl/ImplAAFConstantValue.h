//@doc
//@class    AAFConstantValue | Implementation class for AAFConstantValue
#ifndef __ImplAAFConstantValue_h__
#define __ImplAAFConstantValue_h__

/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/


class ImplAAFDataDef;
class ImplAAFParameter;


#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif


class ImplAAFConstantValue : public ImplAAFParameter
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFConstantValue ();

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


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFConstantValue)

private:
  OMVariableSizeProperty<aafUInt8>	_value;
};

#endif // ! __ImplAAFConstantValue_h__


