<?xml version="1.0" encoding="UTF-8"?>
<!--
  $Id$ $Name$

  The contents of this file are subject to the AAF SDK Public
  Source License Agreement (the "License"); You may not use this file
  except in compliance with the License.  The License is available in
  AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
  Association or its successor.

  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
  the License for the specific language governing rights and limitations
  under the License.

  The Original Code of this file is Copyright 1998-2008, Licensor of the
  AAF Association.

  The Initial Developer of the Original Code of this file and the
  Licensor of the AAF Association is British Broadcasting Corporation.
  All rights reserved.
-->
<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:xs="http://www.w3.org/2001/XMLSchema" 
    xmlns:aafm="http://www.aafassociation.org/aafx/v1.1/20050628" 
    exclude-result-prefixes="aafm">

    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

    <xsl:template match="aafm:ClassDefinition">

        <xsl:variable name="isDefObject">
            <xsl:call-template name="is_sub_class_of">
                <xsl:with-param name="class" select="."/>
                <xsl:with-param name="superClassQSymbol" select="concat($baselinePrefix, ':DefinitionObject')"/>
            </xsl:call-template>
        </xsl:variable>
    
        <xs:element name="{aafm:Symbol}">
            <xsl:if test="aafm:IsConcrete = 'false'">
                <xsl:attribute name="abstract">
                    <xsl:value-of select="'true'"/>
                </xsl:attribute>
            </xsl:if>
            <xs:complexType>
                <xs:all>
                    <xsl:call-template name="prop_defs_in_class">
                        <xsl:with-param name="class" select="."/>
                    </xsl:call-template>
                </xs:all>
                <xsl:if test="$isDefObject = 'true'">
                    <xs:attribute ref="aaf:uid" use="required"/>
                </xsl:if>
            </xs:complexType>
        </xs:element>

    </xsl:template>


</xsl:stylesheet>

