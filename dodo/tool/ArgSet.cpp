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

#ifndef _ArgSet_h_
#include "ArgSet.h"
#endif

#if !defined(macintosh)
#ifndef _bld_cfg_h_
#include "bld_cfg.h"
#endif

#include <assert.h>
#endif


#if AAF_BUILD_CONFIG_EXPLICIT_TEMPLATES
template class Vector<ArgDef>;
#endif

void ArgSet::AppendArg (const ArgDef & src)
{
  _args.Append (src);
}


int ArgSet::GetNumArgs () const
{
  return _args.size();
}


ArgDef ArgSet::GetArg (int index) const
{
  assert (index >= 0);
  assert (index < GetNumArgs());

  return _args[index];
}


ArgSet::eInitArgsStat ArgSet::InitArgs
(TextStream & text,
 int numExpected)
{
  // we'll re-init even if we've been init'd already
  _args.Clear ();

  if (! numExpected)
	{
	  // If no args expected, don't try to consume "()"
	  return kInitNoError;
	}

  if (! text.Expect ("("))
    return kInitNoStartParen;

  while (numExpected--)
    {
      ArgDef thisArg;
      ArgDef::eInitStat stat;

	  if (! text.GetLength())
		return kInitArgsTooFew;

	  //
	  // why did this line cause Purify to
	  // think there was a memory leak?
	  //
      // if (text.Expect(")"))

	  TextStream closeParen (")");
	  if (text.Expect(closeParen))
		{
		  return kInitArgsTooFew;
		}

      stat = thisArg.Init (text);
      switch (stat)
		{
		case ArgDef::kInitBadEscape:
		  return kInitNoEndParen;

		case ArgDef::kInitNotTerminated:
		  return kInitNoEndParen;

		default:
		  // unknown error
		  assert (0);
		  break;

		case ArgDef::kInitNoError:
		  _args.Append (thisArg);
		  break;
		}
    }

  if (! text.Expect(")"))
	{
	  return kInitArgsTooMany;
	}
  
  return kInitNoError;
}


const char * ArgSet::InitErrString (eInitArgsStat stat)
{
  switch (stat)
    {
    default:
      assert (0);

    case kInitNoError :		return "No error";
    case kInitArgsTooFew :	return "Too few arguments";
    case kInitArgsTooMany :	return "Too many arguments";
    case kInitNoStartParen :	return "No opening parenthesis";
    case kInitNoEndParen :	return "No closing parenthisis";
    }
}
