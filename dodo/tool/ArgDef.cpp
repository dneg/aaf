/******************************************\
 *                                          *
 * Advanced Authoring Format                *
 *                                          *
 * Copyright (c) 1998 Avid Technology, Inc. *
 * Copyright (c) 1998 Microsoft Corporation *
 *                                          *
 \******************************************/

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
