/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFBuiltinClasses_h__
#include "ImplAAFBuiltinClasses.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __AAFClassDefUIDs_h__
#include "AAFClassDefUIDs.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFUtils.h"
#include "ImplAAFDictionary.h"

#include <assert.h>


ImplAAFBuiltinClasses::ImplAAFBuiltinClasses (ImplAAFDictionary* dictionary)
  : _dictionary (dictionary)
{}

ImplAAFBuiltinClasses::~ImplAAFBuiltinClasses ()
{}


//
// Builtin class-def lookup
//

#define AAF_TABLE_BEGIN() \
const /*static*/ ImplAAFBuiltinClasses::ClassTblEntry \
ImplAAFBuiltinClasses::sBuiltinClassTable[] = \
{

#define AAF_CLASS(name, id, parent, concrete) \
  { &AUID_AAF##name, L"" L# name L"", &AUID_AAF##parent },

#define AAF_TABLE_END() \
};

const static aafUID_t NULL_AUID = { 0 };

// Special case: classes which descend from AAFRoot (actualy only
// AAFObject) should report NULL as their parent class.
#define AUID_AAFRoot NULL_AUID

#define AUID_AAFClassDefinition AUID_AAFClassDef
#define AUID_AAFDataDefinition AUID_AAFDataDef
#define AUID_AAFDefinitionObject AUID_AAFDefObject
#define AUID_AAFEdgeCode AUID_AAFEdgecode
#define AUID_AAFOperationDefinition AUID_AAFOperationDef
#define AUID_AAFInterchangeObject AUID_AAFObject
#define AUID_AAFJPEGImageData AUID_AAFJPEGData
#define AUID_AAFMIDIFileData AUID_AAFMIDIData
#define AUID_AAFParameterDefinition AUID_AAFParameterDef
#define AUID_AAFPropertyDefinition AUID_AAFPropertyDef
#define AUID_AAFTypeDefinition AUID_AAFTypeDef
#define AUID_AAFTypeDefinitionEnumeration AUID_AAFTypeDefEnum
#define AUID_AAFTypeDefinitionExtendibleEnumeration AUID_AAFTypeDefExtEnum
#define AUID_AAFTypeDefinitionFixedArray AUID_AAFTypeDefFixedArray
#define AUID_AAFTypeDefinitionInteger AUID_AAFTypeDefInt
#define AUID_AAFTypeDefinitionRecord AUID_AAFTypeDefRecord
#define AUID_AAFTypeDefinitionRename AUID_AAFTypeDefRename
#define AUID_AAFTypeDefinitionSet AUID_AAFTypeDefSet
#define AUID_AAFTypeDefinitionStream AUID_AAFTypeDefStream
#define AUID_AAFTypeDefinitionString AUID_AAFTypeDefString
#define AUID_AAFTypeDefinitionStrongObjectReference AUID_AAFTypeDefStrongObjRef
#define AUID_AAFTypeDefinitionVariableArray AUID_AAFTypeDefVariableArray
#define AUID_AAFTypeDefinitionWeakObjectReference AUID_AAFTypeDefWeakObjRef
#define AUID_AAFUNIXLocator AUID_AAFUnixLocator
#define AUID_AAFCodecDefinition AUID_AAFCodecDef
#define AUID_AAFContainerDefinition AUID_AAFContainerDef
#define AUID_AAFPluggableDefinition AUID_AAFPluggableDef

#include "AAFMetaDictionary.h"


AAFRESULT
ImplAAFBuiltinClasses::NewBuiltinClassDef (const aafUID_t & rClassID,
										   ImplAAFClassDef ** ppResult)
{
  aafUInt32 i;
  const aafUInt32 kNumClasses =
	sizeof (sBuiltinClassTable) / sizeof (sBuiltinClassTable[0]);

  for (i = 0; i < kNumClasses; i++)
	{
	  if (EqualAUID (sBuiltinClassTable[i].pThisId, &rClassID))
		{
		  // We've found the desired class in our table.

		  // Don't call CreateImplObject()!!! It will automatically
		  // initialize the OM properties of this object, which will
		  // cause infinite recursion.
		  // ImplAAFClassDef * pcd =
		  //   (ImplAAFClassDef *) _dictionary->CreateImplObject(AUID_AAFClassDef);

		  ImplAAFObject *pObj = _dictionary->pvtCreateBaseClassInstance(&AUID_AAFClassDef);
		  if (! pObj) return AAFRESULT_NOMEMORY;
		  pObj->setClassFactory(_dictionary);

		  ImplAAFClassDef * pcd = static_cast<ImplAAFClassDef*>(pObj);
		  assert (pcd);

		  AAFRESULT hr;
		  ImplAAFClassDef * pParentClass = 0;

		  assert (_dictionary);
		  if (EqualAUID (sBuiltinClassTable[i].pParentId, &NULL_AUID))
			pParentClass = 0;
		  else
			{
			  hr = _dictionary->LookupClass (sBuiltinClassTable[i].pParentId,
											 &pParentClass);
			  assert (AAFRESULT_SUCCEEDED (hr));
			  assert (pParentClass);
			}

		  hr = pcd->Initialize (sBuiltinClassTable[i].pThisId,
								pParentClass,
								sBuiltinClassTable[i].pName);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  if (pParentClass)
			{
			  pParentClass->ReleaseReference ();
			  pParentClass = 0;
			}

		  assert (ppResult);
		  *ppResult = pcd;
		  return AAFRESULT_SUCCESS;
		}
	}
  // not found
  return AAFRESULT_NO_MORE_OBJECTS;
}


AAFRESULT ImplAAFBuiltinClasses::ImportBuiltinClassDef
  (const aafUID_t & classID,
   ImplAAFClassDef ** ppResult)
{
  AAFRESULT hr;
  ImplAAFClassDefSP pcd;

  assert (ppResult);

  //
  // It's important that we perform the following in this order:
  // (1) Create object, without initializing the OM properties
  // (2) Register the object in the dict, even before OM props are
  //     initialized
  // (3) Init OM properties *after* the class has been registered.
  //
  hr = NewBuiltinClassDef (classID, &pcd);
  if (AAFRESULT_FAILED (hr))
	return hr;
  assert (pcd);

  assert (_dictionary);
  hr = _dictionary->RegisterClass (pcd);

  if (AAFRESULT_FAILED (hr))
	return hr;

  assert (pcd);

  ImplAAFObjectSP objSP;
  objSP = (ImplAAFObject*) pcd;
  _dictionary->pvtInitObjectProperties (objSP);

  assert (ppResult);
  *ppResult = pcd;
  (*ppResult)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}
