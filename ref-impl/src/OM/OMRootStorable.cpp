/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
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

// @doc OMEXTERNAL
#include "OMRootStorable.h"

OMRootStorable::OMRootStorable(void)
: _clientRoot(0x0002, L"Header"),
  _dictionary(0x0001, L"MetaDictionary")
{
  initialize(0, 0);
}

OMRootStorable::OMRootStorable(OMStorable* clientRoot,
                               OMDictionary* dictionary)
: _clientRoot(0x0002, L"Header"),
  _dictionary(0x0001, L"MetaDictionary")
{
  initialize(clientRoot, dictionary);
}

OMRootStorable::~OMRootStorable(void)
{
}

// {B3B398A5-1C90-11d4-8053-080036210804}
const OMClassId OMRootStorable::_rootClassId = 
{ 0xb3b398a5, 0x1c90, 0x11d4, { 0x80, 0x53, 0x8, 0x0, 0x36, 0x21, 0x8, 0x4 } };

const OMClassId& OMRootStorable::classId(void) const
{
  return _rootClassId;
}

  // @mfunc Save this <c OMRootStorable>.
void OMRootStorable::save(void) const
{
  TRACE("OMRootStorable::save");

  store()->save(classId());
  store()->save(_persistentProperties);
}

  // @mfunc Restore the contents of an <c OMRootStorable>.
void OMRootStorable::restoreContents(void)
{
  TRACE("OMRootStorable::restoreContents");

  store()->restore(_persistentProperties);
}

OMStorable* OMRootStorable::clientRoot(void) const
{
  return _clientRoot;
}

OMDictionary* OMRootStorable::dictionary(void) const
{
  return _dictionary;
}

void OMRootStorable::initialize(OMStorable* clientRoot,
                                OMDictionary* dictionary)
{
  _persistentProperties.put(_clientRoot.address());
  _persistentProperties.put(_dictionary.address());

  _clientRoot = clientRoot;
  _dictionary = dictionary;

  _clientRoot.initialize(OMDictionary::find(0x0002));
  _dictionary.initialize(OMDictionary::find(0x0001));

}
