//@doc
//@class    AAFControlPoint | Implementation class for AAFControlPoint
#ifndef __ImplAAFControlPoint_h__
#define __ImplAAFControlPoint_h__


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



/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/ 

class ImplAAFDataDef;






#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

class ImplAAFTypeDef;

class ImplAAFControlPoint : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFControlPoint ();

protected:
  virtual ~ImplAAFControlPoint ();

public:


  //****************
  // GetTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTime
        // @parm [out,retval] Pointer to an aafRational_t
        (aafRational_t *  pTime);

  //****************
  // GetEditHint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEditHint
        // @parm [out,retval] Pointer to an aafEditHint_t
        (aafEditHint_t *  pEditHint);

  //****************
  // GetValueBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValueBufLen
        // @parm [out] Pointer to an variable used to return the length
        (aafUInt32 *  pLen);

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
  // SetTime()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTime
        // @parm [in] Control Point time
        (aafRational_t  pTime);
			// Sets the position of the control point within an effect, expressed as a rational
  // running from 0 to 1.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect\\\, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  \\\(This is the only code indicating success.\\\)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize\\\(\\\) called on it.)

  //****************
  // SetEditHint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEditHint
        // @parm [in] Control Point Edit hint
        (aafEditHint_t  editHint);

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
    SetTypeDefinition (
      ImplAAFTypeDef*  pTypeDef);

  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef);

private:
	OMFixedSizeProperty<aafUID_t>		_type;
	OMFixedSizeProperty<aafRational_t>	_time;
	OMVariableSizeProperty<aafUInt8>	_value;
	OMFixedSizeProperty<aafUInt16>		_hint;
};

#endif // ! __ImplAAFControlPoint_h__



