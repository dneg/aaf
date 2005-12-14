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
#include <TestVisitor.h>

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

void NameNodes( shared_ptr<Node> spRoot, shared_ptr<EdgeMap> spEdgeMap, map<AxString, Node::LID>& names, AxString& slotName, map<IAAFComponentSP, int>& clipIds, map<Node::LID, shared_ptr<Node> >& nodes )
{

    nodes[spRoot->GetLID()] = spRoot;

    //The very minimum we can do is cast the node to an AAFObjNode.
    shared_ptr<AAFObjNode> spAAFRoot = dynamic_pointer_cast<AAFObjNode>( spRoot );
    AxObject axObj( spAAFRoot->GetAAFObject() );

    AxString oldSlotName = slotName;

    //Object types to consider:
    IAAFHeaderSP            spHeader;
    IAAFContentStorageSP    spContentStorage;
    IAAFMobSP               spMob;
    IAAFMobSlotSP           spMobSlot;
    IAAFSequenceSP          spSequence;
    IAAFComponentSP         spComponent;

    if ( AxIsA( axObj, spComponent ) )
    {
        //Need to name components
        if ( clipIds.find( spComponent ) == clipIds.end() )
        {
            names[L"Component " + slotName] = spRoot->GetLID();
        }
        else
        {
            wstringstream name;
            name << slotName << L"." << clipIds[spComponent];
            slotName = name.str().c_str();
            names[L"Component " + slotName] = spRoot->GetLID();
        }

        if ( AxIsA( axObj, spSequence ) )
        {
            //Now, we need to find the index of each component in the sequence and
            //the address of the object being stored there.
            AxSequence axSequence( spSequence );
            for ( unsigned int i = 0; i < axSequence.CountComponents(); i++ )
            {
                clipIds[axSequence.GetComponentAt(i)] = i + 1;
            }
        }
        else
        {
            //Don't traverse past the component
            slotName = oldSlotName;
            return;
        }
    }
    else if ( AxIsA( axObj, spMobSlot ) )
    {
        AxMobSlot axMobSlot( spMobSlot );
        slotName = axMobSlot.GetName();
        names[slotName] = spRoot->GetLID();
        slotName = slotName.substr( 5 );
    }
    else if ( AxIsA( axObj, spMob ) )
    {
        AxMob axMob( spMob );
        names[axMob.GetName()] = spRoot->GetLID();
    }

    //Traverse
    EdgeMap::ConstEdgeVectorSP children = spEdgeMap->GetChildren( spRoot );
    for ( unsigned int i = 0; i < children->size(); i++ )
    {
        NameNodes( children->at(i)->GetChildNode(), spEdgeMap, names, slotName, clipIds, nodes );
    }

    slotName = oldSlotName;
    return;

}

template<typename EdgeType>
unsigned int CountEdges( map<Node::LID, shared_ptr<Node> >& nodes, shared_ptr<EdgeMap> edgeMap )
{

    unsigned int count = 0;
    map<Node::LID, shared_ptr<Node> >::const_iterator iter;

    for ( iter = nodes.begin(); iter != nodes.end(); iter++ )
    {
        EdgeMap::ConstEdgeVectorSP children = edgeMap->GetChildren( iter->second );
        for ( unsigned int i = 0; i < children->size(); i++ )
        {
            shared_ptr<Edge> spEdge = children->at(i);
            shared_ptr<EdgeType> spTypedEdge = dynamic_pointer_cast<EdgeType>( spEdge );
            if ( spTypedEdge )
            {
                count++;
            }
        }
    }

    return count;

}

template<typename EdgeType>
bool EdgeExists( shared_ptr<Node> parent, Node::LID childId, shared_ptr<EdgeMap> spEdgeMap )
{
    EdgeMap::ConstEdgeVectorSP children = spEdgeMap->GetChildren( parent );
    for ( unsigned int i = 0; i < children->size(); i++ )
    {
        shared_ptr<Edge> spEdge = children->at(i);
        if ( spEdge->GetChildNode()->GetLID() == childId )
        {
            shared_ptr<EdgeType> spTypedEdge = dynamic_pointer_cast<EdgeType>( spEdge );
            if ( spTypedEdge )
            {
                //Edge was found to be of the correct type.
                return true;
            }
            else
            {
                //Edge was found, but it is the wrong type.
                return false;
            }
        }
    }
    //Edge was not found
    return false;
}

void CheckEdges( const AxString& filename )
{
  //Test the Graph Builder
  GraphBuilder graphBuild;
  shared_ptr<NodeFactory> spFactory(new NodeFactoryImpl());
  shared_ptr<const AAFGraphInfo> spGraphInfo( graphBuild.CreateGraph(filename, spFactory) );
  shared_ptr<const TestGraph> spTestGraph( spGraphInfo->GetGraph() );

  //First, attempt to give an appropriate name to every node of a type that we
  //care about (Mob, MobSlot, Sequence, SourceClip).  There are easier ways to
  //do this, but, the test avoids using Analzer structures wherever possible.
  AxString slotName;
  map<AxString, Node::LID> namedNodes;
  map<Node::LID, shared_ptr<Node> > nodeMap;
  map<IAAFComponentSP, int> addressMap;
  NameNodes( spTestGraph->GetRootNode(), spTestGraph->GetEdgeMap(), namedNodes, slotName, addressMap, nodeMap );

  //Now check that there are the correct number of nodes in the graph:
  //  3 Header -> (Content Storage, Dictionary, Identification)
  //  4 Dictionary -> Container Definition
  // 12 Dictionary -> Data Definition
  //  2 Dictionary -> Operation Definition
  // 40 Defined in the XML file
  // --
  // 59 Total
  //All of these edges should be Containment Edges

  cout << "***Graph Builder Tester***" << endl;
  unsigned int containmentEdges = CountEdges<AAFContainment>( nodeMap, spTestGraph->GetEdgeMap() );
  unsigned int mobEdges = CountEdges<AAFMobReference>( nodeMap, spTestGraph->GetEdgeMap() );
  unsigned int slotEdges = CountEdges<AAFSlotReference>( nodeMap, spTestGraph->GetEdgeMap() );
  unsigned int componentEdges = CountEdges<AAFComponentReference>( nodeMap, spTestGraph->GetEdgeMap() );

  cout << "Containment Edges: " << containmentEdges << endl;
  cout << "Mob References: " << mobEdges << endl;
  cout << "Mob Slot References: " << mobEdges << endl;
  cout << "Component References: " << componentEdges << endl;

  assert( containmentEdges == 635);
  assert( mobEdges == 0 );
  assert( slotEdges == 0 );
  assert( componentEdges == 0 );

  //Test the ResolveRefVisitor
  TestInfoRegistrar<RefResolver> registerRefResolver;
  shared_ptr<RefResolver> ref( new RefResolver( wcerr, spTestGraph ) );
  ref->Execute();

  //Now check that there are the correct number of nodes in the graph:
  //59 Containment Edges from before + 4 Mob References + 4 Mob Slot References
  //+ 8 Component References

  cout << "***ResolveRef Tester***" << endl;
  containmentEdges = CountEdges<AAFContainment>( nodeMap, spTestGraph->GetEdgeMap() );
  mobEdges = CountEdges<AAFMobReference>( nodeMap, spTestGraph->GetEdgeMap() );
  slotEdges = CountEdges<AAFSlotReference>( nodeMap, spTestGraph->GetEdgeMap() );
  componentEdges = CountEdges<AAFComponentReference>( nodeMap, spTestGraph->GetEdgeMap() );

  cout << "Containment Edges: " << containmentEdges << endl;
  cout << "Mob References: " << mobEdges << endl;
  cout << "Mob Slot References: " << mobEdges << endl;
  cout << "Component References: " << componentEdges << endl;

  assert( containmentEdges == 635 );
  assert( mobEdges == 71 );
  assert( slotEdges == 71 );
  assert( componentEdges == 128 );

  //Now test that the 16 resolved edges are the edges that we expect.
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.A.1"]], namedNodes[L"Mob 2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.A.2"]], namedNodes[L"Mob 3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.A.3"]], namedNodes[L"Mob 4"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.B"]], namedNodes[L"Mob 5"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.C"]], namedNodes[L"Mob 6"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.D"]], namedNodes[L"Mob 7"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.E"]], namedNodes[L"Mob 8"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.F"]], namedNodes[L"Mob 9"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.G"]], namedNodes[L"Mob 10"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.H"]], namedNodes[L"Mob 11"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.I"]], namedNodes[L"Mob 12"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.J"]], namedNodes[L"Mob 13"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.K"]], namedNodes[L"Mob 14"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.L"]], namedNodes[L"Mob 15"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.M"]], namedNodes[L"Mob 16"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.N"]], namedNodes[L"Mob 17"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.O"]], namedNodes[L"Mob 18"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.P"]], namedNodes[L"Mob 19"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.Q"]], namedNodes[L"Mob 20"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.R"]], namedNodes[L"Mob 21"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.S"]], namedNodes[L"Mob 22"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.T"]], namedNodes[L"Mob 23"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.U"]], namedNodes[L"Mob 24"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.V"]], namedNodes[L"Mob 25"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.W"]], namedNodes[L"Mob 26"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.X"]], namedNodes[L"Mob 27"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.Y"]], namedNodes[L"Mob 28"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.Z"]], namedNodes[L"Mob 29"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AA"]], namedNodes[L"Mob 30"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AB"]], namedNodes[L"Mob 31"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AC"]], namedNodes[L"Mob 32"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AD"]], namedNodes[L"Mob 33"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AE"]], namedNodes[L"Mob 34"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AF"]], namedNodes[L"Mob 35"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AG"]], namedNodes[L"Mob 36"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AH"]], namedNodes[L"Mob 37"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AI"]], namedNodes[L"Mob 38"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AJ"]], namedNodes[L"Mob 39"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AK"]], namedNodes[L"Mob 40"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AL"]], namedNodes[L"Mob 41"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AM"]], namedNodes[L"Mob 42"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AN"]], namedNodes[L"Mob 43"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AO"]], namedNodes[L"Mob 44"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AP"]], namedNodes[L"Mob 45"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AQ"]], namedNodes[L"Mob 46"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AR"]], namedNodes[L"Mob 47"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AS"]], namedNodes[L"Mob 48"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AT"]], namedNodes[L"Mob 49"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AU"]], namedNodes[L"Mob 50"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AV"]], namedNodes[L"Mob 51"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AW"]], namedNodes[L"Mob 52"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AX"]], namedNodes[L"Mob 53"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AY"]], namedNodes[L"Mob 54"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.AZ"]], namedNodes[L"Mob 55"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BA"]], namedNodes[L"Mob 56"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BB"]], namedNodes[L"Mob 57"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BC"]], namedNodes[L"Mob 58"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BD"]], namedNodes[L"Mob 59"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BE"]], namedNodes[L"Mob 60"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BF"]], namedNodes[L"Mob 61"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BG"]], namedNodes[L"Mob 62"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BH"]], namedNodes[L"Mob 63"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BI"]], namedNodes[L"Mob 64"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BJ"]], namedNodes[L"Mob 65"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BK"]], namedNodes[L"Mob 66"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BL"]], namedNodes[L"Mob 67"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BM"]], namedNodes[L"Mob 68"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BN"]], namedNodes[L"Mob 69"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BO"]], namedNodes[L"Mob 70"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BP"]], namedNodes[L"Mob 71"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFMobReference>(nodeMap[namedNodes[L"Component 1.BQ"]], namedNodes[L"Mob 72"], spTestGraph->GetEdgeMap() ) );

  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.A.1"]], namedNodes[L"Slot 2.B"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.A.2"]], namedNodes[L"Slot 3.B"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.A.3"]], namedNodes[L"Slot 4.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.B"]], namedNodes[L"Slot 5.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.C"]], namedNodes[L"Slot 6.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.D"]], namedNodes[L"Slot 7.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.E"]], namedNodes[L"Slot 8.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.F"]], namedNodes[L"Slot 9.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.G"]], namedNodes[L"Slot 10.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.H"]], namedNodes[L"Slot 11.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.I"]], namedNodes[L"Slot 12.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.J"]], namedNodes[L"Slot 13.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.K"]], namedNodes[L"Slot 14.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.L"]], namedNodes[L"Slot 15.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.M"]], namedNodes[L"Slot 16.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.N"]], namedNodes[L"Slot 17.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.O"]], namedNodes[L"Slot 18.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.P"]], namedNodes[L"Slot 19.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.Q"]], namedNodes[L"Slot 20.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.R"]], namedNodes[L"Slot 21.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.S"]], namedNodes[L"Slot 22.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.T"]], namedNodes[L"Slot 23.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.U"]], namedNodes[L"Slot 24.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.V"]], namedNodes[L"Slot 25.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.W"]], namedNodes[L"Slot 26.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.X"]], namedNodes[L"Slot 27.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.Y"]], namedNodes[L"Slot 28.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.Z"]], namedNodes[L"Slot 29.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AA"]], namedNodes[L"Slot 30.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AB"]], namedNodes[L"Slot 31.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AC"]], namedNodes[L"Slot 32.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AD"]], namedNodes[L"Slot 33.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AE"]], namedNodes[L"Slot 34.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AF"]], namedNodes[L"Slot 35.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AG"]], namedNodes[L"Slot 36.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AH"]], namedNodes[L"Slot 37.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AI"]], namedNodes[L"Slot 38.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AJ"]], namedNodes[L"Slot 39.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AK"]], namedNodes[L"Slot 40.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AL"]], namedNodes[L"Slot 41.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AM"]], namedNodes[L"Slot 42.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AN"]], namedNodes[L"Slot 43.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AO"]], namedNodes[L"Slot 44.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AP"]], namedNodes[L"Slot 45.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AQ"]], namedNodes[L"Slot 46.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AR"]], namedNodes[L"Slot 47.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AS"]], namedNodes[L"Slot 48.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AT"]], namedNodes[L"Slot 49.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AU"]], namedNodes[L"Slot 50.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AV"]], namedNodes[L"Slot 51.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AW"]], namedNodes[L"Slot 52.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AX"]], namedNodes[L"Slot 53.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AY"]], namedNodes[L"Slot 54.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.AZ"]], namedNodes[L"Slot 55.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BA"]], namedNodes[L"Slot 56.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BB"]], namedNodes[L"Slot 57.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BC"]], namedNodes[L"Slot 58.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BD"]], namedNodes[L"Slot 59.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BE"]], namedNodes[L"Slot 60.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BF"]], namedNodes[L"Slot 61.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BG"]], namedNodes[L"Slot 62.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BH"]], namedNodes[L"Slot 63.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BI"]], namedNodes[L"Slot 64.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BJ"]], namedNodes[L"Slot 65.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BK"]], namedNodes[L"Slot 66.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BL"]], namedNodes[L"Slot 67.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BM"]], namedNodes[L"Slot 68.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BN"]], namedNodes[L"Slot 69.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BO"]], namedNodes[L"Slot 70.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BP"]], namedNodes[L"Slot 71.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFSlotReference>(nodeMap[namedNodes[L"Component 1.BQ"]], namedNodes[L"Slot 72.A"], spTestGraph->GetEdgeMap() ) );

  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.A.1"]], namedNodes[L"Component 2.B"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.A.2"]], namedNodes[L"Component 3.B"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.A.3"]], namedNodes[L"Component 4.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.A.3"]], namedNodes[L"Component 4.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.A.3"]], namedNodes[L"Component 4.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.B"]], namedNodes[L"Component 5.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.B"]], namedNodes[L"Component 5.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.B"]], namedNodes[L"Component 5.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.D"]], namedNodes[L"Component 7.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.E"]], namedNodes[L"Component 8.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.F"]], namedNodes[L"Component 9.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.F"]], namedNodes[L"Component 9.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.G"]], namedNodes[L"Component 10.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.H"]], namedNodes[L"Component 11.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.H"]], namedNodes[L"Component 11.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.I"]], namedNodes[L"Component 12.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.J"]], namedNodes[L"Component 13.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.L"]], namedNodes[L"Component 15.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.L"]], namedNodes[L"Component 15.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.L"]], namedNodes[L"Component 15.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.M"]], namedNodes[L"Component 16.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.M"]], namedNodes[L"Component 16.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.M"]], namedNodes[L"Component 16.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.N"]], namedNodes[L"Component 17.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.N"]], namedNodes[L"Component 17.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.N"]], namedNodes[L"Component 17.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.O"]], namedNodes[L"Component 18.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.O"]], namedNodes[L"Component 18.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.P"]], namedNodes[L"Component 19.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.P"]], namedNodes[L"Component 19.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.Q"]], namedNodes[L"Component 20.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.Q"]], namedNodes[L"Component 20.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.Q"]], namedNodes[L"Component 20.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.V"]], namedNodes[L"Component 25.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.W"]], namedNodes[L"Component 26.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.X"]], namedNodes[L"Component 27.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.Y"]], namedNodes[L"Component 28.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.Z"]], namedNodes[L"Component 29.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AA"]], namedNodes[L"Component 30.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AB"]], namedNodes[L"Component 31.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AC"]], namedNodes[L"Component 32.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AD"]], namedNodes[L"Component 33.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AE"]], namedNodes[L"Component 34.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AE"]], namedNodes[L"Component 34.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AF"]], namedNodes[L"Component 35.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AF"]], namedNodes[L"Component 35.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AG"]], namedNodes[L"Component 36.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AG"]], namedNodes[L"Component 36.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AH"]], namedNodes[L"Component 37.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AH"]], namedNodes[L"Component 37.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AI"]], namedNodes[L"Component 38.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AI"]], namedNodes[L"Component 38.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AJ"]], namedNodes[L"Component 39.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AJ"]], namedNodes[L"Component 39.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AK"]], namedNodes[L"Component 40.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AK"]], namedNodes[L"Component 40.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AK"]], namedNodes[L"Component 40.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AL"]], namedNodes[L"Component 41.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AL"]], namedNodes[L"Component 41.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AL"]], namedNodes[L"Component 41.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AM"]], namedNodes[L"Component 42.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AM"]], namedNodes[L"Component 42.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AM"]], namedNodes[L"Component 42.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AN"]], namedNodes[L"Component 43.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AN"]], namedNodes[L"Component 43.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AN"]], namedNodes[L"Component 43.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AO"]], namedNodes[L"Component 44.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AO"]], namedNodes[L"Component 44.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AO"]], namedNodes[L"Component 44.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AP"]], namedNodes[L"Component 45.A"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AQ"]], namedNodes[L"Component 46.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AQ"]], namedNodes[L"Component 46.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AT"]], namedNodes[L"Component 49.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AT"]], namedNodes[L"Component 49.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AT"]], namedNodes[L"Component 49.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AU"]], namedNodes[L"Component 50.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AU"]], namedNodes[L"Component 50.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AU"]], namedNodes[L"Component 50.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AV"]], namedNodes[L"Component 51.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AV"]], namedNodes[L"Component 51.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AV"]], namedNodes[L"Component 51.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AW"]], namedNodes[L"Component 52.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AW"]], namedNodes[L"Component 52.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AW"]], namedNodes[L"Component 52.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AY"]], namedNodes[L"Component 54.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AZ"]], namedNodes[L"Component 55.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AZ"]], namedNodes[L"Component 55.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.AZ"]], namedNodes[L"Component 55.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BA"]], namedNodes[L"Component 56.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BA"]], namedNodes[L"Component 56.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BA"]], namedNodes[L"Component 56.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BB"]], namedNodes[L"Component 57.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BB"]], namedNodes[L"Component 57.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BB"]], namedNodes[L"Component 57.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BC"]], namedNodes[L"Component 58.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BC"]], namedNodes[L"Component 58.A.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BC"]], namedNodes[L"Component 58.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BD"]], namedNodes[L"Component 59.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BF"]], namedNodes[L"Component 61.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BF"]], namedNodes[L"Component 61.A.2.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BG"]], namedNodes[L"Component 62.A.2.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BG"]], namedNodes[L"Component 62.A.2.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BH"]], namedNodes[L"Component 63.A.2.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BH"]], namedNodes[L"Component 63.A.2.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BI"]], namedNodes[L"Component 64.A.2.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BI"]], namedNodes[L"Component 64.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BJ"]], namedNodes[L"Component 65.A.2.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BK"]], namedNodes[L"Component 66.A.2.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BL"]], namedNodes[L"Component 67.A.2.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BM"]], namedNodes[L"Component 68.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BM"]], namedNodes[L"Component 68.A.2.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BM"]], namedNodes[L"Component 68.A.2.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BN"]], namedNodes[L"Component 69.A.2.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BN"]], namedNodes[L"Component 69.A.2.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BN"]], namedNodes[L"Component 69.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BO"]], namedNodes[L"Component 70.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BO"]], namedNodes[L"Component 70.A.2.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BO"]], namedNodes[L"Component 70.A.2.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BO"]], namedNodes[L"Component 70.A.2.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BP"]], namedNodes[L"Component 71.A.2.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BP"]], namedNodes[L"Component 71.A.2.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BP"]], namedNodes[L"Component 71.A.2.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BP"]], namedNodes[L"Component 71.A.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BQ"]], namedNodes[L"Component 72.A.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BQ"]], namedNodes[L"Component 72.A.2.1"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BQ"]], namedNodes[L"Component 72.A.2.2"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BQ"]], namedNodes[L"Component 72.A.2.3"], spTestGraph->GetEdgeMap() ) );
  assert(EdgeExists<AAFComponentReference>(nodeMap[namedNodes[L"Component 1.BQ"]], namedNodes[L"Component 72.A.3"], spTestGraph->GetEdgeMap() ) );

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

    cout << "Test 1: Correct Requirement Registration" << endl;

    shared_ptr<const Requirement> req01(new Requirement(L"TEST01", Requirement::APP, Requirement::ADHOC, L"TEST01", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req02(new Requirement(L"TEST02", Requirement::APP, Requirement::ADHOC, L"TEST02", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req03(new Requirement(L"TEST03", Requirement::APP, Requirement::ADHOC, L"TEST03", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req04(new Requirement(L"TEST04", Requirement::APP, Requirement::ADHOC, L"TEST04", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req05(new Requirement(L"TEST05", Requirement::APP, Requirement::ADHOC, L"TEST05", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req06(new Requirement(L"TEST06", Requirement::APP, Requirement::ADHOC, L"TEST06", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req07(new Requirement(L"TEST07", Requirement::APP, Requirement::ADHOC, L"TEST07", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req08(new Requirement(L"TEST08", Requirement::APP, Requirement::ADHOC, L"TEST08", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req09(new Requirement(L"TEST09", Requirement::APP, Requirement::ADHOC, L"TEST09", L"description", L"document", L"version", L"section") );
    shared_ptr<const Requirement> req10(new Requirement(L"TEST10", Requirement::APP, Requirement::ADHOC, L"TEST10", L"description", L"document", L"version", L"section") );

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
        cout << "Exception should not have been thrown." << endl;
        testStatus = false;
    }

    if (testStatus)
    {
        try
        {
            AxString temp = L"TEST01";
            if ( rReg.GetRequirement(temp) != req01 )
            {
                cout << "Wrong requirment returned." << endl;
                testStatus = false;
            }
        }
        catch ( RequirementRegistryException ex )
        {
            wcout << ex.widewhat() << endl;
            cout << "Exception should not have been thrown." << endl;
            testStatus = false;
        }
    }

    if (testStatus)
    {
        if (rReg.GetAllRequirements().size() != 10)
        {
            cout << "All requirements were not properly registered." << endl;
            testStatus = false;
        }
    }

    if (testStatus) {
        cout << "Test 1 passed" << endl;
    }
    else
    {
        cout << "Test 1 failed" << endl;
        assert( false );
    }

    /*Test 2:
     *
     * Ensure that re-registering throws an exception.
     *
     */

    cout << "Test 2: Duplicate Requirement Registration" << endl;

    try
    {
        rReg.Register( req01 );
        cout << "Exception should have been thrown when re-registering requirement." << endl;
        cout << "Test 2 failed" << endl;
        assert( false );
    }
    catch ( RequirementRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        cout << "Test 2 passed" << endl;
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
        cout << "Exception should have been thrown when searching for an unregistered requirement." << endl;
        cout << "Test 3 failed" << endl;
        assert( false );
    }
    catch ( RequirementRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        cout << "Test 3 passed" << endl;
    }

    //Test Registry

    TestRegistry& tReg = TestRegistry::GetInstance();

    /*Test 4:
     *
     * Register test and verify that their requirements are stored
     *
     */

    cout << "Test 4: Correct Test Registration" << endl;

    try
    {
        TestInfoRegistrar<Test1Stub> t1;
        TestInfoRegistrar<Test2Stub> t2;
    }
    catch ( TestRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        cout << "Exception should not have been thrown." << endl;
        testStatus = false;
    }

    if (testStatus)
    {
        try
        {
            if ( tReg.GetConstRequirementsForTest(L"Test1Stub")->size() != 3 )
            {
                cout << "Incorrect number of requirements returned." << endl;
                testStatus = false;
            }
        }
        catch ( TestRegistryException ex )
        {
            wcout << ex.widewhat() << endl;
            cout << "Exception should not have been thrown." << endl;
            testStatus = false;
        }
    }

    if (testStatus)
    {
        if (tReg.GetRequirementCoverage().size() != 6)
        {
            cout << "All requirements for test were not properly stored." << endl;
            testStatus = false;
        }
    }

    if (testStatus) {
        cout << "Test 4 passed" << endl;
    }
    else
    {
        cout << "Test 4 failed" << endl;
        assert( false );
    }

    /*Test 5:
     *
     * Ensure that re-registering throws an exception.
     *
     */

    cout << "Test 5: Duplicate Test Registration" << endl;

    try
    {
        TestInfoRegistrar<Test1Stub> tir;
        cout << "Exception should have been thrown when re-registering requirement." << endl;
        cout << "Test 5 failed" << endl;
        assert( false );
    }
    catch ( TestRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        cout << "Test 5 passed" << endl;
    }

    /*Test 6:
     *
     * Ensure that searching for requirements from an unregistered test throws
     * an exception.
     *
     */

    cout << "Test 6: Search for Unregistered Test" << endl;

    try
    {
        AxString temp = L"Unregistered";
        tReg.GetConstRequirementsForTest(L"Unregistered");
        cout << "Exception should have been thrown when searching for an unregistered test." << endl;
        cout << "Test 6 failed" << endl;
        assert( false );
    }
    catch ( TestRegistryException ex )
    {
        wcout << ex.widewhat() << endl;
        cout << "Test 6 passed" << endl;
    }

    /*
     * NOTE: The verifyTestResultCoverage requires a hierarchy of test results
     *       therefore it will be tested in CheckTestResultRequiremetns.
     *
     */
}

void CheckTestResultRequirements()
{

    bool testStatus = true;

    cout << endl << "Test the requirement functionality of test results." << endl << endl;

    RequirementRegistry& rReg = RequirementRegistry::GetInstance();
    TestRegistry& tReg = TestRegistry::GetInstance();

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

    /*Detail Level Test Results:
     *
     * A Detail Level Test Result may have:
     *  (a) Any requirement
     *  (b) Child results with any requirement
     *
     * When a child result is attached, no requirement checking or updating is
     * done.
     *
     * When setting the status of a requirement, an exception is thrown if it
     * is not covered by the Test Result.
     *
     */

    /*Test 1:
     *
     * Attempt to create the structure:
     *      1 (req01)
     *          1.1 (req02)
     *          1.2 (req01, req03)
     *              1.2.1 (req04)
     *          1.3 (req02, req03)
     *
     * This structure should be accepted - in fact, there should not be any
     * structure that is not accepted.
     */

    cout << "Test 1: Detail Level Test Result Construction" << endl;

    Requirement::RequirementMapSP rmDLTR100(new Requirement::RequirementMap());
    Requirement::RequirementMapSP rmDLTR110(new Requirement::RequirementMap());
    Requirement::RequirementMapSP rmDLTR120(new Requirement::RequirementMap());
    Requirement::RequirementMapSP rmDLTR121(new Requirement::RequirementMap());
    Requirement::RequirementMapSP rmDLTR130(new Requirement::RequirementMap());

    (*rmDLTR100)[req01->GetName()] = req01;

    (*rmDLTR110)[req02->GetName()] = req02;

    (*rmDLTR120)[req01->GetName()] = req01;
    (*rmDLTR120)[req03->GetName()] = req03;

    (*rmDLTR121)[req04->GetName()] = req04;

    (*rmDLTR130)[req02->GetName()] = req02;
    (*rmDLTR130)[req03->GetName()] = req02;

    shared_ptr<DetailLevelTestResult> dltr100(new DetailLevelTestResult(L"DLTR100", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, rmDLTR100));
    shared_ptr<DetailLevelTestResult> dltr110(new DetailLevelTestResult(L"DLTR110", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, rmDLTR110));
    shared_ptr<DetailLevelTestResult> dltr120(new DetailLevelTestResult(L"DLTR120", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, rmDLTR120));
    shared_ptr<DetailLevelTestResult> dltr121(new DetailLevelTestResult(L"DLTR121", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, rmDLTR121));
    shared_ptr<DetailLevelTestResult> dltr130(new DetailLevelTestResult(L"DLTR130", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, rmDLTR130));

    dltr100->AppendSubtestResult(dltr110);
    dltr100->AppendSubtestResult(dltr120);
    dltr100->AppendSubtestResult(dltr130);
    dltr120->AppendSubtestResult(dltr121);

    Requirement::RequirementMap passMap;
    Requirement::RequirementMap warnMap;
    Requirement::RequirementMap failMap;

    if (testStatus) {
        testStatus = false;
        passMap = dltr100->GetRequirements(TestResult::PASS);
        warnMap = dltr100->GetRequirements(TestResult::WARN);
        failMap = dltr100->GetRequirements(TestResult::FAIL);
        if (passMap.size() != 1) {
            cout << "DLTR100 has an incorrect number of passing results" << endl;
        }
        else if (warnMap.size() != 0) {
            cout << "DLTR100 has an incorrect number of warning results" << endl;
        }
        else if (failMap.size() != 0) {
            cout << "DLTR100 has an incorrect number of failing results" << endl;
        }
        else if (passMap.find(req01->GetName()) == passMap.end() ) {
            cout << "DLTR100 does not contain requirement TEST01 with status PASS" << endl;
        }
        else
        {
            testStatus = true;
        }
    }

    if (testStatus) {
        testStatus = false;
        passMap = dltr110->GetRequirements(TestResult::PASS);
        warnMap = dltr110->GetRequirements(TestResult::WARN);
        failMap = dltr110->GetRequirements(TestResult::FAIL);
        if (passMap.size() != 1) {
            cout << "DLTR110 has an incorrect number of passing results" << endl;
        }
        else if (warnMap.size() != 0) {
            cout << "DLTR110 has an incorrect number of warning results" << endl;
        }
        else if (failMap.size() != 0) {
            cout << "DLTR110 has an incorrect number of failing results" << endl;
        }
        else if (passMap.find(req02->GetName()) == passMap.end() ) {
            cout << "DLTR110 does not contain requirement TEST02 with status PASS" << endl;
        }
        else
        {
            testStatus = true;
        }
    }

    if (testStatus) {
        testStatus = false;
        passMap = dltr120->GetRequirements(TestResult::PASS);
        warnMap = dltr120->GetRequirements(TestResult::WARN);
        failMap = dltr120->GetRequirements(TestResult::FAIL);
        if (passMap.size() != 2) {
            cout << "DLTR120 has an incorrect number of passing results" << endl;
        }
        else if (warnMap.size() != 0) {
            cout << "DLTR120 has an incorrect number of warning results" << endl;
        }
        else if (failMap.size() != 0) {
            cout << "DLTR120 has an incorrect number of failing results" << endl;
        }
        else if (passMap.find(req01->GetName()) == passMap.end() ) {
            cout << "DLTR120 does not contain requirement TEST01 with status PASS" << endl;
        }
        else if (passMap.find(req03->GetName()) == passMap.end() ) {
            cout << "DLTR120 does not contain requirement TEST03 with status PASS" << endl;
        }
        else
        {
            testStatus = true;
        }
    }

    if (testStatus) {
        testStatus = false;
        passMap = dltr121->GetRequirements(TestResult::PASS);
        warnMap = dltr121->GetRequirements(TestResult::WARN);
        failMap = dltr121->GetRequirements(TestResult::FAIL);
        if (passMap.size() != 1) {
            cout << "DLTR121 has an incorrect number of passing results" << endl;
        }
        else if (warnMap.size() != 0) {
            cout << "DLTR121 has an incorrect number of warning results" << endl;
        }
        else if (failMap.size() != 0) {
            cout << "DLTR121 has an incorrect number of failing results" << endl;
        }
        else if (passMap.find(req04->GetName()) == passMap.end() ) {
            cout << "DLTR121 does not contain requirement TEST04 with status PASS" << endl;
        }
        else
        {
            testStatus = true;
        }
    }

    if (testStatus) {
        testStatus = false;
        passMap = dltr130->GetRequirements(TestResult::PASS);
        warnMap = dltr130->GetRequirements(TestResult::WARN);
        failMap = dltr130->GetRequirements(TestResult::FAIL);
        if (passMap.size() != 2) {
            cout << "DLTR130 has an incorrect number of passing results" << endl;
        }
        else if (warnMap.size() != 0) {
            cout << "DLTR130 has an incorrect number of warning results" << endl;
        }
        else if (failMap.size() != 0) {
            cout << "DLTR130 has an incorrect number of failing results" << endl;
        }
        else if (passMap.find(req02->GetName()) == passMap.end() ) {
            cout << "DLTR130 does not contain requirement TEST02 with status PASS" << endl;
        }
        else if (passMap.find(req03->GetName()) == passMap.end() ) {
            cout << "DLTR130 does not contain requirement TEST03 with status PASS" << endl;
        }
        else
        {
            testStatus = true;
        }
    }

    if (testStatus) {
        cout << "Test 1 passed" << endl;
    }
    else
    {
        cout << "Test 1 failed" << endl;
        assert( false );
    }

    /*Test 2:
     *
     * Make sure that:
     *  (a) No exception is thrown when setting the status of a requirement.
     *  (b) That a requirements status is actually changed.
     *  (c) An exception is thrown if the requirement is not in the test result.
     *
     */

    cout << "Test 2: Detail Level Set Requirement Status" << endl;

    testStatus = true;

    if (testStatus)
    {
        try
        {
            dltr120->SetRequirementStatus(TestResult::WARN, req01);
            dltr120->SetRequirementStatus(TestResult::FAIL, req03);
        }
        catch ( RequirementMismatchException ex )
        {
            wcout << ex.widewhat() << endl;
            cout << "Exception was incorrectly thrown by SetRequirementStatus" << endl;
            testStatus = false;
        }
    }

    if (testStatus) {
        testStatus = false;
        passMap = dltr120->GetRequirements(TestResult::PASS);
        warnMap = dltr120->GetRequirements(TestResult::WARN);
        failMap = dltr120->GetRequirements(TestResult::FAIL);
        if (passMap.size() != 0) {
            cout << "DLTR120 has an incorrect number of passing results" << endl;
        }
        else if (warnMap.size() != 1) {
            cout << "DLTR120 has an incorrect number of warning results" << endl;
        }
        else if (failMap.size() != 1) {
            cout << "DLTR120 has an incorrect number of failing results" << endl;
        }
        else if (warnMap.find(req01->GetName()) == passMap.end() ) {
            cout << "DLTR120 does not contain requirement TEST01 with status WARN" << endl;
        }
        else if (failMap.find(req03->GetName()) == passMap.end() ) {
            cout << "DLTR120 does not contain requirement TEST03 with status FAIL" << endl;
        }
        else
        {
            testStatus = true;
        }
    }

    if (testStatus)
    {
        try
        {
            dltr100->SetRequirementStatus(TestResult::WARN, req10);
            cout << "Exception was not thrown by SetRequirementStatus" << endl;
            testStatus = false;
        }
        catch ( RequirementMismatchException ex )
        {
            cout << "Exception caught" << endl;
        }
    }

    if (testStatus) {
        cout << "Test 2 passed" << endl;
    }
    else
    {
        cout << "Test 2 failed" << endl;
        assert( false );
    }

    /*Test 3:
     *
     * Make sure that requirements on a parent arn't automatically updated by
     * appending test result with the same child requirement with a different
     * status.
     *
     */

    cout << "Test 3: Detail Level Append doesn't modify parent requirements" << endl;

    Requirement::RequirementMapSP rmDLTR122(new Requirement::RequirementMap());

    (*rmDLTR122)[req01->GetName()] = req01;

    shared_ptr<DetailLevelTestResult> dltr122(new DetailLevelTestResult(L"DLTR122", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, rmDLTR122));

    dltr120->AppendSubtestResult(dltr122);

    testStatus = false;
    passMap = dltr120->GetRequirements(TestResult::PASS);
    warnMap = dltr120->GetRequirements(TestResult::WARN);
    failMap = dltr120->GetRequirements(TestResult::FAIL);
    if (passMap.size() != 0) {
        cout << "DLTR120 has an incorrect number of passing results" << endl;
    }
    else if (warnMap.size() != 1) {
        cout << "DLTR120 has an incorrect number of warning results" << endl;
    }
    else if (failMap.size() != 1) {
        cout << "DLTR120 has an incorrect number of failing results" << endl;
    }
    else if (warnMap.find(req01->GetName()) == passMap.end() ) {
        cout << "DLTR120 does not contain requirement TEST01 with status WARN" << endl;
    }
    else if (failMap.find(req03->GetName()) == passMap.end() ) {
        cout << "DLTR120 does not contain requirement TEST03 with status FAIL" << endl;
    }
    else
    {
        testStatus = true;
    }

    if (testStatus) {
        cout << "Test 3 passed" << endl;
    }
    else
    {
        cout << "Test 3 failed" << endl;
        assert( false );
    }

    /*Test Level Test Result:
     *
     * A Test Level Test Result must have all requirements that its associated
     * test has.
     *
     * A Test Level Test Result may have child results with requirments that are
     * covered by it.
     *
     * When a child result is attached, it is verified that all requirments on
     * the child and all requirements on its descendants are covered by the
     * test result.  No requirement updating is done.
     *
     * When setting the status of a requirement, an exception is thrown if it
     * is not covered by the Test Result.
     *
     */

    /*Test 4:
     *
     * Verify that a Test Level Test Result can only be created if it matches
     * the exact set of requirements as its associated test.
     */

    cout << "Test 4: Test Level Test Result Construction" << endl;

    testStatus = true;
    shared_ptr<Test> test1(new Test1Stub(wcout));
    shared_ptr<Test> test2(new Test2Stub(wcout));

    try
    {
        Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(test2->GetCoveredRequirements()));
        shared_ptr<TestLevelTestResult> tltr1(new TestLevelTestResult(L"TLTR1", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, test1, spMyReqs));
        cout << "TLTR1 incorrectly created" << endl;
        testStatus = false;
    }
    catch ( RequirementMismatchException ex )
    {
        cout << "Exception caught" << endl;
    }

    if (testStatus)
    {
        try
        {
            Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(test1->GetCoveredRequirements()));
            shared_ptr<TestLevelTestResult> tltr2(new TestLevelTestResult(L"TLTR2", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, test2, spMyReqs));
            cout << "TLTR2 incorrectly created" << endl;
            testStatus = false;
        }
        catch ( RequirementMismatchException ex )
        {
            cout << "Exception caught" << endl;
        }
    }

    if (testStatus)
    {
        try
        {
            Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap());
            (*spMyReqs)[L"TEST08"]= rReg.GetRequirement(L"TEST08");
            (*spMyReqs)[L"TEST09"]= rReg.GetRequirement(L"TEST09");
            (*spMyReqs)[L"TEST10"]= rReg.GetRequirement(L"TEST10");
            shared_ptr<TestLevelTestResult> tltr1(new TestLevelTestResult(L"TLTR1", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, test1, spMyReqs));
            cout << "TLTR1 incorrectly created" << endl;
            testStatus = false;
        }
        catch ( RequirementMismatchException ex )
        {
            cout << "Exception caught" << endl;
        }
    }

    shared_ptr<TestLevelTestResult> tltr1;
    shared_ptr<TestLevelTestResult> tltr2;
    try
    {
        Requirement::RequirementMapSP spMyReqs1(new Requirement::RequirementMap(test1->GetCoveredRequirements()));
        shared_ptr<TestLevelTestResult> _tltr1(new TestLevelTestResult(L"TLTR1", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, test1, spMyReqs1));
        tltr1 = _tltr1;
        Requirement::RequirementMapSP spMyReqs2(new Requirement::RequirementMap(test2->GetCoveredRequirements()));
        shared_ptr<TestLevelTestResult> _tltr2(new TestLevelTestResult(L"TLTR2", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, test2, spMyReqs2));
        tltr2 = _tltr2;
    }
    catch ( RequirementMismatchException ex )
    {
        wcout << ex.widewhat() << endl;
        cout << "Exception incorrectly thrown when createing a Test Level Test Result" << endl;
        testStatus = false;
    }

    if (testStatus) {
        cout << "Test 4 passed" << endl;
    }
    else
    {
        cout << "Test 4 failed" << endl;
        assert( false );
    }

    /*Test 5:
     *
     * Verify that appending a subtest result throws an exception if it contains
     * requirements not contained in the parent.
     *
     */

    cout << "Test 5: Incorrect Append to Test Level Test Result" << endl;

    try
    {
        tltr1->AppendSubtestResult(dltr100);
        cout << "DLTR100 incorrectly appended to TLTR1" << endl;
        cout << "Test 5 failed" << endl;
        assert( false );
    }
    catch ( RequirementMismatchException ex )
    {
        cout << "Exception caught" << endl;
        cout << "Test 5 passed" << endl;
    }

    /*Test 6:
     *
     * Verify that no exception is thrown if a valid subtest result is appended
     * and that appending the subtest result does not cause the status of
     * requirements to change.
     *
     */

    cout << "Test 6: Incorrect Append to Test Level Test Result" << endl;

    testStatus = true;

    try
    {
        tltr2->AppendSubtestResult(dltr100);
    }
    catch ( RequirementMismatchException ex )
    {
        wcout << ex.widewhat() << endl;
        cout << "Exception incorrectly thrown when appending DLTR100 to TLTR2" << endl;
        testStatus = false;
    }

    if (testStatus)
    {
        testStatus = false;
        passMap = tltr2->GetRequirements(TestResult::PASS);
        warnMap = tltr2->GetRequirements(TestResult::WARN);
        failMap = tltr2->GetRequirements(TestResult::FAIL);
        if (passMap.size() != 4) {
            cout << "TLTR2 has an incorrect number of passing results" << endl;
        }
        else if (warnMap.size() != 0) {
            cout << "TLTR2 has an incorrect number of warning results" << endl;
        }
        else if (failMap.size() != 0) {
            cout << "TLTR2 has an incorrect number of failing results" << endl;
        }
        else if (passMap.find(req01->GetName()) == passMap.end() ) {
            cout << "TLTR2 does not contain requirement TEST01 with status PASS" << endl;
        }
        else if (passMap.find(req02->GetName()) == passMap.end() ) {
            cout << "TLTR2 does not contain requirement TEST02 with status PASS" << endl;
        }
        else if (passMap.find(req03->GetName()) == passMap.end() ) {
            cout << "TLTR2 does not contain requirement TEST03 with status PASS" << endl;
        }
        else if (passMap.find(req04->GetName()) == passMap.end() ) {
            cout << "TLTR2 does not contain requirement TEST04 with status PASS" << endl;
        }
        else
        {
            testStatus = true;
        }
    }

    if (testStatus) {
        cout << "Test 6 passed" << endl;
    }
    else
    {
        cout << "Test 6 failed" << endl;
        assert( false );
    }

    /*Test 7:
     *
     * Make sure that:
     *  (a) No exception is thrown when setting the status of a requirement.
     *  (b) That a requirements status is actually changed.
     *  (c) An exception is thrown if the requirement is not in the test result.
     *
     */

    cout << "Test 7: Change Test Level Requirement Status" << endl;

    testStatus = true;

    try
    {
        tltr2->SetRequirementStatus(TestResult::WARN, req10);
        cout << "Status of requirement TEST10 incorrectly changed on TLTR2" << endl;
        testStatus = false;
    }
    catch ( RequirementMismatchException ex )
    {
        cout << "Exception caught" << endl;
    }

    if (testStatus)
    {
        try
        {
            tltr2->SetRequirementStatus(TestResult::FAIL, req01);
        }
        catch ( RequirementMismatchException ex )
        {
            wcout << ex.widewhat() << endl;
            cout << "Exception incorrectly thrown when changing status of requirement TEST01 in TLTR2" << endl;
            testStatus = false;
        }
    }

    if (testStatus)
    {
        testStatus = false;
        passMap = tltr2->GetRequirements(TestResult::PASS);
        warnMap = tltr2->GetRequirements(TestResult::WARN);
        failMap = tltr2->GetRequirements(TestResult::FAIL);
        if (passMap.size() != 3) {
            cout << "TLTR2 has an incorrect number of passing results" << endl;
        }
        else if (warnMap.size() != 0) {
            cout << "TLTR2 has an incorrect number of warning results" << endl;
        }
        else if (failMap.size() != 1) {
            cout << "TLTR2 has an incorrect number of failing results" << endl;
        }
        else
        {
            testStatus = true;
        }
    }

    if (testStatus) {
        cout << "Test 7 passed" << endl;
    }
    else
    {
        cout << "Test 7 failed" << endl;
        assert( false );
    }

    /*Test Phase Level Test Result:
     *
     * A Test Phase Level Test Result is automatically updated to include the
     * requirments covered by all of its subtest results when a new subtest
     * result is attached.
     *
     * A Test Phase Level Test Result can have its requirment status refreshed
     * if changes are made to subtest results after they have been appended.
     *
     */

    /*Test 8:
     *
     * Ensure that requirements are automatically added when subtest results are
     * appended.
     *
     */

    cout << "Test 8: Append to Test Phase Level Test Result" << endl;

    shared_ptr<TestPhaseLevelTestResult> tpltr1(new TestPhaseLevelTestResult(L"TPLTR1", L"Desc", L"Explain", L"DocRef", TestResult::FAIL));

    tpltr1->AppendSubtestResult(tltr2);

    tltr1->SetRequirementStatus(TestResult::FAIL, req08);
    tltr1->SetRequirementStatus(TestResult::WARN, req05);

    tpltr1->AppendSubtestResult(tltr1);

    testStatus = false;
    passMap = tpltr1->GetRequirements(TestResult::PASS);
    warnMap = tpltr1->GetRequirements(TestResult::WARN);
    failMap = tpltr1->GetRequirements(TestResult::FAIL);
    if (passMap.size() != 3) {
        cout << "TPLTR1 has an incorrect number of passing results" << endl;
    }
    else if (warnMap.size() != 1) {
        cout << "TPLTR1 has an incorrect number of warning results" << endl;
    }
    else if (failMap.size() != 2) {
        cout << "TPLTR1 has an incorrect number of failing results" << endl;
    }
    else if (failMap.find(req01->GetName()) == failMap.end() ) {
        cout << "TPLTR1 does not contain requirement TEST01 with status FAIL" << endl;
    }
    else if (warnMap.find(req05->GetName()) == warnMap.end() ) {
        cout << "TLTR1 does not contain requirement TEST05 with status WARN" << endl;
    }
    else
    {
        testStatus = true;
    }

    if (testStatus) {
        cout << "Test 8 passed" << endl;
    }
    else
    {
        cout << "Test 8 failed" << endl;
        assert( false );
    }

    /*Test 9:
     *
     * Ensure that the UpdateRequirementStatus function properly updates
     * requirements from subtest results.
     *
     */

    cout << "Test 9: Update Test Phase Level Test Result Requirement Status" << endl;

    tltr1->SetRequirementStatus(TestResult::WARN, req01);
    tltr2->SetRequirementStatus(TestResult::WARN, req04);
    tpltr1->UpdateRequirementStatus();

    testStatus = false;
    passMap = tpltr1->GetRequirements(TestResult::PASS);
    warnMap = tpltr1->GetRequirements(TestResult::WARN);
    failMap = tpltr1->GetRequirements(TestResult::FAIL);
    if (passMap.size() != 2) {
        cout << "TPLTR1 has an incorrect number of passing results" << endl;
    }
    else if (warnMap.size() != 2) {
        cout << "TPLTR1 has an incorrect number of warning results" << endl;
    }
    else if (failMap.size() != 2) {
        cout << "TPLTR1 has an incorrect number of failing results" << endl;
    }
    else if (failMap.find(req01->GetName()) == failMap.end() ) {
        cout << "TPLTR1 does not contain requirement TEST01 with status FAIL" << endl;
    }
    else if (warnMap.find(req04->GetName()) == warnMap.end() ) {
        cout << "TLTR1 does not contain requirement TEST04 with status WARN" << endl;
    }
    else
    {
        testStatus = true;
    }

    if (testStatus) {
        cout << "Test 9 passed" << endl;
    }
    else
    {
        cout << "Test 9 failed" << endl;
        assert( false );
    }

    /*Top Level Test Result:
     *
     * A Top Level Test Result is automatically updated to include the
     * requirments covered by all of its subtest results when a new subtest
     * result is attached.
     *
     * A Top Level Test Result can have its requirment status refreshed if
     * changes are made to subtest results after they have been appended.
     *
     */

    /*Test 10:
     *
     * Ensure that requirements are automatically added when subtest results are
     * appended.
     *
     */

    cout << "Test 10: Append to Top Level Test Result" << endl;

    Requirement::RequirementMapSP spMyReqs1(new Requirement::RequirementMap(test1->GetCoveredRequirements()));
    shared_ptr<TestLevelTestResult> tltr3(new TestLevelTestResult(L"TLTR1", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, test1, spMyReqs1));
    Requirement::RequirementMapSP spMyReqs2(new Requirement::RequirementMap(test2->GetCoveredRequirements()));
    shared_ptr<TestLevelTestResult> tltr4(new TestLevelTestResult(L"TLTR2", L"Desc", L"Explain", L"DocRef", TestResult::FAIL, test2, spMyReqs2));

    shared_ptr<TestPhaseLevelTestResult> tpltr2(new TestPhaseLevelTestResult(L"TPLTR1", L"Desc", L"Explain", L"DocRef", TestResult::FAIL));

    tpltr2->AppendSubtestResult(tltr3);
    tpltr2->AppendSubtestResult(tltr4);

    shared_ptr<TopLevelTestResult> top(new TopLevelTestResult(L"TOP", L"Desc", L"Explain", L"DocRef", TestResult::FAIL));


    top->AppendSubtestResult(tpltr2);
    top->AppendSubtestResult(tpltr1);

    testStatus = false;
    passMap = top->GetRequirements(TestResult::PASS);
    warnMap = top->GetRequirements(TestResult::WARN);
    failMap = top->GetRequirements(TestResult::FAIL);
    if (passMap.size() != 2) {
        cout << "TOP has an incorrect number of passing results" << endl;
    }
    else if (warnMap.size() != 2) {
        cout << "TOP has an incorrect number of warning results" << endl;
    }
    else if (failMap.size() != 2) {
        cout << "TOP has an incorrect number of failing results" << endl;
    }
    else if (failMap.find(req01->GetName()) == failMap.end() ) {
        cout << "TOP does not contain requirement TEST01 with status FAIL" << endl;
    }
    else if (warnMap.find(req04->GetName()) == warnMap.end() ) {
        cout << "TOP does not contain requirement TEST04 with status WARN" << endl;
    }
    else
    {
        testStatus = true;
    }

    if (testStatus) {
        cout << "Test 10 passed" << endl;
    }
    else
    {
        cout << "Test 10 failed" << endl;
        assert( false );
    }

    /*Test 11:
     *
     * Ensure that the UpdateRequirementStatus function properly updates
     * requirements from subtest results.
     *
     */

    cout << "Test 11: Update Top Level Test Result Requirement Status" << endl;

    tltr2->SetRequirementStatus(TestResult::PASS, req01);
    tltr4->SetRequirementStatus(TestResult::WARN, req02);
    tpltr1->UpdateRequirementStatus();
    tpltr2->UpdateRequirementStatus();
    top->UpdateRequirementStatus();

    testStatus = false;
    passMap = top->GetRequirements(TestResult::PASS);
    warnMap = top->GetRequirements(TestResult::WARN);
    failMap = top->GetRequirements(TestResult::FAIL);
    if (passMap.size() != 1) {
        cout << "TOP has an incorrect number of passing results" << endl;
    }
    else if (warnMap.size() != 3) {
        cout << "TOP has an incorrect number of warning results" << endl;
    }
    else if (failMap.size() != 2) {
        cout << "TOP has an incorrect number of failing results" << endl;
    }
    else if (warnMap.find(req01->GetName()) == passMap.end() ) {
        cout << "TOP does not contain requirement TEST01 with status PASS" << endl;
    }
    else if (warnMap.find(req02->GetName()) == warnMap.end() ) {
        cout << "TOP does not contain requirement TEST02 with status WARN" << endl;
    }
    else
    {
        testStatus = true;
    }

    if (testStatus) {
        cout << "Test 11 passed" << endl;
    }
    else
    {
        cout << "Test 11 failed" << endl;
        assert( false );
    }

    //Test Registry

    /*Test 12:
     *
     * Ensure that the VerifyTestResultCoverage function in the Test Registry
     * works correctly
     *
     */

    cout << "Test 12: Test Registry VerifyTestResultCoverage" << endl;

    shared_ptr<TestPhaseLevelTestResult> tpltr3(new TestPhaseLevelTestResult(L"TPLTR1", L"Desc", L"Explain", L"DocRef", TestResult::FAIL));

    tpltr3->AppendSubtestResult(tltr3);

    shared_ptr<TopLevelTestResult> topFail(new TopLevelTestResult(L"TOP", L"Desc", L"Explain", L"DocRef", TestResult::FAIL));

    topFail->AppendSubtestResult(tpltr3);

    if ( !tReg.VerifyTestResultCoverage(top) ) {
        cout << "Registry did not correctly verify complete coverage." << endl;
        cout << "Test 12 failed" << endl;
        assert( false );
    } else if ( tReg.VerifyTestResultCoverage(topFail) ) {
        cout << "Registry incorrectly verified complete coverage." << endl;
        cout << "Test 12 failed" << endl;
        assert( false );
    } else {
        cout << "Test 12 passed" << endl;
    }

}


} // end of namespace

//used to test the basic data structures of the software architecture
int main( int argc, char** argv )
{

  using namespace aafanalyzer;

  AxInit initObj;
  AxCmdLineArgs args( argc, argv );
  pair<bool,const char*> inputArg = args.get( argc-1, 1 );
  wostringstream ssOutFile;
  ssOutFile << inputArg.second;
  AxString filename( ssOutFile.str().c_str() );

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
  shared_ptr<TestVisitor> spTestVisitor(new TestVisitor());
  shared_ptr<Node> fooOne(new Node());
  shared_ptr<Node> fooTwo(new Node());
  spEdgeMap->AddEdge(shared_ptr<Edge>(new Edge(childTwo, fooOne)));
  spEdgeMap->AddEdge(shared_ptr<Edge>(new Edge(childTwo, fooTwo)));

  DepthFirstTraversal dfs(spEdgeMap, parentOne);
  dfs.TraverseDown(spTestVisitor, parentOne);
  cout << "********************" << endl;
  dfs.TraverseUp(spTestVisitor, fooOne);

  //test the Acyclic Visitor
  TestInfoRegistrar<AcyclicAnalysis> regAA;
  cout << "***CYCLE TESTER (no cycles)***" << endl;
  shared_ptr<AcyclicVisitor> spAcyclicVisitorA(new AcyclicVisitor(wcout));
  dfs.TraverseDown(spAcyclicVisitorA, parentOne);
  shared_ptr<const TestResult> resultA( spAcyclicVisitorA->GetTestResult() );
  assert( resultA->GetResult() == TestResult::PASS );

  cout << "***CYCLE TESTER (w/ cycle present)***" << endl;
  shared_ptr<AcyclicVisitor> spAcyclicVisitorB(new AcyclicVisitor(wcout));
  spEdgeMap->AddEdge(shared_ptr<Edge>(new Edge(fooTwo, parentTwo)));//adds a cycle
  dfs.TraverseDown(spAcyclicVisitorB, parentOne);
  shared_ptr<const TestResult> resultB( spAcyclicVisitorB->GetTestResult() );
  assert( resultB->GetResult() == TestResult::FAIL );

  CheckEdges( filename );

  cout << "Completed Basic Testing Successfully!" << endl << endl;

  CheckRegistries();
  CheckTestResultRequirements();

  return 0;
}
