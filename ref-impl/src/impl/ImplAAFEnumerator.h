#ifndef __ImplAAFEnumerator_h__
#define __ImplAAFEnumerator_h__
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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFObjectCreation.h"
#include "ImplAAFRoot.h"
#include "OMReferenceContainerIter.h"

template<class T>
class ImplAAFEnumerator: public ImplAAFRoot
{
public:
	ImplAAFEnumerator();

protected:
	virtual ~ImplAAFEnumerator();
	
public:
	AAFRESULT STDMETHODCALLTYPE Initialize(const aafClassID_t* pClassID,
		ImplAAFRoot *pObj,OMReferenceContainerIterator* pIterator);

	AAFRESULT STDMETHODCALLTYPE NextOne (T ** ppItem);
	AAFRESULT STDMETHODCALLTYPE Next(aafUInt32  count, T ** ppItems,
		aafUInt32 *  pFetched);
	AAFRESULT STDMETHODCALLTYPE Skip(aafUInt32  count);
	AAFRESULT STDMETHODCALLTYPE Reset ();
	AAFRESULT STDMETHODCALLTYPE Clone(ImplAAFEnumerator<T> ** ppEnum);

protected:
	const aafClassID_t* _pClassID; // needed for Clone() 
	ImplAAFRoot* _pObj;
	OMReferenceContainerIterator* _pIterator;
};

template <class T>
ImplAAFEnumerator<T>::ImplAAFEnumerator()
: _pClassID(0), _pObj(0), _pIterator(0)
{
}

template <class T>
ImplAAFEnumerator<T>::~ImplAAFEnumerator ()
{
	if(_pObj)
	{
		_pObj->ReleaseReference();
		_pObj=NULL;
	}

	delete(_pIterator);
	_pIterator=0;
}

template <class T>
AAFRESULT STDMETHODCALLTYPE 
	ImplAAFEnumerator<T>::Initialize(const aafClassID_t* pClassID,
		ImplAAFRoot *pObj,OMReferenceContainerIterator* pIterator)
{
	AAFRESULT ar=AAFRESULT_SUCCESS;
	
	if(_pObj)
	{
		_pObj->ReleaseReference();
		_pObj = 0;
	}
	
	_pObj = pObj;
	if(pObj)
		pObj->AcquireReference();
	
	delete(_pIterator);
	_pIterator = pIterator;

	_pClassID=pClassID;

	return(ar);
}

template <class T>
AAFRESULT STDMETHODCALLTYPE	
	ImplAAFEnumerator<T>::NextOne(
		T **ppItem)
{
	if(ppItem==NULL)
		return(AAFRESULT_NULL_PARAM);

	if(_pIterator==NULL)
		return(AAFRESULT_NOT_INITIALIZED);

	AAFRESULT ar=AAFRESULT_NO_MORE_OBJECTS;
	
	if (_pIterator->before() || _pIterator->valid())
	{
		if (++(*_pIterator))
		{
			OMObject* object = _pIterator->currentObject();
			T* obj = dynamic_cast<T*>(object);
			// assert(obj != 0); // tjb - consistent way to handle this ?
			*ppItem = obj;
			(*ppItem)->AcquireReference();
			ar = AAFRESULT_SUCCESS;
		}
	}
	return(ar);
}

template <class T>
AAFRESULT STDMETHODCALLTYPE
	ImplAAFEnumerator<T>::Next(
		aafUInt32  count,
		T **ppItems,
		aafUInt32 *pFetched)
{
	aafUInt32			numItems;
	AAFRESULT			ar=AAFRESULT_SUCCESS;

	if(ppItems==NULL||pFetched==NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(count==0)
		return(AAFRESULT_INVALID_PARAM);

	for (numItems = 0; numItems < count; numItems++)
	{
		ar = NextOne(&ppItems[numItems]);
		if (FAILED(ar))
			break;
	}
	
	if (pFetched)
		*pFetched=numItems;

	return(ar);
}

template <class T>
AAFRESULT STDMETHODCALLTYPE
	ImplAAFEnumerator<T>::Skip (
		aafUInt32  count)
{
	if(_pIterator==NULL)
		return(AAFRESULT_NOT_INITIALIZED);

	if(count==0)
		return(AAFRESULT_INVALID_PARAM);

	AAFRESULT ar=AAFRESULT_SUCCESS;
	aafUInt32 n;
	
	for(n=1;n<=count;n++)
	{
		// Defined behavior of skip is to NOT advance at all if it would push 
		// us off of the end
		if(!++(*_pIterator))
		{
			// Off the end, decrement n and iterator back to the starting 
			// position
			while(n>=1)
			{
				--(*_pIterator);
				n--;
			}
			ar=AAFRESULT_NO_MORE_OBJECTS;
			break;
		}
	}

	return(ar);
}

template <class T>
AAFRESULT STDMETHODCALLTYPE
	ImplAAFEnumerator<T>::Reset ()
{
	if(_pIterator==NULL)
		return(AAFRESULT_NOT_INITIALIZED);

	AAFRESULT ar=AAFRESULT_SUCCESS;
	_pIterator->reset();
	return(ar);
}

template <class T>
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEnumerator<T>::Clone (
      ImplAAFEnumerator<T> **ppEnum)
{
	if(_pIterator==NULL||_pClassID==NULL||_pObj==NULL)
		return(AAFRESULT_NOT_INITIALIZED);

	AAFRESULT ar=AAFRESULT_SUCCESS;
	ImplAAFEnumerator<T> *pResult;

	pResult=dynamic_cast<ImplAAFEnumerator<T>*>(CreateImpl(*_pClassID));
	if(pResult==NULL)
		return E_FAIL;

    ar=pResult->Initialize(_pClassID,_pObj,_pIterator->copy());
	if(SUCCEEDED(ar))
	{
		*ppEnum=pResult;
	}
	else
	{
	  pResult->ReleaseReference();
	  pResult=0;
	  *ppEnum=NULL;
	}
	
	return(ar);
}

#endif // ! __ImplAAFEnumerator_h__
