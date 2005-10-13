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
#include <AxMob.h>
#include <AxUtil.h>

//Boost files
#include <boost/shared_ptr.hpp>

using namespace boost;

class TestFileBuilder
{
    public:
        TestFileBuilder( const char* outFile );
        ~TestFileBuilder();

        shared_ptr<AxCompositionMob> AddTopLevel( const AxString& name );
        shared_ptr<AxCompositionMob> AddLowerLevel( const AxString& name );
        shared_ptr<AxCompositionMob> AddSubClip( const AxString& name );
        shared_ptr<AxCompositionMob> AddAdjustedClip( const AxString& name );

        shared_ptr<AxMasterMob> AddTemplateClip( const AxString& name );
        shared_ptr<AxMasterMob> AddClip( const AxString& name );

        shared_ptr<AxSourceMob> AddFileSource( const AxString& name );
        shared_ptr<AxSourceMob> AddRecordingSource( const AxString& name );
        shared_ptr<AxSourceMob> AddImportSource( const AxString& name );
        shared_ptr<AxSourceMob> AddTapeSource( const AxString& name );
        shared_ptr<AxSourceMob> AddFilmSource( const AxString& name );

        void Attach( AxMob& parent, AxMob& child );
        void AttachEOC( AxMob& parent );
        void AttachOOF( AxMob& parent );

    private:
    
        AxFile _axFile;
        aafUInt32 _mobCount;
        const aafMobID_t GenerateMobId();

};

#endif /*TESTFILE_H_*/
