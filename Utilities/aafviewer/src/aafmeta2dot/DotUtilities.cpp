/*
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aaf2dot.
 *
 *  aaf2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  aaf2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with aaf2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <iostream>
#include <assert.h>
using namespace std;

#include <DotUtilities.h>


//-----------------------------------------------------------------------------
string 
ProcessStringForQuoting( string s )
{
   string ret = s;
   int index = 0;
   while ( index < ret.size() )
   {
      // escape newlines
      if ( ret[ index ] == '\n' )
      {
	 ret.erase( index, 1 );
	 ret.insert( index, "\\n" );
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
   else if ( attribute.size() > maxLength )
   {
      retAttribute.resize( maxLength - 1 );
      retAttribute.resize( maxLength, '~' );
   }

   // spread value over lines.
   int newLineIndex = maxWidth;
   while ( newLineIndex < retAttribute.size() )
   {
      retAttribute.insert( newLineIndex, "\\n" );
      newLineIndex += maxWidth + 2;
   }

   return retAttribute;
}


