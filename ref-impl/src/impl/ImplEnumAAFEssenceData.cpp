/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif
#ifndef __ImplAAFContentStorage_h__
#include "ImplAAFContentStorage.h"
#endif






#ifndef __ImplEnumAAFEssenceData_h__
#include "ImplEnumAAFEssenceData.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFEssenceData;


ImplEnumAAFEssenceData::ImplEnumAAFEssenceData ()
{
  _current = 0;
  _contentStorage = NULL;
}


ImplEnumAAFEssenceData::~ImplEnumAAFEssenceData ()
{
  if (_contentStorage)
  {
    _contentStorage->ReleaseReference();
    _contentStorage = NULL;
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFEssenceData::NextOne (ImplAAFEssenceData ** ppEssenceData)
{
  AAFRESULT hr = AAFRESULT_SUCCESS;
  aafUInt32 cur = _current, siz = 0;

  hr = _contentStorage->GetNumEssenceData (&siz);
  if (AAFRESULT_SUCCESS == hr)
  {    
    if (cur < siz)
    {
      hr = _contentStorage->GetNthEssenceData (cur, ppEssenceData);
      if (AAFRESULT_SUCCESS == hr)
        _current = ++cur;
    }
		else
			return AAFRESULT_NO_MORE_OBJECTS;
  }
  return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFEssenceData::Next (aafUInt32  count,
                           ImplAAFEssenceData ** ppEssenceData,
                           aafUInt32 *  pFetched)
{
	aafUInt32		numEssenceData = 0;
	AAFRESULT		result = AAFRESULT_SUCCESS;

	if ((pFetched == NULL) || (NULL == ppEssenceData))
		return AAFRESULT_NULL_PARAM;

	for (numEssenceData = 0; numEssenceData < count; numEssenceData++)
	{
		result = NextOne(&ppEssenceData[numEssenceData]);
		if (AAFRESULT_SUCCESS != result)
			break;
	}
	
	*pFetched = numEssenceData;

	return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFEssenceData::Skip (aafUInt32  count)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  aafUInt32 newCurrent = _current + count;
  aafUInt32 siz = 0;

  result = _contentStorage->GetNumEssenceData(&siz);
  if (AAFRESULT_SUCCESS == result)
  {
    if (newCurrent < siz)
      _current = newCurrent;
    else
      result = AAFRESULT_NO_MORE_OBJECTS;
  }

  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFEssenceData::Reset ()
{
  _current = 0;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFEssenceData::Clone (ImplEnumAAFEssenceData ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFEssenceData *theEnum = (ImplEnumAAFEssenceData *)CreateImpl (CLSID_EnumAAFEssenceData);
	
	XPROTECT()
	{
		CHECK(theEnum->SetContentStorage(_contentStorage));
		CHECK(theEnum->Reset());
		CHECK(theEnum->Skip(_current));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}



//Internal
AAFRESULT
    ImplEnumAAFEssenceData::SetContentStorage(ImplAAFContentStorage *pContentStorage)
{
  if (_contentStorage)
    _contentStorage->ReleaseReference();
  _contentStorage = 0;

  _contentStorage = pContentStorage;

  if (pContentStorage)
    pContentStorage->AcquireReference();

  return AAFRESULT_SUCCESS;
}









