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
  printf("// Meta definitions for initializing the AAFDictionary.\n");
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
  printf("// \n");
  printf("//   name      = the name of the class\n");
  printf("//   dataid    = the auid used to identify the class\n");
  printf("//   parent    = the immediate ancestor class\n");
  printf("// \n");
  printf("// AAF_PROPERTY(name, dataid, datatag, type, container)\n");
  printf("// \n");
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
  printf("#ifndef AAF_PROPERTY\n");
  printf("#define AAF_PROPERTY(name, dataid, datatag, type, container)\n");
  printf("#endif\n");
  printf("\n");
  printf("// Helper definitions\n");
  printf("\n");
  printf("#define AAF_LITERAL_AUID(l , w1, w2, b1, b2, b3 , b4, b5, b6, b7, b8) {l , w1, w2, {b1, b2, b3 , b4, b5, b6, b7, b8}}");
  printf("\n");

  #
  # Initialize global variables.
  #
  class = "0"
  parent = "root"
}

/Key/ {
  next # Discard the key line
}

  {

  if ($13 != "") {
    if ($13 == "Class") {
      if ($10 != class) {
        parent = class;
        class = $10;
        printf("\n");
        printf("// %s\n", class);
        printf("//\n");
      }
      # AAF_CLASS(name, dataid, parent)
      printf("AAF_CLASS(%s, AAF_LITERAL_AUID(0x%s%s%s%s, 0x%s%s, 0x%s%s, 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x%s), %s)\n",
             $10, $2, $3, $4, $5, $6, $7, $8, $9, $1, parent);
    } else {
      # AAF_PROPERTY(name, dataid, datatag, type, container)
      printf("AAF_PROPERTY(%s, 0, 0, %s, %s)\n", $10, $13, class);
    }
  }
}

END {
  printf("\n");
  printf("//\n");
  printf("// End of automatically generated file.\n");
}
