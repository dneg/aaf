//@doc
//@class    EnumAAFProperties | Implementation class for EnumAAFProperties
#ifndef __ImplEnumAAFProperties_h__
#define __ImplEnumAAFProperties_h__

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

#include "ImplAAFEnumerator.h"

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
class OldImplAAFEnumerator
{
public:
  //
  // Constructor/destructor
  //
  //********
  OldImplAAFEnumerator (ImplAAFCollection<T> * pCollection);

  virtual ~OldImplAAFEnumerator ();

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
OldImplAAFEnumerator<T>::OldImplAAFEnumerator
(
 ImplAAFCollection<T> * pCollection
)
  : _pCollection (pCollection),
	_current (0)
{}


template <typename T>
OldImplAAFEnumerator<T>::~OldImplAAFEnumerator ()
{}


template <typename T>
AAFRESULT OldImplAAFEnumerator<T>::NextOne
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
AAFRESULT OldImplAAFEnumerator<T>::Next
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
AAFRESULT OldImplAAFEnumerator<T>::Skip
(
 aafUInt32  count
)
{
  AAFRESULT	hr;
  aafUInt32	newCurrent;
  aafUInt32 size;

  newCurrent = _current + count;

  _pCollection->GetNumElements(&size);
  // Matt: changed from < to <=
  if(newCurrent <= size)
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
AAFRESULT OldImplAAFEnumerator<T>::Reset()
{
  _current = 0;
  return AAFRESULT_SUCCESS;
}

class ImplAAFProperty;


template <typename T> class ImplAAFCollection;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplEnumAAFProperties : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFProperties ();

protected:
  virtual ~ImplEnumAAFProperties ();

public:

  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next Property
        (ImplAAFProperty ** ppProperty);


  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of properties requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pNumFetched)] array to receive properties
         ImplAAFProperty ** ppProperties,

         // @parm [out,ref] number of actual Properties fetched into ppProperties array
         aafUInt32 *  pNumFetched);


  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
        // @parm [in] Number of elements to skip
        (aafUInt32  count);


  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();



  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
        // @parm [out,retval] new enumeration
        (ImplEnumAAFProperties ** ppEnum);


public:
  AAFRESULT Initialize (ImplAAFCollection<ImplAAFProperty*> * pProperties);

private:
  OldImplAAFEnumerator<ImplAAFProperty*> * _rep;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplEnumAAFProperties> ImplEnumAAFPropertiesSP;

#endif // ! __ImplEnumAAFProperties_h__
