/***********************************************************************
 *
 *              Copyright (c) 2000 Avid Technology, Inc.
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

#include "ImplAAFWeakRefValue.h"

#ifndef __ImplAAFStorable_h__
#include "ImplAAFStorable.h"
#endif

// TBD: Note the following include for ImplAAFClassDef.h really belongs
// in ImplAAFTypeDefWeakObjRef.h!
#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif


#include "OMProperty.h"
//#include "OMRefProperty.h" // TBD: include header for class for singleton strong references.

#include <assert.h>
#include <string.h>


ImplAAFWeakRefValue::ImplAAFWeakRefValue ()
{}


ImplAAFWeakRefValue::~ImplAAFWeakRefValue ()
{
}







AAFRESULT ImplAAFWeakRefValue::Initialize (
  const ImplAAFTypeDefWeakObjRef *referenceType,    
  OMProperty *property)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  result = ImplAAFRefValue::Initialize(referenceType, property);
  
  if (AAFRESULT_SUCCEEDED(result))
  {
    // This instance is now fully initialized.
    setInitialized();
  }
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE ImplAAFWeakRefValue::GetObject(ImplAAFStorable **ppObject) const
{
  AAFRESULT result = AAFRESULT_NOT_IMPLEMENTED;
  
  if (NULL == ppObject)
    return AAFRESULT_NULL_PARAM;
  
  return result;
}


AAFRESULT STDMETHODCALLTYPE ImplAAFWeakRefValue::SetObject(ImplAAFStorable *pObject)
{
  AAFRESULT result = AAFRESULT_NOT_IMPLEMENTED;
  
  if (NULL == pObject)
    return AAFRESULT_NULL_PARAM;
  
  return result;
}

//
// WriteTo
//
AAFRESULT STDMETHODCALLTYPE ImplAAFWeakRefValue::WriteTo(
  OMProperty* pOmProp)
{
  return (ImplAAFRefValue::WriteTo(pOmProp));
}
