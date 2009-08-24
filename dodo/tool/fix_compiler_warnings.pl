#!/usr/bin/perl -w
use strict;

my $file = shift || die "Need filename";
die "Too many arguments" if (scalar @ARGV != 0 );

# These fixes are required since some parts of the AAF API pass
# non-const strings to internal functions requiring const strings

open(IN, $file) || die "$!";
while (<IN>) {
	if ($file eq 'AAFTypeDefEnum.tmp')
	{
		s/^(\s*)pElementNames,$/${1}const_cast<aafCharacter_constptr*>(pElementNames),/;
	}
	if ($file eq 'AAFTypeDefRecord.tmp')
	{
		s/^(\s*)pMemberNames,$/${1}const_cast<aafCharacter_constptr*>(pMemberNames),/;
	}
	print;
}
