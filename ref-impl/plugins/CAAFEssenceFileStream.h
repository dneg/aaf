//@doc
//@class    AAFEssenceFileStream | Implementation class for AAFEssenceFileStream
#ifndef __CAAFEssenceFileStream_h__
#define __CAAFEssenceFileStream_h__

//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif

#include <stdio.h>


class CAAFEssenceFileContainer;

//
// Include the standard plugin interfaces.
//
#include "AAFPlugin.h"


// Private state for this file.
typedef enum _FileStreamMode
{
  openUnknown = 0,
  openNew = 1,
  openRead = 2,
  openAppend = 3
} FileStreamMode;



class CAAFEssenceFileStream
  : public IAAFEssenceStream,
    public CAAFUnknown
{
protected:
  //********
  //
  // Constructor/destructor
  //
  CAAFEssenceFileStream (CAAFEssenceFileContainer *pContainer,
                         CAAFEssenceFileStream *pPrevFileStream);
  virtual ~CAAFEssenceFileStream ();


public:
  //
  // IAAFEssenceStream interface methods.
  //

  // Write some number of bytes to the stream exactly and with no formatting or compression.
  STDMETHOD(Write)
    (/*[in]*/ aafUInt32  bytes, // write this many bytes
     /*[out, size_is(bytes), length_is(*bytesWritten)]*/ aafDataBuffer_t  buffer, // here is the buffer
     /*[out,ref]*/ aafUInt32 *  bytesWritten); // return bytes actually written.

  // Read some number of bytes from the stream exactly and with no formatting or compression.
  STDMETHOD (Read)
    (/*[in]*/ aafUInt32  buflen, // to a buffer of this size
     /*[out, size_is(buflen), length_is(*bytesRead)]*/ aafDataBuffer_t  buffer, // here is the buffer
     /*[out,ref]*/ aafUInt32 *  bytesRead); // Return bytes actually read 

  // Seek to the absolute byte offset into the stream.
  STDMETHOD (Seek)
    (/*[in]*/ aafPosition_t  byteOffset); // The absolute byte offset into the stream. 

  // Seek forward or backward the given byte count.
  STDMETHOD (SeekRelative)
    (/*[in]*/ aafInt32  byteOffset); // The relative byte offset into the stream. 

  // Returns kAAFTrue if the byte offset is within the stream.
  STDMETHOD (IsPosValid)
    (/*[in]*/ aafPosition_t  byteOffset, // The absolute byte offset into the stream.
     /*[out]*/ aafBoolean_t *  isValid); // The result. 

  // Returns the position within the stream.
  STDMETHOD (GetPosition)
    (/*[out]*/ aafPosition_t *  position); // The position within the stream. 

  // Returns the length of the stream.
  STDMETHOD (GetLength)
    (/*[out]*/ aafLength_t *  position); // The length of the stream. 

  // Ensure that all bits are written.
  STDMETHOD (FlushCache)
     ();


  // Sets the size of the cache buffer used for further operations.
			// Destroys the current contents of the cache.
  STDMETHOD (SetCacheSize)
    (/*[in]*/ aafUInt32  itsSize); // The size of the cache buffer. 



public:
  //
  // Basic factory methods
  //


  // Attempt to create an essence file stream for reading and writing.
  // Returns one of the following:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  // AAFRESULT_FILE_EXISTS
  //   - the given path already points to a file-system file.
  STDMETHOD (Create)
    (/*[in,string]*/ const aafCharacter * pFilePath, // The local file-system path to a file
     /*[in]*/ aafMobID_constptr pMobID); // Optional mobID identifying the external media 

  // Attempt to open an essence file stream for reading.
  // Returns one of the following:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  // AAFRESULT_FILE_NOT_FOUND
  //   - the given path does not point to a valid file-system file.
  // AAFRESULT_NOT_READABLE
  //   - file is write-only, cannot be opened for reading.
  STDMETHOD (OpenRead)
    (/*[in,string]*/ const aafCharacter * pFilePath, // The local file-system path to a file
     /*[in]*/ aafMobID_constptr pMobID); // Optional mobID identifying the external media 

  // Attempt to open an essence file stream for appending.
  // Returns one of the following:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  // AAFRESULT_FILE_NOT_FOUND
  //   - the given path does not point to a valid file-system file.
  // AAFRESULT_NOT_WRITEABLE
  //   - file is read-only, cannot be opened for writing.
  STDMETHOD (OpenAppend)
    (/*[in,string]*/ const aafCharacter * pFilePath, // The local file-system path to a file
     /*[in]*/ aafMobID_constptr pMobID); // Optional mobID identifying the external media 



protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // Declare the IUnknown methods (implemented by CAAFUnknown).
  // 
  AAF_DECLARE_STANDARD_UNKNOWN()
  //
  //********

  //
  // Factory method called by container.
  //
  static CAAFEssenceFileStream *CreateFileStream(CAAFEssenceFileContainer *pContainer);

public :
  //
  // Return the path.
  const wchar_t * FilePath() const;

  CAAFEssenceFileStream* PrevFileStream() const;
  void SetPrevFileStream(CAAFEssenceFileStream *pPrevFileStream);
 
  // Remove this file stream from the container's list of open streams.
  void RemoveFileStreamFromContainer();

  // Init the stream for a particular essence data file.
  // Returns one of the following:
  //
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  STDMETHOD (Init)
    (/*[in,string]*/ const aafCharacter * pFilePath, // The local file-system path to a file
     /*[in]*/ aafMobID_constptr pMobID); // Optional mobID identifying the external media 

  // Cleanup any internally allocated buffers.
  void CleanupBuffers(void);

  // Close the currently opened file handle if it exists.
  void CloseFile();

  // Use the internal path buffers to test to see if the file already exists.
  bool FileAlreadyExists();

  // Utility to get the end of file position using ansi functions.
  bool GetEOFPos(fpos_t *pos);



  void SetStreamMode(FileStreamMode streamMode) { _streamMode = streamMode; };
  FileStreamMode StreamMode() const { return _streamMode; };


  // Private operation state for this file
  typedef enum _FileStreamOp
  {
    opUnknown = 0,
    opRead = 1,
    opWrite = 2,
    opSeek = 3,
    opSetpos = 4,
    opFlush = 5
  } FileStreamOp;
  //
  // Utility to set the current operation and return the last
  // stream operation. This is used to synchronzize read and write operations.
  FileStreamOp SetStreamOp(FileStreamOp streamOp); 

  
  //
  // The container to the container that created this file steam.
  CAAFEssenceFileContainer *_pContainer;

  //
  // Previous file stream.
  CAAFEssenceFileStream *_pPrevFileStream;

  //
  // Wide character path copied from the Init() parameter.
  wchar_t *_pwPath;
  //
  // Optional mobID identifying the external media
  aafMobID_t *_pMobID;
  //
  // Ascii character path
  char *_pPath;
  //
  // File handle.
  FILE *_pFile;
  //
  // Open mode of the file
  FileStreamMode _streamMode;
  //
  // Save the last stream operation so that we can
  FileStreamOp _lastStreamOp;
  //
  // Cached eof position for openRead and openAppend modes.
  aafPosition_t _startingEOF;
};

#endif // ! __CAAFEssenceFileStream_h__



