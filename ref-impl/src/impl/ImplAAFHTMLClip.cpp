/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFHTMLClip_h__
#include "ImplAAFHTMLClip.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFHTMLClip::ImplAAFHTMLClip () :
  _beginAnchor(PID_HTMLClip_BeginAnchor, "BeginAnchor"),
  _endAnchor(PID_HTMLClip_EndAnchor, "EndAnchor")
{
  _persistentProperties.put(_beginAnchor.address());
  _persistentProperties.put(_endAnchor.address());
}


ImplAAFHTMLClip::~ImplAAFHTMLClip ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::GetBeginAnchor (
      wchar_t *  pName,
      aafInt32  bufSize)
{
  if (NULL == pName)
    return(AAFRESULT_NULL_PARAM);

  bool stat = _beginAnchor.copyToBuffer(pName, bufSize);
  if (! stat)
    return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	
  return (AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::GetBeginAnchorBufLen (
      aafUInt32 *  pLen)
{
  if (NULL == pLen)
    return (AAFRESULT_NULL_PARAM);

  *pLen = _beginAnchor.size();

  return (AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::SetBeginAnchor (
      wchar_t *  pName)
{
  if (NULL == pName)
    return(AAFRESULT_NULL_PARAM);

  _beginAnchor = pName;

  return (AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::GetEndAnchor (
      wchar_t *  pName,
      aafInt32  bufSize)
{
  if (NULL == pName)
    return(AAFRESULT_NULL_PARAM);

  bool stat = _endAnchor.copyToBuffer(pName, bufSize);
  if (! stat)
    return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	
  return (AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::GetEndAnchorBufLen (
      aafUInt32 *  pLen)
{
  if (NULL == pLen)
    return (AAFRESULT_NULL_PARAM);

  *pLen = _endAnchor.size();

  return (AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHTMLClip::SetEndAnchor (
      wchar_t *  pName)
{
  if (NULL == pName)
    return(AAFRESULT_NULL_PARAM);

  _endAnchor = pName;

  return (AAFRESULT_SUCCESS); 
}




OMDEFINE_STORABLE(ImplAAFHTMLClip, AUID_AAFHTMLClip);


