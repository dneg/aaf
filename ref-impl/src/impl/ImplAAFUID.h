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
