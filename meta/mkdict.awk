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
# note that many other columns are named, but not used by this script
#
#      Column                          Purpose
#
# r_reg					  => which of the SMPTE registers this belongs in Groups Types Labels
#                            (and Aliases - non SMPTE)
# r_nest				  => whether this line will be a Node L:eaf or Child in SMPTE
# ul_1..ul_16             => AUID (with appropriate reordering).
# r_sym                   => class/property/type/member/label/alias name.
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
#							 For arrays the element type.
#							 For properties where the property type
#                            ($C["s_type_sym"]) is a reference then this field is
#                            the type (class) of the referenced object,
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
  firstInstance = -1; // set to 1 by Aliases
}

/^#fields/ {
	# set up indirect input column mapping
	# e.g. if 18th input column == "r_sym", $C["r_sym"]==$18
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
  if ($C["r_sym"] == "") next 

  # Diagnostics
  # printf("// <%s> \n", $C["s_parent_sym"]);

  # Groups Register (SMPTE name for Classes)
  if( $C["r_reg"] == "Groups" ){

    if ($C["s_type_sym"] == "Class" || $C["s_type_sym"] == "AAFClass") { # This item is a class

	  # assert( $C["r_nest"] == "Leaf" )
      if ($C["r_sym"] != class) { # This is a new class
        if (class != "" ) {
          # end the old one
          printf("AAF_CLASS_END(%s,%s,\n  %s,\n  %s)\n",
                 class, cguid, parent, concrete);
          printf("AAF_CLASS_SEPARATOR()\n");
          parent = $C["s_parent_sym"];
        } else {
          parent = "Root"
        }
        class = $C["r_sym"];
        printf("\n");
        printf("// %s\n", class);
        printf("//\n");
      }
      if ($C["r_minOccurs"] != "") {
        classError(class, C["r_minOccurs"]);
        errors++;
      }
      if ($C["s_target_sym"] != "") {
        classError(class, C["s_target_sym"]);
        errors++;
      }
      concrete = "true";
      if ($C["r_isAbstract"] == "abstract") {
        concrete = "false";
      } else if ($C["r_isAbstract"] != "") {
        classError(class, C["r_isAbstract"]);
        errors++;
      }
      # AAF_CLASS(name, id, parent)
      cguid = formatAUID($C["ul_1"], $C["ul_2"], $C["ul_3"], $C["ul_4"],
                         $C["ul_5"], $C["ul_6"], $C["ul_7"], $C["ul_8"],
                         $C["ul_9"], $C["ul_10"], $C["ul_11"], $C["ul_12"],
                         $C["ul_13"], $C["ul_14"], $C["ul_15"], $C["ul_16"], "  ");
      printf("AAF_CLASS(%s,%s,\n  %s,\n  %s)\n",
             $C["r_sym"], cguid, parent, concrete);

    } else { # this item is a property

	  # assert( $C["r_nest"] == "Child" )
      type = $C["s_type_sym"];

      # match reference types and separate out the target type
	  if        ( 1==match(type,"(WeakReferenceVector)(.*)", typebits) ) {
        reftype = "WEAK_REFERENCE_VECTOR";
      } else if ( 1==match(type,"(WeakReferenceSet)(.*)", typebits) ) {
        reftype = "WEAK_REFERENCE_SET";
      } else if ( 1==match(type,"(WeakReference)(.*)", typebits) ) {
        reftype = "WEAK_REFERENCE";
      } else if ( 1==match(type,"(StrongReferenceVector)(.*)", typebits) ) {
        reftype = "STRONG_REFERENCE_VECTOR";
      } else if ( 1==match(type,"(StrongReferenceSet)(.*)", typebits) ) {
        reftype = "STRONG_REFERENCE_SET";
      } else if ( 1==match(type,"(StrongReference)(.*)", typebits) ) {
        reftype = "STRONG_REFERENCE";
      } else {
        reftype="";
      }

      if (reftype != "") {
        if ($C["s_target_sym"] == "" || $C["s_target_sym"] == typebits[2]) {
          type = sprintf("AAF_REFERENCE_TYPE(%s, %s)", typebits[1], typebits[2]);
	    } else if (typebits[2]=="" && $C["s_target_sym"] != "") {
          type = sprintf("AAF_REFERENCE_TYPE(%s, %s)", typebits[1], $C["s_target_sym"]);
        } else {
          propertyError($C["r_sym"], class, C["s_target_sym"])
          errors++;
        }
      } else {
        if ($C["s_target_sym"] == "") {
          type = sprintf("AAF_TYPE(%s)", type);
        } else {
          propertyError($C["r_sym"], class, C["s_target_sym"]);
          errors++;
        }
      }

      if ($C["r_minOccurs"] == "1") {
        mandatory = "true";
      } else if ($C["r_minOccurs"] == "0") {
        mandatory = "false";
      } else {
        propertyError($C["r_sym"], class, C["r_minOccurs"]);
        errors++;
      }
      if ($C["s_parent_sym"] != class) {
        propertyError($C["r_sym"], class, C["s_parent_sym"]);
        errors++;
      }
      if ($C["r_isAbstract"] != "") {
        propertyError($C["r_sym"], class, C["r_isAbstract"]);
        errors++;
      }
      if ($C["r_isUnique"] == "uid") {
        uid = "true";
        if ($C["r_minOccurs"] == "O") {
          propertyError($C["r_sym"], class, C["r_isUnique"]);
          printError("A unique identifier must be mandatory.\n");
          errors++;
        }
      } else {
        uid = "false";
      }
      # AAF_PROPERTY(name, id, tag, type, mandatory, container)
      pguid = formatAUID($C["ul_1"], $C["ul_2"], $C["ul_3"], $C["ul_4"],
                         $C["ul_5"], $C["ul_6"], $C["ul_7"], $C["ul_8"],
                         $C["ul_9"], $C["ul_10"], $C["ul_11"], $C["ul_12"],
                         $C["ul_13"], $C["ul_14"], $C["ul_15"], $C["ul_16"], "    ");
	  ppid = formatTag( $C["r_tag"] );
      printf("  AAF_PROPERTY(%s,%s,\n    %s,\n    %s,\n    %s,\n    %s,\n    %s)\n", $C["r_sym"], pguid, ppid, type, mandatory, uid, class);
    }

  # Types Register
  } else if( $C["r_reg"] == "Types" ) {

    if ($C["s_type_sym"] == "type" ) { # a type

	  # assert( $C["r_nest"] == "Leaf" )
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

      tguid = formatAUID($C["ul_1"], $C["ul_2"], $C["ul_3"], $C["ul_4"],
                         $C["ul_5"], $C["ul_6"], $C["ul_7"], $C["ul_8"],
                         $C["ul_9"], $C["ul_10"], $C["ul_11"], $C["ul_12"],
                         $C["ul_13"], $C["ul_14"], $C["ul_15"], $C["ul_16"], "  ");

      kind = $C["r_kind"];
      qualif = $C["r_qualif"];

      typeName = $C["r_sym"];

	  // diagnostic comments
      printf("\n");
      if ((qualif == "weak") || (qualif == "strong"))
        printf("// %s<%s>\n", referenceTypeName( kind, qualif ), $C["s_target_sym"]);
	  else
        printf("// %s\n", typeName);
      printf("//\n");

      if        (kind == "integer") {
        printf("AAF_TYPE_DEFINITION_INTEGER(%s, %s, %s, %s)\n", typeName, tguid, qualif, $C["r_value"]);

      } else if (kind == "array") {
        elementType = $C["s_target_sym"];
        if (qualif == "varying") {
          printf("AAF_TYPE_DEFINITION_VARYING_ARRAY(%s, %s,\n  AAF_TYPE(%s))\n", typeName, tguid, elementType);

        } else if (qualif == "fixed") {
          printf("AAF_TYPE_DEFINITION_FIXED_ARRAY(%s, %s,\n  AAF_TYPE(%s), %s)\n", typeName, tguid, elementType, $C["r_minOccurs"]);

        } else if (qualif == "strong") {
          # Special cases for strong reference vectors.
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);

        } else if (qualif == "weak") {
          # Special cases for weak reference vectors.
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);

        } else {
          typeError(typeName, C["r_qualif"]);
          errors++;
        }

      } else if (kind == "set") {
        elementType = $C["s_target_sym"];
        # Special cases for strong/weak reference sets.
        if (qualif == "strong") {
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);

        } else if (qualif == "weak") {
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, elementType, tguid, elementType);

        } else { 
          printf("AAF_TYPE_DEFINITION_SET(%s, %s,\n  AAF_TYPE(%s))\n", typeName, tguid, elementType);

        }

      } else if (kind == "reference") {
        targetType = $C["s_target_sym"];
        if (qualif == "strong") {
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);

        } else if (qualif == "weak" ) {
		  typeName = referenceTypeName( kind, qualif );
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE(\n  AAF_REFERENCE_TYPE_NAME(%s, %s), %s,\n  AAF_TYPE(%s))\n", typeName, targetType, tguid, targetType);

        } else {
          typeError(typeName, C["r_qualif"]);
          errors++;
        }

      } else if (kind == "enumeration" ) {
        etype = $C["s_target_sym"];
        printf("AAF_TYPE_DEFINITION_ENUMERATION(%s, %s, AAF_TYPE(%s))\n", typeName, tguid, etype);

      } else if (kind == "record") {
        printf("AAF_TYPE_DEFINITION_RECORD(%s, %s)\n", typeName, tguid)
		;
      } else if (kind == "rename") {
        printf("AAF_TYPE_DEFINITION_RENAME(%s, %s, %s)\n", typeName, tguid, $C["s_target_sym"]);

      } else if (kind == "string") {
        printf("AAF_TYPE_DEFINITION_STRING(%s, %s, %s)\n", typeName, tguid, $C["s_target_sym"]);

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
        printError("Unknown kind of type");
	    errors++;
      }

	  # set parentTypeName for use by members
	  parentTypeName = typeName;


    } else if ($C["s_type_sym"] == "member") { # a "member" of a type

	  # assert( $C["r_nest"] == "Child" )
      memberName = $C["r_sym"];
      if (kind == "enumeration" ) {
        printf("  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(%s,\n    %s, %s)\n", memberName, $C["r_value"], parentTypeName);
      } else if (kind == "record") {
        printf("  AAF_TYPE_DEFINITION_RECORD_FIELD(%s, AAF_TYPE(%s),\n    %s)\n", memberName, $C["s_target_sym"], parentTypeName);
      } else if (kind == "extendible") {

      eguid = formatAUID($C["ul_1"], $C["ul_2"], $C["ul_3"], $C["ul_4"],
                         $C["ul_5"], $C["ul_6"], $C["ul_7"], $C["ul_8"],
                         $C["ul_9"], $C["ul_10"], $C["ul_11"], $C["ul_12"],
                         $C["ul_13"], $C["ul_14"], $C["ul_15"], $C["ul_16"], "    ");

        printf("  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(%s,%s,\n    %s)\n", memberName, eguid, parentTypeName);
      } else if ((kind == "reference") && (qualif == "weak")) {
        printf("  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(%s, %s,\n    AAF_REFERENCE_TYPE_NAME(%s, %s))\n", memberName, $C["s_parent_sym"], typeName, targetType);
      } else {
        printError("Member of unknown kind of type");
	    errors++;
      }

	} else {
        printError("Unknown child of type");
	    errors++;
	}

  # Aliases Register (non SMPTE)
  } else if( $C["r_reg"] == "Aliases" ) {

    if ($C["s_type_sym"] == "alias") {

	  # assert( $C["r_nest"] == "Leaf" )
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

      aalias = $C["r_sym"];
      aoriginal = $C["g_alias"];
      printf("AAF_CLASS_ALIAS(%s, %s)\n", aoriginal, aalias); 
	}

  # Labels Register
  } else if( $C["r_reg"] == "Labels" ) {

    next # not emitting any macros for Labels yet

  # Instances Register
  } else if( $C["r_reg"] == "Instances" ) {

    if ($C["s_type_sym"] == "Instance" )
    { 
        # a set of instances of a [subclass of] DefinitionObject

        # assert( $C["r_nest"] == "Stalk" )

        if( firstInstance<0 )
        {
            # haven't had any aliases yet
            printError("Instances in wrong context");
	        errors++;
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
                printf("  AAF_INSTANCE_END(%s, %s, %s)\n", iclass, isym, iid);
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

        igsym = $C["r_sym"];
        if( igsym=="" ) nullError( "r_sym", "Instance" );

        igtarget = $C["s_target_sym"];
        if( igtarget=="" ) nullError( "s_target_sym", "Instance" );

        printf("AAF_INSTANCE_GROUP(%s, %s)\n", igsym, igtarget);

        isym = "";
    }
    else
    {
      # an instance of either a [subclass of] DefinitionObject or a property of the subclass

       if( $C["r_nest"] == "Leaf" )
       { 
            # an instance of a [subclass of] DefinitionObject

            # finish previous instance
            if( isym!="" )
            {
                printf("  AAF_INSTANCE_END(%s, %s, %s)\n", iclass, isym, iid);
                printf("  AAF_INSTANCE_SEPARATOR()\n");
            }

            isym = $C["r_sym"];

            iid = formatAUID($C["ul_1"], $C["ul_2"], $C["ul_3"], $C["ul_4"],
                             $C["ul_5"], $C["ul_6"], $C["ul_7"], $C["ul_8"],
                             $C["ul_9"], $C["ul_10"], $C["ul_11"], $C["ul_12"],
                             $C["ul_13"], $C["ul_14"], $C["ul_15"], $C["ul_16"], "      ");

            iclass = $C["s_type_sym"];
            idesc = $C["r_detail"];

            printf("  AAF_INSTANCE(%s, %s, %s, %s)\n", iclass, isym, iid, "\"" idesc "\"");

            # basic properties of DefinitionObject
            printf("    AAF_INSTANCE_PROPERTY(%s, %s, %s)\n", "Name", "String", "\"" isym "\"");

            if( idesc != "" ) printf("    AAF_INSTANCE_PROPERTY(%s, %s, %s)\n", "Description", "String", "\"" idesc "\"");

            printf("    AAF_INSTANCE_PROPERTY(%s, %s, %s)\n", "Identification", "AUID", iid);

	   }
       else if( $C["r_nest"] == "Child" )
       {
            # a property of the subclass

            ipsym = $C["r_sym"];
            iptype = $C["s_type_sym"];
            ipvalue = $C["r_value"];
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
                printf("  AAF_INSTANCE_END(%s, %s, %s)\n", iclass, isym, iid);
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
  s = formatAUIS(O01, O02, O03, O04, O05, O06, O07, O08,
                 O09, O10, O11, O12, O13, O14, O15, O16);
  return sprintf("\n%s// %s\n%sAAF_LITERAL_AUID(0x%s%s%s%s,\n%s  0x%s%s, 0x%s%s,\n%s  0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s, 0x%s)",
             indent, s, indent,
             O09, O10, O11, O12, indent,
             O13, O14, O15, O16, indent,
             O01, O02, O03, O04,
             O05, O06, O07, O08);
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
  printError(sprintf("Illegal entry \"%s\" in column \"%s\", for %s.\n", $C[column], column, elementName));
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

