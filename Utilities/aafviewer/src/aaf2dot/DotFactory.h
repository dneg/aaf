#ifndef __DOTFACTORY_H__
#define __DOTFACTORY_H__

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


#include <DotRecordNode.h>
#include <DotEdge.h>

#include <AxSmartPointer.h>

#include <vector>

#include <DotGraph.h>
#include <DotSubGraph.h>
#include <DotRecordNode.h>
#include <DotEdge.h>

// the dot factory is used to create and hold the DOT elements (graph, sub-graph, record-node, edge.)
class DotFactory
{
  public:
   DotFactory() : _graphUID( 0 ), _subGraphUID( 0 ), _recordNodeUID( 0 ), _edgeUID( 0 ) {};
   ~DotFactory();

   DotGraph* CreateGraph( std::string name, std::string uid );
   DotSubGraph* CreateSubGraph( std::string name, std::string uid );
   DotRecordNode* CreateRecordNode( std::string name, std::string uid );
   DotEdge* CreateEdge( std::string name, std::string uid );

   DotGraph* GetGraph( std::string uid );
   DotSubGraph* GetSubGraph( std::string uid );
   DotRecordNode* GetRecordNode( std::string uid );
   DotRecordNode* GetRecordNodeAAFUID( std::string aafUID );
   DotRecordNode* GetRecordNodeMobID( std::string mobID );
   DotRecordNode* GetRecordNodeMobSlotID( std::string mobID, std::string mobSlotID );
   DotEdge* GetEdge( std::string uid );


   std::vector< DotEdge* > GetSlotEdgesWithSourceNode( DotRecordNode *node );
	
   void RemoveDotElement( DotElement *element );
   void RegisterRemoveDotElement( DotElement *element );

   void ResolveReferences();

   std::vector< DotElement* >& GetDotElements() { return _dotElements; }

   std::string CreateGraphUID();
   std::string CreateSubGraphUID();
   std::string CreateRecordNodeUID();
   std::string CreateEdgeUID();

  private:
   std::vector< DotElement* > _dotElements;
   std::vector< DotElement* > _registeredRemoveElements;

   long _graphUID;
   long _subGraphUID;
   long _recordNodeUID;
   long _edgeUID;

};


#endif	// __DOTFACTORY_H__

