#ifndef __AxImplPlugin_h__
#define __AxImplPlugin_h__

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

#include <AxTypes.h>

#include <AAF.h>

#include <vector>

// AxImplPlugin provides generic reusable support for IAAFPlugin that
// can used to instantiate CAxImplPlugin.  CAxImplPlugin implements the
// IAAFPlugin COM interface.

// codecDefID - A unique ID that identifies this codec.
// e.g. kAAFCDCICodec or kAAFWAVECodec

// categoryClassAUID - The plugin provides an implementation of this type of object.
// This is one of: AUID_CodecDefinition, AUID_InterpolationDefinition, etc.
// FIXME - Spell out the "etc" classes.

// manufacturerUID - A UID that identifies the manufacturer.  This is the same for all
// codecs created by a single manufacturer.

// descriptorAUID - This is the AUID of the EssenceDescriptor object that will
// be created by the SDK and attached to the SourceMob that represents the
// essence processed by the codec.

// dataDefID - One of DDEF_Picture, or DDEF_Sound.


class AxImplPlugin
{
public:

	class DescriptorParameters {
	public:

		DescriptorParameters()
		{}

		DescriptorParameters (
			const aafUID_t& codecDefID,
			const AxString& name,
			const AxString& desc,
			const aafUID_t& categoryClassAUID,
			const AxString& version,
			const aafUID_t& manufacturerID,
			const AxString& manufacturerName,
			bool  isSoftwareOnly,
			bool  isAccel,
			bool  supportsAuth,
			const AxString& manufacturerLocatorPath
        )
			: _codecDefID( codecDefID ),
			  _name( name ),
			  _desc( desc ),
			  _categoryClassAUID( categoryClassAUID ),
			  _version( version ),
			  _manufacturerID( manufacturerID ),
			  _manufacturerName( manufacturerName ),
			  _isSoftwareOnly( isSoftwareOnly ),
			  _isAccel( isAccel ),
			  _supportsAuth( supportsAuth ),
			  _manufacturerLocatorPath( manufacturerLocatorPath )
		{}

		aafUID_t GetCodecDefID()              { return _codecDefID;              }
		AxString GetCodecName()               { return _name;                    }
		AxString GetCodecDesc()               { return _desc;                    }
		aafUID_t GetCategoryClassAUID()       { return _categoryClassAUID;       }
		AxString GetVersionString()           { return _version;                 }
		aafUID_t GetManufacturerID()          { return _manufacturerID;          }
		AxString GetManufacturerName()        { return _manufacturerName;        }
		bool     GetIsSoftwareOnly()          { return _isSoftwareOnly;          }
		bool     GetIsAccellerated()          { return _isAccel;                 }
		bool     GetSupportsAuthentication()  { return _supportsAuth;            }
		AxString GetManufacturerLocatorPath() { return _manufacturerLocatorPath; }

	private:
		aafUID_t _codecDefID;
		AxString _name;
		AxString _desc;
		aafUID_t _categoryClassAUID;
		AxString _version;
		aafUID_t _manufacturerID;
		AxString _manufacturerName;
		bool     _isSoftwareOnly;
		bool     _isAccel;
		bool     _supportsAuth;
		AxString _manufacturerLocatorPath;
	};

	class DefinitionParameters {
	public:
		DefinitionParameters();

		DefinitionParameters( const aafUID_t& codecDefID,
							  const aafUID_t& descriptorAUID,
							  const aafUID_t& dataDefID,
		                      const AxString& name,
							  const AxString& desc )
		: _codecDefID( codecDefID ),
		  _descriptorAUID( _descriptorAUID ),
	      _dataDefID( dataDefID ),
	      _name( name ),
	      _desc( desc )
		{}

		aafUID_t GetCodecDefID()            { return _codecDefID; }
		aafUID_t GetEssenceDescriptorAUID() { return _descriptorAUID; }
		aafUID_t GetDataDefID()             { return _dataDefID; }
		AxString GetName()                  { return _name; }
		AxString GetDesc()                  { return _desc; }

	private:
		aafUID_t _codecDefID;     // e.g. AAFCodecCDCI
		aafUID_t _descriptorAUID; // e.g. AUID_AAFCDCIDescriptor
		aafUID_t _dataDefID;      // e.g. DDEF_Picture
		AxString _name;
		AxString _desc;
	};

public:

	AxImplPlugin();
	AxImplPlugin( const aafUID_t& pluginDescAUID );
	~AxImplPlugin();

	// Add one DefinitionParamerters for each IAAFDefObject the
	// codec will create in response to a call to 
	// GetIndexedDefinitionObject().
	void AddDefinitionParameters( const DefinitionParameters& params );	

	// Set the DescriptorParameters used to construct a IAAFDefObject
	// in responce to a call to CreateDescriptor.
	void SetDescriptorParameters( const DescriptorParameters& params );

	// This is the ID returned by GetPluginDescriptorID().
	void SetPluginDescriptorAUID( const aafUID_t& pluginDescAUID );

	//
	// IAAFPlugin methods
	//

	void CountDefinitions(
		aafUInt32 *pDefCount );

	void GetIndexedDefinitionID(
		aafUInt32 index, aafUID_t *result );

	void GetPluginDescriptorID(
		aafUID_t *result );

    void GetIndexedDefinitionObject(
		aafUInt32 index,
        IAAFDictionary *dict,
        IAAFDefObject **def );
 
	void CreateDescriptor(
		IAAFDictionary *dict,
		IAAFPluginDef **desc );

private:

	// Prohibited
	AxImplPlugin( const AxImplPlugin& );
	AxImplPlugin& operator=( const AxImplPlugin& );

	aafUID_t _pluginDescriptorAUID;
	std::vector< DefinitionParameters > _definitionParams;
	DescriptorParameters _descriptorParams;
};

#endif