#!/usr/bin/perl
# script to bump build number
# (c) Copyright AAF Association Inc 2001

use Time::Local;

# Needs to be run at the top level of the AAF tree
# You will need to have a complete up-to date checkout
# you will be prompted for your Sourceforge password three times in all

$AAFSDKBuildFile = "../ref-impl/include/AAFSDKBuild.h";
$AAFSDKBuildTemp = "../ref-impl/include/AAFSDKBuild.new";
$latestBuildOK = "latestBuildOK.txt";
$cvsRootFile = "CVS/Root";

# read though the file and pick up the variables we are interested in  

open(BUILDFILE, $AAFSDKBuildFile) or die "Can't find file $AAFSDKBuildFile";

while(<BUILDFILE>)
	{
	if(($num) = /#define AAF_MAJOR_VERSION (\d+)/)
	    {
		$majVer = $num;
		}
	if(($num) = /#define AAF_MINOR_VERSION (\d+)/)
	    {
		$minVer = $num;
		}
	if(($num) = /#define AAF_MAINT_RELEASE (\d+)/)
	    {
		$maintRel = $num;
		}
	
	if(($num) = /#define AAF_RELEASE_STAGE kAAFVersion(\s+)/)
	    {
		$release = $num;
		}
	 
	if(($num) = /#define AAF_BUILD_NUMBER (\d+)/)
	    {
		$curBuild = $num;
		}
	if(($num) = /#define AAF_CURRENT_YEAR \"(\d+)\"/)
	    {
		$curYear = $num;
		}
	}
close (BUILDFILE);

$year = (localtime(time))[5];
$year += 1900;

$curBuild++;

print "clock year is $year in file $curYear\n";

# now reopen file and rewrite it

open(NEWBUILDFILE, "> $AAFSDKBuildTemp") or die "Can't create file $AAFSDKBuildTemp";
open(BUILDFILE, $AAFSDKBuildFile) or die "Can't find file $AFSDKBuildFile";

while(<BUILDFILE>)
	{
	if(/#define AAF_BUILD_NUMBER (\d+)/)
	    {
		print NEWBUILDFILE "#define AAF_BUILD_NUMBER $curBuild\n";
		}
	elsif(/#define AAF_CURRENT_YEAR \"(\d+)\"/)
	    {
		print NEWBUILDFILE "#define AAF_CURRENT_YEAR \"$year\"\n";
		}
	elsif(/#define AAF_SHORT_VERSION_STRING \"\d+.\d+.\d+ build \d+\"/)
	    {
		print NEWBUILDFILE "#define AAF_SHORT_VERSION_STRING \"$majVer.$minVer.$maintRel build $curBuild\"\n";
		}
	else {
		print NEWBUILDFILE $_;
	  }
	}
close (BUILDFILE);

$buildTag = sprintf "Build-%04d", ($curBuild);

print "rename( $AAFSDKBuildTemp, $AAFSDKBuildFile)\n";

rename($AAFSDKBuildTemp, $AAFSDKBuildFile) == 0 or die "Can't rename ";

print "cvs ci -m \"Update version for new build\" $AAFSDKBuildFile\n";

system("cvs", "ci", "-m", "Update version for new build", $AAFSDKBuildFile) == 0 or die "Can't run cvs ci";

print "cvs -z3 -q tag $buildTag\n";
system("cvs", "-z3" "-q", "tag", $buildTag) == 0 or die "Can't run cvs tag";

open(CVSROOTFILE, $cvsRootFile) or die "Can't open file CVS Root";
if (!(($userid) = (<CVSROOTFILE> =~ m!:\S+:(\S+)\@cvs.aaf.sourceforge.net:/cvsroot/aaf!)))
	{
	print "couldn't find username in CVS root file\n";
	}
close(CVSROOTFILE);

# update file for build web page...

open(BUILDOKFILE, "> $latestBuildOK") or die "Can't create file $latestBuildOK";
print BUILDOKFILE "$buildTag\n";
close(BUILDOKFILE);
print "scp $latestBuildOK $userid\@aaf.sourceforge.net:/home/groups/a/aa/aaf/htdocs/build/"
system("scp", $latestBuildOK, "$userid\@aaf.sourceforge.net:/home/groups/a/aa/aaf/htdocs/build/") == 0 or die "Can't scp file";
unlink $latestBuildOK;