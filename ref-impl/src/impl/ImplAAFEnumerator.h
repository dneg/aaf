#ifndef __ImplAAFEnumerator_h__
#define __ImplAAFEnumerator_h__
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFCollection_h__
#include "ImplAAFCollection.h"
#endif

//
// Standard enumerator implementation which works with elements of
// type T, and works on an ImplAAFCollection<T>.  The class
// ImplAAFCollection<T> must have the following methods defined:
//
// - AAFRESULT GetNumElements([out] aafInt32 * pCount);
//
// - AAFRESULT GetNthElement([in] aafInt32 index,
//                           [out] T * pElem);
// 


template <typename T>
class ImplAAFEnumerator
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEnumerator (ImplAAFCollection<T> * pCollection);

  virtual ~ImplAAFEnumerator ();

public:

  //****************
  // NextOne()
  //
  virtual AAFRESULT
    NextOne
        // @parm [out,retval] The Next element
        (T * pElem);


  //****************
  // Next()
  //
  virtual AAFRESULT
    Next
        (// @parm [in] number of elements requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pNumFetched)] array to receive elements
         T * pElems,

         // @parm [out,ref] number of actual elements fetched into pElems array
         aafUInt32 *  pNumFetched);


  //****************
  // Skip()
  //
  virtual AAFRESULT
    Skip
        // @parm [in] Number of elements to skip
        (aafUInt32  count);


  //****************
  // Reset()
  //
  virtual AAFRESULT
    Reset ();

private:
  aafInt32            _current;
  ImplAAFCollection<T> * _pCollection;
};


//
// Implementation stuff
//

template <typename T>
ImplAAFEnumerator<T>::ImplAAFEnumerator
(
 ImplAAFCollection<T> * pCollection
)
  : _pCollection (pCollection),
	_current (0)
{}


template <typename T>
ImplAAFEnumerator<T>::~ImplAAFEnumerator ()
{}


template <typename T>
AAFRESULT ImplAAFEnumerator<T>::NextOne
(
 T * pElem
)
{
  AAFRESULT	result;
  aafUInt32	cur = _current;
  aafUInt32  siz;

  if (pElem == NULL)
	return AAFRESULT_NULL_PARAM;

  if (!_pCollection) return AAFRESULT_NULL_PARAM;
  _pCollection->GetNumElements(&siz);
  if(cur < siz)
	{
	  result = _pCollection->GetNthElement(cur, pElem);
	  _current = ++cur;
	}
  else
	result = AAFRESULT_NO_MORE_OBJECTS;

  return result;
}


template <typename T>
AAFRESULT ImplAAFEnumerator<T>::Next
( aafUInt32 count,
  T * pElems,
  aafUInt32 * pFetched)
{
  T * pElem;
  aafUInt32 numElems;
  HRESULT hr;

  if (pFetched == NULL && count != 1)
	return AAFRESULT_NULL_PARAM;

  // Point at the first element in the array.
  pElem = pElems;
  for (numElems = 0; numElems < count; numElems++)
	{
	  hr = NextOne(pElem);
	  if (FAILED(hr))
		break;

	  // Point at the next element in the array.  This
	  // will increment off the end of the array when
	  // numElems == count-1, but the for loop should
	  // prevent access to this location.
	  pElem++;
	}
	
  if (pFetched)
	*pFetched = numElems;

  return hr;
}


template <typename T>
AAFRESULT ImplAAFEnumerator<T>::Skip
(
 aafUInt32  count
)
{
  AAFRESULT	hr;
  aafUInt32	newCurrent;
  aafUInt32 size;

  newCurrent = _current + count;

  _pCollection->GetNumElements(&size);
  if(newCurrent < size)
	{
	  _current = newCurrent;
	  hr = AAFRESULT_SUCCESS;
	}
  else
	{
	  hr = AAFRESULT_NO_MORE_OBJECTS;
	}

  return hr;
}


template <typename T>
AAFRESULT ImplAAFEnumerator<T>::Reset()
{
  _current = 0;
  return AAFRESULT_SUCCESS;
}


#endif // ! __ImplAAFEnumerator_h__
