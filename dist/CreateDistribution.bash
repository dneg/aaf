#! /bin/bash
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

usage ()
{
	echo "Usage: $0 {release|sdk} dist_list dist_file AAFSDK AAFPLATFORM AAFSDKBUILD_H {WINPLATFORM}"
	printf "\n";
	printf "\tdist_list     - A file containing a list of files and directories\n";
	printf "\t                that make up the distribution.  This file path, and the files\n";
	printf "\t                listed therein, can be absolute or relative to the AAFSDK path.\n";
	printf "\tdist_file     - Name of the distribution.  This is the full path excluding the\n";
	printf "\t                extension (e.g. excluding zip, or 1_0_1.tar.gz). This file paht\n";
	printf "\t                can be absolute, or relative the AAFSDK path.\n"
	printf "\tAAFSDK        - The value of the AAFSDK build variable.\n";
	printf "\tAAFPLATFORM   - The value of the AAFPLATFORM build variable.\n";
	printf "\tAAFSDKBUILD_H - Path of AAFSDKBuild.h file.\n";
        printf "\tWINPLATFORM   - Set to Win32 or x64 to identify the Windows build type.\n";
	printf "\n";
	printf "\tThis script is normally called by the \"release-dist\" and \"sdk-dist\"\n";
	printf "\tmake targets.\n";
	exit 1;
}

create_unix_distribution ()
{
	echo create_unix_distribution $1 $2 $3;

	cd $1
	tarname=$2.tar
	filename=$tarname.gz
	FileList=`grep -v \# $3`
	tar cvf $tarname ${FileList} || exit 1
	gzip -f $tarname || exit 1
	if [ ! -r $filename ] ; then
		exit 1
	fi
	echo "output file is: "
	echo "  $1/${filename}"
}

create_darwin_distribution ()
{
	echo create_darwin_distribution $1 $2 $3 $5;
	AAFSDK="$1"
	DIST_FILE_NAME="$2"
	DIST_LIST="$3"
	AAFSDKBUILD_H="$4"
	RELEASE_TYPE="$5"

	$AAFSDK/Scripts/mkDistRoot "$AAFSDK" "$DIST_LIST"
	$AAFSDK/Scripts/mkDistPackage "$AAFSDK" "$DIST_FILE_NAME" "$AAFSDKBUILD_H" "$RELEASE_TYPE"
	$AAFSDK/Scripts/rmDistRoot "$AAFSDK"
}

create_win_distribution ()
{
	echo create_win_distribution $1 $2 $3 $4;

	cd $1
	filename=$2.$4.zip
	FileList=$(grep -v \# $3 | sed "s/{PlatformDir}/${4}/g")
	zip -r ${filename} ${FileList} || exit 1
	echo "output file is: "
	echo "  $1/${filename}"
}


#
# main
#

if [ $# -eq 0 ]; then
	usage;
fi

DIST_LIST=$2;
DIST_FILE=$3;
AAFSDK=$4;
AAFPLATFORM=$5;
AAFSDKBUILD_H=$6;
WINPLATFORM=$7;

AAF_MAJOR_VERSION=`grep "define AAF_MAJOR_VERSION" ${AAFSDKBUILD_H} | awk '{print $3}'`;
AAF_MINOR_VERSION=`grep "define AAF_MINOR_VERSION" ${AAFSDKBUILD_H} | awk '{print $3}'`;
AAF_MAINT_RELEASE=`grep "define AAF_MAINT_RELEASE" ${AAFSDKBUILD_H} | awk '{print $3}'`;
AAF_RELEASE_STAGE=`grep "define AAF_RELEASE_STAGE" ${AAFSDKBUILD_H} | awk '{print $3}'`;
AAF_PATCH_LEVEL=`grep "define AAF_PATCH_LEVEL" ${AAFSDKBUILD_H} | awk '{print $3}'`;

VERSION_STRING=${AAF_MAJOR_VERSION}.${AAF_MINOR_VERSION}.${AAF_MAINT_RELEASE}.${AAF_PATCH_LEVEL}

DIST_FILE_NAME=${DIST_FILE}-${AAFPLATFORM}-${VERSION_STRING}

echo AAF_MAJOR_VERSION = $AAF_MAJOR_VERSION
echo AAF_MINOR_VERSION = $AAF_MINOR_VERSION
echo AAF_MAINT_RELEASE = $AAF_MAINT_RELEASE
echo AAF_RELEASE_STAGE = $AAF_RELEASE_STAGE
echo AAF_PATCH_LEVEL = $AAF_PATCH_LEVEL

echo VERSION_STRING = $VERSION_STRING
echo DIST_FILE_NAME = $DIST_FILE_NAME

case ${AAFPLATFORM} in
	Win)                    create_win_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST ${WINPLATFORM};;

	i686Linux|x86_64Linux)	create_unix_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST ;;

	MipsIrix)		create_unix_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST ;;

	SparcSolaris)	        create_unix_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST ;;

	i386Darwin|x86_64Darwin|UniversalDarwin)	create_unix_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST ;;

	PPCDarwin)		create_darwin_distribution "$AAFSDK" "$DIST_FILE_NAME" "$DIST_LIST" "$AAFSDKBUILD_H" $1 ;;

	*)			echo; echo "** Unknown AAFPLATFORM \"${AAFPLATFORM}\" **"; usage;;
esac
