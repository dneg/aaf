/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#include "AAFRational.h"

AAFRational::AAFRational(const AAFRational& s)
{
	num = s.num;
	denom = s.denom;
}

AAFRational& AAFRational::operator=(const AAFRational& s)
{
	if(this != &s)
	{
		num = s.num;
		denom = s.denom;
	}
	return(*this);
}

AAFRational AAFRational::operator+(const AAFRational& s) const
{
	AAFRational	result;
	result.num = (num * s.denom) + (s.num * denom);
	result.denom = s.denom * denom;
	return result;
}

AAFRational AAFRational::operator-(const AAFRational& s) const
{
	AAFRational	result;
	result.num = (num * s.denom) - (s.num * denom);
	result.denom = s.denom * denom;
	return result;
}

AAFRational AAFRational::operator*(const AAFRational& s) const
{
	AAFRational	result;
	result.num = num * s.num;
	result.denom = denom * s.denom;
	return(result);
}

AAFRational AAFRational::operator/(const AAFRational& s) const
{
	AAFRational	result;
	result.num = num * s.denom;
	result.denom = denom * s.num;
	return(result);
}

AAFRational::operator aafRational_t() const
{ 
	aafRational_t result;
	result.numerator = num;
	result.denominator = denom;
	return(result);
}
