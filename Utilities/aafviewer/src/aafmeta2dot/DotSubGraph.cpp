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

#include <DotSubGraph.h>
#include <DotFactory.h>
#include <DotUtilities.h>
#include <DotLogging.h>


//-----------------------------------------------------------------------------
DotSubGraph::DotSubGraph( std::string name, std::string uid ) 
   :	DotElement( name, uid )
{
}


//-----------------------------------------------------------------------------
void
DotSubGraph::Write( ofstream &dotFile, DotProfile &profile )
{
   dotFile << "subGraph " << _name << " {" << endl;
   WriteGlobalElementAttributes( dotFile, profile );

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

   dotFile << "};" << endl;
}


//-----------------------------------------------------------------------------
void
DotSubGraph::AddRecordNode( DotRecordNode *node )
{
   // todo: check if already exists
   _nodes.push_back( node );
}


//-----------------------------------------------------------------------------
void
DotSubGraph::AddEdge( DotEdge *edge )
{
   // todo: check if already exists
   _edges.push_back( edge );
}

