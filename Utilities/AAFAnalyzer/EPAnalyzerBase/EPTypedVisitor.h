//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __EPTYPEDVISITOR_h__
#define __EPTYPEDVISITOR_h__

//Edit Protocol Analyzer Base files
#include "EPTypedObjNode.h"

//AAF Analyzer Base files
#include <TypedVisitor.h>

//Ax files
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <set>

class AxMob;
class AxMobSlot;

namespace aafanalyzer {

using namespace std;
using namespace boost;

class EdgeMap;

class EPTypedVisitor : public TypedVisitor
{

  //Allow ancestor PreOrderVisit and PostOrderVisit functions to be called.
  using TypedVisitor::PreOrderVisit;
  using TypedVisitor::PostOrderVisit;

  public:

    typedef set<boost::shared_ptr<AAFObjNode> > MobSlotNodeSet;

    // Default constructor traverse uses the default TypedVisitor also
    // (i.e. uses Visitor::FOLLOW_CONTAINED).
    EPTypedVisitor();

    // Specialize the traversal by using this constructor.
    EPTypedVisitor( Visitor::Follow_e follow );

    virtual ~EPTypedVisitor();

    #include <EPTypedVisitor.h.gen>

  protected:

    //Functions commonly needed by EPTypedVisitors.  Put them in the base class
    //to avoid constantly re-implementing them.
    AxString GetMobName( AxMob& axMob, const AxString& type );
    AxString GetMobName( boost::shared_ptr<EdgeMap> spEdgeMap, Node& node );
    AxString GetMobSlotName( boost::shared_ptr<EdgeMap> spEdgeMap, Node& node );

    //TODO: These are currently only used by EPContainedTrackVisitor, they may
    //      not be needed by any other class and therefore can be pushed down
    //      to EPContainedTrackVisitor.  They are being left here for the time
    //      being in case any future tests require them.
    boost::shared_ptr<MobSlotNodeSet> GetEssenceTracks( boost::shared_ptr<EdgeMap> spEdgeMap, Node& node );
    boost::shared_ptr<MobSlotNodeSet> GetAudioTracks( boost::shared_ptr<EdgeMap> spEdgeMap, Node& node );
    boost::shared_ptr<MobSlotNodeSet> GetVideoTracks( boost::shared_ptr<EdgeMap> spEdgeMap, Node& node );
    boost::shared_ptr<MobSlotNodeSet> GetTimecodeTracks( boost::shared_ptr<EdgeMap> spEdgeMap, Node& node );
    boost::shared_ptr<MobSlotNodeSet> GetEdgecodeTracks( boost::shared_ptr<EdgeMap> spEdgeMap, Node& node );

  private:

    template <typename AAFObjectType, typename EPObjectType>
    bool ForwardPreOrderVisit( EPTypedObjNode<AAFObjectType, EPObjectType>& node )
    {
      boost::shared_ptr<Node> spNode = node.GetSharedPointerToNode();
      boost::shared_ptr<AAFTypedObjNode<AAFObjectType> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<AAFObjectType> >(spNode);
      return this->PreOrderVisit( *spBaseNode );
    }

    template <typename AAFObjectType, typename EPObjectType>
    bool ForwardPostOrderVisit( EPTypedObjNode<AAFObjectType, EPObjectType>& node )
    {
      boost::shared_ptr<Node> spNode = node.GetSharedPointerToNode();
      boost::shared_ptr<AAFTypedObjNode<AAFObjectType> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<AAFObjectType> >(spNode);
      return this->PostOrderVisit( *spBaseNode );
    }

    // prohibited
    EPTypedVisitor( const EPTypedVisitor& );
    EPTypedVisitor& operator=( const EPTypedVisitor& );

};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
