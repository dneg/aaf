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

//Test is not being run since libURI is insufficient, so get rid of
//the problem code.  Leave it commented for future reference.
//#include <URI.h>

//STL files
#include <sstream>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {
 
EPLocatorVisitor::EPLocatorVisitor( wostream& log,
                                    boost::shared_ptr<TestLevelTestResult> spTestResult )
  : _log(log),
    _spTestResult( spTestResult )
{}
    
EPLocatorVisitor::~EPLocatorVisitor()
{}

bool EPLocatorVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRGBAImageFileSource>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPLocatorVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPCDCIImageFileSource>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPLocatorVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImageFileSource>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPLocatorVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMonoAudioFileSource>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
}
  
bool EPLocatorVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node )
{
    boost::shared_ptr<EPTypedObjNode<IAAFSourceMob, EPFileSource> > spGeneric( node.DownCast<IAAFSourceMob, EPFileSource>() );
    return this->PreOrderVisit( *spGeneric );
}

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
                    _spTestResult->AddSingleResult( L"REQ_EP_060", 
                                                     L"EssenceDescriptor::Locator property of " + 
                                                    mobName + 
                                                    L" does not contain a locator pointing to any known file.", 
                                                    TestResult::WARN,
						    node );
                }
                
                locIter.Reset();
                
            }
            else if ( containerFormat != kAAFContainerDef_AAF )
            {
                _spTestResult->AddSingleResult( L"REQ_EP_059", 
                                                 L"Essence associated with " + mobName +
                                                L" is neither internal or external.",
                                                 TestResult::FAIL,
						node );
                testPassed = false;
            }
            
            //REQ_EP_061
            bool locatorFound = false;
            while ( locIter.NextOne( spLocator ) )
            {
                //If this is a Network Locator
                AxLocator axLocator( spLocator );
                IAAFNetworkLocatorSP spNetLoc;
                if ( AxIsA( axLocator, spNetLoc ) )
                {
                    AxNetworkLocator axNetLoc( spNetLoc );
                    if ( CheckNetworkLocator( axNetLoc ) )
                    {
                        locatorFound = true;
                        break;
                    }
                }
            }
            
            if ( !locatorFound )
            {
              wstring explain = L"EssenceDescriptor::Locator property of " + mobName + 
                                L" does not include any NetworkLocators that complies with"
                                L" the constrains set out in Section 6.9 of the AAF Edit Protocol.";
                _spTestResult->AddSingleResult( L"REQ_EP_061", 
                                                explain,
                                                TestResult::WARN,
						node );
            }
            
        }
        catch ( const AxExHResult& ex )
        {
            if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
            {
                throw ex;
            }
            
            wstring explain = mobName + L" does not have an EssenceDescriptor::Locator property.";

            _spTestResult->AddSingleResult( L"REQ_EP_060",
                                            explain,
                                            TestResult::WARN,
					    node );

            _spTestResult->AddSingleResult( L"REQ_EP_061",
                                            explain,
                                            TestResult::WARN,
					    node );
        }
                 
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
        {
            throw ex;
        }
        _spTestResult->AddSingleResult( L"REQ_EP_059", 
                                         mobName +
                                        L" does not have a FileDescriptor::ContainerDefinition property.",
                                        TestResult::FAIL,
					node );
        testPassed = false;
    }        
    
    return testPassed;
}

bool EPLocatorVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node )
{
    
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    AxImportDescriptor
      axImpDes( AxQueryInterface<IAAFEssenceDescriptor, IAAFImportDescriptor>( axSrcMob.GetEssenceDescriptor() ) );

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
            IAAFNetworkLocatorSP spNetLoc;
            if ( AxIsA( axLocator, spNetLoc ) )
            {
                AxNetworkLocator axNetLoc( spNetLoc );
                if ( CheckNetworkLocator( axNetLoc ) )
                {
                    locatorFound = true;
                    break;
                }
            }
        }
            
        if ( !locatorFound )
        {
          wstring explain = L"EssenceDescriptor::Locator property of " +
                            this->GetMobName( axSrcMob, L"Import Source" ) +
                            L" does not include any NetworkLocators that complies "
                            L"with the constraints set out in Section 6.9 of the AAF Edit Protocol.";

          _spTestResult->AddSingleResult( L"REQ_EP_075", 
                                          explain,
                                          TestResult::FAIL,
					  node );
          return false;
        }
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() != AAFRESULT_PROP_NOT_PRESENT )
        {
            throw ex;
        }

        wstring explain = this->GetMobName( axSrcMob, EPImportSource::GetName() ) + 
                          L" does not have an EssenceDescriptor::Locator property.";

        _spTestResult->AddSingleResult( L"REQ_EP_075", 
                                        explain,
                                        TestResult::FAIL,
					node );
        return false;
    }

    return true;
}

bool EPLocatorVisitor::CheckNetworkLocator( AxNetworkLocator& axNetLocator )
{

//    try
//    {
        //1. Has a URI that complies with RFC 2396.
        //NOTE: This code is not sufficient.  The libURI library does not seem
        //      to accept relative URIs (or I don't correctly understand how
        //      to specify one) and the library will not take wide characters.
        //      A new URI validator needs to be found/written.
//        AxString path( axNetLocator.GetPath() );
//        stringstream ss;
//        ss << path.c_str();
//        libURI::URI( ss.str().c_str() );

        //TODO: 2. Is an absolute or relative URI (can it be anything else).
        //TODO: 3. If absolute, conforms to RFC 1738.
        //TODO: 4. If relative, the base URI is determined from the URI of the AAF file
        //         itself.
//    }
//    catch ( libURI::URIException ex ) 
//    {
//        return false;
//    }
  
    return true;
}

} // end of namespace aafanalyzer
