#! /bin/bash
###############################################################################
#
# $Id$ $Name$
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
# The Original Code of this file is Copyright 1998-2004, Licensor of the
# AAF Association.
#
###############################################################################

usage ()
{
	echo "Usage: $0 {release|sdk} dist_list dist_file AAFSDK AAFPLATFORM AAFSDKBUILD_H"
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
	printf "\n";
	printf "\tThis script is normally called by the \"release-dist\" and \"sdk-dist\"\n";
        printf "\tmake targets.\n";
	exit -1;
}

create_unix_distribution ()
{
	echo create_unix_distribution $1 $2 $3;

	cd $1;
	filename=$2.tar.gz;
	FileList=`grep -v \# $3`;
	tar cvf - ${FileList} | gzip -c > ${filename}
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


create_macos9_distribution ()
{
	echo create_mac_distribution $1 $2 $3;

	if [ ! -x `which stuff` ]; then
	    echo Error StuffIt command line program not available.
	    exit -1;
	fi

	cd $1;
	dirname=$2
	sitformat=sitx
	sitname=$2.${sitformat}

	FileList=`grep -v \# $3`;

        mkdir -p $dirname;
	for file in  ${FileList} ; do
	    echo ${file}
	    mkdir -p ${dirname}/`dirname "${file}"`

	    # Wait.. this is Mac, simply cp'ing a file would be too simple.
	    # It won't copy the resource fork.  
	    # cp -r "${file}" ${dirname}/`dirname "${file}"`

	    rm -f /tmp/tmp.sitx
	    stuff -q --format=sitx --compression_method=0 --name=/tmp/tmp.sitx "${file}"
	    unstuff -q --destination=${dirname}/`dirname "${file}"` /tmp/tmp.sitx
	done

	echo ============ Creating StuffIt Archive ==================
	cd ${dirname}
	cd ..
	echo /usr/local/bin/stuff --format=${sitformat} --overwrite --name=`basename ${dirname}` `basename ${sitname}`
	/usr/local/bin/stuff --format=${sitformat} --overwrite --name=`basename ${sitname}` `basename ${dirname}`
}

create_win_distribution ()
{
	echo create_zip_distribution $1 $2 $3;

	cd $1;
	filename=$2.zip;
	FileList=`grep -v \# $3`;
	zip -r ${filename} ${FileList} ;
}


#
# main
#

if [ $# != 6 ]; then
	usage;
fi

DIST_LIST=$2;
DIST_FILE=$3;
AAFSDK=$4;
AAFPLATFORM=$5;
AAFSDKBUILD_H=$6;

AAF_MAJOR_VERSION=`grep "define AAF_MAJOR_VERSION" ${AAFSDKBUILD_H} | awk '{print $3}'`;
AAF_MINOR_VERSION=`grep "define AAF_MINOR_VERSION" ${AAFSDKBUILD_H} | awk '{print $3}'`;
AAF_MAINT_RELEASE=`grep "define AAF_MAINT_RELEASE" ${AAFSDKBUILD_H} | awk '{print $3}'`;
AAF_RELEASE_STAGE=`grep "define AAF_RELEASE_STAGE" ${AAFSDKBUILD_H} | awk '{print $3}'`;
AAF_PATCH_LEVEL=`grep "define AAF_PATCH_LEVEL"  ${AAFSDKBUILD_H} | awk '{print $3}'`;

VERSION_STRING=${AAF_MAJOR_VERSION}_${AAF_MINOR_VERSION}_${AAF_MAINT_RELEASE}.${AAF_PATCH_LEVEL}

DIST_FILE_NAME=${DIST_FILE}.${AAFPLATFORM}.${VERSION_STRING}

echo AAF_MAJOR_VERSION = $AAF_MAJOR_VERSION
echo AAF_MINOR_VERSION = $AAF_MINOR_VERSION
echo AAF_MAINT_RELEASE = $AAF_MAINT_RELEASE
echo AAF_RELEASE_STAGE = $AAF_RELEASE_STAGE
echo AAF_PATCH_LEVEL  = $AAF_PATCH_LEVEL

echo VERSION_STRING = $VERSION_STRING
echo DIST_FILE_NAME = $DIST_FILE_NAME

case ${AAFPLATFORM} in
        i686Linux ) create_unix_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST  ;;

	MipsIrix  ) create_unix_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST  ;;

	Win       ) create_win_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST  ;;

	Mac       ) create_macos9_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST  ;;

	PPCDarwin ) create_darwin_distribution "$AAFSDK" "$DIST_FILE_NAME" "$DIST_LIST" "$AAFSDKBUILD_H" $1 ;;

	*         ) usage;;
esac
	

