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

#include "AxImplNullEssenceCodec.h"
#include "AxPluginFctry.h"
#include "CAxClassFactory.h"
#include "AxPluginRegistry.h"
#include "AxPluginUtil.h"
#include "AxEx.h"

#include <AAFCodecDefs.h>
#include <AAFStoredObjectIDs.h>
#include <AAFDataDefs.h>

#include <AAFResult.h>

#include <memory>

namespace {

//=---------------------------------------------------------------------=

// NullEssenceCodecRegister is a simple class that registers
// the CAxNullEssenceCodec in its constructor, and removes
// the component from the registry in its destructor.
// A static global instance of the class performs this work when
// the library is loaded, and unloaded.

// c5ca905c-540a-4903-ba70-83bb8a143c11
const CLSID CLSID_CAxNullEssenceCodec =
{ 0xc5ca905c, 0x540a, 0x4903, { 0xba, 0x70, 0x83, 0xbb, 0x8a, 0x14, 0x3c, 0x11 } };

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

//=---------------------------------------------------------------------=

//
// This information is required to initialize AxImplPlugin.
//

// 101eb379-0723-40f0-bdc9-777601e057f4
//const aafUID_t CAX_NULL_ESSENCE_CODECDEF_ID =
//{ 0x101eb379, 0x0723, 0x40f0, { 0xbd, 0xc9, 0x77, 0x76, 0x01, 0xe0, 0x57, 0xf4 } };

// The unique id that identifies this codec.  (Fill the same role as, for example,
// kAAFCDCICodec does in the CDCI codec.)
// ef279d53-3dcf-4ebe-b539-9797877161ab
const aafUID_t AX_NULL_ESSENCE_CODEC_ID =
{ 0xef279d53, 0x3dcf, 0x4ebe, { 0xb5, 0x39, 0x97, 0x97, 0x87, 0x71, 0x61, 0xab } };

// AAF Codec Manufacturer ID.
// 7dbd0a0b-f599-4f8b-aba3-f55ec34513ae
const aafUID_t CAX_AAF_CODEC_MANUFACTURER_ID =
{ 0x7dbd0a0b, 0xf599, 0x4f8b, {0xab, 0xa3, 0xf5, 0x5e, 0xc3, 0x45, 0x13, 0xae} };


} // End of anonymous name space.

//=---------------------------------------------------------------------=

CAxNullEssenceCodec::CAxNullEssenceCodec( IUnknown* pUnkOuter )
:  CAxUnknown( pUnkOuter ),
   CAxPlugin<AxImplPlugin>(
		std::auto_ptr<AxImplPlugin>( new AxImplPlugin ) ),
   CAxEssenceCodec<AxImplNullEssenceCodec>(
		std::auto_ptr<AxImplNullEssenceCodec>( new AxImplNullEssenceCodec ) )
{
	//
	// AxImplNullEssenceCodec init
	//

	// nothing to init for AxImplNullEssenceCodec


	//
	// AxImplPlugin init
	//

	// Some of the AxImplPlugin parameters come from AxImplNullEssenceCode.
	// For example, AxImpleNullEssenceCode know the type of essence descriptor
	// object it requires, hence, it is best to just ask it for the uid.
	// The codec name and description from AxImplEssenceCodec() - although this
	// is somewhat arbitrary.

	// Init the definition parameters vector required by
	// GetIndexedDefinitionID(), and GetIndexedDefinitionObject()
	AxImplPlugin::DefinitionParameters defParams(
		AX_NULL_ESSENCE_CODEC_ID,
		GetEssenceCodecImpl().GetDescriptorAUID(),
		GetEssenceCodecImpl().GetDataDefID(),
		GetEssenceCodecImpl().GetName(),
		GetEssenceCodecImpl().GetDesc() );

	GetPluginImpl().AddDefinitionParameters( defParams );

	AxImplPlugin::DescriptorParameters descParams( 
			 AX_NULL_ESSENCE_CODEC_ID,
			 GetEssenceCodecImpl().GetName(),
			 GetEssenceCodecImpl().GetDesc(),
			 AUID_AAFCodecDef,
			 L"0.0",
			 CAX_AAF_CODEC_MANUFACTURER_ID,
			 L"AAF Association Inc.",
			 true,   // isSoftwareOnly
			 false,  // isAccel,
			 false,  // supportsAuth
			 L"http://www.aafassociation.org" );

	GetPluginImpl().SetDescriptorParameters( descParams );
}

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
