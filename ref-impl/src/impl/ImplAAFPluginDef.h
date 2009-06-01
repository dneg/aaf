//@doc
//@class    AAFPluginDescriptor | Implementation class for AAFPluginDescriptor
#ifndef __ImplAAFPluginDef_h__
#define __ImplAAFPluginDef_h__


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
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

template <class T> 
class ImplAAFEnumerator;
class ImplAAFLocator;
typedef ImplAAFEnumerator<ImplAAFLocator> ImplEnumAAFPluginLocators;

#include "OMStrongRefProperty.h"
#include "OMStrongRefVectorProperty.h"
#include "OMWideStringProperty.h"

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#include "ImplAAFLocator.h"
#include "ImplAAFNetworkLocator.h"

class ImplAAFPluginDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFPluginDef ();

protected:
  virtual ~ImplAAFPluginDef ();

public:
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        // @parm [in] Pointer to an AUID reference
        (const aafUID_t & pAuid,
		 const aafCharacter * name,
		 const aafCharacter * description);

  //****************
  // GetCategoryClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCategoryClass
        // @parm [out] The CategoryClass
        (aafUID_t *  pCategoryClass);


  //****************
  // SetCategoryClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCategoryClass
        // @parm [in] The Category Class
        (const aafUID_t & categoryClass);


  //****************
  // GetPluginVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginVersion
        // @parm [out] The Plugin Version
        (aafVersionType_t *  pVersion);


  //****************
  // SetPluginVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPluginVersion
        (aafVersionType_t *  pVersion);


  //****************
  // GetPluginVersionString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginVersionString
        (// @parm [in,string] Plugin version string
         aafCharacter *  pVersionString,

         // @parm [in] length of the buffer to hold plugin version string
         aafUInt32  bufSize);


  //****************
  // GetProductVersionStringLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginVersionStringBufLen
        // @parm [out] Mob Name
        (aafUInt32 *  pLen);


  //****************
  // SetPluginVersionString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPluginVersionString
        // @parm [in, string] Plugin version
        (const aafCharacter * pVersionString);


  //****************
  // GetPluginManufacturerName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginManufacturerName
        (// @parm [in,string] Plugin Manufacturer Name
         aafCharacter *  pManufacturerName,

         // @parm [in] length of the buffer to hold plugin Manufacturer Name
         aafUInt32  bufSize);


  //****************
  // GetPluginManufacturerNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginManufacturerNameBufLen
        // @parm [out] Manufacturer Name
        (aafUInt32 *  pLen);


  //****************
  // SetPluginManufacturerName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPluginManufacturerName
        // @parm [in, string] Plugin Manufacturer
        (const aafCharacter * pManufacturerName);


  //****************
  // GetManufacturerInfo()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetManufacturerInfo
        // @parm [out,retval] ManufacturerInfo property value
        (ImplAAFNetworkLocator ** ppResult);


  //****************
  // SetManufacturerInfo()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetManufacturerInfo
        // @parm [in] ManufacturerInfo property value
        (ImplAAFNetworkLocator * pManufacturerInfo);


  //****************
  // GetManufacturerID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetManufacturerID
        // @parm [out] The ManufacturerID
        (aafUID_t *  pManufacturerID);


  //****************
  // SetManufacturerID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetManufacturerID
        // @parm [in] The Category Class
        (const aafUID_t & manufacturerID);


  //****************
  // GetHardwarePlatform()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetHardwarePlatform
        // @parm [out] The HardwarePlatform
        (aafHardwarePlatform_t *  pHardwarePlatform);


  //****************
  // SetHardwarePlatform()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetHardwarePlatform
        // @parm [in] The Category Class
        (aafHardwarePlatform_constref  hardwarePlatform);


  //****************
  // GetPlatformVersionRange()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPlatformVersionRange
        (// @parm [out] The Minimum Platform Version
         aafVersionType_t *  pMinVersion,

         // @parm [out] The Maximum Platform Version
         aafVersionType_t *  pMaxVersion);


  //****************
  // SetPlatformMinimumVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPlatformMinimumVersion
        // @parm [in] The Minimum Platform Version
        (const aafVersionType_t & minVersion);


  //****************
  // SetPlatformMaximumVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPlatformMaximumVersion
        // @parm [in] The Maximum Platform Version
        (const aafVersionType_t & maxVersion);


  //****************
  // GetEngine()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEngine
        // @parm [out] The software engine
        (aafEngine_t *  pEngine);


  //****************
  // SetEngine()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEngine
        // @parm [in] The software engine
        (aafEngine_constref engine);


  //****************
  // GetEngineVersionRange()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEngineVersionRange
        (// @parm [out] The Minimum Engine Version
         aafVersionType_t *  pMinVersion,

         // @parm [out] The Maximum Engine Version
         aafVersionType_t *  pMaxVersion);


  //****************
  // SetEngineMinimumVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEngineMinimumVersion
        // @parm [in] The Minimum Engine Version
        (const aafVersionType_t & minVersion);


  //****************
  // SetEngineMaximumVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEngineMaximumVersion
        // @parm [in] The Minimum Engine Version
        (const aafVersionType_t & maxVersion);


  //****************
  // GetPluginAPI()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginAPI
        // @parm [out] The PluginAPI
        (aafPluginAPI_t *  pPluginAPI);


  //****************
  // SetPluginAPI()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPluginAPI
        // @parm [in] The Category Class
        (aafPluginAPI_constref  pluginAPI);


  //****************
  // GetPluginAPIVersionRange()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginAPIVersionRange
        (// @parm [out] The Minimum Plugin API Version
         aafVersionType_t *  pMinVersion,

         // @parm [out] The Maximum Plugin API Version
         aafVersionType_t *  pMaxVersion);


  //****************
  // SetPluginAPIMinimumVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPluginAPIMinimumVersion
        // @parm [out] The Minimum Plugin API Version
        (const aafVersionType_t & minVersion);


  //****************
  // SetPluginAPIMaximumVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPluginAPIMaximumVersion
        // @parm [in] The Maximum Plugin API Version
        (const aafVersionType_t & maxVersion);


  //****************
  // IsSoftwareOnly()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsSoftwareOnly
        // @parm [out] The IsSoftwareOnly
        (aafBool *  pIsSoftwareOnly);



  //****************
  // SetIsSoftwareOnly()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsSoftwareOnly
        // @parm [in] The isSoftwareOnly flag
        (aafBool  isSoftwareOnly);



  //****************
  // IsAccelerated()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsAccelerated
        // @parm [out] The IsAccelerated
        (aafBool *  pIsAccelerated);




  //****************
  // SetIsAccelerated()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsAccelerated
        // @parm [in] The isAccelerated flag
        (aafBool  isAccelerated);


  //****************
  // SupportsAuthentication()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SupportsAuthentication
        // @parm [out] The SupportsAuthentication
        (aafBool *  pSupportsAuthentication);


  //****************
  // SetSupportsAuthentication()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSupportsAuthentication
        // @parm [in] The SupportsAuthentication flag
        (aafBool  SupportsAuthentication);


  //****************
  // CountLocators()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountLocators
        // @parm [out] Returns the number of locators
        (aafUInt32 *  pCount);


  //****************
  // AppendLocator()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendLocator
        // @parm [in] Locator to append
        (ImplAAFLocator * pLocator);


  //****************
  // PrependLocator()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependLocator
        // @parm [in] Locator to append
        (ImplAAFLocator * pLocator);


  //****************
  // InsertLocatorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertLocatorAt
        // @parm [in] index to place new locator
        (aafUInt32 index,

        // @parm [in] Locator to insert
		ImplAAFLocator * pLocator);


  //****************
  // GetLocatorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLocatorAt
        // @parm [in] index to place new locator
        (aafUInt32 index,

        // @parm [out, retval] returned Locator
		ImplAAFLocator ** ppLocator);


  //****************
  // RemoveLocatorAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveLocatorAt
        // @parm [in] index of locator to remove
        (aafUInt32 index);



  //****************
  // GetPluginDescriptorID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginDescriptorID
        // @parm [out] The unique plugin descriptor id
        (aafUID_t *  pDescriptorID);


  //****************
  // SetPluginDescriptorID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPluginDescriptorID
        // @parm [in] New Mob ID
        (aafUID_t  descriptorID);
  

  //****************
  // GetLocators()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLocators
        // @parm [out, retval] Plugin Locator Enumeration
        (ImplEnumAAFPluginLocators ** ppEnum);

  //****************
  // SetDefinitionObject()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDefinitionObjectID
        (aafUID_t def);  //@parm [in] Def of this object


  //****************
  // GetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
   GetDefinitionObjectID
        (aafUID_t *pDef);  //@parm [retval][out] Def of this object


public:
	virtual AAFRESULT
		GetNthLocator (aafInt32 index, ImplAAFLocator **ppLocator);
  //	virtual AAFRESULT
  //		 GetNumLocators (aafInt32 *  pCount);

private:
	OMFixedSizeProperty<aafUID_t>					_categoryClass;
	OMFixedSizeProperty<aafVersionType_t>			_pluginVersion;
	OMWideStringProperty                            _pluginVersionString;
	OMWideStringProperty                            _pluginManufacturerName;
	OMStrongReferenceProperty<ImplAAFNetworkLocator> _manufacturerURL;
	OMFixedSizeProperty<aafUID_t>					_pluginManufacturerID;
	OMFixedSizeProperty<aafUID_t>					_platform;
	OMFixedSizeProperty<aafVersionType_t>			_minPlatformVersion;
	OMFixedSizeProperty<aafVersionType_t>			_maxPlatformVersion;
	OMFixedSizeProperty<aafUID_t>					_engine;
	OMFixedSizeProperty<aafVersionType_t>			_minEngineVersion;
	OMFixedSizeProperty<aafVersionType_t>			_maxEngineVersion;
	OMFixedSizeProperty<aafUID_t>					_pluginAPI;
	OMFixedSizeProperty<aafVersionType_t>			_minPluginAPIVersion;
	OMFixedSizeProperty<aafVersionType_t>			_maxPluginAPIVersion;
	OMFixedSizeProperty<aafBool>					_softwareOnly;
	OMFixedSizeProperty<aafBool>					_accelerator;
    OMStrongReferenceVectorProperty<ImplAAFLocator> _locators;
	OMFixedSizeProperty<aafBool>					_authentication;
	OMFixedSizeProperty<aafUID_t>					_defObj;
};

#endif // ! __ImplAAFPluginDef_h__


