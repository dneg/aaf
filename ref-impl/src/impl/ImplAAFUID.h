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

#ifndef __ImplAAFUID_h__
#define __ImplAAFUID_h__


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#include <string.h>

//
// Intended to be a simple class wrapping an aafUID_t, allowing
// equality tests (by overloaded comparison operator) and assignment
// (by builtin assignment operator).
//
class ImplAAFUID
{
public:
  ImplAAFUID () {memset (&_rep, 0, sizeof (_rep));}

  ImplAAFUID (const aafUID_t & auid)
  { memcpy (&_rep, &auid, sizeof (_rep)); }

  inline friend int operator==
  (const ImplAAFUID & lhs, const ImplAAFUID & rhs);

private:
  aafUID_t _rep;
};


//
// Implementation
//
inline int operator==
(
 const ImplAAFUID & lhs,
 const ImplAAFUID & rhs)
{
  return ! memcmp (&lhs._rep, &rhs._rep, sizeof (lhs._rep));
}

#endif // ! __ImplAAFUID_h__
