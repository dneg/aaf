/***********************************************************************
*
*              Copyright (c) 1998-2001 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

#include "AAF.h"
#include "AAFResult.h"
#include "AAFFileMode.h"
#include "AAFDefUIDs.h"
#include "AAFFileKinds.h"

#include "ModuleTest.h"
#include "CAAFBuiltinDefs.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <wchar.h>

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFRawStorage>  IAAFRawStorageSP;
typedef IAAFSmartPointer<IAAFRandomFile>  IAAFRandomFileSP;
typedef IAAFSmartPointer<IAAFSetFileBits> IAAFSetFileBitsSP;
typedef IAAFSmartPointer<IAAFGetFileBits> IAAFGetFileBitsSP;


// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
	{ // delete the file.
	  remove(cFileName);
	}
}

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
{
  if (!expression)
    throw r;
}

// Function to compare COM interface pointers, taken from 
// CAAFTypeDefFixedArrayTest.cpp.
template <class T1, class T2>
aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
{
  IAAFSmartPointer<IUnknown>    spUnk1, spUnk2;
	
  checkResult(cls1->QueryInterface(IID_IUnknown, (void **)&spUnk1));
  checkResult(cls2->QueryInterface(IID_IUnknown, (void **)&spUnk2));
	
  if (spUnk1 == spUnk2)
	return kAAFTrue;
  else
	return kAAFFalse;
}

static HRESULT checkModeFlag (aafUInt32 modeFlags,
							  HRESULT expectedResult)
{
  // Check mod flags
  aafProductIdentification_t	ProductInfo = { 0 };
  HRESULT temphr;
  IAAFFile * pFile = 0;
  temphr = AAFFileOpenNewModify(L"foo",
								modeFlags,
								&ProductInfo,
								&pFile);
  if (expectedResult != temphr)
	{
	  return AAFRESULT_TEST_FAILED;
	}
  if (pFile)
	{
	  return AAFRESULT_TEST_FAILED;
	}

  return AAFRESULT_SUCCESS;
}


static HRESULT checkModeFlags ()
{
  HRESULT temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_EAGER_LOADING,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_REVERTABLE,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_UNBUFFERED,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_RECLAIMABLE,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_USE_LARGE_SS_SECTORS,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_CLOSE_FAIL_DIRTY,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_DEBUG0_ON,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  temphr = checkModeFlag (AAF_FILE_MODE_DEBUG1_ON,
						  AAFRESULT_NOT_IN_CURRENT_VERSION);
  if (AAFRESULT_FAILED (temphr)) return temphr;

  aafUInt32 i;
  for (i = 4; i < 28; i++)
	{
	  temphr = checkModeFlag ((1 << i),
							  AAFRESULT_BAD_FLAGS);
	  if (AAFRESULT_FAILED (temphr)) return temphr;
	}
  return AAFRESULT_SUCCESS;
}


#define MOB_NAME_TEST L"MOBTest"
#define MOB_NAME_SIZE 16

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
 0x13, 0x00, 0x00, 0x00,
 {0x3920c87a, 0xa466, 0x11d4, {0x93, 0x6f, 0x00, 0x06, 0x29, 0xee, 0xa6, 0x5f}}};



static aafProductVersion_t sVers =
{
  1,                 // major
  0,                 // minor
  0,                 // tertiary
  0,                 // patchLevel
  kAAFVersionUnknown // type
};

static aafProductIdentification_t
sInitProductInfo (aafProductVersion_t & v)
{
  aafProductIdentification_t r;
  r.companyName = L"AAF Developers Desk";
  r.productName = L"AAFFile Test";
  r.productVersionString = NULL;
  r.productID = UnitTestProductID;
  r.platform = NULL;
  r.productVersion = &v;

  return r;
};

static aafProductIdentification_t sIdent =
  sInitProductInfo (sVers);


//
// Functions to open files using standard disk API
//
static HRESULT localOpenFileDiskWrite
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  return AAFFileOpenNewModify(pFileName, 0, &sIdent, ppFile);
}


static HRESULT localOpenFileDiskRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  return AAFFileOpenExistingRead(pFileName, 0, ppFile);
}


//
// Functions to open files using RawStorage API, using raw storages on
// disk
//
static HRESULT localOpenFileDiskStgWrite
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a disk raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageDisk (pFileName,
							  kAAFFileExistence_new,
							  kAAFFileAccess_modify,
							  &pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_write,
								   &aafFileKindAafSSBinary,
								   0,
								   &sIdent,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  return (*ppFile)->Open();
}


static HRESULT localOpenFileDiskStgRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a disk raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageDisk (pFileName,
							  kAAFFileExistence_existing,
							  kAAFFileAccess_read,
							  &pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  return (*ppFile)->Open();
}



//
// Functions to open files using RawStorage API, using raw storages in
// memory
//
static HRESULT localOpenFileMemStgWrite
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a mem raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageMemory (kAAFFileAccess_modify,
								&pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_write,
								   &aafFileKindAafSSBinary,
								   0,
								   &sIdent,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  return (*ppFile)->Open();
}


static HRESULT localOpenFileMemStgRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a mem raw storage.
  IAAFRawStorageSP pStg;
  checkResult
	(AAFCreateRawStorageMemory (kAAFFileAccess_read,
								&pStg));

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);

  IAAFRandomFileSP pRandFile;
  HRESULT hr;
  hr = (*ppFile)->QueryInterface(IID_IAAFRandomFile,
								 (void **)&pRandFile);
  if (FAILED (hr)) return hr;
  
  IAAFSetFileBitsSP psfb;
  hr = pRandFile->SetFileBits (&psfb);
  if (FAILED (hr)) return hr;

  char cFileName[FILENAME_MAX];
  size_t status = wcstombs(cFileName, pFileName, FILENAME_MAX);
  assert (status != (size_t)-1);

  FILE * f = fopen (cFileName, "rb");
  assert (f);

  aafUInt64 position = 0;

  while (1)
	{
	  aafUInt8 c;
	  size_t numRead;
	  numRead = fread (&c, 1, 1, f);
	  if (1 != numRead)
		break;
	  hr = psfb->SetSize (position+1);
	  if (FAILED (hr)) return hr;
	  hr = psfb->WriteAt (&c, 1, position++);
	  if (FAILED (hr)) return hr;
	}
  fclose (f);

  return (*ppFile)->Open();
}



//
// We'll implement our own raw storage, to support the IAAFRawStorage
// and IAAFRandomRawStorage interfaces.
//
class CCustomRawStorage :
  public IAAFRawStorage,
  public IAAFRandomRawStorage
{
public:
  CCustomRawStorage (const aafWChar * pFileName,
					 aafFileAccess_e access,
					 aafFileExistence_e exist)
	: _refCnt (1),
	  _access (access),
	  _file (0),
	  _extent (0)
  { assert (pFileName);
    char cFileName[FILENAME_MAX];
	size_t status = wcstombs(cFileName, pFileName, FILENAME_MAX);
	assert (status != (size_t)-1);
	if (kAAFFileAccess_write == access)
	  _file = fopen (cFileName, "wb");
	else if (kAAFFileAccess_read == access)
	  {
		assert (kAAFFileExistence_existing == exist);
		_file = fopen (cFileName, "rb");
	  }
	else if (kAAFFileAccess_modify == access)
	  {
		switch (exist)
		  {
		  case kAAFFileExistence_existing:
			_file = fopen (cFileName, "r+b");
			break;
		  case kAAFFileExistence_new:
			_file = fopen (cFileName, "w+b");
			break;
		  default:
			assert (0);
		  }
	  }
	else
	  assert (0);
	assert (_file); }

  virtual ~CCustomRawStorage ()
  { if (_file) { fclose (_file); _file = 0; } }

  // IUnknown overrides
  HRESULT STDMETHODCALLTYPE
    QueryInterface (REFIID iid, void ** ppIfc)
    { if (! ppIfc) return AAFRESULT_NULL_PARAM;
	  if (equalUID (iid, IID_IUnknown))
		{ IUnknown * runk = (IAAFRawStorage*) this;
		*ppIfc = (void*) runk;
		AddRef ();
		return AAFRESULT_SUCCESS; }
	  else if (equalUID (iid, IID_IAAFRawStorage))
		{ IAAFRawStorage * rrs = this;
		*ppIfc = (void*) rrs;
		AddRef ();
		return AAFRESULT_SUCCESS; }
	  else if (equalUID (iid, IID_IAAFRandomRawStorage))
		{ IAAFRandomRawStorage * rrrs = this;
		*ppIfc = (void*) rrrs;
		AddRef ();
		return AAFRESULT_SUCCESS; }
	  else
		return E_NOINTERFACE; }

  aafUInt32 STDMETHODCALLTYPE
    AddRef () {return ++_refCnt;}

  aafUInt32 STDMETHODCALLTYPE
    Release ()
    { aafUInt32 r = --_refCnt;
	  if (! r) delete this;
	  return r; }

  // IAAFRawStorage overrides
  HRESULT STDMETHODCALLTYPE
    IsReadable (aafBoolean_t * pResult)
    { if (!pResult) return AAFRESULT_NULL_PARAM;
	  *pResult = ((kAAFFileAccess_read == _access) ||
				  (kAAFFileAccess_modify == _access)) ?
		kAAFTrue : kAAFFalse;
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    Read (aafMemPtr_t buf,
		  aafUInt32 bufSize,
		  aafUInt32 *pNumRead)
    { if (! _file) return AAFRESULT_NOT_INITIALIZED;
	  if (! pNumRead) return AAFRESULT_NULL_PARAM;
	  aafBoolean_t readable = kAAFFalse;
	  IsReadable (&readable);
	  if (! readable) return AAFRESULT_NOT_READABLE;
	  size_t actualByteCount = fread(buf, 1, bufSize, _file);
	  *pNumRead = actualByteCount;
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    IsWriteable (aafBoolean_t * pResult)
    { if (!pResult) return AAFRESULT_NULL_PARAM;
	  *pResult = ((kAAFFileAccess_write == _access) ||
				  (kAAFFileAccess_modify == _access)) ?
		kAAFTrue : kAAFFalse;
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    Write (aafMemConstPtr_t buf,
		   aafUInt32 bufSize,
		   aafUInt32 *pNumWritten)
    { if (! _file) return AAFRESULT_NOT_INITIALIZED;
	  if (! pNumWritten) return AAFRESULT_NULL_PARAM;
	  aafBoolean_t writeable = kAAFFalse;
	  IsWriteable (&writeable);
	  if (! writeable) return AAFRESULT_NOT_WRITEABLE;
	  size_t actualByteCount = fwrite(buf, 1, bufSize, _file);
	  *pNumWritten = actualByteCount;
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    Synchronize ()
  { if (! _file) return AAFRESULT_NOT_INITIALIZED;
    fflush(_file);
    return AAFRESULT_SUCCESS; }

  // IAAFRandomRawStorage overrides
  HRESULT STDMETHODCALLTYPE
    ReadAt (aafUInt64 position,
			aafMemPtr_t buf,
			aafUInt32 bufSize,
			aafUInt32 *pNumRead)
  { pvtSetPosition (position);
    return Read (buf, bufSize, pNumRead); }

  HRESULT STDMETHODCALLTYPE
    WriteAt(aafUInt64 position,
			aafMemConstPtr_t buf,
			aafUInt32 bufSize,
			aafUInt32 *pNumWritten)
  { pvtSetPosition (position);
    return Write (buf, bufSize, pNumWritten); }

  HRESULT STDMETHODCALLTYPE
    GetSize (aafUInt64 * pSize)
    { if (! pSize) return AAFRESULT_NULL_PARAM;
	  long int oldPosition = ftell(_file);
	  long int seekStatus = fseek(_file, 0, SEEK_END);
	  assert (seekStatus == 0);
	  long int position = ftell(_file);
	  *pSize = position;
	  pvtSetPosition (oldPosition);
	  return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    IsExtendable (aafBoolean_t * pResult)
      { if (! pResult) return AAFRESULT_NULL_PARAM;
	    *pResult = kAAFTrue;
		return AAFRESULT_SUCCESS; }

  HRESULT STDMETHODCALLTYPE
    GetExtent (aafUInt64 * pExtent)
    { if (! pExtent) return AAFRESULT_NULL_PARAM;
	  *pExtent = _extent;
	return AAFRESULT_NOT_IMPLEMENTED; }

  HRESULT STDMETHODCALLTYPE
    SetExtent (aafUInt64 extent)
    { // Our hack implementation depends on ANSI file stuff to extend
	  // the file for us.  Simply cache the requested extent, and feed
	  // it back to the client if requested (via GetExtent()).
	  _extent = extent;
	  return AAFRESULT_SUCCESS; }

private:
  void pvtSetPosition (aafUInt64 position)
  { int seekStatus = fseek(_file, static_cast<long>(position), SEEK_SET);
    assert (0 == seekStatus); }

  int equalUID(const GUID & a, const GUID & b)
  { return (0 == memcmp((&a), (&b), sizeof (aafUID_t))); }

  aafUInt32       _refCnt;
  aafFileAccess_e _access;  
  FILE *          _file;
  aafUInt64       _extent;
};


//
// Functions to open files using RawStorage API, using raw storages
// implemented by client (in this case, implemented by our own
// CCustomRawStorage).
//
static HRESULT localOpenFileCustomStgWrite
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a custom raw storage.
  IAAFRawStorage * pStg =
	new CCustomRawStorage (pFileName,
						   kAAFFileAccess_modify,
						   kAAFFileExistence_new);

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_write,
								   &aafFileKindAafSSBinary,
								   0,
								   &sIdent,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  pStg->Release ();
  return (*ppFile)->Open();
}


static HRESULT localOpenFileCustomStgRead
  (const aafWChar * pFileName,
   IAAFFile ** ppFile)
{
  assert (pFileName);
  assert (ppFile);

  // Create a custom raw storage.
  IAAFRawStorage * pStg =
	new CCustomRawStorage (pFileName,
						   kAAFFileAccess_read,
						   kAAFFileExistence_existing);

  // create the file and open it.
  checkResult
	(AAFCreateAAFFileOnRawStorage (pStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   ppFile));
  assert (ppFile);
  checkExpression (0 != (*ppFile), AAFRESULT_TEST_FAILED);
  pStg->Release ();
  return (*ppFile)->Open();
}



//
// Use for both standard disk api, and raw storage on disk API.
//
static HRESULT
localCloseDisk (const aafWChar *,
				IAAFFile ** ppFile)
{
  assert (ppFile);
  assert (*ppFile);
  return (*ppFile)->Close ();
}


//
// Use for raw storage in memory, to write out the memory bits to
// disk.
//
static HRESULT
localCloseFileMemStgWrite (const aafWChar * pFileName,
						   IAAFFile ** ppFile)
{
  HRESULT hr;
  assert (pFileName);
  assert (ppFile);
  assert (*ppFile);
  hr = (*ppFile)->Close ();
  if (FAILED (hr)) return hr;

  IAAFRandomFileSP pRandFile;
  hr = (*ppFile)->QueryInterface(IID_IAAFRandomFile,
								 (void **)&pRandFile);
  if (FAILED (hr)) return hr;
  
  IAAFGetFileBitsSP pgfb;
  hr = pRandFile->GetFileBits (&pgfb);
  if (FAILED (hr)) return hr;

  aafUInt64 size = 0;
  hr = pgfb->GetSize (&size);
  if (FAILED (hr)) return hr;

  char cFileName[FILENAME_MAX];
  size_t status = wcstombs(cFileName, pFileName, FILENAME_MAX);
  assert (status != (size_t)-1);

  FILE * f = fopen (cFileName, "wb");
  assert (f);

  aafUInt64 position;
  for (position = 0; position < size; position ++)
	{
	  aafUInt8 c;
	  hr = pgfb->ReadAt (&c, 1, position);
	  if (FAILED (hr)) return hr;
	  fwrite (&c, 1, 1, f);
	}
  fclose (f);

  return S_OK;
}


//
// Table (and types it needs) to drive selection of open/close
// functions for various types of storages.
//
typedef HRESULT (*fileMgmtFunc_t)(const aafWChar *,
								  IAAFFile**);
//
struct fileInfo_t
{
  enum
  {
	kStgTypeDisk
	, kStgTypeDiskStg
	, kStgTypeMemStg
	, kStgTypeCustomStg
  }              storageType;
  wchar_t *      fileName;
  char *         description;
  fileMgmtFunc_t openWriteFunc;
  fileMgmtFunc_t openReadFunc;
  fileMgmtFunc_t closeWriteFunc;
  fileMgmtFunc_t closeReadFunc;

};
//
static const fileInfo_t sFileDescriptions[] =
  {
	{
	  fileInfo_t::kStgTypeDisk,
	  L"AAFRandomRawStgTest.aaf",
	  "Disk",
	  localOpenFileDiskWrite,
	  localOpenFileDiskRead,
	  localCloseDisk,
	  localCloseDisk
	},
	{
	  fileInfo_t::kStgTypeDiskStg,
	  L"AAFRandomRawStgTestDisk.aaf",
	  "Disk Storage",
	  localOpenFileDiskStgWrite,
	  localOpenFileDiskStgRead,
	  localCloseDisk,
	  localCloseDisk
	},
	{
	  fileInfo_t::kStgTypeMemStg,
	  L"AAFRandomRawStgTestMem.aaf",
	  "Mem Storage",
	  localOpenFileMemStgWrite,
	  localOpenFileMemStgRead,
	  localCloseFileMemStgWrite,
	  localCloseDisk
	},
	{
	  fileInfo_t::kStgTypeCustomStg,
	  L"AAFRandomRawStgTestCstm.aaf",
	  "Custom Storage",
	  localOpenFileCustomStgWrite,
	  localOpenFileCustomStgRead,
	  localCloseDisk,
	  localCloseDisk
	}
  };
//
const int kNumDescriptions =
  sizeof (sFileDescriptions) / sizeof (sFileDescriptions[0]);



static HRESULT CreateAAFFile(fileInfo_t info)
{
  IAAFFile *       pFile = NULL;
  bool             bFileOpen = false;
  IAAFHeader *     pHeader = NULL;
  IAAFDictionary * pDictionary = NULL;
  IAAFMob *        pMob = NULL;
  HRESULT  	       hr = S_OK;

  try 
	{
      // Remove the previous test file if any.
      RemoveTestFile(info.fileName);

	  // Check for illegal mode flags.
	  checkModeFlags ();

	  checkResult
		(info.openWriteFunc (info.fileName, &pFile));
	  bFileOpen = true;
  
	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF
	  // objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));

      // Make sure the header returns us the same dictionary as the
	  // file
	  IAAFDictionarySP pDictionaryFromHeader;
	  checkResult(pHeader->GetDictionary(&pDictionaryFromHeader));
	  checkExpression(AreUnksSame(pDictionary,
								  pDictionaryFromHeader)==kAAFTrue);

	  CAAFBuiltinDefs defs (pDictionary);
 	  
	  // Create a concrete subclass of Mob
	  checkResult(defs.cdMasterMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));
    
	  // Initialize the Mob properties
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(MOB_NAME_TEST));

	  // Add the source mob into the tree
	  checkResult(pHeader->AddMob(pMob));

	  // Attempt to save the file.
	  checkResult(pFile->Save());

	  // Attempt to close the file.
	  checkResult(info.closeWriteFunc(info.fileName, &pFile));
	  bFileOpen = false;
    }
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}


  // Cleanup and return
  if (pMob)
	pMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
	pHeader->Release();
			
  if (pFile)
	{	// Close file
	  if (bFileOpen)
		{
		  pFile->Save();
		  info.closeWriteFunc(info.fileName, &pFile);
		}
	  pFile->Release();
	}

  return hr;
}

static HRESULT ReadAAFFile(fileInfo_t info, wchar_t * filename)
{
  IAAFFile *					pFile = NULL;
  bool bFileOpen = false;
  IAAFHeader *				pHeader = NULL;
  IEnumAAFMobs *mobIter = NULL;
  IAAFMob			*pMob = NULL;
  aafNumSlots_t				numMobs, n;
  HRESULT						hr = S_OK;
  aafWChar					name[500];
  aafMobID_t					mobID;
  aafFileRev_t					testRev;
	  
  try
	{
	  // Open the file
	  checkResult
		(info.openReadFunc (filename, &pFile));
	  bFileOpen = true;

	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression (1 == numMobs, AAFRESULT_TEST_FAILED);

	  checkResult(pHeader->GetMobs (NULL, &mobIter));
	  for(n = 0; n < numMobs; n++)
		{
		  checkResult(mobIter->NextOne (&pMob));
		  checkResult(pMob->GetName (name, sizeof(name)));
		  checkResult(pMob->GetMobID (&mobID));
		  checkExpression(wcscmp( name,
								  MOB_NAME_TEST) == 0,
						  AAFRESULT_TEST_FAILED);
		  checkExpression(memcmp(&mobID,
								 &TEST_MobID,
								 sizeof(mobID)) == 0,
						  AAFRESULT_TEST_FAILED);

		  pMob->Release();
		  pMob = NULL;
		}

	  mobIter->Release();
	  mobIter = NULL;

	  checkResult(pFile->GetRevision(&testRev));
	  checkExpression(kAAFRev1 == testRev, AAFRESULT_TEST_FAILED);

	  checkResult(info.closeReadFunc(filename, &pFile));
	  bFileOpen = false;

	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}

  // Cleanup and return
  if (mobIter)
    mobIter->Release();

  if (pMob)
	pMob->Release();

  if (pHeader)
	pHeader->Release();
			
  if (pFile)
	{	// Close file
	  if (bFileOpen)
		info.closeReadFunc(filename, &pFile);
 		pFile->Release();
	}

	return hr;
}

// Required function prototype.
extern "C" HRESULT CAAFRandomRawStorage_test(testMode_t mode);

HRESULT CAAFRandomRawStorage_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  try
	{
	  for (int wType = 0;
		   wType < kNumDescriptions;
		   wType++)
		{
		  if (mode == kAAFUnitTestReadWrite)
			{
			  cout << "  Writing "
				   << sFileDescriptions[wType].description
				   << endl;
			  hr = CreateAAFFile (sFileDescriptions[wType]);
			}
		  else
			hr = AAFRESULT_SUCCESS;
		  if (AAFRESULT_FAILED (hr))
			break;

		  for (int rType = 0;
			   rType < kNumDescriptions;
			   rType++)
			{
			  cout << "    Reading "
				   << sFileDescriptions[rType].description
				   << endl;
			  hr = ReadAAFFile (sFileDescriptions[rType],
								sFileDescriptions[wType].fileName);
			  if (AAFRESULT_FAILED (hr))
				break;
			}
		  if (AAFRESULT_FAILED (hr))
			break;
		}
	}
  catch (...)
	{
	  cerr << "CAAFRandomRawStorage_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
