//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=











#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFRIFFChunk_h__
#include "ImplAAFRIFFChunk.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFRIFFChunk::ImplAAFRIFFChunk () :

  _chunkID(PID_RIFFChunk_ChunkID,	L"ChunkID"),
  _chunkLength(PID_RIFFChunk_ChunkLength,	L"ChunkLength"),
  _chunkData(PID_RIFFChunk_ChunkData,	L"ChunkData")
{
  // Add the properties into the property set.
  _persistentProperties.put(_chunkID.address());
  _persistentProperties.put(_chunkLength.address());
  _persistentProperties.put(_chunkData.address());

  // Initial default property values.
  _chunkID = 0;
  _chunkLength = 0;

}



ImplAAFRIFFChunk::~ImplAAFRIFFChunk ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::Initialize (
      aafUInt32  chunkID,
      aafUInt32  chunkLength,
      aafUInt32  bytes,
      aafDataBuffer_t  buffer)
{
	if( isInitialized() )
    {
        return AAFRESULT_ALREADY_INITIALIZED;
    }
	_chunkID = chunkID;
	_chunkLength = chunkLength;
	aafUInt32 bytesWritten;
	
	AAFRESULT ar = WriteChunkData (bytes, buffer, &bytesWritten);
	if( ar == AAFRESULT_SUCCESS )
    {
        setInitialized();
    }
  	return ar;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::SetChunkID (
      aafUInt32  chunkID)
{
	_chunkID = chunkID;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::GetChunkID (
      aafUInt32 *  pChunkID)
{
	if( pChunkID == NULL )
    {
        return AAFRESULT_NULL_PARAM;
    }

    *pChunkID = _chunkID;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::SetChunkLength (
      aafUInt32  chunkLength)
{
	_chunkLength = chunkLength;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::GetChunkLength (
      aafUInt32 *  pChunkLength)
{
	if( pChunkLength == NULL )
    {
        return AAFRESULT_NULL_PARAM;
    }
   

    *pChunkLength = _chunkLength;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::WriteChunkData (
      aafUInt32  bytes,
      aafDataBuffer_t  buffer,
      aafUInt32 *  pBytesWritten)
{
	if (NULL == buffer || NULL == pBytesWritten)
    	return AAFRESULT_NULL_PARAM;
  	// Cannot access the data property if it is NOT associated with a file.
  	if (!persistent())
    	return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  	_chunkData.write(buffer, bytes, *pBytesWritten);
  	
  	if (0 < bytes && 0 == *pBytesWritten)
    	return AAFRESULT_CONTAINERWRITE;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFRIFFChunk::ReadChunkData (
      aafUInt32  bytes,
      aafDataBuffer_t  buffer,
      aafUInt32 *  pBytesRead)
{
	
	if (NULL == buffer || NULL == pBytesRead)
    return AAFRESULT_NULL_PARAM;
  // Cannot access the data property if it is NOT associated with a file.
  if (!persistent())
    return AAFRESULT_OBJECT_NOT_PERSISTENT;
  
  _chunkData.read(buffer, bytes, *pBytesRead);
  if (0 < bytes && 0 == *pBytesRead)
    return AAFRESULT_END_OF_DATA;

  return AAFRESULT_SUCCESS;
	
}


AAFRESULT STDMETHODCALLTYPE ImplAAFRIFFChunk::SetChunkDataPosition(
    aafPosition_t position )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omPosition = position;
    _chunkData.setPosition( omPosition );


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFRIFFChunk::GetChunkDataPosition(
    aafPosition_t*  pPosition )
{
    if( pPosition == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omPosition = _chunkData.position();
    *pPosition = omPosition;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFRIFFChunk::GetChunkDataSize(
    aafLength_t* pSize )
{
    if( pSize == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omSize = _chunkData.size();
    *pSize = omSize;


    return AAFRESULT_SUCCESS;
}





