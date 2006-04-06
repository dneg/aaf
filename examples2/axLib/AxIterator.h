#ifndef __AxIterator_h__
#define __AxIterator_h__

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

#include "AxTypes.h"
#include "AxSmartPointer.h"

#include <AAFTypes.h>
#include "AAFSmartPointer2.h"

#include <iosfwd>
#include <vector>
#include <memory>


template<class Type>
class AddrOfSmartPointer2
{
public:
  AddrOfSmartPointer2( IAAFSmartPointer2<Type>* sp )
    : _sp( sp )
  {}

  Type** GetAddrOf() const
  {
    return _sp->GetAddrOf();
  }

private:
  IAAFSmartPointer2<Type>* _sp;
};

class AddrOfAafUID
{
public:
  AddrOfAafUID( aafUID_t* p )
    : _p( p )
  {}

  aafUID_t* GetAddrOf() 
  {
    return _p;
  }

private:
  aafUID_t* _p;
};

// This template will work for all IAAFEnum* enumerators
// that have the following methods:
// NextOne(), Next(int), Reset(), Skip(), Clone()

template <class Type, class EnumeratorType, class AddrOfOperator>
class AxIterator {
public:

	AxIterator();
	AxIterator( const AxIterator<Type, EnumeratorType, AddrOfOperator>& other );
	AxIterator( const IAAFSmartPointer<EnumeratorType>& spEnumerator );

	~AxIterator();

	AxIterator& operator=( const AxIterator<Type, EnumeratorType, AddrOfOperator>& rhs );

	bool NextOne( Type& );

	std::auto_ptr< std::vector< Type > > Next( aafUInt32 count );

	void Reset();

	void Skip( aafUInt32 count );

	std::auto_ptr< AxIterator<Type, EnumeratorType, AddrOfOperator> > Clone();	

private:

	IAAFSmartPointer< EnumeratorType > _spEnumerator;
};

typedef AxIterator< IAAFSmartPointer2<IAAFProperty>,                IEnumAAFProperties,         AddrOfSmartPointer2<IAAFProperty> >
        AxPropertyIter;

typedef AxIterator< IAAFSmartPointer2<IAAFPropertyValue>,           IEnumAAFPropertyValues,     AddrOfSmartPointer2<IAAFPropertyValue> >
        AxPropertyValueIter;

typedef AxIterator< IAAFSmartPointer2<IAAFMob>,                     IEnumAAFMobs,               AddrOfSmartPointer2<IAAFMob> >
        AxMobIter;

typedef AxIterator< IAAFSmartPointer2<IAAFEssenceData>,             IEnumAAFEssenceData,        AddrOfSmartPointer2<IAAFEssenceData> >
        AxEssenceIter;

typedef AxIterator< IAAFSmartPointer2<IAAFMobSlot>,                 IEnumAAFMobSlots,           AddrOfSmartPointer2<IAAFMobSlot> >
        AxMobSlotIter;

typedef AxIterator< IAAFSmartPointer2<IAAFDataDef>,                 IEnumAAFDataDefs,           AddrOfSmartPointer2<IAAFDataDef> >
        AxDataDefsIter;

typedef AxIterator< IAAFSmartPointer2<IAAFOperationDef>,            IEnumAAFOperationDefs,      AddrOfSmartPointer2<IAAFOperationDef> >
        AxOperationDefIter;

typedef AxIterator< IAAFSmartPointer2<IAAFParameterDef>,            IEnumAAFParameterDefs,      AddrOfSmartPointer2<IAAFParameterDef> >
        AxParameterDefsIter;
        
typedef AxIterator< IAAFSmartPointer2<IAAFPluginDef>,               IEnumAAFPluginDefs,         AddrOfSmartPointer2<IAAFPluginDef> >
        AxPluginDefsIter;
        
typedef AxIterator< IAAFSmartPointer2<IAAFContainerDef>,            IEnumAAFContainerDefs,      AddrOfSmartPointer2<IAAFContainerDef> >
        AxContainerDefsIter;
        
typedef AxIterator< IAAFSmartPointer2<IAAFInterpolationDef>,        IEnumAAFInterpolationDefs,  AddrOfSmartPointer2<IAAFInterpolationDef> >
        AxInterpolationDefsIter;
        
typedef AxIterator< IAAFSmartPointer2<IAAFCodecDef>,                IEnumAAFCodecDefs,          AddrOfSmartPointer2<IAAFCodecDef> >
        AxCodecDefsIter;

typedef AxIterator< IAAFSmartPointer2<IAAFKLVDataDefinition>,       IEnumAAFKLVDataDefs,        AddrOfSmartPointer2<IAAFKLVDataDefinition> >
        AxKLVDataDefsIter;
        
typedef AxIterator< IAAFSmartPointer2<IAAFTaggedValueDefinition>,   IEnumAAFTaggedValueDefs,    AddrOfSmartPointer2<IAAFTaggedValueDefinition> >
        AxTaggedValueDefsIter;

typedef AxIterator< aafUID_t,                                       IEnumAAFCodecFlavours,      AddrOfAafUID >
        AxCodecFlavoursIter;

typedef AxIterator< aafUID_t,                                       IEnumAAFLoadedPlugins,      AddrOfAafUID >
        AxLoadedPluginsIter;

typedef AxIterator< IAAFSmartPointer2<IAAFComponent>,               IEnumAAFComponents,         AddrOfSmartPointer2<IAAFComponent> >
        AxComponentsIter;

typedef AxIterator< IAAFSmartPointer2<IAAFSegment>,                 IEnumAAFSegments,           AddrOfSmartPointer2<IAAFSegment> >
        AxSegmentsIter;

typedef AxIterator< IAAFSmartPointer2<IAAFClassDef>,                IEnumAAFClassDefs,          AddrOfSmartPointer2<IAAFClassDef> >
        AxClassDefIter;

typedef AxIterator< IAAFSmartPointer2<IAAFTypeDef>,                 IEnumAAFTypeDefs,           AddrOfSmartPointer2<IAAFTypeDef> >
        AxTypeDefIter;

typedef AxIterator< IAAFSmartPointer2<IAAFPropertyDef>,             IEnumAAFPropertyDefs,       AddrOfSmartPointer2<IAAFPropertyDef> >
        AxPropertyDefIter;

typedef AxIterator< IAAFSmartPointer2<IAAFLocator>,                 IEnumAAFLocators,           AddrOfSmartPointer2<IAAFLocator> >
        AxLocatorIter;

typedef AxIterator< IAAFSmartPointer2<IAAFParameter>,               IEnumAAFParameters,         AddrOfSmartPointer2<IAAFParameter> >
        AxParameterIter;

typedef AxIterator< IAAFSmartPointer2<IAAFControlPoint>,            IEnumAAFControlPoints,      AddrOfSmartPointer2<IAAFControlPoint> >
        AxControlPointIter;
#if 0
// the old ones
typedef AxIterator< IAAFSmartPointer<IAAFProperty>,      IEnumAAFProperties >	  AxPropertyIter;
typedef AxIterator< IAAFSmartPointer<IAAFPropertyValue>, IEnumAAFPropertyValues > AxPropertyValueIter;
typedef AxIterator< IAAFSmartPointer<IAAFMob>,           IEnumAAFMobs >		      AxMobIter;
typedef AxIterator< IAAFSmartPointer<IAAFEssenceData>,   IEnumAAFEssenceData >	  AxEssenceIter;
typedef AxIterator< IAAFSmartPointer<IAAFMobSlot>,       IEnumAAFMobSlots >	      AxMobSlotIter;
typedef AxIterator< IAAFSmartPointer<IAAFDataDef>,       IEnumAAFDataDefs >       AxDataDefsIter;
typedef AxIterator< aafUID_t,                            IEnumAAFCodecFlavours >  AxCodecFlavoursIter;
typedef AxIterator< aafUID_t,                            IEnumAAFLoadedPlugins >  AxLoadedPluginsIter;
typedef AxIterator< IAAFSmartPointer<IAAFComponent>,	 IEnumAAFComponents >	  AxComponentsIter;
typedef AxIterator< IAAFSmartPointer<IAAFSegment>,	 IEnumAAFSegments >	  AxSegmentsIter;
typedef AxIterator< IAAFSmartPointer<IAAFClassDef>,      IEnumAAFClassDefs >      AxClassDefIter;
typedef AxIterator< IAAFSmartPointer<IAAFTypeDef>,       IEnumAAFTypeDefs >       AxTypeDefIter;
typedef AxIterator< IAAFSmartPointer<IAAFPropertyDef>,   IEnumAAFPropertyDefs >   AxPropertyDefIter;
typedef AxIterator< IAAFSmartPointer<IAAFLocator>,       IEnumAAFLocators >       AxLocatorIter;
typedef AxIterator< IAAFSmartPointer<IAAFParameter>,     IEnumAAFParameters >     AxParameterIter;
typedef AxIterator< IAAFSmartPointer<IAAFControlPoint>,  IEnumAAFControlPoints >  AxControlPointIter;
#endif

// Records contain a set of named values that must be iterated over as well,
// but don't have any sort of native iterator.  This wrapper presents
// an iterator like iterface for records.

// ... probably better if record iterator is in separate file.

class AxRecordIterator {
public:
	typedef std::pair<AxString, IAAFPropertyValueSP> Pair;
	
	AxRecordIterator( IAAFPropertyValueSP& spPropVal,
					  IAAFTypeDefRecordSP& spTypeDef );
	~AxRecordIterator();

	bool NextOne( Pair& ret );

	void Reset();

	std::auto_ptr<AxRecordIterator> Clone();

private:
	AxRecordIterator();
	AxRecordIterator( const AxRecordIterator& );
	AxRecordIterator& operator=( const AxRecordIterator& );
	
	IAAFPropertyValueSP _spPropVal;
	IAAFTypeDefRecordSP _spTypeDef;

	aafUInt32 _count;
	aafUInt32 _current;
};


// Iterator for fixed and variable arrays.
//
// These formerly worked around a lack of (full) implementation
// (AAFRESULT_NOT_IN_CURRENT_VERSION) of the GetElement() COM
// interface for arrays.  As of 10 June 2003 the
// IAAFTypeDef{Fixed,Variable}Array::GetElements() interface is
// implemented, hence, this class is redundant.
//
// Type is intended to be IAAFTypeDefFixedArray or IAAFTypeDefVariableArray

template <class TypeDef>
class AxArrayIterator {
public:
	typedef IAAFSmartPointer<TypeDef> TypeDefSP;

	AxArrayIterator( TypeDefSP spTypeDef, IAAFPropertyValueSP spPropVal );

	~AxArrayIterator();

	bool NextOne( IAAFPropertyValueSP&  );

	void Reset();

	std::auto_ptr< AxArrayIterator<TypeDef> > Clone();
	
private:

	TypeDefSP _spTypeDef;
	IAAFPropertyValueSP _spPropVal;
	aafUInt32 _count;
	aafUInt32 _current;
};

typedef AxArrayIterator<IAAFTypeDefFixedArray> AxFixedArrayIterator;
typedef AxArrayIterator<IAAFTypeDefVariableArray> AxVariableArrayIterator;

#endif
