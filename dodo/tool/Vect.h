#ifndef _Vect_h_
#define _Vect_h_

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

//
// Simple vector class.  operator[] will grow allocated array if index
// goes beyond current bounds.
//
template <class T>
struct Vector
{
  Vector ();
  //
  // Ctor creates an empty one

  ~Vector ();
  //
  // Dtor

  Vector (const Vector & src);
  Vector operator= (const Vector & src);
  //
  // copy, op=

  int size () const;
  //
  // Returns the number of elements

  const T & operator[] (int index )const;
  T & operator[] (int index );
  //
  // Array index.  Will assert() if attempt is made to index beyond
  // end.

  void Append (const T & elem);
  //
  // Appends elem to end of this vector.

  void Clear ();
  //
  // Removes contents of this vector.

  T Take ();
  //
  // Removes the first element from this vector and returns it.

private:
  T *_rep;
  int _numAllocd;
};


//
// implementation
//

#include <assert.h>


template <class T>
Vector<T>::Vector()
: _rep (0),
  _numAllocd (0)
{}


template <class T>
Vector<T>::~Vector()
{
  delete[] _rep;
}


template <class T>
Vector<T>::Vector (const Vector & src)
: _numAllocd (src._numAllocd)
{
  _rep = new T[_numAllocd];
  assert (_rep);
  for (int i = 0; i < size(); i++)
	{
	  _rep[i] = src._rep[i];
	}
}


template <class T>
Vector<T> Vector<T>::operator= (const Vector & src)
{
  if (&src != this)
	{
	  delete[] _rep;
	  _numAllocd = src._numAllocd;
	  _rep = new T[_numAllocd];
	  assert (_rep);
	  for (int i = 0; i < size(); i++)
		{
		  _rep[i] = src._rep[i];
		}
	}
  return *this;
}


template <class T>
int Vector<T>::size () const
{
  return (_numAllocd);
}


template <class T>
const T & Vector<T>::operator[] (int index) const
{
  assert (index >= 0);
  assert (index < _numAllocd);
  return _rep[index];
}


template <class T>
T & Vector<T>::operator[] (int index)
{
  assert (index >= 0);
  assert (index < _numAllocd);
  return _rep[index];
}


template <class T>
void Vector<T>::Append (const T & elem)
{
  int oldsize = _numAllocd;
  _numAllocd++;
  T * newRep = new T[_numAllocd];
  assert (newRep);
  for (int i = 0; i < oldsize; i++)
	{
	  newRep[i] = _rep[i];
	}
  delete[] _rep;
  _rep = newRep;

  _rep[size()-1] = elem;
}


template <class T>
void Vector<T>::Clear ()
{
  _numAllocd = 0;
  delete[] _rep;
  _rep = 0;
}


template <class T>
T Vector<T>::Take ()
{
  _numAllocd--;
  T retval = _rep[0];
  T * newRep = new T[_numAllocd];
  assert (newRep);
  for (int i = 0; i < _numAllocd; i++)
	{
	  newRep[i] = _rep[i+1];
	}
  delete[] _rep;
  _rep = newRep;
  return retval;
}


#endif // ! _Vect_h_
