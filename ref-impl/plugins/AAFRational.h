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

#ifndef AAF_RATIONAL
#define AAF_RATIONAL	1
#include "AAFTypes.h"

class AAFRational    {
	aafInt32	num;
	aafInt32	denom;
public:
	// -------- construct a null string
	AAFRational() { num = 0; denom = 1; }
	// --- construct with initializer
	AAFRational(aafInt32 newNum, aafInt32 newDenom) { num = newNum; denom = newDenom; };
	AAFRational(aafRational_t newVal) { num = newVal.numerator; denom = newVal.denominator; };
	// ------- copy constructor
	AAFRational(AAFRational& s);
	// ------- destructor
	~AAFRational() { }
	// ---------- assignment
	AAFRational& operator=(AAFRational& s);
	// ---------- conversion to double *
	operator double() { return (double)num / (double)denom; }
	operator aafRational_t();
	AAFRational operator+(AAFRational& s);
	AAFRational operator-(AAFRational& s);
	AAFRational operator*(AAFRational& s);
	AAFRational operator/(AAFRational& s);
	// ------- relational operators
	aafBool operator==(AAFRational& s)
	{ return (aafBool) (num == s.num ? AAFTrue : AAFFalse); }
	aafBool operator!=(AAFRational& s)
		{ return (aafBool) (num != s.num ? AAFTrue : AAFFalse); }
	aafBool operator>(AAFRational& s)
		{ return (aafBool) ((double)*this > (double)s ? AAFTrue : AAFFalse); }
	aafBool operator<(AAFRational& s)
		{ return (aafBool) ((double)*this < (double)s ? AAFTrue : AAFFalse); }
	aafBool operator<=(AAFRational& s)
		{ return (aafBool) (*this > s ? AAFFalse : AAFTrue); }
	aafBool operator>=(AAFRational& s)
		{ return (aafBool) (*this < s ? AAFFalse : AAFTrue); }
};

#endif