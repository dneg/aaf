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
#include <sstream>
#include <assert.h>
using namespace std;

#include <DotThingWithID.h>
#include <DotUtilities.h>
#include <DotLogging.h>


//-----------------------------------------------------------------------------
void
DotThingWithID::AddID( DotThingID id )
{
   if ( GetID( id.GetName() ).size() > 0 )
   {
      RemoveID( id.GetName() );
   }
   _ids.push_back( id );
}


//-----------------------------------------------------------------------------
string
DotThingWithID::GetID( string name )
{
   string ret;

   DotThingIDVector::iterator iter;
   bool found = false;
   for ( iter=_ids.begin(); iter!=_ids.end() && !found; iter++ )
   {
      if ( (*iter).GetName().compare( name ) == 0 )
      {
	 found = true;
	 ret = (*iter).GetValue();
      }
   }

   return ret;
}


//-----------------------------------------------------------------------------
void
DotThingWithID::RemoveID( string name )
{
   DotThingIDVector::iterator iter;
   bool found = false;
   for ( iter=_ids.begin(); iter!=_ids.end() && !found; iter++ )
   {
      if ( (*iter).GetName().compare( name ) == 0 )
      {
	 found = true;
	 _ids.erase( iter );
      }
   }
}

