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
//=---------------------------------------------------------------------=

#include "CAxNullEssenceCodec.h"

#include "AxPluginRegistry.h"
#include "AxPluginFctry.h"
#include "CAxClassFactory.h"
#include "AxPluginUtil.h"

#include <AAFStoredObjectIDs.h>
#include <AAFCodecDefs.h>
#include <AAFDataDefs.h>
#include <AAFResult.h>
#include <AAFSmartPointer.h>

#include <iostream>
#include <memory>

#define CHECK_HRESULT( exp ) { HRESULT hr = (exp); if ( S_OK != hr ) return hr; }

// c5ca905c-540a-4903-ba70-83bb8a143c11
const CLSID CLSID_CAxNullEssenceCodec =
{ 0xc5ca905c, 0x540a, 0x4903, { 0xba, 0x70, 0x83, 0xbb, 0x8a, 0x14, 0x3c, 0x11 } };

// 101eb379-0723-40f0-bdc9-777601e057f4
const aafUID_t CAX_NULL_ESSENCE_CODECDEF_ID =
{ 0x101eb379, 0x0723, 0x40f0, { 0xbd, 0xc9, 0x77, 0x76, 0x01, 0xe0, 0x57, 0xf4 } };

// ef279d53-3dcf-4ebe-b539-9797877161ab
const aafUID_t kAAFCodecNull =
{ 0xef279d53, 0x3dcf, 0x4ebe, { 0xb5, 0x39, 0x97, 0x97, 0x87, 0x71, 0x61, 0xab } };

#if 0
#define TRACE std::cout << __FILE__ << ":" << __LINE__ << std::endl;
#else
#define TRACE
#endif

//=---------------------------------------------------------------------=

namespace {

// NullEssenceCodecRegister is a simple class that registers
// the CAxNullEssenceCodec  in its constructor, and removes
// the component from the registry in its destructor.
// A static global instance of the class performs this work when
// the library is loaded, and unloaded.

class NullEssenceCodecRegister
{
public:

	NullEssenceCodecRegister()
	{
		std::auto_ptr<AxPluginFctryPrtcl> nullEssenceCodecFctry(
			new AxPluginFctry<CAxNullEssenceCodec>( CLSID_CAxNullEssenceCodec ) );

		AxPluginRegistry::GetInstance().AddFactory( nullEssenceCodecFctry );
	}

	~NullEssenceCodecRegister()
	{
		AxPluginRegistry::GetInstance().RemoveFactory( CLSID_CAxNullEssenceCodec );
	}

};

NullEssenceCodecRegister nullEssenceCodecRegister;

} // End of anonymous name space.

//=---------------------------------------------------------------------=

CAxNullEssenceCodec::CAxNullEssenceCodec( IUnknown* pUnkOuter )
:  CAxUnknown( pUnkOuter ),
	_numSupportedDefinitions(1),
	_numFlavours(1),
	_codecDefAUID(kAAFCodecNull),
	_codecName( L"NullCodec" ),
	_codecDesc( L"Handles null data." )
{}

CAxNullEssenceCodec::~CAxNullEssenceCodec()
{}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::NondelegatingQueryInterface(const IID& iid, void** ppv)
{
	if ( IID_IAAFPlugin == iid ) {
		return FinishQI( static_cast<IAAFPlugin*>(this), ppv );
	}

	else if ( IID_IAAFEssenceCodec == iid ) {
		return FinishQI( static_cast<IAAFEssenceCodec*>(this), ppv );
	}

	else {
		return CAxUnknown::NondelegatingQueryInterface( iid, ppv );
	}
}

//
//  IAAFPlugin methods
//

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::CountDefinitions(
		aafUInt32 *pDefCount )
{
	TRACE

	*pDefCount = _numSupportedDefinitions;
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetIndexedDefinitionID(
		aafUInt32 index, aafUID_t *result )
{
	TRACE

	if ( !result ) {
		return AAFRESULT_NULL_PARAM;
	}
	
	// For the moment, hard coded support for one codec definition.
	if ( index != 0 ) {
		return AAFRESULT_BADINDEX;
	}

	*result = _codecDefAUID;
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetPluginDescriptorID(
		aafUID_t *result )
{
	TRACE
	return !S_OK;
}


HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetIndexedDefinitionObject(
		aafUInt32 index,
		IAAFDictionary *dict, 
		IAAFDefObject **def )
{
	TRACE

	// Try block becaue the smart pointer code could through an exception.

	try {
	
		if ( !dict ) {
			return AAFRESULT_NULL_PARAM;
		}

		if ( !def ) {
			return AAFRESULT_NULL_PARAM;
		}
	
		// For the moment, hard coded support for on codec definition.
		if ( _numSupportedDefinitions != 1 ||
			index != 0 ) {
			return AAFRESULT_BADINDEX;
		}

		// Question? The axLib wrappers would be useful here.  Do we
		// want to drag those into a codec????
		IAAFSmartPointer<IUnknown> unknown;
		CHECK_HRESULT( dict->CreateInstance( AUID_AAFCodecDef, IID_IUnknown, &unknown ) );

		IAAFSmartPointer<IAAFCodecDef> codecDef;
		CHECK_HRESULT( unknown->QueryInterface( IID_IAAFCodecDef,
										        reinterpret_cast<void**>(&codecDef) ) );

		CHECK_HRESULT( codecDef->Initialize( _codecDefAUID,
											 _codecName,
											 _codecDesc ) );


		IAAFSmartPointer<IAAFClassDef> fileClass;
		CHECK_HRESULT( dict->LookupClassDef( AUID_AAFRGBADescriptor, &fileClass ) );

		CHECK_HRESULT( codecDef->SetFileDescriptorClass( fileClass ) );
	
		// Add an essence kind.
		IAAFSmartPointer<IAAFDataDef> dataDef;
		CHECK_HRESULT( dict->LookupDataDef( DDEF_Picture, &dataDef ) );
		CHECK_HRESULT( codecDef-> AddEssenceKind( dataDef ) );

		// Finally get a IAAFDefObject pointer to return to the caller.
		CHECK_HRESULT( codecDef->QueryInterface( IID_IAAFDefObject,
												 reinterpret_cast<void**>(def) ) );

	}

	catch( ... ) {

		return AAFRESULT_UNEXPECTED_EXCEPTION;

	}
	
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::CreateDescriptor(
		IAAFDictionary *dict,
		IAAFPluginDef **desc )
{
	TRACE

	try {

		if ( !dict ) {
			return AAFRESULT_NULL_PARAM;
		}

		if ( !desc ) {
			return AAFRESULT_NULL_PARAM;
		}

		IAAFSmartPointer<IUnknown> unknown;
		CHECK_HRESULT( dict->CreateInstance( AUID_AAFPluginDef, IID_IUnknown, &unknown ) );

		IAAFSmartPointer<IAAFPluginDef> pluginDef;
		CHECK_HRESULT( unknown->QueryInterface( IID_IAAFPluginDef,
												reinterpret_cast<void**>(&pluginDef) ) );

		CHECK_HRESULT( pluginDef->Initialize( CAX_NULL_ESSENCE_CODECDEF_ID,
											  _codecName,
											  _codecDesc ) );
		
		// See cdci codec for many other things that can be set in plugin def.

		// The easiest way to get a raw pointer from the smart pointer is to
		// perform a query interface.
		CHECK_HRESULT( pluginDef->QueryInterface( IID_IAAFPluginDef,
												  reinterpret_cast<void**>(desc) ) );
	}

	catch ( ... ) {

			return AAFRESULT_UNEXPECTED_EXCEPTION;
	}
	
	return S_OK;
}

//
//  IAAFEssenceCodec methods
//

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::SetEssenceAccess(
		IAAFEssenceAccess * pEssenceAccess )
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::CountFlavours(
		aafUInt32*  pCount )
{
	TRACE

	if ( !pCount ) {
		return AAFRESULT_NULL_PARAM;
	}

	*pCount = _numFlavours;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetIndexedFlavourID(
		aafUInt32  index,
		aafUID_t *  pVariant )
{
	TRACE

	if ( !pVariant ) {
		return AAFRESULT_NULL_PARAM;
	}
	
	// For the moment, hard coded support for one codec definition.
	if ( index != 0 ) {
		return AAFRESULT_BADINDEX;
	}

	*pVariant = kAAFNilCodecFlavour;

	
	return !S_OK;
}
 
HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::CountDataDefinitions(
		aafUInt32*  pCount )
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetIndexedDataDefinition(
		aafUInt32  index,
		aafUID_t *  pDataDefID )
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetMaxCodecDisplayNameLength(
		aafUInt32*  pBufSize )
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetCodecDisplayName(
		aafUID_constref  flavour,
		aafCharacter *  pName,
		aafUInt32  bufSize )
{
	TRACE
	return !S_OK;
}
	
HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::CountChannels(
		IAAFSourceMob * fileMob,
		aafUID_constref  essenceKind,
		IAAFEssenceStream * stream,
		aafUInt16 *  pNumChannels )
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetSelectInfo(
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafSelectInfo_t *  pSelectInfo )
{
	TRACE
	return !S_OK;
}


HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::ValidateEssence(
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafCheckVerbose_t  verbose,
		aafCheckWarnings_t  outputWarnings,
		aafUInt32  bufSize,
		aafCharacter *  pErrorText,
		aafUInt32*  pBytesRead)
{
	TRACE
	return !S_OK;
}


HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::Create(
		IAAFSourceMob * fileMob,
		aafUID_constref  flavour,
		aafUID_constref  essenceKind,
		aafRational_constref  sampleRate,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::Open(
		IAAFSourceMob * fileMob,
		aafMediaOpenMode_t  openMode,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable)
{
	TRACE
	return !S_OK;
}
	
HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::CountSamples(
		aafUID_constref  essenceKind,
		aafLength_t *  pNumSamples)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::WriteSamples(
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesWritten,
		aafUInt32 *  bytesWritten)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::ReadSamples(
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesRead,
		aafUInt32 *  bytesRead)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::Seek(
		aafPosition_t  sampleFrame)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::CompleteWrite(
		IAAFSourceMob * pFileMob)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::CreateDescriptorFromStream(
		IAAFEssenceStream * pStream,
		IAAFSourceMob * pSourceMob)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetCurrentEssenceStream(
		IAAFEssenceStream ** ppStream)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::PutEssenceFormat(
		IAAFEssenceFormat * pFormat)
{
	TRACE
	return !S_OK;
}
	
HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetEssenceFormat(
		IAAFEssenceFormat * pFormatTemplate,
		IAAFEssenceFormat ** ppNewFormat)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetDefaultEssenceFormat(
		IAAFEssenceFormat ** ppNewFormat)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetEssenceDescriptorID(
		aafUID_t *  pDescriptorID)
{
	TRACE

	if ( !pDescriptorID ) {
		return AAFRESULT_NULL_PARAM;
	}

	*pDescriptorID = AUID_AAFRGBADescriptor;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetEssenceDataID(
		aafUID_t *  pEssenceDataID)
{
	TRACE
	return !S_OK;
}
	
HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetIndexedSampleSize(
		aafUID_constref  essenceDefID,
		aafPosition_t  sampleOffset,
		aafLength_t *  pLength)
{
	TRACE
	return !S_OK;
}

HRESULT STDMETHODCALLTYPE CAxNullEssenceCodec::GetLargestSampleSize(
		aafUID_constref  essenceDefID,
		aafLength_t *  pLength)
{
	TRACE
	return !S_OK;
}
