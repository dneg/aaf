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

#ifdef WIN32
#pragma warning (disable:4786)
#endif


#include <iostream>
#include <sstream>
#include <assert.h>
#include <fstream>
#include <algorithm>
using namespace std;

#include <DotGraph.h>
#include <DotFactory.h>
#include <DotUtilities.h>
#include <DotLogging.h>

int defaultNodeFontSize = 10;

//-----------------------------------------------------------------------------
DefaultNode::DefaultNode()
   :	DotElement( "node","default-node" )
{
   SetElementAttribute( "shape", "record" );
   SetElementAttribute( "fontname", "Courier" );
   char buffer[10];
   sprintf( buffer, "\"%d\"", defaultNodeFontSize );
   SetElementAttribute( "fontsize", buffer );
}


//-----------------------------------------------------------------------------
void
DefaultNode::Write( ofstream &dotFile, DotProfile &profile )
{
   dotFile << _name << " [ ";

   WriteElementAttributes( dotFile, profile );

   dotFile << " ];" << endl;
}


//-----------------------------------------------------------------------------
DefaultEdge::DefaultEdge()
   :	DotElement( "edge","default-edge" )
{
   SetElementAttribute( "weight", "1" );
   SetElementAttribute( "color", "black" );
   SetElementAttribute( "style", "solid" );
}


//-----------------------------------------------------------------------------
void 
DefaultEdge::Write( ofstream &dotFile, DotProfile &profile )
{
   dotFile << _name << " [ ";

   WriteElementAttributes( dotFile, profile );

   dotFile << " ];" << endl;
}


//-----------------------------------------------------------------------------
DotGraph::DotGraph( std::string name, std::string uid ) 
   :	DotElement( name, uid )
{
   // ordering does not work properly in GraphViz 1.9 when using clusters
   // also, it causes it to fail sometimes (when choosing -nodictionary for example)
//   SetElementAttribute( "ordering", "out" );
}


//-----------------------------------------------------------------------------
void
DotGraph::Write( ofstream &dotFile, DotProfile &profile )
{
   if ( profile.GetConcentrate() )
   {
      SetElementAttribute( "concentrate", "true" );
   }
   else
   {
      SetElementAttribute( "concentrate", "false" );
   }

   dotFile << "digraph " << _name << "{" << endl;

   dotFile << "graph [ ";
   WriteElementAttributes( dotFile, profile );
   dotFile << " ];" << endl;

   _defaultNode.Write( dotFile, profile );
   _defaultEdge.Write( dotFile, profile );

   DotSubGraphVector::iterator iterSubGraph;
   for ( iterSubGraph=_subGraphs.begin(); iterSubGraph!=_subGraphs.end(); iterSubGraph++ )
   {
      (*iterSubGraph)->Write( dotFile, profile );
   }
   DotRecordNodeVector::iterator iterNode;
   for ( iterNode=_nodes.begin(); iterNode!=_nodes.end(); iterNode++ )
   {
      (*iterNode)->Write( dotFile, profile );
   }
   sort( _edges.begin(), _edges.end(),DotEdge::CompareDotEdgePtr );
   DotEdgeVector::iterator iterEdge;
   for ( iterEdge=_edges.begin(); iterEdge!=_edges.end(); iterEdge++ )
   {
      (*iterEdge)->Write( dotFile, profile );
   }

   dotFile << "}";
}


//-----------------------------------------------------------------------------
void
DotGraph::AddRecordNode( DotRecordNode *node )
{
   // todo: check if already exists
   _nodes.push_back( node );
}


//-----------------------------------------------------------------------------
void
DotGraph::AddEdge( DotEdge *edge )
{
   // todo: check if already exists
   _edges.push_back( edge );
}


//-----------------------------------------------------------------------------
void
DotGraph::AddSubGraph( DotSubGraph *subGraph )
{
   // todo: check if already exists
   _subGraphs.push_back( subGraph );
}

