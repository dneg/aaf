#ifndef __AxDefObject_h__
#define __AxDefObject_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxBaseObj.h"
#include "AxObject.h"

class AxDefObject : public AxObject {
public:
	AxDefObject( IAAFDefObjectSP spIaafDefObject );
	~AxDefObject();

	aafUID_t GetAUID();

	AxString GetName();

	AxString GetDescription();

	inline operator IAAFDefObjectSP ()
	{ return _spIaafDefObject; }

private:
	AxDefObject();
	AxDefObject ( const AxDefObject& );
	AxDefObject & operator=( const AxDefObject & );
	
	IAAFDefObjectSP _spIaafDefObject;
};

//=---------------------------------------------------------------------=

class AxContainerDef : public AxDefObject {
public:
	AxContainerDef( IAAFContainerDefSP spIaafContainerDef );
	~AxContainerDef();

	void Initialize( const aafUID_t& uid,
			 const AxString& name,
			 const AxString& desc );

	inline operator IAAFContainerDefSP ()
	{ return _spIaafContainerDef; }

private:
	IAAFContainerDefSP _spIaafContainerDef;
};

//=---------------------------------------------------------------------=

class AxInterpolationDef : public AxDefObject {
public:
	AxInterpolationDef( IAAFInterpolationDefSP spIaafInterpolationDef );
	~AxInterpolationDef();

	void Initialize( const aafUID_t& uid,
			 const AxString& name,
			 const AxString& desc );

	inline operator IAAFInterpolationDefSP ()
	{ return _spIaafInterpolationDef; }

private:
	IAAFInterpolationDefSP _spIaafInterpolationDef;
};

//=---------------------------------------------------------------------=

class AxParameterDef : public AxDefObject {
public:
	AxParameterDef( IAAFParameterDefSP spIaafParameterDef );
	~AxParameterDef();

	void Initialize( const aafUID_t& uid,
			 const AxString& name,
			 const AxString& desc,
			 IAAFTypeDefSP spIaafTypeDef );

	IAAFTypeDefSP GetTypeDefinition ();
	void SetDisplayUnits ( const AxString& displayUnits );
	AxString GetDisplayUnits ();
	aafUInt32 GetDisplayUnitsBufLen ();

	inline operator IAAFParameterDefSP ()
	{ return _spIaafParameterDef; }

private:
	IAAFParameterDefSP _spIaafParameterDef;
};

//=---------------------------------------------------------------------=

class AxPluginDef : public AxDefObject {
public:
	AxPluginDef( IAAFPluginDefSP spIaafPluginDef );
	~AxPluginDef();

	void Initialize( const aafUID_t& uid,
					 const AxString& name,
					 const AxString& desc );

	void SetCategoryClass( const aafUID_t& uid );
	void SetPluginVersionString( const AxString& ver );
	void SetManufacturerID( const aafUID_t& uid );
	void SetPluginManufacturerName( const AxString& name);
	void SetIsSoftwareOnly( bool isSoftware );
	void SetIsAccelerated( bool isAccel );
	void SetSupportsAuthentication( bool supportsAuth );
	void SetManufacturerInfo( IAAFNetworkLocatorSP manuInfo );

	inline operator IAAFPluginDefSP ()
	{ return _spIaafPluginDef; }

private:
	IAAFPluginDefSP _spIaafPluginDef;
};

//=---------------------------------------------------------------------=

class AxCodecDef : public AxDefObject {
public:

	AxCodecDef( IAAFCodecDefSP spIaafCodecDef );
	~AxCodecDef();

	void Initialize( const aafUID_t& uid,
					 const AxString& name,
					 const AxString& desc );

	void SetFileDescriptorClass( IAAFClassDefSP spClass );

	void AddEssenceKind( IAAFDataDefSP spEssenceKind );

	aafBoolean_t IsEssenceKindSupported( IAAFDataDefSP spIaafDataDef );

	aafUInt32 CountEssenceKinds();

	IEnumAAFDataDefsSP GetEssenceKinds();

	aafBoolean_t AreThereFlavours();

	IEnumAAFCodecFlavoursSP EnumCodecFlavours();

	inline operator IAAFCodecDefSP ()
	{ return _spIaafCodecDef; }

private:
	AxCodecDef();
	AxCodecDef( const AxCodecDef& );
	AxCodecDef& operator=( const AxCodecDef& );

    IAAFCodecDefSP _spIaafCodecDef;
};

//=---------------------------------------------------------------------=

class AxDataDef: public AxDefObject {
public:
	AxDataDef( IAAFDataDefSP spIaafDataDef );
    AxDataDef( IAAFDataDef2SP spIaafDataDef );
    AxDataDef( IAAFDataDef3SP spIaafDataDef );
	~AxDataDef();

    void Initialize( const aafUID_t&,
                const AxString& name,
                const AxString& desc );

	bool IsSoundKind();
	bool IsPictureKind();
    bool IsMatteKind();
    bool IsPictureWithMatteKind();
    bool IsEdgecodeKind();
    bool IsTimecodeKind();
    bool IsAuxiliaryKind();
    bool IsDescriptiveMetadataKind();

    bool DoesDataDefConvertTo ( IAAFDataDefSP );
    bool IsDataDefOf ( IAAFDataDefSP );
    bool DoesDataDefConvertFrom ( IAAFDataDefSP );
		
	inline operator IAAFDataDefSP ()
	{ return AxQueryInterface<IAAFDataDef3,IAAFDataDef>( _spIaafDataDef ); }
    
    inline operator IAAFDataDef2SP ()
    { return AxQueryInterface<IAAFDataDef3,IAAFDataDef2>( _spIaafDataDef ); }
    
    inline operator IAAFDataDef3SP ()
    { return _spIaafDataDef; }

private:
	AxDataDef();
	AxDataDef( const AxDataDef& );
	AxDataDef& operator=( const AxDataDef& );
	
	IAAFDataDef3SP _spIaafDataDef;
};

//=---------------------------------------------------------------------=

class AxOperationDef: public AxDefObject {
public:
	AxOperationDef( IAAFOperationDefSP spIaafOperationDef );
	~AxOperationDef();

	void Initialize( const aafUID_t&,
				const AxString& name,
				const AxString& desc );
	
	void SetDataDef( IAAFDataDefSP );
	void SetIsTimeWarp( aafBoolean_t );
	void SetCategory( const aafUID_t& category_auid );
	void SetNumberInputs( aafInt32 );
	void SetBypass( aafUInt32 );
	
	IAAFDataDefSP GetDataDef();
	aafBoolean_t IsTimeWarp();
	aafUID_t GetCategory();
	aafInt32 GetNumberInputs();
	aafUInt32 GetBypass();
	
	inline operator IAAFOperationDefSP ()
	{ return _spIaafOperationDef; }

private:
	AxOperationDef();
	AxOperationDef( const AxOperationDef& );
	AxOperationDef& operator=( const AxOperationDef& );
	
	IAAFOperationDefSP _spIaafOperationDef;
};

//=---------------------------------------------------------------------=

class AxKLVDataDef: public AxDefObject {
public:
    AxKLVDataDef( IAAFKLVDataDefinitionSP spIaafKLVDataDef );
    ~AxKLVDataDef();

    void Initialize( const aafUID_t&,
                const AxString& name,
                const AxString& desc );
    
    void AddParentProperty( IAAFPropertyDefSP );
    void RemoveParentProperty( IAAFPropertyDefSP );
    void SetKLVDataType( IAAFTypeDefSP );
    
    IEnumAAFPropertyDefsSP GetParentProperties();
    aafUInt32 CountParentProperties();
    IAAFTypeDefSP GetKLVDataType();
    
    inline operator IAAFKLVDataDefinitionSP ()
    { return _spIaafKLVDataDef; }

private:
    AxKLVDataDef();
    AxKLVDataDef( const AxKLVDataDef& );
    AxKLVDataDef& operator=( const AxKLVDataDef& );
    
    IAAFKLVDataDefinitionSP _spIaafKLVDataDef;
};

//=---------------------------------------------------------------------=

class AxTaggedValueDef: public AxDefObject {
public:
    AxTaggedValueDef( IAAFTaggedValueDefinitionSP spIaafTaggedValueDef );
    ~AxTaggedValueDef();

    void Initialize( const aafUID_t&,
                const AxString& name,
                const AxString& desc );
    
    void AddParentProperty( IAAFPropertyDefSP );
    void RemoveParentProperty( IAAFPropertyDefSP );
    
    IEnumAAFPropertyDefsSP GetParentProperties();
    aafUInt32 CountParentProperties();
    
    inline operator IAAFTaggedValueDefinitionSP ()
    { return _spIaafTaggedValueDef; }

private:
    AxTaggedValueDef();
    AxTaggedValueDef( const AxTaggedValueDef& );
    AxTaggedValueDef& operator=( const AxTaggedValueDef& );
    
    IAAFTaggedValueDefinitionSP _spIaafTaggedValueDef;
};

#endif
