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

#include "AxPropertyValueDump.h"

#include "AxProperty.h"
#include "AxMetaDef.h"
#include "AxUtil.h"

#include <iomanip.h>

// Fixme - This should be above thie iomanip.h include, but if I put
// it there I get many "ambiguos symbol" errors.
#include "AxObject.h"

namespace {

	AxString sepNothing( L"" );

};

//=---------------------------------------------------------------------=

AxPropertyValueDump::AxPropertyValueDump( std::wostream& os,
					  bool dumpValueType,
					  bool dumpValue,
					  const AxString& sep )
:	_os( os ),
	_dumpValueType( dumpValueType ),
	_dumpValue( dumpValue ),
	_sep( sep )
{
	if ( !_dumpValue ) {
		_s = sepNothing;
	}
	else {
		_s = _sep;
	}
}

AxPropertyValueDump::~AxPropertyValueDump()
{}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::SetDumpValue( bool flag )
{
	_dumpValue = flag;
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::SetDumpValueType( bool flag )
{
	_dumpValueType = flag;
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP&, IAAFTypeDefCharacterSP& )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefCharacter" << _s;
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spPropVal,
				   IAAFTypeDefIndirectSP& spTypeDefIndirect )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefIndirect" << _s;

	if ( _dumpValue ) {
		AxTypeDefIndirect axIndirect( spTypeDefIndirect );
		AxTypeDef axActualTypeDef( axIndirect.GetActualType(spPropVal) );
		
		_os << L"of type ";
		_os << AxTypeCatMap::getInstance().getStr( axActualTypeDef.GetTypeCategory() );
	}
}

//=---------------------------------------------------------------------=

// FIXME - This is a bit messy, and the same switch will be repeated
// where ever integers are processed.  Could be refactored using
// an integer processing protocol similar to AxPropertyValuePrtcl.

// FIXME - What is a good way to specify formating options!

// FIXME - NT is outputing unsigned integers with a sign...?

#define CASE( T ) \
case sizeof( aaf##T ) : \
{ \
	aaf##T i; \
    axTypeDef.GetInteger( spPropVal, &i ); \
	using namespace std; \
    _os << L"(" << AxStringUtil::mbtowc( #T ) << L") " << dec << i << L" 0x" << hex << i << dec; \
	break; \
}

// CASE64 overccomes VC6 compile error:
// "error C2593: 'operator <<' is ambiguous"

#define CASE64( T, T2 ) \
case sizeof( aaf##T ) : \
{ \
	aaf##T i; \
	axTypeDef.GetInteger( spPropVal, &i ); \
	using namespace std; \
	_os << L"(" << AxStringUtil::mbtowc( #T ) << L") " << dec << (T2)i << L" 0x"  << hex << (T2)i << dec; \
	break; \
}

void AxPropertyValueDump::process( IAAFPropertyValueSP& spPropVal,
				   IAAFTypeDefIntSP& spTypeDef )
{
	
	if ( _dumpValueType )
		_os << L"IAAFTypeDefInt" << _s;

	bool fault = false;
	AxTypeDefInt axTypeDef( spTypeDef );

	if ( _dumpValue ) {

		if ( axTypeDef.IsSigned() ) {
			switch ( axTypeDef.GetSize() ) {
				CASE( Int8 )
				CASE( Int16 )
				CASE( Int32 )
				CASE64( Int64, long int )
				default:
					fault = true;
			}
		}
		else {
			switch ( axTypeDef.GetSize() ) {
				CASE( UInt8 )
				CASE( UInt16 )
				CASE( UInt32 )
				CASE64( UInt64, unsigned long int )
				default:
					fault = true;
			}
		}

		if (fault) {
			throw AxEx( L"unexpected integer size" );
		}
	}
}

#undef CASE

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spPropVal,
				   IAAFTypeDefRenameSP& spTypeDef )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefRename" << _s;

	if ( _dumpValue ) {
		AxTypeDefRename axTypeDefRename( spTypeDef );
		AxTypeDef axBaseTypeDef( axTypeDefRename.GetBaseType() );
		
		_os << L"of type ";
		_os << AxTypeCatMap::getInstance().getStr( axBaseTypeDef.GetTypeCategory() );
	}
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP&, IAAFTypeDefEnumSP& )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefEnum" << _s;
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP&, IAAFTypeDefExtEnumSP& )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefExtEnum" << _s;
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP&,
				   IAAFTypeDefFixedArraySP& spTypeDefFixedArray )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefFixedArray" << _s;

	if ( _dumpValue ) {
		AxTypeDefFixedArray axFixedArray( spTypeDefFixedArray );
		AxTypeDef axTypeDefOfArray( axFixedArray.GetType() ); 
		aafUInt32 count = axFixedArray.GetCount();

		_os << count << L" element";

		if ( count != 1 ) {
				_os << L"s";
		}

		_os << L" of type ";
		_os << AxTypeCatMap::getInstance().getStr( axTypeDefOfArray.GetTypeCategory() );
	}

}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spPropVal,
								   IAAFTypeDefRecordSP& spTDR )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefRecord" << _s;

	if ( _dumpValue ) {
		AxTypeDefRecord axTDR( spTDR );

		_os << axTDR.GetCount() << L" members";
	}
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP&, IAAFTypeDefSetSP& )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefSet" << _s;
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spIaafPropertyValue,
				   IAAFTypeDefStreamSP& spIaafTypeDefStream )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefStream" << _s;

	if ( _dumpValue ) {
		AxTypeDefStream axTypeDefStream( spIaafTypeDefStream );

		_os << static_cast<long>(axTypeDefStream.GetSize( spIaafPropertyValue ));
		_os << L" bytes";
	}
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spPropVal, IAAFTypeDefStringSP& spTypeDef )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefString" << _s;

	AxTypeDefString axTypeDefString( spTypeDef );
	
	if ( _dumpValue ) {
		_os << axTypeDefString.GetElements( spPropVal );
	}
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spPropVal,
				   IAAFTypeDefStrongObjRefSP& spTypeDef )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefStrongObjRef" << _s;

	if ( _dumpValue ) {

		AxTypeDefStrongObjRef axStrongObjRef( spTypeDef );

		AxObject axObj( AxQueryInterface<IUnknown,IAAFObject>(
			axStrongObjRef.GetObject( spPropVal, IID_IAAFObject ),
			IID_IAAFObject ) );
		
		_os << L" to object of class " << *axObj.GetClassName();
	}
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spPropVal,
				   IAAFTypeDefWeakObjRefSP& spTypeDef )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefWeakObjRef" << _s;

	if ( _dumpValue ) {

		AxTypeDefWeakObjRef axWeakObjRef( spTypeDef );

		AxObject axObj( AxQueryInterface<IUnknown,IAAFObject>(
			axWeakObjRef.GetObject( spPropVal, IID_IAAFObject ),
			IID_IAAFObject ) );
		
		_os << L"to object of class " << *axObj.GetClassName() << L" named \"";

		// Pluck out the value of the Name property.
		AxPropertyIter axPropIter( axObj.CreatePropertyIter() );
		AxPropertyIter::Pair nextProp;
		for (nextProp = axPropIter.NextOne();
			 nextProp.first;
			 nextProp = axPropIter.NextOne()) {
			AxProperty axProp ( nextProp.second );
			if ( L"Name" == *axProp.GetName() ) {
				AxPropertyValue axPropVal( axProp.GetValue() );
				AxPropertyValueDump dump( _os, false, true, _s );
				axPropVal.Process( dump );
			}
		}

		_os << L"\"";
	}
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP&, IAAFTypeDefObjectRefSP& )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefObjectRef" << _s;
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spPropVal,
				   IAAFTypeDefOpaqueSP& spTypeDefOpaque )
{
	if ( _dumpValueType )
		_os << L"IAAFTypeDefOpaque" << _s;

	if ( _dumpValue ) {
		AxTypeDefOpaque axOpaque( spTypeDefOpaque );

		aafUID_t typeUid = axOpaque.GetActualTypeID( spPropVal );
		
		_os << L"uid " << typeUid;

		AxTypeDef axActualTypeDef( axOpaque.GetActualType(spPropVal) );
		_os << L" of type ";
		_os << AxTypeCatMap::getInstance().getStr( axActualTypeDef.GetTypeCategory() );
	}
}

//=---------------------------------------------------------------------=

void AxPropertyValueDump::process( IAAFPropertyValueSP& spVal, IAAFTypeDefVariableArraySP& spDef )
{
	
	if ( _dumpValueType ) {
		_os << L"IAAFTypeDefVariableArray" << _s;
	}

	if ( _dumpValue ) {
		AxTypeDefVariableArray axVarArray( spDef );
		AxTypeDef axTypeDefOfArray( axVarArray.GetType() ); 

		_os << axVarArray.GetCount(spVal) << L" element";

		if ( axVarArray.GetCount(spVal) != 1 ) {
			_os << L"s";
		}

		_os << L" of type ";
		_os << AxTypeCatMap::getInstance().getStr( axTypeDefOfArray.GetTypeCategory() );
	}
}

//=---------------------------------------------------------------------=
