#! /bin/bash

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
	tar cvf - ${FileList} | gzip -c > $filename
}

create_sdk_distribution ()
{
	echo create_sdk_distribution $1;
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
AAF_BUILD_NUMBER=`grep "define AAF_BUILD_NUMBER"  ${AAFSDKBUILD_H} | awk '{print $3}'`;

VERSION_STRING=${AAF_MAJOR_VERSION}_${AAF_MINOR_VERSION}_${AAF_MAINT_RELEASE}.${AAF_BUILD_NUMBER}

DIST_FILE_NAME=${DIST_FILE}.${AAFPLATFORM}.${VERSION_STRING}

echo AAF_MAJOR_VERSION = $AAF_MAJOR_VERSION
echo AAF_MINOR_VERSION = $AAF_MINOR_VERSION
echo AAF_MAINT_RELEASE = $AAF_MAINT_RELEASE
echo AAF_RELEASE_STAGE = $AAF_RELEASE_STAGE
echo AAF_BUILD_NUMBER  = $AAF_BUILD_NUMBER

echo VERSION_STRING = $VERSION_STRING
echo DIST_FILE_NAME = $DIST_FILE_NAME

case ${AAFPLATFORM} in
        i686Linux ) create_unix_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST  ;;
	MipsIrix  ) create_unix_distribution $AAFSDK $DIST_FILE_NAME $DIST_LIST  ;;
	*         ) usage;;
esac
	

