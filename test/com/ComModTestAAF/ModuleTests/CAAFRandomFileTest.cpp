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

#include "AAF.h"
#include "AAFResult.h"
#include "AAFFileMode.h"
#include "AAFDefUIDs.h"
#include "AAFFileKinds.h"

#include "ModuleTest.h"
#include "CAAFBuiltinDefs.h"

#include <iostream.h>
#include <stdio.h>
#include <assert.h>

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFRawStorage>  IAAFRawStorageSP;
typedef IAAFSmartPointer<IAAFFile>        IAAFFileSP;
typedef IAAFSmartPointer<IAAFRandomFile>  IAAFRandomFileSP;
typedef IAAFSmartPointer<IAAFSetFileBits> IAAFSetFileBitsSP;
typedef IAAFSmartPointer<IAAFGetFileBits> IAAFGetFileBitsSP;

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


static HRESULT TestGetSetFileBits ()
{
  HRESULT hr;
  // Create a disk raw storage and a file on it, to be opened for
  // writing.
  IAAFRawStorageSP pWriteStg;
  checkResult
	(AAFCreateRawStorageMemory (kAAFFileAccess_modify,
								&pWriteStg));
	  
  IAAFFileSP pWriteFile;
  checkResult
	(AAFCreateAAFFileOnRawStorage (pWriteStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_write,
								   &aafFileKindAafSSBinary,
								   0,
								   &sIdent,
								   &pWriteFile));
  assert (pWriteFile);
  checkExpression (0 != (IAAFFile*)pWriteFile,
				   AAFRESULT_TEST_FAILED);
  
  IAAFRandomFileSP pWriteRandFile;
  checkResult(pWriteFile->QueryInterface(IID_IAAFRandomFile,
										 (void **)&pWriteRandFile));

  // Test for null param
  hr = pWriteRandFile->GetFileBits (0);
  checkExpression (AAFRESULT_NULL_PARAM == hr, AAFRESULT_TEST_FAILED);
  hr = pWriteRandFile->SetFileBits (0);
  checkExpression (AAFRESULT_NULL_PARAM == hr, AAFRESULT_TEST_FAILED);

  IAAFGetFileBitsSP pgfb;
  IAAFSetFileBitsSP psfb;
  // GetBits should fail because file's never been opened
  hr = pWriteRandFile->GetFileBits (&pgfb);
  checkExpression (AAFRESULT_BADOPEN == hr, AAFRESULT_TEST_FAILED);

  checkResult (pWriteFile->Open());
  // Both GetBits and SetBits should fail while file is open.
  hr = pWriteRandFile->GetFileBits (&pgfb);
  checkExpression (AAFRESULT_BADCLOSE == hr, AAFRESULT_TEST_FAILED);
  hr = pWriteRandFile->SetFileBits (&psfb);
  checkExpression (AAFRESULT_BADOPEN == hr, AAFRESULT_TEST_FAILED);
  checkResult (pWriteFile->Save());
  checkResult (pWriteFile->Close());

  // This time GetBits should succeed.
  checkResult (pWriteRandFile->GetFileBits (&pgfb));
  // SetBits should fail because file's already been opened.
  hr = pWriteRandFile->SetFileBits (&psfb);
  checkExpression (AAFRESULT_BADOPEN == hr, AAFRESULT_TEST_FAILED);

  // Now let's create a mem raw storage and a file on it, this time to
  // be opened for reading.
  IAAFRawStorageSP pReadStg;
  checkResult
	(AAFCreateRawStorageMemory (kAAFFileAccess_read,
								&pReadStg));

  IAAFFileSP pReadFile;
  checkResult
	(AAFCreateAAFFileOnRawStorage (pReadStg,
								   kAAFFileExistence_existing,
								   kAAFFileAccess_read,
								   0,
								   0,
								   0,
								   &pReadFile));
  assert (pReadFile);
  checkExpression (0 != (IAAFFile*)pReadFile, AAFRESULT_TEST_FAILED);
  
  IAAFRandomFileSP pReadRandFile;
  checkResult(pReadFile->QueryInterface(IID_IAAFRandomFile,
										(void **)&pReadRandFile));
  
  // GetBits should fail because file's never been opened
  hr = pReadRandFile->GetFileBits (&pgfb);
  checkExpression (AAFRESULT_BADOPEN == hr, AAFRESULT_TEST_FAILED);
  // SetBits should succeed 'cause file's never been opened
  checkResult (pWriteRandFile->GetFileBits (&pgfb));

  // Now let's import the bits from the file-for-write we made before,
  // so we can legally open this one.
  // First, re-get the GetFileBits from the write file...
  checkResult (pWriteRandFile->GetFileBits (&pgfb));
  checkResult (pReadRandFile->SetFileBits (&psfb));
  aafUInt64 size = 0;
  checkResult (pgfb->GetSize (&size));
  for (aafUInt64 pos = 0; pos < size; pos++)
	{
	  aafUInt8 c;
	  checkResult (pgfb->ReadAt (&c, 1, pos));
	  checkResult (psfb->WriteAt (&c, 1, pos));
	}

  checkResult (pReadFile->Open());
  // SetBits should fail because file is still open.
  hr = pReadRandFile->SetFileBits (&psfb);
  checkExpression (AAFRESULT_BADOPEN == hr, AAFRESULT_TEST_FAILED);
  checkResult (pReadFile->Close());

  // GetBits Should still fail, because file's never been opened for
  // *write*.
  hr = pReadRandFile->GetFileBits (&pgfb);
  checkExpression (AAFRESULT_NOT_WRITEABLE == hr,
				   AAFRESULT_TEST_FAILED);

  // SetBits should fail, again because file's been opened.
  hr = pReadRandFile->SetFileBits (&psfb);
  checkExpression (AAFRESULT_BADOPEN == hr, AAFRESULT_TEST_FAILED);

  return AAFRESULT_SUCCESS;
}



// Required function prototype.
extern "C" HRESULT CAAFRandomFile_test(testMode_t /*mode*/);

HRESULT CAAFRandomFile_test(testMode_t /*mode*/)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  try
	{
	  hr = TestGetSetFileBits ();
	}
  catch (...)
	{
	  cerr << "CAAFRandomFile_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  if (SUCCEEDED(hr))
	{
	  // Open() and SaveCopyAs() method were not in the current version
	  // of the toolkit at the time this module test was written.
	  hr = AAFRESULT_NOT_IN_CURRENT_VERSION;
	}

  return hr;
}
