#ifndef __DOTGRAPH_H__
#define __DOTGRAPH_H__


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
#include <DotRecordNode.h>
#include <DotEdge.h>
#include <DotSubGraph.h>

#include <vector>
#include <string>


//-----------------------------------------------------------------------------
// holds the default attributes for all nodes
class DefaultNode : public DotElement
{
  public:
   DefaultNode();
   virtual ~DefaultNode() {}

   virtual void Write( std::ofstream &dotFile, DotProfile &profile );

   virtual void RemoveReferenceToElement( DotElement *element ) {};

   virtual void ResolveReferences( DotFactory *dotFactory ) {};
};

extern int defaultNodeFontSize;


//-----------------------------------------------------------------------------
// holds the default attributes for all edges
class DefaultEdge : public DotElement
{
  public:
   DefaultEdge();

   virtual void Write( std::ofstream &dotFile, DotProfile &profile );

   virtual void RemoveReferenceToElement( DotElement *element ) {};

   virtual void ResolveReferences( DotFactory *dotFactory ) {};
};


//-----------------------------------------------------------------------------
// the top-level element in the DOT graph; contains all the record-nodes, edges 
// and sub-graphs
class DotGraph : public DotElement
{
  public:
   DotGraph( std::string name, std::string uid );
   virtual ~DotGraph() {};

   virtual void Write( std::ofstream &dotFile, DotProfile &profile );

   virtual void RemoveReferenceToElement( DotElement *element ) {};

   virtual void ResolveReferences( DotFactory *dotFactory ) {};

   void AddRecordNode( DotRecordNode *node );
   void AddEdge( DotEdge *edge );
   void AddSubGraph( DotSubGraph *subGraph );

  private:
   DotRecordNodeVector _nodes;
   DotEdgeVector _edges;
   DotSubGraphVector _subGraphs;

   DefaultNode _defaultNode;
   DefaultEdge _defaultEdge;

};

#endif	//__DOTGRAPH_H__

