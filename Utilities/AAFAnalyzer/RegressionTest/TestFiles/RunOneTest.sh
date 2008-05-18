#! /bin/bash

# RunOneTest.sh - Run a single test given the name of the root name of
# the xml file that describes the test aaf file to create and the
# expected/actual result files. It is intended to be executed from a
# make file that passes the environment variables and arguments
# necessary for its execution.  Rather than attemtping to execute it
# directly run "make taget" from the
# AAFAnalyzer/RegressionTest/TestFiles directory.

# $1 - The test name, e.g. EPTrackContentsTest
# $2 - The test name used to filter results. e.g. "Edit Protocol Track Contents Test"

if [ $# -ne 2 ]; then
    echo incorrect number of arguments
    exit -1
fi

BUILDTESTFILE="${AAFSDKBINDIR}/BuildTestFile${EXE}"
ANALYZE="${AAFSDKBINDIR}/AAFAnalyzer${EXE} -reqs ../../Requirements/AAFRequirements.xml"
export LD_LIBRARY_PATH=${AAFSDKBINDIR}:${LD_LIBRARY_PATH}

rm -f $1.aaf
rm -f $1.actual

echo "${BUILDTESTFILE} $1.xml $1.aaf"
${BUILDTESTFILE} $1.xml $1.aaf
if [ $? -ne 0 ]; then
    echo BuildTestFiles failed with code $?
    exit -1
fi

echo "${ANALYZE} -filter \"$2\" -verbose $1.aaf > $1.actual"
${ANALYZE} -filter "$2" -verbose $1.aaf > $1.actual

echo diff $1.expected $1.actual
diff $1.expected $1.actual > /dev/null


