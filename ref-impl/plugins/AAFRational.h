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