/*
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aafmeta2dot.
 *
 *  aafmeta2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  aafmeta2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with aafmeta2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <iostream>
using namespace std;

#include <AAFClassDefUIDs.h>
#include <AAFPropertyDefs.h>

#include <AAFClassDefinition.h>
#include <AAFDotHome.h>
#include <Utilities.h>


//-----------------------------------------------------------------------------
AAFClassDefinition::AAFClassDefinition( string name, string auid, string description,
					bool isConcrete, AAFClassDefinition *parent )
  : AAFMetaDefinition( name, auid, description ), _isConcrete( isConcrete ),
    _parent( parent )
{
}


//-----------------------------------------------------------------------------
AAFClassDefinition::~AAFClassDefinition()
{
  ErasePointerVector( _properties );
}


//-----------------------------------------------------------------------------
void 
AAFClassDefinition::InitForBuild()
{
  AAFMetaDefinition::InitForBuild();
}


//-----------------------------------------------------------------------------
void 
AAFClassDefinition::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
  if ( _wasBuilt )
    {
      return;
    }

   DotRecordNode *node = factory->CreateRecordNode( _name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   if ( !_isConcrete )
   {
      node->SetElementAttribute( "fillcolor", "lightyellow2" );
   }
   else
   {
      node->SetElementAttribute( "fillcolor", "lightyellow" );
   }


   if ( !dotHome->GetProfile()->GetTypeDefs() )
   {
      PropertyVector::iterator iter;
      for ( iter=_properties.begin(); iter!=_properties.end(); iter++ )
      {
	 (*iter)->BuildDot( dotHome, factory, graph, node );
      }
   }
   
   if ( _parent != 0 )
   {
      DotEdge *edge = factory->CreateEdge( _name + _parent->GetName(), factory->CreateEdgeUID() );
      edge->SetElementAttribute( "dir", "back" );
      edge->SetElementAttribute( "arrowtail", "empty" );
      edge->SetElementAttribute( "color", "green" );
      edge->SetElementAttribute( "labelfontcolor", "green" );
      edge->SetElementAttribute( "arrowsize", "2.0" );
      edge->SetElementAttribute( "weight", dotHome->GetProfile()->GetGeneralisationEdgeWeight() );

      DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
      targetEdgeEnd->SetReference( node );
      edge->SetTarget( targetEdgeEnd );

      DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
      edge->SetSource( sourceEdgeEnd );
      // don't know parent yet
      sourceEdgeEnd->SetAAFUIDReference( _parent->GetAUID() );

      graph->AddEdge( edge );
   }

   graph->AddRecordNode( node );

  _wasBuilt = true;
}


 
//-----------------------------------------------------------------------------
void
AAFClassDefinition::AddProperty( AAFPropertyDefinition *property )
{
  _properties.push_back( property );
}




//-----------------------------------------------------------------------------
AAFPropertyDefinition*
AAFClassDefinition::FindPropertyDefinition( string auid )
{
  AAFPropertyDefinition *definition = 0;

  PropertyVector::iterator iter;
  bool found = false;
  for ( iter=_properties.begin(); iter!=_properties.end() && !found; iter++ )
  {
    if ( (*iter)->GetAUID().compare( auid ) == 0 )
    {
      found = true;
      definition = *iter;
    }
  } 

  return definition;
}

