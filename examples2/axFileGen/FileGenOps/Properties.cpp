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

#include <axFileGen.h>

#include <AxDictionary.h>
#include <AxObject.h>
#include <AxMetaDef.h>

#include <AAFTypeDefUIDs.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  SetInt32Property,           
  L"SetInt32Property",
  L"Set a 32 bit integer property value on any AAF object.",
  L"FileName ObjectName prop_name integer_value",
  L"If the property does not exist, it is automatically created by the SDK.",
  5,
  5 )

SetInt32Property::~SetInt32Property()
{}

aafUID_t _auid = { 0x6840c4c2, 0x02fa, 0x41fb,
		    { 0xb6, 0xfc, 0xf9, 0x5b, 0x46, 0xf3, 0x4b, 0x19 } };

void SetInt32Property::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString objName  = argv[2];
	AxString propName = argv[3];
	AxString val      = argv[4];

    AxDictionary axDict( DictionaryFromFileOp( fileName ) );

	// FIXME - There is no map for type id's in axLib (as exists for class ids).
	// Is one needed?  This is first time I found a need to use them.
	
	IAAFTypeDefSP spTypeDef = axDict.LookupTypeDef( kAAFTypeID_UInt32 );

	IAAFObjectSP spObj;
	GetInstance( objName ).GetCOM( spObj );
	
	IAAFClassDefSP spClassDef;
	CHECK_HRESULT( spObj->GetDefinition( &spClassDef ) );
		
	IAAFPropertyDefSP spPropDef;
	CHECK_HRESULT( spClassDef->RegisterOptionalPropertyDef( 
						_auid, propName.c_str(), spTypeDef, &spPropDef ) );


	IAAFTypeDefIntSP spTypeDefInt;
	CHECK_HRESULT( spTypeDef->QueryInterface( IID_IAAFTypeDefInt,
											  reinterpret_cast<void**>(&spTypeDefInt) ) );

	IAAFPropertyValueSP spPropVal;
	aafInt32 int32Val = AxStringUtil::strtol( val );
	CHECK_HRESULT( spTypeDefInt->CreateValue( reinterpret_cast<aafMemPtr_t>(&int32Val),
											  sizeof(int32Val), &spPropVal ) );

	CHECK_HRESULT( spObj->SetPropertyValue( spPropDef, spPropVal ) );
}

} // end of namespace
