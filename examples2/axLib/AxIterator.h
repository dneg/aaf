#ifndef __AxIterator_h__
#define __AxIterator_h__

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


#include "AxTypes.h"
#include "AxSmartPointer.h"

#include <AAFTypes.h>

#include <iosfwd>
#include <vector>
#include <memory>

using namespace std;

// This template will work for all IAAFEnum* enumerators
// that have the following methods:
// NextOne(), Next(int), Reset(), Skip(), Clone()

template <class Type, class EnumeratorType>
class AxIterator {
public:

	AxIterator();
	AxIterator( const AxIterator<Type, EnumeratorType>& other );
	AxIterator( const IAAFSmartPointer<EnumeratorType >& spEnumerator );

	~AxIterator();

	AxIterator& operator=( const AxIterator<Type, EnumeratorType>& rhs );

	bool NextOne( Type& );

	auto_ptr< vector< Type > > Next( aafUInt32 count );

	void Reset();

	void Skip( aafUInt32 count );

	auto_ptr< AxIterator<Type, EnumeratorType> > Clone();	

private:

	IAAFSmartPointer< EnumeratorType > _spEnumerator;
};

typedef AxIterator< IAAFSmartPointer<IAAFProperty>,      IEnumAAFProperties >	  AxPropertyIter;
typedef AxIterator< IAAFSmartPointer<IAAFPropertyValue>, IEnumAAFPropertyValues > AxPropertyValueIter;
typedef AxIterator< IAAFSmartPointer<IAAFMob>,           IEnumAAFMobs >		  AxMobIter;
typedef AxIterator< IAAFSmartPointer<IAAFEssenceData>,   IEnumAAFEssenceData >	  AxEssenceIter;
typedef AxIterator< IAAFSmartPointer<IAAFMobSlot>,       IEnumAAFMobSlots >	  AxMobSlotIter;
typedef AxIterator< IAAFSmartPointer<IAAFDataDef>,       IEnumAAFDataDefs >       AxDataDefsIter;
typedef AxIterator< aafUID_t,                            IEnumAAFCodecFlavours >  AxCodecFlavoursIter;
typedef AxIterator< aafUID_t,                            IEnumAAFLoadedPlugins >  AxLoadedPluginsIter;

// Records contain a set of named values that must be iterated over as well,
// but don't have any sort of native iterator.  This wrapper presents
// an iterator like iterface for records.

// ... probably better if record iterator is in separate file.

class AxRecordIterator {
public:
	typedef pair<AxString, IAAFPropertyValueSP> Pair;
	
	AxRecordIterator( IAAFPropertyValueSP& spPropVal,
					  IAAFTypeDefRecordSP& spTypeDef );
	~AxRecordIterator();

	bool NextOne( Pair& ret );

	void Reset();

	auto_ptr<AxRecordIterator> Clone();

private:
	AxRecordIterator();
	AxRecordIterator( const AxRecordIterator& );
	AxRecordIterator& operator=( const AxRecordIterator& );
	
	IAAFPropertyValueSP _spPropVal;
	IAAFTypeDefRecordSP _spTypeDef;

	aafUInt32 _count;
	int _current;
};


// Iterator for fixed and variable arrays. Works around lack of 
// implementation (AAFRESULT_NOT_IN_CURRENT_VERSION) of the 
// GetElement() COM interface for arrays.

// Type is indented to be IAAFTypeDefFixedArray or IAAFTypeDefVariableArray

template <class TypeDef>
class AxArrayIterator {
public:
	typedef IAAFSmartPointer<TypeDef> TypeDefSP;

	AxArrayIterator( TypeDefSP spTypeDef, IAAFPropertyValueSP spPropVal );

	~AxArrayIterator();

	bool NextOne( IAAFPropertyValueSP&  );

	void Reset();

	auto_ptr<AxArrayIterator> Clone();
	
private:

	TypeDefSP _spTypeDef;
	IAAFPropertyValueSP _spPropVal;
	aafUInt32 _count;
	int _current;
};

typedef AxArrayIterator<IAAFTypeDefFixedArray> AxFixedArrayIterator;
typedef AxArrayIterator<IAAFTypeDefVariableArray> AxVariableArrayIterator;

#endif
