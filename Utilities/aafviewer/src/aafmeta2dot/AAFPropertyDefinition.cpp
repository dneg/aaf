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

#include <AAFPropertyDefinition.h>
#include <AAFDotHome.h>
#include <AAFClassDefinition.h>
#include <Utilities.h>


//-----------------------------------------------------------------------------
AAFPropertyDefinition::AAFPropertyDefinition( string name, string auid, string description, 
					      bool isOptional, AAFTypeDefinition *typeDef,
					      bool isUniqueIdentifier )
  : AAFMetaDefinition( name, auid, description ), _isOptional( isOptional ), _typeDef( typeDef ),
    _isUniqueIdentifier( isUniqueIdentifier )
{
}


//-----------------------------------------------------------------------------
void 
AAFPropertyDefinition::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph, DotRecordNode *node )
{
   aafUInt32 count = 0;
   AAFTypeDefinition *typeDef = _typeDef;
   bool isArrayOrSet = false;

   AAFTypeDefinitionRef *typeRef = 0;
   if ( dynamic_cast< AAFTypeDefinitionFixedArray* > ( _typeDef ) != 0 )
   {
      AAFTypeDefinitionFixedArray *faDef = dynamic_cast< AAFTypeDefinitionFixedArray* > ( _typeDef );
      typeRef = dynamic_cast< AAFTypeDefinitionRef*> ( faDef->GetElementTypeDef() );
      count = faDef->GetCount();
      isArrayOrSet = true;
   }
   else if ( dynamic_cast< AAFTypeDefinitionVariableArray* > ( _typeDef ) != 0 )
   {
      AAFTypeDefinitionVariableArray *vaDef = dynamic_cast< AAFTypeDefinitionVariableArray* > ( _typeDef );
      typeRef = dynamic_cast< AAFTypeDefinitionRef*> ( vaDef->GetElementTypeDef() );
      isArrayOrSet = true;
   }
   else if ( dynamic_cast< AAFTypeDefinitionSet* > ( _typeDef ) != 0 )
   {
      AAFTypeDefinitionSet *sDef = dynamic_cast< AAFTypeDefinitionSet* > ( _typeDef );
      typeRef = dynamic_cast< AAFTypeDefinitionRef*> ( sDef->GetElementTypeDef() );
      isArrayOrSet = true;
   }
   else
   {
      typeRef = dynamic_cast< AAFTypeDefinitionRef*> ( _typeDef );
   }

   if ( typeRef != 0 && !dotHome->GetProfile()->GetRefAsProperty() )
   {
      DotEdge *edge = factory->CreateEdge( _name, factory->CreateEdgeUID() );

      if ( dynamic_cast< AAFTypeDefinitionStrongObjRef* > ( typeRef ) )
      {
	 edge->SetElementAttribute( "color", "black" );
	 edge->SetElementAttribute( "arrowtail", "diamond" );
	 edge->SetElementAttribute( "labelfontcolor", "black" );
	 edge->SetElementAttribute( "weight", dotHome->GetProfile()->GetStrongRefEdgeWeight() );
      }
      else
      {
	 edge->SetElementAttribute( "color", "blue" );
	 edge->SetElementAttribute( "labelfontcolor", "blue" );
	 edge->SetElementAttribute( "constraint", "false" );
      }
      edge->SetElementAttribute( "arrowhead", "open" );
      edge->SetElementAttribute( "arrowsize", "1.5" );

      if ( count == 0 )
      {
	 if ( _isOptional )
	 {
	    if ( isArrayOrSet )
	    {
	       edge->SetElementAttribute( "headlabel", "\"0..n\"" );
	    }
	    else
	    {
	       edge->SetElementAttribute( "headlabel", "\"0..1\"" );
	    }
	 }
	 else
	 {
	    if ( isArrayOrSet )
	    {
	       edge->SetElementAttribute( "headlabel", "\"1..n\"" );
	    }
	    else
	    {
	       edge->SetElementAttribute( "headlabel", "\"1\"" );
	    }
	 }
      }
      else
      {
	 char buffer[ 16 ];
	 if ( _isOptional )
	 {
	    sprintf( buffer, "\"0..%d\"", count );
	    edge->SetElementAttribute( "headlabel", buffer );
	 }
	 else
	 {
	    sprintf( buffer, "\"1..%d\"", count );
	    edge->SetElementAttribute( "headlabel", buffer );
	 }
      }

      DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
      sourceEdgeEnd->SetReference( node );
      edge->SetSource( sourceEdgeEnd );

      DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
      // don't know target yet
      targetEdgeEnd->SetAAFUIDReference( typeRef->GetRefClassDef( dotHome )->GetAUID() );
      edge->SetTarget( targetEdgeEnd );

      graph->AddEdge( edge );
   }
   else
   {
      string name;
      if ( _isOptional )
      {
	 name = "+ ";
      }
      if ( _isUniqueIdentifier )
      {
	 name += "<<ID>> ";
      }
      name += _name;
      DotRecordNodeClassAttribute classAttribute( name, _typeDef->GetName() );
   
      node->AddClassAttribute( classAttribute );
   }
}




