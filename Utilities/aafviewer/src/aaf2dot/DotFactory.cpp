
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


#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include <iostream>
#include <sstream>
#include <exception>
#include <fstream>
using namespace std;

#include <DotFactory.h>
#include <Utilities.h>
#include <Logging.h>


//-----------------------------------------------------------------------------
DotFactory::~DotFactory()
{
   ErasePointerVector( _dotElements );
}


//-----------------------------------------------------------------------------
DotGraph*
DotFactory::CreateGraph( string name, string uid )
{
   if ( uid.length() > 0 )
   {
      DotGraph *graph = GetGraph( uid );

      if ( graph != 0 )
      {
	 RemoveDotElement( graph );
      }
   }

   DotGraph *graph = 0;
   graph = new DotGraph( name, uid );
   if ( graph != 0 )
   {
      _dotElements.push_back( graph );
   }

   return graph;
}


//-----------------------------------------------------------------------------
DotSubGraph*
DotFactory::CreateSubGraph( string name, string uid )
{
   if ( uid.length() > 0 )
   {
      DotSubGraph *subGraph = GetSubGraph( uid );

      if ( subGraph != 0 )
      {
	 RemoveDotElement( subGraph );
      }
   }

   DotSubGraph *subGraph = 0;
   subGraph = new DotSubGraph( name, uid );
   if ( subGraph != 0 )
   {
      _dotElements.push_back( subGraph );
   }

   return subGraph;
}


//-----------------------------------------------------------------------------
DotRecordNode*
DotFactory::CreateRecordNode( string name, string uid )
{
   if ( uid.length() > 0 )
   {
      DotRecordNode *node = GetRecordNode( uid );

      if ( node != 0 )
      {
	 RemoveDotElement( node );
      }
   }

   DotRecordNode *node = 0;
   node = new DotRecordNode( name, uid );
   if ( node != 0 )
   {
      _dotElements.push_back( node );
   }

   return node;
}


//-----------------------------------------------------------------------------
DotEdge*
DotFactory::CreateEdge( string name, string uid )
{
   if ( uid.length() > 0 )
   {
      DotEdge *edge = GetEdge( uid );

      if ( edge != 0 )
      {
	 RemoveDotElement( edge );
      }
   }

   DotEdge *edge = 0;
   edge = new DotEdge( name, uid );
   if ( edge != 0 )
   {
      _dotElements.push_back( edge );
   }

   return edge;
}


//-----------------------------------------------------------------------------
DotGraph*
DotFactory::GetGraph( string uid )
{
   if ( uid.length() == 0 )
   {
      return 0;
   }

   DotGraph *retGraph = 0;

   vector< DotElement* >::iterator iter;
   bool found = false;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end() && !found; iter++ )
   {
      DotGraph *graph = dynamic_cast< DotGraph* > (*iter);
      if ( graph != 0 && graph->GetUID().compare( uid ) == 0 )
      {
	 found = true;
	 retGraph = graph;
      }
   }

   return retGraph;
}


//-----------------------------------------------------------------------------
DotSubGraph*
DotFactory::GetSubGraph( string uid )
{
   if ( uid.length() == 0 )
   {
      return 0;
   }

   DotSubGraph *retSubGraph = 0;

   vector< DotElement* >::iterator iter;
   bool found = false;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end() && !found; iter++ )
   {
      DotSubGraph *subSubGraph = dynamic_cast< DotSubGraph* > (*iter);
      if ( subSubGraph != 0 && subSubGraph->GetUID().compare( uid ) == 0 )
      {
	 found = true;
	 retSubGraph = subSubGraph;
      }
   }

   return retSubGraph;
}


//-----------------------------------------------------------------------------
DotRecordNode*
DotFactory::GetRecordNode( string uid )
{
   if ( uid.length() == 0 )
   {
      return 0;
   }

   DotRecordNode *retNode = 0;

   vector< DotElement* >::iterator iter;
   bool found = false;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end() && !found; iter++ )
   {
      DotRecordNode *node = dynamic_cast< DotRecordNode* > (*iter);
      if ( node != 0 && node->GetUID().compare( uid ) == 0 )
      {
	 found = true;
	 retNode = node;
      }
   }

   return retNode;
}


//-----------------------------------------------------------------------------
DotEdge*
DotFactory::GetEdge( string uid )
{
   if ( uid.length() == 0 )
   {
      return 0;
   }

   DotEdge *retEdge = 0;

   vector< DotElement* >::iterator iter;
   bool found = false;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end() && !found; iter++ )
   {
      DotEdge *edge = dynamic_cast< DotEdge* > (*iter);
      if ( edge != 0 && edge->GetUID().compare( uid ) == 0 )
      {
	 found = true;
	 retEdge = edge;
      }
   }

   return retEdge;
}


//-----------------------------------------------------------------------------
void
DotFactory::RemoveDotElement( DotElement *element)
{
   // remove all references to this element
   vector< DotElement* >::iterator iter;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end(); iter++ )
   {
      (*iter)->RemoveReferenceToElement( element );
   }


   // remove the element
   bool found = false;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end() && !found; iter++ )
   {
      if ( (*iter)->GetUID().compare( element->GetUID() ) == 0 )
      {
	 found = true;
	 _dotElements.erase( iter );
	 delete *iter;
      }
   }

}


//-----------------------------------------------------------------------------
void
DotFactory::ResolveReferences()
{
   vector< DotElement* >::iterator iter;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end(); iter++ )
   {
      (*iter)->ResolveReferences( this );
   }

   while ( _registeredRemoveElements.size() > 0 )
   {
      vector< DotElement* >::iterator iter;
      for ( iter=_registeredRemoveElements.begin(); 
	    iter!=_registeredRemoveElements.end(); iter++ )
      {
	 RemoveDotElement( *iter );
      }
      _registeredRemoveElements.erase( _registeredRemoveElements.begin(),
				       _registeredRemoveElements.end() );

      ResolveReferences();
   }
}


//-----------------------------------------------------------------------------
void
DotFactory::RegisterRemoveDotElement( DotElement *element )
{
   _registeredRemoveElements.push_back( element );
}


//-----------------------------------------------------------------------------
// A MOB will have slots and a Nested Scope will have pseudo slots, ie segments
vector< DotEdge* > 
DotFactory::GetSlotEdgesWithSourceNode( DotRecordNode *node )
{
   vector< DotEdge* > retVector;
   vector< DotElement* >::iterator iter;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end(); iter++ )
   {
      DotEdge *edge = dynamic_cast< DotEdge* > (*iter);
      if ( edge != 0 )
      {
	 DotEdgeEnd *target = edge->GetTarget();
	 if ( target != 0 ) 
	 {
	    DotRecordNode *reference = target->GetReference();
	    if ( reference != 0 )
	    {
	       if ( reference->GetID( "isslot" ).length() > 0 || 
		    reference->GetID( "issegment" ).length() > 0 )
	       {
		  DotEdgeEnd *source = edge->GetSource();
		  if ( source != 0 )
		  {
		     DotRecordNode *reference = source->GetReference();
		     if ( reference->GetUID().compare( node->GetUID() ) == 0 )
		     {
			retVector.push_back( edge );
		     }
		  }
	       }
	    }
	 }
      }
   }

   return retVector;
}


//-----------------------------------------------------------------------------
DotRecordNode* 
DotFactory::GetRecordNodeAAFUID( string aafUID )
{
   if ( aafUID.length() == 0 )
   {
      return 0;
   }

   DotRecordNode *retNode = 0;

   vector< DotElement* >::iterator iter;
   bool found = false;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end() && !found; iter++ )
   {
      DotRecordNode *node = dynamic_cast< DotRecordNode* > (*iter);
      if ( node != 0 )
      {
	 string nodeAAFUID = node->GetID( "aafuid" );
	 if ( nodeAAFUID.compare( aafUID ) == 0 )
	 {
	    found = true;
	    retNode = node;
	 }
      }
   }

   return retNode;
}


//-----------------------------------------------------------------------------
DotRecordNode* 
DotFactory::GetRecordNodeMobID( string mobID )
{
   if ( mobID.length() == 0 )
   {
      return 0;
   }

   DotRecordNode *retNode = 0;

   vector< DotElement* >::iterator iter;
   bool found = false;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end() && !found; iter++ )
   {
      DotRecordNode *node = dynamic_cast< DotRecordNode* > (*iter);
      if ( node != 0 )
      {
	 string nodeMobID = node->GetID( "mobid" );
	 if ( nodeMobID.compare( mobID ) == 0 )
	 {
	    found = true;
	    retNode = node;
	 }
      }
   }

   return retNode;
}

	
//-----------------------------------------------------------------------------
DotRecordNode* 
DotFactory::GetRecordNodeMobSlotID( string mobID, string mobSlotID )
{
   if ( mobID.length() == 0  || mobSlotID.length() == 0 )
   {
      return 0;
   }

   DotRecordNode *retNode = 0;

   vector< DotElement* >::iterator iter;
   bool found = false;
   for ( iter=_dotElements.begin(); iter!=_dotElements.end() && !found; iter++ )
   {
      DotEdge *edge = dynamic_cast< DotEdge* > (*iter);
      if ( edge != 0 )
      {
	 DotEdgeEnd *source = edge->GetSource();
	 DotEdgeEnd *target = edge->GetTarget();
	 if ( source->GetReference() != 0 && target->GetReference() != 0 )
	 {
	    string sourceMobID = source->GetReference()->GetID( "mobid" );
	    string targetMobSlotID = target->GetReference()->GetID( "mobslotid" );
	    if ( sourceMobID.compare( mobID ) == 0 && targetMobSlotID.compare( mobSlotID ) == 0 )
	    {
	       found = true;
	       retNode = target->GetReference();
	    }
	 }
      }
   }

   return retNode;
}


//-----------------------------------------------------------------------------
string
DotFactory::CreateGraphUID()
{
   ostringstream newID;
   newID << "G" << ++_graphUID;
   return newID.str();
}


//-----------------------------------------------------------------------------
string
DotFactory::CreateSubGraphUID()
{
   ostringstream newID;
   newID << "SG" << ++_subGraphUID;
   return newID.str();
}


//-----------------------------------------------------------------------------
string
DotFactory::CreateRecordNodeUID()
{
   ostringstream newID;
   newID << "N" << ++_recordNodeUID;
   return newID.str();
}


//-----------------------------------------------------------------------------
string
DotFactory::CreateEdgeUID()
{
   ostringstream newID;
   newID << "E" << ++_edgeUID;
   return newID.str();
}


