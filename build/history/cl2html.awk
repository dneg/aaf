#
# Generate an html table from a change log (output of cvs2cl.pl)
#
# $ cat ChangeLog | awk -f build/history/cl2html.awk > changelog.html
#
# Tim Bingham
#
BEGIN {
  date = "";
  printHeader();
}

/^[0-9]+-[0-9]+-[0-9]+/ {
  if (date != "") {
#    printf("[%s]\n", filesandcomments);
    gsub("\t", " ", filesandcomments);
    filesandcomments = trim(filesandcomments, 3);
    f = split(filesandcomments, fields, ":");
    printf("[%d]\n", f);
    for (i = 1; i <= f; i++) {
      printf("[%d : \"%s\"]\n", i, fields[i]);
    }
    if (f < 2) {
      printf("cl2html : Error near \"%s\"\n", filesandcomments) | "cat 1>&2";
      exit(1);
    }
    if (match(fields[1], "/$")) {
      dir = fields[1];
      files = trim(fields[2], 1);
      cs = 3;
    } else {
      dir = "";
      files = fields[1];
      cs = 2;
    }
    comments = trim(fields[cs], 1);
    for (i = cs + 1; i <= f; i++) {
      comments = comments ":" fields[i]
    }

    printf("[dir      = \"%s\"]\n", dir);
    printf("[files    = \"%s\"]\n", files);
    printf("[comments = \"%s\"]\n", comments);
    /* print previous table row */
    printRow(date, name, files, comments, color);
  }
  /* build new table row */
  date = $1;
  name = $3;
  filesandcomments = "";
  files = "";
  comments = "";
  color = "#FFCCCC";
}

/^\t/ {
  /* accumulate the file names and comments */
  filesandcomments = filesandcomments $0;
}

END {
  if (date != "") {
    /* print last table row */
    printRow(date, name, files, comments, color);
  }
  printTrailer();
}

function printHeader() {
#  printf("<HTML>");
#  printf("<BODY>");
  printf("\
<TABLE width=\"100%%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
}

function printRow(date, name, files, comments, color) {
  printf("\
<TR>\n\
  <TD width=\"50\" height=\"22\">%s</TD>\n\
  <TD height=\"22\" width=\"112\" bgcolor=\"%s\">%s</TD>\n\
  <TD colspan=\"2\" height=\"22\" bgcolor=\"%s\">%s</TD>\n\
  <TD colspan=\"3\" height=\"22\" bgcolor=\"%s\">%s</TD>\n\
</TR>\n",
    date, color, name, color, files, color, comments);
}

function printTrailer() {
  printf("</TABLE>\n");
#  printf("</HTML>");
#  printf("</BODY>");
}

# Remove first n characters of s
function trim(s, n) {
  return substr(s, n + 1, length(s) - n);
}
