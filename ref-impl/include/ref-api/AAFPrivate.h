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

//=--------------------------------------------------------------------------=
// Private AAF Interfaces.
//=--------------------------------------------------------------------------=
//
#ifndef __AAFPrivate_h__
#define __AAFPrivate_h__

#ifndef __AAF_h__
#include "AAF.h"
#endif

#ifdef __cplusplus
interface IAAFRoot;
interface IAAFHTMLClip;
interface IAAFHTMLDescriptor;
interface IAAFMetaDictionary;
interface IAAFPropValData;
interface IAAFStreamPropertyValue;
interface IAAFStrongRefArrayValue;
interface IAAFStrongRefSetValue;
interface IAAFStrongRefValue;
interface IAAFTextClip;
interface IAAFWeakRefArrayValue;
interface IAAFWeakRefSetValue;
interface IAAFWeakRefValue;
interface IEnumAAFStorablePropVals;
#else
typedef interface IAAFRoot IAAFRoot;
typedef interface IAAFHTMLClip IAAFHTMLClip;
typedef interface IAAFHTMLDescriptor IAAFHTMLDescriptor;
typedef interface IAAFMetaDictionary IAAFMetaDictionary;
typedef interface IAAFPropValData IAAFPropValData;
typedef interface IAAFStreamPropertyValue IAAFStreamPropertyValue;
typedef interface IAAFStrongRefArrayValue IAAFStrongRefArrayValue;
typedef interface IAAFStrongRefSetValue IAAFStrongRefSetValue;
typedef interface IAAFStrongRefValue IAAFStrongRefValue;
typedef interface IAAFTextClip IAAFTextClip;
typedef interface IAAFWeakRefArrayValue IAAFWeakRefArrayValue;
typedef interface IAAFWeakRefSetValue IAAFWeakRefSetValue;
typedef interface IAAFWeakRefValue IAAFWeakRefValue;
typedef interface IEnumAAFStorablePropVals IEnumAAFStorablePropVals;
#endif

// IAAFRoot

// ************************
//
// Interface IAAFRoot
//
// ************************


#ifndef __IAAFRoot_INTERFACE_DEFINED__
#define __IAAFRoot_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFRoot;

#undef  INTERFACE
#define INTERFACE   IAAFRoot

DECLARE_INTERFACE_(IAAFRoot, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFRoot methods *** */


  //
  // Return private implementation pointer for delegation.
  // NOTE: This is NOT the pointer to the COM object's implementation
  // object!
  //
  STDMETHOD(GetImplRep)(THIS_ void **) PURE;

  //
  // Intialize class extensions. This method is called after the
  // contained Impl object has been completely initialized. This
  // allows the aggregated extension object access to all of the
  // interfaces and property data of its controlling unknown.
  //
  STDMETHOD(InitializeExtension)(THIS_ REFCLSID clsid) PURE;


  END_INTERFACE
};
#endif // __IAAFRoot_INTERFACE_DEFINED__



// IAAFHTMLClip

// ************************
//
// Interface IAAFHTMLClip
//
// ************************



#ifndef __IAAFHTMLClip_INTERFACE_DEFINED__
#define __IAAFHTMLClip_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFHTMLClip;

#undef  INTERFACE
#define INTERFACE   IAAFHTMLClip

DECLARE_INTERFACE_(IAAFHTMLClip, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFHTMLClip methods *** */

  //***********************************************************
  //
  // SetBeginAnchor()
  //
  // Sets the Begin Anchor string property.
  //
  // Set the BeginAnchor property to the value specified in
  // pBeginAnchor.  A copy is made of the data so the caller
  // retains ownership of the *pBeginAnchor buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pBeginAnchor pointer is valid.
  // 
  // If this method fails the BeginAnchor property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBeginAnchor arg is NULL.
  //
  STDMETHOD(SetBeginAnchor) (THIS_
    // buffer from which BeginAnchor is to be read
    /*[in, string]*/ aafCharacter_constptr  pBeginAnchor) PURE;


  //***********************************************************
  //
  // GetBeginAnchor()
  //
  // Gets the Begin Anchor string property.
  // 
  // Writes the BeginAnchor property, with a trailing null
  // character, into the pBeginAnchor buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the BeginAnchor property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetBeginAnchorBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pBeginAnchor.
  // 
  // Succeeds if:
  // - The pBeginAnchor pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   BeginAnchor.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBeginAnchor arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold BeginAnchor.
  //
  STDMETHOD(GetBeginAnchor) (THIS_
    // buffer into which BeginAnchor is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pBeginAnchor,

    // size of *pBeginAnchor buffer in bytes
    /*[in]*/ aafUInt32  bufSize) PURE;


  //***********************************************************
  //
  // GetBeginAnchorBufLen()
  //
  // Returns size of buffer (in bytes) required for GetBeginAnchor().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD(GetBeginAnchorBufLen) (THIS_
    // size of required buffer, in bytes
    /*[out]*/ aafUInt32 *  pBufSize) PURE;


  //***********************************************************
  //
  // SetEndAnchor()
  //
  // Sets the End Anchor string property.
  //
  // Set the EndAnchor property to the value specified in
  // pEndAnchor.  A copy is made of the data so the caller
  // retains ownership of the *pEndAnchor buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pEndAnchor pointer is valid.
  // 
  // If this method fails the EndAnchor property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEndAnchor arg is NULL.
  //
  STDMETHOD(SetEndAnchor) (THIS_
    // buffer from which EndAnchor is to be read
    /*[in, string]*/ aafCharacter_constptr  pEndAnchor) PURE;


  //***********************************************************
  //
  // GetEndAnchor()
  //
  // Gets the End Anchor string property.
  // 
  // Writes the EndAnchor property, with a trailing null
  // character, into the pEndAnchor buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the EndAnchor property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetEndAnchorBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pEndAnchor.
  // 
  // Succeeds if:
  // - The pEndAnchor pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   EndAnchor.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pEndAnchor arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold EndAnchor.
  //
  STDMETHOD(GetEndAnchor) (THIS_
    // buffer into which EndAnchor is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pEndAnchor,

    // size of *pEndAnchor buffer in bytes
    /*[in]*/ aafUInt32  bufSize) PURE;


  //***********************************************************
  //
  // GetEndAnchorBufLen()
  //
  // Returns size of buffer (in bytes) required for GetEndAnchor().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD(GetEndAnchorBufLen) (THIS_
    // size of required buffer, in bytes
    /*[out]*/ aafUInt32 *  pBufSize) PURE;

  END_INTERFACE
};
#endif // __IAAFHTMLClip_INTERFACE_DEFINED__



// IAAFHTMLDescriptor

// ************************
//
// Interface IAAFHTMLDescriptor
//
// ************************





#ifndef __IAAFHTMLDescriptor_INTERFACE_DEFINED__
#define __IAAFHTMLDescriptor_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFHTMLDescriptor;

#undef  INTERFACE
#define INTERFACE   IAAFHTMLDescriptor

DECLARE_INTERFACE_(IAAFHTMLDescriptor, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFHTMLDescriptor methods *** */



  END_INTERFACE
};
#endif // __IAAFHTMLDescriptor_INTERFACE_DEFINED__



// IAAFMetaDictionary

// ************************
//
// Interface IAAFMetaDictionary
//
// ************************



#ifndef __IAAFMetaDictionary_INTERFACE_DEFINED__
#define __IAAFMetaDictionary_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFMetaDictionary;

#undef  INTERFACE
#define INTERFACE   IAAFMetaDictionary

DECLARE_INTERFACE_(IAAFMetaDictionary, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFMetaDictionary methods *** */


  END_INTERFACE
};
#endif // __IAAFMetaDictionary_INTERFACE_DEFINED__



// IAAFPropValData

// ************************
//
// Interface IAAFPropValData
//
// ************************



#ifndef __IAAFPropValData_INTERFACE_DEFINED__
#define __IAAFPropValData_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFPropValData;

#undef  INTERFACE
#define INTERFACE   IAAFPropValData

DECLARE_INTERFACE_(IAAFPropValData, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFPropValData methods *** */


  END_INTERFACE
};
#endif // __IAAFPropValData_INTERFACE_DEFINED__



// IAAFStreamPropertyValue

// ************************
//
// Interface IAAFStreamPropertyValue
//
// ************************



#ifndef __IAAFStreamPropertyValue_INTERFACE_DEFINED__
#define __IAAFStreamPropertyValue_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFStreamPropertyValue;

#undef  INTERFACE
#define INTERFACE   IAAFStreamPropertyValue

DECLARE_INTERFACE_(IAAFStreamPropertyValue, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFStreamPropertyValue methods *** */


  END_INTERFACE
};
#endif // __IAAFStreamPropertyValue_INTERFACE_DEFINED__



// IAAFStrongRefArrayValue

// ************************
//
// Interface IAAFStrongRefArrayValue
//
// ************************



#ifndef __IAAFStrongRefArrayValue_INTERFACE_DEFINED__
#define __IAAFStrongRefArrayValue_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFStrongRefArrayValue;

#undef  INTERFACE
#define INTERFACE   IAAFStrongRefArrayValue

DECLARE_INTERFACE_(IAAFStrongRefArrayValue, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFStrongRefArrayValue methods *** */


  END_INTERFACE
};
#endif // __IAAFStrongRefArrayValue_INTERFACE_DEFINED__



// IAAFStrongRefSetValue

// ************************
//
// Interface IAAFStrongRefSetValue
//
// ************************



#ifndef __IAAFStrongRefSetValue_INTERFACE_DEFINED__
#define __IAAFStrongRefSetValue_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFStrongRefSetValue;

#undef  INTERFACE
#define INTERFACE   IAAFStrongRefSetValue

DECLARE_INTERFACE_(IAAFStrongRefSetValue, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFStrongRefSetValue methods *** */


  END_INTERFACE
};
#endif // __IAAFStrongRefSetValue_INTERFACE_DEFINED__



// IAAFStrongRefValue

// ************************
//
// Interface IAAFStrongRefValue
//
// ************************



#ifndef __IAAFStrongRefValue_INTERFACE_DEFINED__
#define __IAAFStrongRefValue_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFStrongRefValue;

#undef  INTERFACE
#define INTERFACE   IAAFStrongRefValue

DECLARE_INTERFACE_(IAAFStrongRefValue, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFStrongRefValue methods *** */


  END_INTERFACE
};
#endif // __IAAFStrongRefValue_INTERFACE_DEFINED__



// IAAFTextClip

// ************************
//
// Interface IAAFTextClip
//
// ************************





#ifndef __IAAFTextClip_INTERFACE_DEFINED__
#define __IAAFTextClip_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFTextClip;

#undef  INTERFACE
#define INTERFACE   IAAFTextClip

DECLARE_INTERFACE_(IAAFTextClip, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFTextClip methods *** */



  END_INTERFACE
};
#endif // __IAAFTextClip_INTERFACE_DEFINED__



// IAAFWeakRefArrayValue

// ************************
//
// Interface IAAFWeakRefArrayValue
//
// ************************



#ifndef __IAAFWeakRefArrayValue_INTERFACE_DEFINED__
#define __IAAFWeakRefArrayValue_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFWeakRefArrayValue;

#undef  INTERFACE
#define INTERFACE   IAAFWeakRefArrayValue

DECLARE_INTERFACE_(IAAFWeakRefArrayValue, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFWeakRefArrayValue methods *** */


  END_INTERFACE
};
#endif // __IAAFWeakRefArrayValue_INTERFACE_DEFINED__



// IAAFWeakRefSetValue

// ************************
//
// Interface IAAFWeakRefSetValue
//
// ************************



#ifndef __IAAFWeakRefSetValue_INTERFACE_DEFINED__
#define __IAAFWeakRefSetValue_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFWeakRefSetValue;

#undef  INTERFACE
#define INTERFACE   IAAFWeakRefSetValue

DECLARE_INTERFACE_(IAAFWeakRefSetValue, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFWeakRefSetValue methods *** */


  END_INTERFACE
};
#endif // __IAAFWeakRefSetValue_INTERFACE_DEFINED__



// IAAFWeakRefValue

// ************************
//
// Interface IAAFWeakRefValue
//
// ************************



#ifndef __IAAFWeakRefValue_INTERFACE_DEFINED__
#define __IAAFWeakRefValue_INTERFACE_DEFINED__

EXTERN_C const IID IID_IAAFWeakRefValue;

#undef  INTERFACE
#define INTERFACE   IAAFWeakRefValue

DECLARE_INTERFACE_(IAAFWeakRefValue, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IAAFWeakRefValue methods *** */


  END_INTERFACE
};
#endif // __IAAFWeakRefValue_INTERFACE_DEFINED__



// IEnumAAFStorablePropVals

// ************************
//
// Interface IEnumAAFStorablePropVals
//
// ************************



#ifndef __IEnumAAFStorablePropVals_INTERFACE_DEFINED__
#define __IEnumAAFStorablePropVals_INTERFACE_DEFINED__

EXTERN_C const IID IID_IEnumAAFStorablePropVals;

#undef  INTERFACE
#define INTERFACE   IEnumAAFStorablePropVals

DECLARE_INTERFACE_(IEnumAAFStorablePropVals, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;

  /* *** IEnumAAFStorablePropVals methods *** */


  END_INTERFACE
};
#endif // __IEnumAAFStorablePropVals_INTERFACE_DEFINED__



#endif // __AAFPrivate_h__
