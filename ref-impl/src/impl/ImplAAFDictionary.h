//@doc
//@class    AAFDictionary | Implementation class for AAFDictionary
#ifndef __ImplAAFDictionary_h__
#define __ImplAAFDictionary_h__

/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/


class ImplAAFBuiltins;

class ImplAAFClassDef;

class ImplEnumAAFClassDefs;

class ImplAAFTypeDef;

class ImplEnumAAFTypeDefs;

class ImplAAFDataDef;

class ImplEnumAAFDataDefs;

class ImplAAFEffectDef;

class ImplEnumAAFEffectDefs;

class ImplEnumAAFPluggableDefs;

class ImplAAFCodecDef;

class ImplEnumAAFCodecDefs;

class ImplEnumAAFContainerDefs;

class ImplAAFContainerDef;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMClassFactory.h"


#include "ImplAAFClassDef.h"
#include "ImplAAFEffectDef.h"
#include "ImplAAFDefObject.h"
#include "ImplAAFCodecDef.h"
#include "ImplAAFContainerDef.h"
#include "ImplAAFTypeDef.h"

class ImplAAFDictionary :
  public OMClassFactory, 
  public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDictionary ();

protected:
  virtual ~ImplAAFDictionary ();

public:
  // Create an instance of the appropriate derived class, given the class id.
  //  This method implements the OMClassFactory interface.
  //
  OMStorable* create(const OMClassId& classId) const;


  //****************
  // CreateInstance()
  //
  // Creates a single uninitialized AAF object of the class associated 
  // with a specified stored object id.
  virtual AAFRESULT STDMETHODCALLTYPE 
  CreateInstance (
    // Class identifier (AUID) of the stored object. This is the
    // corresponding SMPTE identifier (as a GUID) for all predefined
    // built-in classes.
    aafUID_t * pAUID,

    // Address of output variable that receives the 
    // object pointer requested in pAUID
    ImplAAFObject ** ppvObject);


  //****************
  // LookupClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupClass
        (// @parm [in,ref] Class Unique ID
         aafUID_t *  pClassID,

         // @parm [out,retval] Class Definition
         ImplAAFClassDef ** ppClassDef);


  //****************
  // RegisterClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterClass
        (// @parm [in] Class Definition
         ImplAAFClassDef * pClassDef);


  //****************
  // GetClassDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetClassDefinitions
        // @parm [out,retval] Class Definition Enumeration
        (ImplEnumAAFClassDefs ** ppEnum);


  //****************
  // RegisterType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterType
        (// @parm [in] Type Definition Object
         ImplAAFTypeDef * pTypeDef);

  //****************
  // LookupType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupType
        (// @parm [in,ref] Type Unique ID
         aafUID_t *  pTypeID,

         // @parm [out,retval] Type Definition Object
         ImplAAFTypeDef ** ppTypeDef);

  //****************
  // GetTypeDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinitions
        // @parm [out,retval] Type Def Enumeration
        (ImplEnumAAFTypeDefs ** ppEnum);


  //****************
  // RegisterDataDefintion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterDataDefintion
        // @parm [in] Data Definition Object
        (ImplAAFDataDef * pDataDef);

  //****************
  // LookupDataDefintion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupDataDefintion
        (// @parm [in,ref] Datak Defintion Unique ID
         aafUID_t *  pDataDefintionID,

         // @parm [out,retval] Data Definition Object
         ImplAAFDataDef ** ppDataDef);

  //****************
  // GetDataDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDefinitions
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFDataDefs ** ppEnum);


  //****************
  // RegisterEffectDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterEffectDefinition
        // @parm [in] Effect Definition Object
        (ImplAAFEffectDef * pEffectDef);

  //****************
  // LookupEffectDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupEffectDefinition
        (// @parm [in,ref] Effect Unique ID
         aafUID_t *  effectID,

         // @parm [out,retval] Effect definition object
         ImplAAFEffectDef ** ppEffectDef);

  //****************
  // GetEffectDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEffectDefinitions
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFEffectDefs ** ppEnum);

  //****************
  // RegisterParameterDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterParameterDefinition
        // @parm [in] Parameter Definition Object
        (ImplAAFParameterDef * pParameterDef);

  //****************
  // LookupParameterDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupParameterDefinition
        (// @parm [in,ref] Parameter Unique ID
         aafUID_t *  ParameterID,

         // @parm [out,retval] Parameter definition object
         ImplAAFParameterDef ** ppParameterDef);

  //****************
  // GetParameterDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParameterDefinitions
        // @parm [out,retval] Definition Enumeration
        (ImplEnumAAFParameterDefs ** ppEnum);

  //****************
  // RegisterCodecDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterCodecDefinition
        // @parm [in] Pluggable Definition
        (ImplAAFCodecDef * pPlugDef);

  //****************
  // RegisterCodecDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
   GetCodecDefinitions
      // @parm [out,retval] Definition Enumeration
      (ImplEnumAAFCodecDefs ** ppEnum);

   	AAFRESULT LookupCodecDefinition(aafUID_t *containerID, ImplAAFCodecDef **result);

	//****************
  // RegisterContainerDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterContainerDefinition
        // @parm [in] Pluggable Definition
        (ImplAAFContainerDef * pPlugDef);

  //****************
  // GetContainerDefinitions()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetContainerDefinitions
        // @parm [out,retval] Definition Enumeration
       (ImplEnumAAFContainerDefs ** ppEnum);

    AAFRESULT LookupContainerDefinition(aafUID_t *containerID, ImplAAFContainerDef **result);

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFDictionary)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFDictionaryTest.cpp.
  static AAFRESULT test();

public:
	// Internal to the SDK
  //****************
  // GetNumEssenceData()
  //
  virtual AAFRESULT
    GetNumCodecDefs
        (aafUInt32 *  pNumCodecDefs);  //@parm [out,retval] Total number of pluggable defs

  virtual AAFRESULT
    GetNthCodecDef (aafInt32 index, ImplAAFCodecDef **ppEnum);

  virtual AAFRESULT
    GetNumContainerDefs
        (aafUInt32 *  pNumContainerDefs);  //@parm [out,retval] Total number of pluggable defs

  virtual AAFRESULT
    GetNthContainerDef (aafInt32 index, ImplAAFContainerDef **ppEnum);

  AAFRESULT LookupPropDef (OMPropertyId opid,
						   ImplAAFPropertyDef ** ppd);
private:
  // make sure built-in types are initialized.
  void InitBuiltins();

private:
  ImplAAFBuiltins * _pBuiltins;

    OMStrongReferenceVectorProperty<ImplAAFCodecDef>		_codecDefinitions;
    OMStrongReferenceVectorProperty<ImplAAFContainerDef>	_containerDefinitions;
    OMStrongReferenceVectorProperty<ImplAAFEffectDef>		_effectDefinitions;
    OMStrongReferenceVectorProperty<ImplAAFParameterDef>	_parameterDefinitions;
    OMStrongReferenceVectorProperty<ImplAAFTypeDef>			_typeDefinitions;
    OMStrongReferenceVectorProperty<ImplAAFClassDef>		_classDefinitions;
};

#endif // ! __ImplAAFDictionary_h__
