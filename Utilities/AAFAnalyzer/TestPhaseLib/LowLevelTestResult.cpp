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

//Test/Result files
#include "LowLevelTestResult.h"
#include "TestRegistry.h"

//Requirement files
#include <RequirementMismatchException.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;
using namespace boost;

LowLevelTestResult::LowLevelTestResult( const Requirement::RequirementMapSP& requirements ) 
    : TestResult (requirements)
{}

LowLevelTestResult::LowLevelTestResult( const wstring& name, const wstring& desc,
                                        const wstring& explain, const wstring& docRef,
                                        Result defaultResult,
                                        const Requirement::RequirementMapSP& requirements )
    : TestResult (name, desc, explain, docRef, defaultResult, requirements)
{}

LowLevelTestResult::~LowLevelTestResult()
{}

void LowLevelTestResult::SetRequirementStatus( TestResult::Result level, const shared_ptr<const Requirement>& req)
{
    //If the requirement was already in a map store it in the map with
    //the worst possible status.  Otherwise, the requirement should not be
    //set because it is not part of this test result.
    Result oldReqLevel;
    if ( this->ContainsRequirment( req->GetId(), oldReqLevel ) ) {
        if ( level > oldReqLevel )
        {
            this->RemoveRequirement(req->GetId());
            this->AddRequirement(level, req);
        }
    } else if ( TestRegistry::GetInstance().IsUnsafeRequirements() ) {
        //If unsafe requirements are being used, then go ahead and add it to the
        //list of requirements stored by this test.  Do not register the 
        //requirement - it should not be reported as being covered by the 
        //associated test (although it is possible that it has been loaded).
        this->AddRequirement(level, req);
    } else {
            wstring msg;
            msg = L"Attempting to set status of " + req->GetId() + L" which does not exist in " + this->GetName();
            throw RequirementMismatchException ( msg.c_str() );

    }
}

} // end of namespace diskstream
