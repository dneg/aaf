/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef _MacroSet_h_
#include "MacroSet.h"
#endif

#ifndef _SourceInfo_h_
#include "SourceInfo.h"
#endif

#if !defined(macintosh)
#ifndef _bld_cfg_h_
#include "bld_cfg.h"
#endif

#include <assert.h>
#endif

#include <stdlib.h>
#include <string.h>


#if AAF_BUILD_CONFIG_EXPLICIT_TEMPLATES
template class Vector<MacroDef>;
#endif

MacroSet::MacroSet ()
{
  _nameInitials[0] = '\0';
}

void MacroSet::Append
(const MacroDef & src)
{
  char initial = src.GetInitial();
  char * tmp;
  _macs.Append (src);
  for (tmp = _nameInitials;
       *tmp;
       tmp++)
	{
	  if (*tmp == initial)
		return;
	}
  // Made it here without finding our initial.  Add it.
  assert ((tmp - _nameInitials) < sizeof (_nameInitials));
  *tmp++ = initial;
  *tmp = '\0';
}


int MacroSet::GetNumMacros () const
{
  return _macs.size();
}


MacroDef MacroSet::GetMacro (int index) const
{
  assert (index >= 0);
  assert (index < GetNumMacros());

  return _macs[index];
}


bool MacroSet::SearchMacroName
(TextStream & input,
 MacroDef & foundMacro) const
{
  const char * pInitial = input.GetCString();
  const char * tmp;
  for (tmp = _nameInitials;
	   *tmp;
	   tmp++)
	{
	  if (*tmp == *pInitial)
		break;
	}
  if (*tmp != *pInitial)
	{
	  // Did't find it in our cached initials.  Bail out.
	  return false;
	}

  for (int i = 0; i < GetNumMacros(); i++)
    {
      if (GetMacro(i).IsMacro(input))
		{
		  foundMacro = GetMacro(i);
		  return true;
		}
    }
  return false;
}


void MacroSet::import
(TextStream & input,
 bool ignoreText)
{
  char filename[100];
  unsigned int i = 0;

#if defined(macintosh)
  filename[0] = ':';
#endif

  for (i = 1; i < sizeof (filename); )
	{
	  char c;
	  bool stat;
	  stat = input.Consume (c);
	  if (! stat)
		{
		  err_exit ("Expecting end of line after filename.", input);
		}
	  assert (c);
	  if ('\n' == c)
		{
		  filename[i] = '\0';
		  break;
		}
#if defined(macintosh)
  	  else if ('\/' == c)
  	    {
  	      if (2 == i && '.' == filename[1])
  	      {
  	        i = 0; // reset and skip over the first "./"
  	        continue;
  	      }
          // On the mac we need to begin with a partial path.
          c = ':';
        }
#endif
	  filename[i] = c;
	  i++;
	}
  if (sizeof (filename) == i)
	{
	  err_exit ("Filename too long.", input);
	}
  
  FILE * importFile;
  importFile = fopen (filename, "r");
  if (! importFile)
	{
	  err_exit1 ("Could not open filename %s for reading.",
			  filename,
			  input);
	}

  TextStream importText;
  importText.Append (importFile, SourceInfo (filename, 0));
  fclose (importFile);
  AddMacros (importText, ignoreText);
}


TextStream MacroSet::readName
(TextStream & input)
{
  TextStream name;

  while (1)
	{
	  char c;
	  bool stat;

	  stat = input.Consume (c);
	  if (!stat)
		{
		  if (! name.GetLength())
			{
			  // haven't gotten any macro name info yet
			  err_exit ("Expecting macro name.", input);
			}
		  else
			{
			  err_exit ("Expecting arg count after macro name.", input);
			}
		}
	  assert (c);
	  if (! name.GetLength())
		{
		  if ((' ' == c) || ('\t' == c))
			{
			  // swallow extra whitespace
			  continue;
			}
		  if ('\n' == c)
			{
			  err_exit ("Expecting macro name.", input);
			}
		}
	  // if we're here, we've gotten at least part of the name.
	  if ('\n' == c)
		{
		  err_exit ("Expecting arg count after macro name.", input);
		}
	  if ((' ' == c) || ('\t' == c))
		{
		  assert (name.GetLength());
		  return name;
		}
	  name.Append (c);
	}
}


int MacroSet::readCount
(TextStream & input)
{
  int count = -1;

  while (1)
	{
	  char c;
	  bool stat;

	  stat = input.Consume (c);
	  if (!stat)
		{
		  if (count < 0)
			{
			  // haven't gotten any count info yet
			  err_exit ("Expecting argument count.", input);
			}
		  else
			{
			  return count;
			}
		}
	  assert (c);
	  if (count < 0)
		{
		  if ((' ' == c) || ('\t' == c))
			{
			  // swallow extra whitespace
			  continue;
			}
		  if ('\n' == c)
			{
			  err_exit ("Expecting argument count.", input);
			}
		}
	  // if we're here, we've gotten at least part of the count.
	  if ('\n' == c)
		{
		  return count;
		}
	  if ((' ' == c) || ('\t' == c))
		{
		  if (count < 0)
			{
			  // if we haven't started getting count yet, swallow
			  // leading whitespace.
			  continue;
			}
		  else
			{
			  err_exit ("Expecting end-of-line after argument count.", input);
			}
		}
	  if ((c < '0') || (c > '9'))
		{
		  err_exit ("Illegal decimal digit in count.", input);
		}
	  if (count < 0)
		{
		  // haven't started getting count yet.
		  count = 0;
		}
	  count = (count * 10) + (c - '0');
	}
}


TextStream MacroSet::readDefinition
(TextStream & input)
{
  TextStream def;
  SourceInfo si;

  while (1)
	{
	  char c;
	  bool stat;
	  bool startofline = true;

	  if (startofline && input.Expect ("#endm\n"))
		{
		  // Got the end token.
		  return def;
		}

	  if (startofline)
		{
		  si = input.GetCurSourceInfo();
		  si = SourceInfo (si.GetFileName(), si.GetLineNumber()+1);
		}

	  startofline = false;

	  stat = input.Consume (c);
	  if (!stat)
		{
		  if (! def.GetLength())
			{
			  // haven't gotten any macro definition info yet
			  err_exit ("Expecting macro definition.", input);
			}
		  else
			{
			  err_exit ("Expecting #endm statement after macro"
					 "definition.",
					 input); 
			}
		}
	  assert (c);
	  bool dont_append = false;
	  if ('\\' == c)
		{
		  // next character is escaped.
		  stat = input.Consume (c);
		  if (!stat || (('\n' != c) && ('#' != c) && ('\\' != c)))
			{
			  err_exit ("Escape char must be followed by newline, #,"
						" or \\ character.", input); 
			}
		  assert (c);
		  if ('\n' == c)
			{
			  dont_append = true;
			}
		}
	  if ('\n' == c)
		{
		  startofline = true;
		  def.SetCurSourceInfo (si);
		}
	  if (! dont_append)
		{
		  def.Append (c);
		}
	}
}


void MacroSet::readMacro
(TextStream & input)
{
  TextStream name;
  int argCount;
  TextStream definition;

  name = readName (input);
  argCount = readCount (input);
  definition = readDefinition (input);

  MacroDef mac;
  mac.Init (name, argCount);

  MacroDef::eAppendStat stat;
  stat = mac.AppendMacroExpansion (definition);
  switch (stat)
	{
	case MacroDef::kAppendNoError:
	  Append (mac);
	  return;

	case MacroDef::kAppendArgRange:
	  err_exit ("Macro argument out of range.", input);
	  return; // above doesn't exit, but compiler doesn't know that

	case MacroDef::kAppendBadArg:
	  err_exit ("Bad macro argument", input);
	  return; // above doesn't exit, but compiler doesn't know that

	default:
	  assert (0);
	}
}


void MacroSet::discardLine
(TextStream & input)
{
  while (1)
	{
	  char c;
	  bool stat;
	  stat = input.Consume (c);
	  {
		// bail out if end of line, or end of text.
		if (! stat)
		  return;
		if ('\n' == c)
		  return;
	  }
	}
}


void MacroSet::AddMacros
(TextStream & input,
 bool ignoreText)
{
  while (input.GetLength())
	{
	  if (input.Expect ("#import "))
		{
		  import (input, ignoreText);
		}

	  else if (input.Expect ("#startm "))
		{
		  readMacro (input);
		}

	  else if (input.Expect ("#c") || input.Snoop ("\n") || ignoreText)
		{
		  // if this is a comment, or we're ignoring non-macro text, then
		  // discard the rest of this line.
		  discardLine (input);
		}
	  else
		{
		  err_exit ("Improper text outside of macro definition.", input);
		}
	}
}


void MacroSet::err_exit
(const char * msg,
 const TextStream & lineInfo)
{
  const char * fn = lineInfo.GetCurSourceInfo().GetFileName();

  fprintf (stderr,
		   "%s Line %d: %s\n",
		   fn ? fn : "[stdin]",
		   lineInfo.GetCurSourceInfo().GetLineNumber(),
		   msg);
  exit (1);
}


void MacroSet::err_exit1
(const char * msg1,
 const char * msg2,
 const TextStream & lineInfo)
{
  size_t size = strlen(msg1) + strlen(msg2) + 1;
  char * buf = new char[size];
  assert (buf);
  sprintf (buf, msg1, msg2);
  assert (strlen (buf) < size);
  err_exit (buf, lineInfo);
}


bool MacroSet::ApplyMacros
(TextStream & input,
 TextStream & output)
{
  bool retval = false;
  bool atNewline = true;

  while (input.GetLength())
	{
	  SourceInfo si = input.GetCurSourceInfo();
	  MacroDef found;

	  if (SearchMacroName(input, found))
		{
		  ArgSet as;
		  ArgSet::eInitArgsStat iaStat;
		  MacroDef::eInvokeMacroStat imStat;

		  iaStat = as.InitArgs (input, found.GetNumArgs());
		  if (ArgSet::kInitNoError != iaStat)
			{
			  assert ((ArgSet::kInitArgsTooFew == iaStat) ||
					  (ArgSet::kInitArgsTooMany == iaStat) ||
					  (ArgSet::kInitNoStartParen == iaStat) ||
					  (ArgSet::kInitNoEndParen == iaStat));
			  err_exit1 ("Error initializing arguments: %s",
						 ArgSet::InitErrString (iaStat),
						 input);
			}
		  imStat = found.InvokeMacro (as, output, si);
		  assert (MacroDef::kInvokeNoError == imStat);
		  retval = true;
		}
	  else
		{
		  // Ditch dodo comments, import statements and macro definitions; otherwise pass
		  // it on to output.

		  if (atNewline && input.Expect ("#c"))
			{
			  discardLine (input);
			}

		  if (atNewline && input.Expect ("#import "))
			{
			  discardLine (input);
			}

		  else if (atNewline && input.Expect ("#startm "))
			{
			  char c;
			  bool stat;

			  // discard until we get a \n#endm
			  while (! input.Expect ("\n#endm"))
				{
				  stat = input.Consume (c);
				  if (! stat)
					{
					  err_exit ("File ended inside macro.", input);
					}
				  assert (c);
				  // discard character
				}
			  // and get everything after the #endm, including newline.
			  discardLine (input);
			}
		  else
			{
			  char c;
			  bool stat;

			  stat = input.Consume (c);
			  assert (stat);
			  output.Append (c);
			  if ('\n' != c)
				{
				  output.SetCurSourceInfo (si);
				  atNewline = false;
				}
			  else
				{
				  atNewline = true;
				}
			}
		}
	}
  return retval;
}


void MacroSet::dump (FILE* fp) const
{
  int i;
  int numMacs = _macs.size();
  assert (fp);
  fprintf (fp, "MacroSet: number of macros = %d\n", numMacs);
  for (i = 0; i < numMacs; i++)
	{
	  _macs[i].dump(fp);
	}
}
