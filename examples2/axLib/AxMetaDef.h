#ifndef __AxMetaDef_h__
#define __AxMetaDef_h__

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
#include "AxBaseObj.h"

// Please refer to the UML, for an overview of the class inheritance
// graph implemented here.

class AxMetaDefinition : public AxBaseObj {

public:
	AxMetaDefinition( IAAFMetaDefinitionSP spIaafMetaDefinition );
	virtual ~AxMetaDefinition();

	aafUID_t GetAUID();

private:
	AxMetaDefinition();
	AxMetaDefinition( const AxMetaDefinition& );
	AxMetaDefinition& operator=( const AxMetaDefinition& );
		
	IAAFMetaDefinitionSP _spIaafMetaDefinition;
};


//=---------------------------------------------------------------------=

class AxClassDef : public AxMetaDefinition {
public:
	AxClassDef( IAAFClassDefSP spIaafClassDef );
	~AxClassDef();

	// vanilla wrapper
	IUnknownSP CreateInstance( const IID& iid );

	// Type safe wrapper.  First calls vanilla wrapper to get an IUnknown given
	// iid, then casts that to Type using iid.
	template< class Type >
	void CreateInstance( const IID& iid, IAAFSmartPointer<Type>& sp )
	{
		AxQueryInterface( CreateInstance( iid ), sp, iid );
	}

	template< class Type >
	IAAFSmartPointer<Type> CreateInstance( const IID& iid )
	{
		IAAFSmartPointer<Type> sp;
		AxQueryInterface( CreateInstance( iid ), sp, iid );
	}

	IAAFClassDefSP GetParent();
	aafBoolean_t IsConcrete();

	IEnumAAFPropertyDefsSP GetPropertyDefs();
	IAAFPropertyDefSP LookupPropertyDef( const aafUID_t& propertyId );

	operator IAAFClassDefSP ()
	{ return _spIaafClassDef; }

private:
	IAAFClassDefSP _spIaafClassDef;
};


//=---------------------------------------------------------------------=

class AxPropertyDef : public AxMetaDefinition {
public:
	AxPropertyDef( IAAFPropertyDefSP spIaafPropertyDef );
	~AxPropertyDef();

//    AxString GetName();
	aafBoolean_t GetIsOptional();
	aafBoolean_t GetIsUniqueIdentifier();
	IAAFTypeDefSP GetTypeDef();

private:
	IAAFPropertyDefSP _spIaafPropertyDef;
};

//=---------------------------------------------------------------------=


class AxTypeDef : public AxMetaDefinition {

public:
	AxTypeDef( IAAFTypeDefSP spIaafTypeDef );
	virtual ~AxTypeDef();

	virtual eAAFTypeCategory_t GetTypeCategory();

	IAAFTypeDefSP GetTypeDefSP();

	operator IAAFTypeDefSP ()
	{ return _spIaafTypeDef; }

private:
	AxTypeDef();
	AxTypeDef( const AxTypeDef& );
	AxTypeDef& operator=( const AxTypeDef& );
		
	IAAFTypeDefSP _spIaafTypeDef;
};

//=---------------------------------------------------------------------=

class AxTypeDefIndirect : public AxTypeDef {
public:
	AxTypeDefIndirect( IAAFTypeDefIndirectSP spIaafTypeDefIndirect );
	virtual ~AxTypeDefIndirect();

	IAAFTypeDefSP GetActualType( IAAFPropertyValueSP& );
	IAAFPropertyValueSP GetActualValue( IAAFPropertyValueSP& );

private:
	AxTypeDefIndirect();
	AxTypeDefIndirect( const AxTypeDefIndirect& );
	AxTypeDefIndirect& operator=( const AxTypeDefIndirect& );
		
	IAAFTypeDefIndirectSP _spIaafTypeDefIndirect;
};

//=---------------------------------------------------------------------=

class AxTypeDefOpaque : public AxTypeDefIndirect {
public:
	AxTypeDefOpaque( IAAFTypeDefOpaqueSP spIaafTypeDefOpaque  );
	virtual ~AxTypeDefOpaque();

	aafUID_t GetActualTypeID( IAAFPropertyValueSP& );

	aafUInt32 GetHandleBufLen( IAAFPropertyValueSP& );

private:
	AxTypeDefOpaque();
	AxTypeDefOpaque( const AxTypeDefOpaque & );
	AxTypeDefOpaque& operator=( const AxTypeDefOpaque & );
		
	IAAFTypeDefOpaqueSP _spIaafTypeDefOpaque ;
};

//=---------------------------------------------------------------------=


class AxTypeDefRename : public AxTypeDef {
public:
	AxTypeDefRename( IAAFTypeDefRenameSP spIaafTypeDefRename );
	virtual ~AxTypeDefRename();

	void Initialize( const aafUID_t&, IAAFTypeDefSP, const AxString& );

	IAAFTypeDefSP GetBaseType();
	IAAFPropertyValueSP GetBaseValue( IAAFPropertyValueSP& );

	operator IAAFTypeDefRenameSP ()
	{ return _spIaafTypeDefRename; }

private:
	AxTypeDefRename();
	AxTypeDefRename( const AxTypeDefRename& );
	AxTypeDefRename& operator=( const AxTypeDefRename& );
		
	IAAFTypeDefRenameSP _spIaafTypeDefRename;
};


//=---------------------------------------------------------------------=


class AxTypeDefInt : public AxTypeDef {
public:
	AxTypeDefInt( IAAFTypeDefIntSP spIaafTypeDefInt );
	virtual ~AxTypeDefInt();

	aafUInt32 GetSize();
	aafBoolean_t IsSigned();

	void GetInteger( IAAFPropertyValueSP& spPropVal,
					 aafMemPtr_t pVal,
					 aafUInt32  valSize );

	template <typename IntType>
	IntType GetInteger( IAAFPropertyValueSP& spPropVal )
	{
		IntType val;
		GetInteger( spPropVal, &val, sizeof( IntType ) );
		return val;
	}

	template <typename IntType>
	void GetInteger( IAAFPropertyValueSP& spPropVal, IntType* pVal )
	{
		GetInteger( spPropVal, reinterpret_cast<aafMemPtr_t>(pVal), sizeof( IntType ) );
	}


private:

	AxTypeDefInt();
	AxTypeDefInt( const AxTypeDefInt& );
	AxTypeDefInt& operator=( const AxTypeDefInt& );
		
	IAAFTypeDefIntSP _spIaafTypeDefInt;
};

//=---------------------------------------------------------------------=

class AxTypeDefFixedArray : public AxTypeDef {
public:
	AxTypeDefFixedArray( IAAFTypeDefFixedArraySP spIaafTypeDefFixedArray );
	virtual ~AxTypeDefFixedArray();

	IAAFTypeDefSP GetType();

	aafUInt32 GetCount();

	IEnumAAFPropertyValuesSP GetElements( IAAFPropertyValueSP& );

private:
	AxTypeDefFixedArray();
	AxTypeDefFixedArray( const AxTypeDefFixedArray& );
	AxTypeDefFixedArray& operator=( const AxTypeDefFixedArray& );
		
	IAAFTypeDefFixedArraySP _spIaafTypeDefFixedArray;
};

//=---------------------------------------------------------------------=

class AxTypeDefRecord : public AxTypeDef {
public:
	AxTypeDefRecord( IAAFTypeDefRecordSP spIaafTypeDefRecord );
	virtual ~AxTypeDefRecord();

	aafUInt32 GetCount();

	IAAFTypeDefSP GetMemberType( aafUInt32 index );

	AxString GetMemberName( aafUInt32 index );

	IAAFPropertyValueSP GetValue( IAAFPropertyValueSP& spRecordPropVal, aafUInt32 index );

private:
	AxTypeDefRecord();
	AxTypeDefRecord( const AxTypeDefRecord& );
	AxTypeDefRecord& operator=( const AxTypeDefRecord& );
		
	IAAFTypeDefRecordSP _spIaafTypeDefRecord;
};

//=---------------------------------------------------------------------=

class AxTypeDefStream : public AxTypeDef {
public:
	AxTypeDefStream( IAAFTypeDefStreamSP spIaafTypeDefStream );
	virtual ~AxTypeDefStream();

	aafInt64 GetSize( IAAFPropertyValueSP& );

private:
	AxTypeDefStream();
	AxTypeDefStream( const AxTypeDefStream& );
	AxTypeDefStream& operator=( const AxTypeDefStream& );
		
	IAAFTypeDefStreamSP _spIaafTypeDefStream;
};

//=---------------------------------------------------------------------=

class AxTypeDefString : public AxTypeDef {
public:
	AxTypeDefString( IAAFTypeDefStringSP spIaafTypeDefString );
	virtual ~AxTypeDefString();

	AxString GetElements( IAAFPropertyValueSP& );

private:
	AxTypeDefString();
	AxTypeDefString( const AxTypeDefString& );
	AxTypeDefString& operator=( const AxTypeDefString& );
		
	IAAFTypeDefStringSP _spIaafTypeDefString;
};

//=---------------------------------------------------------------------=

class AxTypeDefSet : public AxTypeDef {
public:
	AxTypeDefSet( IAAFTypeDefSetSP spIaafTypeDefSet );
	virtual ~AxTypeDefSet();

	IAAFTypeDefSP GetElementType();

	IEnumAAFPropertyValuesSP GetElements( IAAFPropertyValueSP& );

private:
	AxTypeDefSet();
	AxTypeDefSet( const AxTypeDefSet& );
	AxTypeDefSet& operator=( const AxTypeDefSet& );
		
	IAAFTypeDefSetSP _spIaafTypeDefSet;
};

//=---------------------------------------------------------------------=

class AxTypeDefObjRef : public AxTypeDef {
public:
	AxTypeDefObjRef( IAAFTypeDefObjectRefSP spIaafTypeDefObjRef );
	virtual ~AxTypeDefObjRef();

	virtual IUnknownSP GetObject( IAAFPropertyValueSP& spPropVal,
			  					  const IID& iid );

	IAAFClassDefSP GetObjectType();

private:
	AxTypeDefObjRef();
	AxTypeDefObjRef( const AxTypeDefObjRef& );
	AxTypeDefObjRef& operator=( const AxTypeDefObjRef& );
		
	IAAFTypeDefObjectRefSP _spIaafTypeDefObjRef;
};

//=---------------------------------------------------------------------=

class AxTypeDefWeakObjRef : public AxTypeDefObjRef {

public:
	AxTypeDefWeakObjRef( IAAFTypeDefWeakObjRefSP spIaafTypeDefWeakObjRef );
	virtual ~AxTypeDefWeakObjRef();

private:
	AxTypeDefWeakObjRef();
	AxTypeDefWeakObjRef( const AxTypeDefWeakObjRef& );
	AxTypeDefWeakObjRef& operator=( const AxTypeDefWeakObjRef& );
		
	IAAFTypeDefWeakObjRefSP _spIaafTypeDefWeakObjRef;
};

//=---------------------------------------------------------------------=

class AxTypeDefStrongObjRef : public AxTypeDefObjRef {

public:
	AxTypeDefStrongObjRef( IAAFTypeDefStrongObjRefSP spIaafTypeDefStrongObjRef );
	virtual ~AxTypeDefStrongObjRef();

private:
	AxTypeDefStrongObjRef();
	AxTypeDefStrongObjRef( const AxTypeDefStrongObjRef& );
	AxTypeDefStrongObjRef& operator=( const AxTypeDefStrongObjRef& );
		
	IAAFTypeDefStrongObjRefSP _spIaafTypeDefStrongObjRef;
};

//=---------------------------------------------------------------------=

class AxTypeDefVariableArrayEx : public AxTypeDef {
public:
	AxTypeDefVariableArrayEx( IAAFTypeDefVariableArrayExSP spIaafTypeDefVariableArrayEx );
	virtual ~AxTypeDefVariableArrayEx();

private:
	AxTypeDefVariableArrayEx();
	AxTypeDefVariableArrayEx( const AxTypeDefVariableArrayEx& );
	AxTypeDefVariableArrayEx& operator=( const AxTypeDefVariableArrayEx& );
		
	IAAFTypeDefVariableArrayExSP _spIaafTypeDefVariableArrayEx;
};

//=---------------------------------------------------------------------=

class AxTypeDefVariableArray : public AxTypeDefVariableArrayEx {
public:
	AxTypeDefVariableArray( IAAFTypeDefVariableArraySP spIaafTypeDefVariableArray );
	virtual ~AxTypeDefVariableArray();

	IAAFTypeDefSP GetType();

	aafUInt32 GetCount( IAAFPropertyValueSP& spPropVal );

	IEnumAAFPropertyValuesSP GetElements( IAAFPropertyValueSP& spPropVal );

private:
	AxTypeDefVariableArray();
	AxTypeDefVariableArray( const AxTypeDefVariableArray& );
	AxTypeDefVariableArray& operator=( const AxTypeDefVariableArray& );
		
	IAAFTypeDefVariableArraySP _spIaafTypeDefVariableArray;
};

#endif
