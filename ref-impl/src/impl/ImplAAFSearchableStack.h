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
