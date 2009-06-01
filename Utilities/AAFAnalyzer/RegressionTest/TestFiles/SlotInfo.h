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

#ifndef __SlotInfo_h_
#define __SlotInfo_h_

#include <AxTypes.h>
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace boost;

class SlotInfo
{

    public:

        typedef pair<const AxString, const bool> OptionalStringAttrib;
        typedef pair<const int, const bool> OptionalIntAttrib;

        SlotInfo( const AxString& element,
                  const aafRational_t editRate,
                  OptionalStringAttrib name,
                  OptionalIntAttrib physicalTrackNum,
                  OptionalIntAttrib markedInPoint,
                  OptionalIntAttrib markedOutPoint,
                  OptionalIntAttrib userPos,
                  aafPosition_t origin )
            : element( element ),
              editRate( editRate ),
              name( name ),
              physicalTrackNum( physicalTrackNum ),
              componentsSinceSlot( 0 ),
              markedInPoint( markedInPoint ),
              markedOutPoint( markedOutPoint ),
              userPos( userPos ),
              origin( origin )
        {}

        ~SlotInfo()
        {}

        SlotInfo( const SlotInfo& other )
            : element( other.element ),
              editRate( other.editRate ),
              name( other.name ),
              physicalTrackNum( other.physicalTrackNum ),
              componentsSinceSlot( other.componentsSinceSlot ),
              markedInPoint( other.markedInPoint ),
              markedOutPoint( other.markedOutPoint ),
              userPos( other.userPos ),
              origin( other.origin )
        {}

        AxString                            element;
        aafRational_t                       editRate;
        OptionalStringAttrib                name;
        OptionalIntAttrib                   physicalTrackNum;
        aafUInt32                           componentsSinceSlot;
        OptionalIntAttrib                   markedInPoint;
        OptionalIntAttrib                   markedOutPoint;
        OptionalIntAttrib                   userPos;
        aafPosition_t                       origin;

    private:

        //prohibited:
        SlotInfo& operator=( const SlotInfo& other );

};

} // end of namespace diskstream

#endif /*__SlotInfo_h_*/
