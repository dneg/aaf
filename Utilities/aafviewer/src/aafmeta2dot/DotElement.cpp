
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

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include <iostream>
#include <sstream>
#include <assert.h>
#include <fstream>
using namespace std;

#include <DotElement.h>
#include <DotUtilities.h>
#include <DotLogging.h>



//-----------------------------------------------------------------------------
void
DotElement::WriteElementAttributes( ofstream &dotFile, DotProfile &profile )
{
   DotElementAttributeMap::iterator iter;
   for ( iter=_attributes.begin(); iter!=_attributes.end(); iter++ )
   {
      if ( iter != _attributes.begin() )
      {
	 dotFile << ", ";
      }
      dotFile << (*iter).first << " = " << (*iter).second;
   }
}


//-----------------------------------------------------------------------------
void 
DotElement::WriteGlobalElementAttributes( ofstream &dotFile, DotProfile &profile )
{
   DotElementAttributeMap::iterator iter;
   for ( iter=_attributes.begin(); iter!=_attributes.end(); iter++ )
   {
      dotFile << (*iter).first << " = " << (*iter).second << ";" << endl;
   }
}


//-----------------------------------------------------------------------------
void 
DotElement::SetElementAttribute( string name, string value ) 
{ 
   DotElementAttributeMap::iterator iter;
   iter = _attributes.find( name );
   if ( iter != _attributes.end() )
   {
      _attributes.erase( iter );
   }
   _attributes.insert( DotElementAttributeMap::value_type( name, value ) );
}


//-----------------------------------------------------------------------------
string 
DotElement::GetElementAttribute( string name )
{
   DotElementAttributeMap::iterator iter;
   iter = _attributes.find( name );
   if ( iter == _attributes.end() )
   {
      return "";
   }
   return (*iter).second;
}


