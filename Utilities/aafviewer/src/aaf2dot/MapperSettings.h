#ifndef __MAPPERSETTINGS_H__
#define __MAPPERSETTINGS_H__

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

#include <string>

typedef struct
{
      char *objectName;
      char *propertyName;
      bool displayHex;
} IntegerMapSetting;

#define INTEGER_MAP_SETTINGS_SIZE	1

// a table of integer property values that should be shown in hex
extern IntegerMapSetting integerMapSettings[ INTEGER_MAP_SETTINGS_SIZE ];

// returns whether the integer value is shown in hex
bool DisplayHex( std::string objectName, std::string propertyName );


#endif //__MAPPERSETTINGS_H__

