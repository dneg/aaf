/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


#ifndef __ImplAAFBuiltinClasses_h__
#include "ImplAAFBuiltinClasses.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __AAFClassDefUIDs_h__
#include "AAFClassDefUIDs.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFUtils.h"
#include "ImplAAFDictionary.h"

#include <assert.h>


/*static*/
bool ImplAAFBuiltinClasses::sBuiltinsInited = ImplAAFBuiltinClasses::sInitBuiltins ();


//
// Class definition table
//

#define AAF_TABLE_BEGIN() \
/*static*/ ImplAAFBuiltinClasses::ClassTblEntry \
ImplAAFBuiltinClasses::sBuiltinClassTable[] = \
{

#define AAF_CLASS(name, id, parent, concrete) \
  { &AUID_AAF##name, L"" L# name L"", &AUID_AAF##parent, 0, 0 },

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

const /*static*/ aafUInt32 ImplAAFBuiltinClasses::ksNumClassDefs =
  sizeof (ImplAAFBuiltinClasses::sBuiltinClassTable)
  / sizeof (ImplAAFBuiltinClasses::sBuiltinClassTable[0]);

//
// Prop definition table
//

#define AAF_TABLE_BEGIN() \
/*static*/ \
ImplAAFBuiltinClasses::PropTblEntry \
ImplAAFBuiltinClasses::sBuiltinPropTable[] = \
{

#define AAF_PROPERTY(name, id, tag, type, mandatory, container) \
  { L"" L# name L"", \
	id, \
    tag, \
    &kAAFTypeID_##type, \
    &kAAFClassID_##container, \
    mandatory, \
	ImplAAFBuiltinClasses::CreateOMPropType##type, \
    0 \
  },

#define AAF_TABLE_END() \
};

#define kAAFTypeID_AAF_REFERENCE_TYPE(type, target) \
  kAAFTypeID_##target##type

#define kAAFTypeID_AAF_TYPE(type) \
  kAAFTypeID_##type

#define CreateOMPropTypeAAF_REFERENCE_TYPE(type, target) \
  CreateOMPropType##type

#define CreateOMPropTypeAAF_TYPE(target) \
  CreateOMPropTypeSimple

#if 0

//
// Compatibility 
//
#define kAAFTypeID_WeakReference         kAAFTypeID_RefAUID
#define kAAFTypeID_WeakReferenceVector   kAAFTypeID_RefAUIDArray
#define kAAFTypeID_WeakReferenceSet      kAAFTypeID_RefAUIDArray
#define kAAFTypeID_StrongReference       kAAFTypeID_ObjRef
#define kAAFTypeID_StrongReferenceVector kAAFTypeID_ObjRefArray
#define kAAFTypeID_StrongReferenceSet    kAAFTypeID_ObjRefArray

// names changed to be more clear
#define kAAFTypeID_String kAAFTypeID_WCharString

// hax here to "alias" different types
#define kAAFTypeID_RefAUID kAAFTypeID_AUID
#define kAAFTypeID_RefAUIDArray kAAFTypeID_AUIDArray
#define kAAFTypeID_Length kAAFTypeID_Int64
#define kAAFTypeID_Position kAAFTypeID_Int64
#define kAAFTypeID_ColorSitingType kAAFTypeID_UInt32
#define kAAFTypeID_EdgeType kAAFTypeID_UInt32
#define kAAFTypeID_FilmType kAAFTypeID_UInt32
//#define kAAFTypeID_Boolean kAAFTypeID_UInt8
// on NT sizeof(aafBool) == 4 and that's
// how it is currently persisted
#define kAAFTypeID_Boolean kAAFTypeID_UInt32
#define kAAFTypeID_CompCodeArray kAAFTypeID_UInt8
#define kAAFTypeID_TCSource kAAFTypeID_UInt32
#define kAAFTypeID_TapeCaseType kAAFTypeID_UInt32
#define kAAFTypeID_TapeFormatType kAAFTypeID_UInt32
#define kAAFTypeID_VideoSignalType kAAFTypeID_UInt32
#define kAAFTypeID_LayoutType kAAFTypeID_Int32
#define kAAFTypeID_EditHintType kAAFTypeID_UInt16
#define kAAFTypeID_JPEGTableIDType kAAFTypeID_UInt32
#define kAAFTypeID_PulldownKindType kAAFTypeID_UInt32
#define kAAFTypeID_PulldownDirectionType kAAFTypeID_UInt32
#define kAAFTypeID_PhaseFrameType kAAFTypeID_UInt16

// bogus types 
#define kAAFTypeID_DataStream kAAFTypeID_UInt8Array
#define kAAFTypeID_DataValue kAAFTypeID_UInt8Array
#define kAAFTypeID_ReferenceType kAAFTypeID_UInt8Array
#define kAAFTypeID_StringArray kAAFTypeID_UInt8Array
#define kAAFTypeID_TimeStamp kAAFTypeID_UInt8Array
#define kAAFTypeID_Int64Array kAAFTypeID_UInt8Array
#define kAAFTypeID_PositionArray kAAFTypeID_UInt8Array
#define kAAFTypeID_ProductVersion kAAFTypeID_UInt8Array
#define kAAFTypeID_Rational kAAFTypeID_UInt8Array
#define kAAFTypeID_VersionType kAAFTypeID_UInt8Array
#define kAAFTypeID_Rectangle kAAFTypeID_UInt8Array
#define kAAFTypeID_Int32Array kAAFTypeID_UInt8Array
#define kAAFTypeID_CompSizeArray kAAFTypeID_UInt8Array
#define kAAFTypeID_RGBALayout kAAFTypeID_UInt8Array

#else

// Streams are not yet implemented.
#define kAAFTypeID_DataStream kAAFTypeID_UInt8Array

// String arrays are currently implemented as a single
// null-character-delimited string
#define kAAFTypeID_StringArray kAAFTypeID_String

#endif

#include "AAFMetaDictionary.h"

const /*static*/ aafUInt32 ImplAAFBuiltinClasses::ksNumPropDefs =
  sizeof (ImplAAFBuiltinClasses::sBuiltinPropTable)
  / sizeof (ImplAAFBuiltinClasses::sBuiltinPropTable[0]);



const /*static*/ aafUID_t * ImplAAFBuiltinClasses::sAxClassIDs[] =
{
  &AUID_AAFHeader,
  &AUID_AAFDictionary,
  &AUID_AAFClassDef,
  &AUID_AAFPropertyDef,
  &AUID_AAFDefObject,
  &AUID_AAFTypeDef,
  &AUID_AAFTypeDefInt,

  &AUID_AAFInterpolationDefinition,
  &AUID_AAFContainerDef,
  &AUID_AAFCodecDef,
  &AUID_AAFDataDef,
  &AUID_AAFOperationDef,
  &AUID_AAFParameterDef,
  &AUID_AAFIdentification,
  &AUID_AAFContentStorage,
  &AUID_AAFPluginDescriptor,

  &AUID_AAFTypeDefRecord,
  &AUID_AAFTypeDefVariableArray,
  &AUID_AAFTypeDefFixedArray,
  &AUID_AAFTypeDefString,
  &AUID_AAFTypeDefEnum,
  &AUID_AAFTypeDefRename,
  &AUID_AAFTypeDefStrongObjRef,

  /*
  &AUID_AAFTypeDefExtEnum,
  &AUID_AAFTypeDefSet,
  &AUID_AAFTypeDefStream,
  &AUID_AAFTypeDefWeakObjRef,
  */

  &AUID_AAFLocator,
  &AUID_AAFNetworkLocator,

  &AUID_AAFObject	// this one is controversial...
};

const /*static*/ aafUInt32 ImplAAFBuiltinClasses::ksNumAxClasses =
sizeof (ImplAAFBuiltinClasses::sAxClassIDs) / sizeof (ImplAAFBuiltinClasses::sAxClassIDs[0]);



ImplAAFBuiltinClasses::ImplAAFBuiltinClasses (ImplAAFDictionary* dictionary)
  : _dictionary (dictionary),
	_axClassDefs (0),
	_axPropDefs (0)
{
  _axClassDefs = new ImplAAFClassDefSP[ksNumAxClasses];
  instantiateProps ();
  instantiateClasses ();
}


ImplAAFBuiltinClasses::~ImplAAFBuiltinClasses ()
{
  delete[] _axClassDefs;
  delete[] _axPropDefs;
}


AAFRESULT
ImplAAFBuiltinClasses::NewBuiltinClassDef (const aafUID_t & rClassID,
										   ImplAAFClassDef ** ppResult)
{
  AAFRESULT status = AAFRESULT_NO_MORE_OBJECTS;

  ImplAAFUID popped;
  aafUInt32 i;
  const aafUInt32 kNumClasses =
	sizeof (sBuiltinClassTable) / sizeof (sBuiltinClassTable[0]);

  assert (! _createStack.isPresent (rClassID));

  _createStack.push (rClassID);

  for (i = 0; i < kNumClasses; i++)
	{
	  if (EqualAUID (sBuiltinClassTable[i].pThisId, &rClassID))
		{
		  // We've found the desired class in our table.
		  ImplAAFClassDef * pcd = (ImplAAFClassDef*)
			_dictionary->pvtInstantiate(&AUID_AAFClassDef);
		  assert (pcd);
		  pcd->InitOMProperties ();

		  status = InitBuiltinClassDef (rClassID, pcd);
		  if (AAFRESULT_SUCCEEDED (status))
			{
			  assert (ppResult);
			  *ppResult = pcd;
			}
		  break;
		}
	}

  popped = _createStack.pop ();
  assert (popped == rClassID);
  return status;
}



AAFRESULT
ImplAAFBuiltinClasses::InitBuiltinClassDef (const aafUID_t & rClassID,
											ImplAAFClassDef * pClass)
{
  assert (pClass);
  AAFRESULT status = AAFRESULT_NO_MORE_OBJECTS;
  ImplAAFUID popped;
  aafUInt32 i;
  const aafUInt32 kNumClasses =
	sizeof (sBuiltinClassTable) / sizeof (sBuiltinClassTable[0]);

  assert (! _initStack.isPresent (rClassID));

  _initStack.push (rClassID);

  for (i = 0; i < kNumClasses; i++)
	{
	  if (EqualAUID (sBuiltinClassTable[i].pThisId, &rClassID))
		{
		  AAFRESULT hr;
		  hr = pClass->pvtInitialize (sBuiltinClassTable[i].pThisId,
									  sBuiltinClassTable[i].pParentId,
									  sBuiltinClassTable[i].pName);

		  assert (AAFRESULT_SUCCEEDED (hr));

		  RegisterBuiltinProperties (pClass);

		  status = AAFRESULT_SUCCESS;
		  break;
		}
	}
  // not found
  popped = _initStack.pop ();
  assert (popped == rClassID);
  return status;
}


AAFRESULT ImplAAFBuiltinClasses::LookupOmPid
  (const aafUID_t & rAuid,
   OMPropertyId & outPid) const
{
  aafUInt32 i;

  for (i = 0; i < ksNumPropDefs; i++)
	{
	  if (EqualAUID (&rAuid, &sBuiltinPropTable[i].id))
		{
		  // Yup, matches.  Return the pid.
		  outPid = sBuiltinPropTable[i].tag;
		  return AAFRESULT_SUCCESS;
		}
	}
  // Not found.
  return E_FAIL;
}


aafUInt32 ImplAAFBuiltinClasses::countAxProps () const
{
  aafUInt32 numProps = 0;
  aafUInt32 classIdx;
  for (classIdx = 0; classIdx < ksNumAxClasses; classIdx++)
	{
	  for (aafUInt32 i = 0;
		   i < ksNumPropDefs;
		   i++)
		{
		  if (EqualAUID (sBuiltinPropTable[i].pOwnerClassGuid,
						 sAxClassIDs[classIdx]))
			numProps++;
		}
	}
  return numProps;
}


ImplAAFPropertyDef * ImplAAFBuiltinClasses::lookupAxProp (OMPropertyId pid)
{
  assert (_axPropDefs);

  ImplAAFPropertyDef * result = 0;
  aafUInt32 count = countAxProps();

  for (aafUInt32 i = 0; i < count; i++)
	{
	  if (_axPropDefs[i].pid == pid)
		{
		  result = _axPropDefs[i].pPropDef;
		  assert (result);
		  break;
		}
	}
  return result;
}


ImplAAFClassDef * ImplAAFBuiltinClasses::LookupAxiomaticClass
(const aafUID_t & classId)
{
  assert (_axClassDefs);

  ImplAAFClassDef * result = 0;

  for (aafUInt32 i = 0; i < ksNumAxClasses; i++)
	{
	  aafUID_t testId;
	  AAFRESULT hr = _axClassDefs[i]->GetAUID (&testId);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  if (EqualAUID (&testId, &classId))
		{
		  result = _axClassDefs[i];
		  assert (result);
		  break;
		}
	}
  return result;
}


void ImplAAFBuiltinClasses::instantiateProps ()
{
  assert (! _axPropDefs);
  aafUInt32 numProps = countAxProps ();
  _axPropDefs = new PropDefRecord[numProps];
  assert (_axPropDefs);

  aafUInt32 propIdx = 0;
  aafUInt32 classIdx = 0;
  for (classIdx = 0; classIdx < ksNumAxClasses; classIdx++)
	{
	  ClassTblEntry * classInfo =
		lookupClassEntry (*sAxClassIDs[classIdx]);
	  assert (classInfo);

	  PropTblEntry * propInfo = classInfo->pProperties;

	  while (propInfo)
		{
		  ImplAAFObject * obj =
			_dictionary->pvtInstantiate (&AUID_AAFPropertyDef);
		  assert (obj);
		  ImplAAFPropertyDef * propDef = dynamic_cast<ImplAAFPropertyDef*>(obj);
		  assert (propDef);
		  
		  AAFRESULT hr = propDef->Initialize
			(&propInfo->id,
			 propInfo->tag,
			 propInfo->name,
			 propInfo->pTypeGuid,
			 propInfo->mandatory ? AAFFalse : AAFTrue);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  propDef->SetOMPropCreateFunc (propInfo->omPropCreateFunc);

		  assert (propIdx < numProps);
		  _axPropDefs[propIdx].pid = propInfo->tag;
		  _axPropDefs[propIdx].pPropDef = propDef;
		  _axPropDefs[propIdx].ownerClassId = *propInfo->pOwnerClassGuid;

		  // no longer need this ref count
		  assert (propDef);
		  propDef->ReleaseReference ();

		  propIdx++;
		  propInfo = propInfo->nextProp;
		}
	}  

  assert (propIdx == numProps);
  for (propIdx = 0; propIdx < numProps; propIdx++)
	{
	  OMPropertySet * ps = _axPropDefs[propIdx].pPropDef->propertySet();
	  assert (ps);
	  size_t numOmProps = ps->count ();
	  size_t context = 0;
	  while (numOmProps--)
		{
		  OMProperty * p = 0;
		  ps->iterate (context, p);
		  assert (p);
		  ImplAAFPropertyDef * pd = lookupAxProp (p->propertyId());
		  assert (pd);
		  p->initialize (pd);
		}
	}
}


void ImplAAFBuiltinClasses::instantiateClasses ()
{
  aafUInt32 classIdx;
  // foreach axiomatic class id, instantiate a class def object.
  for (classIdx = 0; classIdx < ksNumAxClasses; classIdx++)
	{
	  // instantiate the class def object
	  ImplAAFObject * tmp = _dictionary->pvtInstantiate (&AUID_AAFClassDef);
	  assert (tmp);
	  _axClassDefs[classIdx] = dynamic_cast<ImplAAFClassDef*>(tmp);
	  assert (_axClassDefs[classIdx]);
	  tmp->ReleaseReference ();
	  tmp = 0;

	  // classdef->initialize (id, parentId, name)
	  const ClassTblEntry * cte = lookupClassEntry(*sAxClassIDs[classIdx]);
	  assert (cte);

	  AAFRESULT hr;
	  hr = _axClassDefs[classIdx]->pvtInitialize (cte->pThisId,
												  cte->pParentId,
												  cte->pName);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  // foreach property def contained in that class def, get the
	  // (already existing) axiomatic property def object, init it,
	  // and install it into the class def object.
	  PropTblEntry * pte = cte->pProperties;
	  while (pte)
		{
		  ImplAAFPropertyDef * propDef = lookupAxProp (pte->tag);
		  assert (propDef);
			  
		  hr = _axClassDefs[classIdx]->
			pvtAppendExistingPropertyDef (propDef);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  pte = pte->nextProp;
		}
	}

  // Initialize the OM properties for each axiomatic class.
  for (classIdx = 0; classIdx < ksNumAxClasses; classIdx++)
	{
	  ImplAAFClassDef * pcd = _axClassDefs[classIdx];
	  assert (pcd);
	  OMPropertySet * ps = pcd->propertySet();
	  assert (ps);
	  size_t numOmProps = ps->count ();
	  size_t context = 0;
	  while (numOmProps--)
		{
		  OMProperty * p = 0;
		  ps->iterate (context, p);
		  assert (p);
		  ImplAAFPropertyDef * pd = lookupAxProp (p->propertyId());
		  assert (pd);
		  p->initialize (pd);
		}
	}
}

bool ImplAAFBuiltinClasses::IsAxiomaticClass
(const aafUID_t & classId) const
{
  bool result = false;

  for (aafUInt32 classIdx = 0;
	   classIdx < ksNumAxClasses;
	   classIdx++)
	{
	  if (EqualAUID (&classId, sAxClassIDs[classIdx]))
		{
		  result = true;
		  break;
		}
	}

  return result;
}


aafUInt32 ImplAAFBuiltinClasses::CountAxClasses () const
{
  return ksNumAxClasses;
}


void ImplAAFBuiltinClasses::RegisterBuiltinProperties
(
 ImplAAFClassDef * pClassDef
) const
{
  assert (pClassDef);

  // Get auid for given class def.
  aafUID_t classAuid;
  AAFRESULT hr;
  hr = pClassDef->GetAUID (&classAuid);
  assert (AAFRESULT_SUCCEEDED (hr));
  
  /*
  if (EqualAUID (&classAuid, &AUID_AAFPropertyDef))
	{
	  // Axiomatic class def describing a property definition object.
	  RegisterAxiomaticPropertyDefProperties (pClassDef);
	}
  else*/
	{
	  // Look through all known builtin properties.  Identify which ones
	  // belong to this class.  For each one, instantiate it and add it to
	  // the class.
	  aafUInt32 i;
	  for (i = 0; i < ksNumPropDefs; i++)
		{
		  if (EqualAUID (&classAuid, sBuiltinPropTable[i].pOwnerClassGuid))
			{
			  // Yup, in this class.

			  ImplAAFPropertyDef * pd = 0;
			  hr = pClassDef->pvtAppendPropertyDef
				((aafUID_t *) &sBuiltinPropTable[i].id,
				 sBuiltinPropTable[i].name,
				 sBuiltinPropTable[i].pTypeGuid,
				 sBuiltinPropTable[i].mandatory ? AAFFalse : AAFTrue,
				 &pd);
			  assert (AAFRESULT_SUCCEEDED (hr));

			  // Don't need these anymore
			  assert (pd);
			  pd->ReleaseReference ();
			  pd = 0;
			}
		}
	}
}

/*static*/ OMProperty *
ImplAAFBuiltinClasses::CreateOMPropTypeSimple
  (OMPropertyId /*pid*/,
   const char * /*name*/)
{
  // Let's return 0 so the caller will attempt to use the type def to
  // create the property.
  return 0;
  
  // assert (name);
  // // HACK! set element size to 1! It's not easy to get element size at
  // // this point, and setting the value will resize it to the correct
  // // size anyway.
  // return new OMSimpleProperty (pid, name, /*elemSize*/ 1);
}

/*static*/ OMProperty *
ImplAAFBuiltinClasses::CreateOMPropTypeWeakReference
  (OMPropertyId pid,
   const char * name)
{
  assert (name);
  return new OMSimpleProperty (pid, name, sizeof (aafUID_t));
}

/*static*/ OMProperty *
ImplAAFBuiltinClasses::CreateOMPropTypeWeakReferenceSet
  (OMPropertyId pid,
   const char * name)
{
  assert (name);
  return new OMSimpleProperty (pid, name, sizeof (aafUID_t));
}

/*static*/ OMProperty *
ImplAAFBuiltinClasses::CreateOMPropTypeWeakReferenceVector
  (OMPropertyId pid,
   const char * name)
{
  assert (name);
  return new OMSimpleProperty (pid, name, sizeof (aafUID_t));
}

/*static*/ OMProperty *
ImplAAFBuiltinClasses::CreateOMPropTypeStrongReference
  (OMPropertyId pid,
   const char * name)
{
  assert (name);
  return new OMStrongReferenceProperty<ImplAAFObject> (pid, name);
}

/*static*/ OMProperty *
ImplAAFBuiltinClasses::CreateOMPropTypeStrongReferenceSet
  (OMPropertyId pid,
   const char * name)
{
  assert (name);
  return new OMStrongReferenceVectorProperty<ImplAAFObject> (pid, name);
}

/*static*/ OMProperty *
ImplAAFBuiltinClasses::CreateOMPropTypeStrongReferenceVector
  (OMPropertyId pid,
   const char * name)
{
  assert (name);
  return new OMStrongReferenceVectorProperty<ImplAAFObject> (pid, name);
}


/*static*/
bool ImplAAFBuiltinClasses::sInitBuiltins ()
{
  if (sBuiltinsInited)
	return true;

  // Go through class table, wiring up the parent IDs and properties
  aafUInt32 thisIdx;
  aafUInt32 parentIdx;
  for (thisIdx = 0; thisIdx < ksNumClassDefs; thisIdx++)
	{
	  sBuiltinClassTable[thisIdx].pParent = 0;

	  // Look for this class' parent
	  for (parentIdx = 0; parentIdx < ksNumClassDefs; parentIdx++)
		{
		  if (EqualAUID (sBuiltinClassTable[thisIdx].pParentId,
						 sBuiltinClassTable[parentIdx].pThisId))
			{
			  // 'parent' is parent of 'this'
			  sBuiltinClassTable[thisIdx].pParent =
				&sBuiltinClassTable[thisIdx];
			  break; // out of 'parent' loop
			}
		}

	  // Look for properties for this class
	  aafUInt32 propIdx;
	  PropTblEntry * pLastPropInClass = 0;
	  // Make sure this is zeroed first, in case there are not props.
	  sBuiltinClassTable[thisIdx].pProperties = 0;
	  for (propIdx = 0; propIdx < ksNumPropDefs; propIdx++)
		{
		  if (EqualAUID (sBuiltinClassTable[thisIdx].pThisId,
						sBuiltinPropTable[propIdx].pOwnerClassGuid))
			{
			  // This prop belongs in this class.  Add it to the list.
			  if (pLastPropInClass)
				{
				  // this class already has props identified for it.
				  // Add it to the list.
				  assert (0 != sBuiltinClassTable[thisIdx].pProperties);
				  pLastPropInClass->nextProp = &sBuiltinPropTable[propIdx];
				}
			  else
				{
				  // This class has no props identified for it yet.
				  assert (0 ==
						  sBuiltinClassTable[thisIdx].pProperties);
				  sBuiltinClassTable[thisIdx].pProperties =
					&sBuiltinPropTable[propIdx];
				}
			  pLastPropInClass = &sBuiltinPropTable[propIdx];
			  // terminate this in case it's the last one
			  pLastPropInClass->nextProp = 0;
			}
		}
	}

  return true;
}

/*static*/
ImplAAFBuiltinClasses::ClassTblEntry *
ImplAAFBuiltinClasses::lookupClassEntry
(const aafUID_t & classId)
{
  assert (sBuiltinsInited);

  ClassTblEntry * result = 0;

  aafUInt32 i;
  for (i = 0; i < ksNumClassDefs; i++)
	{
	  if (EqualAUID (sBuiltinClassTable[i].pThisId, &classId))
		{
		  result = &sBuiltinClassTable[i];
		  break;
		}
	}
  assert (result);
  return (result);
}


void ImplAAFBuiltinClasses::AssurePropertyTypes ()
{
  aafUInt32 i;
  for (i = 0; i < ksNumAxClasses; i++)
	{
	  ImplAAFClassDefSP pcd;
	  AAFRESULT hr;
	  assert (_dictionary);
	  pcd = 0;
	  hr = _dictionary->LookupClass (sAxClassIDs[i], &pcd);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  assert (pcd);
	  pcd->AssurePropertyTypesLoaded ();
	}
}
