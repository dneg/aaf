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

#include <iostream>
using namespace std;


#include <MapperSettings.h>
#include <Logging.h>


//-----------------------------------------------------------------------------
IntegerMapSetting integerMapSettings[ INTEGER_MAP_SETTINGS_SIZE ] = 
{
   { "CDCIDescriptor",			"FrameIndexByteOrder",		true }
};


//-----------------------------------------------------------------------------
bool DisplayHex( string objectName, string propertyName )
{
   bool displayHex = false;

   bool found = false;
   for ( int i=0; i<INTEGER_MAP_SETTINGS_SIZE && !found; i++ )
   {
      if ( objectName.compare( integerMapSettings[i].objectName ) == 0 &&
	   propertyName.compare( integerMapSettings[i].propertyName ) == 0 )
      {
	 found = true;
	 displayHex = integerMapSettings[i].displayHex;
      }
   }

   return displayHex;
}

