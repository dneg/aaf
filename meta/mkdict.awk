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
# Generate an AAF meta dictionary from a .csv spreadsheet.
#
# Command line:
#
# awk -f mkdict.awk AAFMetaDict.csv > AAFMetaDictionary.h
#
# Authors
#
#   Tim Bingham tim_bingham@avid.com - original
#   Oliver Morgan oliver@metaglue.com - extensive revisions for new features
#
# This program uses the following spreadsheet columns for the
# purposes indicated ...
#
#      Column                          Purpose
#
# registerC               => which of the SMPTE registers this belongs in Groups Types Labels
#                            (and Aliases - non SMPTE)
# nodeleafC               => wheteher this line will be a Node L:eaf or Child in SMPTE
# verC                    => (calculated in spreadsheet, ignored) SLO7C for ULs
# levelC                  => (calculated in spreadsheet, ignored) number of used SLO8C-SLO15C for UL
# SLO00C .. SLO15C        => AUID (with appropriate reordering).
# elementNameC            => class/property name.
# typeNameC               => if this field == "class" then the entity is a
#                            class, otherwise it is a property and this field
#                            denotes the type of the property. When this
#                            field == "class" this row is taken to be the
#                            start of a class definition. All the following
#                            rows (until the start of the next class) are
#                            taken to be properties of this class.
# kindC                   => For enumeration members the value of the member
#                            For record members the name of the member
# qualifC                 => For enumerations the type of every member
#                            For arrays indicates whether the array is fixed
#                            or varying
#                            For references indicates whether the reference
#                            is strong or weak.
# elementTypeC            => For arrays the element type.
#                            For references (singleton, array and set) the
#                            type of the target.
# isMandatoryC            => if this field == "M" then this is a required
#                            property. If this field is "O" then this is an
#                            optional property.
# parentC                 => if this entity is a class then this field is
#                            the immediate ancestor (otherwise the field
#                            is checked to see that it is the same as the
#                            current class).
# tag1C                   => property id most significant byte (for compatibility with 4 byte tags), ignored
# tag2C                   => property id most significant byte (for compatibility with 4 byte tags), ignored
# tag3C                   => property id upper byte. used only for Properties
# tag4C                   => property id lower byte. used only for Properties
# referenceTypeC          => if this is a property and the property type
#                            ($C["typeNameC"]) is a reference then this field is
#                            the type (class) of the referenced object,
#                            otherwise this field should be empty.
# isAbstractC             => if this entity is a class and the class is
#                            abstract, this field contains "abstract" otherwise
#                            this field should be empty.
# isUidC                  => if this entity is a property and the property is
#                            unique identifier, this field contains "uid"
#                            otherwise this field should be empty.
# aliasC                  => full classname of which this is an (short) alias as used in code
#
BEGIN {
  #
  # symbolic names for columns are input from line beginning #fields
  #
  errors = 0;
  FS=","
  processedfields=0;
  #
  # Write a header to the generated file.
  #
  printf("//=---------------------------------------------------------------------=\n");
  printf("//\n");
  printf("// This file was GENERATED for the AAF SDK\n");
  printf("//\n");
  printf("// $""Id$ $""Name$\n");
  printf("//\n");
  printf("// The contents of this file are subject to the AAF SDK Public\n");
  printf("// Source License Agreement (the \"License\"); You may not use this file\n");
  printf("// except in compliance with the License.  The License is available in\n");
  printf("// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF\n");
  printf("// Association or its successor.\n");
  printf("//\n");
  printf("// Software distributed under the License is distributed on an \"AS IS\"\n");
  printf("// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See\n");
  printf("// the License for the specific language governing rights and limitations\n");
  printf("// under the License.\n");
  printf("//\n");
  printf("// The Original Code of this file is Copyright 1998-2004, Licensor of the\n");
  printf("// AAF Association.\n");
  printf("//\n");
  printf("// The Initial Developer of the Original Code of this file and the\n");
  printf("// Licensor of the AAF Association is Avid Technology.\n");
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
  "echo $USER$USERNAME" | getline user
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
  printf("//     name      = class name\n");
  printf("//     alias     = another, usually shorter, name by which the\n");
  printf("//                 class is also known\n");
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
}

/^#fields/ {
	# set up indirect input column mapping
	# e.g. if 18th input column == "elementNameC", $C["elementNameC"]==$18
	for( i=1; i<=NF; i++)
	{
		if( $i != "" ) C[ $i ] = i;
	}
	processedfields=1;
}

/^#/ {
  next # Discard the line
}

{
  # Fields must be set up before processing begins
  if( !processedfields ) {
      printError(sprintf("File generation failed: no #fields record.\n", errors));
      exit 1
  }

  # Discard lines with no name field
  if ($C["elementNameC"] == "") next 

  # Diagnostics
  # printf("// <%s> \n", $C["parentC"]);

  # Groups Register (SMPTE name for Classes)
  if( $C["registerC"] == "Groups" ){

    if ($C["typeNameC"] == "Class") { # This item is a class

	  # assert( $C["nlcC"] == "Leaf" )
      if ($C["elementNameC"] != class) { # This is a new class
        if (class != "" ) {
          # end the old one
          printf("AAF_CLASS_END(%s,%s,\n  %s,\n  %s)\n",
                 class, cguid, parent, concrete);
          printf("AAF_CLASS_SEPARATOR()\n");
          parent = $C["parentC"];
        } else {
          parent = "Root"
        }
        class = $C["elementNameC"];
        printf("\n");
        printf("// %s\n", class);
        printf("//\n");
      }
      if ($C["isMandatoryC"] != "") {
        classError(class, C["isMandatoryC"]);
        errors++;
      }
      if ($C["referenceTypeC"] != "") {
        classError(class, C["referenceTypeC"]);
        errors++;
      }
      concrete = "true";
      if ($C["isAbstractC"] == "abstract") {
        concrete = "false";
      } else if ($C["isAbstractC"] != "") {
        classError(class, C["isAbstractC"]);
        errors++;
      }
      # AAF_CLASS(name, id, parent)
      cguid = formatAUID($C["SLO00C"], $C["SLO01C"], $C["SLO02C"], $C["SLO03C"],
                         $C["SLO04C"], $C["SLO05C"], $C["SLO06C"], $C["SLO07C"],
                         $C["SLO08C"], $C["SLO09C"], $C["SLO10C"], $C["SLO11C"],
                         $C["SLO12C"], $C["SLO13C"], $C["SLO14C"], $C["SLO15C"], "  ");
      printf("AAF_CLASS(%s,%s,\n  %s,\n  %s)\n",
             $C["elementNameC"], cguid, parent, concrete);

    } else { # this item is a property

	  # assert( $C["nlcC"] == "Child" )
      type = $C["typeNameC"];
      if (type == "Array of Int32") {
        type = "Int32Array"
      } else if (type == "Array of Int64") {
        type = "Int64Array"
      }

      if        (type == "WeakReference") {
        reftype = "WEAK_REFERENCE";
      } else if (type == "WeakReferenceVector") {
        reftype = "WEAK_REFERENCE_VECTOR";
      } else if (type == "WeakReferenceSet") {
        reftype = "WEAK_REFERENCE_SET";
      } else if (type == "StrongReference") {
        reftype = "STRONG_REFERENCE";
      } else if (type == "StrongReferenceVector") {
        reftype = "STRONG_REFERENCE_VECTOR";
      } else if (type == "StrongReferenceSet") {
        reftype = "STRONG_REFERENCE_SET";
      } else {
        reftype="";
      }
      if (reftype != "") {
        if ($C["referenceTypeC"] != "") {
          # type = sprintf("AAF_%s(%s, %s)", reftype, type, $C["referenceTypeC"]);
          type = sprintf("AAF_REFERENCE_TYPE(%s, %s)", type, $C["referenceTypeC"]);
        } else {
          propertyError($C["elementNameC"], class, C["referenceTypeC"])
          errors++;
        }
      } else {
        if ($C["referenceTypeC"] == "") {
          type = sprintf("AAF_TYPE(%s)", type);
        } else {
          propertyError($C["elementNameC"], class, C["referenceTypeC"]);
          errors++;
        }
      }

      if ($C["isMandatoryC"] == "M") {
        mandatory = "true";
      } else if ($C["isMandatoryC"] == "O") {
        mandatory = "false";
      } else {
        propertyError($C["elementNameC"], class, C["isMandatoryC"]);
        errors++;
      }
      if ($C["parentC"] != class) {
        propertyError($C["elementNameC"], class, C["parentC"]);
        errors++;
      }
      if ($C["isAbstractC"] != "") {
        propertyError($C["elementNameC"], class, C["isAbstractC"]);
        errors++;
      }
      if ($C["isUidC"] == "uid") {
        uid = "true";
        if ($C["isMandatoryC"] == "O") {
          propertyError($C["elementNameC"], class, C["isUidC"]);
          printError("A unique identifier must be mandatory.\n");
          errors++;
        }
      } else {
        uid = "false";
      }
      # AAF_PROPERTY(name, id, tag, type, mandatory, container)
      pguid = formatAUID($C["SLO00C"], $C["SLO01C"], $C["SLO02C"], $C["SLO03C"],
                         $C["SLO04C"], $C["SLO05C"], $C["SLO06C"], $C["SLO07C"],
                         $C["SLO08C"], $C["SLO09C"], $C["SLO10C"], $C["SLO11C"],
                         $C["SLO12C"], $C["SLO13C"], $C["SLO14C"], $C["SLO15C"], "    ");
	  ppid = formatPID( $C["tag3C"], $C["tag4C"] );
      printf("  AAF_PROPERTY(%s,%s,\n    %s,\n    %s,\n    %s,\n    %s,\n    %s)\n", $C["elementNameC"], pguid, ppid, type, mandatory, uid, class);
    }

  # Types Register
  } else if( $C["registerC"] == "Types" ) {

    if ($C["typeNameC"] == "type" ) { # a type

	  # assert( $C["nlcC"] == "Leaf" )
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
        printf("AAF_TYPE_SEPARATOR()\n");
      }

      tguid = formatAUID($C["SLO00C"], $C["SLO01C"], $C["SLO02C"], $C["SLO03C"],
                         $C["SLO04C"], $C["SLO05C"], $C["SLO06C"], $C["SLO07C"],
                         $C["SLO08C"], $C["SLO09C"], $C["SLO10C"], $C["SLO11C"],
                         $C["SLO12C"], $C["SLO13C"], $C["SLO14C"], $C["SLO15C"], "  ");

      typeName = $C["elementNameC"];
      parentTypeName = typeName;
      printf("\n");
      printf("// %s", typeName);
      if (($C["qualifC"] == "weak") || ($C["qualifC"] == "strong")) {
        printf("<%s>\n", $C["elementTypeC"]);
      } else {
        printf("\n");
      }
      printf("//\n");
      kind = $C["kindC"];
      qualif = $C["qualifC"];
      if        (kind == "integer") {
        printf("AAF_TYPE_DEFINITION_INTEGER(%s, %s, %s, %s)\n", typeName, tguid, $C["qualifC"], $C["elementTypeC"]);
      } else if (kind == "enumeration" ) {
        etype = $C["qualifC"];
        printf("AAF_TYPE_DEFINITION_ENUMERATION(%s, %s, AAF_TYPE(%s))\n", typeName, tguid, etype);
      } else if (kind == "array") {
        elementType = $C["elementTypeC"];
        if ($C["qualifC"] == "varying") {
          printf("AAF_TYPE_DEFINITION_VARYING_ARRAY(%s, %s,\n  AAF_TYPE(%s))\n", typeName, tguid, elementType);
        } else if ($C["qualifC"] == "fixed") {
          printf("AAF_TYPE_DEFINITION_FIXED_ARRAY(%s, %s,\n  AAF_TYPE(%s), %s)\n", typeName, tguid, elementType, $C["isMandatoryC"]);
        } else if ($C["qualifC"] == "strong") {
          # Special cases for strong reference vectors.
            printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);
        } else if ($C["qualifC"] == "weak") {
          # Special cases for weak reference vectors.
            printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);
        } else {
          typeError(typeName, C["qualifC"]);
          errors++;
        }
      } else if (kind == "record") {
        printf("AAF_TYPE_DEFINITION_RECORD(%s, %s)\n", typeName, tguid);
      } else if (kind == "rename") {
        printf("AAF_TYPE_DEFINITION_RENAME(%s, %s, %s)\n", typeName, tguid, $C["elementTypeC"]);
      } else if (kind == "string") {
        printf("AAF_TYPE_DEFINITION_STRING(%s, %s, %s)\n", typeName, tguid, $C["elementTypeC"]);
      } else if (kind == "extendible") {
        printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(%s, %s)\n", typeName, tguid);
      } else if (kind == "character") {
        printf("AAF_TYPE_DEFINITION_CHARACTER(%s, %s)\n", typeName, tguid);
      } else if (kind == "indirect") {
        printf("AAF_TYPE_DEFINITION_INDIRECT(%s, %s)\n", typeName, tguid);
      } else if (kind == "opaque") {
        printf("AAF_TYPE_DEFINITION_OPAQUE(%s, %s)\n", typeName, tguid);
      } else if (kind == "set") {
        elementType = $C["elementTypeC"];
        # Special cases for strong/weak reference sets.
        if ($C["qualifC"] == "strong") {
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);
        } else if ($C["qualifC"] == "weak") {
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);
        } else { 
          printf("AAF_TYPE_DEFINITION_SET(%s, %s,\n  AAF_TYPE(%s))\n", typeName, tguid, elementType);
        }
      } else if (kind == "reference") {
        targetType = $C["elementTypeC"];
        if ($C["qualifC"] == "strong") {
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);
        } else if ($C["qualifC"] == "weak" ) {
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);
        } else {
          typeError(typeName, C["qualifC"]);
          errors++;
        }
      } else if (kind == "stream") {
          printf("AAF_TYPE_DEFINITION_STREAM(%s, %s)\n", typeName, tguid);
      } else {
        printError("Unknown kind of type");
	    errors++;
      }

    } else if ($C["typeNameC"] == "member") { # a "member" of a type

	  # assert( $C["nlcC"] == "Child" )
      memberName = $C["elementNameC"];
      if (kind == "enumeration" ) {
        printf("  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(%s,\n    %s, %s)\n", memberName, $C["kindC"], parentTypeName);
      } else if (kind == "record") {
        printf("  AAF_TYPE_DEFINITION_RECORD_FIELD(%s, AAF_TYPE(%s),\n    %s)\n", memberName, $C["kindC"], parentTypeName);
      } else if (kind == "extendible") {

      eguid = formatAUID($C["SLO00C"], $C["SLO01C"], $C["SLO02C"], $C["SLO03C"],
                         $C["SLO04C"], $C["SLO05C"], $C["SLO06C"], $C["SLO07C"],
                         $C["SLO08C"], $C["SLO09C"], $C["SLO10C"], $C["SLO11C"],
                         $C["SLO12C"], $C["SLO13C"], $C["SLO14C"], $C["SLO15C"], "    ");

        printf("  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(%s,%s,\n    %s)\n", memberName, eguid, parentTypeName);
      } else if ((kind == "reference") && (qualif == "weak")) {
        printf("  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(%s, %s,\n    AAF_REFERENCE_TYPE_NAME(%s, %s))\n", memberName, $C["parentC"], typeName, targetType);
      } else {
        printError("Member of unknown kind of type");
	    errors++;
      }

	} else {
        printError("Unknown child of type");
	    errors++;
	}

  # Aliases Register (non SMPTE)
  } else if( $C["registerC"] == "Aliases" ) {

    if ($C["typeNameC"] == "alias") {

	  # assert( $C["nlcC"] == "Leaf" )
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

      } else {
        printf("AAF_ALIAS_SEPARATOR()\n");
      }

      aalias = $C["elementNameC"];
      aoriginal = $C["aliasC"];
      printf("AAF_CLASS_ALIAS(%s, %s)\n", aoriginal, aalias); 
	}

  # Labels Register
  } else if( $C["registerC"] == "Labels" ) {

    next # not emitting any macros for Labels yet

  # Unknown Register
  } else {
      printError("Unknown Register");
	  errors++;
  }
}

END {
  if (errors == 0 ){
    printf("\n");
    printf("AAF_ALIAS_TABLE_END()\n");
    printf("\n");
    printf("// Undefine all macros\n");
    printf("//\n");
    printf("#undef AAF_TABLE_BEGIN\n");
    printf("\n");
    printf("#undef AAF_TABLE_END\n");
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
    printf("#undef AAF_LITERAL_AUID\n");
    printf("\n");
    printf("//\n");
    printf("// End of automatically generated file.\n");
  } else {
    printError(sprintf("File generation failed with %d errors.\n", errors));
    exit 1
  }
}

function formatAUID(O00, O01, O02, O03, O04, O05, O06, O07,
                    O08, O09, O10, O11, O12, O13, O14, O15,
                    indent)
{
  s = formatAUIS(O00, O01, O02, O03, O04, O05, O06, O07,
                 O08, O09, O10, O11, O12, O13, O14, O15);
  return sprintf("\n%s// %s\n%sAAF_LITERAL_AUID(0x%s%s%s%s,\n%s  0x%s%s, 0x%s%s,\n%s  0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s)",
             indent, s, indent,
             O08, O09, O10, O11, indent,
             O12, O13, O14, O15, indent,
             O00, O01, O02, O03,
             O04, O05, O06, O07);
}

function formatAUIS(O00, O01, O02, O03, O04, O05, O06, O07,
                    O08, O09, O10, O11, O12, O13, O14, O15)
{
  return sprintf("{%s%s%s%s-%s%s-%s%s-%s%s-%s%s%s%s%s%s}",
             O08, O09, O10, O11,
             O12, O13, O14, O15,
             O00, O01, O02, O03,
             O04, O05, O06, O07);
}

function formatPID( t3, t4 )
{
  while( length(t3)<2 ) t3 = "0" t3;
  t3 = tolower( substr( t3,1,2) );
  while( length(t4)<2 ) t4 = "0" t4;
  t4 = tolower( substr( t4,1,2) );
  return sprintf( "0x%s%s",t3,t4 );
}

function printError(message)
{
  printf("#error %s", message);
  printf("Error : %s", message) | "cat 1>&2";
}

function elementError(column, elementName)
{
  printError(sprintf("Illegal entry \"%s\" in column \"%s\", for %s.\n", $column, column, elementName));
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

