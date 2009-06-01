<?xml version="1.0" encoding="UTF-8"?>
<!--
  $Id$ $Name$

  The contents of this file are subject to the AAF SDK Public Source
  License Agreement Version 2.0 (the "License"); You may not use this
  file except in compliance with the License.  The License is available
  in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
  Advanced Media Workflow Association, Inc., or its successor.

  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
  the License for the specific language governing rights and limitations
  under the License.  Refer to Section 3.3 of the License for proper use
  of this Exhibit.

  WARNING:  Please contact the Advanced Media Workflow Association,
  Inc., for more information about any additional licenses to
  intellectual property covering the AAF Standard that may be required
  to create and distribute AAF compliant products.
  (http://www.amwa.tv/policies).

  Copyright Notices:
  The Original Code of this file is Copyright 1998-2009, licensor of the
  Advanced Media Workflow Association.  All rights reserved.

  The Initial Developer of the Original Code of this file and the
  licensor of the Advanced Media Workflow Association is
  British Broadcasting Corporation.
  All rights reserved.
-->
<xsl:stylesheet version="1.0" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:aafm="http://www.aafassociation.org/aafx/v1.1/20050628">

    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

    <xsl:template match="node() | @*">
        <xsl:copy>
            <xsl:apply-templates select="@* | node()"/>
        </xsl:copy>
    </xsl:template>
    
    <xsl:key name="classDefSymbolKey" match="//aafm:ClassDefinition" use="aafm:Symbol"/>
    <xsl:key name="classDefIdentKey" match="//aafm:ClassDefinition" use="aafm:Identification"/>
    <xsl:key name="classDefLQSymbolKey" match="//aafm:ClassDefinition" use="aafm:LQSymbol"/>
    
    <xsl:key name="propertyDefSymbolKey" match="//aafm:PropertyDefinition" use="aafm:Symbol"/>
    <xsl:key name="propertyDefIdentKey" match="//aafm:PropertyDefinition" use="aafm:Identification"/>
    <xsl:key name="propertyDefLQSymbolKey" match="//aafm:PropertyDefinition" use="aafm:LQSymbol"/>
    
    <xsl:key name="typeDefSymbolKey" match="//aafm:Definitions/*[starts-with(name(), 'TypeDefinition')]" use="aafm:Symbol"/>
    <xsl:key name="typeDefIdentKey" match="//aafm:Definitions/*[starts-with(name(), 'TypeDefinition')]" use="aafm:Identification"/>
    <xsl:key name="typeDefLQSymbolKey" match="//aafm:Definitions/*[starts-with(name(), 'TypeDefinition')]" use="aafm:LQSymbol"/>

    
    <xsl:template match="aafm:ParentClass">
        <aafm:ParentClass>
            <xsl:choose>
                <xsl:when test="key('classDefSymbolKey', .)">
                    <xsl:value-of select="key('classDefSymbolKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:when test="key('classDefIdentKey', .)">
                    <xsl:value-of select="key('classDefIdentKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="key('classDefLQSymbolKey', .)/aafm:QSymbol"/>
                </xsl:otherwise>
            </xsl:choose>
        </aafm:ParentClass>
    </xsl:template>

    <xsl:template match="aafm:MemberOf">
        <aafm:MemberOf>
            <xsl:choose>
                <xsl:when test="key('classDefSymbolKey', .)">
                    <xsl:value-of select="key('classDefSymbolKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:when test="key('classDefIdentKey', .)">
                    <xsl:value-of select="key('classDefIdentKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="key('classDefLQSymbolKey', .)/aafm:QSymbol"/>
                </xsl:otherwise>
            </xsl:choose>
        </aafm:MemberOf>
    </xsl:template>

    <xsl:template match="aafm:ElementType">
        <aafm:ElementType>
            <xsl:choose>
                <xsl:when test="key('typeDefSymbolKey', .)">
                    <xsl:value-of select="key('typeDefSymbolKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:when test="key('typeDefIdentKey', .)">
                    <xsl:value-of select="key('typeDefIdentKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="key('typeDefLQSymbolKey', .)/aafm:QSymbol"/>
                </xsl:otherwise>
            </xsl:choose>
        </aafm:ElementType>
    </xsl:template>
    
    <xsl:template match="aafm:RenamedType">
        <aafm:RenamedType>
            <xsl:choose>
                <xsl:when test="key('typeDefSymbolKey', .)">
                    <xsl:value-of select="key('typeDefSymbolKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:when test="key('typeDefIdentKey', .)">
                    <xsl:value-of select="key('typeDefIdentKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="key('typeDefLQSymbolKey', .)/aafm:QSymbol"/>
                </xsl:otherwise>
            </xsl:choose>
        </aafm:RenamedType>
    </xsl:template>
    
    <xsl:template match="aafm:ReferencedType">
        <aafm:ReferencedType>
            <xsl:choose>
                <xsl:when test="key('classDefSymbolKey', .)">
                    <xsl:value-of select="key('classDefSymbolKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:when test="key('classDefIdentKey', .)">
                    <xsl:value-of select="key('classDefIdentKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="key('classDefLQSymbolKey', .)/aafm:QSymbol"/>
                </xsl:otherwise>
            </xsl:choose>
        </aafm:ReferencedType>
    </xsl:template>
    
    <xsl:template match="aafm:Type">
        <aafm:Type>
            <xsl:choose>
                <xsl:when test="key('typeDefSymbolKey', .)">
                    <xsl:value-of select="key('typeDefSymbolKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:when test="key('typeDefIdentKey', .)">
                    <xsl:value-of select="key('typeDefIdentKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="key('typeDefLQSymbolKey', .)/aafm:QSymbol"/>
                </xsl:otherwise>
            </xsl:choose>
        </aafm:Type>
    </xsl:template>

    <xsl:template match="aafm:ElementOf">
        <aafm:ElementOf>
            <xsl:choose>
                <xsl:when test="key('typeDefSymbolKey', .)">
                    <xsl:value-of select="key('typeDefSymbolKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:when test="key('typeDefIdentKey', .)">
                    <xsl:value-of select="key('typeDefIdentKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="key('typeDefLQSymbolKey', .)/aafm:QSymbol"/>
                </xsl:otherwise>
            </xsl:choose>
        </aafm:ElementOf>
    </xsl:template>

    <xsl:template match="aafm:MetaDefRef">
        <aafm:MetaDefRef>
            <xsl:choose>
                <xsl:when test="key('propertyDefSymbolKey', .)">
                    <xsl:value-of select="key('propertyDefSymbolKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:when test="key('propertyDefIdentKey', .)">
                    <xsl:value-of select="key('propertyDefIdentKey', .)/aafm:QSymbol"/>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:value-of select="key('propertyDefLQSymbolKey', .)/aafm:QSymbol"/>
                </xsl:otherwise>
            </xsl:choose>
        </aafm:MetaDefRef>
    </xsl:template>
    
    
</xsl:stylesheet>

