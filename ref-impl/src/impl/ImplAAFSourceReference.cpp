/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#include <assert.h>
#include <AAFResult.h>


ImplAAFSourceReference::ImplAAFSourceReference ():
	_sourceID(			PID_SOURCEREFERENCE_SOURCEID,		"sourceId"),
	_sourceMobSlotId(	PID_SOURCEREFERENCE_SRCMOBSLOTID,	"sourceMobSlotId")
{
	_persistentProperties.put(		_sourceID.address());
	_persistentProperties.put(		_sourceMobSlotId.address());
}


ImplAAFSourceReference::~ImplAAFSourceReference ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::GetSourceID (aafUID_t *  /*pSourceID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::SetSourceID (aafUID_t   /*sourceID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::GetSourceMobSlotID (aafTrackID_t *  /*pMobSlotID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::SetSourceMobSlotID (aafTrackID_t   /*mobSlotID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



extern "C" const aafClassID_t CLSID_AAFSourceReference;

OMDEFINE_STORABLE(AAFSourceReference, CLSID_AAFSourceReference);

