#ifndef _AAF_DOMAIN_UTILS_
#define _AAF_DOMAIN_UTILS_
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

public:

	IAAFInterpolationDef *CreateInterpolationDefinition(IAAFDictionary *dict, aafUID_t interpolationDefID);

	virtual IAAFTypeDef *CreateTypeDefinition(IAAFDictionary *pDict, aafUID_t typeDefID);
	IAAFVaryingValue *AAFAddEmptyVaryingVal(IAAFDictionary *dict, IAAFParameterDef *pParameterDef, IAAFOperationGroup *pOutputEffect);
	IAAFParameter *AAFAddConstantVal(IAAFDictionary *dict, IAAFParameterDef *pParameterDef, long buflen, void *buf, IAAFOperationGroup *pGroup);
	void AAFAddOnePoint(IAAFDictionary *dict, aafRational_t percentTime, long buflen, void *buf, IAAFTypeDef *typeDef, IAAFVaryingValue *pVVal);
	virtual IAAFParameterDef *CreateParameterDefinition(IAAFDictionary *dict, aafUID_t parmDefID);
	HRESULT SetIntegerPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, aafUID_t* pPropID, const aafUID_t* pIntTypeID,
							   aafMemPtr_t pValue, aafUInt32 ValueSize, IAAFDictionary *dict);

	HRESULT GetIntegerPropFromObject(IAAFObject* pObj, const aafUID_t* pClassID, aafUID_t* pPropID,
								 const aafUID_t* pIntTypeID, aafMemPtr_t pValue, aafUInt32 ValueSize, IAAFDictionary *dict);
	virtual void RegisterAAFProperties(IAAFDictionary * dict) { };
	HRESULT AddPropertyToClass(IAAFDictionary *dict, const aafUID_t* pClassID, const aafUID_t* pPropTypeID, const aafUID_t* pPropID, aafCharacter*  pName);
	HRESULT SetObjRefArrayPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, const aafUID_t* pElementTypeID, aafUID_t* pPropID, IAAFObject** pArray, aafUInt32 numObjects);
	HRESULT GetObjRefArrayPropFromObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject*** pArray, aafUInt32* pNumObjects);
	HRESULT SetObjRefPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject* pValue);
	HRESULT GetObjRefPropFromObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject** ppObject);

	void SetDictionary(IAAFDictionary *dict);
protected:
	IAAFDictionary	*_dict;

private:
};
#endif