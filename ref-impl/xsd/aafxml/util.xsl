<?xml version="1.0" encoding="utf-8"?>
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
<xsl:stylesheet 
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
    version="1.0" 
    xmlns:xs="http://www.w3.org/2001/XMLSchema" 
    xmlns:aafm="http://www.aafassociation.org/aafx/v1.1/20050628" 
    exclude-result-prefixes="aafm">

    <xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

    
    <!-- get the base type category, i.e. beyond renames -->
    <xsl:template name="get_base_type_category">
        <xsl:param name="type" select="/.."/>
        
        <xsl:choose>
            <xsl:when test="name($type) = 'TypeDefinitionRename'">
                <xsl:call-template name="get_base_type_category">
                    <xsl:with-param name="type" select="key('typeDefQSymbolKey', $type/aafm:RenamedType)"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="name($type)"/>
            </xsl:otherwise>
        </xsl:choose>
        
    </xsl:template>
    
    
    <!-- get the base type symbol, i.e. beyond renames -->
    <xsl:template name="get_base_type_qsymbol">
        <xsl:param name="type" select="/.."/>
        
        <xsl:choose>
            <xsl:when test="name($type) = 'TypeDefinitionRename'">
                <xsl:call-template name="get_base_type_qsymbol">
                    <xsl:with-param name="type" select="key('typeDefQSymbolKey', $type/aafm:RenamedType)"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$type/aafm:QSymbol"/>
            </xsl:otherwise>
        </xsl:choose>
        
    </xsl:template>
    
    <!-- check if the class is the root class -->
    <!-- a class is the root class if the ParentClass element is missing -->
    <xsl:template name="is_root_class">
        <xsl:param name="class" select="/.."/>

        <xsl:choose>
            <xsl:when test="$class/aafm:ParentClass">
                <xsl:value-of select="'false'"/>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="'true'"/>
            </xsl:otherwise>
        </xsl:choose>

    </xsl:template>


    <!-- check if $class is a sub class of class with symbol $superClassSymbol -->
    <xsl:template name="is_sub_class_of">
        <xsl:param name="class" select="/.."/> 
        <xsl:param name="superClassQSymbol" select="''"/>
        
        <xsl:choose>
            <xsl:when test="$class/aafm:QSymbol = $superClassQSymbol">
                <xsl:value-of select="'true'"/>
            </xsl:when>
            <xsl:when test="$class/aafm:ParentClass">
                <xsl:call-template name="is_sub_class_of">
                    <xsl:with-param name="class" select="key('classDefQSymbolKey', $class/aafm:ParentClass)"/>
                    <xsl:with-param name="superClassQSymbol" select="$superClassQSymbol"/>                    
                </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="'false'"/>
            </xsl:otherwise>
        </xsl:choose>
        
    </xsl:template>
    
    
    <!-- get the type of the property that is the unique identifier in the class --> 
    <xsl:template name="get_unique_id_property_type">
        <xsl:param name="class" select="/.."/>
        
        <xsl:choose>
            <xsl:when test="key('propertyDefMemberOfQSymbolKey', $class/aafm:QSymbol)[aafm:IsUniqueIdentifier = 'true']">
                <xsl:value-of select="key('propertyDefMemberOfQSymbolKey', $class/aafm:QSymbol)[aafm:IsUniqueIdentifier = 'true']/aafm:Type"/>
            </xsl:when>
            <xsl:when test="count($class/aafm:ParentClass) = 0">
                <xsl:message terminate="true">Could not find unique id property</xsl:message>
            </xsl:when>
            <xsl:otherwise>
                <xsl:call-template name="get_unique_id_property_type">
                    <xsl:with-param name="class" select="key('classDefQSymbolKey', $class/aafm:ParentClass)"/>
                </xsl:call-template>
            </xsl:otherwise>
        </xsl:choose>
        
    </xsl:template>
    
    
    <!-- return 'true' of the type is mapped to a xsdl simpleType --> 
    <xsl:template name="is_xsd_simple_type">
        <xsl:param name="type" select="/.."/>

        <xsl:choose>
            <xsl:when test="name($type) = 'TypeDefinitionRename'">
                <xsl:call-template name="is_xsd_simple_type">
                    <xsl:with-param name="type" select="key('typeDefQSymbolKey', $type/aafm:RenamedType)"/>
                </xsl:call-template>
            </xsl:when>
            <xsl:when test="name($type) = 'TypeDefinitionEnumeration' or name($type) = 'TypeDefinitionExtendibleEnumeration' or name($type) = 'TypeDefinitionInteger'">
                <xsl:value-of select="'true'"/>
            </xsl:when>
            <xsl:when test="$type/aafm:QSymbol = concat($baselinePrefix, ':Rational') or $type/aafm:QSymbol = concat($baselinePrefix, ':AUID') or $type/aafm:QSymbol = concat($baselinePrefix, ':MobIDType') or $type/aafm:QSymbol = concat($baselinePrefix, ':DateStruct') or $type/aafm:QSymbol = concat($baselinePrefix, ':TimeStruct') or $type/aafm:QSymbol = concat($baselinePrefix, ':TimeStamp') or $type/aafm:QSymbol = concat($baselinePrefix, ':VersionType')">
                <xsl:value-of select="'true'"/>
            </xsl:when>
            <xsl:when test="name($type) = 'TypeDefinitionString'">
                <xsl:variable name="elementBaseTypeCategory">
                    <xsl:call-template name="get_base_type_category">
                        <xsl:with-param name="type" select="key('typeDefQSymbolKey', $type/aafm:ElementType)"/>
                    </xsl:call-template>
                </xsl:variable>
                <xsl:choose>
                    <xsl:when test="$elementBaseTypeCategory = 'TypeDefinitionCharacter'">
                        <xsl:value-of select="'false'"/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="'true'"/>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:when>
            <xsl:when test="name($type) = 'TypeDefinitionVariableArray'">
                <xsl:variable name="baseTypeQSymbol">
                    <xsl:call-template name="get_base_type_qsymbol">
                        <xsl:with-param name="type" select="$type"/>
                    </xsl:call-template>
                </xsl:variable>
                <xsl:choose>
                    <xsl:when test="$baseTypeQSymbol = concat($baselinePrefix, ':DataValue')">
                        <xsl:value-of select="'true'"/>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:value-of select="'false'"/>
                    </xsl:otherwise>
                </xsl:choose>
            </xsl:when>
            <xsl:when test="name($type) = 'TypeDefinitionWeakObjectReference'">
                <xsl:variable name="isMetaDefRef">
                    <xsl:call-template name="is_sub_class_of">
                        <xsl:with-param name="class" select="key('classDefQSymbolKey', $type/aafm:ReferencedType)"/>
                        <xsl:with-param name="superClassQSymbol" select="concat($baselinePrefix, ':MetaDefinition')"/>
                    </xsl:call-template>
                </xsl:variable>
                <xsl:variable name="isDefObjectRef">
                    <xsl:call-template name="is_sub_class_of">
                        <xsl:with-param name="class" select="key('classDefQSymbolKey', $type/aafm:ReferencedType)"/>
                        <xsl:with-param name="superClassQSymbol" select="concat($baselinePrefix, ':DefinitionObject')"/>
                    </xsl:call-template>
                </xsl:variable>
                <xsl:choose>
                    <xsl:when test="$isMetaDefRef = 'true' or $isDefObjectRef = 'true'">
                        <xsl:value-of select="'true'"/>
                     </xsl:when>
                     <xsl:otherwise>
                        <xsl:variable name="uniqueIdTypeQSymbol">
                            <xsl:call-template name="get_unique_id_property_type">
                                <xsl:with-param name="class" select="key('classDefQSymbolKey', $type/aafm:ReferencedType)"/>
                            </xsl:call-template>
                        </xsl:variable>
                        <xsl:call-template name="is_xsd_simple_type">
                            <xsl:with-param name="type" select="key('typeDefQSymbolKey', $uniqueIdTypeQSymbol)"/>
                        </xsl:call-template>
                     </xsl:otherwise>
                 </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="'false'"/>
            </xsl:otherwise>
        </xsl:choose>                
    
    </xsl:template>
    
</xsl:stylesheet>
