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

#ifndef __EPMultiChannelAudioTest_h_
#define __EPMultiChannelAudioTest_h_

//Test/Result files
#include <Test.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

class TestLevelTestResult;

using namespace boost;

class EPMultiChannelAudioTest : public Test
{
 public:

  EPMultiChannelAudioTest( wostream& log,
                        shared_ptr<const TestGraph> spGraph );
  virtual ~EPMultiChannelAudioTest();

  virtual shared_ptr<TestLevelTestResult> Execute();
  virtual AxString GetName() const;
  virtual AxString GetDescription() const;
  static const TestInfo GetTestInfo();

 private:

  // prohibited
  EPMultiChannelAudioTest();
  EPMultiChannelAudioTest( const EPMultiChannelAudioTest& );
  EPMultiChannelAudioTest& operator=( const EPMultiChannelAudioTest& );

};

} // end of namespace diskstream

#endif /* __EPMultiChannelAudioTest_h_*/
