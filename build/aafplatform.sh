###############################################################################
#
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
# 
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
# 
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
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

    *86:Windows_NT)
	echo Win
	exit 0;;

    "Power Macintosh:Darwin")
	echo PPCDarwin
	exit 0;;
esac

echo Unknown
exit 1

