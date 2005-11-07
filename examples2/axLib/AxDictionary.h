#ifndef __AxDictionary_h__
#define __AxDictionary_h__

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
#include "AxObject.h"

//=---------------------------------------------------------------------=

class AxDictionary : public AxObject {

public:
	AxDictionary( IAAFDictionarySP spIaafDictionary );
	~AxDictionary();

	bool isKnownTypeDef( const aafUID_t& typeId );
	bool isKnownOperationDef( const aafUID_t& opId );
	bool isKnownParameterDef( const aafUID_t& paramId );
	
	void RegisterClassDef( IAAFClassDefSP spIaafClassDef );

	IAAFTypeDefSP LookupTypeDef( const aafUID_t& typeId );
	IAAFClassDefSP LookupClassDef( const aafUID_t& classId );
	IAAFDataDefSP LookupDataDef( const aafUID_t& dataDefId );
	IAAFOperationDefSP LookupOperationDef( const aafUID_t& opDefId );
	IAAFParameterDefSP LookupParameterDef( const aafUID_t& interpDefId );
	IAAFContainerDefSP LookupContainerDef( const aafUID_t& contDefId );
	IAAFInterpolationDefSP LookupInterpolationDef( const aafUID_t& interpDefId );
	IAAFCodecDefSP LookupCodecDef( const aafUID_t& codecDefId );

	IEnumAAFClassDefsSP GetClassDefs();
	IEnumAAFTypeDefsSP GetTypeDefs();
	IEnumAAFDataDefsSP GetDataDefs();
	IEnumAAFOperationDefsSP GetOperationDefs();
	IEnumAAFParameterDefsSP GetParameterDefs();
    IEnumAAFPluginDefsSP GetPluginDefs();
    IEnumAAFContainerDefsSP GetContainerDefs();
    IEnumAAFInterpolationDefsSP GetInterpolationDefs();
    IEnumAAFCodecDefsSP GetCodecDefs();

	void RegisterOpaqueTypeDef( IAAFTypeDefSP );
	void RegisterOperationDef( IAAFOperationDefSP );
	void RegisterParameterDef( IAAFParameterDefSP );
	void RegisterCodecDef( IAAFCodecDefSP );
	void RegisterKLVDataKey( const aafUID_t& uuid,
				 IAAFTypeDefSP spTypeDef );
    void RegisterDataDef( IAAFDataDefSP );
    void RegisterPluginDef( IAAFPluginDefSP );
    void RegisterContainerDef( IAAFContainerDefSP );
    void RegisterInterpolationDef( IAAFInterpolationDefSP );

	IUnknownSP CreateInstance( const aafUID_t& auid,
				  			   const IID& iid );

	
	IUnknownSP CreateMetaInstance( const aafUID_t& auid,
				  				   const IID& iid );

	aafUInt32 CountOpaqueTypeDefs();

	inline operator IAAFDictionarySP ()
	{ return _spIaafDictionary; }

private:
	AxDictionary();
	AxDictionary( const AxDictionary& );
	AxDictionary& operator=( const AxDictionary& );
	
	IAAFDictionarySP _spIaafDictionary;
};

//=---------------------------------------------------------------------=


template <class Type>
void AxCreateInstance( AxDictionary& axDict,
	    	           IAAFSmartPointer<Type>& sp )
{
        Type* dummy = 0;

	IUnknownSP spIUnknown = axDict.CreateInstance( AxAUID(dummy), AxIID(dummy) );
	AxQueryInterface( spIUnknown, sp );
}

template <class Type>
void AxCreateInstance( IAAFDictionarySP spDict,
	    	           IAAFSmartPointer<Type>& sp )
{
        Type* dummy = 0;

	AxDictionary axDict( spDict );
	IUnknownSP spIUnknown = axDict.CreateInstance( AxAUID(dummy), AxIID(dummy) );
	AxQueryInterface( spIUnknown, sp );
}

template <class Type>
IAAFSmartPointer<Type> AxCreateInstance( AxDictionary& dict )
{
	Type* dummy = 0;

	IUnknownSP spIUnknown = dict.CreateInstance( AxAUID(dummy), AxIID(dummy) );
	IAAFSmartPointer<Type> sp;
	AxQueryInterface( spIUnknown, sp );
	return sp;
}

template <class Type>
IAAFSmartPointer<Type> AxCreateInstance( IAAFDictionarySP spDict )
{
	Type* dummy = 0;
	
	AxDictionary dict(spDict);
	IUnknownSP spIUnknown = dict.CreateInstance( AxAUID(dummy), AxIID(dummy) );
	IAAFSmartPointer<Type> sp;
	AxQueryInterface( spIUnknown, sp );
	return sp;
}

template <class Type>
IAAFSmartPointer<Type> AxCreateMetaInstance( AxDictionary& dict )
{
	Type* dummy = 0;

	IUnknownSP spIUnknown = dict.CreateMetaInstance( AxAUID(dummy), AxIID(dummy) );
	IAAFSmartPointer<Type> sp;
	AxQueryInterface( spIUnknown, sp );
	return sp;
}

#endif
