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

#ifndef __EPLocatorVisitor_h_
#define __EPLocatorVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//Test/Result files
#include <TestResult.h>

//Ax files
#include <AxTypes.h>

//Not in the aafanalyzer namespace so the forward declaration must be made
//outside of the namespace.
class AxNetworkLocator;

namespace aafanalyzer {

class TestLevelTestResult;

class EPLocatorVisitor : public EPTypedVisitor
{

  public:
  
  EPLocatorVisitor( wostream& log,
		    boost::shared_ptr<TestLevelTestResult> spTestResult );
    
    virtual ~EPLocatorVisitor();

    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRGBAImageFileSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPCDCIImageFileSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImageFileSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMonoAudioFileSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node );
    
  private:
   
    wostream& _log;
    boost::shared_ptr<TestLevelTestResult> _spTestResult;

    bool CheckNetworkLocator( AxNetworkLocator& axNetLocator );

    // prohibited
    EPLocatorVisitor();
    EPLocatorVisitor( const EPLocatorVisitor& );
    EPLocatorVisitor& operator=( const EPLocatorVisitor& );
  
};

} // end of namespace diskstream

#endif /*__EPLocatorVisitor_h_*/
