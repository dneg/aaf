// Copyright 2003 BBC Technology

// $Id$
// $Name$

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
