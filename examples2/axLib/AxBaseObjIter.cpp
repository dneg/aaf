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

#include "AxBaseObjIter.h"

#include "AxObject.h"
#include "AxProperty.h"
#include "AxPropertyValue.h"
#include "AxMetaDef.h"
#include "AxIterator.h"

// Get rid of this when recursin object code factored out.
#include "AxPropertyValue.h"

//=---------------------------------------------------------------------=

namespace {

class PropValToIter : public AxPropertyValueNoopPrtcl {
public:

	typedef pair<bool, auto_ptr< AxBaseObjIterPrtcl > > Pair;
	
	PropValToIter( Pair& result );

	virtual ~PropValToIter();

	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefStrongObjRefSP& );

	virtual void process( IAAFPropertyValueSP&,	IAAFTypeDefSetSP& );

	virtual void process( IAAFPropertyValueSP&,	IAAFTypeDefVariableArraySP& );

	virtual void process( IAAFPropertyValueSP&,	IAAFTypeDefRecordSP& );

	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefFixedArraySP& );

	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefIndirectSP& );

	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefOpaqueSP& );

	virtual void process( IAAFPropertyValueSP&, IAAFTypeDefRenameSP& );

private:

	inline Post( auto_ptr< AxBaseObjIterPrtcl > iter ) {
		_result.first = true;
		_result.second = iter;
	}
	
	Pair& _result;
};

PropValToIter::PropValToIter( Pair& result )
:	_result( result )
{}

PropValToIter::~PropValToIter()
{}

void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
			     IAAFTypeDefStrongObjRefSP& spIaafTypeDefStrongObjRef )
{
	AxTypeDefStrongObjRef axStrongObjRef( spIaafTypeDefStrongObjRef );

	IUnknownSP spIUnknown;
	spIUnknown = axStrongObjRef.GetObject( spIaafPropertyValue,
				  						   IID_IAAFObject );

	IAAFObjectSP spIaafObject;
	AxQueryInterface( spIUnknown, spIaafObject, IID_IAAFObject );

	AxObject axObj( spIaafObject );
	auto_ptr< AxBaseObjIterPrtcl > iter(
		new AxBaseSolitaryObjIter<AxObject>( spIaafObject ) );

	Post( iter );
}

void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
			     IAAFTypeDefSetSP& spIaafTypeDefSet )
{
	AxTypeDefSet axTypeDefSet( spIaafTypeDefSet );

	auto_ptr< AxBaseObjIterPrtcl > iter( 
		new AxBaseObjIter<AxPropertyValueIter, AxPropertyValue>(
			axTypeDefSet.GetElements( spIaafPropertyValue ) ) );

	Post( iter );
}

void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
			     IAAFTypeDefVariableArraySP& spIaafTypeDefVariableArray )
{
	auto_ptr<AxArrayIterator<IAAFTypeDefVariableArray> > axArrayIter(
		new AxArrayIterator<IAAFTypeDefVariableArray>
			(spIaafTypeDefVariableArray, spIaafPropertyValue) );

	auto_ptr<AxBaseObjIterPrtcl> iter(
		new AxBaseArrayObjIter<IAAFTypeDefVariableArray>( axArrayIter ) );

	Post( iter );
}

void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
			     IAAFTypeDefRecordSP& spIaafTypeDefRecord )
{
	// Records are a bit weird, the are a set of named attribute/value
	// pairs, but are not IAAFObjects which are also a set of named
	// attribute/value (i.e. property/value) pairs.  In order to 
	// process the individual record fields using this framework,
	// a special iterator is required.

	auto_ptr<AxRecordIterator> axRecordIter( 
		new AxRecordIterator( spIaafPropertyValue, spIaafTypeDefRecord ) );

	auto_ptr<AxBaseObjIterPrtcl> iter(
		new AxBaseRecordObjIter( axRecordIter ) );

	Post( iter );
}

void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
			     IAAFTypeDefFixedArraySP& spIaafTypeDefFixedArray )
{
	auto_ptr<AxArrayIterator<IAAFTypeDefFixedArray> > axArrayIter(
		new AxArrayIterator<IAAFTypeDefFixedArray>
			(spIaafTypeDefFixedArray, spIaafPropertyValue) );

	auto_ptr<AxBaseObjIterPrtcl> iter(
		new AxBaseArrayObjIter<IAAFTypeDefFixedArray>( axArrayIter ) );

	Post( iter );

}

void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
			     IAAFTypeDefIndirectSP& spIaafTypeDefIndirect )
{
	AxTypeDefIndirect axIndirect( spIaafTypeDefIndirect );

	AxPropertyValue axActualPropVal( axIndirect.GetActualValue(spIaafPropertyValue) );

	auto_ptr< AxBaseObjIterPrtcl > iter(
		new AxBaseSolitaryObjIter<AxPropertyValue>( axActualPropVal ) );

	Post( iter );
}

void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
			     IAAFTypeDefOpaqueSP& spIaafTypeDefOpaque )
{
	AxTypeDefOpaque axOpaque( spIaafTypeDefOpaque );
	
	AxPropertyValue axActualPropVal( axOpaque.GetActualValue( spIaafPropertyValue ) );

	auto_ptr< AxBaseObjIterPrtcl > iter(
		new AxBaseSolitaryObjIter<AxPropertyValue>( axActualPropVal ) );

	Post( iter );
}

void PropValToIter::process( IAAFPropertyValueSP& spIaafPropertyValue,
			     IAAFTypeDefRenameSP& spIaafTypeDefRename )
{
	AxTypeDefRename axTypeDefRename( spIaafTypeDefRename );
	AxPropertyValue axBasePropVal( axTypeDefRename.GetBaseValue( spIaafPropertyValue ) );

	auto_ptr< AxBaseObjIterPrtcl > iter(
		new AxBaseSolitaryObjIter<AxPropertyValue>( axBasePropVal ) );

	Post( iter );

}

} // end of namespace

//=---------------------------------------------------------------------=

AxBaseRecordObjIter::AxBaseRecordObjIter( auto_ptr<AxRecordIterator> axRecordIter )
:	_axRecordIter( axRecordIter )
{}

AxBaseRecordObjIter::~AxBaseRecordObjIter()
{}

AxBaseObjIterPrtcl::Pair AxBaseRecordObjIter::NextOne()
{
	AxBaseObjIterPrtcl::Pair retPair;

	retPair.first = false;

	AxRecordIterator::Pair nextRecordPair;

	if ( _axRecordIter->NextOne( nextRecordPair ) ) {
		auto_ptr<AxBaseObj>
			intrChgObj( new AxBaseObjAny<AxRecordIterator::Pair>( nextRecordPair ) );

		retPair.first  = true;
		retPair.second = intrChgObj;
	}

	return retPair;
}

void AxBaseRecordObjIter::Reset()
{
	_axRecordIter->Reset();
}

auto_ptr<AxBaseObjIterPrtcl> AxBaseRecordObjIter::Clone()
{
	auto_ptr<AxBaseObjIterPrtcl> clone( 
		new AxBaseRecordObjIter( _axRecordIter->Clone() ) );

	return clone;
}

//=---------------------------------------------------------------------=

template <class TypeDef>
AxBaseArrayObjIter<TypeDef>::
AxBaseArrayObjIter( auto_ptr<AxArrayIterator<TypeDef> > axArrayIter )
:	_axArrayIter( axArrayIter )
{}

template <class TypeDef>
AxBaseArrayObjIter<TypeDef>::~AxBaseArrayObjIter()
{}

template <class TypeDef>
AxBaseObjIterPrtcl::Pair AxBaseArrayObjIter<TypeDef>::NextOne()
{
	AxBaseObjIterPrtcl::Pair retPair;

	retPair.first = false;

	AxArrayIterator<TypeDef>::Pair next;

	next = _axArrayIter->NextOne();
	if ( next.first ) {
		auto_ptr<AxBaseObj>
			intrChgObj( new AxPropertyValue( next.second ) );

		retPair.first  = true;
		retPair.second = intrChgObj;
	}

	return retPair;
}

template <class TypeDef>
void AxBaseArrayObjIter<TypeDef>::Reset()
{
	_axArrayIter->Reset();
}

template <class TypeDef>
auto_ptr<AxBaseObjIterPrtcl> AxBaseArrayObjIter<TypeDef>::Clone()
{
	auto_ptr<AxBaseObjIterPrtcl> clone( 
		new AxBaseArrayObjIter( _axArrayIter->Clone() ) );

	return clone;
}
//=---------------------------------------------------------------------=

AxBaseObjIterPrtcl::~AxBaseObjIterPrtcl()
{}


inline void AxBaseObjRecIter::Push( auto_ptr< AxBaseObjIterPrtcl > iter )
{
	_deque.push_back( iter );
}

inline void AxBaseObjRecIter::Pop()
{
	_deque.pop_back();
}

inline const auto_ptr< AxBaseObjIterPrtcl >& AxBaseObjRecIter::Top()
{
	return _deque.back();
}

inline bool AxBaseObjRecIter::Empty()
{
	return _deque.empty();
}

inline int AxBaseObjRecIter::Size()
{
	return _deque.size();
}

AxBaseObjRecIter::AxBaseObjRecIter( auto_ptr< AxBaseObjIterPrtcl >  root )
:	_root( root )
{
	AxBaseObjRecIter::Push( _root->Clone() );
}


AxBaseObjRecIter::~AxBaseObjRecIter()
{}

AxBaseObjRecIter::Pair AxBaseObjRecIter::NextOne( int& level )
{
	if ( Empty() ) {
		return Pair(false,auto_ptr<AxBaseObj>());
	}

	const auto_ptr< AxBaseObjIterPrtcl >&iter = Top();
	Pair result = iter->NextOne();

	if ( !result.first ) {
		Pop();
		return NextOne( level );
	}

	level = GetLevel();

	// Peek at the pointer, use it, but don't take ownership!
	try {
		if ( dynamic_cast< AxObject* >( result.second.get() ) ) {

			AxObject& obj = dynamic_cast< AxObject& >( *result.second.get() );

			HandleObjectRecursion( obj );
		}
		else if ( dynamic_cast< AxProperty* >( result.second.get() ) ) {

			AxProperty& prop = dynamic_cast< AxProperty& >( *result.second.get() );

			HandlePropertyRecursion( prop );
		}	
		else if ( dynamic_cast< AxPropertyValue* >( result.second.get() ) ) {

			AxPropertyValue& propVal = dynamic_cast< AxPropertyValue& >( *result.second.get() );

			HandlePropertyValueRecursion( propVal );
		}                      
		else if ( dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>* >( result.second.get() ) ) {

			AxBaseObjAny<AxRecordIterator::Pair>& recPair =
				dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>& >( *result.second.get() );

			HandleRecordPropertyValueRecursion(	recPair.get() );
		
		}
	}
	catch ( const AxExHResult& ex ) {
		auto_ptr< AxBaseObjIterPrtcl > iter(
			new AxBaseSolitaryObjIter<AxBaseObjAny<AxExHResult> >(
				AxBaseObjAny<AxExHResult>( ex ) ) );
		Push( iter );
	}
	
	return result;
}

void AxBaseObjRecIter::PopStack()
{
	Pop();
}

int AxBaseObjRecIter::GetLevel()
{
	return Size();
}

void AxBaseObjRecIter::HandleObjectRecursion( AxObject& obj )
{
	auto_ptr< AxBaseObjIterPrtcl > iter( 
		new AxBaseObjIter<AxPropertyIter, AxProperty>( obj.CreatePropertyIter() ) );

	Push( iter );
}

void AxBaseObjRecIter::HandlePropertyRecursion( AxProperty& prop )
{
	AxPropertyValue propVal( prop.GetValue() );
		
	auto_ptr< AxBaseObjIterPrtcl > iter(
		new AxBaseSolitaryObjIter<AxPropertyValue>( propVal ) );

	Push( iter );
}

void AxBaseObjRecIter::HandlePropertyValueRecursion( AxPropertyValue& propVal )
{
	PropValToIter::Pair result;
	
	PropValToIter valuePrtcl( result );

	propVal.Process( valuePrtcl );

	if ( result.first ) {
		Push( result.second );
	}
}

void AxBaseObjRecIter::HandleRecordPropertyValueRecursion( AxRecordIterator::Pair& recPair )
{
	auto_ptr< AxBaseObjIterPrtcl > iter(
		new AxBaseSolitaryObjIter<AxPropertyValue>( AxPropertyValue(recPair.second) ) );

	Push( iter );
}

//=---------------------------------------------------------------------=
