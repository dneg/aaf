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




#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefStream_h__
#include "ImplAAFTypeDefStream.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefStream::ImplAAFTypeDefStream ()
{}


ImplAAFTypeDefStream::~ImplAAFTypeDefStream ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::Initialize (
      const aafUID_t *  /*pID*/,
      ImplAAFTypeDef * /*pTypeDef*/,
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetType (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetCount (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafInt64 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::AppendElements (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplAAFPropertyValue ** /*ppMemberPropVals*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::CreateEmpty (
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetElementValues (
      ImplAAFPropertyValue * /*pInPropVal*/,
      aafInt64  /*startElement*/,
      ImplAAFPropertyValue ** /*pOutPropVals*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



 AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefStream::GetElements (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafInt64  /*startElement*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFTypeDefStream, AUID_AAFTypeDefStream);


