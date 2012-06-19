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
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################


OS=`(uname -s) 2>/dev/null` || OS=Unknown

if [ "$OS" = "SunOS" ] ; then
	# Under Solaris "uname -p" returns "sparc" or "i386" for modern machines
	MACHINE=`(uname -p) 2>/dev/null` || MACHINE=Unknown
else
	# Other OSes use the POSIX -m flag for machine hardware type
	MACHINE=`(uname -m) 2>/dev/null` || MACHINE=Unknown
fi

case "${MACHINE}:${OS}" in
	*:IRIX*)
	echo MipsIrix
	exit 0;;

	*:Linux)
	echo ${MACHINE}Linux
	exit 0;;

	sparc*:SunOS)
	echo SparcSolaris
	exit 0;;

	# New cygwin environments return "CYGWIN_NT-5.x" so set to Windows_NT
	*86:CYGWIN_NT*)
	OS=Windows_NT
	echo Win
	exit 0;;

	# Cygwin builds
	*86:Windows_NT)
	echo Win
	exit 0;;

	# Mingw builds (E.g. using MSYS shell)
	*86:MINGW32_NT*)
	echo Win
	exit 0;;

	"Power Macintosh:Darwin")
	echo PPCDarwin
	exit 0;;

	*:Darwin)
	echo ${MACHINE}Darwin
	exit 0;;

	# FreeBSD and OpenBSD
	*86:FreeBSD|*86:OpenBSD)
	echo ${MACHINE}${OS}
	exit 0;;
esac

echo Unknown
exit 1
