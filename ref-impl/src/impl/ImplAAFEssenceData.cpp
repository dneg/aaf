/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFSourceMob_h__
#include "ImplAAFSourceMob.h"
#endif


#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif
#ifndef __ImplAAFHeader_h__
#include "ImplAAFHeader.h"
#endif
#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFResult.h"

const int PID_ESSENCEDATA_FILEMOBID     = 0;
const int PID_ESSENCEDATA_MEDIADATA     = 1;

const aafUID_t kNullID = {0};

ImplAAFEssenceData::ImplAAFEssenceData () :
  _fileMobID(PID_ESSENCEDATA_FILEMOBID, "fileMobID"),
  _mediaData(PID_ESSENCEDATA_MEDIADATA, "mediaData")
{
  // Add the properties into the property set.
  _persistentProperties.put(_fileMobID.address());
  _persistentProperties.put(_mediaData.address());

  // Initial default property values.
  _fileMobID = kNullID;
}


ImplAAFEssenceData::~ImplAAFEssenceData ()
{
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::Write (aafUInt32  bytes,
                           aafDataBuffer_t buffer,
                           aafUInt32 *bytesWritten)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == buffer || NULL == bytesWritten)
    return AAFRESULT_NULL_PARAM;
  
  try
  {
    _mediaData.write(buffer, bytes, *bytesWritten);
    if (0 < bytes && 0 == *bytesWritten)
      result = AAFRESULT_NOT_IMPLEMENTED;
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }
  return result;
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::Read (aafUInt32  bytes,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesRead)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == buffer || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;
  
  try
  {
    _mediaData.read(buffer, bytes, *bytesRead);
    if (0 < bytes && 0 == *bytesRead)
      result = AAFRESULT_NOT_IMPLEMENTED;
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }
  return result;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::SetPosition (aafPosition_t  offset)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  try
  {
    OMUInt64 tmpOffset = offset;
    _mediaData.setPosition(tmpOffset);
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }

  return result;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetPosition (aafPosition_t  *pOffset)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  if (NULL == pOffset)
    return AAFRESULT_NULL_PARAM;

  try
  {
    OMUInt64 tmpOffset;
    tmpOffset = _mediaData.position();
    *pOffset = tmpOffset;
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }

  return result;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetSize (aafLength_t *pSize)
{
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;


  AAFRESULT result = AAFRESULT_SUCCESS;

  try
  {
    *pSize = _mediaData.size();
  }
  //catch (OMException& ome)
  //{
  //}
  catch(...)
  {
    result = AAFRESULT_INTERNAL_ERROR;
  }

  return result;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::SetFileMob (ImplAAFSourceMob * pFileMob)
{
	AAFRESULT result = AAFRESULT_SUCCESS;
	ImplAAFMob *pMob = NULL;
	ImplAAFHeader *pHeader = NULL;
	aafUID_t mobID;

	if(NULL == pFileMob)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		CHECK(pFileMob->GetMobID(&mobID));

		// Does a mob with the ID already exist?  If not, return error
		CHECK(MyHeadObject(&pHeader));
		CHECK(pHeader->LookupMob(&mobID, &pMob));

		// TODO: Make sure the mob is a valid File source mob???
		_fileMobID = mobID;
	} /* XPROTECT */
	XEXCEPT
	{	// save the error code.
		result = (XCODE());
	}
	XEND;

	// cleanup
	if (pHeader)
	{
		pHeader->ReleaseReference();
		pHeader = NULL;
	}
	return(result);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetFileMob (ImplAAFSourceMob ** ppFileMob)
{
	AAFRESULT result = AAFRESULT_SUCCESS;
	ImplAAFMob *pMob = NULL;
	ImplAAFSourceMob *pSourceMob = NULL;
	ImplAAFEssenceDescriptor *pEssenceDescriptor = NULL;
	ImplAAFHeader *pHeader = NULL;
	aafUID_t mobID;

	if(NULL == ppFileMob)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		CHECK(GetFileMobID(&mobID));

		// Does a mob with the ID already exist?  If not, return error
		CHECK(MyHeadObject(&pHeader));
		CHECK(pHeader->LookupMob(&mobID, &pMob));

		// This should be a valid file mob which is a file mob.
		pSourceMob = dynamic_cast<ImplAAFSourceMob *>(pMob);
		if (NULL == pSourceMob)
			RAISE(AAFRESULT_NOT_FILEMOB);

		// Does the source mob contain a file descriptor?
		CHECK(pSourceMob->GetEssenceDescriptor(&pEssenceDescriptor));
		if (dynamic_cast<ImplAAFFileDescriptor *>(pEssenceDescriptor))
		{
			(*ppFileMob) = pSourceMob;
			pSourceMob->AcquireReference();
		}

	} /* XPROTECT */
	XEXCEPT
	{	// save the error code.
		result = (XCODE());
	}
	XEND;

	// cleanup
	// Note: pMob and pSourceMob are temp and should not be released.
	if (pEssenceDescriptor)
	{
		pEssenceDescriptor->ReleaseReference();
		pEssenceDescriptor = NULL;
	}
	if (pHeader)
	{
		pHeader->ReleaseReference();
		pHeader = NULL;
	}
	return(result);

}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetFileMobID (aafUID_t *  pFileMobID)
{
  if (NULL == pFileMobID)
    return AAFRESULT_NULL_PARAM;

  _fileMobID = *pFileMobID;
  return AAFRESULT_SUCCESS;
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


