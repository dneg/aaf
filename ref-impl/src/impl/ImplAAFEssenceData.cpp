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

#ifndef __ImplAAFSourceMob_h__
#include "ImplAAFSourceMob.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

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


const aafMobID_t kNullMobID = {0};

ImplAAFEssenceData::ImplAAFEssenceData () :
  _fileMobID(PID_EssenceData_MobID,	"MobID"),
  _mediaData(PID_EssenceData_Data,	"Data")
{
  // Add the properties into the property set.
  _persistentProperties.put(_fileMobID.address());
  _persistentProperties.put(_mediaData.address());

  // Initial default property values.
  _fileMobID = kNullMobID;
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
  if (NULL == buffer || NULL == bytesWritten)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  _mediaData.write(buffer, bytes, *bytesWritten);
  if (0 < bytes && 0 == *bytesWritten)
    return AAFRESULT_CONTAINERWRITE;

  return AAFRESULT_SUCCESS;
}


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceData::Read (aafUInt32  bytes,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesRead)
{
  if (NULL == buffer || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  _mediaData.read(buffer, bytes, *bytesRead);
  if (0 < bytes && 0 == *bytesRead)
    return AAFRESULT_END_OF_DATA;

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::SetPosition (aafPosition_t  offset)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  OMUInt64 tmpOffset = offset;
  _mediaData.setPosition(tmpOffset);

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetPosition (aafPosition_t  *pOffset)
{
  if (NULL == pOffset)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  OMUInt64 tmpOffset;
  tmpOffset = _mediaData.position();
  *pOffset = tmpOffset;

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::GetSize (aafLength_t *pSize)
{
  if (NULL == pSize)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;

  *pSize = _mediaData.size();

  return AAFRESULT_SUCCESS;
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceData::SetFileMob (ImplAAFSourceMob * pFileMob)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFMob *pMob = NULL;
  ImplAAFSourceMob *pSourceMob = NULL;
  ImplAAFEssenceDescriptor *pEssenceDescriptor = NULL;
  ImplAAFFileDescriptor *pFileDescriptor = NULL;
  ImplAAFHeader *pHeader = NULL;
  aafMobID_t mobID;

  if(NULL == pFileMob)
    return(AAFRESULT_NULL_PARAM);

  XPROTECT()
  {
    CHECK(pFileMob->GetMobID(&mobID));

 // move to ImplAAFContentStorage::AppendEssenceData().
    // Does a mob with the ID already exist?  If not, return error
    CHECK(pFileMob->MyHeadObject(&pHeader));
    CHECK(pHeader->LookupMob(mobID, &pMob));


    // Make sure the mob is a valid File source mob???
    pSourceMob = dynamic_cast<ImplAAFSourceMob *>(pMob);
    if (NULL == pSourceMob)
      RAISE(AAFRESULT_INVALID_FILE_MOB);
    // Must       
    result = pSourceMob->GetEssenceDescriptor(&pEssenceDescriptor);
    if (AAFRESULT_SUCCESS != result)
      RAISE(AAFRESULT_INVALID_FILE_MOB);

    pFileDescriptor = dynamic_cast<ImplAAFFileDescriptor *>(pEssenceDescriptor);
    if (NULL == pFileDescriptor)
      RAISE(AAFRESULT_INVALID_FILE_MOB);

    // The mob id refers to a valid file source mob so we can save
    // the id.
    _fileMobID = mobID;
  } /* XPROTECT */
  XEXCEPT
  {  // save the error code.
    result = (XCODE());
	NO_PROPAGATE();
  }
  XEND;

  // cleanup
  if (pEssenceDescriptor)
  {
    pEssenceDescriptor->ReleaseReference();
    pEssenceDescriptor = NULL;
  }
  if (pMob)
  {
    pMob->ReleaseReference();
    pMob = NULL;
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
    ImplAAFEssenceData::GetFileMob (ImplAAFSourceMob ** ppFileMob)
{
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFMob *pMob = NULL;
  ImplAAFSourceMob *pSourceMob = NULL;
  ImplAAFEssenceDescriptor *pEssenceDescriptor = NULL;
  ImplAAFHeader *pHeader = NULL;
  aafMobID_t mobID;

  if(NULL == ppFileMob)
    return(AAFRESULT_NULL_PARAM);

  XPROTECT()
  {
    CHECK(GetFileMobID(&mobID));

    // Does a mob with the ID already exist?  If not, return error.
    // NOTE: Will return AAFRESULT_OBJECT_NOT_PERSISTENT if this object has
    // not been appended to to the file.
    CHECK(MyHeadObject(&pHeader));
    CHECK(pHeader->LookupMob(mobID, &pMob));

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
  {  // save the error code.
    result = (XCODE());
	NO_PROPAGATE();
  }
  XEND;

  // cleanup
  // Note: pMob and pSourceMob are temp and should not be released.
  if (pEssenceDescriptor)
  {
    pEssenceDescriptor->ReleaseReference();
    pEssenceDescriptor = NULL;
  }
  if (pMob)
  {
    pMob->ReleaseReference();
    pMob = NULL;
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
    ImplAAFEssenceData::GetFileMobID (aafMobID_t *  pFileMobID)
{
  if (NULL == pFileMobID)
    return AAFRESULT_NULL_PARAM;

  *pFileMobID = _fileMobID;
  return AAFRESULT_SUCCESS;
}



const OMMaterialIdentification&
  ImplAAFEssenceData::identification(void) const
{
  return *reinterpret_cast<const OMMaterialIdentification*>(&_fileMobID.reference());
}




