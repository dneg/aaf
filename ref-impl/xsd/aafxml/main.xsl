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
<xsl:stylesheet version="1.1" 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    xmlns:xs="http://www.w3.org/2001/XMLSchema" 
    xmlns:aafm="http://www.aafassociation.org/aafx/v1.1/20050628" 
    xmlns:extc="http://exslt.org/common"
    exclude-result-prefixes="aafm extc">

    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

    <xsl:include href="util.xsl"/>
    <xsl:include href="constraints.xsl"/>
    <xsl:include href="classdef.xsl"/>
    <xsl:include href="propdef.xsl"/>
    <xsl:include href="typedef.xsl"/>


    <xsl:variable name="baselinePrefix">
        <xsl:value-of select="//aafm:Symbolspace[@isBaseline = 'true']/aafm:Prefix"/>
    </xsl:variable>


    <xsl:key name="typeDefQSymbolKey" match="//aafm:Definitions/*[starts-with(name(), 'TypeDefinition')]" use="aafm:QSymbol"/>
    <xsl:key name="elementTypeQSymbolKey" match="//aafm:Definitions/*[name() = 'TypeDefinitionFixedArray' or name() = 'TypeDefinitionSet' or name() = 'TypeDefinitionVariableArray']" use="aafm:ElementType"/>
    <xsl:key name="classDefQSymbolKey" match="//aafm:ClassDefinition" use="aafm:QSymbol"/>
    <xsl:key name="classDefParentQSymbolKey" match="//aafm:ClassDefinition" use="aafm:ParentClass"/>
    <xsl:key name="propertyDefQSymbolKey" match="//aafm:PropertyDefinition" use="aafm:QSymbol"/>
    <xsl:key name="propertyDefMemberOfQSymbolKey" match="//aafm:PropertyDefinition" use="aafm:MemberOf"/>
    

    
    <xsl:template match="/">
        <xsl:apply-templates select="aafm:AAFFileModel/aafm:Symbolspaces/aafm:Symbolspace"/>
    </xsl:template>
    
    
    <xsl:template match="aafm:Symbolspace">

        <xsl:choose>
            <xsl:when test="@isBaseline = 'true'">
                <xsl:call-template name="symbolspace_to_xsd"/> 
            </xsl:when>
            <xsl:otherwise>
                <xsl:document href="{aafm:Prefix}.xsd">
                    <xsl:call-template name="symbolspace_to_xsd"/> 
                </xsl:document>
            </xsl:otherwise>
        </xsl:choose>

    </xsl:template>

    
    <xsl:template name="symbolspace_to_xsd">

        <xsl:variable name="dummy">
            <xsl:for-each select="../aafm:Symbolspace"> 
                <xsl:element name="{aafm:Prefix}:dummy" namespace="{aafm:URI}"/>
            </xsl:for-each>
        </xsl:variable>

        <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
            
            <xsl:for-each select="../aafm:Symbolspace"> 
                <xsl:copy-of select="extc:node-set($dummy)/*/namespace::*[. = current()/aafm:URI]"/>
            </xsl:for-each>
            <xsl:attribute name="targetNamespace"><xsl:value-of select="aafm:URI"/></xsl:attribute>
            <xsl:attribute name="elementFormDefault">qualified</xsl:attribute>
            <xsl:attribute name="attributeFormDefault">unqualified</xsl:attribute>

            <xsl:if test="@isBaseline = 'true'">
                <xs:include schemaLocation="metadict.xsd"/>
            </xsl:if>
            
            <xsl:variable name="thisSymbolspace">
                <xsl:value-of select="aafm:URI"/>
            </xsl:variable>
            
            <xsl:for-each select="../aafm:Symbolspace"> 
                <xsl:if test="aafm:URI != $thisSymbolspace">
                    <xs:import namespace="{aafm:URI}" schemaLocation="{aafm:Prefix}.xsd"/>
                </xsl:if>
            </xsl:for-each>
            
            <xsl:if test="@isBaseline = 'true'">
    
                <xsl:text>&#10;&#10;&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment>  AAF Root Element  </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
        
                <xs:element name="AAF">
                    <xs:complexType>
                        <xs:sequence>
                            <xs:element name="Extensions" minOccurs="0">
                                <xs:complexType>
                                    <xs:sequence>
                                        <xs:element ref="{$baselinePrefix}:Extension" minOccurs="0" maxOccurs="unbounded"/>
                                    </xs:sequence>
                                </xs:complexType>
                            </xs:element>
                            <xs:element ref="{$baselinePrefix}:Header"/>
                        </xs:sequence>
                        <xs:attribute use="required" type="{$baselinePrefix}:_T_Version" name="version"/>
                    </xs:complexType>
        
                    <xs:unique name="_U_Extensions_Identification">
                        <xs:selector xpath="{$baselinePrefix}:Extensions/{$baselinePrefix}:Extension"/>
                        <xs:field xpath="{$baselinePrefix}:Identification"/>
                    </xs:unique>
                    <xs:unique name="_U_Extensions_Symbolspace">
                        <xs:selector xpath="{$baselinePrefix}:Extensions/{$baselinePrefix}:Extension"/>
                        <xs:field xpath="{$baselinePrefix}:Symbolspace"/>
                    </xs:unique>
                    
                    <xsl:for-each select="//aafm:PropertyDefinition">
                        <xsl:call-template name="constraints_for_sets">
                            <xsl:with-param name="property" select="."/>
                            <xsl:with-param name="isRoot" select="'true'"/>
                        </xsl:call-template>
                        <xsl:call-template name="constraints_for_weak_references">
                            <xsl:with-param name="property" select="."/>
                            <xsl:with-param name="typeQSymbol" select="aafm:Type"/>
                        </xsl:call-template>
                    </xsl:for-each>
                    
                </xs:element>
        
                <xsl:text>&#10;&#10;&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment>  Global attributes </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
        
                <xs:attribute name="uid" type="{$baselinePrefix}:_T_AUID"/>
                <xs:attribute name="byteOrder" type="{$baselinePrefix}:_T_ByteOrder"/>
                <xs:attribute name="stream" type="xs:ENTITY"/>
                <xs:attribute name="actualType" type="{$baselinePrefix}:_T_AUID"/>
                <xs:attribute name="escaped" type="xs:boolean"/>
        
        
                <xsl:text>&#10;&#10;&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment>  AAF-X Types  </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
                <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
                <xsl:text>&#10;</xsl:text>
        
                <xs:simpleType name="_T_ByteOrder">
                    <xs:restriction base="xs:string">
                        <xs:enumeration value="BigEndian"/>
                        <xs:enumeration value="LittleEndian"/>
                    </xs:restriction>
                </xs:simpleType>
                <xs:simpleType name="_T_HexByteArray">
                    <xs:restriction base="xs:string">
                        <xs:pattern value="(\s*[0-9a-fA-F][0-9a-fA-F])*\s*"/>
                    </xs:restriction>
                </xs:simpleType>
                <xs:simpleType name="_T_AUID">
                    <xs:union memberTypes="xs:token">
                        <xs:simpleType>
                            <xs:restriction base="xs:anyURI">
                                <xs:pattern>
                                    <xsl:attribute name="value">
                                        <xsl:text>urn:x-ul:[0-9a-fA-F]{8}\.([0-9a-fA-F]){4}\.([0-9a-fA-F]){4}\.([0-9a-fA-F]){8}\.([0-9a-fA-F]){8}</xsl:text>
                                    </xsl:attribute>
                                </xs:pattern>
                                <xs:pattern>
                                    <xsl:attribute name="value">
                                        <xsl:text>urn:uuid:[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}</xsl:text>
                                    </xsl:attribute>
                                </xs:pattern>
                            </xs:restriction>
                        </xs:simpleType>
                        <xs:simpleType>
                            <xs:restriction base="xs:string">
                                <xs:pattern>
                                    <xsl:attribute name="value">
                                        <xsl:text>[^\s]+ [^\s]+</xsl:text>
                                    </xsl:attribute>
                                </xs:pattern>
                            </xs:restriction>
                        </xs:simpleType>
                    </xs:union>
                </xs:simpleType>
                <xs:simpleType name="_T_Version">
                    <xs:restriction base="xs:string">
                        <xs:pattern>
                            <xsl:attribute name="value">
                                <xsl:text>\d{1,3}\.\d{1,3}</xsl:text>
                            </xsl:attribute>
                        </xs:pattern>
                    </xs:restriction>
                </xs:simpleType>
                
                
            </xsl:if>
    
    
            <xsl:text>&#10;&#10;&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment>  Classes </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
    
            <xsl:for-each select="//aafm:ClassDefinition[aafm:Symbolspace = $thisSymbolspace]">
                <xsl:sort select="aafm:Symbol" order="ascending"/>
                <xsl:apply-templates select="."/>
            </xsl:for-each>
    
    
            <xsl:text>&#10;&#10;&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment>  Properties </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
    
            <xsl:for-each select="//aafm:PropertyDefinition[aafm:Symbolspace = $thisSymbolspace]">
                <xsl:sort select="aafm:Symbol" order="ascending"/>
                <xsl:apply-templates select="."/>
            </xsl:for-each>
    
    
            <xsl:text>&#10;&#10;&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment>  Types </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
            <xsl:comment> ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ </xsl:comment>
            <xsl:text>&#10;</xsl:text>
    
            <xsl:for-each select="//aafm:Definitions/*[starts-with(name(), 'TypeDefinition') and aafm:Symbolspace = $thisSymbolspace]">
                <xsl:sort select="aafm:Symbol" order="ascending"/>
                <xsl:apply-templates select="."/>
            </xsl:for-each>

        </xs:schema>
        
    </xsl:template>

</xsl:stylesheet>
