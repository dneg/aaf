// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <Aif2XtlTimecode.h>

#include <sstream>

#include <assert.h>
#include <stdio.h>

Aif2XtlTimecode::Aif2XtlTimecode( aafLength_t sampleCount,
				  const aafRational_t& rate )
: _sampleCount(sampleCount),
  _rateNum(rate.numerator),
  _rateDen(rate.denominator)
{}

Aif2XtlTimecode::~Aif2XtlTimecode()
{}

std::string Aif2XtlTimecode::GetXtlTimecodeString()
{
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	unsigned int millisecond;

	// quick and dirty, convert better code would not resort to
	// floating point

	double timeInSeconds = static_cast<double>(_sampleCount) *
						   static_cast<double>(_rateDen) /
						   static_cast<double>(_rateNum);

	hour = static_cast<unsigned int>(timeInSeconds / 3600.0);
	timeInSeconds -= hour * 3600.0;

	minute = static_cast<unsigned int>(timeInSeconds / 60.0);
	timeInSeconds -= minute * 60.0;

	second = static_cast<unsigned int>(timeInSeconds);
	timeInSeconds -= second;

	millisecond = static_cast<unsigned int>(timeInSeconds * 1000.0);

	// Sanity check.
	// I am a little leary of the floating point calcs.
	// Ensure millisecond is less than 1000 until some thought,
	// and use of integers, is invested.  This would be an
	// assertion in better code.
	if ( millisecond >= 1000 ) {
		 millisecond  =  999;
	}

	assert( hour        <   24 );
	assert( minute      <   60 );
	assert( second      <   60 );
	assert( millisecond < 1000 );
 
	char millisecond_string[4];
	::sprintf( millisecond_string, "%03d", millisecond ); 


	std::stringstream os;
	os << hour << ":" << minute << ":" << second << "." << millisecond_string;

	return os.str();
}
