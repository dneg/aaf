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


    <xsl:template match="aafm:TypeDefinitionCharacter">

        <xs:complexType name="{aafm:Symbol}">
            <xs:simpleContent>
                <xs:extension base="xs:string">
                    <xs:attribute ref="{$baselinePrefix}:escaped" use="optional"/>
                </xs:extension>
            </xs:simpleContent>
        </xs:complexType>

        <xsl:call-template name="type_as_element"/>
        
    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionEnumeration">

        <xsl:choose>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':Boolean')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="xs:boolean"/>
                </xs:simpleType>
            </xsl:when>
            <xsl:otherwise>
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="xs:token">
                        <xsl:for-each select="aafm:Elements/aafm:Name">
                            <xs:enumeration value="{.}"/>
                        </xsl:for-each>
                    </xs:restriction>
                </xs:simpleType>
            </xsl:otherwise>
        
        </xsl:choose>

        <xsl:call-template name="type_as_element"/>
        
    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionExtendibleEnumeration">

        <xs:simpleType name="{aafm:Symbol}">
            <xs:restriction base="xs:token">
                <xsl:for-each select="//aafm:ExtendibleEnumerationElement[aafm:ElementOf = current()/aafm:QSymbol]">
                    <xs:enumeration value="{aafm:Name}"/>
                </xsl:for-each>
            </xs:restriction>
        </xs:simpleType>

        <xsl:call-template name="type_as_element"/>

    </xsl:template>

    <xsl:template match="aafm:ExtendibleEnumerationElement">
        <!-- Do nothing - processed in aafm:TypeDefinitionExtendibleEnumeration -->
    </xsl:template>
    

    <xsl:template match="aafm:TypeDefinitionFixedArray">

        <xsl:variable name="elementBaseTypeCategory">
            <xsl:call-template name="get_base_type_category">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:ElementType)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="elementBaseTypeQSymbol">
            <xsl:call-template name="get_base_type_qsymbol">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:ElementType)"/>
            </xsl:call-template>
        </xsl:variable>

        <xsl:choose>

            <xsl:when test="$elementBaseTypeCategory = 'TypeDefinitionStrongObjectReference'">
                <xs:complexType name="{aafm:Symbol}">
                    <xs:group ref="{substring-before($elementBaseTypeQSymbol, ':')}:_G_{substring-after($elementBaseTypeQSymbol, ':')}" minOccurs="{aafm:ElementCount}" maxOccurs="{aafm:ElementCount}"/>
                </xs:complexType>
            </xsl:when>
            <xsl:otherwise>
                <xs:complexType name="{aafm:Symbol}">
                    <xs:sequence>
                        <xs:element ref="{aafm:ElementType}" minOccurs="{aafm:ElementCount}" maxOccurs="{aafm:ElementCount}"/>
                    </xs:sequence>
                </xs:complexType>
            </xsl:otherwise>

        </xsl:choose>


    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionIndirect">

        <xs:complexType name="{aafm:Symbol}">
            <xs:complexContent mixed="true">
                <xs:restriction base="xs:anyType">
                    <xs:sequence>
                        <xs:any minOccurs="0" maxOccurs="unbounded" processContents="skip"/>
                    </xs:sequence>
                    <xs:attribute ref="{$baselinePrefix}:actualType" use="required"/>
                    <xs:attribute ref="{$baselinePrefix}:escaped" use="optional"/>
                </xs:restriction>
            </xs:complexContent>
        </xs:complexType>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionInteger">

        <xs:simpleType name="{aafm:Symbol}">
            <xs:union>

                <xsl:choose>

                    <xsl:when test="aafm:Size = 1">
                        <xsl:choose>
                            <xsl:when test="aafm:IsSigned = 'true'">
                                <xsl:attribute name="memberTypes">xs:byte</xsl:attribute> 
                                <xs:simpleType>
                                    <xs:restriction base="xs:string">
                                        <xs:pattern>
                                            <xsl:attribute name="value">
                                                <xsl:text>0x[0-9a-fA-F]{1,2}</xsl:text>
                                            </xsl:attribute>
                                        </xs:pattern>
                                    </xs:restriction>
                                </xs:simpleType>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:attribute name="memberTypes">xs:unsignedByte</xsl:attribute> 
                                <xs:simpleType>
                                    <xs:restriction base="xs:string">
                                        <xs:pattern>
                                            <xsl:attribute name="value">
                                                <xsl:text>0x[0-9a-fA-F]{1,2}</xsl:text>
                                            </xsl:attribute>
                                        </xs:pattern>
                                    </xs:restriction>
                                </xs:simpleType>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:when>

                    <xsl:when test="aafm:Size = 2">
                        <xsl:choose>
                            <xsl:when test="aafm:IsSigned = 'true'">
                                <xsl:attribute name="memberTypes">xs:short</xsl:attribute> 
                                <xs:simpleType>
                                    <xs:restriction base="xs:string">
                                        <xs:pattern>
                                            <xsl:attribute name="value">
                                                <xsl:text>0x[0-9a-fA-F]{1,4}</xsl:text>
                                            </xsl:attribute>
                                        </xs:pattern>
                                    </xs:restriction>
                                </xs:simpleType>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:attribute name="memberTypes">xs:unsignedShort</xsl:attribute> 
                                <xs:simpleType>
                                    <xs:restriction base="xs:string">
                                        <xs:pattern>
                                            <xsl:attribute name="value">
                                                <xsl:text>0x[0-9a-fA-F]{1,4}</xsl:text>
                                            </xsl:attribute>
                                        </xs:pattern>
                                    </xs:restriction>
                                </xs:simpleType>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:when>

                    <xsl:when test="aafm:Size = 4">
                        <xsl:choose>
                            <xsl:when test="aafm:IsSigned = 'true'">
                                <xsl:attribute name="memberTypes">xs:integer</xsl:attribute> 
                                <xs:simpleType>
                                    <xs:restriction base="xs:string">
                                        <xs:pattern>
                                            <xsl:attribute name="value">
                                                <xsl:text>0x[0-9a-fA-F]{1,8}</xsl:text>
                                            </xsl:attribute>
                                        </xs:pattern>
                                    </xs:restriction>
                                </xs:simpleType>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:attribute name="memberTypes">xs:unsignedInt</xsl:attribute> 
                                <xs:simpleType>
                                    <xs:restriction base="xs:string">
                                        <xs:pattern>
                                            <xsl:attribute name="value">
                                                <xsl:text>0x[0-9a-fA-F]{1,8}</xsl:text>
                                            </xsl:attribute>
                                        </xs:pattern>
                                    </xs:restriction>
                                </xs:simpleType>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:when>

                    <xsl:when test="aafm:Size = 8">
                        <xsl:choose>
                            <xsl:when test="aafm:IsSigned = 'true'">
                                <xsl:attribute name="memberTypes">xs:long</xsl:attribute> 
                                <xs:simpleType>
                                    <xs:restriction base="xs:string">
                                        <xs:pattern>
                                            <xsl:attribute name="value">
                                                <xsl:text>0x[0-9a-fA-F]{1,16}</xsl:text>
                                            </xsl:attribute>
                                        </xs:pattern>
                                    </xs:restriction>
                                </xs:simpleType>
                            </xsl:when>
                            <xsl:otherwise>
                                <xsl:attribute name="memberTypes">xs:unsignedLong</xsl:attribute> 
                                <xs:simpleType>
                                    <xs:restriction base="xs:string">
                                        <xs:pattern>
                                            <xsl:attribute name="value">
                                                <xsl:text>0x[0-9a-fA-F]{1,16}</xsl:text>
                                            </xsl:attribute>
                                        </xs:pattern>
                                    </xs:restriction>
                                </xs:simpleType>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:when>

                </xsl:choose>

            </xs:union>
        </xs:simpleType>

        <xsl:call-template name="type_as_element"/>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionOpaque">

        <xs:complexType name="{aafm:Symbol}">
            <xs:simpleContent>
                <xs:extension base="{$baselinePrefix}:_T_HexByteArray">
                    <xs:attribute ref="{$baselinePrefix}:actualType" use="required"/>
                    <xs:attribute ref="{$baselinePrefix}:byteOrder" use="required"/>
                </xs:extension>
            </xs:simpleContent>
        </xs:complexType>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionRecord">

        <xsl:choose>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':Rational')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="xs:string">
                        <xs:pattern>
                            <xsl:attribute name="value">
                                <xsl:text>\-?\d{1,10}(/\-?\d{1,10})?</xsl:text>
                            </xsl:attribute>
                        </xs:pattern>
                    </xs:restriction>
                </xs:simpleType>
            </xsl:when>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':AUID')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="{$baselinePrefix}:_T_AUID"/>
                </xs:simpleType>
            </xsl:when>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':MobIDType')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="xs:anyURI">
                        <xs:pattern>
                            <xsl:attribute name="value">
                                <xsl:text>urn:x-umid:[0-9a-fA-F]{24}-13-[0-9a-fA-F]{6}-[0-9a-fA-F]{32}</xsl:text>
                            </xsl:attribute>
                        </xs:pattern>
                        <xs:pattern>
                            <xsl:attribute name="value">
                                <xsl:text>urn:x-umid:000000000000000000000000-00-000000-00000000000000000000000000000000</xsl:text>
                            </xsl:attribute>
                        </xs:pattern>
                    </xs:restriction>
                </xs:simpleType>
            </xsl:when>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':DateStruct')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:union>
                        <xs:simpleType>
                            <xs:restriction base="xs:date">
                                <xs:pattern>
                                    <xsl:attribute name="value">
                                        <xsl:text>.+(((\+|\-)\d\d:\d\d)|Z)</xsl:text>
                                    </xsl:attribute>
                                </xs:pattern>
                            </xs:restriction>
                        </xs:simpleType>
                        <xs:simpleType>
                            <xs:restriction base="xs:string">
                                <xs:enumeration>
                                    <xsl:attribute name="value">
                                        <xsl:text>0000-00-00Z</xsl:text>
                                    </xsl:attribute>
                                </xs:enumeration>
                            </xs:restriction>
                        </xs:simpleType>
                    </xs:union>
                </xs:simpleType>
            </xsl:when>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':TimeStruct')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:union>
                        <xs:simpleType>
                            <xs:restriction base="xs:time">
                                <xs:pattern>
                                    <xsl:attribute name="value">
                                        <xsl:text>.+(((\+|\-)\d\d:\d\d)|Z)</xsl:text>
                                    </xsl:attribute>
                                </xs:pattern>
                            </xs:restriction>
                        </xs:simpleType>
                        <xs:simpleType>
                            <xs:restriction base="xs:string">
                                <xs:enumeration>
                                    <xsl:attribute name="value">
                                        <xsl:text>00:00:00.00Z</xsl:text>
                                    </xsl:attribute>
                                </xs:enumeration>
                            </xs:restriction>
                        </xs:simpleType>
                    </xs:union>
                </xs:simpleType>
            </xsl:when>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':TimeStamp')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:union>
                        <xs:simpleType>
                            <xs:restriction base="xs:dateTime">
                                <xs:pattern>
                                    <xsl:attribute name="value">
                                        <xsl:text>.+(((\+|\-)\d\d:\d\d)|Z)</xsl:text>
                                    </xsl:attribute>
                                </xs:pattern>
                            </xs:restriction>
                        </xs:simpleType>
                        <xs:simpleType>
                            <xs:restriction base="xs:string">
                                <xs:enumeration>
                                    <xsl:attribute name="value">
                                        <xsl:text>0000-00-00T00:00:00.00Z</xsl:text>
                                    </xsl:attribute>
                                </xs:enumeration>
                            </xs:restriction>
                        </xs:simpleType>
                    </xs:union>
                </xs:simpleType>
            </xsl:when>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':VersionType')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="xs:string">
                        <xs:pattern>
                            <xsl:attribute name="value">
                                <xsl:text>\-?\d{1,3}\.\-?\d{1,3}</xsl:text>
                            </xsl:attribute>
                        </xs:pattern>
                    </xs:restriction>
                </xs:simpleType>
            </xsl:when>

            <xsl:otherwise>
                <xs:complexType name="{aafm:Symbol}">
                    <xs:sequence>
                        <xsl:for-each select="aafm:Members/aafm:Name">
                            <xsl:variable name="pos">
                                <xsl:value-of select="position()"/>
                            </xsl:variable>
                            <xs:element name="{.}" type="{../aafm:Type[position() = $pos]}"/>
                        </xsl:for-each>
                    </xs:sequence>
                </xs:complexType>
            </xsl:otherwise>

        </xsl:choose>

        <xsl:call-template name="type_as_element"/>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionRename">

        <xsl:variable name="renamedBaseTypeCategory">
            <xsl:call-template name="get_base_type_category">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:RenamedType)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="renamedBaseTypeQSymbol">
            <xsl:call-template name="get_base_type_qsymbol">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:RenamedType)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="isSimpleType">
            <xsl:call-template name="is_xsd_simple_type">
                <xsl:with-param name="type" select="."/>
            </xsl:call-template>
        </xsl:variable>

        <xsl:choose>
            <xsl:when test="$renamedBaseTypeCategory = 'TypeDefinitionString'">
                <xsl:variable name="elementTypeQSymbol">
                    <xsl:value-of select="key('typeDefQSymbolKey', $renamedBaseTypeQSymbol)/aafm:ElementType"/>
                </xsl:variable>
                <xsl:variable name="elementBaseTypeCategory">
                    <xsl:call-template name="get_base_type_category">
                        <xsl:with-param name="type" select="key('typeDefQSymbolKey', $elementTypeQSymbol)"/>
                    </xsl:call-template>
                </xsl:variable>
                <xsl:choose>
                    <xsl:when test="$elementBaseTypeCategory = 'TypeDefinitionCharacter'">
                        <xs:complexType name="{aafm:Symbol}">
                            <xs:simpleContent>
                                <xs:extension base="{aafm:RenamedType}"/>
                            </xs:simpleContent>
                        </xs:complexType>
                    </xsl:when>
                    <xsl:otherwise>
                        <xs:simpleType name="{aafm:Symbol}">
                            <xs:restriction base="{aafm:RenamedType}"/>
                        </xs:simpleType>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:when>
            <xsl:when test="$isSimpleType = 'true'">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="{aafm:RenamedType}"/>
                </xs:simpleType>
            </xsl:when>
            <xsl:otherwise>
                <xs:complexType name="{aafm:Symbol}">
                    <xs:complexContent>
                        <xs:extension base="{aafm:RenamedType}"/>
                    </xs:complexContent>
                </xs:complexType>
            </xsl:otherwise>
        </xsl:choose>

        <xsl:call-template name="type_as_element"/>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionSet">

        <xsl:variable name="elementBaseTypeCategory">
            <xsl:call-template name="get_base_type_category">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:ElementType)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="elementBaseTypeQSymbol">
            <xsl:call-template name="get_base_type_qsymbol">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:ElementType)"/>
            </xsl:call-template>
        </xsl:variable>

        <xsl:choose>

            <xsl:when test="$elementBaseTypeCategory = 'TypeDefinitionStrongObjectReference'">
                <xs:complexType name="{aafm:Symbol}">
                    <xs:group ref="{substring-before($elementBaseTypeQSymbol, ':')}:_G_{substring-after($elementBaseTypeQSymbol, ':')}" minOccurs="0" maxOccurs="unbounded"/>
                </xs:complexType>
            </xsl:when>
            <xsl:otherwise>
                <xs:complexType name="{aafm:Symbol}">
                    <xs:sequence>
                        <xs:element ref="{aafm:ElementType}" minOccurs="0" maxOccurs="unbounded"/>
                    </xs:sequence>
                </xs:complexType>
            </xsl:otherwise>

        </xsl:choose>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionStream">

        <xs:complexType name="{aafm:Symbol}">
            <xs:attribute ref="{$baselinePrefix}:stream" use="required"/>
            <xs:attribute ref="{$baselinePrefix}:byteOrder" use="optional"/>
        </xs:complexType>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionString">

        <xsl:variable name="elementBaseTypeCategory">
            <xsl:call-template name="get_base_type_category">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:ElementType)"/>
            </xsl:call-template>
        </xsl:variable>

        <xsl:choose>

            <xsl:when test="$elementBaseTypeCategory = 'TypeDefinitionCharacter'">
                <xs:complexType name="{aafm:Symbol}">
                    <xs:simpleContent>
                        <xs:extension base="xs:string">
                            <xs:attribute ref="{$baselinePrefix}:escaped" use="optional"/>
                        </xs:extension>
                    </xs:simpleContent>
                </xs:complexType>
            </xsl:when>
            <xsl:otherwise>
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:list itemType="{aafm:ElementType}"/>
                </xs:simpleType>
            </xsl:otherwise>
        </xsl:choose>

        <xsl:if test="aafm:QSymbol = concat($baselinePrefix, ':String')">
            <xs:element name="{aafm:Symbol}" type="{aafm:QSymbol}"/>
        </xsl:if>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionStrongObjectReference">

        <xs:group name="_G_{aafm:Symbol}">
            <xs:choice>
                <xsl:call-template name="class_reference_group">
                    <xsl:with-param name="classQSymbol" select="aafm:ReferencedType"/>
                </xsl:call-template>
            </xs:choice>
        </xs:group>
        <xs:complexType name="{aafm:Symbol}">
            <xs:group ref="{substring-before(aafm:QSymbol, ':')}:_G_{substring-after(aafm:QSymbol, ':')}"/>
        </xs:complexType>

    </xsl:template>


    <xsl:template match="aafm:TypeDefinitionVariableArray">

        <xsl:variable name="elementBaseTypeCategory">
            <xsl:call-template name="get_base_type_category">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:ElementType)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="elementBaseTypeQSymbol">
            <xsl:call-template name="get_base_type_qsymbol">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', current()/aafm:ElementType)"/>
            </xsl:call-template>
        </xsl:variable>
        
        <xsl:choose>
            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':DataValue')">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="{$baselinePrefix}:_T_HexByteArray"/>
                </xs:simpleType>
            </xsl:when>

            <xsl:when test="aafm:QSymbol = concat($baselinePrefix, ':StringArray')">
                <xs:complexType name="{aafm:Symbol}">
                    <xs:sequence>
                        <xs:element ref="{$baselinePrefix}:String" minOccurs="0" maxOccurs="unbounded"/>
                    </xs:sequence>
                </xs:complexType>
            </xsl:when>

            <xsl:otherwise>
                <xsl:choose>
                    <xsl:when test="$elementBaseTypeCategory = 'TypeDefinitionStrongObjectReference'">
                        <xs:complexType name="{aafm:Symbol}">
                            <xs:group ref="{substring-before($elementBaseTypeQSymbol, ':')}:_G_{substring-after($elementBaseTypeQSymbol, ':')}" minOccurs="0" maxOccurs="unbounded"/>
                        </xs:complexType>
                    </xsl:when>
                    <xsl:otherwise>
                        <xs:complexType name="{aafm:Symbol}">
                            <xs:sequence>
                                <xs:element ref="{aafm:ElementType}" minOccurs="0" maxOccurs="unbounded"/>
                            </xs:sequence>
                        </xs:complexType>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:otherwise>

        </xsl:choose>

    </xsl:template>



    <xsl:template match="aafm:TypeDefinitionWeakObjectReference">

        <xsl:variable name="uniqueIdPropType">
            <xsl:call-template name="get_unique_id_property_type">
                <xsl:with-param name="class" select="key('classDefQSymbolKey', current()/aafm:ReferencedType)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="isSimpleType">
            <xsl:call-template name="is_xsd_simple_type">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $uniqueIdPropType)"/>
            </xsl:call-template>
        </xsl:variable>

        <xsl:choose>
            <xsl:when test="$isSimpleType = 'true'">
                <xs:simpleType name="{aafm:Symbol}">
                    <xs:restriction base="{$uniqueIdPropType}"/>
                </xs:simpleType>
            </xsl:when>
            <xsl:otherwise>
                <xs:complexType name="{aafm:Symbol}">
                    <xs:complexContent>
                        <xs:extension base="{$uniqueIdPropType}"/>
                    </xs:complexContent>
                </xs:complexType>
            </xsl:otherwise>
        </xsl:choose>

        <xsl:call-template name="type_as_element"/>

    </xsl:template>


    
    <xsl:template name="type_as_element">
    
        <xsl:if test="key('elementTypeQSymbolKey', current()/aafm:QSymbol)">
            <xs:element name="{aafm:Symbol}" type="{aafm:QSymbol}"/>
        </xsl:if>
    
    </xsl:template>
    
    <xsl:template name="class_reference_group">
        <xsl:param name="classQSymbol" select="''"/>
        
        <xs:element ref="{$classQSymbol}"/>
        
        <xsl:for-each select="key('classDefParentQSymbolKey', $classQSymbol)">
            <xsl:call-template name="class_reference_group">
                <xsl:with-param name="classQSymbol" select="aafm:QSymbol"/>
            </xsl:call-template>
        </xsl:for-each>
    </xsl:template>
    
    
</xsl:stylesheet>

