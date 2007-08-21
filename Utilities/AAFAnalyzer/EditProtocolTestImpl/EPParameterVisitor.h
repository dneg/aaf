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

#ifndef __EPParameterVisitor_h_
#define __EPParameterVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//STL files
#include <set>

namespace aafanalyzer {

class TestLevelTestResult;

using namespace boost;
using namespace std;

class EPParameterVisitor : public EPTypedVisitor
{

  public:
  
    EPParameterVisitor( wostream& log,
			shared_ptr<EdgeMap> spEdgeMap,
			shared_ptr<TestLevelTestResult> spTestResult );

    virtual ~EPParameterVisitor();

    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFParameter>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFVaryingValue>& node );
    
  private:
  
    // prohibited
    EPParameterVisitor();
    EPParameterVisitor( const EPParameterVisitor& );
    EPParameterVisitor& operator=( const EPParameterVisitor& );

    wostream& _log;
    shared_ptr<EdgeMap> _spEdgeMap;
    shared_ptr<TestLevelTestResult> _spTestResult;
    set<aafUID_t> _validInterpolationDefs;         
};

} // end of namespace diskstream

#endif /*__EPParameterVisitor_h_*/
