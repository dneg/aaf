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
    xmlns:aafm="http://www.aafassociation.org/aafx/v1.1/20050628">

    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

    <xsl:param name="instanceDoc" select="''"/>
    
    
    <xsl:template match="node() | @*">
        <xsl:copy>
            <xsl:apply-templates select="@* | node()"/>
        </xsl:copy>
    </xsl:template>
    

    <xsl:template match="/aafm:AAFFileModel">
        <xsl:copy>
            <aafm:Symbolspaces>
                <xsl:apply-templates select="aafm:Baseline" mode="Symbolspaces"/>
                <xsl:apply-templates select="document($instanceDoc)/*/aafm:Extensions/*" mode="Symbolspaces"/>
            </aafm:Symbolspaces>
            <aafm:Definitions>
                <xsl:apply-templates select="aafm:Baseline"/>
                <xsl:apply-templates select="document($instanceDoc)/*/aafm:Extensions/*"/>
            </aafm:Definitions>
        </xsl:copy>
    </xsl:template>
    
    <xsl:template match="aafm:Baseline" mode="Symbolspaces">
        <aafm:Symbolspace isBaseline="true">
            <aafm:URI>
                <xsl:value-of select="aafm:Symbolspace"/>
            </aafm:URI>
            <aafm:Prefix>aaf</aafm:Prefix>
        </aafm:Symbolspace>
    </xsl:template>
    
    <xsl:template match="aafm:Extension" mode="Symbolspaces">
        <aafm:Symbolspace>
            <aafm:URI>
                <xsl:value-of select="aafm:Symbolspace"/>
            </aafm:URI>
            <aafm:Prefix>ns<xsl:value-of select="position()"/></aafm:Prefix>
        </aafm:Symbolspace>
    </xsl:template>
    
    <xsl:template match="aafm:Baseline">
        <xsl:variable name="symbolspace">
            <xsl:value-of select="aafm:Symbolspace"/>
        </xsl:variable>
        <xsl:variable name="prefix">
            <xsl:text>aaf</xsl:text>
        </xsl:variable>
        <xsl:for-each select="aafm:Definitions/*"> 
            <xsl:copy>
                <xsl:apply-templates select="@*"/>
                <aafm:Symbolspace><xsl:value-of select="$symbolspace"/></aafm:Symbolspace>
                <aafm:QSymbol><xsl:value-of select="$prefix"/><xsl:text>:</xsl:text><xsl:value-of select="aafm:Symbol"/></aafm:QSymbol>
                <aafm:LQSymbol><xsl:value-of select="$symbolspace"/><xsl:text> </xsl:text><xsl:value-of select="aafm:Symbol"/></aafm:LQSymbol>
                <xsl:apply-templates select="node()"/>
            </xsl:copy>
        </xsl:for-each>
    </xsl:template>
    
    <xsl:template match="aafm:Extension">
        <xsl:variable name="symbolspace">
            <xsl:value-of select="aafm:Symbolspace"/>
        </xsl:variable>
        <xsl:variable name="prefix">
            <xsl:text>ns</xsl:text><xsl:value-of select="position()"/>
        </xsl:variable>
        <xsl:for-each select="aafm:Definitions/*"> 
            <xsl:copy>
                <xsl:apply-templates select="@*"/>
                <aafm:Symbolspace><xsl:value-of select="$symbolspace"/></aafm:Symbolspace>
                <aafm:QSymbol><xsl:value-of select="$prefix"/><xsl:text>:</xsl:text><xsl:value-of select="aafm:Symbol"/></aafm:QSymbol>
                <aafm:LQSymbol><xsl:value-of select="$symbolspace"/><xsl:text> </xsl:text><xsl:value-of select="aafm:Symbol"/></aafm:LQSymbol>
                <xsl:apply-templates select="node()"/>
            </xsl:copy>
        </xsl:for-each>
    </xsl:template>
    
    
</xsl:stylesheet>

