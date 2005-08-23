//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

#ifdef _WIN32
// Disable VC6 
// warning C4660: template-class specialization 'AxArrayIterator<struct IAAFTypeDefFixedArray>' is already instantiated
// etc
#pragma warning( disable:4660 )
#endif

#include "AxIterator.h"

// Required for record iterator... probably better
// if record iterator is in separate file.
#include "AxMetaDef.h"

#include <AAFResult.h>
#include <AAFSmartPointer2.h>

//=---------------------------------------------------------------------=

template <class Type, class EnumeratorType, class AddrOfOperator>
AxIterator<Type, EnumeratorType, AddrOfOperator>::AxIterator()
:	_spEnumerator()
{}


template <class Type, class EnumeratorType, class AddrOfOperator>
AxIterator<Type, EnumeratorType, AddrOfOperator>::
AxIterator( const IAAFSmartPointer<EnumeratorType>& spEnumerator )
:	_spEnumerator( spEnumerator )
{}

template <class Type, class EnumeratorType, class AddrOfOperator>
AxIterator<Type, EnumeratorType, AddrOfOperator>::
AxIterator( const AxIterator<Type, EnumeratorType, AddrOfOperator>& other )
:	_spEnumerator( other._spEnumerator )
{}

template <class Type, class EnumeratorType, class AddrOfOperator>
AxIterator<Type, EnumeratorType, AddrOfOperator>::~AxIterator()
{}

template <class Type, class EnumeratorType, class AddrOfOperator>
AxIterator<Type, EnumeratorType, AddrOfOperator>& AxIterator<Type, EnumeratorType, AddrOfOperator>::
operator=( const AxIterator<Type, EnumeratorType, AddrOfOperator>& rhs )
{
	if ( this == &rhs ) {
		return *this;
	}

	_spEnumerator = rhs._spEnumerator;

	return *this;
}

template <class Type, class EnumeratorType, class AddrOfOperator>
void AxIterator<Type, EnumeratorType, AddrOfOperator>::Skip( aafUInt32 count )
{
	CHECK_HRESULT( _spEnumerator->Skip( count ) );
}

template <class Type, class EnumeratorType, class AddrOfOperator>
void AxIterator<Type, EnumeratorType, AddrOfOperator>::Reset()
{
	// Reset is documented to always succeed... but who knows, maybe
	// it will change some day, or maybe the documentation is wrong.

	CHECK_HRESULT( _spEnumerator->Reset() );
}


template <class Type, class EnumeratorType, class AddrOfOperator>
bool AxIterator<Type, EnumeratorType, AddrOfOperator>::NextOne( Type& ret )
{
	HRESULT hr;

	Type type;

	AddrOfOperator addrOp(&type);

	hr = _spEnumerator->NextOne( addrOp.GetAddrOf() );

	if ( hr == AAFRESULT_NO_MORE_OBJECTS ) {
	  return false;
	}

	CHECK_HRESULT( hr );

	ret = type;
	return true;
}

template <class Type, class EnumeratorType, class AddrOfOperator>
std::auto_ptr< std::vector< Type > > 
AxIterator<Type, EnumeratorType, AddrOfOperator>::Next( aafUInt32 count )
{
	aafUInt32 i;

	std::auto_ptr< std::vector< Type > >
		typeV( new std::vector< Type > );

	for( i = 0; i < count; i++ ) {
		Type tmp;
		if ( NextOne( tmp ) ) {
			typeV->push_back(tmp);
		}
		else {
			break;
		}
	}

	return typeV;
}

template <class Type, class EnumeratorType, class AddrOfOperator>
std::auto_ptr< AxIterator<Type, EnumeratorType, AddrOfOperator> > AxIterator<Type, EnumeratorType, AddrOfOperator>::Clone()
{
	std::auto_ptr< AxIterator<Type, EnumeratorType, AddrOfOperator> >
		pAxIterator( new AxIterator<Type, EnumeratorType, AddrOfOperator>( _spEnumerator ) ) ;

	return pAxIterator;
}

//=---------------------------------------------------------------------=

AxRecordIterator::AxRecordIterator( IAAFPropertyValueSP& spPropVal,
				    IAAFTypeDefRecordSP& spTypeDef )
:	_spPropVal( spPropVal ),
	_spTypeDef( spTypeDef ),
	_count( AxTypeDefRecord(spTypeDef).GetCount() ),
	_current( 0 )
{}

AxRecordIterator::~AxRecordIterator()
{}

bool AxRecordIterator::NextOne( std::pair<AxString, IAAFPropertyValueSP>& ret )
{
	if ( _current == _count ) {
		return false;
	}

	AxTypeDefRecord axTDR( _spTypeDef );

	ret.first = axTDR.GetMemberName( _current );
	ret.second = axTDR.GetValue( _spPropVal, _current );

	_current++;
	
	return true;
}

void AxRecordIterator::Reset()
{
	_current = 0;
}

std::auto_ptr<AxRecordIterator> AxRecordIterator::Clone()
{
	std::auto_ptr<AxRecordIterator> clone(
		new AxRecordIterator( _spPropVal, _spTypeDef ) );

	clone->_current = _current;

	return clone;
}

//=---------------------------------------------------------------------=


template <>
AxArrayIterator<IAAFTypeDefFixedArray>::AxArrayIterator( TypeDefSP spTypeDef,
							 IAAFPropertyValueSP spPropVal )
:	_spTypeDef( spTypeDef ),
	_spPropVal( spPropVal ),
	_current( 0 )
{
	CHECK_HRESULT( _spTypeDef->GetCount( &_count ) );
}


template <>
AxArrayIterator<IAAFTypeDefVariableArray>::AxArrayIterator( TypeDefSP spTypeDef,
							    IAAFPropertyValueSP spPropVal )
:	_spTypeDef( spTypeDef ),
	_spPropVal( spPropVal ),
	_current( 0 )
{
	CHECK_HRESULT( _spTypeDef->GetCount( spPropVal, &_count ) );
}

template <class TypeDef>
AxArrayIterator<TypeDef>::~AxArrayIterator()
{}

template <class TypeDef>
bool AxArrayIterator<TypeDef>::NextOne( IAAFPropertyValueSP& ret )
{
	if ( _current == _count ) {
		return false;
	}
	
	CHECK_HRESULT( _spTypeDef->GetElementValue( _spPropVal, _current, &ret ) );	

	_current++;

	return true;
}

template <class TypeDef>
void AxArrayIterator<TypeDef>::Reset()
{
	_current = 0;
}

template <class TypeDef>
std::auto_ptr<AxArrayIterator<TypeDef> > AxArrayIterator<TypeDef>::Clone()
{
	std::auto_ptr<AxArrayIterator> iter(
		new AxArrayIterator<TypeDef>( _spTypeDef, _spPropVal ) );

	iter->_current = _current;

	return iter;
}

//=---------------------------------------------------------------------=

//=---------------------------------------------------------------------=

template class AxIterator< IAAFSmartPointer2<IAAFProperty>,      IEnumAAFProperties, AddrOfSmartPointer2<IAAFProperty> >;
template class AxIterator< IAAFSmartPointer2<IAAFPropertyValue>, IEnumAAFPropertyValues, AddrOfSmartPointer2<IAAFPropertyValue> >;
template class AxIterator< IAAFSmartPointer2<IAAFMob>,           IEnumAAFMobs, AddrOfSmartPointer2<IAAFMob> >;
template class AxIterator< IAAFSmartPointer2<IAAFEssenceData>,   IEnumAAFEssenceData, AddrOfSmartPointer2<IAAFEssenceData> >;
template class AxIterator< IAAFSmartPointer2<IAAFMobSlot>,       IEnumAAFMobSlots, AddrOfSmartPointer2<IAAFMobSlot> >;
template class AxIterator< IAAFSmartPointer2<IAAFDataDef>,       IEnumAAFDataDefs, AddrOfSmartPointer2<IAAFDataDef> >;
template class AxIterator< IAAFSmartPointer2<IAAFOperationDef>,  IEnumAAFOperationDefs, AddrOfSmartPointer2<IAAFOperationDef> >;
template class AxIterator< IAAFSmartPointer2<IAAFParameterDef>,  IEnumAAFParameterDefs, AddrOfSmartPointer2<IAAFParameterDef> >;
template class AxIterator< aafUID_t,                             IEnumAAFCodecFlavours, AddrOfAafUID >;
template class AxIterator< aafUID_t,                             IEnumAAFLoadedPlugins, AddrOfAafUID >;
template class AxIterator< IAAFSmartPointer2<IAAFComponent>,     IEnumAAFComponents, AddrOfSmartPointer2<IAAFComponent> >;
template class AxIterator< IAAFSmartPointer2<IAAFSegment>,       IEnumAAFSegments, AddrOfSmartPointer2<IAAFSegment> >;
template class AxIterator< IAAFSmartPointer2<IAAFClassDef>,      IEnumAAFClassDefs, AddrOfSmartPointer2<IAAFClassDef> >;
template class AxIterator< IAAFSmartPointer2<IAAFTypeDef>,       IEnumAAFTypeDefs, AddrOfSmartPointer2<IAAFTypeDef> >;
template class AxIterator< IAAFSmartPointer2<IAAFPropertyDef>,   IEnumAAFPropertyDefs, AddrOfSmartPointer2<IAAFPropertyDef> >;
template class AxIterator< IAAFSmartPointer2<IAAFLocator>,       IEnumAAFLocators, AddrOfSmartPointer2<IAAFLocator> >;


template class AxArrayIterator< IAAFTypeDefFixedArray >;
template class AxArrayIterator< IAAFTypeDefVariableArray >;
