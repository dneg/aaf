/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

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
