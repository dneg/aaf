//@doc
//@class    AAFPluginDef | Implementation class for AAFPluginDef
#ifndef __CAAFPluginDef_h__
#define __CAAFPluginDef_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Initial Developer of the Original Code of this file and the 
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif








#ifndef __CAAFDefObject_h__
#include "CAAFDefObject.h"
#endif


class CAAFPluginDef
  : public IAAFPluginDef,
    public CAAFDefObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFPluginDef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFPluginDef ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Init all fields of a definition object.
  //
  STDMETHOD (Initialize) (
    // AUID for new DeObject
    /*[in, ref]*/ aafUID_constref  id,

    // Name for new DefObject
    /*[in]*/ aafCharacter_constptr  pName,

    // Description for new DefObject
    /*[in]*/ aafCharacter_constptr  pDescription);

  //***********************************************************
  //
  // GetCategoryClass()
  //
  // Obtains the Category Class, which is identifies the stored
  // classID of the subclass of AAFPluggableDefinition which
  // references this plugin descriptor.  This ID is written into the
  // caller-allocated aafUID_t specified by the pCategoryClass
  // argument.
  // 
  // Succeeds if all of the following are true:
  // - the pCategoryClass pointer is valid.
  // 
  // If this method fails nothing will be written to *pCategoryClass.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pCategoryClass arg is NULL.
  //
  STDMETHOD (GetCategoryClass) (
    // The CategoryClass 
    /*[out]*/ aafUID_t *  pCategoryClass);


  //***********************************************************
  //
  // SetCategoryClass()
  //
  // Sets the Category Class, which is identifies the stored classID
  // of the subclass of AAFPluggableDefinition which references this
  // plugin descriptor.
  // 
  // If this method fails the Category Class property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetCategoryClass) (
    // The Category Class 
    /*[in, ref]*/ aafUID_constref  categoryClass);


  //***********************************************************
  //
  // GetPluginVersion()
  //
  // Gets the Plugin Version property associated with this plugin
  // descriptor and places it into *pVersion. 
  //
  // Succeeds if all of the following are true:
  // - the pVersion pointer is valid.
  // 
  // If this method fails, nothing will be written to *pVersion.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pVersion arg is NULL.
  //
  STDMETHOD (GetPluginVersion) (
    // The Plugin Version 
    /*[out]*/ aafVersionType_t *  pVersion);


  //***********************************************************
  //
  // SetPluginVersion()
  //
  // Sets the plugin version property of this plugin descriptor.
  //
  // Succeeds if all of the following are true:
  // - pVersion pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pVersion arg is NULL.
  //
  STDMETHOD (SetPluginVersion) (
    // The plugin Version 
    /*[in]*/ aafVersionType_t *  pVersion);


  //***********************************************************
  //
  // SetPluginVersionString()
  //
  // Sets the Plugin Version String property.
  //
  // Set the PluginVersionString property to the value specified in
  // pPluginVersionString.  A copy is made of the data so the caller
  // retains ownership of the *pPluginVersionString buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pPluginVersionString pointer is valid.
  // 
  // If this method fails the PluginVersionString property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPluginVersionString arg is NULL.
  //
  STDMETHOD (SetPluginVersionString) (
    // buffer from which PluginVersionString is to be read 
    /*[in, string]*/ aafCharacter_constptr  pPluginVersionString);


  //***********************************************************
  //
  // GetPluginVersionString()
  //
  // Gets the Plugin Version String property.
  // 
  // Writes the PluginVersionString property, with a trailing null
  // character, into the pPluginVersionString buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the PluginVersionString property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetPluginVersionStringBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pPluginVersionString.
  // 
  // Succeeds if:
  // - The pPluginVersionString pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   PluginVersionString.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPluginVersionString arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold PluginVersionString.
  //
  STDMETHOD (GetPluginVersionString) (
    // buffer into which PluginVersionString is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pPluginVersionString,

    // size of *pPluginVersionString buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetPluginVersionStringBufLen()
  //
  // Returns size of buffer (in bytes) required for GetPluginVersionString().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetPluginVersionStringBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // SetPluginManufacturerName()
  //
  // Sets the Plugin Manufacturer Name string property.
  //
  // Set the PluginManufacturerName property to the value specified in
  // pPluginManufacturerName.  A copy is made of the data so the caller
  // retains ownership of the *pPluginManufacturerName buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pPluginManufacturerName pointer is valid.
  // 
  // If this method fails the PluginManufacturerName property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPluginManufacturerName arg is NULL.
  //
  STDMETHOD (SetPluginManufacturerName) (
    // buffer from which PluginManufacturerName is to be read 
    /*[in, string]*/ aafCharacter_constptr  pPluginManufacturerName);


  //***********************************************************
  //
  // GetPluginManufacturerName()
  //
  // Gets the Plugin Manufacturer Name string property.
  // 
  // Writes the PluginManufacturerName property, with a trailing null
  // character, into the pPluginManufacturerName buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the PluginManufacturerName property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetPluginManufacturerNameBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pPluginManufacturerName.
  // 
  // Succeeds if:
  // - The pPluginManufacturerName pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   PluginManufacturerName.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPluginManufacturerName arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold PluginManufacturerName.
  //
  STDMETHOD (GetPluginManufacturerName) (
    // buffer into which PluginManufacturerName is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pPluginManufacturerName,

    // size of *pPluginManufacturerName buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetPluginManufacturerNameBufLen()
  //
  // Returns size of buffer (in bytes) required for GetPluginManufacturerName().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetPluginManufacturerNameBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // GetManufacturerInfo()
  //
  // This method will get an AAFNetworkLocator pointing to
  // ManufacturerInfo for this plugin and place an interface for the
  // locator into the **ppResult argument.  If a ManufacturerInfo
  // locator exists, the result will be AddRef()ed. If not, the
  // result will be NULL.
  //
  // Succeeds if all of the following are true:
  // - the ppResult pointer is valid.
  // 
  // If this method fails nothing will be written to *ppResult.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppResult arg is NULL.
  //
  STDMETHOD (GetManufacturerInfo) (
    // ManufacturerInfo property value 
    /*[out, retval]*/ IAAFNetworkLocator ** ppResult);


  //***********************************************************
  //
  // SetManufacturerInfo()
  //
  // This method will set a locator pointing to the location of
  // ManufacturerInfo for this plugin.  If a ManufacturerInfo
  // already exists for this mob slot, it will be discarded.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetManufacturerInfo) (
    // ManufacturerInfo property value 
    /*[in]*/ IAAFNetworkLocator * pManufacturerInfo);


  //***********************************************************
  //
  // GetManufacturerID()
  //
  // Obtains the manufacturer ID, which is identifies the manfacturer
  // of this plugin.  This ID is written into the caller-allocated
  // aafUID_t specified by the pManufacturerID argument.
  // 
  // Succeeds if all of the following are true:
  // - the pManufacturerID pointer is valid.
  // 
  // If this method fails nothing will be written to
  // *pManufacturerID.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pManufacturerID arg is NULL.
  //
  STDMETHOD (GetManufacturerID) (
    // The ManufacturerID 
    /*[out]*/ aafUID_t *  pManufacturerID);


  //***********************************************************
  //
  // SetManufacturerID()
  //
  // Sets the manufacturer ID, which is identifies the manufacturer
  // of this plugin.
  // 
  // If this method fails the ManufacturerID property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetManufacturerID) (
    // The Category Class 
    /*[in, ref]*/ aafUID_constref  manufacturerID);


  //***********************************************************
  //
  // GetHardwarePlatform()
  //
  // Obtains the hardware platform ID, which identifies the hardware
  // platform which is required to use this plugin.  This ID is
  // written into the caller-allocated variable specified by the
  // pHardwarePlatform argument. The type aafHardwarePlatform_t is an
  // extensible enumerated type, and the value given must be either
  // standard or in the type dictionary.
  // 
  // Succeeds if all of the following are true:
  // - the pHardwarePlatform pointer is valid.
  // 
  // If this method fails nothing will be written to
  // *pHardwarePlatform.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pHardwarePlatform arg is NULL.
  //
  STDMETHOD (GetHardwarePlatform) (
    // The HardwarePlatform 
    /*[out]*/ aafHardwarePlatform_t *  pHardwarePlatform);


  //***********************************************************
  //
  // SetHardwarePlatform()
  //
  // Sets the hardware platform ID, which identifies the hardware
  // platform which is required to use this plugin.  The type
  // aafHardwarePlatform_t is an extensible enumerated type, and the
  // value given must be either standard or in the type dictionary.
  // 
  // If this method fails the HardwarePlatform property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetHardwarePlatform) (
    // The Category Class 
    /*[in]*/ aafHardwarePlatform_constref  hardwarePlatform);


  //***********************************************************
  //
  // GetPlatformVersionRange()
  //
  // Gets the minimum and maximum platform Version properties
  // associated with this plugin descriptor and places it into
  // *pMinVersion, and *pMaxVersion.  These are the minimum and
  // maximum versions of the platform for which this plugin will
  // function.
  //
  // Succeeds if all of the following are true:
  // - both pMinVersion and pMaxVersion pointers are valid.
  // 
  // If this method fails, nothing will be written to *pMinVersion or
  // *pMaxVersion.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pMinVersion or pMaxVersion arg is NULL.
  //
  STDMETHOD (GetPlatformVersionRange) (
    // The Minimum Platform Version
    /*[out]*/ aafVersionType_t *  pMinVersion,

    // The Maximum Platform Version
    /*[out]*/ aafVersionType_t *  pMaxVersion);


  //***********************************************************
  //
  // SetPlatformMinimumVersion()
  //
  // Sets the maximum platform Version property of this plugin
  // descriptor.  This is the minimum version of the platform for
  // which this plugin will function.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetPlatformMinimumVersion) (
    // The Minimum Platform Version 
    /*[in]*/ aafVersionType_constref  minVersion);


  //***********************************************************
  //
  // SetPlatformMaximumVersion()
  //
  // Sets the maximum platform Version properties associated with this
  // plugin descriptor.  This is the maximum version of the platform
  // for which this plugin will function.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetPlatformMaximumVersion) (
    // The Maximum Platform Version 
    /*[in]*/ aafVersionType_constref  maxVersion);


  //***********************************************************
  //
  // GetEngine()
  //
  // Obtains the software engine ID, which identifies the software
  // subsystem used for essence management and playback used by the
  // plugin.  This ID is written into the caller-allocated variable
  // specified by the pHardwarePlatform argument.  The type
  // aafEngine_t is an extensible, enumerated type, and the value
  // given must be either standard or in the type dictionary.
  // 
  // Succeeds if all of the following are true:
  // - the pHardwarePlatform pointer is valid.
  // 
  // If this method fails nothing will be written to
  // *pHardwarePlatform.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pEngine arg is NULL.
  //
  STDMETHOD (GetEngine) (
    // The software engine 
    /*[out]*/ aafEngine_t *  pEngine);


  //***********************************************************
  //
  // SetEngine()
  //
  // Sets the software engine ID, which identifies the software
  // subsystem used for essence management and playback used by the
  // plugin.  The type aafEngine_t is an extensible enumerated type,
  // and the value given must be either standard or in the type
  // dictionary.
  // 
  // If this method fails the engine property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetEngine) (
    // The software engine 
    /*[in]*/ aafEngine_constref  engine);


  //***********************************************************
  //
  // GetEngineVersionRange()
  //
  // Gets the minimum and maximum engine Version properties of the
  // engine which is associated with this plugin and places it into
  // *pMinVersion, and *pMaxVersion.  These are the minimum and
  // maximum versions of the engine for which this plugin will
  // function.
  //
  // Succeeds if all of the following are true:
  // - both pMinVersion and pMaxVersion pointers are valid.
  // 
  // If this method fails, nothing will be written to *pMinVersion or
  // *pMaxVersion.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pMinVersion or pMaxVersion arg is NULL.
  //
  STDMETHOD (GetEngineVersionRange) (
    // The Minimum Engine Version
    /*[out]*/ aafVersionType_t *  pMinVersion,

    // The Maximum Engine Version
    /*[out]*/ aafVersionType_t *  pMaxVersion);


  //***********************************************************
  //
  // SetEngineMinimumVersion()
  //
  // Sets the minimum engine Version property to the minimum useful
  // version of the engine which is associated with this plugin.  This
  // is the minimum version of the engine for which this plugin will
  // function.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetEngineMinimumVersion) (
    // The Minimum Engine Version 
    /*[in]*/ aafVersionType_constref  minVersion);


  //***********************************************************
  //
  // SetEngineMaximumVersion()
  //
  // Sets the minimum engine Version property to the maximum useful
  // version of the engine which is associated with this plugin.  This
  // is the maximum version of the engine for which this plugin will
  // function.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetEngineMaximumVersion) (
    // The Minimum Engine Version 
    /*[in]*/ aafVersionType_constref  pMaxVersion);


  //***********************************************************
  //
  // GetPluginAPI()
  //
  // Obtains the manufacturer ID Class, which identifies the plugin
  // interface supported by the plugin.  This ID is written into the
  // caller-allocated variable specified by the pPluginAPI argument.
  // The type aafPluginAPI_t is an extensible enumerated type, and
  // the value given must be either standard or in the type
  // dictionary.
  // 
  // Succeeds if all of the following are true:
  // - the pPluginAPI pointer is valid.
  // 
  // If this method fails nothing will be written to *pPluginAPI.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pPluginAPI arg is NULL.
  //
  STDMETHOD (GetPluginAPI) (
    // The PluginAPI 
    /*[out]*/ aafPluginAPI_t *  pPluginAPI);


  //***********************************************************
  //
  // SetPluginAPI()
  //
  // Obtains the manufacturer ID, which identifies the plugin
  // interface supported by the plugin.
  // 
  // The type aafPluginAPI_t is an extensible enumerated type, and
  // the value given must be either standard or in the type dictionary.
  // 
  // If this method fails the PluginAPI property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetPluginAPI) (
    // The Category Class 
    /*[in]*/ aafPluginAPI_constref  pluginAPI);


  //***********************************************************
  //
  // GetPluginAPIVersionRange()
  //
  // Gets the minimum and maximum plugin API Version properties of the
  // plugin API which is associated with this plugin descriptor and
  // places it into *pMinVersion, and *pMaxVersion.  These are the
  // minimum and maximum versions of the PluginAPI for which this
  // plugin will function.
  //
  // Succeeds if all of the following are true:
  // - both pMinVersion and pMaxVersion pointers are valid.
  // 
  // If this method fails, nothing will be written to *pMinVersion or
  // *pMaxVersion.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pMinVersion or pMaxVersion arg is NULL.
  //
  STDMETHOD (GetPluginAPIVersionRange) (
    // The Minimum Plugin API Version
    /*[out]*/ aafVersionType_t *  pMinVersion,

    // The Maximum Plugin API Version
    /*[out]*/ aafVersionType_t *  pMaxVersion);


  //***********************************************************
  //
  // SetPluginAPIMinimumVersion()
  //
  // Sets the minimum plugin API Version property to the minimum
  // useful version of the plugin API which is associated with this
  // plugin.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetPluginAPIMinimumVersion) (
    // The Minimum Plugin API Version 
    /*[out]*/ aafVersionType_constref  minVersion);


  //***********************************************************
  //
  // SetPluginAPIMaximumVersion()
  //
  // Sets the maximum engine Version property to the maximum useful
  // version of the plugin API which is associated with this plugin.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetPluginAPIMaximumVersion) (
    // The Maximum Plugin API Version 
    /*[in]*/ aafVersionType_constref  maxVersion);


  //***********************************************************
  //
  // IsSoftwareOnly()
  //
  // Tells whether the given plugin is capable of running in a
  // software-only environment.  A value of AAFTrue indicates that no
  // additional hardware is required.  If the result is AAFTrue, then
  // this plugin may also support hardware acceleration, as long as
  // it also contains a software method of processing the data.
  // 
  // Succeeds if all of the following are true:
  // - the pIsSoftwareOnly pointer is valid.
  // 
  // If this method fails nothing will be written to *pIsSoftwareOnly.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pIsSoftwareOnly arg is NULL.
  //
  STDMETHOD (IsSoftwareOnly) (
    // The IsSoftwareOnly 
    /*[out]*/ aafBoolean_t *  pIsSoftwareOnly);


  //***********************************************************
  //
  // SetIsSoftwareOnly()
  //
  // Sets whether the given plugin is capable of running in a
  // software-only environment, and returns AAFFalse if any hardware
  // is required.  If isSoftwareOnly is AAFTrue, then this plugin may
  // also support hardware acceleration, as long as it also contains
  // a software method of processing the data.  If this method fails
  // the isSoftwareOnly property will not be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetIsSoftwareOnly) (
    // The isSoftwareOnly flag 
    /*[in]*/ aafBoolean_t  isSoftwareOnly);


  //***********************************************************
  //
  // IsAccelerated()
  //
  // Tells whether the given plugin is capable of running with a
  // hardware accelerator.  If the result is AAFTrue, then this
  // plugin may also support software decompression.
  // 
  // Succeeds if all of the following are true:
  // - the pIsAccelerated pointer is valid.
  // 
  // If this method fails nothing will be written to *pIsAccelerated.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pIsAccelerated arg is NULL.
  //
  STDMETHOD (IsAccelerated) (
    // The IsAccelerated 
    /*[out]*/ aafBoolean_t *  pIsAccelerated);


  //***********************************************************
  //
  // SetIsAccelerated()
  //
  // Tells whether the given plugin is capable of running with a
  // hardware accelerator.  If the result is AAFTrue, then this
  // plugin may also support software decompression.
  //
  // If this method fails the IsAccelerated property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetIsAccelerated) (
    // The isAccelerated flag 
    /*[in]*/ aafBoolean_t  isAccelerated);


  //***********************************************************
  //
  // SupportsAuthentication()
  //
  // Tells whether the given plugin is capable of supporting
  // authentication.  The methods for authenticating a plugin are
  // still <tbd>.
  // 
  // Succeeds if all of the following are true:
  // - the pSupportsAuthentication pointer is valid.
  // 
  // If this method fails nothing will be written to
  // *pSupportsAuthentication.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSupportsAuthentication arg is NULL.
  //
  STDMETHOD (SupportsAuthentication) (
    // The SupportsAuthentication 
    /*[out]*/ aafBoolean_t *  pSupportsAuthentication);


  //***********************************************************
  //
  // SetSupportsAuthentication()
  //
  // Tells whether the given plugin is capable of supporting
  // authentication.  The methods for authenticating a plugin are
  // still <tbd>.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetSupportsAuthentication) (
    // The SupportsAuthentication flag 
    /*[in]*/ aafBoolean_t  SupportsAuthentication);



  //***********************************************************
  //
  // CountLocators()
  //
  // Return the number of locators attached to this plugin descriptor.
  // The number of locators may be zero if the plugin is in the
  // current file.
  // 
  // Succeeds if all of the following are true:
  // - the pCount pointer is valid.
  // 
  // If this method fails nothing is written to *pCount.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCount is null.
  //
  STDMETHOD (CountLocators) (
    // Returns the number of locators 
    /*[out]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // AppendLocator()
  //
  // Append another locator to this plugin descriptor.  Use this
  // function to add a locator to be scanned last when searching for
  // the plugin (a secondary location for the plugin).
  // 
  // Succeeds if all of the following are true:
  // - the pLocator pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLocator is null.
  //
  STDMETHOD (AppendLocator) (
    // Locator to append 
    /*[in]*/ IAAFLocator * pLocator);


  //***********************************************************
  //
  // PrependLocator()
  //
  // Append another locator to this plugin descriptor.  Use this
  // function to add a locator to be scanned first when searching for
  // the plugin (a new primary location for the plugin).
  // 
  // Succeeds if all of the following are true:
  // - the pLocator pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLocator is null.
  //
  STDMETHOD (PrependLocator) (
    // Locator to append 
    /*[in]*/ IAAFLocator * pLocator);


  //***********************************************************
  //
  // InsertLocatorAt()
  //
  // Insert another locator to this plugin descriptor at the given
  // index.  Locators already existing at the named and higher indices
  // will be moved up to the next higher index to accommodate.
  // 
  // Succeeds if all of the following are true:
  // - index is less than or equal to the value returned by
  //   CountLocators().
  // - the pLocator pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLocator is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than the value returned by
  //     CountLocators().
  //
  STDMETHOD (InsertLocatorAt) (
    // index to insert locator
    /*[in]*/ aafUInt32  index,

    // Locator to insert
    /*[in]*/ IAAFLocator * pLocator);


  //***********************************************************
  //
  // GetLocatorAt()
  //
  // Gets the locator which exists in this plugin descriptor at the
  // given index.
  // 
  // Succeeds if all of the following are true:
  // - index is less than the value returned by CountLocators().
  // - the ppLocator pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppLocator is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than or equal to the value returned by
  //     CountLocators().
  //
  STDMETHOD (GetLocatorAt) (
    // index to insert locator
    /*[in]*/ aafUInt32  index,

    // returned Locator
    /*[out]*/ IAAFLocator ** ppLocator);


  //***********************************************************
  //
  // RemoveLocatorAt()
  //
  // Removes the locator which exists in this plugin descriptor at the
  // given index.  Locators already existing at indices higher than
  // the named index will be moved down to the next higher index to
  // accommodate.
  // 
  // Succeeds if all of the following are true:
  // - index is less than the value returned by CountLocators().
  // - the ppLocator pointer is valid.
  // 
  // If this method fails no state will be changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppLocator is null.
  //
  // AAFRESULT_BADINDEX
  //   - index is greater than or equal to the value returned by
  //     CountLocators().
  //
  STDMETHOD (RemoveLocatorAt) (
    // index to insert locator 
    /*[in]*/ aafUInt32  index);



  //***********************************************************
  //
  // GetLocators()
  //
  // Places an IEnumAAFPluginLocators enumerator for the plugin
  // locators contained in the AAFPluginDescriptor into the *ppEnum
  // argument.
  // 
  // The returned enumerator is AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - this object has already been initialized.
  // - the ppEnum pointer is valid.
  // 
  // If this method fails nothing will be written to *ppEnum.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEnum is null.
  //
  // E_FAIL
  //   - Failed to create the enumerator.
  //
  STDMETHOD (GetLocators) (
    // Plugin Locator Enumeration 
    /*[out, retval]*/ IEnumAAFPluginLocators ** ppEnum);

    //***********************************************************
  //
  // SetDefinitionObjectID()
  //
  // Sets the definition object handled by this plugin.
  //
  STDMETHOD (SetDefinitionObjectID) (
    // AUID of an AAFDefinitionObject 
    /*[in]*/ aafUID_t  pDef);

    //***********************************************************
  //
  // GetDefinitionObjectID()
  //
  // Gets the definition object handled by this plugin.
  //
  STDMETHOD (GetDefinitionObjectID) (
    // AUID of an AAFDefinitionObject 
    /*[retval,out]*/ aafUID_t*  ppDef);


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFPluginDef_h__



