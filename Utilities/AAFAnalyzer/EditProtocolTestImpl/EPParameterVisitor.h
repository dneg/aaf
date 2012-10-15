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

#ifndef __EPParameterVisitor_h_
#define __EPParameterVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//STL files
#include <set>

namespace aafanalyzer {

class TestLevelTestResult;

using namespace std;

class EPParameterVisitor : public EPTypedVisitor
{

  public:
  
    EPParameterVisitor( wostream& log,
			boost::shared_ptr<EdgeMap> spEdgeMap,
			boost::shared_ptr<TestLevelTestResult> spTestResult );

    virtual ~EPParameterVisitor();

    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFParameter>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFVaryingValue>& node );
    
  private:
  
    // prohibited
    EPParameterVisitor();
    EPParameterVisitor( const EPParameterVisitor& );
    EPParameterVisitor& operator=( const EPParameterVisitor& );

    wostream& _log;
    boost::shared_ptr<EdgeMap> _spEdgeMap;
    boost::shared_ptr<TestLevelTestResult> _spTestResult;
    set<aafUID_t> _validInterpolationDefs;         
};

} // end of namespace diskstream

#endif /*__EPParameterVisitor_h_*/
