#ifndef __AxSmartPointer_h__
#define __AxSmartPointer_h__

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

#include "AxEx.h"

#include <AAFStoredObjectIDs.h>

#include <utility>

// Define AAF_SMART_POINTER_ASSERT before including AAFSmartPointer.h
// to ensure smart pointer assertions failures are handled by an
// AxExSmartPointer exception.

#define AAF_SMART_POINTER_ASSERT(condition) \
	{ if (!(condition)) throw AxExSmartPointer(); }


#include <AAFSmartPointer.h>


// Small template class and macro that are used to create a set of
// functions that will return the IID of an IAAF interface type.  This
// is used to enforce type safety in AxQueryInterface.  The Type*
// argument is a dummy argument required to permit the compiler to
// deduce the correct template instance to use.  It would not work
// without this.
template <class Type>
inline const IID& AxIID( Type* )
{
	throw AxExBadImp( L"AxIID()" );
}

#define AXIID_SPECIALIZE(TYPE, TYPEID) \
template<> inline const IID& AxIID< TYPE >( TYPE * ) { return TYPEID; }


template <class Type>
inline const aafUID_t& AxAUID( Type* )
{
	throw AxExBadImp( L"AxAUID()" );
}

#define AXAUID_SPECIALIZE(TYPE, TYPEID) \
template<> inline const aafUID_t& AxAUID< TYPE >( TYPE * ) { return TYPEID; }

//
// Macros to declare a smart pointer typedef, and to
// define specialized instances of the AxIID and AxAUID
// macros.  Three versions are required:
//

// Declare smart pointer, AxIID, and AxAUID given root
// type name (i.e. no prefix).
#define AXSP_TDEF_A(T) \
  typedef IAAFSmartPointer< IAAF##T >	IAAF##T##SP ; \
  AXIID_SPECIALIZE( IAAF##T, IID_IAAF##T ) \
  AXAUID_SPECIALIZE( IAAF##T, AUID_AAF##T )

// Declare smart pointer, and AxIID given root type name
// (i.e. no prefix).  Used for COM interfaces with no
// associated stored object.
#define AXSP_TDEF_B(T) \
  typedef IAAFSmartPointer< IAAF##T >	IAAF##T##SP ; \
  AXIID_SPECIALIZE( IAAF##T, IID_IAAF##T ) \

// Declare smart ponter and AxIID given full type name.
// Used to declare IEnum types.
#define AXSP_TDEF_C(T) \
  typedef IAAFSmartPointer< T >	T##SP ; \
  AXIID_SPECIALIZE( T, IID_##T )

AXSP_TDEF_A( AIFCDescriptor )
AXSP_TDEF_A( ClassDef )
AXSP_TDEF_A( CodecDef )
AXSP_TDEF_A( CommentMarker )
AXSP_TDEF_A( Component )
AXSP_TDEF_A( CompositionMob )
AXSP_TDEF_A( ConstantValue )
AXSP_TDEF_A( ContainerDef )
AXSP_TDEF_A( ContentStorage )
AXSP_TDEF_A( ControlPoint )
AXSP_TDEF_A( DataDef )
AXSP_TDEF_A( DefObject )
AXSP_TDEF_A( PluginDef )
AXSP_TDEF_A( Dictionary )
AXSP_TDEF_A( Edgecode )
AXSP_TDEF_A( OperationDef )
AXSP_TDEF_B( EssenceAccess )
AXSP_TDEF_A( EssenceData )
AXSP_TDEF_A( EssenceDescriptor )
AXSP_TDEF_A( Event )
AXSP_TDEF_A( EventMobSlot )
AXSP_TDEF_B( File )
AXSP_TDEF_A( FileDescriptor )
AXSP_TDEF_A( FilmDescriptor )
AXSP_TDEF_A( DigitalImageDescriptor )
AXSP_TDEF_A( CDCIDescriptor )
AXSP_TDEF_B( EssenceFormat )
AXSP_TDEF_A( EssenceGroup )
AXSP_TDEF_A( Filler )
AXSP_TDEF_B( FindSourceInfo )
AXSP_TDEF_A( OperationGroup )
AXSP_TDEF_A( GPITrigger )
AXSP_TDEF_A( Header )
AXSP_TDEF_A( Identification )
AXSP_TDEF_A( InterpolationDef )
AXSP_TDEF_A( KLVData )
AXSP_TDEF_A( Locator )
AXSP_TDEF_A( MasterMob )
AXSP_TDEF_B( MasterMobEx )
AXSP_TDEF_A( MetaDefinition )
AXSP_TDEF_A( Mob )
AXSP_TDEF_A( MobSlot )
AXSP_TDEF_A( NestedScope )
AXSP_TDEF_A( NetworkLocator )
AXSP_TDEF_A( Object )
AXSP_TDEF_A( Parameter )
AXSP_TDEF_A( ParameterDef )
AXSP_TDEF_B( Property )
AXSP_TDEF_A( PropertyDef )
AXSP_TDEF_B( PropertyValue )
AXSP_TDEF_B( PluginManager )
AXSP_TDEF_A( Pulldown )
AXSP_TDEF_A( RGBADescriptor )
AXSP_TDEF_A( ScopeReference )
AXSP_TDEF_A( Segment )
AXSP_TDEF_A( Selector )
AXSP_TDEF_A( Sequence )
AXSP_TDEF_A( SourceClip )
AXSP_TDEF_A( SourceMob )
AXSP_TDEF_A( SourceReference )
AXSP_TDEF_A( StaticMobSlot )
AXSP_TDEF_A( TapeDescriptor )
AXSP_TDEF_A( TaggedValue )
AXSP_TDEF_A( TextLocator )
AXSP_TDEF_A( Timecode )
AXSP_TDEF_A( TimelineMobSlot )
AXSP_TDEF_A( Transition )
AXSP_TDEF_A( TIFFDescriptor )
AXSP_TDEF_A( TimecodeStream )
AXSP_TDEF_A( TimecodeStream12M )
AXSP_TDEF_A( TypeDef )
AXSP_TDEF_A( TypeDefCharacter )
AXSP_TDEF_A( TypeDefIndirect )
AXSP_TDEF_A( TypeDefInt )
AXSP_TDEF_A( TypeDefRename )
AXSP_TDEF_A( TypeDefEnum )
AXSP_TDEF_A( TypeDefExtEnum )
AXSP_TDEF_A( TypeDefFixedArray )
AXSP_TDEF_A( TypeDefRecord )
AXSP_TDEF_A( TypeDefSet )
AXSP_TDEF_A( TypeDefStream )
AXSP_TDEF_A( TypeDefString )
AXSP_TDEF_A( TypeDefStrongObjRef )
AXSP_TDEF_A( TypeDefWeakObjRef )
AXSP_TDEF_B( TypeDefObjectRef )
AXSP_TDEF_A( TypeDefOpaque )
AXSP_TDEF_A( TypeDefVariableArray )
AXSP_TDEF_A( VaryingValue )
AXSP_TDEF_A( WAVEDescriptor )
AXSP_TDEF_C( IEnumAAFClassDefs )
AXSP_TDEF_C( IEnumAAFCodecDefs )
AXSP_TDEF_C( IEnumAAFCodecFlavours )
AXSP_TDEF_C( IEnumAAFComponents )
AXSP_TDEF_C( IEnumAAFContainerDefs )
AXSP_TDEF_C( IEnumAAFControlPoints )
AXSP_TDEF_C( IEnumAAFDataDefs )
AXSP_TDEF_C( IEnumAAFIdentifications )
AXSP_TDEF_C( IEnumAAFInterpolationDefs )
AXSP_TDEF_C( IEnumAAFOperationDefs )
AXSP_TDEF_C( IEnumAAFEssenceData )
AXSP_TDEF_C( IEnumAAFKLVData )
AXSP_TDEF_C( IEnumAAFLoadedPlugins )
AXSP_TDEF_C( IEnumAAFLocators )
AXSP_TDEF_C( IEnumAAFMobSlots )
AXSP_TDEF_C( IEnumAAFMobs )
AXSP_TDEF_C( IEnumAAFParameterDefs )
AXSP_TDEF_C( IEnumAAFParameters )
AXSP_TDEF_C( IEnumAAFProperties )
AXSP_TDEF_C( IEnumAAFPropertyDefs )
AXSP_TDEF_C( IEnumAAFPropertyValues )
AXSP_TDEF_C( IEnumAAFPluginDefs )
AXSP_TDEF_C( IEnumAAFPluginLocators )
AXSP_TDEF_C( IEnumAAFSegments )
AXSP_TDEF_C( IEnumAAFTaggedValues )
AXSP_TDEF_C( IEnumAAFTypeDefs )
AXSP_TDEF_B( RawStorage )
AXSP_TDEF_B( RandomRawStorage )
AXSP_TDEF_B( GetFileBits )
AXSP_TDEF_B( SetFileBits )
AXSP_TDEF_B( RandomFile )
AXSP_TDEF_B( Endian )
AXSP_TDEF_B( SearchSource )
AXSP_TDEF_B( EssenceMultiAccess )
AXSP_TDEF_B( TypeDefVariableArrayEx )

AXSP_TDEF_C( IUnknown )

//=---------------------------------------------------------------------=

// This is the type-safe version of AxQueryInterface.  The iid
// parameter can be removed if desired.

template <class TypeSrc, class TypeDst>
inline void AxQueryInterface( IAAFSmartPointer<TypeSrc> spSrc,
							  IAAFSmartPointer<TypeDst>& spDst )
{
	TypeDst* dummy = 0;
	
	CHECK_HRESULT(
		spSrc->QueryInterface( AxIID(dummy), reinterpret_cast<void**>(&spDst) ) );
}

// This version returns spDst.  The compiler can't deduce TypeDst,
// therefore, this must be called with explicit template parameters.
// e.g.: AxQueryInterface<IAAFHeader,IAAFObject>( spHeader, IID_IAAFObject )

template <class TypeSrc, class TypeDst>
inline IAAFSmartPointer< TypeDst >
AxQueryInterface( IAAFSmartPointer<TypeSrc> spSrc )
{
	IAAFSmartPointer< TypeDst > spDst;
	AxQueryInterface( spSrc, spDst );
	return spDst;
}


// AxIsA() - A version of QueryInterface that returns a bool indicating
// success.  Maybe "IsA" is not the best name.  Maybe it would be
// better to return a pair<bool, IAAFSmartPointer<TypeDst>, but using
// TypeDst for one of the arguments permits the compiler to deduce
// TypeDst.

template <class TypeDst>
inline bool AxIsA( IUnknownSP sp, IAAFSmartPointer<TypeDst>& spDst )
{
	HRESULT hr;
	TypeDst* dummy;

	hr = sp->QueryInterface( AxIID( dummy ),
				 reinterpret_cast<void**>(&spDst) );

	if ( SUCCEEDED(hr) ) {
		return true;
	}
	else if ( hr == E_NOINTERFACE ) {
		return false;
	}
	
	CHECK_HRESULT( hr );
	
	// Never reached;
	return false;
}


// AxIsA() - A test to check that calling query interface to make a
// conversion will work. Requires explicit template instantiation ie:
// if(AxIsA<IAAFMasterMob>(mob) - This function is only a test and does
// not return the converted type.
template <class TypeDst>
inline bool AxIsA( IUnknownSP sp)
{
	HRESULT hr;
	TypeDst * dummy;
	hr = sp->QueryInterface( AxIID( dummy ),reinterpret_cast<void**>(&dummy));

	if ( SUCCEEDED(hr) ) {
		return true;
	}
	else if ( hr == E_NOINTERFACE ) {
		return false;
	}
	
	CHECK_HRESULT( hr );
	
	// Never reached;
	return false;
}

#endif
