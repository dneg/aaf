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

 
/***********************************************\
*  Stub only.   Implementation not yet added    *
\***********************************************/

#ifndef __ImplAAFSourceMob_h__
#include "ImplAAFSourceMob.h"
#endif





#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFEssenceData::ImplAAFEssenceData ()
{}


ImplAAFEssenceData::~ImplAAFEssenceData ()
{}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::Write (aafUInt32  /*bytes*/,
                           aafUInt32  /*buflen*/,
                           aafDataBuffer_t  /*buffer*/,
                           aafUInt32 *  /*bytesRead*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::Read (aafUInt32  /*bytes*/,
                           aafUInt32  /*buflen*/,
                           aafDataBuffer_t  /*buffer*/,
                           aafUInt32 *  /*bytesRead*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::Seek (aafPosition_t  /*offset*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetSize (aafLength_t *  /*pSize */)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::SetFileMob (ImplAAFSourceMob * /*pFileMob*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetFileMob (ImplAAFSourceMob ** /*ppFileMob*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetFileMobID (aafUID_t *  /*pFileMobID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



extern "C" const aafClassID_t CLSID_AAFEssenceData;

OMDEFINE_STORABLE(ImplAAFEssenceData, CLSID_AAFEssenceData);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFEssenceData::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFEssenceData, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}


