function usage
{
  echo "Usage : $TOOLNAME [OPTIONS]"
  echo "OPTIONS:"
  echo "  -c = compare only, report files that need to be updated [default]."
  echo "  -u = update any changed files."
  echo "  -h = help, print this message and exit."
}

HEADERS="\
AAFClassDefUIDs.h  \
AAFPropertyDefs.h  \
AAFStoredObjectIDs.h \
AAFExtEnum.h \
AAFPropertyIDs.h \
AAFTypeDefUIDs.h \
"

SOURCE=./include
DEST=../../ref-impl/include

SDKS="\
../../AAFMacSDK/include \
../../AAFMipsIrixSDK/MIPSpro/include \
../../AAFWinSDK/include \
../../AAFi686LinuxSDK/g++/include \
"

FAILURE=1
SUCCESS=0

TOOLNAME=${0##*/}
TOOLPATH=${0%/${TOOLNAME}}

mode="compare"

while getopts ":cuh" opt
do
  case $opt in
  c)
    mode="compare"
    ;;
  u)
    mode="update"
    ;;
  h)
    usage
    exit $SUCCESS
    ;;
  \?)
    echo "$TOOLNAME : Error: \"-$OPTARG\" not recognized."
    usage
    exit $FAILURE
    ;;
  esac
done
shift $(($OPTIND - 1))

for h in $HEADERS
do
  if [[ ! -f ${SOURCE}/$h ]]
  then
    echo "Can't find \"${SOURCE}/$h\"."
    exit $FAILURE
  fi
done

for h in $HEADERS
do
  if [[ ! -f ${DEST}/$h ]]
  then
    if [[ $mode == "update" ]]
    then
      echo "Creating \"${DEST}/$h\"."
      cp ${SOURCE}/$h ${DEST}/$h
    else
      echo "Can't find \"${DEST}/$h\"."
    fi
  fi
done

for h in $HEADERS
do
  cmp -s ${SOURCE}/$h ${DEST}/$h
  if [[ ${?} != 0 ]]
  then
    if [[ $mode == "update" ]]
    then
      echo "Updating \"${DEST}/$h\"."
      cp ${SOURCE}/$h ${DEST}/$h
    else
      echo "File needs updating \"${DEST}/$h\"."
    fi
  fi
done

for h in $HEADERS
do
  for s in $SDKS
  do
    if [[ ! -f $s/$h ]]
    then
      if [[ $mode == "update" ]]
      then
        echo "Creating \"$s/$h\"."
        cp ${SOURCE}/$h $s/$h
      else
        echo "Can't find \"$s/$h\"."
      fi
    fi
  done
done

for h in $HEADERS
do
  for s in $SDKS
  do
    cmp -s $SOURCE/$h $s/$h
    if [[ ${?} != 0 ]]
    then
      if [[ $mode == "update" ]]
      then
        echo "Updating \"$s/$h\"."
        cp $SOURCE/$h $s/$h
      else
        echo "File needs updating \"$s/$h\"."
      fi
    fi
  done
done

exit $SUCCESS
