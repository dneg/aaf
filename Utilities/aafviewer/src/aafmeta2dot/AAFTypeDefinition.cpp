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

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <iostream>
#include <sstream>
using namespace std;

#include <AAFTypeDefUIDs.h>

#include <AAFTypeDefinition.h>
#include <AAFClassDefinition.h>
#include <AAFDotHome.h>
#include <Utilities.h>


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinition::AAFTypeDefinition( string name, string auid, string description )
   : AAFMetaDefinition( name, auid, description )
{
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionUnknown::AAFTypeDefinitionUnknown( string name, string auid, string description )
   : AAFTypeDefinition( name, auid, description )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionUnknown::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionUnknown\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   graph->AddRecordNode( node );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionInt::AAFTypeDefinitionInt( string name, string auid, string description, 
					    aafUInt8 size, bool isSigned )
   : AAFTypeDefinition( name, auid, description ), _size( size ), _isSigned( isSigned )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionInt::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionInt\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   char buffer[ 4 ];
   sprintf( buffer, "%d", _size );
   DotRecordNodeAttribute sizeAttribute( "Size", buffer );
   node->AddAttribute( sizeAttribute );

   if ( _isSigned )
   {
      DotRecordNodeAttribute isSignedAttr( "IsSigned", "true" );
      node->AddAttribute( isSignedAttr );
   }
   else
   {
      DotRecordNodeAttribute isSignedAttr( "IsSigned", "false" );
      node->AddAttribute( isSignedAttr );
   }


   graph->AddRecordNode( node );
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionCharacter::AAFTypeDefinitionCharacter( string name, string auid, string description )
  : AAFTypeDefinition( name, auid, description )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionCharacter::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionCharacter\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   graph->AddRecordNode( node );
}



 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionRename::AAFTypeDefinitionRename( string name, string auid, string description, 
						  AAFTypeDefinition *baseDef )
   : AAFTypeDefinition( name, auid, description ), _baseDef( baseDef )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionRename::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionRename\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   DotEdge *edge = factory->CreateEdge( _name, factory->CreateEdgeUID() );

   edge->SetElementAttribute( "color", "blue" );
   edge->SetElementAttribute( "labelfontcolor", "blue" );
   edge->SetElementAttribute( "arrowhead", "open" );
   edge->SetElementAttribute( "arrowsize", "1.5" );

   DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
   sourceEdgeEnd->SetReference( node );
   edge->SetSource( sourceEdgeEnd );

   DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
   // don't know target yet
   targetEdgeEnd->SetAAFUIDReference( _baseDef->GetAUID() );
   edge->SetTarget( targetEdgeEnd );

   graph->AddEdge( edge );


   graph->AddRecordNode( node );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionEnum::AAFTypeDefinitionEnum( string name, string auid, string description,
					      AAFTypeDefinition *elementTypeDef )
   : AAFTypeDefinition( name, auid, description ), _elementTypeDef( elementTypeDef )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionEnum::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionEnum\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   map< string, aafUInt64 >::iterator iter;
   for ( iter=_elements.begin(); iter!=_elements.end(); iter++ )
   {
      char buffer[21];
      sprintf( buffer, "%d", (unsigned long int)(*iter).second );
      DotRecordNodeAttribute elemAttribute( (*iter).first, buffer );
      node->AddAttribute( elemAttribute );
   }

   DotEdge *edge = factory->CreateEdge( _name, factory->CreateEdgeUID() );

   edge->SetElementAttribute( "color", "blue" );
   edge->SetElementAttribute( "labelfontcolor", "blue" );
   edge->SetElementAttribute( "arrowhead", "open" );
   edge->SetElementAttribute( "arrowsize", "1.5" );

   DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
   sourceEdgeEnd->SetReference( node );
   edge->SetSource( sourceEdgeEnd );

   DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
   // don't know target yet
   targetEdgeEnd->SetAAFUIDReference( _elementTypeDef->GetAUID() );
   edge->SetTarget( targetEdgeEnd );

   graph->AddEdge( edge );


   graph->AddRecordNode( node );
}



//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionEnum::AddElement( string name, aafUInt64 value )
{
   _elements.insert( map< string, aafUInt64 >::value_type( name, value ) );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionExtEnum::AAFTypeDefinitionExtEnum( string name, string auid, string description )
   : AAFTypeDefinition( name, auid, description )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionExtEnum::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionExtEnum\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   map< string, string >::iterator iter;
   for ( iter=_elements.begin(); iter!=_elements.end(); iter++ )
   {
      DotRecordNodeAttribute elemAttribute( (*iter).first, (*iter).second );
      node->AddAttribute( elemAttribute );
   }


   graph->AddRecordNode( node );
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionExtEnum::AddElement( string name, string value )
{
   _elements.insert( map< string, string >::value_type( name, value ) );
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionFixedArray::AAFTypeDefinitionFixedArray( string name, string auid, string description,
							  AAFTypeDefinition *elementTypeDef, aafUInt32 count )
   : AAFTypeDefinition( name, auid, description ), _elementTypeDef( elementTypeDef ), _count( count )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionFixedArray::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionFixedArray\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   char buffer[ 11 ];
   sprintf( buffer, "%d", _count );
   DotRecordNodeAttribute sizeAttribute( "Count", buffer );
   node->AddAttribute( sizeAttribute );

   DotEdge *edge = factory->CreateEdge( _name, factory->CreateEdgeUID() );

   edge->SetElementAttribute( "color", "blue" );
   edge->SetElementAttribute( "labelfontcolor", "blue" );
   edge->SetElementAttribute( "arrowhead", "open" );
   edge->SetElementAttribute( "arrowsize", "1.5" );

   DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
   sourceEdgeEnd->SetReference( node );
   edge->SetSource( sourceEdgeEnd );

   DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
   // don't know target yet
   targetEdgeEnd->SetAAFUIDReference( _elementTypeDef->GetAUID() );
   edge->SetTarget( targetEdgeEnd );

   graph->AddEdge( edge );


   graph->AddRecordNode( node );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionVariableArray::AAFTypeDefinitionVariableArray( string name, string auid, string description,
								AAFTypeDefinition *elementTypeDef )
   : AAFTypeDefinition( name, auid, description ), _elementTypeDef( elementTypeDef )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionVariableArray::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionVariableArray\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   DotEdge *edge = factory->CreateEdge( _name, factory->CreateEdgeUID() );

   edge->SetElementAttribute( "color", "blue" );
   edge->SetElementAttribute( "labelfontcolor", "blue" );
   edge->SetElementAttribute( "arrowhead", "open" );
   edge->SetElementAttribute( "arrowsize", "1.5" );

   DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
   sourceEdgeEnd->SetReference( node );
   edge->SetSource( sourceEdgeEnd );

   DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
   // don't know target yet
   targetEdgeEnd->SetAAFUIDReference( _elementTypeDef->GetAUID() );
   edge->SetTarget( targetEdgeEnd );

   graph->AddEdge( edge );


   graph->AddRecordNode( node );
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionSet::AAFTypeDefinitionSet( string name, string auid, string description,
					    AAFTypeDefinition *elementTypeDef )
   : AAFTypeDefinition( name, auid, description ), _elementTypeDef( elementTypeDef )
{
}

//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionSet::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionSet\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   DotEdge *edge = factory->CreateEdge( _name, factory->CreateEdgeUID() );

   edge->SetElementAttribute( "color", "blue" );
   edge->SetElementAttribute( "labelfontcolor", "blue" );
   edge->SetElementAttribute( "arrowhead", "open" );
   edge->SetElementAttribute( "arrowsize", "1.5" );

   DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
   sourceEdgeEnd->SetReference( node );
   edge->SetSource( sourceEdgeEnd );

   DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
   // don't know target yet
   targetEdgeEnd->SetAAFUIDReference( _elementTypeDef->GetAUID() );
   edge->SetTarget( targetEdgeEnd );

   graph->AddEdge( edge );


   graph->AddRecordNode( node );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionString::AAFTypeDefinitionString( string name, string auid, string description )
   : AAFTypeDefinition( name, auid, description )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionString::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionString\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );


   graph->AddRecordNode( node );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionStream::AAFTypeDefinitionStream( string name, string auid, string description )
   : AAFTypeDefinition( name, auid, description )
{
}

//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionStream::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionStream\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );


   graph->AddRecordNode( node );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionRecord::AAFTypeDefinitionRecord( string name, string auid, string description )
   : AAFTypeDefinition( name, auid, description )
{
}



//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionRecord::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionRecord\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   map< string, AAFTypeDefinition* >::iterator iter;
   for ( iter=_members.begin(); iter!=_members.end(); iter++ )
   {
      DotRecordNodeClassAttribute memAttribute( (*iter).first, (*iter).second->GetName() );
      node->AddClassAttribute( memAttribute );
   }


   graph->AddRecordNode( node );
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionRecord::AddMember( string name, AAFTypeDefinition *typeDef )
{
   _members.insert( map< string, AAFTypeDefinition* >::value_type( name, typeDef ) );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionRef::AAFTypeDefinitionRef( string name, string auid, string description,
					    string refClassDefAuid )
   : AAFTypeDefinition( name, auid, description ), _refClassDef( 0 ), _refClassDefAuid( refClassDefAuid )
{
}



//-----------------------------------------------------------------------------
AAFClassDefinition* 
AAFTypeDefinitionRef::GetRefClassDef( AAFDefinitionHome *home )
{
   if ( _refClassDef == 0 )
   {
      _refClassDef = dynamic_cast< AAFClassDefinition* > ( home->FindDefinition( _refClassDefAuid ) );
   }

   return _refClassDef;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionStrongObjRef::AAFTypeDefinitionStrongObjRef( string name, string auid, string description,
							      string refClassDefAuid )
   : AAFTypeDefinitionRef( name, auid, description, refClassDefAuid ) 
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionStrongObjRef::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionStrongObjRef\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   DotEdge *edge = factory->CreateEdge( _name, factory->CreateEdgeUID() );

   edge->SetElementAttribute( "color", "blue" );
   edge->SetElementAttribute( "labelfontcolor", "blue" );
   edge->SetElementAttribute( "arrowhead", "open" );
   edge->SetElementAttribute( "arrowsize", "1.5" );
   edge->SetElementAttribute( "weight", dotHome->GetProfile()->GetStrongRefEdgeWeight() );

   DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
   sourceEdgeEnd->SetReference( node );
   edge->SetSource( sourceEdgeEnd );

   DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
   // don't know target yet
   targetEdgeEnd->SetAAFUIDReference( GetRefClassDef( dotHome )->GetAUID() );
   edge->SetTarget( targetEdgeEnd );

   graph->AddEdge( edge );

   graph->AddRecordNode( node );


   // build the referenced class
   GetRefClassDef( dotHome )->BuildDot( dotHome, factory, graph );
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionWeakObjRef::AAFTypeDefinitionWeakObjRef( string name, string auid, string description,
							  string refClassDefAuid )
   : AAFTypeDefinitionRef( name, auid, description, refClassDefAuid ) 
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionWeakObjRef::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionWeakObjRef\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );

   DotEdge *edge = factory->CreateEdge( _name, factory->CreateEdgeUID() );

   edge->SetElementAttribute( "color", "blue" );
   edge->SetElementAttribute( "labelfontcolor", "blue" );
   edge->SetElementAttribute( "arrowhead", "open" );
   edge->SetElementAttribute( "arrowsize", "1.5" );

   DotEdgeEnd *sourceEdgeEnd = new DotEdgeEnd();
   sourceEdgeEnd->SetReference( node );
   edge->SetSource( sourceEdgeEnd );

   DotEdgeEnd *targetEdgeEnd = new DotEdgeEnd();
   // don't know target yet
   targetEdgeEnd->SetAAFUIDReference( GetRefClassDef( dotHome )->GetAUID() );
   edge->SetTarget( targetEdgeEnd );

   graph->AddEdge( edge );

   graph->AddRecordNode( node );


   // build the referenced class
   GetRefClassDef( dotHome )->BuildDot( dotHome, factory, graph );
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionIndirect::AAFTypeDefinitionIndirect( string name, string auid, string description )
   : AAFTypeDefinition( name, auid, description )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionIndirect::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionIndirect\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );


   graph->AddRecordNode( node );
}




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
AAFTypeDefinitionOpaque::AAFTypeDefinitionOpaque( string name, string auid, string description )
   : AAFTypeDefinitionIndirect( name, auid, description )
{
}


//-----------------------------------------------------------------------------
void 
AAFTypeDefinitionOpaque::BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph )
{
   string name = "\\<\\<TypeDefinitionOpaque\\>\\>";
   name += "\\n";
   name += _name;
   DotRecordNode *node = factory->CreateRecordNode( name, 
						    factory->CreateRecordNodeUID() );
   node->AddID( DotThingID( "aafuid", _auid ) );

   node->SetElementAttribute( "style", "filled" );
   node->SetElementAttribute( "color", "firebrick" );
   node->SetElementAttribute( "fontcolor", "black" );
   node->SetElementAttribute( "fillcolor", "lightyellow" );


   graph->AddRecordNode( node );
}

