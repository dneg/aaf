#ifndef __ImplAAFEnumerator_h__
#define __ImplAAFEnumerator_h__
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

	virtual AAFRESULT STDMETHODCALLTYPE NextOne (T ** ppItem);
	virtual AAFRESULT STDMETHODCALLTYPE Next(aafUInt32  count, T ** ppItems,
		aafUInt32 *  pFetched);
	virtual AAFRESULT STDMETHODCALLTYPE Skip(aafUInt32  count);
	virtual AAFRESULT STDMETHODCALLTYPE Reset ();
	virtual AAFRESULT STDMETHODCALLTYPE Clone(ImplAAFEnumerator<T> ** ppEnum);

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
