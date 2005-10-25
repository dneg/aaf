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

#ifndef TESTFILE_H_
#define TESTFILE_H_

//Ax files
#include <AxFile.h>
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

class AxMob;

namespace aafanalyzer {

using namespace boost;

class TestFileBuilder
{
    public:
        TestFileBuilder( const char* outFile );
        ~TestFileBuilder();

        shared_ptr<AxMob> AddTopLevel( const AxString& name, bool isNamed );
        shared_ptr<AxMob> AddLowerLevel( const AxString& name, bool isNamed );
        shared_ptr<AxMob> AddSubClip( const AxString& name, bool isNamed );
        shared_ptr<AxMob> AddAdjustedClip( const AxString& name, bool isNamed );

        shared_ptr<AxMob> AddTemplateClip( const AxString& name, bool isNamed );
        shared_ptr<AxMob> AddClip( const AxString& name, bool isNamed );

        shared_ptr<AxMob> AddFileSource( const AxString& name, bool isNamed );
        shared_ptr<AxMob> AddRecordingSource( const AxString& name, bool isNamed );
        shared_ptr<AxMob> AddImportSource( const AxString& name, bool isNamed );
        shared_ptr<AxMob> AddTapeSource( const AxString& name, bool isNamed );
        shared_ptr<AxMob> AddFilmSource( const AxString& name, bool isNamed );

        void Attach( AxMob& parent, AxMob& child );
        void AttachEOC( AxMob& parent );
        void AttachOOF( AxMob& parent );
        
        void AddTimeCode( AxMob& parent, int physicalTrackNum, bool hasTrackNum );
        void AddEdgeCode( AxMob& parent, int physicalTrackNum, bool hasTrackNum );

    private:
    
        AxFile _axFile;
        aafUInt32 _mobCount;
        const aafMobID_t GenerateMobId();

};

} // end of namespace diskstream

#endif /*TESTFILE_H_*/
