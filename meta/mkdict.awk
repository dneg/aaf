#
# Generate an AAF meta dictionary from a .csv spreadsheet.
#
# Command line:
#
#   awk -f  mkdict.awk ../smpte/DictP18.csv > AAFMetaDictionary.h
#
BEGIN {
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
  printf("//   1) #define the macros to suit your usage\n");
  printf("//   2) #include this file\n");
  printf("//   3) #undef the macros\n");
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
  printf("// This file was generated on ");
  system("date");
  printf("// by user ");
  system("print $USERNAME");
  printf("// on system ");
  system("print $COMPUTERNAME");
  printf("//\n");
  printf("// Key to macros.\n");
  printf("//\n");
  printf("// AAF_CLASS(name, dataid, parent)\n");
  printf("//\n");
  printf("//   name      = the name of the class\n");
  printf("//   dataid    = the auid used to identify the class\n");
  printf("//   parent    = the immediate ancestor class\n");
  printf("//\n");
  printf("// AAF_CLASS_END(name)\n");
  printf("//\n");
  printf("//   name      = the name of the class\n");
  printf("//\n");
  printf("// AAF_PROPERTY(name, dataid, datatag, type, container)\n");
  printf("//\n");
  printf("//   name      = the name of the property\n");
  printf("//   dataid    = the auid used to identify the property\n");
  printf("//   datatag   = the short form of the dataid\n");
  printf("//   type      = the type of the property\n");
  printf("//   container = the class that defines this property\n");
  printf("//\n");
  printf("//\n");
  printf("\n");
  printf("// Default empty definitions\n");
  printf("//\n");
  printf("#ifndef AAF_CLASS\n");
  printf("#define AAF_CLASS(name, dataid, parent)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_CLASS_END\n");
  printf("#define AAF_CLASS_END(name)\n");
  printf("#endif\n");
  printf("\n");
  printf("#ifndef AAF_PROPERTY\n");
  printf("#define AAF_PROPERTY(name, dataid, datatag, type, container)\n");
  printf("#endif\n");
  printf("\n");
  printf("// Helper definitions\n");
  printf("//\n");
  printf("#define AAF_LITERAL_AUID(l , w1, w2,  b1, b2, b3 , b4, b5, b6, b7, b8) \\\n");
  printf("                        {l , w1, w2, {b1, b2, b3 , b4, b5, b6, b7, b8}}\n");
  printf("\n");
  printf("// AAF Object model\n");
  printf("//\n");
  #
  # Initialize global variables.
  #

  pidinc = 256 # 0x100
}

/Key/ {
  next # Discard the key line
}

{
  if ($13 != "") { # This item has a name
    # printf("// <%s> \n", $20);
    if ($13 == "Class") { # This item is a class
      if ($10 != class) { # This is a new class
        if (class != "" ) {
          # end the old one
          printf("AAF_CLASS_END(%s)\n", class);
          parent = $20;
        } else {
          parent = "0"
        }
        class = $10;
        printf("\n");
        printf("// %s\n", class);
        printf("//\n");
        pidbase = pidbase + pidinc;
        pid = pidbase + 1;
      }
      # AAF_CLASS(name, dataid, parent)
      printf("AAF_CLASS(%s,\n  AAF_LITERAL_AUID(0x%s%s%s%s,\n    0x%s%s, 0x%s%s,\n    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x%s),\n  %s)\n",
             $10, $2, $3, $4, $5, $6, $7, $8, $9, $1, parent);
    } else {
      # AAF_PROPERTY(name, dataid, datatag, type, container)
      printf("AAF_PROPERTY(%s,\n  AAF_LITERAL_AUID(0x%s%s%s%s,\n    0x%s%s, 0x%s%s,\n    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x%s),\n  %x,\n  %s,\n  %s)\n", $10, $2, $3, $4, $5, $6, $7, $8, $9, $1, pid, $13, class);
      pid = pid + 1;
    }
  }
}

END {
  printf("AAF_CLASS_END(%s)\n", class);
  printf("\n");
  printf("//\n");
  printf("// End of automatically generated file.\n");
}
