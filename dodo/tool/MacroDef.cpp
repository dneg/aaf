/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef _MacroDef_h_
#include "MacroDef.h"
#endif

#if !defined(macintosh)
#include <assert.h>
#endif
#include <string.h>



MacroDef::MacroDef ()
  : _numArgs (0)
{}


void MacroDef::Init
(const TextStream & name,
 int numArgs)
{
  // Only allowing arg values between 0 and 9, inclusive.
  assert (numArgs < 100);
  assert (numArgs >= 0);

  assert (name.GetLength());

  _definition.Clear ();
  _numArgs = numArgs;
  _name = name;
}


MacroDef::eAppendStat MacroDef::AppendMacroExpansion
(TextStream & text)
{
  char c;
  bool stat;
  while (text.Consume(c))
    {
	  if ('%' == c)
		{
		  if (text.Expect ("%"))
			{
			  // escape of '%' character: that's OK
			  _definition.Append (c);
			  _definition.Append (c);
			  continue;
			}

		  int argIdx = 0;
		  for (int digitCount = 0; digitCount < 2; digitCount++)
			{
			  stat = text.Consume(c);
			  if (! stat)
				{
				  return kAppendBadArg;
				}
			  if ((c >= '0') && (c <= '9'))
				{
				  // valid arg values
				  if (0 == digitCount)
					{
					  _definition.Append ('%');
					}
				  _definition.Append (c);
				  argIdx = argIdx*10 + (c - '0');
				  if (argIdx > _numArgs)
					{
					  return kAppendArgRange;
					}
				  continue;
				}
			  else
				{
				  return kAppendBadArg;
				}
			}
		}

	  else
		{
		  _definition.Append (c);
		}
    }
  return kAppendNoError;
}


bool MacroDef::IsMacro
(TextStream & testName) const
{
  return testName.Expect (_name);
}


MacroDef::eInvokeMacroStat MacroDef::InvokeMacro
(const ArgSet & args,
 TextStream & outText,
 const SourceInfo &si) const
{
  if (! _name.GetLength())
    {
      return kInvokeNotInited;
    }

  if (args.GetNumArgs() != _numArgs)
    {
      return kInvokeWrongArgCount;
    }

  TextStream tmp = _definition;

  while (tmp.GetLength())
	{
	  bool stat;
	  char c;

	  stat = tmp.Consume (c);
	  assert (stat);

	  if ('%' != c)
		{
		  outText.Append (c);
		}
	  else
		{
		  // Looks like a macro argument invocation, maybe!
		  stat = tmp.Consume (c);
		  assert (stat);	// should have already checked for this in
							// AppendMacroExpansion()

		  if ('%' == c)
			{
			  // Not a macro, just an escaped '%'
			  outText.Append (c);
			}
		  else
			{
			  int idx = c - '0';
			  TextStream txt;

			  stat = tmp.Consume (c);
			  assert (stat);	// should have already checked for
								// this in AppendMacroExpansion()
			  assert (c >= '0');
			  assert (c <= '9');
			  idx = idx * 10 + (c - '0');

			  // AppendMacroExpansion should have already checked for
			  // proper range
			  assert (idx > 0);
			  assert (idx < 100);
			  txt = args.GetArg(idx-1).GetText();	// index is 0-based
			  outText.SetCurSourceInfo (si);
			  outText.Append (txt);
			}
		}		  
    }
  return kInvokeNoError;
}


const char * MacroDef::InvErrString (eInvokeMacroStat stat)
{
  switch (stat)
    {
    default:
      assert (0);

    case kInvokeNoError :		return "No error";
    case kInvokeWrongArgCount :	return "Wrong argument count";
    case kInvokeNotInited :		return "MacroDef not yet initialized";
    }
}


int MacroDef::GetNumArgs () const
{
  return _numArgs;
}


char MacroDef::GetInitial () const
{
  const char * rep = _name.GetCString();
  if (! rep)
	{
	  return '\0';
	}
  else
	{
	  return *rep;
	}
}


void MacroDef::dump (FILE* fp) const
{
  assert (fp);
  fprintf (fp, "MacroDef - name: \"");
  _name.dump (fp);
  fprintf (fp, "\"\n");
  fprintf (fp, "MacroDef - numArgs: %d\n", _numArgs);
  fprintf (fp, "MacroDef - definition: \"");
  _definition.dump (fp);
  fprintf (fp, "\"\n");
  fprintf (fp, "MacroDef - done with: \"");
  _name.dump (fp);
  fprintf (fp, "\"\n");
}
