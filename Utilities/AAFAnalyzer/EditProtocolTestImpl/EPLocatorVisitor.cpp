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

//Edit Protocol Test files
#include "EPLocatorVisitor.h"
#include "EPLocatorTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <RequirementRegistry.h>
#include <Requirement.h>

//Ax files
#include <AxMob.h>
#include <AxDefObject.h>
#include <AxIterator.h>
#include <AxMetaDef.h>
#include <AxEssence.h>

//AAF files
#include <AAFContainerDefs.h>
#include <AAFResult.h>
#include <AAFTypes.h>
#include <AAFClassDefUIDs.h>

//libURI files
//libURI can be found at: http://sourceforge.net/projects/liburi/
#include <URI.h>

//STL files
#include <sstream>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPLocatorVisitor::EPLocatorVisitor( wostream& log )
    : _log(log), 
      _spResult( new DetailLevelTestResult( L"Edit Protocol Locator Visitor",
                                            L"Visit File and Import Sources and make sure they're locators are acceptable under the Edit Protocol.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPLocatorTest::GetTestInfo().GetName() )
               )                          )
{}
    
EPLocatorVisitor::~EPLocatorVisitor()
{}

bool EPLocatorVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node )
{

    bool testPassed = true;
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );

    AxString mobName = this->GetMobName( axSrcMob, EPFileSource::GetName() );

    try
    {
        //REQ_EP_059
        AxFileDescriptor axFileDes( AxQueryInterface<IAAFEssenceDescriptor, IAAFFileDescriptor>( axSrcMob.GetEssenceDescriptor() ) );
        AxContainerDef axContDef( axFileDes.GetContainerFormat() );
        aafUID_t containerFormat = axContDef.GetAUID();
        
        try
        {
            AxLocatorIter locIter( axFileDes.GetLocators() );
    
            IAAFSmartPointer2<IAAFLocator> spLocator; 
            
            bool fileFound = false;
            
            if ( containerFormat == kAAFContainerDef_External )
            {
                
                //REQ_EP_060    
                //Repeat until the iterator is out of mob slots.
                while ( locIter.NextOne( spLocator ) )
                {
                    AxLocator axLocator( spLocator );
                    AxString fileLocation = axLocator.GetPath();
    
                    //TODO: Determine if the file exists  
                    if ( false )
                    {
                        fileFound = true;
                        break;
                    }
                }
                
                if ( !fileFound )
                {
                    _spResult->AddInformationResult( L"REQ_EP_060", 
                                                     L"EssenceDescriptor::Locator property of " + mobName + L" does not contain a locator pointing to any known file.", 
                                                     TestResult::WARN );
                }
                
                locIter.Reset();
                
            }
            else if ( containerFormat != kAAFContainerDef_AAF )
            {
                _spResult->AddInformationResult( L"REQ_EP_059", 
                                                 L"Essence associated with " + mobName + L" is neither internal or external.",
                                                 TestResult::FAIL );
                testPassed = false;
            }
            
            //REQ_EP_061
            bool locatorFound = false;
            while ( locIter.NextOne( spLocator ) )
            {
                //If this is a Network Locator
                AxLocator axLocator( spLocator );
                AxClassDef clsDef( axLocator.GetDefinition() );
                
                if ( this->IsType( clsDef, kAAFClassID_NetworkLocator, kAAFClassID_Locator ) )
                {
                    AxNetworkLocator axNetLoc( AxQueryInterface<IAAFLocator, IAAFNetworkLocator>( axLocator ) );
                    if ( CheckNetworkLocator( axNetLoc ) )
                    {
                        locatorFound = true;
                        break;
                    }
                }
            }
            
            if ( !locatorFound )
            {
                _spResult->AddInformationResult( L"REQ_EP_061", 
                                                 L"EssenceDescriptor::Locator property of " + mobName + L" does not include any NetworkLocators that complies with the constrains set out in Section 6.9 of the AAF Edit Protocol.",
                                                 TestResult::WARN );
            }
            
        }
        catch ( const AxExHResult& ex )
        {
            if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
            {
                throw ex;
            }
            
            shared_ptr<const Requirement> requirement = RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_060" );
            Requirement::RequirementMapSP reqMapSP(new Requirement::RequirementMap);
            (*reqMapSP)[L"REQ_EP_060"] = requirement;
            requirement = RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_061" );
            (*reqMapSP)[L"REQ_EP_061"] = requirement;
    
            shared_ptr<DetailLevelTestResult> spResult( new DetailLevelTestResult( _spResult->GetName(),
                                        L"-", // desc
                                        mobName + L" does not have an EssenceDescriptor::Locator property.",
                                        L"-", // docref
                                        TestResult::WARN,
                                        reqMapSP ) );
            spResult->SetRequirementStatus( TestResult::WARN, (*reqMapSP)[L"REQ_EP_060"] );
            spResult->SetRequirementStatus( TestResult::WARN, (*reqMapSP)[L"REQ_EP_061"] );
    
            _spResult->AppendSubtestResult( spResult );
            _spResult->SetResult( _spResult->GetAggregateResult() );
            _spResult->SetRequirementStatus( TestResult::WARN, (*reqMapSP)[L"REQ_EP_060"] );
            _spResult->SetRequirementStatus( TestResult::WARN, (*reqMapSP)[L"REQ_EP_061"] );
        }
                 
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
        {
            throw ex;
        }
        _spResult->AddInformationResult( L"REQ_EP_059", 
                                         mobName + L" does not have a FileDescriptor::ContainerDefinition property.",
                                         TestResult::FAIL );
        testPassed = false;
    }        
    
    return testPassed;
}

bool EPLocatorVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node )
{
    
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    AxImportDescriptor axImpDes( AxQueryInterface<IAAFEssenceDescriptor, IAAFImportDescriptor>( axSrcMob.GetEssenceDescriptor() ) );

    try
    {
        AxLocatorIter locIter( axImpDes.GetLocators() );
        IAAFSmartPointer2<IAAFLocator> spLocator;
            
        //REQ_EP_075
        bool locatorFound = false;
        while ( locIter.NextOne( spLocator ) )
        {
            //If this is a Network Locator
            AxLocator axLocator( spLocator );
            AxClassDef clsDef( axLocator.GetDefinition() );

            if ( this->IsType( clsDef, kAAFClassID_NetworkLocator, kAAFClassID_Locator ) )
            {
                AxNetworkLocator axNetLoc( AxQueryInterface<IAAFLocator, IAAFNetworkLocator>( axLocator ) );
                if ( CheckNetworkLocator( axNetLoc ) )
                {
                    locatorFound = true;
                    break;
                }
            }
        }
            
        if ( !locatorFound )
        {
            _spResult->AddInformationResult( L"REQ_EP_075", 
                                             L"EssenceDescriptor::Locator property of " + this->GetMobName( axSrcMob, L"Import Source" ) + L" does not include any NetworkLocators that complies with the constrains set out in Section 6.9 of the AAF Edit Protocol.",
                                             TestResult::FAIL );
            return false;
        }
            
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
        {
            throw ex;
        }
        _spResult->AddInformationResult( L"REQ_EP_075", 
                                         this->GetMobName( axSrcMob, EPImportSource::GetName() ) + L" does not have an EssenceDescriptor::Locator property.",
                                         TestResult::FAIL );
        return false;
    }

    return true;
}

bool EPLocatorVisitor::CheckNetworkLocator( AxNetworkLocator& axNetLocator )
{

    try
    {
        //1. Has a URI that complies with RFC 2396.
        //NOTE: This code is not sufficient.  The libURI library does not seem
        //      to accept relative URIs (or I don't correctly understand how
        //      to specify one) and the library will not take wide characters.
        //      A new URI validator needs to be found/written.
        AxString path( axNetLocator.GetPath() );
        stringstream ss;
        ss << path.c_str();
        libURI::URI( ss.str().c_str() );

        //TODO: 2. Is an absolute or relative URI (can it be anything else).
        //TODO: 3. If absolute, conforms to RFC 1738.
        //TODO: 4. If relative, the base URI is determined from the URI of the AAF file
        //         itself.
    }
    catch ( libURI::URIException ex ) 
    {
        return false;
    }
  
    return true;
}

shared_ptr<DetailLevelTestResult> EPLocatorVisitor::GetResult()
{
    return _spResult;
}

} // end of namespace aafanalyzer
