#ifndef __DOTEDGE_H__
#define __DOTEDGE_H__

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
#include <DotThingWithID.h>
#include <DotRecordNode.h>

#include <list>
#include <string>


//-----------------------------------------------------------------------------
// a edge (association) has 2 edge ends.
class DotEdgeEnd
{
  public:
   DotEdgeEnd() : _reference( 0 ) {};
   DotEdgeEnd( DotRecordNode *reference ) : _reference( reference ) {};
   ~DotEdgeEnd() {};

   // the referenced node for this edge
   void SetReference( DotRecordNode *reference ) { _reference = reference; }
   DotRecordNode* GetReference() { return _reference; }

   // the not yet resolved UID reference; will be resolved to a node reference later
   void SetAAFUIDReference( std::string aafUIDReference ) { _aafUIDReference = aafUIDReference; };
   std::string GetAAFUIDReference() { return _aafUIDReference; }

	
   // the not yet resolved MOB ID reference; will be resolved to a node reference later
   void SetMobIDReference( std::string aafMobIDReference ) { _aafMobIDReference = aafMobIDReference; };
   std::string GetMobIDReference() { return _aafMobIDReference; }

   // the not yet resolved MOB Slot ID reference; will be resolved to a node reference later
   void SetMobSlotIDReference( std::string aafMobSlotIDReference ) { _aafMobSlotIDReference = aafMobSlotIDReference; };
   std::string GetMobSlotIDReference() { return _aafMobSlotIDReference; }

  private:
   DotRecordNode *_reference;

   std::string _aafUIDReference;

   std::string _aafMobIDReference;
   std::string _aafMobSlotIDReference;
};


//-----------------------------------------------------------------------------
// represents an edge in a graph
class DotEdge : public DotElement, public DotThingWithID
{
  public:
   DotEdge( std::string name, std::string uid );
   virtual ~DotEdge();

   // write to DOT file
   virtual void Write( std::ofstream &dotFile, InstanceMapperProfile &profile );

   // if referenced DOT element (AAF object) is not included, then remove references to it
   virtual void RemoveReferenceToElement( DotElement *element );

   // resolve references to nodes
   virtual void ResolveReferences( DotFactory *dotFactory );

   // the 2 edges ends are the source and target
   void SetSource( DotEdgeEnd *source ) { _source = source; }
   void SetTarget( DotEdgeEnd *target ) { _target = target; }

   DotEdgeEnd* GetSource() { return _source; }
   DotEdgeEnd* GetTarget() { return _target; }

  public:
   // used for sorting the edges
   static bool CompareDotEdgePtr( DotEdge *e1, DotEdge *e2 );


  private:
   DotEdgeEnd *_source;
   DotEdgeEnd *_target;


};

typedef std::vector< DotEdge* > DotEdgeVector;


#endif	//__DOTEDGE_H__

