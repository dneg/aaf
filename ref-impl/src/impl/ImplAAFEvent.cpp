/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/







#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFEvent::ImplAAFEvent ()
{}


ImplAAFEvent::~ImplAAFEvent ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::GetPosition (
      aafPosition_t *  /*pPosition*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::SetPosition (
      aafPosition_t  /*Position*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::SetComment (
      wchar_t *  /*pComment*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::GetComment (
      wchar_t *  /*pComment*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEvent::GetCommentBufLen (
      aafUInt32 *  /*pBufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFEvent, AUID_AAFEvent);


