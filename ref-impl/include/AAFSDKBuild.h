//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
//=---------------------------------------------------------------------=


#ifndef _AAF_BUILD_
#define _AAF_BUILD_

#define AAF_PRODUCT_NAME "AAF SDK"

//   This file is intended to the ONE central place that the SDK Version number is set
//   The MAJOR and MINOR version numbers specified below are used in
//   ref-impl/src/impl/AAFUtils.cpp to set the AAFReferenceImplementationVersion
//   which is in turn returned by the API if you call GetProductVersion
//   The same numbers are used to label the binary DLL in platform specific ways
//   so that you can work out what version of the DLL you have from the 'outside'.
//
//   The only  slight problem is that the (classic) Macintosh resource compiler 
//   for AAFMacSDK/include/AAFVersion.r can't handle macro parameters
//   or the stringify # stuff below
//
//   If you want to roll a new version, edit the numbers below then call the perl script
//   ../../build/newbuild.pl which will bump the build number, update the Mac resource and
//   set a CVS tag for the new build number


#define AAF_MAJOR_VERSION 1
#define AAF_MINOR_VERSION 0
#define AAF_MAINT_RELEASE 0

#define AAF_RELEASE_STAGE kAAFVersionBeta
 
// these next two lines are monitored/incremented by newbuild.pl

#define AAF_BUILD_NUMBER 502
#define AAF_CURRENT_YEAR "2001"

//   The lines below shouldn't normally need to be modified
//
//   The macro AAF_FILE_VERSION is used in AAFWinSDK/include/AAFVersion.rc to set the
//   file version properies of the DLL
//
//   The macro AAF_SDK_RELEASE is used by Irix and Linux in ref-impl/src/impl/AAFUtils.cpp
//   to create a global ident string that can be found with the old SCCS 'what' command or
//   strings libcomapi.so | grep "@(#)" 


#define AAF_COPYRIGHT_STRING "Copyright © 1998-" AAF_CURRENT_YEAR

#define stringify(num) # num

#define AAF_SDK_RELEASEZZ(majorNUM, minorNUM, maintNUM, buildNUM) AAF_PRODUCT_NAME " " stringify(majorNUM) "." stringify(minorNUM) "." stringify(maintNUM) " build " stringify(buildNUM)

#define AAF_SDK_VERSIONZZ(majorNUM, minorNUM, maintNUM, buildNUM) stringify(majorNUM) "." stringify(minorNUM) "." stringify(maintNUM) " build " stringify(buildNUM)

#define AAF_FILE_VERSIONZZ(majorNUM, minorNUM, maintNUM, buildNUM) stringify(majorNUM) "." stringify(minorNUM) "." stringify(maintNUM) "." stringify(buildNUM)

#define AAF_SDK_RELEASE AAF_SDK_RELEASEZZ( AAF_MAJOR_VERSION, AAF_MINOR_VERSION, AAF_MAINT_RELEASE, AAF_BUILD_NUMBER)

#define AAF_FILE_VERSION AAF_FILE_VERSIONZZ( AAF_MAJOR_VERSION, AAF_MINOR_VERSION, AAF_MAINT_RELEASE, AAF_BUILD_NUMBER)

#define AAF_SDK_VERSION AAF_SDK_VERSIONZZ( AAF_MAJOR_VERSION, AAF_MINOR_VERSION, AAF_MAINT_RELEASE, AAF_BUILD_NUMBER)

//  Mac rez stuff

#define AAF_LONG_VERSION_STRING " " AAF_COPYRIGHT_STRING " Licensor of the AAF Association, www.AAFassociation.org"

//  this next line will be updated to synchronise it with the numbers above by newbuild.pl

#define AAF_SHORT_VERSION_STRING "1.0.0 build 502"

#endif
