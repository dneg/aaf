#
# awk -f dod2iid.awk C=AAFPulldown AAFPulldown.dod
#
BEGIN {
  FS = "[(), ]*";
}
/AD_DEFAULT_UUID/ {
  printf("const IID IID_I%s = {0x%s,0x%s,0x%s,{0x%s,0x%s,0x%s,0x%s,0x%s,0x%s,0x%s,0x%s}};\n\n\n" , C, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12);
}
