//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#include "CAAFEssenceAccess.h"
#include "ImplAAFEssenceAccess.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"

#include <assert.h>
#include <string.h>



#include "CAAFSourceMob.h"
#include "ImplAAFSourceMob.h"

#include "CAAFSourceClip.h"
#include "ImplAAFSourceClip.h"

#include "CAAFEssenceFormat.h"
#include "ImplAAFEssenceFormat.h"

#include "CAAFLocator.h"
#include "ImplAAFLocator.h"

#include "CAAFDataDef.h"
#include "ImplAAFDataDef.h"

// CLSID for AAFEssenceAccess 
// {aed97eb1-2bc8-11D2-bfaa-006097116212}
EXTERN_C const CLSID CLSID_AAFEssenceAccess = { 0xaed97eb1, 0x2bc8, 0x11D2, { 0xbf, 0xaa, 0x00, 0x60, 0x97, 0x11, 0x62, 0x12 } };





CAAFEssenceAccess::CAAFEssenceAccess (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFEssenceAccess * newRep;
      newRep = new ImplAAFEssenceAccess;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFEssenceAccess::~CAAFEssenceAccess ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::SetEssenceCodecFlavour (aafUID_constref  flavour)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetEssenceCodecFlavour
       (flavour);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::WriteSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  samplesWritten,
        aafUInt32 *  bytesWritten)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->WriteSamples
       (nSamples,
        buflen,
        buffer,
        samplesWritten,
        bytesWritten);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::GetLargestSampleSize (IAAFDataDef * pDataDef,
        aafLength_t*  maxSize)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);

  //
  // set up for pDataDef
  //
  ImplAAFDataDef * internalpDataDef = NULL;
  if (pDataDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pDataDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpDataDef = static_cast<ImplAAFDataDef*>(arg);
      assert (internalpDataDef);
    }

  try
    {
      hr = ptr->GetLargestSampleSize
       (internalpDataDef,
        maxSize);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // no cleanup necessary for pDataDef
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::GetIndexedSampleSize (IAAFDataDef * pDataDef,
        aafPosition_t  frameNum,
        aafLength_t*  frameSize)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);

  //
  // set up for pDataDef
  //
  ImplAAFDataDef * internalpDataDef = NULL;
  if (pDataDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pDataDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpDataDef = static_cast<ImplAAFDataDef*>(arg);
      assert (internalpDataDef);
    }

  try
    {
      hr = ptr->GetIndexedSampleSize
       (internalpDataDef,
        frameNum,
        frameSize);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // no cleanup necessary for pDataDef
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::SetTransformParameters (IAAFEssenceFormat * op)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);

  //
  // set up for op
  //
  ImplAAFEssenceFormat * internalop = NULL;
  if (op)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = op->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalop = static_cast<ImplAAFEssenceFormat*>(arg);
      assert (internalop);
    }

  try
    {
      hr = ptr->SetTransformParameters
       (internalop);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // no cleanup necessary for op
  //
  return hr;
}

	 

HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::CountSamples (IAAFDataDef * pDataDef,
        aafLength_t *  result)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);

  //
  // set up for pDataDef
  //
  ImplAAFDataDef * internalpDataDef = NULL;
  if (pDataDef)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = pDataDef->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalpDataDef = static_cast<ImplAAFDataDef*>(arg);
      assert (internalpDataDef);
    }

  try
    {
      hr = ptr->CountSamples
       (internalpDataDef,
        result);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // no cleanup necessary for pDataDef
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::ReadSamples (aafUInt32  nSamples,
        aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  samplesRead,
        aafUInt32 *  bytesRead)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->ReadSamples
       (nSamples,
        buflen,
        buffer,
        samplesRead,
        bytesRead);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::Seek (aafPosition_t  sampleFrameNum)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Seek
       (sampleFrameNum);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::GetFileFormat (IAAFEssenceFormat * opsTemplate,
        IAAFEssenceFormat ** opsResult)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);

  //
  // set up for opsTemplate
  //
  ImplAAFEssenceFormat * internalopsTemplate = NULL;
  if (opsTemplate)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = opsTemplate->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalopsTemplate = static_cast<ImplAAFEssenceFormat*>(arg);
      assert (internalopsTemplate);
    }
  //
  // set up for opsResult
  //
  ImplAAFEssenceFormat * internalopsResult = NULL;
  ImplAAFEssenceFormat ** pinternalopsResult = NULL;
  if (opsResult)
    {
      pinternalopsResult = &internalopsResult;
    }

  try
    {
      hr = ptr->GetFileFormat
       (internalopsTemplate,
        pinternalopsResult);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // no cleanup necessary for opsTemplate
  //
  //
  // cleanup for opsResult
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalopsResult)
        {
          pUnknown = static_cast<IUnknown *> (internalopsResult->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFEssenceFormat, (void **)opsResult);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalopsResult->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::GetFileFormatParameterList (IAAFEssenceFormat ** ops)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);

  //
  // set up for ops
  //
  ImplAAFEssenceFormat * internalops = NULL;
  ImplAAFEssenceFormat ** pinternalops = NULL;
  if (ops)
    {
      pinternalops = &internalops;
    }

  try
    {
      hr = ptr->GetFileFormatParameterList
       (pinternalops);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ops
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalops)
        {
          pUnknown = static_cast<IUnknown *> (internalops->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFEssenceFormat, (void **)ops);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalops->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::GetEmptyFileFormat (IAAFEssenceFormat ** ops)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);

  //
  // set up for ops
  //
  ImplAAFEssenceFormat * internalops = NULL;
  ImplAAFEssenceFormat ** pinternalops = NULL;
  if (ops)
    {
      pinternalops = &internalops;
    }

  try
    {
      hr = ptr->GetEmptyFileFormat
       (pinternalops);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ops
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalops)
        {
          pUnknown = static_cast<IUnknown *> (internalops->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFEssenceFormat, (void **)ops);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalops->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::PutFileFormat (IAAFEssenceFormat * ops)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);

  //
  // set up for ops
  //
  ImplAAFEssenceFormat * internalops = NULL;
  if (ops)
    {
      HRESULT hStat;
      IAAFRoot * iObj;
      ImplAAFRoot *arg;
      hStat = ops->QueryInterface (IID_IAAFRoot, (void **)&iObj);
      assert (SUCCEEDED (hStat));
      assert (iObj);
      hStat = iObj->GetImplRep((void **)&arg);
      assert (SUCCEEDED (hStat));
      iObj->Release(); // we are through with this interface pointer.
      internalops = static_cast<ImplAAFEssenceFormat*>(arg);
      assert (internalops);
    }

  try
    {
      hr = ptr->PutFileFormat
       (internalops);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // no cleanup necessary for ops
  //
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::GetCodecName (aafUInt32  namelen,
        aafCharacter *  name)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetCodecName
       (namelen,
        name);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::GetCodecID (aafCodecID_t *  codecID)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetCodecID
       (codecID);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::CompleteWrite ()
{
  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->CompleteWrite();
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::WriteMultiSamples (aafUInt16  arrayElemCount,
        aafmMultiXfer_t *  xferArray,
        aafmMultiResult_t *  resultArray)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);




  hr = ptr->WriteMultiSamples (arrayElemCount,
    xferArray,
    resultArray);



  return hr;
}



HRESULT STDMETHODCALLTYPE
    CAAFEssenceAccess::ReadMultiSamples (aafUInt16  elemCount,
        aafmMultiXfer_t *  xferArray,
        aafmMultiResult_t *  resultArray)
{
  HRESULT hr;

  ImplAAFEssenceAccess * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFEssenceAccess*> (pO);
  assert (ptr);




  hr = ptr->ReadMultiSamples (elemCount,
    xferArray,
    resultArray);



  return hr;
}



//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFEssenceAccess::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFEssenceAccess)) 
    { 
        *ppvObj = (IAAFEssenceAccess *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    if (EQUAL_UID(riid,IID_IAAFEssenceMultiAccess)) 
    { 
        *ppvObj = (IAAFEssenceMultiAccess *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFEssenceAccess)
