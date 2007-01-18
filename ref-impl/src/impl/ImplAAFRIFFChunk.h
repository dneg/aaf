//@doc
//@class    AAFRIFFChunk | Implementation class for AAFRIFFChunk
#ifndef __ImplAAFRIFFChunk_h__
#define __ImplAAFRIFFChunk_h__

#include "OMDataStreamProperty.h"

//=---------------------------------------------------------------------=
//
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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=












#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class OMDataStreamPropertyFilter;


class ImplAAFRIFFChunk : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRIFFChunk ();

protected:
  virtual ~ImplAAFRIFFChunk ();
  
  	OMFixedSizeProperty<aafUInt32>          _chunkID;
    OMDataStreamProperty                    _chunkData;
    OMDataStreamPropertyFilter*             _chunkDataFilter;

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] ChunkID value
         aafUInt32  chunkID);


  //****************
  // SetChunkID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetChunkID
        // @parm [in] ChunkID to set
        (aafUInt32  chunkID);

  //****************
  // GetChunkID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetChunkID
        // @parm [out] Retrieved ChunkID
        (aafUInt32 *  pChunkID);


  //****************
  // GetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLength
        // @parm [out] Retrieved Chunk Length
        (aafLength_t *  pLength);


  //****************
  // Write()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Write
        (// @parm [in] Write this many bytes
         aafUInt32  bytes,

         // @parm [out, size_is(bytes)] Data to write
         aafDataBuffer_t  buffer,

         // @parm [out,ref] Number of bytes actually written
         aafUInt32 *  bytesWritten);


  //****************
  // Read()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Read
        (// @parm [in] Read this many bytes
         aafUInt32  bytes,

         // @parm [out, size_is(bytes)] Buffer to read the data to
         aafDataBuffer_t  buffer,

         // @parm [out,ref] Number of bytes actually read
         aafUInt32 *  bytesRead);
         

 //****************
  // SetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
		  SetPosition
        // @parm [in] Offset from the beginning of chunk data
        (aafPosition_t offset);

  //****************
  // GetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPosition
        // @parm [out] Offset from the beginning of chunk data
        (aafPosition_t *  pOffset);

};

#endif // ! __ImplAAFRIFFChunk_h__


