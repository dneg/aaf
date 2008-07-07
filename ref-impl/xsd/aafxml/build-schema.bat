@rem Usage: <baseline metadict filename> <aaf-xml instance filename>

@echo off

echo merging...
java -cp saxon.jar com.icl.saxon.StyleSheet -o tmp_merge.xml %1 merge.xsl instanceDoc=%2
echo normalising...
java -cp saxon.jar com.icl.saxon.StyleSheet -o preprocessed.xml tmp_merge.xml normalise.xsl

echo creating schema...
java -cp saxon.jar com.icl.saxon.StyleSheet -o aaf.xsd preprocessed.xml main.xsl

