#ifndef _AAF_DOMAIN_UTILS_
#define _AAF_DOMAIN_UTILS_
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

/************************************************************************
 *
 * Omf2Aaf.h Describes structures and prototypes for the OMF to AAF 
 *          conversion utility.
 *
 ************************************************************************/

class AAFDomainUtils
{
public:
	AAFDomainUtils();
	virtual ~AAFDomainUtils();
	virtual IAAFInterpolationDef *CreateInterpolationDefinition(IAAFDictionary *dict, aafUID_t interpolationDefID);

	virtual IAAFTypeDef *CreateTypeDefinition(IAAFDictionary *pDict, aafUID_t typeDefID);
	virtual IAAFVaryingValue *AAFAddEmptyVaryingVal(IAAFDictionary *dict, IAAFParameterDef *pParameterDef, IAAFOperationGroup *pOutputEffect);
	virtual IAAFParameter *AAFAddConstantVal(IAAFDictionary *dict, IAAFParameterDef *pParameterDef, long buflen, void *buf, IAAFOperationGroup *pGroup);
	virtual void AAFAddOnePoint(IAAFDictionary *dict, aafRational_t percentTime, long buflen, void *buf, IAAFTypeDef *typeDef, IAAFVaryingValue *pVVal);
	virtual IAAFParameterDef *CreateParameterDefinition(IAAFDictionary *dict, aafUID_t parmDefID);
	virtual HRESULT SetIntegerPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, aafUID_t* pPropID, const aafUID_t* pIntTypeID,
							   aafMemPtr_t pValue, aafUInt32 ValueSize, IAAFDictionary *dict);

	virtual HRESULT GetIntegerPropFromObject(IAAFObject* pObj, const aafUID_t* pClassID, aafUID_t* pPropID,
								 const aafUID_t* pIntTypeID, aafMemPtr_t pValue, aafUInt32 ValueSize, IAAFDictionary *dict);
	virtual void RegisterAAFProperties(IAAFDictionary * /*dict*/) { };
	virtual HRESULT AddPropertyToClass(IAAFDictionary *dict, const aafUID_t* pClassID, const aafUID_t* pPropTypeID, const aafUID_t* pPropID, aafCharacter*  pName);
	virtual HRESULT SetObjRefArrayPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, const aafUID_t* pElementTypeID, aafUID_t* pPropID, IAAFObject** pArray, aafUInt32 numObjects);
	virtual HRESULT GetObjRefArrayPropFromObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject*** pArray, aafUInt32* pNumObjects);
	virtual HRESULT SetObjRefPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject* pValue);
	virtual HRESULT GetObjRefPropFromObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject** ppObject);

	virtual void SetDictionary(IAAFDictionary *dict);
protected:
	IAAFDictionary	*_dict;

private:
};
#endif
