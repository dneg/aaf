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
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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
	AAFRational(const AAFRational& s);
	// ------- destructor
	~AAFRational() { }
	// ---------- assignment
	AAFRational& operator=(const AAFRational& s);
	// ---------- conversion to double *
	operator double() const { return (double)num / (double)denom; }
	operator aafRational_t() const;
	AAFRational operator+(const AAFRational& s) const;
	AAFRational operator-(const AAFRational& s) const;
	AAFRational operator*(const AAFRational& s) const;
	AAFRational operator/(const AAFRational& s) const;
	// ------- relational operators
	aafBool operator==(const AAFRational& s) const
	{ return (aafBool) (num == s.num ? kAAFTrue : kAAFFalse); }
	aafBool operator!=(const AAFRational& s) const
		{ return (aafBool) (num != s.num ? kAAFTrue : kAAFFalse); }
	aafBool operator>(const AAFRational& s) const
		{ return (aafBool) ((double)*this > (double)s ? kAAFTrue : kAAFFalse); }
	aafBool operator<(const AAFRational& s) const
		{ return (aafBool) ((double)*this < (double)s ? kAAFTrue : kAAFFalse); }
	aafBool operator<=(const AAFRational& s) const
		{ return (aafBool) (*this > s ? kAAFFalse : kAAFTrue); }
	aafBool operator>=(const AAFRational& s) const
		{ return (aafBool) (*this < s ? kAAFFalse : kAAFTrue); }
};

#endif
