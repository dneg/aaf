/*
 * $Id$ $Name$
 *
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aafmeta2dot.
 *
 *  aafmeta2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  aafmeta2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with aafmeta2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include <iostream>
using namespace std;

#include <AAFDefinitionHome.h>
#include <Utilities.h>


//-----------------------------------------------------------------------------
AAFDefinitionHome::~AAFDefinitionHome()
{
  ErasePointerVector( _definitions );
}


//-----------------------------------------------------------------------------
void
AAFDefinitionHome::AddDefinition( AAFMetaDefinition *definition )
{
  _definitions.push_back( definition );
}


//-----------------------------------------------------------------------------
AAFMetaDefinition*
AAFDefinitionHome::FindDefinition( string auid )
{
  AAFMetaDefinition *definition = 0;

  DefinitionVector::iterator iter;
  bool found = false;
  for ( iter=_definitions.begin(); iter!=_definitions.end() && !found; iter++ )
  {
    if ( (*iter)->GetAUID().compare( auid ) == 0 )
    {
      found = true;
      definition = *iter;
    }
  } 

  return definition;
}


//-----------------------------------------------------------------------------
AAFMetaDefinition*
AAFDefinitionHome::FindDefinitionByName( string name )
{
   AAFMetaDefinition *definition = 0;

   DefinitionVector::iterator iter;
   int numFound = 0;
   for ( iter=_definitions.begin(); iter!=_definitions.end(); iter++ )
   {
      if ( (*iter)->GetName().compare( name ) == 0 )
      {
	 numFound++;
	 definition = *iter;
      }
   } 

   return definition;
}


//-----------------------------------------------------------------------------
AAFClassDefinition*
AAFDefinitionHome::FindClassDefinition( string auid )
{
   AAFClassDefinition *definition = 0;

   DefinitionVector::iterator iter;
   bool found = false;
   for ( iter=_definitions.begin(); iter!=_definitions.end() && !found; iter++ )
   {
      AAFClassDefinition *classDef = dynamic_cast< AAFClassDefinition* > (*iter);
      if ( classDef != 0 && classDef->GetAUID().compare( auid ) == 0 )
      {
	 found = true;
	 definition = classDef;
      }
   } 

   return definition;
}


//-----------------------------------------------------------------------------
AAFClassDefinition*
AAFDefinitionHome::FindClassDefinitionByName( string name )
{
   AAFClassDefinition *definition = 0;

   DefinitionVector::iterator iter;
   int numFound = 0;
   for ( iter=_definitions.begin(); iter!=_definitions.end(); iter++ )
   {
      AAFClassDefinition *classDef = dynamic_cast< AAFClassDefinition* > (*iter);
      if ( classDef != 0 && classDef->GetName().compare( name ) == 0 )
      {
	 numFound++;
	 definition = classDef;
      }
   } 

   return definition;
}


//-----------------------------------------------------------------------------
AAFTypeDefinition*
AAFDefinitionHome::FindTypeDefinition( string auid )
{
   AAFTypeDefinition *definition = 0;

   DefinitionVector::iterator iter;
   bool found = false;
   for ( iter=_definitions.begin(); iter!=_definitions.end() && !found; iter++ )
   {
      AAFTypeDefinition *typeDef = dynamic_cast< AAFTypeDefinition* > (*iter);
      if ( typeDef != 0 && typeDef->GetAUID().compare( auid ) == 0 )
      {
	 found = true;
	 definition = typeDef;
      }
   } 

   return definition;
}


//-----------------------------------------------------------------------------
AAFTypeDefinition*
AAFDefinitionHome::FindTypeDefinitionByName( string name )
{
   AAFTypeDefinition *definition = 0;

   DefinitionVector::iterator iter;
   int numFound = 0;
   for ( iter=_definitions.begin(); iter!=_definitions.end(); iter++ )
   {
      AAFTypeDefinition *typeDef = dynamic_cast< AAFTypeDefinition* > (*iter);
      if ( typeDef != 0 && typeDef->GetName().compare( name ) == 0 )
      {
	 numFound++;
	 definition = typeDef;
      }
   } 

   return definition;
}

