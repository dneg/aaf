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

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif




#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefSet::ImplAAFTypeDefSet ()
{}


ImplAAFTypeDefSet::~ImplAAFTypeDefSet ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::Initialize (
      const aafUID_t *  /*pID*/,
      ImplAAFTypeDef * /*pTypeDef*/,
      aafCharacter *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::GetType (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::GetCount (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafUInt32 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::AddElement (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplAAFPropertyValue * /*pMemberPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::CreateValueFromValues (
      ImplAAFPropertyValue ** /*pElementValues*/,
      aafUInt32  /*numElements*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::CreateValueFromCArray (
      aafMemPtr_t  /*pInitData*/,
      aafUInt32  /*initDataSize*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::GetElements (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplEnumAAFPropertyValues ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


aafBool ImplAAFTypeDefSet::IsFixedSize (void) const
{
  assert (0);
  return AAFFalse; // Not reached!
}


size_t ImplAAFTypeDefSet::PropValSize (void) const
{
  assert (0);
  return 0; // Not reached!
}


aafBool ImplAAFTypeDefSet::IsRegistered (void) const
{
  assert (0);
  return AAFFalse; // Not reached!
}


size_t ImplAAFTypeDefSet::NativeSize (void) const
{
  assert (0);
  return 0; // Not reached!
}


OMDEFINE_STORABLE(ImplAAFTypeDefSet, AUID_AAFTypeDefSet);
