/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFBuiltinTypes_h__
#include "ImplAAFBuiltinTypes.h"
#endif

#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
#endif

#ifndef __ImplAAFTypeDefVariableArray_h__
#include "ImplAAFTypeDefVariableArray.h"
#endif

#ifndef __ImplAAFTypeDefFixedArray_h__
#include "ImplAAFTypeDefFixedArray.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefRecord_h__
#include "ImplAAFTypeDefRecord.h"
#endif

#ifndef __ImplAAFTypeDefString_h__
#include "ImplAAFTypeDefString.h"
#endif

#ifndef __ImplAAFTypeDefEnum_h__
#include "ImplAAFTypeDefEnum.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ImplAAFDictionary.h"
#include "AAFUtils.h"

#include <assert.h>


/*
UInt8:
 Boolean
 CompCodeArray
 ? EditHintType
 ? FadeType
 ? PulldownKindType
 ? TCSource
 ? TapeCaseType
 ? TapeFormatType
 ? VideoSignalType


UInt16:
 ColorSitingType
 EdgeType
 FilmType

Int16:
 LayoutType

UInt32:

Int32:

Int64:
 Length
 Position

StrongObjRef:
 ObjRef
 ? RefAUID

VarArray<StrongObjRef>
 ObjRefArray
 ? RefAUIDArray

???:
 DataStream
 DataValue
 JPEGTableIDType
 PhaseFrameType
 PulldownDirectionType
 ReferenceType
 StringArray
 TimeStamp


VarArray<Int32>
 Int32Array

VarArray<Int64>
 Int64Array
 PositionArray

FixArray<Int16, 5>
 ProductVersion

FixArray<Int32, 2>
 Rational

FixArray<UInt32, 2>
 VersionType

FixArray<Rational, 4>
 Rectangle

VarArray<UInt8>
 String
 */



ImplAAFBuiltinTypes::ImplAAFBuiltinTypes (ImplAAFDictionary* dictionary) :
  _dictionary(dictionary),
  _TD_AUID (0),
  _TD_AUIDArray (0),
  _TD_UInt8Array (0),
  _TD_UInt8Array8 (0),
  _TD_UInt8 (0),
  _TD_UInt16 (0),
  _TD_Int16 (0),
  _TD_UInt32 (0),
  _TD_Int32 (0),
  _TD_Int64 (0),
  _TD_ObjRef (0),
  _TD_ObjRefArray (0),
  _TD_WCharString (0),
  _TD_FadeType (0)
{
  // This object and the dictionary have the same lifetime so we do
  // not want to change the dictionary's reference count. In fact, a
  // future version of this class may BE an instance of a dictionary!
  assert(_dictionary);
}

ImplAAFBuiltinTypes::~ImplAAFBuiltinTypes ()
{
  if (_TD_AUID)        _TD_AUID->ReleaseReference ();
  if (_TD_AUIDArray)   _TD_AUIDArray->ReleaseReference ();
  if (_TD_UInt8Array)  _TD_UInt8Array->ReleaseReference ();
  if (_TD_UInt8Array8) _TD_UInt8Array8->ReleaseReference ();
  if (_TD_UInt8)       _TD_UInt8->ReleaseReference ();
  if (_TD_UInt16)      _TD_UInt16->ReleaseReference ();
  if (_TD_Int16)       _TD_Int16->ReleaseReference ();
  if (_TD_UInt32)      _TD_UInt32->ReleaseReference ();
  if (_TD_Int32)       _TD_Int32->ReleaseReference ();
  if (_TD_Int64)       _TD_Int64->ReleaseReference ();
  if (_TD_ObjRef)      _TD_ObjRef->ReleaseReference ();
  if (_TD_ObjRefArray) _TD_ObjRefArray->ReleaseReference ();
  if (_TD_WCharString) _TD_WCharString->ReleaseReference ();
  if (_TD_FadeType)    _TD_FadeType->ReleaseReference ();
}



AAFRESULT
ImplAAFBuiltinTypes::TypeDefAUID (ImplAAFTypeDef ** ptd)
{
  if (!_TD_AUID)
	{
	  AAFRESULT hr;
	  ImplAAFTypeDef * pTDUInt8Array8 = NULL;
	  ImplAAFTypeDef * pTDUInt16 = NULL;
	  ImplAAFTypeDef * pTDUInt32 = NULL;

	  // hr = ImplAAFBuiltinTypes::TypeDefUInt8Array8 (&pTDUInt8Array8);
	  hr = _dictionary->LookupType (&kAAFTypeID_UInt8Array8, &pTDUInt8Array8);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (pTDUInt8Array8);

	  // hr = ImplAAFBuiltinTypes::TypeDefUInt16 (&pTDUInt16);
	  hr = _dictionary->LookupType (&kAAFTypeID_UInt16, &pTDUInt16);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (pTDUInt16);

	  // hr = ImplAAFBuiltinTypes::TypeDefUInt32 (&pTDUInt32);
	  hr = _dictionary->LookupType (&kAAFTypeID_UInt32, &pTDUInt32);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (pTDUInt32);

	  // _TD_AUID = (ImplAAFTypeDefRecord *) _dictionary->CreateImplObject(AUID_AAFTypeDefRecord);
	  // if (! _TD_AUID) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefRecord);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_AUID = static_cast<ImplAAFTypeDefRecord*>(pObj);
	  assert (_TD_AUID);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_AUID->SetAUID (&kAAFTypeID_AUID);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  {
		ImplAAFTypeDef * memberTypes[] = {pTDUInt32,
										   pTDUInt16,
										   pTDUInt16,
										   pTDUInt8Array8};
		aafString_t memberNames[] = {L"Data1",
									 L"Data2",
									 L"Data3",
									 L"Data4"};
		hr = _TD_AUID->Initialize (&kAAFTypeID_AUID,
									memberTypes,
									memberNames,
									4,
									L"aafUID_t");
	  if (AAFRESULT_FAILED(hr)) return hr;
	  }
	  pTDUInt8Array8->ReleaseReference ();
	  pTDUInt16->ReleaseReference ();
	  pTDUInt32->ReleaseReference ();
	}
  assert (_TD_AUID);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_AUID;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}

AAFRESULT
ImplAAFBuiltinTypes::TypeDefUInt8 (ImplAAFTypeDef ** ptd)
{
  if (!_TD_UInt8)
	{
	  AAFRESULT hr;

	  // _TD_UInt8 = (ImplAAFTypeDefInt *) _dictionary->CreateImplObject(AUID_AAFTypeDefInt);
	  // if (! _TD_UInt8) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefInt);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_UInt8 = static_cast<ImplAAFTypeDefInt*>(pObj);
	  assert (_TD_UInt8);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_UInt8->SetAUID (&kAAFTypeID_UInt8);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = _TD_UInt8->Initialize (&kAAFTypeID_UInt8, 1, AAFFalse, L"aafUInt8");
	  if (AAFRESULT_FAILED(hr)) return hr;
	}
  assert (_TD_UInt8);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_UInt8;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefUInt16 (ImplAAFTypeDef ** ptd)
{
  if (!_TD_UInt16)
	{
	  AAFRESULT hr;

	  // _TD_UInt16 = (ImplAAFTypeDefInt *) _dictionary->CreateImplObject(AUID_AAFTypeDefInt);
	  // if (! _TD_UInt16) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefInt);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_UInt16 = static_cast<ImplAAFTypeDefInt*>(pObj);
	  assert (_TD_UInt16);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_UInt16->SetAUID (&kAAFTypeID_UInt16);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = _TD_UInt16->Initialize (&kAAFTypeID_UInt16, 2, AAFFalse, L"aafUInt16");
	  if (AAFRESULT_FAILED(hr)) return hr;
	}
  assert (_TD_UInt16);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_UInt16;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefInt16 (ImplAAFTypeDef ** ptd)
{
  if (!_TD_Int16)
	{
	  AAFRESULT hr;

	  // _TD_Int16 = (ImplAAFTypeDefInt *) _dictionary->CreateImplObject(AUID_AAFTypeDefInt);
	  // if (! _TD_Int16) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefInt);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_Int16 = static_cast<ImplAAFTypeDefInt*>(pObj);
	  assert (_TD_Int16);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_Int16->SetAUID (&kAAFTypeID_Int16);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = _TD_Int16->Initialize (&kAAFTypeID_Int16, 2, AAFTrue, L"aafInt16");
	  if (AAFRESULT_FAILED(hr)) return hr;
	}
  assert (_TD_Int16);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_Int16;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefUInt32 (ImplAAFTypeDef ** ptd)
{
  if (!_TD_UInt32)
	{
	  AAFRESULT hr;

	  // _TD_UInt32 = (ImplAAFTypeDefInt *) _dictionary->CreateImplObject(AUID_AAFTypeDefInt);
	  // if (! _TD_UInt32) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefInt);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_UInt32 = static_cast<ImplAAFTypeDefInt*>(pObj);
	  assert (_TD_UInt32);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_UInt32->SetAUID (&kAAFTypeID_UInt32);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = _TD_UInt32->Initialize (&kAAFTypeID_UInt32, 4, AAFFalse, L"aafUInt32");
	  if (AAFRESULT_FAILED(hr)) return hr;
	}
  assert (_TD_UInt32);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_UInt32;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefInt32 (ImplAAFTypeDef ** ptd)
{
  if (!_TD_Int32)
	{
	  AAFRESULT hr;

	  // _TD_Int32 = (ImplAAFTypeDefInt *) _dictionary->CreateImplObject(AUID_AAFTypeDefInt);
	  // if (! _TD_Int32) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefInt);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_Int32 = static_cast<ImplAAFTypeDefInt*>(pObj);
	  assert (_TD_Int32);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_Int32->SetAUID (&kAAFTypeID_Int32);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = _TD_Int32->Initialize (&kAAFTypeID_Int32, 4, AAFTrue, L"aafInt32");
	  if (AAFRESULT_FAILED(hr)) return hr;
	}
  assert (_TD_Int32);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_Int32;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefInt64 (ImplAAFTypeDef ** ptd)
{
  if (!_TD_Int64)
	{
	  AAFRESULT hr;

	  // _TD_Int64 = (ImplAAFTypeDefInt *) _dictionary->CreateImplObject(AUID_AAFTypeDefInt);
	  // if (! _TD_Int64) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefInt);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_Int64 = static_cast<ImplAAFTypeDefInt*>(pObj);
	  assert (_TD_Int64);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_Int64->SetAUID (&kAAFTypeID_Int64);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = _TD_Int64->Initialize (&kAAFTypeID_Int64, 8, AAFTrue, L"aafInt64");
	  if (AAFRESULT_FAILED(hr)) return hr;
	}
  assert (_TD_Int64);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_Int64;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefObjRef (ImplAAFTypeDef ** ptd)
{
  if (!_TD_ObjRef)
	{
	  AAFRESULT hr;

	  // _TD_ObjRef = (ImplAAFTypeDefStrongObjRef *)
	  //   _dictionary->CreateImplObject(AUID_AAFTypeDefStrongObjRef);
	  // if (! _TD_ObjRef) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefStrongObjRef);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_ObjRef = static_cast<ImplAAFTypeDefStrongObjRef*>(pObj);
	  assert (_TD_ObjRef);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_ObjRef->SetAUID (&kAAFTypeID_ObjRef);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = _TD_ObjRef->pvtInitialize (&kAAFTypeID_ObjRef,
									  &AUID_AAFObject,
									  L"Strong Object Reference");
	  if (AAFRESULT_FAILED(hr)) return hr;
	}
  assert (_TD_ObjRef);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_ObjRef;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefObjRefArray (ImplAAFTypeDef ** ptd)
{
  if (!_TD_ObjRefArray)
	{
	  AAFRESULT hr;

	  // _TD_ObjRefArray = (ImplAAFTypeDefVariableArray *) _dictionary->CreateImplObject(AUID_AAFTypeDefVariableArray);
	  // if (! _TD_ObjRefArray) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefVariableArray);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_ObjRefArray = static_cast<ImplAAFTypeDefVariableArray*>(pObj);
	  assert (_TD_ObjRefArray);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_ObjRefArray->SetAUID (&kAAFTypeID_ObjRefArray);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  ImplAAFTypeDef * ptdo = NULL;
	  // hr = TypeDefObjRef (&ptdo);
	  hr = _dictionary->LookupType (&kAAFTypeID_ObjRef, &ptdo);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (ptdo);
	  hr = _TD_ObjRefArray->Initialize (&kAAFTypeID_ObjRefArray, ptdo, L"Strong Object Reference Array");
	  if (AAFRESULT_FAILED(hr)) return hr;
	  ptdo->ReleaseReference ();
	}
  assert (_TD_ObjRefArray);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_ObjRefArray;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefAUIDArray (ImplAAFTypeDef ** ptd)
{
  if (!_TD_AUIDArray)
	{
	  AAFRESULT hr;

	  // _TD_AUIDArray = (ImplAAFTypeDefVariableArray *) _dictionary->CreateImplObject(AUID_AAFTypeDefVariableArray);
	  // if (! _TD_AUIDArray) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefVariableArray);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_AUIDArray = static_cast<ImplAAFTypeDefVariableArray*>(pObj);
	  assert (_TD_AUIDArray);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_AUIDArray->SetAUID (&kAAFTypeID_AUIDArray);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  ImplAAFTypeDef * ptda = NULL;
	  // hr = TypeDefAUID (&ptda);
	  hr = _dictionary->LookupType (&kAAFTypeID_AUID, &ptda);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (ptda);
	  hr = _TD_AUIDArray->Initialize (&kAAFTypeID_AUIDArray, ptda, L"AUID Array");
	  if (AAFRESULT_FAILED(hr)) return hr;
	  ptda->ReleaseReference ();
	}
  assert (_TD_AUIDArray);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_AUIDArray;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefUInt8Array (ImplAAFTypeDef ** ptd)
{
  if (!_TD_UInt8Array)
	{
	  AAFRESULT hr;
	  // _TD_UInt8Array = (ImplAAFTypeDefVariableArray *) _dictionary->CreateImplObject(AUID_AAFTypeDefVariableArray);
	  // if (! _TD_UInt8Array) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefVariableArray);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_UInt8Array = static_cast<ImplAAFTypeDefVariableArray*>(pObj);
	  assert (_TD_UInt8Array);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_UInt8Array->SetAUID (&kAAFTypeID_UInt8Array);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  ImplAAFTypeDef * ptd8 = NULL;
	  // hr = TypeDefUInt8 (&ptd8);
	  hr = _dictionary->LookupType (&kAAFTypeID_UInt8, &ptd8);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (ptd8);
	  hr = _TD_UInt8Array->Initialize (&kAAFTypeID_UInt8Array, ptd8, L"UInt8 Array");
	  if (AAFRESULT_FAILED(hr)) return hr;
	  ptd8->ReleaseReference ();
	}
  assert (_TD_UInt8Array);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_UInt8Array;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefUInt8Array8 (ImplAAFTypeDef ** ptd)
{
  if (!_TD_UInt8Array8)
	{
	  AAFRESULT hr;

	  // _TD_UInt8Array8 = (ImplAAFTypeDefFixedArray *) _dictionary->CreateImplObject(AUID_AAFTypeDefFixedArray);
	  // if (! _TD_UInt8Array8) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefFixedArray);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_UInt8Array8 = static_cast<ImplAAFTypeDefFixedArray*>(pObj);
	  assert (_TD_UInt8Array8);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_UInt8Array8->SetAUID (&kAAFTypeID_UInt8Array8);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  ImplAAFTypeDef * ptd8 = NULL;
	  // hr = TypeDefUInt8 (&ptd8);
	  hr = _dictionary->LookupType (&kAAFTypeID_UInt8, &ptd8);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (ptd8);
	  hr = _TD_UInt8Array8->Initialize (&kAAFTypeID_UInt8Array8, ptd8, 8, L"UInt8[8]");
	  if (AAFRESULT_FAILED(hr)) return hr;
	  ptd8->ReleaseReference ();
	}
  assert (_TD_UInt8Array8);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_UInt8Array8;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefWCharString (ImplAAFTypeDef ** ptd)
{
  if (!_TD_WCharString)
	{
	  AAFRESULT hr;
	  // _TD_WCharString = (ImplAAFTypeDefString *) _dictionary->CreateImplObject(AUID_AAFTypeDefString);
	  // if (! _TD_WCharString) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefString);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_WCharString = static_cast<ImplAAFTypeDefString*>(pObj);
	  assert (_TD_WCharString);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_WCharString->SetAUID (&kAAFTypeID_WCharString);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  ImplAAFTypeDef * ptd16 = NULL;
	  // hr = TypeDefUInt16 (&ptd16);
	  hr = _dictionary->LookupType (&kAAFTypeID_UInt16, &ptd16);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (ptd16);
	  hr = _TD_WCharString->Initialize (&kAAFTypeID_WCharString, ptd16, L"Wide Char String");
	  if (AAFRESULT_FAILED(hr)) return hr;
	  ptd16->ReleaseReference ();
	}
  assert (_TD_WCharString);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_WCharString;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT
ImplAAFBuiltinTypes::TypeDefFadeType (ImplAAFTypeDef ** ptd)
{
  if (!_TD_FadeType)
	{
	  AAFRESULT hr;

	  // _TD_FadeType = (ImplAAFTypeDefEnum *) _dictionary->CreateImplObject (AUID_AAFTypeDefEnum);
	  // if (! _TD_FadeType) return AAFRESULT_NOMEMORY;

	  assert (_dictionary);
	  ImplAAFObject * pObj =
		_dictionary->pvtCreateBaseClassInstance (&AUID_AAFTypeDefEnum);
	  if (! pObj) return AAFRESULT_NOMEMORY;
	  pObj->setClassFactory(_dictionary);

	  _TD_FadeType = static_cast<ImplAAFTypeDefEnum*>(pObj);
	  assert (_TD_FadeType);

	  // Hack so later recursive lookup will succeed
	  hr = _TD_FadeType->SetAUID (&kAAFTypeID_FadeType);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  ImplAAFTypeDef * ptd32 = NULL;
	  // hr = TypeDefUInt32 (&ptd32);
	  hr = _dictionary->LookupType (&kAAFTypeID_UInt32, &ptd32);
	  if (AAFRESULT_FAILED(hr)) return hr;
	  assert (ptd32);
	  aafInt64 elemVals[] =
	  {
		kFadeNone,
		kFadeLinearAmp,
		kFadeLinearPower
	  };
	  aafString_t elemNames[] =
	  {
		L"kFadeNone",
		L"kFadeLinearAmp",
		L"kFadeLinearPower"
	  };
	  int numElems = sizeof (elemVals) / sizeof(elemVals[0]);
	  assert ((sizeof (elemNames) / sizeof (elemNames[0])) == numElems);
	  hr = _TD_FadeType->Initialize (&kAAFTypeID_FadeType,
									 ptd32,
									 elemVals,
									 elemNames,
									 numElems,
									 L"FadeType");
	  if (AAFRESULT_FAILED(hr)) return hr;
	  ptd32->ReleaseReference ();
	}
  assert (_TD_FadeType);
  if (! ptd) return AAFRESULT_NULL_PARAM;
  *ptd = _TD_FadeType;
  (*ptd)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


//
// Builtin type-def lookup
//

//
// Eventually, this table should be automatically generated from some
// MetaDictionary-like entity.
//
/*const*/ /*static*/ ImplAAFBuiltinTypes::TypeTblEntry
ImplAAFBuiltinTypes::sBuiltinTypeTable[] =
{
  { &kAAFTypeID_AUID, TypeDefAUID },
  { &kAAFTypeID_AUIDArray, TypeDefAUIDArray },
  { &kAAFTypeID_FadeType, TypeDefFadeType },
  { &kAAFTypeID_Int16, TypeDefInt16 },
  { &kAAFTypeID_Int32, TypeDefInt32 },
  { &kAAFTypeID_Int64, TypeDefInt64 },
  { &kAAFTypeID_ObjRef, TypeDefObjRef },
  { &kAAFTypeID_ObjRefArray, TypeDefObjRefArray },
  { &kAAFTypeID_UInt16, TypeDefUInt16 },
  { &kAAFTypeID_UInt32, TypeDefUInt32 },
  { &kAAFTypeID_UInt8, TypeDefUInt8 },
  { &kAAFTypeID_UInt8Array, TypeDefUInt8Array },
  { &kAAFTypeID_UInt8Array8, TypeDefUInt8Array8 },
  { &kAAFTypeID_WCharString, TypeDefWCharString },
};

AAFRESULT ImplAAFBuiltinTypes::ImportBuiltinTypeDef (const aafUID_t & rTypeID,
													 ImplAAFTypeDef ** ppResult)
{
  //
  // It's important that we perform the following in this order:
  // (1) Create object, without initializing the OM properties
  // (2) Register the object in the dict, even before OM props are
  //     initialized
  // (3) Init OM properties *after* the type has been registered.
  //
  aafUInt32 i;
  const aafUInt32 kNumTypes =
	sizeof (sBuiltinTypeTable) / sizeof (sBuiltinTypeTable[0]);

  for (i = 0; i < kNumTypes; i++)
	{
	  if (EqualAUID (sBuiltinTypeTable[i].pID, &rTypeID))
		{
		  ImplAAFTypeDef * ptd = 0;
		  // (1) create
		  pTypeFunc_t pFunc = sBuiltinTypeTable[i].pTypeFunc;
		  AAFRESULT hr =
			(this->*pFunc)(&ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  assert (_dictionary);
		  // (2) register
		  hr = _dictionary->RegisterType (ptd);
		  if (AAFRESULT_FAILED (hr))
			{
			  assert (ptd);
			  ptd->ReleaseReference ();
			  return hr;
			}

		  assert (ptd);
		  // (3) init OM props
		  ImplAAFObjectSP objSP;
		  objSP = ptd;
		  _dictionary->pvtInitObjectProperties (objSP);

		  assert (ppResult);
		  *ppResult = ptd;
		  return AAFRESULT_SUCCESS;
		}
	}
  // not found
  return AAFRESULT_NO_MORE_OBJECTS;
}
