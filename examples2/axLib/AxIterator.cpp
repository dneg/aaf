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

#include "AxIterator.h"

// Required for record iterator... probably better
// if record iterator is in separate file.
#include "AxMetaDef.h"

#include <AAFResult.h>

//=---------------------------------------------------------------------=

template <class Type, class EnumeratorType>
AxIterator<Type, EnumeratorType>::AxIterator()
:	_spEnumerator()
{}


template <class Type, class EnumeratorType>
AxIterator<Type, EnumeratorType>::
AxIterator( const IAAFSmartPointer<EnumeratorType>& spEnumerator )
:	_spEnumerator( spEnumerator )
{}

template <class Type, class EnumeratorType>
AxIterator<Type, EnumeratorType>::
AxIterator( const AxIterator<Type, EnumeratorType>& other )
:	_spEnumerator( other._spEnumerator )
{}

template <class Type, class EnumeratorType>
AxIterator<Type, EnumeratorType>::~AxIterator()
{}

template <class Type, class EnumeratorType>
AxIterator<Type, EnumeratorType>& AxIterator<Type, EnumeratorType>::
operator=( const AxIterator<Type, EnumeratorType>& rhs )
{
	if ( this == &rhs ) {
		return *this;
	}

	_spEnumerator = rhs._spEnumerator;

	return *this;
}

template <class Type, class EnumeratorType>
void AxIterator<Type, EnumeratorType>::Skip( aafUInt32 count )
{
	CHECK_HRESULT( _spEnumerator->Skip( count ) );
}

template <class Type, class EnumeratorType>
void AxIterator<Type, EnumeratorType>::Reset()
{
	// Reset is documented to always succeed... but who knows, maybe
	// it will change some day, or maybe the documentation is wrong.

	CHECK_HRESULT( _spEnumerator->Reset() );
}


template <class Type, class EnumeratorType>
bool AxIterator<Type, EnumeratorType>::NextOne( Type& ret )
{
	HRESULT hr;

	Type type;

	hr = _spEnumerator->NextOne( &type );

	if ( hr == AAFRESULT_NO_MORE_OBJECTS ) {
	  return false;
	}

	CHECK_HRESULT( hr );

	ret = type;
	return true;
}

template <class Type, class EnumeratorType>
auto_ptr< vector< Type > > 
AxIterator<Type, EnumeratorType>::Next( aafUInt32 count )
{
	int i;

	auto_ptr< vector< Type > >
		typeV( new vector< Type > );

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

template <class Type, class EnumeratorType>
auto_ptr< AxIterator<Type, EnumeratorType> > AxIterator<Type, EnumeratorType>::Clone()
{
	auto_ptr< AxIterator<Type, EnumeratorType> >
		pAxIterator( new AxIterator<Type, EnumeratorType>( _spEnumerator ) ) ;

	return pAxIterator;
}

//=---------------------------------------------------------------------=

AxRecordIterator::AxRecordIterator( IAAFPropertyValueSP& spPropVal,
				    IAAFTypeDefRecordSP& spTypeDef )
:	_spPropVal( spPropVal ),
	_spTypeDef( spTypeDef ),
	_current( 0 ),
	_count( AxTypeDefRecord(spTypeDef).GetCount() )
{}

AxRecordIterator::~AxRecordIterator()
{}

bool AxRecordIterator::NextOne( pair<AxString, IAAFPropertyValueSP>& ret )
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

auto_ptr<AxRecordIterator> AxRecordIterator::Clone()
{
	auto_ptr<AxRecordIterator> clone(
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
auto_ptr<AxArrayIterator<TypeDef> > AxArrayIterator<TypeDef>::Clone()
{
	auto_ptr<AxArrayIterator> iter(
		new AxArrayIterator<TypeDef>( _spTypeDef, _spPropVal ) );

	iter->_current = _current;

	return iter;
}

//=---------------------------------------------------------------------=

template class AxIterator< IAAFSmartPointer<IAAFProperty>,      IEnumAAFProperties >;
template class AxIterator< IAAFSmartPointer<IAAFPropertyValue>, IEnumAAFPropertyValues >;
template class AxIterator< IAAFSmartPointer<IAAFMob>,           IEnumAAFMobs >;
template class AxIterator< IAAFSmartPointer<IAAFEssenceData>,   IEnumAAFEssenceData >;
template class AxIterator< IAAFSmartPointer<IAAFMobSlot>,       IEnumAAFMobSlots >;
template class AxIterator< IAAFSmartPointer<IAAFDataDef>,       IEnumAAFDataDefs >;
template class AxIterator< aafUID_t,                            IEnumAAFCodecFlavours >;
template class AxIterator< aafUID_t,                            IEnumAAFLoadedPlugins >;
template class AxIterator< IAAFSmartPointer<IAAFComponent>,		IEnumAAFComponents >;

template class AxArrayIterator< IAAFTypeDefFixedArray >;
template class AxArrayIterator< IAAFTypeDefVariableArray >;
