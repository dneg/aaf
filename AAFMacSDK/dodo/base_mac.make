########################################################################
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
########################################################################

dod = {aaf}dodo:
macros = {dod}macros:


all Ä "{macros}base.mac"


MAKEFILE = base_mac.make


"{macros}base.mac" Ä "{dod}CopyrightMessage.txt" "{dod}sync_copyright.pl" # {MAKEFILE}
	perl "{dod}sync_copyright.pl" -macro "{macros}base.mac" -copyright "{dod}CopyrightMessage.txt" > "{dod}base.mac.tmp"
	copy_if_diff -p "{dod}base.mac.tmp" "{macros}base.mac"
	rm "{dod}base.mac.tmp"


cleantmp Ä
	rm "{dod}base.mac.tmp"
	
clean Ä cleantmp

realclean Ä clean