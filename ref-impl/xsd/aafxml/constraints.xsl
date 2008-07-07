<?xml version="1.0" encoding="utf-8"?>
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

    
    <xsl:template name="constraints_for_sets">
        <xsl:param name="property" select="/.."/>
        <xsl:param name="isRoot" select="'true'"/>

        <xsl:variable name="baseTypeCategory">
            <xsl:call-template name="get_base_type_category">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $property/aafm:Type)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="baseTypeQSymbol">
            <xsl:call-template name="get_base_type_qsymbol">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $property/aafm:Type)"/>
            </xsl:call-template>
        </xsl:variable>

        <xsl:if test="$baseTypeCategory = 'TypeDefinitionSet'">
            <xsl:variable name="elementTypeSymbol">
                <xsl:value-of select="key('typeDefQSymbolKey', $baseTypeQSymbol)/aafm:ElementType"/>
            </xsl:variable>
            <xsl:variable name="elementBaseTypeQSymbol">
                <xsl:call-template name="get_base_type_qsymbol">
                    <xsl:with-param name="type" select="key('typeDefQSymbolKey', $elementTypeSymbol)"/>
                </xsl:call-template>
            </xsl:variable>
            <xsl:variable name="elementBaseTypeCategory">
                <xsl:call-template name="get_base_type_category">
                    <xsl:with-param name="type" select="key('typeDefQSymbolKey', $elementTypeSymbol)"/>
                </xsl:call-template>
            </xsl:variable>
            <xsl:choose>
                <xsl:when test="$elementBaseTypeCategory = 'TypeDefinitionStrongObjectReference'">
                    <xsl:variable name="referencedTypeQSymbol">
                        <xsl:value-of select="key('typeDefQSymbolKey', $elementBaseTypeQSymbol)/aafm:ReferencedType"/> 
                    </xsl:variable>
                    <xsl:variable name="isDefObject">
                        <xsl:call-template name="is_sub_class_of">
                            <xsl:with-param name="class" select="key('classDefQSymbolKey', $referencedTypeQSymbol)"/>
                            <xsl:with-param name="superClassQSymbol" select="concat($baselinePrefix, ':DefinitionObject')"/>
                        </xsl:call-template>
                    </xsl:variable>
                    <xsl:variable name="isMetaDefObject">
                        <xsl:call-template name="is_sub_class_of">
                            <xsl:with-param name="class" select="key('classDefQSymbolKey', $referencedTypeQSymbol)"/>
                            <xsl:with-param name="superClassQSymbol" select="concat($baselinePrefix, ':MetaDefinition')"/>
                        </xsl:call-template>
                    </xsl:variable>
                    <xsl:choose>
                        <xsl:when test="//aafm:Definitions/*[name() = 'TypeDefinitionWeakObjectReference']/aafm:TargetSet/aafm:MetaDefRef[position() = last() and . = $property/aafm:QSymbol]">
                            <xsl:if test="$isRoot = 'true'">
                                <xsl:choose>
                                    <xsl:when test="$isMetaDefObject = 'true'">
                                        <!-- ignore -->
                                    </xsl:when>
                                    <xsl:when test="$isDefObject = 'true'">
                                        <xs:key name="_K_{translate(aafm:QSymbol, ':', '_')}">
                                            <xs:selector xpath=".//{aafm:QSymbol}/*"/>
                                            <xs:field xpath="@{$baselinePrefix}:uid"/>
                                        </xs:key>
                                    </xsl:when>
                                    <xsl:otherwise>
                                        <xsl:variable name="uniqueIdentPropQSymbol">
                                            <xsl:call-template name="get_unique_identifier_property">
                                                <xsl:with-param name="class" select="key('classDefQSymbolKey', $referencedTypeQSymbol)"/>
                                            </xsl:call-template>
                                        </xsl:variable>
                                        <xsl:variable name="uniqueIdentTypeQSymbol">
                                            <xsl:value-of select="key('propertyDefQSymbolKey', $uniqueIdentPropQSymbol)/aafm:Type"/> 
                                        </xsl:variable>
                                        <xs:key name="_K_{translate(aafm:QSymbol, ':', '_')}">
                                            <xs:selector xpath=".//{aafm:QSymbol}/*"/>
                                            <xsl:call-template name="get_constraint_fields">
                                                <xsl:with-param name="propertyQSymbol" select="$uniqueIdentPropQSymbol"/> 
                                                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $uniqueIdentTypeQSymbol)"/>
                                            </xsl:call-template>
                                        </xs:key>
                                    </xsl:otherwise>
                                </xsl:choose>
                            </xsl:if>
                        </xsl:when>
                        <xsl:otherwise>
                            <xsl:if test="$isRoot != 'true'">
                                <xsl:choose>
                                    <xsl:when test="$isMetaDefObject = 'true'">
                                        <!-- ignore -->
                                    </xsl:when>
                                    <xsl:when test="$isDefObject = 'true'">
                                        <xs:unique name="_U_{translate(aafm:QSymbol, ':', '_')}">
                                            <xs:selector xpath="*"/>
                                            <xs:field xpath="@{$baselinePrefix}:uid"/>
                                        </xs:unique>
                                    </xsl:when>
                                    <xsl:otherwise>
                                        <xsl:variable name="uniqueIdentPropQSymbol">
                                            <xsl:call-template name="get_unique_identifier_property">
                                                <xsl:with-param name="class" select="key('classDefQSymbolKey', $referencedTypeQSymbol)"/>
                                            </xsl:call-template>
                                        </xsl:variable>
                                        <xsl:variable name="uniqueIdentTypeQSymbol">
                                            <xsl:value-of select="key('propertyDefQSymbolKey', $uniqueIdentPropQSymbol)/aafm:Type"/> 
                                        </xsl:variable>
                                        <xs:unique name="_U_{translate(aafm:QSymbol, ':', '_')}">
                                            <xs:selector xpath="*"/>
                                            <xsl:call-template name="get_constraint_fields">
                                                <xsl:with-param name="propertyQSymbol" select="$uniqueIdentPropQSymbol"/> 
                                                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $uniqueIdentTypeQSymbol)"/>
                                            </xsl:call-template>
                                        </xs:unique>
                                    </xsl:otherwise>
                                </xsl:choose>
                            </xsl:if>
                        </xsl:otherwise>
                    </xsl:choose>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:if test="$isRoot != 'true'"> 
                        <xs:unique name="_U_{translate(aafm:QSymbol, ':', '_')}">
                            <xs:selector xpath="{$elementTypeSymbol}"/>
                            <xsl:call-template name="get_constraint_fields">
                                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $elementTypeSymbol)"/>
                            </xsl:call-template>
                        </xs:unique>
                    </xsl:if>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:if>
        
    </xsl:template>

    
    <xsl:template name="constraints_for_weak_references">
        <xsl:param name="property" select="/.."/>
        <xsl:param name="typeQSymbol" select="''"/>
        <xsl:param name="elementQSymbol" select="''"/>
        
        <xsl:variable name="baseTypeCategory">
            <xsl:call-template name="get_base_type_category">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $typeQSymbol)"/>
            </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="baseTypeQSymbol">
            <xsl:call-template name="get_base_type_qsymbol">
                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $typeQSymbol)"/>
            </xsl:call-template>
        </xsl:variable>
        
        <xsl:choose>
            <xsl:when test="$baseTypeCategory = 'TypeDefinitionSet' or $baseTypeCategory = 'TypeDefinitionVariableArray' or $baseTypeCategory = 'TypeDefinitionFixedArray'">
                <xsl:call-template name="constraints_for_weak_references">
                    <xsl:with-param name="property" select="$property"/>
                    <xsl:with-param name="typeQSymbol" select="key('typeDefQSymbolKey', $baseTypeQSymbol)/aafm:ElementType"/>
                    <xsl:with-param name="elementQSymbol" select="key('typeDefQSymbolKey', $baseTypeQSymbol)/aafm:ElementType"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="$baseTypeCategory = 'TypeDefinitionWeakObjectReference'">
                <xsl:variable name="targetPropertyQSymbol">
                    <xsl:value-of select="key('typeDefQSymbolKey', $baseTypeQSymbol)/aafm:TargetSet/aafm:MetaDefRef[position() = last()]"/> 
                </xsl:variable>
                <xsl:variable name="referencedTypeQSymbol">
                    <xsl:value-of select="key('typeDefQSymbolKey', $baseTypeQSymbol)/aafm:ReferencedType"/>
                </xsl:variable>
                <xsl:variable name="isMetaDefRef">
                    <xsl:call-template name="is_sub_class_of">
                        <xsl:with-param name="class" select="key('classDefQSymbolKey', $referencedTypeQSymbol)"/>
                        <xsl:with-param name="superClassQSymbol" select="concat($baselinePrefix, ':MetaDefinition')"/>
                    </xsl:call-template>
                </xsl:variable>
                <xsl:choose>
                    <xsl:when test="$isMetaDefRef = 'true'">
                        <!-- ignore -->
                    </xsl:when>
                    <xsl:otherwise>
                        <xs:keyref name="_KR_{translate($property/aafm:QSymbol, ':', '_')}" refer="{$baselinePrefix}:_K_{translate($targetPropertyQSymbol, ':', '_')}">
                            <xsl:choose>
                                <xsl:when test="$elementQSymbol = ''">
                                    <xs:selector xpath=".//{$property/aafm:QSymbol}"/>
                                </xsl:when>
                                <xsl:otherwise>
                                    <xs:selector xpath=".//{$property/aafm:QSymbol}/{$elementQSymbol}"/>
                                </xsl:otherwise>
                            </xsl:choose>
                            <xsl:call-template name="get_constraint_fields">
                                <xsl:with-param name="type" select="key('typeDefQSymbolKey', $baseTypeQSymbol)"/>
                            </xsl:call-template>
                        </xs:keyref>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:when>
        </xsl:choose>
        
    </xsl:template>

    
    <xsl:template name="get_constraint_fields">
        <xsl:param name="type" select="/.."/>
        <xsl:param name="propertyQSymbol" select="''"/> 
        
        <xsl:variable name="isSimpleType">
            <xsl:call-template name="is_xsd_simple_type">
                <xsl:with-param name="type" select="$type"/>
            </xsl:call-template>
        </xsl:variable>
        
        <xsl:choose>
            <xsl:when test="$isSimpleType = 'true'">
                <xsl:choose>
                    <xsl:when test="$propertyQSymbol = ''">
                        <xs:field xpath="."/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xs:field xpath="{$propertyQSymbol}"/>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
                <xsl:message terminate="true">Constraint fields not yet implemented for non-simpleType types</xsl:message>
            </xsl:otherwise>
        </xsl:choose>
        
    </xsl:template>
    
    
    <xsl:template name="get_unique_identifier_property">
        <xsl:param name="class" select="/.."/>
        
        <xsl:variable name="propertyQSymbol">
            <xsl:value-of select="key('propertyDefMemberOfQSymbolKey', $class/aafm:QSymbol)[aafm:IsUniqueIdentifier = 'true']/aafm:QSymbol"/>
        </xsl:variable>
        <xsl:choose>
            <xsl:when test="$propertyQSymbol = ''">
                <xsl:variable name="isRootClass">
                    <xsl:call-template name="is_root_class">
                        <xsl:with-param name="class" select="$class"/>
                    </xsl:call-template>
                </xsl:variable>
                <xsl:if test="$isRootClass = 'true'">
                    <xsl:message terminate="yes">Class has no unique identifier property</xsl:message>
                </xsl:if>
                <xsl:call-template name="get_unique_identifier_property">
                    <xsl:with-param name="class" select="key('classDefQSymbolKey', $class/aafm:ParentClass)"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$propertyQSymbol"/>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>

</xsl:stylesheet>
