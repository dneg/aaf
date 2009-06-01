#!/usr/bin/perl
###############################################################################
#
# $Id$ $Name$
#
# The contents of this file are subject to the AAF SDK Public Source
# License Agreement Version 2.0 (the "License"); You may not use this
# file except in compliance with the License.  The License is available
# in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
# Advanced Media Workflow Association, Inc., or its successor.
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.  Refer to Section 3.3 of the License for proper use
# of this Exhibit.
#
# WARNING:  Please contact the Advanced Media Workflow Association,
# Inc., for more information about any additional licenses to
# intellectual property covering the AAF Standard that may be required
# to create and distribute AAF compliant products.
# (http://www.amwa.tv/policies).
#
# Copyright Notices:
# The Original Code of this file is Copyright 1998-2009, licensor of the
# Advanced Media Workflow Association.  All rights reserved.
#
###############################################################################

#
# Program to summarise CVS history in a format suitable for HTML build page
#

use Time::Local;

$cvs2cl = $ENV{CVS2CL} or die "need Environment VAR CVS2CL to point at cvs2cl.pl";


# get current timestamp into CVS style '2001-8-22 12:15:00'

sub getTimeStamp
	{
	my ($sec,$min,$hour,$day,$mon,$year,$wday,$yday,$isdst) = localtime(time);
	$mon++;
	$year += 1900;
	$timeStamp = sprintf "%d/%02d/%02d %02d:%02d:%02d", ($year, $mon, $day, $hour, $min, $sec);
	}
	
sub writeTimeStamp
	{
	local($timestamp) = @_[0];
	open(TSTAMP, "> .timestamp") or die "Can't create .timestamp file";
	print TSTAMP "$timestamp\n";
	close TSTAMP;
	}

# Fetch last timestamp

sub getLastTimeStamp
	{
	if(!open(TSTAMP, ".timestamp"))
		{
		print "No existing Timestamp\n";
		$lastTimeStamp = "2000-1-1 00:00:00";
		}
	else {
		($lastTimeStamp) = <TSTAMP>;
		chomp $lastTimeStamp;
		close (TSTAMP);
		}
	}


#-----------------------------

getLastTimeStamp();
getTimeStamp();

print "Current Time $timeStamp\n";
print "Last    Time $lastTimeStamp\n";

# run the commmand

$lastTimeStamp = "-d\'\>$lastTimeStamp\'";
print "$cvs2cl -l $lastTimeStamp\n";
system($cvs2cl, "-l", $lastTimeStamp) == 0 or die "Can't run cvs2cl";

	open(LOG, "ChangeLog");
	(@logfile) = <LOG>;
	close (LOG);


# Save current timestamp

writeTimeStamp($timeStamp);
