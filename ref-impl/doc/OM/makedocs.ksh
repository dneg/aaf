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
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

#
# Generate the Object Manager documentation
#
# These commands generate html, other formats are possible
#
# Autoduck works by extracting specially tagged comments from
# the source files.
#

# OMDOC  - The Object Manager autoduck directory
# SRC    - The Object Manager sources
#
if [ "${LOCAL}" == "" ]
then
  OMDOC=doc/OM
  SRC="src/OM/*.cpp src/OM/*.h include/OM/*.h"
else
  OMDOC=docs
  SRC="src/*.cpp src/*.h"
fi
#
# Where autoduck was installed
#
ADROOT="c:/program files/autoduck"
#
# The autoduck program
#
ADEXEC="${ADROOT}/autoduck.exe"
#
# The standard autoduck html format
#
ADHTML="${ADROOT}/html.fmt"
#
# Extract both the internal and external documentation
#
# OMEXTERNAL = documentation of interest to Object Manager clients
# OMINTERNAL = documentation of interest to Object Manager developers
#
FLAGS="OMEXTERNAL | OMINTERNAL"
#
# The generated html file
#
OUTPUT=${OMDOC}/ObjectManager.html
#
# Customized files
#
OMHTML=${OMDOC}/html.fmt
OMCONT=${OMDOC}/contents.d
#
if [ ! -x "${ADEXEC}" ]; then
  echo "Error : Can't find \"${ADEXEC}\"."
  exit 1
fi
#
if [ ! -f "${ADHTML}" ]; then
  echo "Error : Can't find \"${ADHTML}\"."
  exit 1
fi
#
if [ ! -f "${OMHTML}" ]; then
  echo "Error : Can't find \"${OMHTML}\"."
  exit 1
fi
#
"${ADEXEC}" /x "${FLAGS}" /s ${OMHTML} /o ${OUTPUT} /r html /f "${ADHTML}" ${SRC} ${OMCONT}
#
