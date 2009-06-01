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

#ifndef __ImplAAFSearchableStack_h__
#define __ImplAAFSearchableStack_h__


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif


//
// Intended to be a "searchable stack", that is one where the client
// can determine if an element is already on the stack.  Tests are
// done by equality.
//
// The type with which this template is instantiated must support the
// following publicly-available operations:
//
// equality ( operator== )
// assignment ( operator= )
// default construction
// destruction
//


template <typename T>
class ImplAAFSearchableStack
{
public:
  // ctors, dtor, assignment
  ImplAAFSearchableStack ();
  ~ImplAAFSearchableStack ();
  ImplAAFSearchableStack (const ImplAAFSearchableStack & src);
  const ImplAAFSearchableStack operator= (const ImplAAFSearchableStack & src);

  // Pushes item onto the stack.
  void push (const T & item);

  // Removes the most recently push()ed item which is still present
  // from the stack, and returns it.  Requires the stack to be not
  // empty (that is, size() must return non-zero).
  T pop ();

  // Returns the number of items on the stack.  Returns zero if this
  // stack is empty.
  inline aafUInt32 size () const;

  // Returns true if item is present on stack; returns false
  // otherwise.  Will return false if this stack is empty.
  bool isPresent (const T & item) const;

private:
  T *       _rep;
  aafUInt32 _numElems;
  aafUInt32 _numAllocated;
};

//
// Implementation
//

//
// Our Mac setup doesn't allow #including assert.h in any header file,
// so we'll implement our own assertions using exceptions.
//
#ifndef AAF_STACK_ASSERT
  #ifdef  NDEBUG
    #define AAF_STACK_ASSERT(condition) \
      ((void) 0)
  #else
    #define AAF_STACK_ASSERT(condition) \
      if (! (condition)) throw #condition
  #endif // ! NDEBUG
#endif // ! AAF_STACK_ASSERT


template <typename T>
ImplAAFSearchableStack<T>::ImplAAFSearchableStack ()
: _rep (0),
  _numElems (0),
  _numAllocated (0)
{}


template <typename T>
ImplAAFSearchableStack<T>::~ImplAAFSearchableStack ()
{
  delete[] _rep;
}


template <typename T>
ImplAAFSearchableStack<T>::ImplAAFSearchableStack (const ImplAAFSearchableStack & src)
: _rep(0),
  _numElems (src._numElems),
  _numAllocated (src._numElems)
{
  _rep = new T[_numAllocated];
  for (aafUInt32 i = 0; i < _numElems; i++)
	_rep[i] = src._rep[i];
}


template <typename T>
const ImplAAFSearchableStack<T>
ImplAAFSearchableStack<T>::operator= (const ImplAAFSearchableStack & src)
{
  if (&src != this)
	{
	  delete[] _rep;
	  _numElems = src._numElems;
	  _numAllocated = src._numElems;
	  _rep = new T[_numAllocated];
	  for (aafUInt32 i = 0; i < _numElems; i++)
		_rep[i] = src._rep[i];
	}
  return *this;
}


template <typename T>
void ImplAAFSearchableStack<T>::push (const T & item)
{
  AAF_STACK_ASSERT (_numElems <= _numAllocated);
  if (_numElems == _numAllocated)
	// need to allocated more room for new item
	{
	  _numAllocated++;
	  T * newRep = new T[_numAllocated];
	  for (aafUInt32 i = 0; i < _numElems; i++)
		newRep[i] = _rep[i];
	  delete[] _rep;
	  _rep = newRep;
	}
  _rep[_numElems++] = item;
}


template <typename T>
T ImplAAFSearchableStack<T>::pop ()
{
  AAF_STACK_ASSERT (0 != size());
  return _rep[--_numElems];
}


template <typename T>
inline aafUInt32 ImplAAFSearchableStack<T>::size () const
{
  return _numElems;
}


template <typename T>
bool ImplAAFSearchableStack<T>::isPresent (const T & item) const
{
  for (aafUInt32 i = 0; i < _numElems; i++)
	{
	  if (_rep[i] == item)
		return true;
	}
  return false;
}



#endif // ! __ImplAAFSearchableStack_h__
