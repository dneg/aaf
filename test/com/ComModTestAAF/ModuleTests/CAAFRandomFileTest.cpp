//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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

#include <iostream>
using namespace std;
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


static HRESULT TestGetSetFileBits (
    aafUID_constref fileKind,
    aafProductIdentification_constref productID)
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
								   kAAFFileAccess_modify,
								   &kAAFFileKind_Aaf4KBinary,
								   0,
								   &productID,
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
extern "C" HRESULT CAAFRandomFile_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

HRESULT CAAFRandomFile_test(
    testMode_t /* mode */,
    aafUID_t fileKind,
    testRawStorageType_t /* rawStorageType */,
    aafProductIdentification_t productID)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  try
	{
	  hr = TestGetSetFileBits (fileKind, productID);
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
