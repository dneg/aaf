#ifndef __DOTELEMENT_H__
#define __DOTELEMENT_H__

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


#include <string>
#include <fstream>
#include <map>

#include <InstanceMapperProfile.h>

class DotFactory;


typedef std::map< std::string, std::string > DotElementAttributeMap;

// base class for all elements of the DOT graph
// it holds the attributes for the DOT element
class DotElement
{
  public:
   DotElement( std::string name, std::string uid ) : _name( name ), _uid( uid ) {};

   // write element to DOT file
   virtual void Write( std::ofstream &dotFile, InstanceMapperProfile &profile ) = 0;

   // write the element attributes
   void WriteElementAttributes( std::ofstream &dotFile, InstanceMapperProfile &profile );
   void WriteGlobalElementAttributes( std::ofstream &dotFile, InstanceMapperProfile &profile );

   int GetNumElementAttributes() { return _attributes.size(); }

   void SetElementAttribute( std::string name, std::string value );
   std::string GetElementAttribute( std::string name );

   std::string GetName() { return _name; }
   std::string GetUID() { return _uid; }

   // remove references to elements that are not included
   virtual void RemoveReferenceToElement( DotElement *element ) = 0;

   // resolve references to nodes
   virtual void ResolveReferences( DotFactory *dotFactory ) = 0;

  protected:
   std::string _name;
   std::string _uid;

   DotElementAttributeMap _attributes;
};


#endif	//__DOTELEMENT_H__

