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

#include "CAAFEssenceFileStream.h"
#include "CAAFEssenceFileContainer.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "AAFResult.h"

#include <errno.h>






//
// NOTE: The following two routines will have to be rewritten
// if fpos_t is defined to be a structure or aafPosition_t is a 
// structure.
//
/*inline*/ bool AafPos2AnsiPos(fpos_t *ansiPos, const aafPosition_t *aafPos)
{
#if defined (__GLIBC__) && ((__GLIBC__ >= 2) && (__GLIBC_MINOR__ >= 2))
  // For glibc 2.2 and above fpos_t is a struct
  // NYI
  return false;
#else
  // For first version just assume that platform an perform conversion.
  if (sizeof(fpos_t) < sizeof(aafPosition_t))
  {
    // The following test assumes 64 bit arithematic!
    aafPosition_t trunPos = (AAFCONSTINT64(0x00000000FFFFFFFF) & *aafPos);
    if (trunPos != *aafPos && AAFCONSTINT64(0xFFFFFFFFFFFFFFFF) != *aafPos)
      return false;

    *ansiPos = *aafPos;
  }
  else
    *ansiPos = *aafPos;

  return true;
#endif
}


/*inline*/ bool AnsiPos2AafPos(aafPosition_t *aafPos, const fpos_t *ansiPos)
{
#if defined (__GLIBC__) && ((__GLIBC__ >= 2) && (__GLIBC_MINOR__ >= 2))
  // For glibc 2.2 and above fpos_t is a struct
  // NYI
  return false;
#else
  // For first version just assume that platform an perform conversion.
  *aafPos = *ansiPos;

  return true;
#endif
}



//
// Constructor 
//
CAAFEssenceFileStream::CAAFEssenceFileStream (CAAFEssenceFileContainer *pContainer,
                                              CAAFEssenceFileStream *pPrevFileStream)
  : CAAFUnknown(NULL),
  _pContainer(pContainer),
  _pPrevFileStream(pPrevFileStream),
  _pwPath(NULL),
  _pMobID(NULL),
  _pPath(NULL),
  _pFile(NULL),
  _streamMode(openUnknown),
  _lastStreamOp(opUnknown)
{
  assert(NULL != pContainer);

  // We are saving a reference to our container so we need to increment
  // the referencen count.
  _pContainer->AddRef();
}


CAAFEssenceFileStream::~CAAFEssenceFileStream ()
{
  // Cleanup memory 
  CleanupBuffers();

  // Make sure the file handle is closed.
  CloseFile();

  // Remove this file stream frome the container's private list of 
  // open file streams.
  RemoveFileStreamFromContainer();

  // We are finished with our container's reference so we need
  // to decrement the reference count.
  _pContainer->Release();
  _pContainer = NULL;
}

const wchar_t * CAAFEssenceFileStream::FilePath() const 
{ 
  return _pwPath; 
}

CAAFEssenceFileStream* CAAFEssenceFileStream::PrevFileStream() const
{
  return _pPrevFileStream;
}

void CAAFEssenceFileStream::SetPrevFileStream(CAAFEssenceFileStream *pPrevFileStream)
{
  _pPrevFileStream = pPrevFileStream;
}

void CAAFEssenceFileStream::RemoveFileStreamFromContainer()
{
  assert(NULL != _pContainer);
  CAAFEssenceFileStream *pLastFileStream = _pContainer->LastFileStream();
  assert(NULL != pLastFileStream);
  
  // If this stream is the last stream then just set the new last file stream
  // to the previous file stream.
  if (pLastFileStream == this)
  {
    _pContainer->SetLastFileStream(pLastFileStream->_pPrevFileStream);
  }
  else
  { 
    // Search for the this stream in the containers list.
    CAAFEssenceFileStream *pCurrentFileStream = pLastFileStream;
    CAAFEssenceFileStream *pPreviousFileStream = NULL;
    while (pCurrentFileStream && pCurrentFileStream != this)
    {
      pPreviousFileStream = pCurrentFileStream;
      pCurrentFileStream = pCurrentFileStream->_pPrevFileStream;
    }

    assert(pCurrentFileStream == this);
    // Remove this link in the container's file stream list...
    if (pCurrentFileStream == this)
      pPreviousFileStream->_pPrevFileStream = _pPrevFileStream;
  }
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::Init (const aafCharacter * pFilePath,
        aafMobID_constptr pMobID)
{
  if (NULL == pFilePath)
    return E_INVALIDARG;

  // Return an error if this stream has already been opened for
  // this instance.
  if (_pFile)
    return AAFRESULT_ALREADY_OPEN;  // AAFRESULT_STREAM_REOPEN ???

  // Cleanup any previously allocated buffers. 
  CleanupBuffers();

  // Computer the number of wide and multibyte characters.
  size_t i;
  size_t charCount = 0;
  for (i = 0; pFilePath[i]; ++i)
    ;
  charCount = i + 1; // include the terminating null.

  // Copy the wide character path name.
  _pwPath = new wchar_t[charCount];
  if (NULL == _pwPath)
    return AAFRESULT_NOMEMORY;
  for (i = 0; i < charCount; ++i)
    _pwPath[i] = pFilePath[i];
  
  // Convert the wide character path the an ascii character path.
  // Allocate the maximum possible multibyte string for the current
  // locale.
  size_t byteCount = (MB_CUR_MAX * (charCount - 1)) + 1;
  _pPath = new char[byteCount];
  if (NULL == _pPath)
    return AAFRESULT_NOMEMORY;
  size_t convertedBytes = wcstombs( _pPath, _pwPath, byteCount);
  if (-1 == convertedBytes)
    return E_INVALIDARG;

  //
  // Copy the optional mobID it it exists.
  if (pMobID)
  {
    _pMobID = new aafMobID_t;
    if (NULL == _pMobID)
      return AAFRESULT_NOMEMORY;
    memcpy(_pMobID, pMobID, sizeof(aafMobID_t));
  }


	return S_OK;
}


// Cleanup any internally allocated buffers.
void CAAFEssenceFileStream::CleanupBuffers(void)
{
  if (_pwPath)
  {
    delete [] _pwPath;
    _pwPath = NULL;
  }

  if (_pMobID)
  {
    delete _pMobID;
    _pMobID = NULL;
  }

  if (_pPath)
  {
    delete [] _pPath;
    _pPath = NULL;
  }
}


// Close the currently opened file handle if it exists.
void CAAFEssenceFileStream::CloseFile()
{
  // Make sure the file handle is closed.
  if (_pFile)
  {
    fclose(_pFile);
    _pFile = NULL;
  }
}



// Use the internal path buffers to test to see if the file already exists.
bool CAAFEssenceFileStream::FileAlreadyExists()
{
  errno = 0;

  FILE *pFile = fopen(_pPath, "rb");
  if (NULL != pFile)
  {
    fclose(pFile);
    return true;
  }

  return false;
}


// Utility to get the end of file position using ansi functions.
bool CAAFEssenceFileStream::GetEOFPos(fpos_t *pos)
{
  if (!_pFile || !pos)
    return false;

  // Save the current position.
  fpos_t currentPos;
  errno = 0;
  if (0 != fgetpos(_pFile, &currentPos))
    return false;

  errno = 0;
  if (0 != fseek(_pFile, 0, SEEK_END))
    return false;

  errno = 0;
  if (0 != fgetpos(_pFile, pos))
    return false;

  //
  // Save operation so that we can synchronize reading and writing.
  SetStreamOp(opSetpos);

  errno = 0;
  if (0 != fsetpos(_pFile, &currentPos))
    return false;

  return true;
}

//
// Utility to set the current operation and return the last
// stream operation. This is used to synchronzize read and write operations.
//
CAAFEssenceFileStream::FileStreamOp CAAFEssenceFileStream::SetStreamOp(FileStreamOp streamOp) 
{ 
  FileStreamOp tmpOp = _lastStreamOp; 
  _lastStreamOp = streamOp;
  return tmpOp;
}




HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::Create (const aafCharacter *  pFilePath,
        aafMobID_constptr  pMobID)
{
  HRESULT hr = Init(pFilePath, pMobID);
  if (AAFRESULT_SUCCESS != hr)
    return hr;

  // Make sure that we do NOT attempt to overwrite an existing file
  // by first attempting to open the file for reading. If this succeeds then
  // the file exists and we close the file and return an error.
  if (FileAlreadyExists())
    return AAFRESULT_FILE_EXISTS;

  // Attempt to create a new file for reading and writing.
  errno = 0;
  _pFile = fopen(_pPath, "w+b");
  if (NULL == _pFile)
  {
    return AAFRESULT_BADOPEN;
  }

  SetStreamMode(openNew);
  aafPosition_t zero = 0;
  AafPos2AnsiPos(&_startingEOF, &zero);

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::OpenRead (const aafCharacter * pFilePath,
        aafMobID_constptr pMobID)
{
  HRESULT hr = Init(pFilePath, pMobID);
  if (AAFRESULT_SUCCESS != hr)
    return hr;

  // Attempt to open the file for reading.
  errno = 0;
  _pFile = fopen(_pPath, "rb");
  if (NULL == _pFile)
  { //If the file does not exist or cannot be found return an error.
    return AAFRESULT_FILE_NOT_FOUND;
  }

  SetStreamMode(openRead);
  if (!GetEOFPos(&_startingEOF))
    return AAFRESULT_INTERNAL_ERROR;

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::OpenAppend (const aafCharacter * pFilePath,
        aafMobID_constptr  pMobID)
{
  HRESULT hr = Init(pFilePath, pMobID);
  if (AAFRESULT_SUCCESS != hr)
    return hr;

  // Attempt to open the file for appending and reading.

  //1. _pFile = fopen(_pPath, "a+b"); cannot write after reading
  //2. _pFile = fopen(_pPath, "ab"); cannot read
  //3. _pFile = fopen(_pPath, "ra+b"); read-only
  //4. _pFile = fopen(_pPath, "a+rb"); same as 1.

  // Make sure that we do NOT attempt to overwrite an existing file
  // by first attempting to open the file for reading. If this succeeds then
  // the file exists and we close the file and return an error.
  if (FileAlreadyExists())
  {
    // Open the file read/write and explicitly enforce the "append" mode.
    errno = 0;
    _pFile = fopen(_pPath, "a+b"); // now works without pre-compiled headers!
  }
  else
  {
    // Open the file read/write and explicitly enforce the "append" mode.
    errno = 0;
    _pFile = fopen(_pPath, "w+b");
  }

  if (NULL == _pFile)
  { //If the file does not exist or cannot be found return an error.
    return AAFRESULT_FILE_NOT_FOUND;
  }
  
 

  SetStreamMode(openAppend);
  if (!GetEOFPos(&_startingEOF))
    return AAFRESULT_INTERNAL_ERROR;

  return AAFRESULT_SUCCESS;
}







HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::Write (
      aafUInt32 bytes,
      aafDataBuffer_t  buffer,
      aafUInt32 * bytesWritten)
{
  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == buffer || NULL == bytesWritten)
    return E_INVALIDARG;
  if (0 > bytes)
    return E_INVALIDARG;
  if (openRead == _streamMode)
    return AAFRESULT_NOT_WRITEABLE;

  //
  // Synchronize with reading if necessary.
  FileStreamOp lastStreamOp = SetStreamOp(opWrite);
  if (opRead == lastStreamOp)
  {
    errno = 0;
    int result = 0;
    if (openAppend == _streamMode)
      result = fseek(_pFile, 0, SEEK_END);
    else
      result = fseek(_pFile, 0, SEEK_CUR);
    if (0 != result)
      return AAFRESULT_INTERNAL_ERROR;
  }
  

  // Write the given data to the file at the current file
  // position.
  errno = 0;
  *bytesWritten = fwrite(buffer, 1, bytes, _pFile);
  if (*bytesWritten != (size_t)bytes)
  { // What error code should we return?
    long err = errno;

    return AAFRESULT_CONTAINERWRITE;
  }

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == buffer)
    return E_INVALIDARG;
  if (NULL == bytesRead)
    return E_INVALIDARG;

  if (feof(_pFile))
    return AAFRESULT_EOF;


  //
  // Synchronize with writing if necessary.
  FileStreamOp lastStreamOp = SetStreamOp(opRead);
  if (opWrite == lastStreamOp)
  {
    errno = 0;
    int result = fflush(_pFile); 
    if (0 != result)
      return AAFRESULT_INTERNAL_ERROR;
  }



  // Write the given data to the file at the current file
  // position.
  errno = 0;
  *bytesRead = fread(buffer, 1, buflen, _pFile);
  if (ferror(_pFile))
  { // What error code should we return?
    long err = errno;

    return AAFRESULT_END_OF_DATA;
  }
  else if (feof(_pFile))
    return AAFRESULT_EOF; 

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::Seek (aafPosition_t  byteOffset)
{
  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;

  fpos_t pos;
  if (!AafPos2AnsiPos(&pos, &byteOffset))
    return E_INVALIDARG;

  //
  // Save operation so that we can synchronize reading and writing.
  SetStreamOp(opSetpos);

  errno = 0;
  if (0 != fsetpos(_pFile, &pos))
  { // What error code should we return?
    if (feof(_pFile))
      return AAFRESULT_BADSAMPLEOFFSET;
    else
      return AAFRESULT_INTERNAL_ERROR;
  }

  return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::SeekRelative (aafInt32  byteOffset)
{
  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;


  //
  // Save operation so that we can synchronize reading and writing.
  SetStreamOp(opSeek);
  
  errno = 0;
  if (0 != fseek(_pFile, byteOffset, SEEK_CUR))
  { // What error code should we return?
    if (feof(_pFile))
      return AAFRESULT_EOF;
    else
      return AAFRESULT_INTERNAL_ERROR;
  }
  
  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::IsPosValid (aafPosition_t  byteOffset,
        aafBool *  isValid)
{
  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == isValid)
    return E_INVALIDARG;
  
  *isValid = kAAFFalse;

  if (0 < byteOffset)
  {
    aafLength_t length = 0;
    HRESULT hr = GetLength(&length);
    if (AAFRESULT_SUCCESS != hr)
      return hr;

    if (byteOffset < length)
    {
      *isValid = kAAFTrue;
    }
    else if (byteOffset == length)
    {
      if (_streamMode != openRead)
      { // If we are not in read-only mode then
        // we don't know whether or not the next
        // file operation will be a read or a write
        // so we just return true.
        *isValid = kAAFTrue;
      }
    }
  }

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::GetPosition (aafPosition_t *  position)
{
  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == position)
    return E_INVALIDARG;

  
  if (ferror(_pFile))
  { // The file's error state is still set. The position is undefined
    // What error code should we return?
    return AAFRESULT_INTERNAL_ERROR;
  }
  else if (feof(_pFile))
  { // We are already at the end of file so return the size of the file.
    return GetLength(position);
  }

  // Attempt to get the current position.
  fpos_t pos;
  errno = 0;
  if (0 != fgetpos(_pFile, &pos))
  {
    // What error code should we return?
    return AAFRESULT_INTERNAL_ERROR;
  }
  
  // Convert ansi position to aaf position.
  AnsiPos2AafPos(position, &pos);

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::GetLength (aafLength_t *  position)
{
  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  if (NULL == position)
    return E_INVALIDARG;

  if (openRead == _streamMode)
  { // Use our cached value.
    // Convert ansi position to aaf position.
    AnsiPos2AafPos(position, &_startingEOF);
  }
  else
  {
    fpos_t pos;
    if (!GetEOFPos(&pos))
      return AAFRESULT_INTERNAL_ERROR;
    // Convert ansi position to aaf position.
    AnsiPos2AafPos(position, &pos);
  }
  
  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::FlushCache ()
{
  if (NULL != _pFile)
  {
    errno = 0;

    //
    // Save operation so that we can synchronize reading and writing.
    SetStreamOp(opFlush);
  
    fflush(_pFile);
  }

  return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileStream::SetCacheSize (aafUInt32  itsSize)
{
  // PRE-CONDITION
  // Ansi states that setvbuf should be called before the first read or write
  // to the stream.

  if (NULL == _pFile) 
    return AAFRESULT_NOT_OPEN;
  // Validate the ansi limits for io buffer size..
  if (2 < itsSize && 32768 > itsSize)
    return E_INVALIDARG;
  
  // Round down the buffer size.
  size_t bufferSize = (size_t)((itsSize/ 2) * 2);
  
  // Attempt to set the buffer size for this file stream.
  errno = 0;
  if(0 != setvbuf(_pFile, NULL, _IOFBF, bufferSize))
  { // Default buffer has not been changed.
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  return AAFRESULT_SUCCESS;
}




//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFEssenceFileStream::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IAAFEssenceStream interface
    if (EQUAL_UID(riid,IID_IAAFEssenceStream)) 
    { 
        *ppvObj = (IAAFEssenceStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (EQUAL_UID(riid,IID_IAAFPlugin)) 
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Factory method called by the container to create an uninitialized instance
// of a file stream in memory.
//
CAAFEssenceFileStream *CAAFEssenceFileStream::CreateFileStream(CAAFEssenceFileContainer *pContainer)
{
  assert(NULL != pContainer);
  CAAFEssenceFileStream *pEssenceFileStream = NULL;

  pEssenceFileStream = new CAAFEssenceFileStream(pContainer, pContainer->LastFileStream());
 	if (NULL != pEssenceFileStream)
  {
    pContainer->SetLastFileStream(pEssenceFileStream);
    pEssenceFileStream->AddRef();
  }

  return pEssenceFileStream;
}
