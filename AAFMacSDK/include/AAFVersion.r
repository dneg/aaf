/***********************************************************************
*
* The contents of this file are subject to the AAF SDK Public
* Source License Agreement (the "License"); You may not use this file
* except in compliance with the License.  The License is available in
* AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
* Association or its successor.
*
* Software distributed under the License is distributed on an "AS IS"
* basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
* the License for the specific language governing rights and limitations
* under the License.
*
* The Original Code of this file is Copyright 1998-2001, Licensor of the
* AAF Association.
*
* The Initial Developer of the Original Code of this file and the
* Licensor of the AAF Association is Avid Technology.
* All rights reserved.
*
************************************************************************/

/*
 	File:		AAFVersion.r
*/

#ifndef __AAFVERSION_R__
#define __AAFVERSION_R__

#include "MacTypes.r"

#ifndef MAJOR_VERSION
#define MAJOR_VERSION 0x01
#endif

#ifndef MINOR_VERSION
#define MINOR_VERSION 0x00
#endif

#ifndef RELEASE_STAGE
#define RELEASE_STAGE release
#endif

#ifndef NON_FINAL_RELEASE
#define NON_FINAL_RELEASE 0x01
#endif

#ifndef REGION_CODE
#define REGION_CODE 0
#endif

#ifndef SHORT_VERSION_STRING
#define SHORT_VERSION_STRING "1.0rc1"
#endif

#ifndef LONG_VERSION_STRING
#define LONG_VERSION_STRING " Copyright © 1998-2001 Licensor of the AAF Association, www.AAFassociation.org"
#endif

#ifndef AAF_SDK_RELEASE
#define AAF_SDK_RELEASE "AAF SDK Release Candidate 1"
#endif

resource 'vers' (1) {
	MAJOR_VERSION,
	MINOR_VERSION,
	RELEASE_STAGE,
	NON_FINAL_RELEASE,
	REGION_CODE,
	SHORT_VERSION_STRING,
	SHORT_VERSION_STRING LONG_VERSION_STRING
};

resource 'vers' (2) {
	MAJOR_VERSION,
	MINOR_VERSION,
	RELEASE_STAGE,
	NON_FINAL_RELEASE,
	REGION_CODE,
	SHORT_VERSION_STRING,
	AAF_SDK_RELEASE
};


#endif /* __AAFVERSION_R__ */
