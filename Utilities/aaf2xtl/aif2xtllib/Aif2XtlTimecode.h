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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Siemens SBS Media
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef _Aif2XtlTimecode_h_
#define _Aif2XtlTimecode_h_

#include <AAFTypes.h>
#include <string>

class Aif2XtlTimecode {
public:
	Aif2XtlTimecode( aafLength_t sampleCount,
			         const aafRational_t& rate );
	~Aif2XtlTimecode();


	// Xtl timecode is of the form HH:MM::SS.fff
	// where ff is a fraction of one second.
	std::string GetXtlTimecodeString();

private:
	aafLength_t _sampleCount;
	aafInt32 _rateNum;
	aafInt32 _rateDen;
};

#endif
