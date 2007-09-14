//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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

//Regression Test Structure files
#include "Test1Stub.h"
#include "Test2Stub.h"

//Base Test files
#include <AcyclicAnalysis.h>
#include <AcyclicVisitor.h>
#include <ResolveRefVisitor.h>
#include <DumpVisitor.h>
#include <TestGraph.h>

//Test/Result files
#include <TopLevelTestResult.h>
#include <TestPhaseLevelTestResult.h>
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>
#include <TestResult.h>
#include <TestInfoRegistrar.h>
#include <TestRegistry.h>
#include <TestRegistryException.h>

//Requirement files
#include <RequirementRegistry.h>
#include <RequirementMismatchException.h>
#include <RequirementRegistryException.h>
#include <RequirementLoader.h>
#include <Requirement.h>

//AAF Analyzer Base files
#include <GraphBuilder.h>
#include <ResolveRefVisitor.h>
#include <TypedVisitor.h>
#include <NodeFactoryImpl.h>
#include <NodeFactory.h>
#include <AAFTypedObjNode.h>
#include <AAFGraphInfo.h>
#include <AAFContainment.h>
#include <AAFMobReference.h>
#include <AAFSlotReference.h>
#include <AAFComponentReference.h>
#include <RefResolver.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>
#include <EdgeMap.h>
#include <Edge.h>
#include <TestGraph.h>

//Ax files
#include <AxTypes.h>
#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxSmartPointer.h>
#include <AxInit.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <iostream>
#include <map>
#include <sstream>

namespace {

using namespace aafanalyzer;
using namespace std;
using namespace boost;

void BasicBuilderTest( const AxString& filename )
{
  //Test the Graph Builder
  GraphBuilder graphBuild;
  shared_ptr<NodeFactory> spFactory(new NodeFactoryImpl());
  shared_ptr<const AAFGraphInfo> spGraphInfo( graphBuild.CreateGraph(filename, spFactory) );
  shared_ptr<const TestGraph> spTestGraph( spGraphInfo->GetGraph() );

  //Test the ResolveRefVisitor
  shared_ptr<RefResolver> ref( new RefResolver( wcerr, spTestGraph ) );
  ref->Execute();

  // Dump the full graph to std out so that it can be compared to a
  // known good result.
  shared_ptr<DumpVisitor> spDumpVisitor(new DumpVisitor(Visitor::FOLLOW_ALL));
  DepthFirstTraversal dfs(spTestGraph->GetEdgeMap(), spTestGraph->GetRootNode());
  dfs.TraverseDown(spDumpVisitor);
}

void CheckRegistries()
{
    bool testStatus = true;

    cout << endl << "Test the Requirement and Test Registries." << endl << endl;

    //Requirement Registry

    RequirementRegistry& rReg = RequirementRegistry::GetInstance();

    /*Test 1:
     *
     * Register requirements and verify that they are all stored
     *
     */

    wcout << L"Test 1: Correct Requirement Registration" << endl;

    shared_ptr<const Requirement> req01(new Requirement(L"TEST01", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST01", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req02(new Requirement(L"TEST02", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST02", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req03(new Requirement(L"TEST03", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST03", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req04(new Requirement(L"TEST04", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST04", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req05(new Requirement(L"TEST05", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST05", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req06(new Requirement(L"TEST06", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST06", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req07(new Requirement(L"TEST07", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST07", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req08(new Requirement(L"TEST08", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST08", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req09(new Requirement(L"TEST09", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST09", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req10(new Requirement(L"TEST10", Requirement::APP, L"app", Requirement::ADHOC, L"adhoc", L"TEST10", L"description", L"document", L"version", L"section") );

    try
    {
        rReg.Register( req01 );
        rReg.Register( req02 );
        rReg.Register( req03 );
        rReg.Register( req04 );
        rReg.Register( req05 );
        rReg.Register( req06 );
        rReg.Register( req07 );
        rReg.Register( req08 );
        rReg.Register( req09 );
        rReg.Register( req10 );
    }
    catch ( RequirementRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        wcout << L"Exception should not have been thrown." << endl;
        testStatus = false;
    }

    if (testStatus)
    {
        try
        {
            AxString temp = L"TEST01";
            if ( rReg.GetRequirement(temp) != req01 )
            {
                wcout << L"Wrong requirment returned." << endl;
                testStatus = false;
            }
        }
        catch ( RequirementRegistryException ex )
        {
            wcout << ex.widewhat() << endl;
            wcout << L"Exception should not have been thrown." << endl;
            testStatus = false;
        }
    }

    if (testStatus)
    {
        if (rReg.GetAllRequirements().size() != 10)
        {
            wcout << L"All requirements were not properly registered." << endl;
            testStatus = false;
        }
    }

    if (testStatus) {
        wcout << L"Test 1 passed" << endl;
    }
    else
    {
        wcout << L"Test 1 failed" << endl;
        assert( false );
    }

    /*Test 2:
     *
     * Ensure that re-registering throws an exception.
     *
     */

    wcout << L"Test 2: Duplicate Requirement Registration" << endl;

    try
    {
        rReg.Register( req01 );
        wcout << L"Exception should have been thrown when re-registering requirement." << endl;
        wcout << L"Test 2 failed" << endl;
        assert( false );
    }
    catch ( RequirementRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        wcout << L"Test 2 passed" << endl;
    }

    /*Test 3:
     *
     * Ensure that searching for an unregistered requirement throws an exception.
     *
     */

    cout << "Test 3: Search for Unregistered Requirement" << endl;

    try
    {
        AxString temp = L"Unregistered";
        rReg.GetRequirement(temp);
        wcout << L"Exception should have been thrown when searching for an unregistered requirement." << endl;
        wcout << L"Test 3 failed" << endl;
        assert( false );
    }
    catch ( RequirementRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        wcout << L"Test 3 passed" << endl;
    }

    //Test Registry

    TestRegistry& tReg = TestRegistry::GetInstance();

    /*Test 4:
     *
     * Register test and verify that their requirements are stored
     *
     */

    wcout << L"Test 4: Correct Test Registration" << endl;

    try
    {
        TestInfoRegistrar<Test1Stub> t1;
        TestInfoRegistrar<Test2Stub> t2;
    }
    catch ( TestRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        wcout << L"Exception should not have been thrown." << endl;
        testStatus = false;
    }

    if (testStatus)
    {
        try
        {
            if ( tReg.GetConstRequirementsForTest(L"Test1Stub")->size() != 3 )
            {
                wcout << L"Incorrect number of requirements returned." << endl;
                testStatus = false;
            }
        }
        catch ( TestRegistryException ex )
        {
            wcout << ex.widewhat() << endl;
            wcout << L"Exception should not have been thrown." << endl;
            testStatus = false;
        }
    }

    if (testStatus)
    {
        if (tReg.GetRequirementCoverage().size() != 6)
        {
            wcout << L"All requirements for test were not properly stored." << endl;
            testStatus = false;
        }
    }

    if (testStatus) {
        wcout << L"Test 4 passed" << endl;
    }
    else
    {
        wcout << L"Test 4 failed" << endl;
        assert( false );
    }

    /*Test 5:
     *
     * Ensure that re-registering throws an exception.
     *
     */

    wcout << L"Test 5: Duplicate Test Registration" << endl;

    try
    {
        TestInfoRegistrar<Test1Stub> tir;
        wcout << L"Exception should have been thrown when re-registering requirement." << endl;
        wcout << L"Test 5 failed" << endl;
        assert( false );
    }
    catch ( TestRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        wcout << L"Test 5 passed" << endl;
    }

    /*Test 6:
     *
     * Ensure that searching for requirements from an unregistered test throws
     * an exception.
     *
     */

    wcout << L"Test 6: Search for Unregistered Test" << endl;

    try
    {
        AxString temp = L"Unregistered";
        tReg.GetConstRequirementsForTest(L"Unregistered");
        wcout << L"Exception should have been thrown when searching for an unregistered test." << endl;
        wcout << L"Test 6 failed" << endl;
        assert( false );
    }
    catch ( TestRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        wcout << L"Test 6 passed" << endl;
    }

    /*
     * NOTE: The verifyTestResultCoverage requires a hierarchy of test results
     *       therefore it will be tested in CheckTestResultRequiremetns.
     *
     */
}

void CheckTestResultRequirements()
{

    wcout << endl << L"Test the requirement functionality of test results." << endl << endl;

    RequirementRegistry& rReg = RequirementRegistry::GetInstance();

    shared_ptr<const Requirement> req01(rReg.GetRequirement(L"TEST01") );
    shared_ptr<const Requirement> req02(rReg.GetRequirement(L"TEST02") );
    shared_ptr<const Requirement> req03(rReg.GetRequirement(L"TEST03") );
    shared_ptr<const Requirement> req04(rReg.GetRequirement(L"TEST04") );
    shared_ptr<const Requirement> req05(rReg.GetRequirement(L"TEST05") );
    shared_ptr<const Requirement> req06(rReg.GetRequirement(L"TEST06") );
    shared_ptr<const Requirement> req07(rReg.GetRequirement(L"TEST07") );
    shared_ptr<const Requirement> req08(rReg.GetRequirement(L"TEST08") );
    shared_ptr<const Requirement> req09(rReg.GetRequirement(L"TEST09") );
    shared_ptr<const Requirement> req10(rReg.GetRequirement(L"TEST10") );

    // JPT REVIEW - I modified the design of the TestResult classes to
    // an extent that rendered the tests that were implemented here
    // unusable. This should be rewritten.
}


} // end of namespace

//used to test the basic data structures of the software architecture
int main( int argc, char** argv )
{
  using namespace aafanalyzer;

  CheckRegistries();
  CheckTestResultRequirements();

  AxInit initObj;
  AxCmdLineArgs args( argc, argv );

  pair<bool,int> reqsArg = args.get( "-reqs" );
  pair<bool, const char*> requirementsFile( false, 0 );
  requirementsFile = args.get( reqsArg.second+1 );
  if ( !(reqsArg.first && requirementsFile.first) )
  {
    wcout << L"missing requirements file name argument" << endl;
    exit(-1);
  }

  pair<bool,int> fileArg = args.get( "-file" );
  pair<bool, const char*> aafTestFile( false, 0 );
  aafTestFile = args.get( fileArg.second+1 );
  if ( !(fileArg.first && aafTestFile.first) )
  {
    wcout << L"missing AAF test file name argument" << endl;
    exit(-1);
  }

  RequirementLoader loader;
  loader.ParseXML( requirementsFile.second );

  // Register tests implementations use by this regression test.
  TestInfoRegistrar<RefResolver> registerRefResolver;
  TestInfoRegistrar<AcyclicAnalysis> acyclicAnalysis;

  //shared_ptr<Node> parentOne(new AAFTypedObjNode<IAAFObject>(L"Header"));
  shared_ptr<Node> parentOne(new Node());
  shared_ptr<Node> parentTwo(new Node());
  shared_ptr<Node> childOne(new Node());
  shared_ptr<Node> childTwo(new Node());
  shared_ptr<Edge> edgeOne(new Edge(parentOne, childOne));
  shared_ptr<Edge> edgeTwo(new Edge(parentOne, childTwo));
  shared_ptr<Edge> edgeThree(new Edge(parentTwo, childTwo));

  //test the EdgeMap class
  shared_ptr<EdgeMap> spEdgeMap(new EdgeMap());

  spEdgeMap->AddEdge(edgeOne);
  spEdgeMap->AddEdge(edgeTwo);
  spEdgeMap->AddEdge(edgeThree);
  EdgeMap::ConstEdgeVectorSP theParentsOne = spEdgeMap->GetParents(childOne);
  EdgeMap::ConstEdgeVectorSP theParentsTwo = spEdgeMap->GetParents(childTwo);
  EdgeMap::ConstEdgeVectorSP theChildrenOne = spEdgeMap->GetChildren(parentOne);
  EdgeMap::ConstEdgeVectorSP theChildrenTwo = spEdgeMap->GetChildren(parentTwo);

  //ensure the correct edges were returned by checking Node LIDs
  assert(theParentsOne->front()->GetParentNode()->GetLID() == parentOne->GetLID());//parent of childOne
  assert(theParentsTwo->front()->GetParentNode()->GetLID() == parentOne->GetLID());//parent of childTwo
  assert(theParentsTwo->back()->GetParentNode()->GetLID() == parentTwo->GetLID());//parent of childTwo
  assert(theChildrenOne->front()->GetChildNode()->GetLID() == childOne->GetLID());//child of parentOne
  assert(theChildrenOne->back()->GetChildNode()->GetLID() == childTwo->GetLID());//child of parentOne
  assert(theChildrenTwo->front()->GetChildNode()->GetLID() == childTwo->GetLID());//child of parentTwo

  //test the DepthFirstTraversal class
  shared_ptr<DumpVisitor> spDumpVisitor(new DumpVisitor());
  shared_ptr<Node> fooOne(new Node());
  shared_ptr<Node> fooTwo(new Node());
  spEdgeMap->AddEdge(shared_ptr<Edge>(new Edge(childTwo, fooOne)));
  spEdgeMap->AddEdge(shared_ptr<Edge>(new Edge(childTwo, fooTwo)));

  DepthFirstTraversal dfs(spEdgeMap, parentOne);
  wcout << L"Traverse Down Test Graph:" << endl;
  dfs.TraverseDown(spDumpVisitor, parentOne);
  wcout << L"Traverse Up Test Graph:" << endl;
  dfs.TraverseUp(spDumpVisitor, fooOne);

  shared_ptr<TestGraph> spTestGraph( new TestGraph(spEdgeMap, parentOne) );
  shared_ptr<AcyclicAnalysis> spAcyclicTest( new AcyclicAnalysis(wcout, spTestGraph) );

  //test the Acyclic Visitor without cycles
  shared_ptr<TestLevelTestResult> spAcyclicResult( new TestLevelTestResult(spAcyclicTest) );
  shared_ptr<AcyclicVisitor> spAcyclicVisitorA(new AcyclicVisitor(wcout, spAcyclicResult));
  dfs.TraverseDown(spAcyclicVisitorA, parentOne);
  spAcyclicResult->ConsolidateResults();
  assert( spAcyclicResult->GetResult() == TestResult::PASS );

  //test the Acyclic Visitor without cycles
  shared_ptr<TestLevelTestResult> spCyclicResult( new TestLevelTestResult(spAcyclicTest) );
  shared_ptr<AcyclicVisitor> spAcyclicVisitorB(new AcyclicVisitor(wcout,spCyclicResult));
  spEdgeMap->AddEdge(shared_ptr<Edge>(new Edge(fooTwo, parentTwo)));//adds a cycle
  dfs.TraverseDown(spAcyclicVisitorB, parentOne);
  spCyclicResult->ConsolidateResults();
  assert( spCyclicResult->GetResult() == TestResult::FAIL );

  wcout << L"Load AAF file, resolve references, and dump full graph:" << endl;
  BasicBuilderTest( AxStringUtil::mbtowc(aafTestFile.second) );

  wcout << L"Completed Basic Testing Successfully" << endl << endl;

  return 0;
}
