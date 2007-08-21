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

//Base Test files
#include "DumpVisitor.h"

// AAF Analyzer Base file
#include <AAFObjNode.h>

//Analyzer Base files
#include <Node.h>
#include <Edge.h>

// Ax files
#include <AxSmartPointer.h>
#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxUtil.h>

// AAF files
#include <AAFExtEnum.h>

//STL files
#include <iostream>
#include <sstream>

namespace {

using namespace aafanalyzer;
using namespace std;



pair<bool,wstring> UsageCodeToString( const aafUID_t& id )
{
#define COMPARE(WHAT,X) WHAT ( id == X ) return make_pair(true, L#X)

  COMPARE(     if, kAAFUsage_SubClip);
  COMPARE(else if, kAAFUsage_AdjustedClip);
  COMPARE(else if, kAAFUsage_TopLevel);
  COMPARE(else if, kAAFUsage_LowerLevel);
  COMPARE(else if, kAAFUsage_Template);
  else return make_pair( false, wstring() );

#undef COMPARE
}

pair<bool,wstring> GetAAFInfo( AAFObjNode& aafNode )
{
  IAAFObjectSP spObj = aafNode.GetAAFObject();
  AxObject axObj(aafNode.GetAAFObject());
  wstring unnamed( L"<unnamed>" );
  
  IAAFMob2SP spMob;
  if ( AxIsA<IAAFMob2>( axObj, spMob ) )
  {
    AxMob axMob(spMob);
    AxString mobName = axMob.GetName( unnamed );
    
        aafUID_t usageCode = axMob.GetUsageCode( AxConstants::NULL_UID );
    
    wstringstream ss;
    ss << mobName;

    pair<bool,wstring> usage = UsageCodeToString(usageCode);
    if ( usage.first )
    {
      ss << L", " << usage.second;
    }

    return make_pair(true, ss.str());
  }
  
  IAAFTimelineMobSlotSP spMobSlot;
  if ( AxIsA<IAAFTimelineMobSlot>( axObj, spMobSlot ) )
  {
    return make_pair(true, AxTimelineMobSlot(spMobSlot).GetName( unnamed ) );
  }
  
  return make_pair(false,wstring());
}

wstring PreNodeInfo( Node& node )
{
  wstringstream ss;
  ss << node.GetName();

  try
  {
    pair<bool,wstring> info = GetAAFInfo( dynamic_cast<AAFObjNode&>(node) );
    if ( info.first )
    {
      ss << L", " << info.second;
    }
  }
  catch ( bad_cast& )
  {
    // intentional noop
  }

  return ss.str();
}

wstring PostNodeInfo( Node& node )
{
  return PreNodeInfo(node);
}

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

using namespace std;

DumpVisitor::DumpVisitor()
  : Visitor( Visitor::FOLLOW_ALL )
{}

DumpVisitor::DumpVisitor( Visitor::Follow_e follow )
  : Visitor( follow )
{}

DumpVisitor::~DumpVisitor()
{}

bool DumpVisitor::PreOrderVisit(Node& node)
{
  if ( GetLastEdgeKind() == Edge::EDGE_KIND_CONTAINMENT )
  {
    wcout << "C";
  }
  else
  {
    wcout << "R";
  }
  
  wcout << LevelToIndent(GetLevel()) << node.GetLID() << " " << PreNodeInfo(node) << endl;
  return true;
}

bool DumpVisitor::PostOrderVisit(Node& node)
{
  return true;
}

basic_string<wchar_t> DumpVisitor::LevelToIndent(size_t l)
{
  basic_string<wchar_t> indent;

  for (; l > 0 ; l-- ) 
  {
    indent += L"  ";
  }
  return indent;
}

} // end of namespace diskstream
