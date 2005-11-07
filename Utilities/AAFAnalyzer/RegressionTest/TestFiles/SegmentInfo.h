//=---------------------------------------------------------------------=
//
// $Id$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

#ifndef __SegmentInfo_h_
#define __SegmentInfo_h_

#include <AxTypes.h>
#include <AxComponent.h>
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace boost;

class SegmentInfo
{

    public:

        SegmentInfo( const AxString& element, shared_ptr<AxSegment> axSegment, const AxString& trackType, const AxString& slotType, const aafRational_t editRate, InputParser::OptionalStringAttrib name, InputParser::OptionalIntAttrib physicalTrackNum )
            : element( element ), 
              segment( axSegment ), 
              trackType( trackType ), 
              slotType( slotType ), 
              editRate( editRate ), 
              name( name ),
              physicalTrackNum( physicalTrackNum )
        {}
        
        ~SegmentInfo()
        {}

        SegmentInfo( const SegmentInfo& other )
            : element( other.element ), 
              segment( other.segment ), 
              trackType( other.trackType ), 
              slotType( other.slotType ), 
              editRate( other.editRate ), 
              name( other.name ),
              physicalTrackNum( other.physicalTrackNum )
        {}
        
        AxString                            element;
        shared_ptr<AxSegment>               segment;
        AxString                            trackType;
        AxString                            slotType;
        aafRational_t                       editRate;
        InputParser::OptionalStringAttrib   name;
        InputParser::OptionalIntAttrib      physicalTrackNum;

    private:
    
        //prohibited:
        SegmentInfo& operator=( const SegmentInfo& other );        

};

} // end of namespace diskstream

#endif /*__SegmentInfo_h_*/
