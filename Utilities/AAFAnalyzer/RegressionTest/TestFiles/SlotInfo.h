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

#ifndef __SlotInfo_h_
#define __SlotInfo_h_

#include <AxTypes.h>
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace boost;

class SlotInfo
{

    public:

        SlotInfo( const AxString& element, const aafRational_t editRate, InputParser::OptionalStringAttrib name, InputParser::OptionalIntAttrib physicalTrackNum )
            : element( element ), 
              editRate( editRate ), 
              name( name ),
              physicalTrackNum( physicalTrackNum ),
              componentsSinceSlot( 0 )
        {}
        
        ~SlotInfo()
        {}

        SlotInfo( const SlotInfo& other )
            : element( other.element ), 
              editRate( other.editRate ), 
              name( other.name ),
              physicalTrackNum( other.physicalTrackNum ),
              componentsSinceSlot( other.componentsSinceSlot )
        {}
        
        AxString                            element;
        aafRational_t                       editRate;
        InputParser::OptionalStringAttrib   name;
        InputParser::OptionalIntAttrib      physicalTrackNum;
        aafUInt32                           componentsSinceSlot;

    private:
    
        //prohibited:
        SlotInfo& operator=( const SlotInfo& other );        

};

} // end of namespace diskstream

#endif /*__SlotInfo_h_*/
