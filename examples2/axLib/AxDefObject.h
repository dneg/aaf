#ifndef __AxDefObject_h__
#define __AxDefObject_h__

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
	~AxDataDef();

	bool IsSoundKind();
	bool IsPictureKind();
		
	inline operator IAAFDataDefSP ()
	{ return _spIaafDataDef; }

private:
	AxDataDef();
	AxDataDef( const AxDataDef& );
	AxDataDef& operator=( const AxDataDef& );
	
	IAAFDataDefSP _spIaafDataDef;
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

#endif
