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

#pragma warning (disable:4786)

#include <iostream>
#include <sstream>
#include <assert.h>
#include <fstream>
using namespace std;

#include <DotEdge.h>
#include <DotRecordNode.h>
#include <DotFactory.h>
#include <Utilities.h>
#include <Logging.h>


 
//-----------------------------------------------------------------------------
DotEdge::DotEdge( string name, string uid )
   :	_source( 0 ), 
	_target( 0 ),
	DotElement( name, uid ) 
{
}


//-----------------------------------------------------------------------------
DotEdge::~DotEdge()
{
   delete _source;
   delete _target;
}


//-----------------------------------------------------------------------------
void 
DotEdge::Write( ofstream &dotFile, InstanceMapperProfile &profile )
{
   if ( _source != 0 && _target != 0 &&
	_source->GetReference() != 0 && _target->GetReference() != 0)
   {
      dotFile << _source->GetReference()->GetUID() << " -> "
	      << _target->GetReference()->GetUID();

      dotFile << "[";

      DotElement::WriteElementAttributes( dotFile, profile );

      dotFile << " ];" << endl;
   }
}


//-----------------------------------------------------------------------------
void 
DotEdge::RemoveReferenceToElement( DotElement *element )
{
   DotRecordNode *node = dynamic_cast< DotRecordNode* > ( element );
   if ( node != 0 )
   {
      if ( _source->GetReference()->GetUID().compare( node->GetUID() ) == 0 )
      {
	 _source = 0;
      }
      if ( _target->GetReference()->GetUID().compare( node->GetUID() ) == 0 )
      {
	 _target = 0;
      }
   }
}


//-----------------------------------------------------------------------------
// Note the assumption that Mob-MobSlot (a strong reference) references are already resolved
void 
DotEdge::ResolveReferences( DotFactory *dotFactory )
{
   if ( _source->GetReference() == 0 )
   {
      DotRecordNode *refNode = 0;
      if ( _source->GetAAFUIDReference().length() != 0 )
      {
	 refNode = dotFactory->GetRecordNodeAAFUID( _source->GetAAFUIDReference() );
	 if ( refNode == 0 )
	 {
	    Logging::DebugLogStream() << "Note: referenced node could not be found" << endl;
	 }
	 _source->SetReference( refNode );
      }
   }

   if ( _target->GetReference() == 0 )
   {
      DotRecordNode *refNode = 0;
      if ( _target->GetAAFUIDReference().length() != 0 )
      {
	 refNode = dotFactory->GetRecordNodeAAFUID( _target->GetAAFUIDReference() );
	 if ( refNode == 0 )
	 {
	    Logging::DebugLogStream() << "Note: referenced node could not be found" << endl;
	 }
	 _target->SetReference( refNode );
      }
      else if ( _target->GetMobIDReference().length() != 0 &&
		_target->GetMobSlotIDReference().length() != 0 )
      {
	 refNode = dotFactory->GetRecordNodeMobSlotID( _target->GetMobIDReference(),
						       _target->GetMobSlotIDReference() );
	 if ( refNode == 0 )
	 {
	    // if the referenced MOB is a source MOB, then the ID's should be zero and no warning is required
	    if ( _target->GetMobSlotIDReference().compare( "0" ) != 0 ||
		 _target->GetMobIDReference().compare( nullMobID ) != 0 )
	    {
	       Logging::DebugLogStream() << "Warning: referenced MOB slot not found." << endl
					 << "\tMob ID = " << _target->GetMobIDReference() << endl
					 << "\tMob Slot ID = " << _target->GetMobSlotIDReference() << endl;
	    }
	 }
	 _target->SetReference( refNode );
      }

   }
}


//-----------------------------------------------------------------------------
bool 
DotEdge::CompareDotEdgePtr( DotEdge *e1, DotEdge *e2 )
{
   string e1Label = e1->GetElementAttribute( "label" );
   string e2Label = e2->GetElementAttribute( "label" );

   // place edges with no label at the end of the list
   if ( e1Label.size() == 0 )
   {
      return false;
   }
   else if ( e2Label.size() == 0 )
   {
      return true;
   }
   
   return e1Label < e2Label;

}
