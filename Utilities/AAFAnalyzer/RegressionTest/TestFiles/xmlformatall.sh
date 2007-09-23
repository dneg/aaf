#! /bin/bash

# This script runs all the xml test files through "xmlformat" in order
# to normallize their formatting.  Google "xmlformat" to locate the
# source distribution. Put the xmlformat.pl perl script anywhere in
# your path.

for f in *.xml
do
  echo xmlformat.pl $f
  cp $f /tmp
  xmlformat.pl /tmp/$f > $f
done
