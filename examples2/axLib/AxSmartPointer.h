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

#define AXIID_SPECIALIZE(TYPE) \
template<> inline const IID& AxIID<TYPE>( TYPE* ) { return IID_##TYPE; }

//
// Usual smart pointer typedefs
//

#define AXSP_TDEF(T) \
  typedef IAAFSmartPointer< T >	T##SP ; \
  AXIID_SPECIALIZE(T)

AXSP_TDEF( IAAFAIFCDescriptor )
AXSP_TDEF( IAAFClassDef )
AXSP_TDEF( IAAFCodecDef )
AXSP_TDEF( IAAFCommentMarker )
AXSP_TDEF( IAAFComponent )
AXSP_TDEF( IAAFCompositionMob )
AXSP_TDEF( IAAFConstantValue )
AXSP_TDEF( IAAFContainerDef )
AXSP_TDEF( IAAFContentStorage )
AXSP_TDEF( IAAFControlPoint )
AXSP_TDEF( IAAFDataDef )
AXSP_TDEF( IAAFDefObject )
AXSP_TDEF( IAAFPluginDef )
AXSP_TDEF( IAAFDictionary )
AXSP_TDEF( IAAFEdgecode )
AXSP_TDEF( IAAFOperationDef )
AXSP_TDEF( IAAFEssenceAccess )
AXSP_TDEF( IAAFEssenceData )
AXSP_TDEF( IAAFEssenceDescriptor )
AXSP_TDEF( IAAFEvent )
AXSP_TDEF( IAAFEventMobSlot )
AXSP_TDEF( IAAFFile )
AXSP_TDEF( IAAFFileDescriptor )
AXSP_TDEF( IAAFFilmDescriptor )
AXSP_TDEF( IAAFDigitalImageDescriptor )
AXSP_TDEF( IAAFCDCIDescriptor )
AXSP_TDEF( IAAFEssenceFormat )
AXSP_TDEF( IAAFEssenceGroup )
AXSP_TDEF( IAAFFiller )
AXSP_TDEF( IAAFFindSourceInfo )
AXSP_TDEF( IAAFOperationGroup )
AXSP_TDEF( IAAFGPITrigger )
AXSP_TDEF( IAAFHeader )
AXSP_TDEF( IAAFIdentification )
AXSP_TDEF( IAAFInterpolationDef )
AXSP_TDEF( IAAFKLVData )
AXSP_TDEF( IAAFLocator )
AXSP_TDEF( IAAFMasterMob )
AXSP_TDEF( IAAFMasterMobEx )
AXSP_TDEF( IAAFMetaDefinition )
AXSP_TDEF( IAAFMob )
AXSP_TDEF( IAAFMobSlot )
AXSP_TDEF( IAAFNestedScope )
AXSP_TDEF( IAAFNetworkLocator )
AXSP_TDEF( IAAFObject )
AXSP_TDEF( IAAFParameter )
AXSP_TDEF( IAAFParameterDef )
AXSP_TDEF( IAAFProperty )
AXSP_TDEF( IAAFPropertyDef )
AXSP_TDEF( IAAFPropertyValue )
AXSP_TDEF( IAAFPluginManager )
AXSP_TDEF( IAAFPulldown )
AXSP_TDEF( IAAFRGBADescriptor )
AXSP_TDEF( IAAFScopeReference )
AXSP_TDEF( IAAFSegment )
AXSP_TDEF( IAAFSelector )
AXSP_TDEF( IAAFSequence )
AXSP_TDEF( IAAFSourceClip )
AXSP_TDEF( IAAFSourceMob )
AXSP_TDEF( IAAFSourceReference )
AXSP_TDEF( IAAFStaticMobSlot )
AXSP_TDEF( IAAFTapeDescriptor )
AXSP_TDEF( IAAFTaggedValue )
AXSP_TDEF( IAAFTextLocator )
AXSP_TDEF( IAAFTimecode )
AXSP_TDEF( IAAFTimelineMobSlot )
AXSP_TDEF( IAAFTransition )
AXSP_TDEF( IAAFTIFFDescriptor )
AXSP_TDEF( IAAFTimecodeStream )
AXSP_TDEF( IAAFTimecodeStream12M )
AXSP_TDEF( IAAFTypeDef )
AXSP_TDEF( IAAFTypeDefCharacter )
AXSP_TDEF( IAAFTypeDefIndirect )
AXSP_TDEF( IAAFTypeDefInt )
AXSP_TDEF( IAAFTypeDefRename )
AXSP_TDEF( IAAFTypeDefEnum )
AXSP_TDEF( IAAFTypeDefExtEnum )
AXSP_TDEF( IAAFTypeDefFixedArray )
AXSP_TDEF( IAAFTypeDefRecord )
AXSP_TDEF( IAAFTypeDefSet )
AXSP_TDEF( IAAFTypeDefStream )
AXSP_TDEF( IAAFTypeDefString )
AXSP_TDEF( IAAFTypeDefStrongObjRef )
AXSP_TDEF( IAAFTypeDefWeakObjRef )
AXSP_TDEF( IAAFTypeDefObjectRef )
AXSP_TDEF( IAAFTypeDefOpaque )
AXSP_TDEF( IAAFTypeDefVariableArray )
AXSP_TDEF( IAAFVaryingValue )
AXSP_TDEF( IAAFWAVEDescriptor )
AXSP_TDEF( IEnumAAFClassDefs )
AXSP_TDEF( IEnumAAFCodecDefs )
AXSP_TDEF( IEnumAAFCodecFlavours )
AXSP_TDEF( IEnumAAFComponents )
AXSP_TDEF( IEnumAAFContainerDefs )
AXSP_TDEF( IEnumAAFControlPoints )
AXSP_TDEF( IEnumAAFDataDefs )
AXSP_TDEF( IEnumAAFIdentifications )
AXSP_TDEF( IEnumAAFInterpolationDefs )
AXSP_TDEF( IEnumAAFOperationDefs )
AXSP_TDEF( IEnumAAFEssenceData )
AXSP_TDEF( IEnumAAFKLVData )
AXSP_TDEF( IEnumAAFLoadedPlugins )
AXSP_TDEF( IEnumAAFLocators )
AXSP_TDEF( IEnumAAFMobSlots )
AXSP_TDEF( IEnumAAFMobs )
AXSP_TDEF( IEnumAAFParameterDefs )
AXSP_TDEF( IEnumAAFParameters )
AXSP_TDEF( IEnumAAFProperties )
AXSP_TDEF( IEnumAAFPropertyDefs )
AXSP_TDEF( IEnumAAFPropertyValues )
AXSP_TDEF( IEnumAAFPluginDefs )
AXSP_TDEF( IEnumAAFPluginLocators )
AXSP_TDEF( IEnumAAFSegments )
AXSP_TDEF( IEnumAAFTaggedValues )
AXSP_TDEF( IEnumAAFTypeDefs )
AXSP_TDEF( IAAFRawStorage )
AXSP_TDEF( IAAFRandomRawStorage )
AXSP_TDEF( IAAFGetFileBits )
AXSP_TDEF( IAAFSetFileBits )
AXSP_TDEF( IAAFRandomFile )
AXSP_TDEF( IAAFEndian )
AXSP_TDEF( IAAFSearchSource )
AXSP_TDEF( IAAFEssenceMultiAccess )
AXSP_TDEF( IAAFTypeDefVariableArrayEx )

AXSP_TDEF( IUnknown )

//=---------------------------------------------------------------------=

// This is the type-safe version of AxQueryInterface.  The iid
// parameter can be removed if desired.

template <class TypeSrc, class TypeDst>
inline void AxQueryInterface( IAAFSmartPointer<TypeSrc> spSrc,
							  IAAFSmartPointer<TypeDst>& spDst,
							  const IID& iid )
{
	TypeDst* dummy;

	if ( !( iid == AxIID( dummy ) ) ) {
		throw AxEx( L"IID TypeDst mismatch" );
	}
	
	CHECK_HRESULT(
		spSrc->QueryInterface( AxIID(dummy), reinterpret_cast<void**>(&spDst) ) );
}

// This version returns spDst.  The compiler can't deduce TypeDst,
// therefore, this must be called with explicit template parameters.
// e.g.: AxQueryInterface<IAAFHeader,IAAFObject>( spHeader, IID_IAAFObject )

template <class TypeSrc, class TypeDst>
inline IAAFSmartPointer< TypeDst >
AxQueryInterface( IAAFSmartPointer<TypeSrc> spSrc,
				  const IID& iid )
{
	IAAFSmartPointer< TypeDst > spDst;
	AxQueryInterface( spSrc, spDst, iid );
	return spDst;
}


// AxIsA() - A versin of QueryInterface that returns a bool indicating
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


#endif
