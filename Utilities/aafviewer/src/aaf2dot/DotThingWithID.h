#ifndef __DOTTHINGWITHID_H__
#define __DOTTHINGWITHID_H__

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
#include <vector>

//-----------------------------------------------------------------------------
// top-level class representing a 'thing' (name/value pair) in DOT
class DotThingID
{
  public:
   DotThingID( std::string name, std::string value ) : _name( name ), _value( value ) {};

   std::string GetName() { return _name; }
   std::string GetValue() { return _value; }

  private:
   std::string _name;
   std::string _value;
};

typedef std::vector< DotThingID > DotThingIDVector;


//-----------------------------------------------------------------------------
// a DOT thing that has a unique ID
class DotThingWithID
{
  public:
   DotThingWithID() {};
   virtual ~DotThingWithID() {};

   void AddID( DotThingID id);
   std::string GetID( std::string name );

  private:
   DotThingIDVector _ids;

   void RemoveID( std::string name );
};


#endif	//__DOTTHINGWITHID_H__

