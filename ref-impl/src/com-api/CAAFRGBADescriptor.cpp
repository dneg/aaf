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



#include "CAAFRGBADescriptor.h"
#include "ImplAAFRGBADescriptor.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

class OMAssertionViolation;

#include <assert.h>
#include <string.h>






// CLSID for AAFRGBADescriptor 
// {e684d771-b935-11d2-bf9d-00104bc9156d}
EXTERN_C const CLSID CLSID_AAFRGBADescriptor = { 0xe684d771, 0xb935, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };





CAAFRGBADescriptor::CAAFRGBADescriptor (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFDigitalImageDescriptor (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFRGBADescriptor * newRep;
      newRep = new ImplAAFRGBADescriptor;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFRGBADescriptor::~CAAFRGBADescriptor ()
{
}



HRESULT STDMETHODCALLTYPE
    CAAFRGBADescriptor::SetPixelLayout (aafUInt32  numberElements,
        aafRGBAComponent_t*  PixelLayoutArray)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetPixelLayout
       (numberElements,
        PixelLayoutArray);
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
    CAAFRGBADescriptor::CountPixelLayoutElements (aafUInt32 *  pResult)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountPixelLayoutElements
       (pResult);
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
    CAAFRGBADescriptor::GetPixelLayout (aafUInt32  numberElements,
        aafRGBAComponent_t*  PixelLayoutArray)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetPixelLayout
       (numberElements,
        PixelLayoutArray);
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
    CAAFRGBADescriptor::SetPalette (aafUInt32  numberElements,
        aafUInt8*  pPalette)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetPalette
       (numberElements,
        pPalette);
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
    CAAFRGBADescriptor::GetPalette (aafUInt32  numberRecords,
        aafUInt8*  pPalette)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetPalette
       (numberRecords,
        pPalette);
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
    CAAFRGBADescriptor::GetPaletteSize (aafUInt32 *  pNumberBytes)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetPaletteSize
       (pNumberBytes);
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
    CAAFRGBADescriptor::CountPaletteLayoutElements (aafUInt32 *  pResult)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->CountPaletteLayoutElements
       (pResult);
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
    CAAFRGBADescriptor::SetPaletteLayout (aafUInt32  numberElements,
        aafRGBAComponent_t*  PaletteLayoutArray)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->SetPaletteLayout
       (numberElements,
        PaletteLayoutArray);
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
    CAAFRGBADescriptor::GetPaletteLayout (aafUInt32  numberElements,
        aafRGBAComponent_t*  PaletteLayoutArray)
{
  HRESULT hr;

  ImplAAFRGBADescriptor * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplAAFRGBADescriptor*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->GetPaletteLayout
       (numberElements,
        PaletteLayoutArray);
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


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFRGBADescriptor::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFRGBADescriptor)) 
    { 
        *ppvObj = (IAAFRGBADescriptor *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFDigitalImageDescriptor::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFRGBADescriptor)

