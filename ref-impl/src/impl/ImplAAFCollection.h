#ifndef __ImplAAFCollection_h__
#define __ImplAAFCollection_h__
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


//
// Standard collection implementaiton for use with the standard
// enumerator (ImplAAFEnumerator<T>).
//
// - AAFRESULT GetNumElements([out] aafInt32 * pCount);
//
// - AAFRESULT GetNthElement([in] aafInt32 index,
//                           [out] T * pElem);
// 


template <typename T>
class ImplAAFCollection
{
public:
  //
  // Constructor/destructor
  //
  //********
  // ImplAAFCollection ();

  virtual ~ImplAAFCollection/*<T>*/  () {}

public:

  //****************
  // GetNumElements()
  //
  virtual AAFRESULT
    GetNumElements
        // @parm [out,retval] Return the count of elements
        (aafUInt32 * pCount) = 0;


  //****************
  // GetNthElement()
  //
  virtual AAFRESULT
    GetNthElement
        (// @parm [in] zero-based index into collection
         aafUInt32  index,

         // @parm [out] returned element
         T * pElem) = 0;
};


#if 0 // Not ready for these derived types yet
template <typename T>
class ImplAAFCollectionData : public ImplAAFCollection<T>
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCollectionData<T>
    (T * pData,
     aafUInt32 numElems);

  virtual ~ImplAAFCollectionData<T> ();

public:

  // overrides
  AAFRESULT
    GetNumElements
        // @parm [out,retval] Return the count of elements
        (aafUInt32 * pCount);

  virtual AAFRESULT
    GetNthElement
        (// @parm [in] zero-based index into collection
         aafUInt32  index,

         // @parm [out] returned element
         T * pElem);

private:
  T * _pData;
  aafUInt32 _numElems;
};


template <typename T, typename PropElemType>
class ImplAAFCollectionOMVector : public ImplAAFCollection<T>
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCollectionOMVector<T, PropElemType>
    (OMStrongReferenceVectorProperty<PropElemType> * pProp);

  virtual ~ImplAAFCollectionOMVector<T, PropElemType> ();

public:

  // overrides
  AAFRESULT
    GetNumElements
        // @parm [out,retval] Return the count of elements
        (aafUInt32 * pCount);

  virtual AAFRESULT
    GetNthElement
        (// @parm [in] zero-based index into collection
         aafUInt32  index,

         // @parm [out] returned element
         T * pElem);

private:
  OMStrongReferenceVectorProperty<PropElemType> * _pProp;
};


//
// Implementation for ImplAAFCollectionData
//

template <typename T>
ImplAAFCollectionData<T>::ImplAAFCollectionData<T>
(
 T * pData,
 aafUInt32 numElems
)
  : _pData (pData),
	_numElems (numElems)
{
  assert (pData);
}


template <typename T>
ImplAAFCollectionData<T>::~ImplAAFCollectionData<T> ()
{}


template <typename T> AAFRESULT
ImplAAFCollectionData<T>::GetNumElements
(
 aafUInt32 * pCount
)
{
  if (! pCount)
	return AAFRESULT_NULL_PARAM;

  *pCount =_numElems;
  return AAFRESULT_SUCCESS;
}


template <typename T> AAFRESULT
ImplAAFCollectionData<T>::GetNthElement
(
 aafUInt32  index,
 T * pElem
)
{
  if (! pElem)
	return AAFRESULT_NULL_PARAM;

  aafUInt32 num;
  GetNumElements (&num);
  if (index >= num)
	return AAFRESULT_NO_MORE_OBJECTS;

  *pElem = _pData[index];

  return AAFRESULT_SUCCESS;
}


//
// Implementation for ImplAAFCollectionOMVector
//

template <typename T, typename PropElemType>
ImplAAFCollectionOMVector<T, PropElemType>::ImplAAFCollectionOMVector<T, PropElemType>
(
 OMStrongReferenceVectorProperty<PropElemType> * pProp
)
  : _pProp (pProp)
{
  assert (pProp);
}


template <typename T, typename PropElemType>
ImplAAFCollectionOMVector<T, PropElemType>::~ImplAAFCollectionOMVector<T, PropElemType> ()
{}


template <typename T, typename PropElemType> AAFRESULT
ImplAAFCollectionOMVector<T, PropElemType>::GetNumElements
(
 aafUInt32 * pCount
)
{
  if (! pCount)
	return AAFRESULT_NULL_PARAM;

  *pCount =_components.getSize(numCpnts);
  return AAFRESULT_SUCCESS;
}


template <typename T, typename PropElemType> AAFRESULT
ImplAAFCollectionOMVector<T, PropElemType>::GetNthElement
(
 aafUInt32  index,
 T * pElem
)
{
  aafUInt32 num;

  if (! pElem)
	return AAFRESULT_NULL_PARAM;

  aafUInt32 num;
  GetNumElements (&num);
  if (index >= num)
	return AAFRESULT_NO_MORE_OBJECTS;

  _components.getValueAt(*pElem, index);
  (*pElem)->AcquireReference();

  return AAFRESULT_SUCCESS;
}
#endif //0


#endif // ! __ImplAAFCollection_h__
