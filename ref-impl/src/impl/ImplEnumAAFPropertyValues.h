//@doc
//@class    EnumAAFPropertyValues | Implementation class for EnumAAFPropertyValues
#ifndef __ImplEnumAAFPropertyValues_h__
#define __ImplEnumAAFPropertyValues_h__


//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



class ImplAAFPropertyValue;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "ImplAAFEnumerator.h"
#include "ImplAAFTypeDefArray.h"
#include "ImplAAFPropertyValue.h"

class ImplEnumAAFPropertyValues : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFPropertyValues ();

protected:
  virtual ~ImplEnumAAFPropertyValues ();

public:

  // Internal initialize method.
  AAFRESULT STDMETHODCALLTYPE Initialize( ImplAAFTypeDefArray* pDef,
					  ImplAAFPropertyValue* pVal );

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


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFPropertyValuesTest.cpp.
  static AAFRESULT test();
 
private:
  ImplAAFTypeDefArray* _pDef;
  ImplAAFPropertyValue* _pVal;
  aafUInt32 _count;
  aafUInt32 _current;
};

#endif // ! __ImplEnumAAFPropertyValues_h__


