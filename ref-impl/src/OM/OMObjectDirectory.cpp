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
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

// @doc OMINTERNAL
#include "OMObjectDirectory.h"
#include "OMAssertions.h"
#include "OMPortability.h"

#include <string.h>
#include <iostream.h>

OMObjectDirectory::OMObjectDirectory(int capacity)
: _capacity(capacity), _current(0)
{
  TRACE("OMObjectDirectory::OMObjectDirectory");

  _table = new TableEntry[_capacity];
  ASSERT("Valid heap pointer", _table != 0);
  for (int i = 0; i < _capacity; i++) {
    _table[i]._object = 0;
    _table[i]._name = 0;
  }
}

OMObjectDirectory::~OMObjectDirectory(void)
{
  for (int i = 0; i < _current; i++) {
    delete [] _table[i]._name;
  }
  delete [] _table;
  _table = 0;
}

bool OMObjectDirectory::lookup(const char* name, const OMStorable*& p) const
{
  TRACE("OMObjectDirectory::lookup");
  PRECONDITION("Valid name to look up", validString(name));
  bool result = false;
  
  for (int i = 0; i < _current; i++) {
    int status = strcmp(name, _table[i]._name);
    if (status == 0) {
      result = true;
      p = _table[i]._object;
      break;
    }
  }
  return result;
}

void OMObjectDirectory::insert(const char* name, const OMStorable* p)
{
  TRACE("OMObjectDirectory::insert");

  if (_current < _capacity) {
    char* n = new char[strlen(name) + 1];
    ASSERT("Valid heap pointer", n != 0);
    strcpy(n , name);
    _table[_current]._object = const_cast<OMStorable *>(p);
    _table[_current]._name = n;
    _current++;
  } else {
    ASSERT("Table not full", false);
  }
}

int OMObjectDirectory::count(void) const
{
  return _current;
}

void OMObjectDirectory::dump(void) const
{
  for (int i = 0; i < _current; i++) {
    cout << i << " [" << _table[i]._object << "] \"" << _table[i]._name << "\"" << endl;
  }
}
