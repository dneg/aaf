#/usr/bin/perl
# perl idlcleanup.perl AAF.idl |perl this-file > AAFNoSpace.idl
$/ = "";
$* =1;
while (<>) {
	s|(^\s*//)\s*\n\s*//\s*\n|\1\n|;
	# remove double blank comments
	s|^\s*\n||;  # remove blank lines
	s|^(//\s*{filename:)|\n\1|; # put a blank line before filename line
	
	print;
}
