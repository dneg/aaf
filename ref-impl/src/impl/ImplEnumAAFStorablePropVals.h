//@doc
//@class    EnumAAFStorablePropVals | Implementation class for EnumAAFStorablePropVals
#ifndef __ImplEnumAAFStorablePropVals_h__
#define __ImplEnumAAFStorablePropVals_h__


/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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


