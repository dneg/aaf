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

//
// Main() for AutoDodo - Yet Another Macro Processor.  This one allows
// multi-line macro expansions, and is portable across platforms.
//

#if defined(macintosh)
#define _MAC
#endif


// #define DEBUG 1


#ifndef _TextStream_h_
#include "TextStream.h"
#endif

#ifndef _MacroSet_h_
#include "MacroSet.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if !defined(macintosh)
#include <assert.h>
#else
#include <CursorCtl.h>
#endif

static void printHelp ()
{
  printf ("\n");
  printf ("This is a macro file for use with AutoDodo.  The syntax is as\n");
  printf ("follows:\n");
  printf ("\n");
  printf ("All AutoDodo commands in the macro file start at the beginning of a\n");
  printf ("line, and all start with the '#' character.  The following commands\n");
  printf ("are recognized:\n");
  printf ("\n");
  printf (" #c		- comment\n");
  printf (" #startm	- start of macro definition\n");
  printf (" #endm	- end of macro definition\n");
  printf ("\n");
  printf ("One special case is within macro expansion, places where arguments\n");
  printf ("are to be placed are denoted with the string \"%%n\", where n is a\n");
  printf ("digit starting at 1.\n");
  printf ("\n");
  printf ("Comment lines begin with \"#c\"; everything following up to the end\n");
  printf ("of the line is ignored.\n");
  printf ("\n");
  printf ("A macro definition is given by the following syntax.  Note that you\n");
  printf ("should strip off the comment characters (\"#c \") at the beginning of\n");
  printf ("each line if you intend to use them.\n");
  printf ("\n");
  printf ("#startm <macro_name> <num_args>\n");
  printf ("<macro expansion specification line 1>\n");
  printf ("<macro expansion specification line 2>\n");
  printf ("...\n");
  printf ("<macro expansion specification line n>\n");
  printf ("#endm\n");
  printf ("\n");
  printf ("In this case, whenever the macro_name(<args>) is encountered in the\n");
  printf ("source file, it will be replaced with the multi-line macro expansion\n");
  printf ("in the output file.\n");
  printf ("\n");
  printf ("In any macro definition line that ends with a '\' (backslash)\n");
  printf ("character immediately before the final newline, the newline is escaped\n");
  printf ("so that it will not appear in the output.\n");
  printf ("\n");
  printf ("Rules:\n");
  printf ("\n");
  printf ("- Any arguments specified within a macro expansion specification\n");
  printf ("  using the \"%%n\" syntax must have numbers that are 1 or greater,\n");
  printf ("  and are less than or equal to the num_args value for that macro.\n");
  printf ("\n");
  printf ("- Any macro invocation in the input file must be given precisely\n");
  printf ("  the number of arguments given in the num_args field of the\n");
  printf ("  macro's definition.\n");
  printf ("\n");
}


//
// Prints usage information to cerr.  Argument is a NULL-terminated
// C-style string naming this program.
//
static void usage (const char * command)
{
  assert (command);
  fprintf (stderr, "Usage: %s -f macro_file\n", command);
  fprintf (stderr,
	   "  Uses stdin for input file and stdout for output file.\n");
  fprintf (stderr, "       %s -h\n", command);
  fprintf (stderr,
	   "  Prints help information.\n");
  exit (1);
}


// Post-processing step: ditch escape characters when they're of the
// form "\n\\#"  (newline, escape, pound-sign).
static TextStream UnEscape
(TextStream & input)
{
  TextStream out;

  //
  // Special case: start of file won't have the leading newline.
  //
  if (input.Expect ("\\#"))
	{
	  out.Append ('#');
	}

  while (input.GetLength())
	{
	  if (input.Expect ("\n\\#"))
		{
		  out.Append ('\n');
		  out.Append ('#');
		}
	  else
		{
		  char c;
		  bool stat;
		  stat = input.Consume (c);
		  assert (stat);
		  out.Append (c);
		}
	}
  return out;
}

#if defined(_MAC)
int main(int argc,char *argv[],char */*envp[]*/)
#else
int main (int argc, char ** argv)
#endif
{
  const char * command = argv[0];

  assert (command);

  if (argc >= 2)
	if (0 == strcmp ("-h", argv[1]))
	  {
		printHelp ();
		exit (0);
	  }

  if (argc <= 2)
    {
      usage (command);  // does not return
    }

  if (0 != strcmp ("-f", argv[1]))
    {
      fprintf (stderr,
	       "Unrecognized command line switch: \"%s\"\n", argv[1]);
      usage (command);	// does not return
    }

  const char * macrofilename = argv[2];
  assert (macrofilename);

  FILE * macrofile;
  macrofile = fopen (macrofilename, "r");
  if (! macrofile)
    {
      fprintf (stderr, "Couldn't open macrofile \"%s\"\n",
	       macrofilename);
      usage (command);	// does not return
    }

#if defined(macintosh)
  // Intialize the MPW cursors so that we can release time 
  // to the operating system (pre-MacOS X).
  InitCursorCtl(NULL);
#endif

  TextStream macroText;
  macroText.Append (macrofile,
		    SourceInfo (macrofilename, 1));

  MacroSet macros;

  macros.AddMacros (macroText);

  TextStream input;
  input.Append (stdin, SourceInfo ("", 1));

  TextStream tmpInput = input;
  macros.AddMacros (tmpInput, true);
#if DEBUG
  fprintf (stderr, "Macro dump:\n");
  macros.dump (stderr);
#endif

  TextStream output;
  bool changed = true;
  for (int repeats = 0;
	   repeats < 10 && changed;
	   repeats++)
	{
#if DEBUG
	  fprintf (stderr, "Pass %d...\n\n", repeats+1);
#endif
#if defined(macintosh)
    // Release time to the operating system (pre-MacOS X).
//	SpinCursor(1);
#endif
	  output.Clear();
	  changed = macros.ApplyMacros (input, output);
	  input = output;
#if DEBUG
	  fprintf (stderr, "\n\nOutput of this pass:\n");
	  output.dump (stderr);
	  fprintf (stderr, "\n\n");
#endif
	}

  if (changed)
	{
	  fprintf (stderr, "Greater than %d levels of macro recursion.\n",
			   10);
	  exit (1);
	}

  // Post-processing step: ditch escape characters when they're of the
  // form "\n\\#"  (newline, escape, pound-sign).
  input = output;
  output = UnEscape (input);

  output.dump (stdout);

  return 0;
}
