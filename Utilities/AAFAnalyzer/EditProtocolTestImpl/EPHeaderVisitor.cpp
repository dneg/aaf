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

//Edit Protocol Test files
#include "EPHeaderVisitor.h"
#include "EPHeaderTest.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <RequirementRegistry.h>

//Ax files
#include <AxHeader.h>

#include <AAFResult.h>
#include <AAFOPDefs.h>

namespace {
    
using namespace aafanalyzer;

} // end of namespace

//======================================================================

namespace aafanalyzer {

 
EPHeaderVisitor::EPHeaderVisitor( wostream& log,
				  boost::shared_ptr<TestLevelTestResult> spTestResult )
  : _log(log),
    _spTestResult( spTestResult )
{}
    
EPHeaderVisitor::~EPHeaderVisitor()
{}

bool EPHeaderVisitor::PreOrderVisit( AAFTypedObjNode<IAAFHeader>& node )
{
  AxHeader axHeader( node.GetAAFObjectOfType() );
    
  try
  {
    aafUID_t opPattern = axHeader.GetOperationalPattern();
    if ( opPattern != kAAFOPDef_EditProtocol )
    {
      _spTestResult->AddSingleResult( L"REQ_EP_255",
				      L"Header::OperationalPattern property is not equal to OpEditProtocol.",
				      TestResult::FAIL,
				      node );
    }
  }
  catch ( const AxExHResult& ex )
  {
    if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
    {
      _spTestResult->AddSingleResult( L"REQ_EP_255",
				      L"Header::OperationalPattern property is not present.",
				      TestResult::FAIL,
				      node );
    }
    else
    {
      throw;
    }
  }

  // There is no need to continue the traversal further, thefore,
  // return false.
  return false;
}

} // end of namespace aafanalyzer
