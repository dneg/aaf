#ifndef __DOTSUBGRAPH_H__
#define __DOTSUBGRAPH_H__

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


#include <DotElement.h>
#include <DotRecordNode.h>
#include <DotEdge.h>

#include <vector>
#include <string>


//-----------------------------------------------------------------------------
// a sub-graph is created when grouping objects that belong to the same MOB
class DotSubGraph : public DotElement
{
  public:
   DotSubGraph( std::string name, std::string uid );
   virtual ~DotSubGraph() {};

   virtual void Write( std::ofstream &dotFile, InstanceMapperProfile &profile );

   virtual void RemoveReferenceToElement( DotElement *element ) {};

   virtual void ResolveReferences( DotFactory *dotFactory ) {};

   void AddRecordNode( DotRecordNode *node );
   void AddEdge( DotEdge *edge );

  private:
   DotRecordNodeVector _nodes;
   DotEdgeVector _edges;

};

typedef std::vector< DotSubGraph* > DotSubGraphVector;

#endif	//__DOTSUBGRAPH_H__

