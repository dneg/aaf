/*
 * $Id$ $Name$
 *
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aaf2dot.
 *
 *  aaf2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  aaf2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with aaf2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <iostream>
#include <assert.h>
using namespace std;

#include <ctype.h>

#include <DotUtilities.h>


//-----------------------------------------------------------------------------
string 
ProcessRecordString( string s )
{
   string ret = s;
   int index = 0;
   while ( index < (int)ret.size() )
   {
      // escape special characters
      if ( ret[ index ] == '\n' )
      {
	 ret.erase( index, 1 );
	 ret.insert( index, "\\n" );
	 index += 1;
      }
      // escape special characters in quotes
      else if ( ret[ index ] == '"' ||
		ret[ index ] == '\\' ||
		ret[ index ] == '<' ||
		ret[ index ] == '>' )
      {
	 ret.insert( index, "\\" );
	 index += 1;
      }
      // replace special Record Node characters with "?"
      else if ( ret[ index ] == '{' ||
		ret[ index ] == '}' ||
		ret[ index ] == '=' ||
		ret[ index ] == '|' )
      {
	 ret[index] = '?';
      }
      // tab etc. become spaces
      else if (isspace((unsigned char)ret[index]))
      {
	 ret[index] = ' ';
      }
      // replace non-printable or control characters with "?"
      else if (!isprint((unsigned char)ret[index]) || iscntrl((unsigned char)ret[index]))
      {
	 ret[index] = '?';
      }

      index++;
   }

   return ret;
}


//-----------------------------------------------------------------------------
string 
LimitAttributeSize( string attribute, int maxLength, int maxWidth )
{
   string retAttribute = attribute;
   if ( maxLength == 0 )
   {
      return "";
   }
   else if ( (int)attribute.size() > maxLength )
   {
      retAttribute.resize( maxLength - 1 );
      retAttribute.resize( maxLength, '~' );
   }

   // spread value over lines.
   int newLineIndex = maxWidth;
   int index = 0;
   bool escape = false;
   while ( index < (int)retAttribute.size() )
   {
      if (index == newLineIndex)
      {
	 if (escape)
	 {
	    index++;
	    if (index < (int)retAttribute.size())
	    {
	       retAttribute.insert(index,"\\n");
	    }
	 }
	 else
	 {
	    retAttribute.insert(index, "\\n");
	 }
	 newLineIndex = index + maxWidth + 3;
      }
      else 
      {
	 if (retAttribute[index] == '\\')
	 {
	    escape = !escape;
	 }
	 else
	 {
	    escape = false;
	 }
      }

      index++;
   }

   return retAttribute;
}


