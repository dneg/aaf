#
# Generate an AAF meta dictionary from a .csv spreadsheet.
#
# Command line:
#
#   awk -f mkdict.awk ../smpte/DictP18-277-1847BMaster.csv | flip -u > AAFMetaDictionary.h
#
# Author
#
#   Tim Bingham tim_bingham@avid.com
#
# This program uses the following spreadsheet columns for the
# purposes indicated ...
#
# $1 - $9 = column(A - I) => AUID (with appropriate reordering).
# $10     = column(J)     => class/property name.
# $22     = column(V)     => if this field == "class" then the entity is a
#                            class, otherwise it is a property and this field
#                            denotes the type of the property. When this
#                            field == "class" this row is taken to be the
#                            start of a class definition. All the following
#                            rows (until the start of the next class) are
#                            taken to be properties of this class.
# $26     = column(Z)     => if this field == "M" then this is a required
#                            property. If this field is "O" then this is an
#                            optional property.
# $29     = column(AC)    => if this entity is a class then this field is
#                            the immediate ancestor (otherwise the field
#                            is checked to see that it is the same as the
#                            current class).
# $30     = column(AD)    => this field is not an input field to this program,
#                            however it is checked. If this entity is a class
#                            then this field should be the same as the class
#                            name (J), otherwise the field should be the same
#                            as the parent/owning class (AC).
# $33     = column(AG)    => property id. This field is ignored if
#                            field $29 == "class"
# $34     = column(AH)    => if this is a property and the property type (V)
#                            is a reference then this field is the type (class)
#                            of the referenced object, otherwise this field
#                            should be empty.
# $35     = column(AI)    => if this entity is a class and the class is
#                            abstract, this field contains "abstract" otherwise
#                            this filed should be empty.
#
BEGIN {
  errors = 0;
  FS=","
  #
  # Write a header to the generated file.
  #
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
  "print $USERNAME" | getline user
  "print $COMPUTERNAME" | getline computer
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
  printf("// AAF_CLASS_END(name)\n");
  printf("//\n");
  printf("//   End an AAF class definition.\n");
  printf("//\n");
  printf("//     name      = the name of the class\n");
  printf("//\n");
  printf("// AAF_CLASS_SEPARATOR()\n");
  printf("//\n");
  printf("//   Separate one AAF class definition from another.\n");
  printf("//\n");
  printf("// AAF_PROPERTY(name, id, tag, type, mandatory, container)\n");
  printf("//\n");
  printf("//   Define an AAF property.\n");
  printf("//\n");
  printf("//     name      = the name of the property\n");
  printf("//     id        = the auid used to identify the property [*]\n");
  printf("//     tag       = the short form of the id\n");
  printf("//     type      = the type of the property [*]\n");
  printf("//     mandatory = true if the property is mandatory\n");
  printf("//                 false if the property is optional\n");
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
  printf("// AAF_TYPE_DEFINITION_ENUMERATION_END(name)\n");
  printf("//\n");
  printf("//   End an AAF enumerated type definition.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
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
  printf("// AAF_TYPE_DEFINITION_RECORD_END(name)\n");
  printf("//\n");
  printf("//   End an AAF record type definition.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
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
  printf("// AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name)\n");
  printf("//\n");
  printf("//   End an AAF extendible enumerated type definition.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_CHARACTER(name, id, size)\n");
  printf("//\n");
  printf("//   Define an AAF character type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     size      = the size (in bytes) of the type\n");
  printf("//\n");
  printf("// AAF_TYPE_DEFINITION_SET(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF set type.\n");
  printf("//\n");
  printf("//     name      = the name of the type\n");
  printf("//     id        = the auid used to identify the type [*]\n");
  printf("//     type      = the element type [*]\n");
  printf("//\n");
  printf("//AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type)\n");
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
  printf("//AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type)\n");
  printf("//\n");
  printf("//   Define an AAF weak reference type.\n");
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
  printf("//AAF_TYPE_DEFINITION_STREAM(name, id)\n");
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
  printf("#define AAF_CLASS_END(name)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_CLASS_SEPARATOR\n");
  printf("#define AAF_CLASS_SEPARATOR()\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_PROPERTY\n");
  printf("#define AAF_PROPERTY(name, id, tag, type, mandatory, container)\n");
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
  printf("#define AAF_TYPE_DEFINITION_ENUMERATION_END(name)\n");
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
  printf("#define AAF_TYPE_DEFINITION_RECORD_END(name)\n");
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
  printf("#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_TYPE_DEFINITION_CHARACTER\n");
  printf("#define AAF_TYPE_DEFINITION_CHARACTER(name, id, size)\n");
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
  printf("// Helper definitions\n");
  printf("//\n");
  printf("#define AAF_LITERAL_AUID(l , w1, w2,  b1, b2, b3 , b4, b5, b6, b7, b8) \\\n");
  printf("                        {l , w1, w2, {b1, b2, b3 , b4, b5, b6, b7, b8}}\n");
  printf("\n");
  printf("// AAF Object model\n");
  printf("//\n");
  printf("\n");
  printf("AAF_TABLE_BEGIN()\n");
  #
  # Initialize global variables.
  #
  firstType = 1;
}

/Key/ {
  next # Discard the key line
}

{
  if ($10 != "") { # This item has a name
    # printf("// <%s> \n", $29);
    if ($22 == "Class") { # This item is a class
      if ($10 != class) { # This is a new class
        if (class != "" ) {
          # end the old one
          printf("AAF_CLASS_END(%s)\n", class);
          printf("AAF_CLASS_SEPARATOR()\n");
          parent = $29;
        } else {
          parent = "Root"
        }
        class = $10;
        printf("\n");
        printf("// %s\n", class);
        printf("//\n");
      }
      if ($26 != "") {
        printError(sprintf("Illegal entry \"%s\" in column Z, for class \"%s\".\n", $26, class));
        errors++;
      }
      if ($30 != $10) {
        printError(sprintf("Illegal entry \"%s\" in column AD, for class \"%s\".\n", $30, class));
        errors++;
      }
      if ($34 != "") {
        printError(sprintf("Illegal entry \"%s\" in column AH, for class \"%s\".\n", $34, class));
        errors++;
      }
      concrete = "true";
      if ($35 == "abstract") {
        concrete = "false";
      } else if ($35 != "") {
        printError(sprintf("Illegal entry \"%s\" in column AI, for class \"%s\".\n", $35, class));
        errors++;
      }
      # AAF_CLASS(name, id, parent)
      printf("AAF_CLASS(%s,\n  AAF_LITERAL_AUID(0x%s%s%s%s,\n    0x%s%s, 0x%s%s,\n    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x%s),\n  %s,\n  %s)\n",
             $10, $2, $3, $4, $5, $6, $7, $8, $9, $1, parent, concrete);
    } else if ($22 == "type" ) {
      # a type
      if (firstType) {
        printf("AAF_CLASS_END(%s)\n", class);
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
          printf("AAF_TYPE_DEFINITION_ENUMERATION_END(%s)\n", typeName);
        } else if (kind == "record") {
          printf("AAF_TYPE_DEFINITION_RECORD_END(%s)\n", typeName);
        } else if (kind == "extendible") {
          printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(%s)\n", typeName);
        }
        printf("AAF_TYPE_SEPARATOR()\n");
      }
#
      guid = sprintf("\n  AAF_LITERAL_AUID(0x%s%s%s%s,\n    0x%s%s, 0x%s%s,\n    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x%s)",
             $2, $3, $4, $5, $6, $7, $8, $9, $1);
#
      typeName = $10;
      parentTypeName = typeName;
      printf("\n");
      printf("// %s", typeName);
      if (($24 == "weak") || ($24 == "strong")) {
        printf("<%s>\n", $25);
      } else {
        printf("\n");
      }
      printf("//\n");
      kind = $23;
      if        (kind == "integer") {
        printf("AAF_TYPE_DEFINITION_INTEGER(%s, %s, %s, %s)\n", typeName, guid, $24, $25);
      } else if (kind == "enumeration" ) {
        printf("AAF_TYPE_DEFINITION_ENUMERATION(%s, %s, AAF_TYPE(%s))\n", typeName, guid, $24);
      } else if (kind == "array") {
        elementType = $25;
        if ($24 == "varying") {
          printf("AAF_TYPE_DEFINITION_VARYING_ARRAY(%s, %s, AAF_TYPE(%s))\n", typeName, guid, elementType);
        } else if ($24 == "fixed") {
          printf("AAF_TYPE_DEFINITION_FIXED_ARRAY(%s, %s, AAF_TYPE(%s), %s)\n", typeName, guid, elementType, $26);
        } else if ($24 == "strong") {
          # Special cases for strong reference vectors.
            printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(\nAAF_REFERENCE_TYPE_NAME(%s, %s), %s, AAF_TYPE(%s))\n", typeName, elementType, guid, elementType);
        } else if ($24 == "weak") {
          # Special cases for weak reference vectors.
            printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(\nAAF_REFERENCE_TYPE_NAME(%s, %s), %s, AAF_TYPE(%s))\n", typeName, elementType, guid, elementType);
        } else {
          printError(sprintf("Illegal entry \"%s\" in column X, for type \"%s\".\n", $24, typeName));
          errors++;
        }
      } else if (kind == "record") {
        printf("AAF_TYPE_DEFINITION_RECORD(%s, %s)\n", typeName, guid);
      } else if (kind == "rename") {
        printf("AAF_TYPE_DEFINITION_RENAME(%s, %s, %s)\n", typeName, guid, $25);
      } else if (kind == "string") {
        printf("AAF_TYPE_DEFINITION_STRING(%s, %s, %s)\n", typeName, guid, $25);
      } else if (kind == "extendible") {
        printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(%s, %s)\n", typeName, guid);
      } else if (kind == "character") {
        printf("AAF_TYPE_DEFINITION_CHARACTER(%s, %s, %s)\n", typeName, guid, $24);
      } else if (kind == "set") {
        elementType = $25;
        # Special cases for strong/weak reference sets.
        if ($24 == "strong") {
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(\nAAF_REFERENCE_TYPE_NAME(%s, %s), %s, AAF_TYPE(%s))\n", typeName, elementType, guid, elementType);
        } else if ($24 == "weak") {
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(\nAAF_REFERENCE_TYPE_NAME(%s, %s), %s, AAF_TYPE(%s))\n", typeName, elementType, guid, elementType);
        } else { 
          printf("AAF_TYPE_DEFINITION_SET(%s, %s, AAF_TYPE(%s))\n", typeName, guid, elementType);
        }
      } else if (kind == "reference") {
        targetType = $25;
        if ($24 == "strong") {
          printf("AAF_TYPE_DEFINITION_STRONG_REFERENCE(\nAAF_REFERENCE_TYPE_NAME(%s, %s), %s, AAF_TYPE(%s))\n", typeName, targetType, guid, targetType);
        } else if ($24 == "weak" ) {
          printf("AAF_TYPE_DEFINITION_WEAK_REFERENCE(\nAAF_REFERENCE_TYPE_NAME(%s, %s), %s, AAF_TYPE(%s))\n", typeName, targetType, guid, targetType);
        } else {
          printError(sprintf("Illegal entry \"%s\" in column X, for type \"%s\".\n", $24, typeName));
          errors++;
        }
      } else if (kind == "stream") {
          printf("AAF_TYPE_DEFINITION_STREAM(%s, %s)\n", typeName, guid);
      } else {
        # better be another type
      }
    } else if ($22 == "member") {
      # a "member" of a type
      memberName = $10;
      if (kind == "enumeration" ) {
        printf("AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(%s, %s, %s)\n", memberName, $23, parentTypeName);
      } else if (kind == "record") {
        printf("AAF_TYPE_DEFINITION_RECORD_FIELD(%s, AAF_TYPE(%s), %s)\n", memberName, $23, parentTypeName);
      } else if (kind == "extendible") {
#
      eguid = sprintf("\n  AAF_LITERAL_AUID(0x%s%s%s%s,\n    0x%s%s, 0x%s%s,\n    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x%s)",
             $2, $3, $4, $5, $6, $7, $8, $9, $1);
#
        printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(%s, %s, %s)\n", memberName, eguid, parentTypeName);
      } else {
        # error, what is this a member of ?
      }     
    } else { # this item is a property
      type = $22;
      if (type == "Array of Int32") {
        type = "Int32Array"
      } else if (type == "Array of Int64") {
        type = "Int64Array"
      }
      #
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
        if ($34 != "") {
          # type = sprintf("AAF_%s(%s, %s)", reftype, type, $34);
          type = sprintf("AAF_REFERENCE_TYPE(%s, %s)", type, $34);
        } else {
          printError(sprintf("Illegal entry \"%s\" in column AH, for property \"%s\" of class \"%s\".\n", $34, $10, class));
          errors++;
        }
      } else {
        if ($34 == "") {
          type = sprintf("AAF_TYPE(%s)", type);
        } else {
          printError(sprintf("Illegal entry \"%s\" in column AH, for property \"%s\" of class \"%s\".\n", $34, $10, class));
          errors++;
        }
      }
      #
      if ($26 == "M") {
        mandatory = "true";
      } else if ($26 == "O") {
        mandatory = "false";
      } else {
        printError(sprintf("Illegal entry \"%s\" in column Z, for property \"%s\" of class \"%s\".\n", $26 , $10, class));
        errors++;
      }
      if ($29 != class) {
        printError(sprintf("Illegal entry \"%s\" in column AC, for property \"%s\" of class \"%s\".\n", $29, $10, class));
        errors++;
      }
      if ($30 != $29) {
        printError(sprintf("Illegal entry \"%s\" in column AD, for property \"%s\" of class \"%s\".\n", $30, $10, class));
        errors++;
      }
      if ($35 != "") {
        printError(sprintf("Illegal entry \"%s\" in column AI, for property \"%s\" of class \"%s\".\n", $35, $10, class));
        errors++;
      }
      # AAF_PROPERTY(name, id, tag, type, mandatory, container)
      printf("AAF_PROPERTY(%s,\n  AAF_LITERAL_AUID(0x%s%s%s%s,\n    0x%s%s, 0x%s%s,\n    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x%s),\n  0x%s,\n  %s,\n  %s,\n  %s)\n", $10, $2, $3, $4, $5, $6, $7, $8, $9, $1, $33, type, mandatory, class);
    }
  }
}

END {
  if (errors == 0 ){
    if (kind == "enumeration" ) {
      printf("AAF_TYPE_DEFINITION_ENUMERATION_END(%s)\n", typeName);
    } else if (kind == "record") {
      printf("AAF_TYPE_DEFINITION_RECORD_END(%s)\n", typeName);
    } else if (kind == "extendible") {
      printf("AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(%s)\n", typeName);
    }
    printf("\n");
    printf("AAF_TYPE_TABLE_END()\n");
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
    printf("#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR\n");
    printf("\n");
    printf("#undef AAF_TYPE_DEFINITION_STREAM\n");
    printf("\n");
    printf("#undef AAF_TYPE_SEPARATOR\n");
    printf("\n");
    printf("//\n");
    printf("// End of automatically generated file.\n");
  } else {
    printError(sprintf("File generation failed with %d errors.\n", errors));
    exit 1
  }
}

function printError(message)
{
  printf("#error %s", message);
  printf("Error : %s", message) | "cat 1>&2";
}
