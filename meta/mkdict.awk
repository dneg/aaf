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
# The Initial Developer of the Original Code of this file and the
# licensor of the Advanced Media Workflow Association is
# Avid Technology.
# All rights reserved.
#
###############################################################################
#
# Generate an AAF meta dictionary from a .csv spreadsheet.
#
# Command line:
#
# awk [-v APP="app"] [-v APPVER="1.1" ] [-v ALIAS=yes ] [-v SYM="s_sym"] -f mkdict.awk AAFMetaDict.csv > AAFMetaDictionary.h
#
#	the csv file must have a line with "_fields" in the first column
#	before any line that does not begin with #
#
#   if the -v APP= value is specified, mkdict will only parse lines with the given text in the r_app field
#   if the -v APPVER= value is specified, mkdict will only parse lines in which r_app is a string like
#   "AAF>=1.2" and the stated relation is true
#   if no APP is specified, will skip lines beginning #
#	always skips lines beginning ##
#   always skips lines of r_nest==Node
#   always skips lines with r_sym blank
#
#	if the -v ALIAS= is non-null, will emit aliases for Classes and Properties in place of sym
#
#   if the -v SYM= value is specified, mkdict will use this field instead of r_sym
#
# Authors
#
#   Tim Bingham tim_bingham@avid.com - original
#   Oliver Morgan oliver@metaglue.com - extensive revisions for new features
#
# This program uses the following spreadsheet columns for the
# purposes indicated ...
#
# note that many other columns are named, but not used by this script
#
#      Column                          Purpose
#
# r_reg					  => which of the SMPTE registers this belongs in Groups Types Labels
#                            (and Aliases - non SMPTE)
# r_nest				  => whether this line will be a Node Leaf or Child in SMPTE
# ul_1..ul_16             => AUID (with appropriate reordering).
# s_sym                   => class/property/type/member/label/alias name
# r_sym                   => the SMPTE version of s_sym (sometimes different)
# s_type_sym              => if this field == "AAFClass" then the entity is a
#                            class, otherwise it is a property and this field
#                            denotes the type of the property. When this
#                            field == "AAFClass" this row is taken to be the
#                            start of a class definition. All the following
#                            rows (until the start of the next class) are
#                            taken to be properties of this class.
#						     if this field == 'type" this row is taken to be the
#							 start of a type definition. If kind is one of:
#								enumeration, record, extendible, reference weak,
#							 then succeeding rows may define members 
# r_kind	              => kind of type
# r_qualif                => For arrays indicates whether the array is fixed
#                            or varying
#                            For references indicates whether the reference
#                            is strong or weak.
# r_value                 => for integers, TRUE if the type is signed
#							 For enumeration members the value of the member
# s_target_sym            => For enumerations the type of every member
#							 For records the type of each member
#								(this should also be sepcified in s_type_sym)
#							 For arrays the element type.
#                            Otherwise this field should be empty.
# r_minOccurs             => if this field == "1" then this is a required
#                            property. If this field is "O" then this is an
#                            optional property.
# r_isAbstract            => if this entity is a class and the class is
#                            abstract, this field contains "abstract" otherwise
#                            this field should be empty.
# r_isUnique              => if this entity is a property and the property is
#                            unique identifier, this field contains "uid"
#                            otherwise this field should be empty.
# s_parent_sym            => For a class this field is
#                            the immediate ancestor
#							 For a property this is the owning class
#							 For a weak ref def member this is the owning class of the property
# q_parent_app		  => the looked-up alias information for the parent
# r_tag                   => property id lower 2 bytes used for properties
# g_alias                 => full classname of which this is an (short) alias as used in code
#
BEGIN {
  #
  # symbolic names for columns are input from line beginning #fields
  #
  errors = 0;
  FS=","
  processedfields=0;
 
  # default value for field name that contains the normative symbols
  if( SYM=="" ) SYM="r_sym";

  #
  # Write a header to the generated file.
  #
  printf("//=---------------------------------------------------------------------=\n");
  printf("//\n");
  printf("// This file was GENERATED for the AAF SDK\n");
  printf("//\n");
  printf("// $""Id$ $""Name$\n");
  printf("//\n");
  printf("// The contents of this file are subject to the AAF SDK Public Source\n");
  printf("// License Agreement Version 2.0 (the \"License\"); You may not use this\n");
  printf("// file except in compliance with the License.  The License is available\n");
  printf("// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the\n");
  printf("// Advanced Media Workflow Association, Inc., or its successor.\n");
  printf("//\n");
  printf("// Software distributed under the License is distributed on an \"AS IS\"\n");
  printf("// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See\n");
  printf("// the License for the specific language governing rights and limitations\n");
  printf("// under the License.  Refer to Section 3.3 of the License for proper use\n");
  printf("// of this Exhibit.\n");
  printf("//\n");
  printf("// WARNING:  Please contact the Advanced Media Workflow Association,\n");
  printf("// Inc., for more information about any additional licenses to\n");
  printf("// intellectual property covering the AAF Standard that may be required\n");
  printf("// to create and distribute AAF compliant products.\n");
  printf("// (http://www.amwa.tv/policies).\n");
  printf("//\n");
  printf("// Copyright Notices:\n");
  printf("// The Original Code of this file is Copyright 1998-2009, licensor of the\n");
  printf("// Advanced Media Workflow Association.  All rights reserved.\n");
  printf("//\n");
  printf("// The Initial Developer of the Original Code of this file and the\n");
  printf("// licensor of the Advanced Media Workflow Association is\n");
  printf("// Avid Technology.\n");
  printf("// All rights reserved.\n");
  printf("//\n");
  printf("//=---------------------------------------------------------------------=\n");
  printf("//\n");
  printf("// Meta definition of the AAF object model.\n");
  printf("//\n");
  printf("// This file contains invocations of the macros described below.\n");
  printf("//\n");
  printf("// To use this file - \n");
  printf("//   1) #define those macros required to suit your usage\n");
  printf("//   2) #include this file\n");
  printf("//\n");
  printf("// This file will provide empty definitions for those macros you\n");
  printf("// choose not to define.\n");
  printf("//\n");
  printf("// After invoking the macros, this file will also undefine them.\n");
  printf("// All macros will be undefined - those you chose to define\n");
  printf("// and those defined by this file.\n");
  printf("//\n");
  printf("// This is an automatically generated file.\n");
  printf("//\n");
  printf("// If you edit this file your changes will be lost\n");
  printf("// the next time this file is generated.\n");
  printf("//\n");
  printf("// The following source files were used as input : \n");
  printf("//\n");
  printf("// ");
  for (i = 1; i < ARGC; i++) {
    printf("%s ", ARGV[i]);
  }
  printf("\n");
  printf("//\n");
  "date" | getline date
  user = getUser()
  "hostname" | getline computer
  printf("// This file was generated on %s\n", date);
  printf("// by user %s on system %s.\n", user, computer);
  printf("//\n");
  printf("// Key to macros.\n");
  printf("//\n");
  printf("//   For formal arguments marked with [*] the actual arguments\n");
  printf("//   supplied by the macro invocations in this file are\n");
  printf("//   themselves macro invocations.\n");
  printf("//\n");
  printf("// AAF_TABLE_BEGIN()\n");
  printf("//\n");
  printf("//   Begin a table of AAF class and property definitions.\n");
  printf("//\n");
  printf("// AAF_TABLE_END()\n");
  printf("//\n");
  printf("//   End a table of AAF class and property definitions.\n");
  printf("//\n");
  printf("// AAF_SYMBOL( symbol, name, alias, description )\n");
  printf("//\n");
  printf("//   Define a container for the preferred symbol etc of AAF meta entry\n");
  printf("//\n");
  printf("//     symbol      = the syntactically correct symbol of the entry\n");
  printf("//     name        = the name of the entry\n");
  printf("//     alias       = the legacy alias name of the entry, if any\n");
  printf("//     description = the text description of the entry, if any\n");
  printf("//\n");
  printf("//   Note: this uses the awkward 2 step approach described by Steve Summit\n");
  printf("//\n");
  printf("// AAF_CLASS(name, id, parent, concrete)\n");
  printf("//\n");
  printf("//   Define an AAF class.\n");
  printf("//\n");
  printf("//     name      = the name of the class\n");
  printf("//     id        = the auid used to identify the class [*]\n");
  printf("//     parent    = the immediate ancestor class\n");
  printf("//     concrete  = true if the class is concrete\n");
  printf("//                 false if the class is abstract\n");
  printf("//\n");
  printf("// AAF_CLASS_END(name, id, parent, concrete)\n");
  printf("//\n");
  printf("//   End an AAF class definition.\n");
  printf("//\n");
  printf("//     name      = the name of the class\n");
  printf("//     id        = the auid used to identify the class [*]\n");
  printf("//     parent    = the immediate ancestor class\n");
  printf("//     concrete  = true if the class is concrete\n");
  printf("//                 false if the class is abstract\n");
  printf("//\n");
  printf("// AAF_CLASS_SEPARATOR()\n");
  printf("//\n");
  printf("//   Separate one AAF class definition from another.\n");
  printf("//\n");
  printf("// AAF_PROPERTY(name, id, tag, type, mandatory, uid, container)\n");
  printf("//\n");
  printf("//   Define an AAF property.\n");
  printf("//\n");
  printf("//     name      = the name of the property\n");
  printf("//     id        = the auid used to identify the property [*]\n");
  printf("//     tag       = the short form of the id\n");
  printf("//     type      = the type of the property [*]\n");
  printf("//     mandatory = true if the property is mandatory\n");
  printf("//                 false if the property is optional\n");
  printf("//     uid       = true if the property is the unique identifier\n");
  printf("//                 for this class, false if the property is not\n");
  printf("//                 the unique identifier for this class\n");
  printf("//     container = the class that defines this property\n");
  printf("//\n");
  printf("// AAF_TYPE(type)\n");
  printf("//\n");
  printf("//   Define an AAF type.\n");
  printf("//\n");
  printf("//     type      = the type\n");
  printf("//\n");
  printf("// AAF_REFERENCE_TYPE(type, target)\n");
  printf("//\n");
  printf("//   Define an AAF reference type.\n");
  printf("//\n");
  printf("//     type      = the reference type\n");
  printf("//     target    = the class of the target of the reference\n");
  printf("//\n");
  printf("// AAF_REFERENCE_TYPE_NAME(type, target)\n");
  printf("//\n");
  printf("//   Define the name of an AAF reference type.\n");
  printf("//\n");
  printf("//     type      = the reference type\n");
  printf("//     target    = the class of the target of the reference\n");
  printf("//\n");
  printf("// AAF_TYPE_TABLE_BEGIN()\n");
  printf("//\n");
  printf("//   Begin a table of AAF type definitions.\n");
  printf("//\n");
  printf("// AAF_TYPE_TABLE_END()\n");
  printf("//\n");
  printf("//   End a table of AAF type definitions.\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed)\n");
  printf("//\n");
  printf("//   Define an AAF integral type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     size      = the size (in bytes) of the type\n");
  printf("//     signed    = true if the type is signed, false otherwise\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_ENUMERATION(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF enumerated type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the type of each enumeration element [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(name, value, container)\n");
  printf("//\n");
  printf("//   Define a member of an AAF enumerated type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     value     = the value of this enumeration member\n");
  printf("//     container = the name of the containing enumerated type\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_ENUMERATION_END(name, id, type)\n");
  printf("//\n");
  printf("//   End an AAF enumerated type definition.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the type of each enumeration element [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_RECORD(name, id)\n");
  printf("//\n");
  printf("//   Define an AAF record type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, parent)\n");
  printf("//\n");
  printf("//   Define a field of an AAF record type.\n");
  printf("//\n");
  printf("//     name      = the name of the field\n");
  printf("//     type      = the type of the field [*]\n");
  printf("//     container = the name of the containing record type\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_RECORD_END(name, id)\n");
  printf("//\n");
  printf("//   End an AAF record type definition.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF varying array type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the element type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count)\n");
  printf("//\n");
  printf("//   Define an AAF fixed array type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the element type [*]\n");
  printf("//     count     = the number of elements\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_RENAME(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF renamed type.\n");
  printf("//   Define \"name\" as a new name for \"type\".\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_STRING(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF string type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the element type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id)\n");
  printf("//\n");
  printf("//   Define an AAF extendible enumerated type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, auid, container)\n");
  printf("//\n");
  printf("//   Define a member of an AAF extendible enumerated type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     auid      = the auid of this enumeration member [*]\n");
  printf("//     container = the name of the containing extendible enumerated type\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name, id)\n");
  printf("//\n");
  printf("//   End an AAF extendible enumerated type definition.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_CHARACTER(name, id)\n");
  printf("//\n");
  printf("//   Define an AAF character type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_INDIRECT(name, id)\n");
  printf("//\n");
  printf("//   Define an AAF indirect type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_OPAQUE(name, id)\n");
  printf("//\n");
  printf("//   Define an AAF opque type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_SET(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF set type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the element type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF strong reference type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the target type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF strong reference set type.\n");
  printf("//\n");
  printf("//     name      = the name of the type [*]\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the target type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF strong reference vector type.\n");
  printf("//\n");
  printf("//     name      = the name of the type [*]\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the target type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF weak reference type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the target type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(name, parent, container)\n");
  printf("//\n");
  printf("//   Define a member of an AAF extendible enumerated type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     parent    = the parent property for member [*]\n");
  printf("//     container = the name of the containing weak reference type\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(name, id, type)\n");
  printf("//\n");
  printf("//   End an AAF extendible enumerated type definition.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the target type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF weak reference set type.\n");
  printf("//\n");
  printf("//     name      = the name of the type [*]\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the target type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF weak reference vector type.\n");
  printf("//\n");
  printf("//     name      = the name of the type [*]\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the target type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_STREAM(name, id)\n");
  printf("//\n");
  printf("//   Define an AAF stream type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//\n");
  printf("// AAF_TYPE_SEPARATOR()\n");
  printf("//\n");
  printf("//   Separate one AAF type definition from another.\n");
  printf("//\n");
  printf("// AAF_ALIAS_TABLE_BEGIN()\n");
  printf("//\n");
  printf("//   Begin a table of AAF alias definitions.\n");
  printf("//\n");
  printf("// AAF_ALIAS_TABLE_END()\n");
  printf("//\n");
  printf("//   End a table of AAF alias definitions.\n");
  printf("//\n");
  printf("// AAF_ALIAS_SEPARATOR()\n");
  printf("//\n");
  printf("//   Separate one AAF alias from another.\n");
  printf("//\n");
  printf("// AAF_CLASS_ALIAS(name, alias)\n");
  printf("//\n");
  printf("//   Define a name used in the source code different from the name of the metadefinition.\n");
  printf("//\n");
  printf("//     name      = class name\n");
  printf("//     alias     = another, usually shorter, name by which the\n");
  printf("//                 class is also known\n");
  printf("//\n");
  printf("// AAF_INSTANCE_TABLE_BEGIN()\n");
  printf("//\n");
  printf("//   Begin a table of AAF definition instance definitions.\n");
  printf("//\n");
  printf("// AAF_INSTANCE_TABLE_END()\n");
  printf("//\n");
  printf("//   End a table of AAF definition instance definitions.\n");
  printf("//\n");
  printf("// AAF_INSTANCE_GROUP_SEPARATOR()\n");
  printf("//\n");
  printf("//   Separate one group of AAF definition instances from another.\n");
  printf("//\n");
  printf("// AAF_INSTANCE_GROUP(name, target)\n");
  printf("//\n");
  printf("//   Begin a group of AAF definition instances.\n");
  printf("//\n");
  printf("//     name      = name of container for these instances\n");
  printf("//     target    = class name of each instance\n");
  printf("//\n");
  printf("// AAF_INSTANCE_GROUP_END(name, target)\n");
  printf("//\n");
  printf("//   End a group of AAF definition instances.\n");
  printf("//\n");
  printf("//     name      = name of container for these instances\n");
  printf("//     target    = class name of each instance\n");
  printf("//\n");
  printf("// AAF_INSTANCE_SEPARATOR()\n");
  printf("//\n");
  printf("//   Separate one AAF definition instance from another.\n");
  printf("//\n");
  printf("// AAF_INSTANCE(class, name, id, desc)\n");
  printf("//\n");
  printf("//   Define an instance of an AAF definition.\n");
  printf("//\n");
  printf("//     class     = the class of the instance\n");
  printf("//     name      = the name of the instance\n");
  printf("//     id        = the auid used to identify the instance [*]\n");
  printf("//     desc      = the text description (may be null)\n");
  printf("//\n");
  printf("// AAF_INSTANCE_END(class, name, id)\n");
  printf("//\n");
  printf("//   Complete an instance of an AAF definition.\n");
  printf("//\n");
  printf("//     class     = the class of the instance\n");
  printf("//     name      = the name of the instance\n");
  printf("//     id        = the auid used to identify the instance [*]\n");
  printf("//\n");
  printf("// AAF_INSTANCE_PROPERTY(name, type, value)\n");
  printf("//\n");
  printf("//   Define a property of an instance of an AAF definition.\n");
  printf("//\n");
  printf("//     name      = the name of the property\n");
  printf("//     id        = the type name of the property [*]\n");
  printf("//     value     = the value of the property [* - maybe]\n");
  printf("//\n");
  printf("// AAF_LITERAL_AUID(l, w1, w2,  b1, b2, b3, b4, b5, b6, b7, b8)\n");
  printf("//\n");
  printf("//   Define an AUID.\n");
  printf("//\n");
  printf("//     l         = Data1\n");
  printf("//     w1        = Data2\n");
  printf("//     w2        = Data3\n");
  printf("//     b1-b8     = Data4\n");
  printf("//\n");
  printf("\n");
  printf("// Default empty definitions so that you only have to define\n");
  printf("// those macros you actually want to use.\n");
  printf("//\n");
  printf("#ifndef AAF_TABLE_BEGIN\n");
  printf("#define AAF_TABLE_BEGIN()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TABLE_END\n");
  printf("#define AAF_TABLE_END()\n");
  printf("#endif\n");
  printf("\n");
  printf("// default definition of AAF_SYM for backwards compatibility\n");
  printf("#ifndef AAF_SYM\n");
  printf("#define AAF_SYM( symbol, name, alias, description ) #symbol\n");
  printf("#endif\n");
  printf("\n");
  printf("// why 2 step? see Steve Summit (1996) \"C Programming FAQs: Frequently Asked Questions\" ISBN: 0-201-84519-9\n");
  printf("#ifndef AAF_SYMBOL\n");
  printf("#define AAF_SYMBOL( a,b,c,d ) AAF_SYM( a,b,c,d )\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_CLASS\n");
  printf("#define AAF_CLASS(name, id, parent, concrete)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_CLASS_END\n");
  printf("#define AAF_CLASS_END(name, id, parent, concrete)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_CLASS_SEPARATOR\n");
  printf("#define AAF_CLASS_SEPARATOR()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_PROPERTY\n");
  printf("#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE\n");
  printf("#define AAF_TYPE(type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_REFERENCE_TYPE\n");
  printf("#define AAF_REFERENCE_TYPE(type, target)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_REFERENCE_TYPE_NAME\n");
  printf("#define AAF_REFERENCE_TYPE_NAME(type, target)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_TABLE_BEGIN\n");
  printf("#define AAF_TYPE_TABLE_BEGIN()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_TABLE_END\n");
  printf("#define AAF_TYPE_TABLE_END()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_INTEGER\n");
  printf("#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_ENUMERATION\n");
  printf("#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_ENUMERATION_MEMBER\n");
  printf("#define AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(name, value, container)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_ENUMERATION_END\n");
  printf("#define AAF_TYPE_DEFINITION_ENUMERATION_END(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_RECORD\n");
  printf("#define AAF_TYPE_DEFINITION_RECORD(name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_RECORD_FIELD\n");
  printf("#define AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, parent)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_RECORD_END\n");
  printf("#define AAF_TYPE_DEFINITION_RECORD_END(name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_VARYING_ARRAY\n");
  printf("#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_FIXED_ARRAY\n");
  printf("#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_RENAME\n");
  printf("#define AAF_TYPE_DEFINITION_RENAME(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_STRING\n");
  printf("#define AAF_TYPE_DEFINITION_STRING(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION\n");
  printf("#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER\n");
  printf("#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, guid, container)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END\n");
  printf("#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_CHARACTER\n");
  printf("#define AAF_TYPE_DEFINITION_CHARACTER(name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_INDIRECT\n");
  printf("#define AAF_TYPE_DEFINITION_INDIRECT(name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_OPAQUE\n");
  printf("#define AAF_TYPE_DEFINITION_OPAQUE(name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_SET\n");
  printf("#define AAF_TYPE_DEFINITION_SET(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_STRONG_REFERENCE\n");
  printf("#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET\n");
  printf("#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR\n");
  printf("#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE\n");
  printf("#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER\n");
  printf("#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(name, parent, container)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE_END\n");
  printf("#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET\n");
  printf("#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR\n");
  printf("#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_STREAM\n");
  printf("#define AAF_TYPE_DEFINITION_STREAM(name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_SEPARATOR\n");
  printf("#define AAF_TYPE_SEPARATOR()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_ALIAS_TABLE_BEGIN\n");
  printf("#define AAF_ALIAS_TABLE_BEGIN()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_ALIAS_TABLE_END\n");
  printf("#define AAF_ALIAS_TABLE_END()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_ALIAS_SEPARATOR\n");
  printf("#define AAF_ALIAS_SEPARATOR()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_CLASS_ALIAS\n");
  printf("#define AAF_CLASS_ALIAS(name, alias)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE_TABLE_BEGIN\n");
  printf("#define AAF_INSTANCE_TABLE_BEGIN()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE_TABLE_END\n");
  printf("#define AAF_INSTANCE_TABLE_END()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE_GROUP_SEPARATOR\n");
  printf("#define AAF_INSTANCE_GROUP_SEPARATOR()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE_GROUP\n");
  printf("#define AAF_INSTANCE_GROUP(name, target)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE_GROUP_END\n");
  printf("#define AAF_INSTANCE_GROUP_END(name, target)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE_SEPARATOR\n");
  printf("#define AAF_INSTANCE_SEPARATOR()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE\n");
  printf("#define AAF_INSTANCE(class, name, id, desc)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE_END\n");
  printf("#define AAF_INSTANCE_END(class, name, id)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_INSTANCE_PROPERTY\n");
  printf("#define AAF_INSTANCE_PROPERTY(name, type, value)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_LITERAL_AUID\n");
  printf("#define AAF_LITERAL_AUID(l, w1, w2,  b1, b2, b3, b4, b5, b6, b7, b8) \\\n");
  printf("                        {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}\n");
  printf("#endif\n");
  printf("\n");
  printf("// AAF Object model\n");
  printf("//\n");
  printf("\n");
  printf("AAF_TABLE_BEGIN()\n");
  #
  # Initialize global variables.
  #
  firstType = 1;
  firstAlias = 1;
  firstInstance = -1; # set to 1 by Aliases
}

/^"*[#_]fields/ {
	# set up indirect input column mapping
	# line beginning #fields is legacy, line beginning _fields is now preferred 
	# e.g. if 18th input column == "r_sym", Col["r_sym"]==18 and CC["r_sym"]==$18

    # set erroneous column number for the source of symbols
	Col[SYM] = -1;

	for( i=1; i<=NF; i++)
	{
		if( stripquotes($i) != "" )
		{
			C[ i ] = stripquotes($i);
			Col[ stripquotes($i) ] = i;
		}
		else
			C[ i ]= -1;
	}

	processedfields=1;
	next
}

/^##/ {
  next # Discard lines beginning ##
}

{
	# strip fields that are quoted strings
	for( i=1; i<=NF; i++)
	{
		if( C[i] != "" ) CC[ C[i] ] = stripquotes($i);
	}

  # Discard Node lines (not used by MetaDictionary.h)
  if( CC["r_nest"] == "" || tolower(CC["r_nest"]) == "node" || tolower(CC["r_nest"]) == "trunk" ) next 

  # Discard lines with no symbol field
  if (CC[SYM] == "") next 

  # If no APP is specified, discard lines beginning #
  if( APP == "" && 1 == index(stripquotes($1),"#") ) next

  # Discard lines that don't apply to the specified application
  if( APP != "" && 0 == index(CC["r_app"], APP) ) next 

	# Discard lines that don't apply to the specified application version
	# allow to specify an application version on the command line:  -v APPVER="1.2"
	# interprets contents of r_app of the form "APP>=1.2" correctly
	if( APPVER!="" )
	{
		# parse appver x.y to tgtver
		# allow 1.1.2, written 1.12
		# assert( 3>=length(APPVER) )
		tgtver = 100*substr(APPVER,1,1)+10*substr(APPVER,3,1);
		if( length(APPVER)>3 ) tgtver = tgtver + substr(APPVER,4,1);

		# substr from r_app
		ver = substr( CC["r_app"], index(CC["r_app"], APP)+length(APP) );

		# evaluate relation
		if( substr(ver,2,1)=="=" ) rel=substr(ver,1,2);
		else if( substr(ver,1,1)=="<" || substr(ver,1,1)==">" ) rel = substr(ver,1,1);
		else rel = "";

		# trim relation from ver
		ver=substr(ver,1+length(rel));
		
		# trim from first space
		if( 0 != index(ver," ") ) ver = substr( ver, 1, index(ver," ")-1 );

		# if no valid relation, never discard
		if( rel!="" )
		{
			# allow 1.1.2, written 1.12
			# assert 3>=length(ver)
			relver = 100*substr(ver,1,1)+substr(ver,3,1);
			if( length(ver)>3 ) relver = relver + substr(ver,4,1);

			# diagnostics
			# printf( "%d %s %d \n", tgtver, rel, relver);

			# evaluate condition and skip if false
			if( rel== "<=" &&!( tgtver <= relver )) next;
			if( rel== "<"  &&!( tgtver <  relver )) next;
			if( rel== "==" &&!( tgtver == relver )) next;
			if( rel== "!=" &&!( tgtver != relver )) next;
			if( rel== ">=" &&!( tgtver >= relver )) next;
			if( rel== ">"  &&!( tgtver >  relver )) next;
		}
	}


  # Fields must be set up before processing begins
  if( !processedfields ) {
      printError(sprintf("File generation failed: no #fields record.\n", errors));
      exit 1
  }

  # Diagnostics
  # printf("// <%s> \n", CC["s_parent_sym"]);

  # Elements Register
  if( CC["r_reg"] == "Elements" ) {

    next # not emitting any macros for Elements yet

  # Groups Register (SMPTE name for Classes)
  } else if( CC["r_reg"] == "Groups" ){

    if (CC["s_type_sym"] == "AAFClass" || CC["s_type_sym"] == "Class" || CC["s_type_sym"] == "Group") { # This item is a class

	  # "Class" allowed for backward compatibility; delete when old metadict.xls purged
	  # "Group" allowed so headers may be generated for classes not derived from AAF
	  #		e.g. ASPA::TimestampedKLV

	  # assert( tolower(CC["r_nest"]) == "leaf" )
      if (CC[SYM] != class) { # This is a new class
        if (class != "" ) {
          # end the old one
          printf("AAF_CLASS_END(%s,%s,\n  %s,\n  %s)\n",
                 class, cguid, parent, concrete);
          printf("AAF_CLASS_SEPARATOR()\n");
		  # use alias
		  parent = getAlias( ALIAS, APP, APPVER, CC["q_parent_app"], CC["s_parent_sym"] );
          if( parent == "" ) parent = "Root";
        } else {
          parent = "Root"
        }
        
        # use alias
		class = getAlias( ALIAS, APP, APPVER, CC["g_app"], CC[SYM] );
        
        class_s = CC["s_sym"];
        
        printf("\n");
        printf("// %s\n", class);
        printf("//\n");
      }
      if (CC["r_minOccurs"] != "") {
        classError(class, Col["r_minOccurs"]);
        errors++;
      }
      if (CC["s_target_sym"] != "") {
        classError(class, Col["s_target_sym"]);
        errors++;
      }
      
      concrete = "true";
      if (CC["r_isAbstract"] == "abstract") {
        concrete = "false";
      } else if (CC["r_isAbstract"] == "") {
        concrete = "true";
      } else {
        classError(class, Col["r_isAbstract"]);
        errors++;
      }
      
      # AAF_CLASS(name, id, parent)
      
      # the registry may contain ul_6 = 0x53, or 0x7f...needs to be changed to 0x06 for AAF
      syntax = "06";
      
      cguid = formatAUID(CC["ul_1"], CC["ul_2"], CC["ul_3"], CC["ul_4"],
                         CC["ul_5"], syntax, CC["ul_7"], CC["ul_8"],
                         CC["ul_9"], CC["ul_10"], CC["ul_11"], CC["ul_12"],
                         CC["ul_13"], CC["ul_14"], CC["ul_15"], CC["ul_16"], "  ");
      printf("AAF_CLASS(%s,%s,\n  %s,\n  %s)\n",
             class, cguid, parent, concrete);

    } else { # this item is a property

	  # assert( tolower(CC["r_nest"]) == "child" )

	  # use alias
	  type = getAlias( ALIAS, APP, APPVER, CC["q_type_app"], CC["s_type_sym"] );

      # match reference types and separate out the target type
      target_type = type;
      if        ( 1 == sub("^WeakReferenceVector", "", target_type) ) {
        reftype = "WEAK_REFERENCE_VECTOR";
      } else if ( 1 == sub("^WeakReferenceSet", "", target_type) ) {
        reftype = "WEAK_REFERENCE_SET";
      } else if ( 1 == sub("^WeakReference", "", target_type) ) {
        reftype = "WEAK_REFERENCE";
      } else if ( 1 == sub("^StrongReferenceVector", "", target_type) ) {
        reftype = "STRONG_REFERENCE_VECTOR";
      } else if ( 1 == sub("^StrongReferenceSet", "", target_type) ) {
        reftype = "STRONG_REFERENCE_SET";
      } else if ( 1 == sub("^StrongReference", "", target_type) ) {
        reftype = "STRONG_REFERENCE";
      } else {
        reftype="";
      }
	  # if reftype, target_type = just the target

      if (reftype != "") {
        reference_type=type;
        sub(target_type, "", reference_type);
		# reference_type = the prefix

        type = sprintf("AAF_REFERENCE_TYPE(%s, %s)", reference_type, target_type);
      } else {
        if (CC["s_target_sym"] == "") {
          type = sprintf("AAF_TYPE(%s)", type);
        } else {
          propertyError(CC[SYM], class, Col["s_target_sym"]);
          errors++;
        }
      }

      if (CC["r_minOccurs"] == "1") {
        mandatory = "true";
      } else if (CC["r_minOccurs"] == "0" || CC["r_minOccurs"] == "" ) {
        mandatory = "false";
      } else {
        propertyError(CC[SYM], class, Col["r_minOccurs"]);
        errors++;
      }
		  # use alias
		  parentsym = getAlias( ALIAS, APP, APPVER, CC["q_parent_app"], CC["s_parent_sym"] );

      if ( parentsym != class ) {
        propertyError(CC[SYM], class, Col["s_parent_sym"]);
        errors++;
      }
      if (CC["r_isAbstract"] != "") {
        propertyError(CC[SYM], class, Col["r_isAbstract"]);
        errors++;
      }
      if (CC["r_isUnique"] == "uid") {
        uid = "true";
        if (CC["r_minOccurs"] == "O") {
          propertyError(CC[SYM], class, Col["r_isUnique"]);
          printError("A unique identifier must be mandatory.\n");
          errors++;
        }
      } else {
        uid = "false";
      }
      # AAF_PROPERTY(name, id, tag, type, mandatory, container)
      pguid = formatAUID(CC["ul_1"], CC["ul_2"], CC["ul_3"], CC["ul_4"],
                         CC["ul_5"], CC["ul_6"], CC["ul_7"], CC["ul_8"],
                         CC["ul_9"], CC["ul_10"], CC["ul_11"], CC["ul_12"],
                         CC["ul_13"], CC["ul_14"], CC["ul_15"], CC["ul_16"], "    ");
	  ppid = formatTag( CC["r_tag"] );
      if (ppid == "0x0000") {
        pcomment = " // dynamic";
      } else {
				pcomment = "";
			}

			# PNT 2007-07-19
			# These four props are defined as optional by SMPTE, and will need to be defined
			# optional in AAF post-v1.1.2. However, in v1.1.2, we declare them as required
			# in the same way they have been in previous toolkits. This hack should be removed
			# when the toolkit is tested to work correctly with these properties as optional.			
			if (ppid == "0x0601" || ppid == "0x2702" || ppid == "0x3001" || ppid == "0x3002") {
				mandatory = "true";
				mcomment = " // mkdict.awk override";
			} else {
				mcomment = "";
			}

      # use alias
	  prop = getAlias( ALIAS, APP, APPVER, CC["g_app"], CC[SYM] );
      
      printf("  AAF_PROPERTY(%s,%s,\n    %s,%s\n    %s,\n    %s,%s\n    %s,\n    %s)\n", prop, pguid, ppid, pcomment, type, mandatory, mcomment, uid, class);
    }

  # Types Register
  } else if( CC["r_reg"] == "Types" ) {

    if (CC["s_type_sym"] == "type" ) { # a type

	  # assert( tolower(CC["r_nest"]) == "leaf" )
      if (firstType) {
        printf("AAF_CLASS_END(%s,%s,\n  %s,\n  %s)\n",
               class, cguid, parent, concrete);
        printf("\n");
        printf("AAF_TABLE_END()\n");
        printf("\n");
        printf("// Types\n");
        printf("//\n");
        printf("\n");
        printf("AAF_TYPE_TABLE_BEGIN()\n");
        firstType = 0;
      } else {
        # end the previous type
        if (kind == "enumeration" ) {
          printf("AAF_TYPE_DEFINITION_ENUMERATION_END(%s, %s, AAF_TYPE(%s))\n", typeName, tguid, etype);
        } else if (kind == "record") {
          printf("AAF_TYPE_DEFINITION_RECORD_END(%s, %s)\n", typeName, tguid);
        } else if (kind == "extendible") {
          printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(%s, %s)\n", typeName, tguid);
        } else if ((kind == "reference") && (qualif == "weak")) {
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);
        }

        if( kind != "formal" ) printf("AAF_TYPE_SEPARATOR()\n");
      }

      tguid = formatAUID(CC["ul_1"], CC["ul_2"], CC["ul_3"], CC["ul_4"],
                         CC["ul_5"], CC["ul_6"], CC["ul_7"], CC["ul_8"],
                         CC["ul_9"], CC["ul_10"], CC["ul_11"], CC["ul_12"],
                         CC["ul_13"], CC["ul_14"], CC["ul_15"], CC["ul_16"], "  ");

      # set kind and qualifier for use by child members
	  kind = CC["r_kind"];
      qualif = CC["r_qualif"];

      # use alias
	  typeName = getAlias( ALIAS, APP, APPVER, CC["g_app"], CC[SYM] );

      # diagnostic comments
      printf("\n");
      if ((qualif == "weak") || (qualif == "strong"))
        printf("// %s<%s>\n", referenceTypeName( kind, qualif ), getAlias( ALIAS, APP, APPVER, CC["q_target_app"], CC["s_target_sym"] ));
	  else
        printf("// %s\n", typeName);
      printf("//\n");

      if        (kind == "formal") {
	    # formal kind is just there as a target - nothing to emit
		printf("// formal type\n");
      } else if (kind == "integer") {
        printf("AAF_TYPE_DEFINITION_INTEGER(%s, %s, %s, %s)\n", typeName, tguid, qualif, CC["r_value"]);

      } else if (kind == "array") {
        # use alias
		elementType = getAlias( ALIAS, APP, APPVER, CC["q_target_app"], CC["s_target_sym"] );
        if (qualif == "varying") {
          printf("AAF_TYPE_DEFINITION_VARYING_ARRAY(%s, %s,\n  AAF_TYPE(%s))\n", typeName, tguid, elementType);

        } else if (qualif == "fixed") {
          printf("AAF_TYPE_DEFINITION_FIXED_ARRAY(%s, %s,\n  AAF_TYPE(%s), %s)\n", typeName, tguid, elementType, CC["r_minOccurs"]);

        } else if (qualif == "strong") {
          # Special cases for strong reference vectors.
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);

        } else if (qualif == "weak") {
          # Special cases for weak reference vectors.
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);

        } else {
          typeError(typeName, Col["r_qualif"]);
          errors++;
        }

      } else if (kind == "set") {
        # use alias
		elementType = getAlias( ALIAS, APP, APPVER, CC["q_target_app"], CC["s_target_sym"] );
        # Special cases for strong/weak reference sets.
        if (qualif == "strong") {
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);

        } else if (qualif == "weak") {
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);

        } else if (qualif == "global") {
          printf("AAF_TYPE_DEFINITION_SET(%s, %s,\n  AAF_TYPE(%s))\n", typeName, tguid, "AUID"); #FIXME

        } else if (qualif == "") { 
          printf("AAF_TYPE_DEFINITION_SET(%s, %s,\n  AAF_TYPE(%s))\n", typeName, tguid, elementType);

        } else {
          typeError(typeName, Col["r_qualif"]);
          errors++;
        }

      } else if (kind == "reference") {
        # use alias
		targetType = getAlias( ALIAS, APP, APPVER, CC["q_target_app"], CC["s_target_sym"] );
		
        if (qualif == "strong") {
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);

        } else if (qualif == "weak" ) {
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);

        } else {
          typeError(typeName, Col["r_qualif"]);
          errors++;
        }

      } else if (kind == "enumeration" ) {
        etype = CC["s_target_sym"];
        printf("AAF_TYPE_DEFINITION_ENUMERATION(%s, %s, AAF_TYPE(%s))\n", typeName, tguid, etype);

      } else if (kind == "record") {
        printf("AAF_TYPE_DEFINITION_RECORD(%s, %s)\n", typeName, tguid)
		;
      } else if (kind == "rename") {
        # use alias
		elementType = getAlias( ALIAS, APP, APPVER, CC["q_target_app"], CC["s_target_sym"] );
        printf("AAF_TYPE_DEFINITION_RENAME(%s, %s, %s)\n", typeName, tguid, elementType);

      } else if (kind == "string") {
        # use alias
		elementType = getAlias( ALIAS, APP, APPVER, CC["q_target_app"], CC["s_target_sym"] );
        printf("AAF_TYPE_DEFINITION_STRING(%s, %s, %s)\n", typeName, tguid, elementType);

      } else if (kind == "extendible") {
        printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(%s, %s)\n", typeName, tguid);

      } else if (kind == "character") {
        printf("AAF_TYPE_DEFINITION_CHARACTER(%s, %s)\n", typeName, tguid);

      } else if (kind == "indirect") {
        printf("AAF_TYPE_DEFINITION_INDIRECT(%s, %s)\n", typeName, tguid);

      } else if (kind == "opaque") {
        printf("AAF_TYPE_DEFINITION_OPAQUE(%s, %s)\n", typeName, tguid);

      } else if (kind == "stream") {
          printf("AAF_TYPE_DEFINITION_STREAM(%s, %s)\n", typeName, tguid);

      } else {
        printError("Unknown kind \"" kind "\" of type");
	    errors++;
      }

	  # set parentTypeName for use by members
	  parentTypeName = typeName;

    } else if( tolower(CC["r_nest"]) == "child" || tolower(CC["r_nest"]) == "link") { # all "member"s of a type are child elements

      # use alias
      memberName = getAlias( ALIAS, APP, APPVER, CC["g_app"], CC[SYM] );
      
      if (kind == "enumeration" )
	  {
		if( CC["s_type_sym"]==etype || CC["s_type_sym"]=="member" ) # "member" is old-style, etype is new-style (!)
			# note concatenate with "" to force string typing and deal with vanishing 0 ?
			printf("  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(%s,\n    %s, %s)\n", memberName, CC["r_value"] "", parentTypeName);
		else
		{
			printError( "Invalid type of member of Enumeration");
			errors++;
		} 
      }
	  else if (kind == "record")
	  {
		if( CC["s_type_sym"]=="member" ) targetType=getAlias( ALIAS, APP, APPVER, CC["q_target_app"], CC["s_target_sym"] ); # "member" is old-style
		else							 targetType=getAlias( ALIAS, APP, APPVER, CC["q_type_app"], CC["s_type_sym"] );	# new-style
		 
		printf("  AAF_TYPE_DEFINITION_RECORD_FIELD(%s, AAF_TYPE(%s),\n    %s)\n", memberName, targetType, parentTypeName);
      }
	  else if (kind == "extendible")
	  {
		if( CC["s_type_sym"]==parentTypeName || CC["s_type_sym"]=="member" ) # "member" is old-style, "AUID" is new-style
		{
			targetType="AUID";		# "member" is old-style
			eguid = formatAUID(	CC["ul_1"], CC["ul_2"], CC["ul_3"], CC["ul_4"],
								CC["ul_5"], CC["ul_6"], CC["ul_7"], CC["ul_8"],
								CC["ul_9"], CC["ul_10"], CC["ul_11"], CC["ul_12"],
								CC["ul_13"], CC["ul_14"], CC["ul_15"], CC["ul_16"], "    ");

			printf("  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(%s,%s,\n    %s)\n", memberName, eguid, parentTypeName);
		}
		else
		{
			# no others allowed at present
			printError( "Invalid type of member of Extendible enumeration");
			errors++;
		}
      }
	  else if ((kind == "reference") && (qualif == "weak"))
	  {
		if( CC["s_type_sym"]=="member" ) # "member" is old-style, no new-style yet
		{
			# use alias
			targetName = getAlias( ALIAS, APP, APPVER, CC["q_target_app"], CC["s_target_sym"] );

			# legacy csv used s_parent_sym for WR member			
			if( targetName == "" ) targetName=CC["s_parent_sym"];
      
			printf("  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(%s, %s,\n    AAF_REFERENCE_TYPE_NAME(%s, %s))\n", memberName, targetName, typeName, targetType);
		}
		else
		{
			# no others allowed at present
			printError( "Invalid type of member of weak reference");
			errors++;
		}
      }
	  else
	  {
			printError("Child member of unknown kind of type");
			errors++;
      }

	} else {
        printError("Unknown child of type");
	    errors++;
	}

  # Aliases Register (non SMPTE)
  } else if( CC["r_reg"] == "Aliases" ) {

    if (CC["s_type_sym"] == "alias") {

	  # assert( tolower(CC["r_nest"]) == "leaf" )
      if (firstAlias) {
        if (kind == "enumeration" ) {
          printf("AAF_TYPE_DEFINITION_ENUMERATION_END(%s, %s, AAF_TYPE(%s))\n", typeName, tguid, etype);
        } else if (kind == "record") {
          printf("AAF_TYPE_DEFINITION_RECORD_END(%s, %s)\n", typeName, tguid);
        } else if (kind == "extendible") {
          printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(%s, %s)\n", typeName, tguid);
        } else if ((kind == "reference") && (qualif == "weak")) {
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);
        }

        printf("\n");
        printf("AAF_TYPE_TABLE_END()\n");
        printf("\n");
        printf("// Aliases\n");
        printf("//\n");
        printf("\n");
        printf("AAF_ALIAS_TABLE_BEGIN()\n");
        printf("\n");
        firstAlias = 0;
        firstInstance = 1;

      } else {
        printf("AAF_ALIAS_SEPARATOR()\n");
      }

	  # no change to advanced alias meechanism here - this section is moribund
      aalias = CC[SYM];
      aoriginal = CC["g_alias"];
      printf("AAF_CLASS_ALIAS(%s, %s)\n", aoriginal, aalias); 
	}

  # Labels Register
  } else if( CC["r_reg"] == "Labels" ) {

    next # not emitting any macros for Labels yet

  # Instances Register
  } else if( CC["r_reg"] == "Instances" ) {

    if ( tolower(CC["r_nest"]) == "stalk" )
    { 
        # a set of instances of a [subclass of] DefinitionObject

        if( firstInstance<0 )
        {
			if (firstAlias)
			{
				# finish previous types, no aliases
				if (kind == "enumeration" ) {
				  printf("AAF_TYPE_DEFINITION_ENUMERATION_END(%s, %s, AAF_TYPE(%s))\n", typeName, tguid, etype);
				} else if (kind == "record") {
				  printf("AAF_TYPE_DEFINITION_RECORD_END(%s, %s)\n", typeName, tguid);
				} else if (kind == "extendible") {
				  printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(%s, %s)\n", typeName, tguid);
				} else if ((kind == "reference") && (qualif == "weak")) {
				  printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);
				}
				printf("AAF_TYPE_TABLE_END()\n");
				printf("\n");
				printf("// Instances\n");
				printf("//\n");
				printf("\n");
				printf("AAF_INSTANCE_TABLE_BEGIN()\n");
				printf("\n");
				firstAlias = 0;
				firstInstance = 1;
			}
			else
			{
				printError("Instances in wrong context");
				errors++;
			}
            firstInstance = 0;
        }
        else if( firstInstance>0 )
        {
            # finish previous aliases
            printf("\n");
            printf("AAF_ALIAS_TABLE_END()\n");
            printf("\n");
            printf("// Instances\n");
            printf("//\n");
            printf("\n");
            printf("AAF_INSTANCE_TABLE_BEGIN()\n");
            printf("\n");
            firstInstance = 0;
            igsym = "";
            isym = "";
        }
        else # firstInstance==0 
        {
            # finish previous instance
            if( isym!="" )
            {
                printf("  AAF_INSTANCE_END(%s, %s, %s)\n", iclass, asym, iid);
            }

            # finish previous instance definition group
            if( igsym!="" )
            {
                printf("AAF_INSTANCE_GROUP_END(%s, %s)\n", igsym, igtarget);
            }
            else
            {
                printError("no instances encountered in previous Instance Group");
	            errors++;
            }
            printf("AAF_INSTANCE_GROUP_SEPARATOR()\n");
        }

        igsym = CC[SYM];
        if( igsym=="" ) nullError( "r_sym", "Instance" );

        igtarget = CC["s_target_sym"];
        if( igtarget=="" ) nullError( "s_target_sym", "Instance" );

        printf("AAF_INSTANCE_GROUP(%s, %s)\n", igsym, igtarget);

        isym = "";
    }
    else
    {
      # an instance of either a [subclass of] DefinitionObject or a property of the subclass

       if( tolower(CC["r_nest"]) == "leaf" || tolower(CC["r_nest"]) == "frond" )
       { 
            # an instance of a [subclass of] DefinitionObject

            # finish previous instance
            if( isym!="" )
            {
                printf("  AAF_INSTANCE_END(%s, %s, %s)\n", iclass, asym, iid);
                printf("  AAF_INSTANCE_SEPARATOR()\n");
            }

            isym = CC[SYM];

            iid = formatAUID(CC["ul_1"], CC["ul_2"], CC["ul_3"], CC["ul_4"],
                             CC["ul_5"], CC["ul_6"], CC["ul_7"], CC["ul_8"],
                             CC["ul_9"], CC["ul_10"], CC["ul_11"], CC["ul_12"],
                             CC["ul_13"], CC["ul_14"], CC["ul_15"], CC["ul_16"], "      ");

            iclass = CC["s_type_sym"];
            idesc = CC["r_detail"];
            iname= CC["r_name"];
			# For instances, want getAlias to return "" not CC[SYM] if there is no alias PNT 2007-07-18 
			ialias = getAlias( ALIAS, APP, APPVER, CC["g_app"], "" );

			asym = "AAF_SYMBOL(" isym "," iname ",\"" ialias "\",\"" idesc "\")"

            printf("  AAF_INSTANCE(%s, %s, %s, %s)\n", iclass, asym, iid, "\"" idesc "\"");

            # basic properties of DefinitionObject
            printf("    AAF_INSTANCE_PROPERTY(%s, %s, %s)\n", "Name", "String", "\"" isym "\"");

            if( idesc != "" ) printf("    AAF_INSTANCE_PROPERTY(%s, %s, %s)\n", "Description", "String", "\"" idesc "\"");

            printf("    AAF_INSTANCE_PROPERTY(%s, %s, %s)\n", "Identification", "AUID", iid);

	   }
       else if( tolower(CC["r_nest"]) == "child" || tolower(CC["r_nest"]) == "stipe" )
       {
            # a property of the subclass

            ipsym = CC[SYM];
            iptype = CC["s_type_sym"];
            ipvalue = CC["r_value"];
            printf("    AAF_INSTANCE_PROPERTY(%s, %s, %s)\n", ipsym, iptype, "\"" ipvalue "\"");
       }
       else
       {
            printError("Unknown child of Instances");
	        errors++;
       }
	}

  # Unknown Register
  } else {
      printError("Unknown Register");
	  errors++;
  }
}

END {
  if (errors == 0 ){

        # finish last instance definition group
        if( firstInstance==0 )
        {
            # finish last instance
            if( isym!="" )
            {
                printf("  AAF_INSTANCE_END(%s, %s, %s)\n", iclass, asym, iid);
            }

            # finish last instance definition group
            if( igsym!="" && igtarget!="" )
            {
                printf("AAF_INSTANCE_GROUP_END(%s, %s)\n", igsym, igtarget);
            }
            else
            {
                printError("no instances encountered in previous Instance Group");
	            errors++;
            }
        }

        printf("\n");
        printf("AAF_INSTANCE_TABLE_END()\n");
        printf("\n");

    printf("// Undefine all macros\n");
    printf("//\n");
    printf("#undef AAF_TABLE_BEGIN\n");
    printf("\n");
    printf("#undef AAF_TABLE_END\n");
    printf("\n");
    printf("#undef AAF_SYM\n");
    printf("\n");
    printf("#undef AAF_SYMBOL\n");
    printf("\n");
    printf("#undef AAF_CLASS\n");
    printf("\n");
    printf("#undef AAF_CLASS_END\n");
    printf("\n");
    printf("#undef AAF_CLASS_SEPARATOR\n");
    printf("\n");
    printf("#undef AAF_PROPERTY\n");
    printf("\n");
    printf("#undef AAF_TYPE\n");
    printf("\n");
    printf("#undef AAF_REFERENCE_TYPE\n");
    printf("\n");
    printf("#undef AAF_REFERENCE_TYPE_NAME\n");
    printf("\n");
    printf("#undef AAF_LITERAL_AUID\n");
    printf("\n");
    printf("#undef AAF_TYPE_TABLE_BEGIN\n");
    printf("\n");
    printf("#undef AAF_TYPE_TABLE_END\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_INTEGER\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_ENUMERATION\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_ENUMERATION_MEMBER\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_ENUMERATION_END\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_RECORD\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_RECORD_FIELD\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_RECORD_END\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_VARYING_ARRAY\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_FIXED_ARRAY\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_RENAME\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_STRING\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_CHARACTER\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_INDIRECT\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_OPAQUE\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_SET\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_STRONG_REFERENCE\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_END\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_STREAM\n");
    printf("\n");
    printf("#undef AAF_TYPE_SEPARATOR\n");
    printf("\n");
    printf("#undef AAF_ALIAS_TABLE_BEGIN\n");
    printf("\n");
    printf("#undef AAF_ALIAS_TABLE_END\n");
    printf("\n");
    printf("#undef AAF_ALIAS_SEPARATOR\n");
    printf("\n");
    printf("#undef AAF_CLASS_ALIAS\n");
    printf("\n");
    printf("#undef AAF_INSTANCE_TABLE_BEGIN\n");
    printf("\n");
    printf("#undef AAF_INSTANCE_TABLE_END\n");
    printf("\n");
    printf("#undef AAF_INSTANCE_GROUP_SEPARATOR\n");
    printf("\n");
    printf("#undef AAF_INSTANCE_GROUP\n");
    printf("\n");
    printf("#undef AAF_INSTANCE_GROUP_END\n");
    printf("\n");
    printf("#undef AAF_INSTANCE_SEPARATOR\n");
    printf("\n");
    printf("#undef AAF_INSTANCE\n");
    printf("\n");
    printf("#undef AAF_INSTANCE_END\n");
    printf("\n");
    printf("#undef AAF_INSTANCE_PROPERTY\n");
    printf("\n");
    printf("#undef AAF_LITERAL_AUID\n");
    printf("\n");
    printf("//\n");
    printf("// End of automatically generated file.\n");
  } else {
    printError(sprintf("File generation failed with %d errors.\n", errors));
    exit 1
  }
}

function formatAUID(O01, O02, O03, O04, O05, O06, O07, O08, 
                    O09, O10, O11, O12, O13, O14, O15, O16,
                    indent)
{
  a01 = formatByte(O01);
  a02 = formatByte(O02);
  a03 = formatByte(O03);
  a04 = formatByte(O04);
  a05 = formatByte(O05);
  a06 = formatByte(O06);
  a07 = formatByte(O07);
  a08 = formatByte(O08);
  a09 = formatByte(O09);
  a10 = formatByte(O10);
  a11 = formatByte(O11);
  a12 = formatByte(O12);
  a13 = formatByte(O13);
  a14 = formatByte(O14);
  a15 = formatByte(O15);
  a16 = formatByte(O16);

  s = formatAUIS(a01, a02, a03, a04, a05, a06, a07, a08,
                 a09, a10, a11, a12, a13, a14, a15, a16);
  l = formatAsLabel(a01, a02, a03, a04, a05, a06, a07, a08,
                    a09, a10, a11, a12, a13, a14, a15, a16);
  return sprintf("\n%s// %s\n%s// %s\n%sAAF_LITERAL_AUID(0x%s%s%s%s,\n%s  0x%s%s, 0x%s%s,\n%s  0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s)",
             indent, s, indent,
             l, indent,
             a09, a10, a11, a12, indent,
             a13, a14, a15, a16, indent,
             a01, a02, a03, a04,
             a05, a06, a07, a08);
}

function formatAUIS(O01, O02, O03, O04, O05, O06, O07, O08,
                    O09, O10, O11, O12, O13, O14, O15, O16)
{
  return sprintf("{%s%s%s%s-%s%s-%s%s-%s%s-%s%s%s%s%s%s}",
             O09, O10, O11, O12,
             O13, O14, O15, O16,
             O01, O02, O03, O04,
             O05, O06, O07, O08);
}

function formatAsLabel(O01, O02, O03, O04, O05, O06, O07, O08,
                       O09, O10, O11, O12, O13, O14, O15, O16)
{
  if ((O01 == "06") && (O02 == "0E") && (O03 == "2B") && (O04 == "34") && (O05 == "02") && (O06 == "06"))
    L06 = 53;
  else
    L06= O06;
    
  return sprintf("%s.%s.%s.%s.%s.%s.%s.%s.%s.%s.%s.%s.%s.%s.%s.%s",
             O01, O02, O03, O04,
             O05, L06, O07, O08,
             O09, O10, O11, O12,
             O13, O14, O15, O16);
}

function formatByte( str )
{
  while( length(str)<2 ) str = "0" str;
  str = toupper( substr( str,1,2) );
  return str;
}

function formatTag( tag )
{
  while( length(tag)<8 ) tag= "0" tag;
  tag = tolower( substr( tag,5,4 ) );
  return sprintf( "0x%s", tag );
}

# unused format function - r_tag now delivered in spreadsheet
function formatPID( t3, t4 )
{
  while( length(t3)<2 ) t3 = "0" t3;
  t3 = tolower( substr( t3,1,2) );
  while( length(t4)<2 ) t4 = "0" t4;
  t4 = tolower( substr( t4,1,2) );
  return sprintf( "0x%s%s",t3,t4 );

}

function referenceTypeName( kind, qualif )
{
         if( kind == "array" ){
			     if( qualif == "strong" )	return "StrongReferenceVector";
			else if( qualif == "weak" )		return "WeakReferenceVector";
			else							return "ERROR";			 
  } else if( kind == "set" ){
			     if( qualif == "strong" )	return "StrongReferenceSet";
			else if( qualif == "weak" )		return "WeakReferenceSet";
			else							return "ERROR";			 
  } else if( kind == "reference" ){
			     if( qualif == "strong" )	return "StrongReference";
			else if( qualif == "weak" )		return "WeakReference";
			else							return "ERROR";			 
  } else {
											return "ERROR";
  }
}

function getAlias( alias, app, appver, appsource, appdefault )
{
	# split into different apps
	nApp = split( appsource, apps, " " );

	# currently doesn't check app, just uses the first one
	nAlias = split( apps[1], appfields, "~" );

	if( nAlias < 2 ) ret = "" ; # no aliases are given
	else
	{
		# merely return the alias for the first app on the line
		ret = appfields[ nAlias ] ;
	}
	
	if( ALIAS == "" || ret== "" ) return appdefault; else return ret;
	
	# really, should:
	# split on space
	# split on ~
	# split on [
	# check app and appver
	# return associated alias
}

function printError(message)
{
  printf("#error %s\n", message);
  printf("Error : %s\n", message) | "cat 1>&2";
}

function elementError(column, elementName)
{
  printError(sprintf("Illegal entry \"%s\" in column \"%s\", for %s.\n", $column, column, elementName));
}

function elError(column, elementName)
{
  printError(sprintf("Illegal entry \"%s\" in column \"%s\", for %s.\n", CC[column], column, elementName));
}

function nullError(column, elementName)
{
  printError(sprintf("Illegal null value in column \"%s\", for %s.\n", column, elementName));
}

function classError(class, column)
{
  elementError(column , sprintf("class \"%s\"", class));
}

function typeError(type, column)
{
  elementError(column , sprintf("type \"%s\"", type));
}

function propertyError(property, class, column)
{
  elementError(column, sprintf("property \"%s\" of class \"%s\"", property, class));
}

function getUser()
{
  result = "unknown"
  "echo $USER" | getline user
  if (user != "") {
    result = user
  } else {
    "echo $USERNAME" | getline user 
    if (user != ""){
      result = user
    }
  }
  return result
}

function stripquotes(input)
{
	if( substr(input,1,1) == "\"" ) return substr(input,2,length(input)-2);
	else return input;
}
