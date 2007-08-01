#ifndef __DOTRECORDNODE_H__
#define __DOTRECORDNODE_H__

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


#include <DotElement.h>
#include <DotThingWithID.h>

#include <vector>
#include <string>

class DotFactory;


//-----------------------------------------------------------------------------
// holds the attributes (the AAF object properties) for the record
class DotRecordNodeAttribute
{
  public:
   DotRecordNodeAttribute( std::string name, std::string value )
      : _name( name ), _value( value ) {};
   void Write( std::ofstream &dotFile, InstanceMapperProfile &profile );

   std::string GetName() { return _name; }
   std::string GetValue() { return _value; }

   // calculates and returns the number of characters for the widest attribute
   int GetWidth( InstanceMapperProfile &profile);

  private:
   std::string _name;
   std::string _value;

   std::string GetAttributeString();

};

typedef std::vector< DotRecordNodeAttribute > DotRecordNodeAttributeVector;


//-----------------------------------------------------------------------------
// holds a record of attributes - equivalent to a AAF object
class DotRecordNode : public DotElement, public DotThingWithID
{
  public:
   DotRecordNode( std::string name, std::string uid ); 
   virtual ~DotRecordNode() {};

   virtual void Write( std::ofstream &dotFile, InstanceMapperProfile &profile );

   virtual void RemoveReferenceToElement( DotElement *element ) {};

   virtual void ResolveReferences( DotFactory *dotFactory ) {};

   void AddAttribute( DotRecordNodeAttribute attribute );

  private:
   DotRecordNodeAttributeVector _attributes;
};

typedef std::vector< DotRecordNode* > DotRecordNodeVector;


extern const char* nullMobID;
extern const char* nullUID;


#endif	//__DOTRECORDNODE_H__

