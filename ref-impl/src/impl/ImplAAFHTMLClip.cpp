/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/







#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFHTMLClip_h__
#include "ImplAAFHTMLClip.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFHTMLClip::ImplAAFHTMLClip ()
{}


ImplAAFHTMLClip::~ImplAAFHTMLClip ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::GetBeginAnchor (
      wchar_t *  /*pName*/,
      aafInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::GetBeginAnchorBufLen (
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::SetBeginAnchor (
      wchar_t *  /*pName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::GetEndAnchor (
      wchar_t *  /*pName*/,
      aafInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::GetEndAnchorBufLen (
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::SetEndAnchor (
      wchar_t *  /*pName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFHTMLClip, AUID_AAFHTMLClip);


