/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


#include <assert.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "OMFile.h"
#include "ImplAAFHeader.h"


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"



ImplAAFObject::ImplAAFObject ()
{}


ImplAAFObject::~ImplAAFObject ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::SetGeneration (aafUID_t *  /*pGeneration*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetGeneration (aafUID_t *  /*pGeneration*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetObjectClass (aafUID_t * pClass)
{
  if (NULL == pClass)
    return AAFRESULT_NULL_PARAM;

  //
  // Take advantage of the virtual method that 
  // returns the stored class id for a given class of OMStorable.
  //
#ifndef _DEBUG
  *pClass = *reinterpret_cast<const aafUID_t *>(&classId());
#else
  // In a debug build copy all of the fields separately so
  // that we can detect changes in the definitions of public aafUID_t
  // and private OMClassId types.
  const OMClassId& id = classId();
  pClass->Data1 = id.Data1;
  pClass->Data2 = id.Data2;
  pClass->Data3 = id.Data3;
  pClass->Data4[0] = id.Data4[0];
  pClass->Data4[1] = id.Data4[1];
  pClass->Data4[2] = id.Data4[2];
  pClass->Data4[3] = id.Data4[3];
  pClass->Data4[4] = id.Data4[4];
  pClass->Data4[5] = id.Data4[5];
  pClass->Data4[6] = id.Data4[6];
  pClass->Data4[7] = id.Data4[7];
#endif

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::EnumProperties (ImplEnumAAFProperties ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetStoredByteOrder (eAAFByteOrder_t *  /*pOrder*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetNativeByteOrder (eAAFByteOrder_t *  /*pOrder*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

//************
// Interfaces ivisible inside the toolkit, but not exposed through the API

// Gets the head object of the file containing this object.
// This function is used to maintain MOB and Definition tables in the
// head object.
AAFRESULT ImplAAFObject::MyHeadObject
	(ImplAAFHeader **header)
{
	OMFile			*myFile;
	OMStorable		*theRoot;
	ImplAAFHeader	*theHeader;

	XPROTECT()
	{
		if(header == NULL)
			RAISE(AAFRESULT_NULL_PARAM);

		if(!attached())
			RAISE(AAFRESULT_OBJECT_NOT_ATTACHED); 

                if(!inFile())
			RAISE(AAFRESULT_OBJECT_NOT_IN_FILE); 

		myFile = file();
		if(myFile == NULL)
			RAISE(AAFRESULT_OBJECT_NOT_IN_FILE);

		theRoot = myFile->root();
		if(theRoot == NULL)
			RAISE(AAFRESULT_BADHEAD);

		theHeader = dynamic_cast<ImplAAFHeader*>(theRoot);
		if(theRoot == NULL)
			RAISE(AAFRESULT_BADHEAD);
		theHeader->AcquireReference();

		*header = theHeader;
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS);
}

//
// Define the symbol for the stored object id
//
OMDEFINE_STORABLE(ImplAAFObject, AUID_AAFObject);
