/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
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
#include <assert.h>

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFRawStorage>  IAAFRawStorageSP;
typedef IAAFSmartPointer<IAAFFile>        IAAFFileSP;
typedef IAAFSmartPointer<IAAFRandomFile>  IAAFRandomFileSP;
typedef IAAFSmartPointer<IAAFGetFileBits> IAAFGetFileBitsSP;

// Required function prototype.
extern "C" HRESULT CAAFGetFileBits_test(testMode_t /*mode*/);

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


static void fillJunk (aafMemPtr_t buf, size_t size)
{
  for (size_t i = 0; i < size; i++)
	{
	  buf[i] = 0xa5;
	}
}

static bool testJunk (aafMemPtr_t buf, size_t size)
{
  for (size_t i = 0; i < size; i++)
	{
	  if (buf[i] != 0xa5)
		return false;
	}
  return true;
}

static HRESULT GetFileBitsTest ()
{
  // Create a memory raw storage and a file on it, to be opened for
  // writing.  We'll use this to get the GetFileBits to be tested.
  IAAFRawStorageSP pWriteStg;
  checkResult
	(AAFCreateRawStorageMemory (kAAFFileAccess_modify,
								&pWriteStg));
  IAAFFileSP pWriteFile;
  checkResult
	(AAFCreateAAFFileOnRawStorage (pWriteStg,
								   kAAFFileExistence_new,
								   kAAFFileAccess_modify,
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

  checkResult (pWriteFile->Open());
  checkResult (pWriteFile->Save());
  checkResult (pWriteFile->Close());

  //
  // Cool.  Now we can get the GetFileBits interface to test.
  //

  IAAFGetFileBitsSP pgfb;
  checkResult (pWriteRandFile->GetFileBits (&pgfb));

  //
  // test GetSize
  //
  HRESULT hr;

  // null param
  hr = pgfb->GetSize (0);
  checkExpression (AAFRESULT_NULL_PARAM == hr, AAFRESULT_TEST_FAILED);

  // Get the size
  aafUInt64 size = 0;
  checkResult (pgfb->GetSize (&size));
  // we know the file is not empty
  checkExpression (0 != size);

  //
  // test ReadAt
  //

  // null param
  hr = pgfb->ReadAt (0, 1, 0);
  checkExpression (AAFRESULT_NULL_PARAM == hr, AAFRESULT_TEST_FAILED);

  // Offset too large
  aafUInt8 foo[100];
  fillJunk (foo, sizeof (foo));
  checkExpression (testJunk (foo, sizeof (foo)), AAFRESULT_TEST_FAILED);
  hr = pgfb->ReadAt (foo, sizeof (foo), size);
  checkExpression (AAFRESULT_OFFSET_SIZE == hr, AAFRESULT_TEST_FAILED);
  checkExpression (testJunk (foo, sizeof (foo)), AAFRESULT_TEST_FAILED);

  // Size too large
  hr = pgfb->ReadAt (foo, 0x7fffffff, size);
  checkExpression (AAFRESULT_OFFSET_SIZE == hr, AAFRESULT_TEST_FAILED);
  checkExpression (testJunk (foo, sizeof (foo)), AAFRESULT_TEST_FAILED);

  // Size/offset too large
  hr = pgfb->ReadAt (foo, sizeof (foo), size-(sizeof (foo)/2));
  checkExpression (AAFRESULT_OFFSET_SIZE == hr, AAFRESULT_TEST_FAILED);
  checkExpression (testJunk (foo, sizeof (foo)), AAFRESULT_TEST_FAILED);

  // See if we can read at start
  checkResult (pgfb->ReadAt (foo, sizeof (foo), 0));
  checkExpression (!testJunk (foo, sizeof (foo)), AAFRESULT_TEST_FAILED);

  // See if we can read at end
  fillJunk (foo, sizeof (foo));
  checkResult (pgfb->ReadAt (foo, sizeof (foo), size-sizeof(foo)));
  checkExpression (!testJunk (foo, sizeof (foo)), AAFRESULT_TEST_FAILED);

  return AAFRESULT_SUCCESS;
}

HRESULT CAAFGetFileBits_test(testMode_t /*mode*/)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  try
	{
	  hr = GetFileBitsTest ();
	}
  catch (...)
	{
	  cerr << "CAAFGetFileBitsTest_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}
