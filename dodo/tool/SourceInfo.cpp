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

#ifndef _SourceInfo_h_
#include "SourceInfo.h"
#endif

#include <assert.h>
#include <string.h>


SourceInfo::SourceInfo
(const char * fileName,
 int lineNumber)
: _lineNumber (lineNumber)
{
  allocate (fileName);
}


SourceInfo::~SourceInfo ()
{
  delete[] _fileName;
}


SourceInfo::SourceInfo
(const SourceInfo & src)
: _lineNumber (src._lineNumber),
  _fileName (0)
{
  allocate (src._fileName);
}


SourceInfo SourceInfo::operator=
(const SourceInfo & src)
{
  if (&src != this)
	{
	  _lineNumber = src._lineNumber;

	  if (_fileName)
		{
		  delete[] _fileName;
		}
	  allocate (src._fileName);
	}
  return *this;
}


void SourceInfo::allocate
(const char * name)
{
  int len;
  if (name)
	{
	  len = strlen (name);
	  _fileName = new char[len+1];
	  assert (_fileName);
	  strcpy (_fileName, name);
	}
  else
	{
	  _fileName = 0;
	}
}


const char * SourceInfo::GetFileName () const
{
  return _fileName;
}


int SourceInfo::GetLineNumber () const
{
  return _lineNumber;
}
