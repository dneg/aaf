#! /bin/sh

MACHINE=`(uname -m) 2>/dev/null` || MACHINE=Unknown
OS=`(uname -s) 2>/dev/null` || OS=Unknown

case "${MACHINE}:${OS}" in
    *:IRIX*)
	echo MipsIrix
	exit 0;;

    *:Linux)
	echo ${MACHINE}Linux
	exit 0;;

    sun*:SunOS)
	echo SunSunOS
	exit 0;;

    *86:Windows_NT)
	echo Win
	exit 0;;
esac

echo Unknown
exit 1

