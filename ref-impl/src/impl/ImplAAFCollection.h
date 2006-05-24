#ifndef __ImplAAFCollection_h__
#define __ImplAAFCollection_h__
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


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
  ASSERTU (pData);
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
  ASSERTU (pProp);
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
