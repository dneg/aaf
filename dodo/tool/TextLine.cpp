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

#ifndef _TextLine_h_
#include "TextLine.h"
#endif

#include <assert.h>
#include <string.h>


TextLine::TextLine ()
: _sourceInfo (0, 0),
  _pStorage (0),
  _numAllocated (0),
  _pData (0)
{}


TextLine::~TextLine ()
{
  delete[] _pStorage;
}


void TextLine::allocate
(const char * text)
{
  assert (text);

  _numAllocated = strlen (text) + 1;
  _pStorage = new char [_numAllocated];
  assert (_pStorage);

  strcpy (_pStorage, text);
  _pData = _pStorage;
}


TextLine::TextLine (const TextLine & src)
: _sourceInfo (src._sourceInfo)
{
  allocate (src._pData);
}


TextLine TextLine::operator= (const TextLine & src)
{
  if (&src != this)
	{
	  delete[] _pStorage;
	  allocate (src._pData);
	  _sourceInfo = src._sourceInfo;
	}
  return *this;
}


int TextLine::Init
(const char * text)
{
  _sourceInfo = SourceInfo (0, 0);

  delete[] _pStorage;
  _pStorage = 0;
  _pData = 0;
  _numAllocated = 0;
  if (text)
	{
	  _numAllocated = strlen (text) + 1;
	  _pStorage = new char[_numAllocated];
	  assert (_pStorage);
	  _pData = _pStorage;
	  strcpy (_pData, text);
	  for (char* tmp = _pData;
		   *tmp;
		   tmp++)
		{
		  if ('\n' == *tmp)
			{
			  // we've found a newline
			  if ((tmp == _pData) ||
				  ('\\' != *(tmp-1)))
				{
				  // Either the newline was the first char, or the
				  // char before it is _not_ an escape char.
				  // Therefore it's a valid newline, and we should
				  // terminate ourselves after it.
				  *(tmp+1) = '\0';
				}
			}
		}
	}
  return (strlen (_pData));
}


int TextLine::GetLength () const
{
  if (_pData)
	{
	  return strlen (_pData);
	}
  else
	{
	  return 0;
	}
}


bool TextLine::Append
(char c)
{
  // check to see if last character is an un-escaped newline
  if (GetLength())
	{
	  assert (_pData);
	  // not empty.  See if last char is newline
	  if ('\n' == _pData[GetLength()-1])
		{
		  if (1 == GetLength())
			{
			  // it's the only character, so it's not escaped
			  return false;
			}
		  // this string is longer than 1, so penultimate char may be
		  // an escape char ('\')
		  assert (GetLength() >= 2);
		  if ('\\' != _pData[GetLength()-2])
			{
			  // it's not escaped, so it's a valid newline.
			  return false;
			}
		}
	}
  else
	{
	  char buf[2];
	  buf[0] = c;
	  buf[1] = '\0';
	  Init (buf);
	  return true;
	}

  assert (_pData);
  assert (_pStorage);
  assert (_pData >= _pStorage);
  if ((_pData + strlen (_pData) + 1) >= (_pStorage + _numAllocated))
	{
	  char * newBuf = new char[2*strlen(_pData) + 1 + 1];
	  assert (newBuf);
	  strcpy (newBuf, _pData);
	  delete[] _pStorage;
	  _pStorage = newBuf;
	  _pData = _pStorage;
	}
  char newChar[2];
  newChar[0] = c;
  newChar[1] = '\0';
  strcat (_pData, newChar);
  return true;
}


bool TextLine::Consume
(char & c)
{
  if (!GetLength())
	{
	  return false;
	}

  assert (_pData);
  assert (*_pData);
  c = *_pData;
  _pData++;
  return true;
}


SourceInfo TextLine::GetSourceInfo () const
{
  return _sourceInfo;
}


void TextLine::SetSourceInfo
(const SourceInfo & si)
{
  _sourceInfo = si;
}
