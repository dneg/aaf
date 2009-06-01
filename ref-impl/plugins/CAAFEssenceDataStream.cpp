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

#include "CAAFEssenceDataStream.h"

#include "AAF.h" // need declaration for IAAFEssenceData.

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "AAFUtils.h"

#include "AAFSmartPointer.h"

// declare a handy typedef for smart pointer
#define TYPEDEF_SP( Object ) \
	typedef IAAFSmartPointer<IAAF##Object> IAAF##Object##SP

// smartpointer typedefs for common objects
TYPEDEF_SP( EssenceData );
TYPEDEF_SP( EssenceData2 );
TYPEDEF_SP( PlainEssenceData );
TYPEDEF_SP( KLVEssenceDataParameters );
TYPEDEF_SP( Mob );
TYPEDEF_SP( SourceMob );
TYPEDEF_SP( MobSlot );

const aafUID_t NULL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

// Debugging log function which is optimised away for default builds
#include <stdarg.h>
inline void plugin_trace(const char *fmt, ...)
{
#ifdef PLUGIN_TRACE
	va_list		ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif
}

// local function for simplifying error handling.
inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw HRESULT(r);
}

inline void checkExpression(bool test, AAFRESULT r)
{
  if (!test)
    throw r;
}

inline void checkAssertion(bool test)
{
  if (!test)
    throw HRESULT(AAFRESULT_ASSERTION_VIOLATION);
}

const CLSID CLSID_AAFEssenceDataStream = { 0x42A63FE1, 0x968A, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
//const IID IID_IAAFEssenceData = { 0x6a33f4e2, 0x8ed6, 0x11d2, { 0xbf, 0x9d, 0x00, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };


CAAFEssenceDataStream::CAAFEssenceDataStream (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown),
  _data(NULL),
  _eek(NULL_UID)
{
}


CAAFEssenceDataStream::~CAAFEssenceDataStream ()
{
	if(_data != NULL)
		_data->Release();
	_data = NULL;
}

HRESULT STDMETHODCALLTYPE
CAAFEssenceDataStream::Init(
            /* [in] */ IUnknown  *essenceData)
{
  if (NULL == essenceData)
    return AAFRESULT_NULL_PARAM;

  if (NULL != _data)
    return AAFRESULT_ALREADY_INITIALIZED;

  IAAFEssenceData2* essenceData2 = NULL;
  AAFRESULT hr = essenceData->QueryInterface (IID_IAAFEssenceData2, (void **)&essenceData2);
  if (AAFRESULT_SUCCEEDED(hr))
  {
    hr = essenceData2->GetPlainEssenceData(0, &_data);
    essenceData2->Release();
    essenceData2 = NULL;
  }

  return hr;
}

  //
  // IAAFEssenceDataStream2 methods.
  //
HRESULT STDMETHODCALLTYPE
CAAFEssenceDataStream::GetEssenceData
			(// @parm [in]  essence data
			IAAFEssenceData ** ppEssenceData)
{
  if (NULL == ppEssenceData)
    return AAFRESULT_NULL_PARAM;

  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

  // FIXME: this will always fail because _data is an IAAFPlainEssenceData, whose QI does not know about IID_IAAFEssenceData
  // return _data->QueryInterface( IID_IAAFEssenceData, (void **)ppEssenceData );
  // should be named: GetPlainEssenceData() coded return _data;

  return _data->QueryInterface( IID_IAAFPlainEssenceData, (void **)ppEssenceData );

}

HRESULT STDMETHODCALLTYPE
CAAFEssenceDataStream::GetEssenceElementKey
        (// @parm [out,ref] key of the essence elements in this stream
         aafUID_t *  pEssenceElementKey)
{
  if (NULL == pEssenceElementKey)
    return AAFRESULT_NULL_PARAM;

  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

  *pEssenceElementKey = _eek;

  AAFRESULT hr = AAFRESULT_SUCCESS;
  return hr;
}

// some well-known EssenceElementKeys
const aafUID_t GC_EEK = { 0x0d010301, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01 } };

// the Avid legacy EEK for DNxHD
const aafUID_t Avid_EEK = { 0x0e040301, 0x0000, 0x0000, {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01 } };

// set EssenceElementKey from member variables and known consts
HRESULT STDMETHODCALLTYPE
CAAFEssenceDataStream::SetEssenceElementKey
        (// @parm [in] essence element key
         aafUID_constref	eek,
         // @parm [in] essence element kind
         aafUInt8  			eeKind,
         // @parm [in] essence element count
         aafUInt8  			eeCount,
         // @parm [in] essence element type
         aafUInt8  			eeType,
         // @parm [in] essence element index
         aafUInt8  			eeIndex,
		 // @parm [in] logical slot id of source slot
         aafSlotID_t  		sourceSlotID )
{
	plugin_trace("CAAFEssenceDataStream::SetEssenceElementKey()\n");

	if (NULL == _data) return AAFRESULT_NOT_INITIALIZED;

	AAFRESULT hr = AAFRESULT_SUCCESS;
	try
	{
		hr = AAFRESULT_INTERNAL_ERROR;

		// SetEEK
		// we know how to set the EssenceElementKey on an EssenceData object
		// via PlainEssenceData then KLVEssenceDataParameters::SetEssenceElementKey()

		IAAFKLVEssenceDataParametersSP pParameters;
		checkResult( _data->QueryInterface(IID_IAAFKLVEssenceDataParameters, (void **)&pParameters) );

		aafUID_t Peek;
		hr = pParameters->GetEssenceElementKey( &Peek );

		// only do this is the file kind allows us to
		if( hr == AAFRESULT_SUCCESS )
		{
			if( true ) // was: if( EqualAUID( &_eek, &NULL_UID ) )
			{
				// if null passed in by caller, preset to what the file kind wants
				if( EqualAUID( &eek, &NULL_UID ) ) _eek = Peek;
				else _eek = eek;

				// make a GC bytes 12..16
				aafUInt32 GCeeIndex = ( ( (eeKind&0xff)<<8 | (eeCount&0xff) )<<8 | (eeType&0xff) )<<8 | (eeIndex&0xff);	

				// overwrite eek bytes 12..16
				_eek.Data2 = (aafUInt16)(GCeeIndex>>16)&0xffff;
				_eek.Data3 = (aafUInt16)(GCeeIndex)&0xffff;

				// set the EssenceElementKey
				checkResult( pParameters->SetEssenceElementKey( _eek ) );

				// set the PhysicalNum in the track of the FileSourceMob
				IAAFSourceMobSP pSourceMob;
				checkResult( _data->GetFileMob( &pSourceMob ) );

				IAAFMobSP pFileMob;
				checkResult( pSourceMob->QueryInterface( IID_IAAFMob,(void**)&pFileMob ) );

				IAAFMobSlotSP pSlot;
				checkResult( pFileMob->LookupSlot( 1, &pSlot ) );
				checkResult( pSlot->SetPhysicalNum( GCeeIndex ) );
			}
			else
			{
				// _eek has been initialized already
				// we ought to leave it alone
			}
		}
		else if( hr ==  AAFRESULT_OPERATION_NOT_PERMITTED )
			// must be a file kind where eek isn't important
			hr = AAFRESULT_CODEC_SEMANTIC_WARN;
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}
	// smart pointers clean themselves up

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Write (
    aafUInt32 bytes,
    aafDataBuffer_t  buffer,
    aafUInt32  *pBytesWritten)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;
  else if (NULL == buffer || NULL == pBytesWritten)
    return AAFRESULT_NULL_PARAM;

  return(_data->Write (bytes, buffer, pBytesWritten));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Read (aafUInt32  buflen,
        aafDataBuffer_t  buffer,
        aafUInt32 *  bytesRead)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;
  else if (NULL == buffer || NULL == bytesRead)
    return AAFRESULT_NULL_PARAM;


	return(_data->Read (buflen, buffer, bytesRead));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::Seek (aafPosition_t  byteOffset)
{
	HRESULT		hr = S_OK;
	aafBool		valid;

	if (NULL == _data)
		return AAFRESULT_NOT_INITIALIZED;

  	hr = IsPosValid (byteOffset, &valid);
	if(hr != S_OK)
		return(hr);

	if(!valid)
		return(AAFRESULT_BADSAMPLEOFFSET);

	return(_data->SetPosition(byteOffset));
}

// NOTE: If the GetPosition and SetPosition methods are changed 
// to use a aafUInt64 then the following code will have to check
// for sign wrapping...

HRESULT STDMETHODCALLTYPE    CAAFEssenceDataStream::SeekRelative (aafInt32  byteOffset)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

  HRESULT hr = S_OK;

  // Get the current position and then just add the given offset 
  // and attempt to set the new position.
  aafPosition_t pos;
  hr = GetPosition(&pos);
  if (SUCCEEDED(hr))
  {
    aafPosition_t newPos = pos + byteOffset;
    hr = Seek(newPos);
  }

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::IsPosValid (aafPosition_t  byteOffset,
        aafBool *  isValid)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;
  if (NULL == isValid)
    return E_INVALIDARG;
  
  *isValid = kAAFFalse;

  if (0 <= byteOffset)
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
      // Since we don't currently save the mode then
      // we don't know whether or not the next
      // file operation will be a read or a write
      // so we just return true.
      *isValid = kAAFTrue;
    }
  }

  return S_OK;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::GetPosition (aafPosition_t *  position)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

	return(_data->GetPosition (position));
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::GetLength (aafLength_t *  position)
{
  if (NULL == _data)
    return AAFRESULT_NOT_INITIALIZED;

	aafLength_t	result;
	HRESULT		status;

	status = _data->GetSize (&result);
	*position = result;
	return status;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::FlushCache ()
{
  return S_OK; //AAFRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceDataStream::SetCacheSize (aafUInt32  /* itsSize */)
{
  return S_OK; //AAFRESULT_NOT_IMPLEMENTED;
}


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFEssenceDataStream::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We support the IID_IAAFEssenceDataStream and the IID_IAAFEssenceDataStream2 interfaces 
    if (EQUAL_UID(riid,IID_IAAFEssenceDataStream)) 
    { 
        *ppvObj = (IAAFEssenceDataStream *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (EQUAL_UID(riid,IID_IAAFEssenceDataStream2)) 
    { 
        *ppvObj = (IAAFEssenceDataStream2 *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (EQUAL_UID(riid,IID_IAAFEssenceStream)) 
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
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFEssenceDataStream)
