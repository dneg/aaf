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

#ifndef _ArgDef_h_
#include "ArgDef.h"
#endif

#include <assert.h>
#include <string.h>


ArgDef::eInitStat ArgDef::Init
		 (TextStream & text)
{
  _rep.Clear();

  // ditch leading whitespace
  while (text.Expect (" ") || text.Expect ("\t") || text.Expect ("\n"))
	{}

  while (text.GetLength())
    {
	  if (text.Expect ("\\"))
		{
		  char c;
		  if (! text.Consume (c))
			return kInitBadEscape;
		  _rep.Append (c);
		}
	  else if (text.Expect (","))
		{
		  return kInitNoError;
		}
	  else if (text.Snoop (")"))
		{
		  return kInitNoError;
		}
	  else
		{
		  char c;
		  if (! text.Consume (c))
			return kInitNotTerminated;
		  _rep.Append (c);
		}
    }
  return kInitNotTerminated;
}


TextStream ArgDef::GetText () const
{
  return _rep;
}
