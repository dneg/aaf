#ifndef __AxDictionary_h__
#define __AxDictionary_h__

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
#include "AxObject.h"

//=---------------------------------------------------------------------=

class AxDictionary : public AxObject {

public:
	AxDictionary( IAAFDictionarySP& spIaafDictionary );
	~AxDictionary();

	bool isKnownTypeDef( const aafUID_t& typeId );
	
	IAAFTypeDefSP LookupTypeDef( const aafUID_t& typeId );
	IAAFClassDefSP LookupClassDef( const aafUID_t& classId );
	IAAFDataDefSP LookupDataDef( const aafUID_t& dataDefId );
	IAAFOperationDefSP LookupOperationDef( const aafUID_t& opDefId );

	void RegisterOpaqueTypeDef( IAAFTypeDefSP );

	void RegisterOperationDef( IAAFOperationDefSP );

	void RegisterKLVDataKey( const aafUID_t& uuid,
							 IAAFTypeDefSP& spTypeDef );

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

// FIXME - How can we confirm that auid and iid match?  (Or is it
// implied if the cast to iid succeeds?)  Perhaps a map of IID to
// AUID.  That would probably work since we have a type-safe
// AxQueryInterface call to confirm that iid is correct given Type.
// Actually, if we had a map<IID,AUID> then these function would not
// require auid or iid arguments - both could be deduced from Type.

template <class Type>
IAAFSmartPointer<Type> AxCreateInstance( AxDictionary& dict, 
										 const aafUID_t& auid,
										 const IID& iid )
{
	Type* dummy;
	if ( !( iid == AxIID( dummy ) ) ) {
		throw AxEx( L"IID Type mismatch" );
	}

	IUnknownSP spIUnknown = dict.CreateInstance( auid, iid );
	IAAFSmartPointer<Type> sp;
	AxQueryInterface( spIUnknown, sp, iid );
	return sp;
}

template <class Type>
IAAFSmartPointer<Type> AxCreateMetaInstance( AxDictionary& dict, 
											 const aafUID_t& auid,
											 const IID& iid )
{
	Type* dummy;
	if ( !( iid == AxIID( dummy ) ) ) {
		throw AxEx( L"IID Type mismatch" );
	}

	IUnknownSP spIUnknown = dict.CreateMetaInstance( auid, iid );
	IAAFSmartPointer<Type> sp;
	AxQueryInterface( spIUnknown, sp, iid );
	return sp;
}

#endif
