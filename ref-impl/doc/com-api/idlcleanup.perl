#/usr/bin/perl
# usage: perl this-file.perl AAF.idl |perl idlblankcomfix.perl > AAFNoSpace.idl
$/ = "";  # munch input as paragraphs rather than lines
while (<>) {
	# replace any two blank // with a single one with same number of initial spaces
	s|(^\s*//)\s*\n+\s*//\s&\n|\1\n|m;

	s|^\s*\n||gm;   # replace blank lines

	# delete the object sections
	s|^\[\n\s*object,\n\s*uuid\(.*\),\n\s*helpstring\(".*"\),\n\s*pointer_default\(unique\)\n\]\n||m;

	# remove comment block at start of each interface
	s|^// \*\*\*\*\*+\n//\n// Interface \S+\n//\n// \*+\n||m;

	# remove comment block at start of each method
	s|^  *//\*\*\*\*\*+\n  //\n  // \S+\(\)\n  //\n||m;

	# remove double comments
	s|// //|//|m;

	# insert Section directives in place of waffle 
	s|This method will return the following codes\. |Returns:\n  //|;
	s|This method will return the following codes:|Returns:|;

	# fix for indent continuation line following a - bullet
	# docjet can't handle additional indent
	# easiest thing is to indent bullets in another 2 spaces !
	s|^(\s*)//(\s+)-(\s+)(\S)|\1//\2  -\3\4|gm;

	# simplify section labels that upset DocJet
	s|// Objects that implement (\S*) also implement the following interfaces:|// Implements:|;
	s|This method succeeds if all of the following are true:|Succeeds if:|;
	s|This method will succeed only if all of the following are true:|Succeeds if:|;
	s|Succeeds if all of the following are true:|Succeeds if:|;
	s|Succeeds if the following is true:|Succeeds if:|;
	s|Valid values include:|Valid values:|;
	s|Valid values are:|Valid values:|;
	s|Values are:|Valid values:|;

	# put a blank line before filename line
	s|^(//\s*{filename:)|\n\1|; 

	# force NOTE to lower
	s|NOTE:|Note:|;
	 
	# Fix comments in MultiAccess
	s|\[IN\] --|--|g;
	s|\[OUT\] --|--|g;

	# make stub comment a section header
	s|NOTE! Stub only.|Stub Only:\n  //|; 
	
	# change comment block for stubbed interfaces
	s|^/\*\*+\\\n\*\s+Stub only.\s+Implementation not yet added\s+\*\n\\\*\*+/|// Stub Only:\n//  Implementation not yet added\n//\n|m;

	# strip out Ascii Art from DigitalImageDescriptor
	s|^// \+-+\+\n(// \|.*\n)*// \+-+.*\n//\s+\+-+\+\n|// {image:DigImgAreas}\n|m;

	# get rid of blank comments at start of interface description
	s|(^//\s*{filename:.*}\n)\s*//\s*\n|\1|m;

	print;
}
