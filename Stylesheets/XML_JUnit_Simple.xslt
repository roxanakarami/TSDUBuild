<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:ts="http://www.ni.com/TestStand/16.0.0/PropertyObjectFile" exclude-result-prefixes="ts">
  <xsl:strip-space elements="*"/>
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="Reports/Report">
    <testsuite>
      <xsl:attribute name="name">Test Results</xsl:attribute>
      <xsl:attribute name="tests">
        <!--xsl:value-of select="ts:Messages/ts:value/@ubound"/!-->
        <xsl:value-of select="@StepCount"/>
      </xsl:attribute>
      <properties>
        <property name="UUT Result" >
          <xsl:attribute name="value">
            <!--xsl:value-of select="ts:Messages/ts:value/@ubound"/!-->
            <xsl:value-of select="@UUTResult"/>
          </xsl:attribute>
        </property>
      </properties>
      <xsl:apply-templates select="Prop[@Type='TEResult']/Prop[@Name='TS']/Prop[@Name='SequenceCall']/Prop[@Name='ResultList']"/>
    </testsuite>
  </xsl:template>
  <xsl:template match="Prop[@Name='ResultList']">
    <xsl:for-each select="Value">
      <testcase>
        <xsl:attribute name="classname">
          <xsl:value-of select="Prop[@Type='TEResult']/Prop[@Name='TS']/Prop[@Name='StepType']/Value"/>
        </xsl:attribute>
        <xsl:attribute name="name">
          <xsl:value-of select="Prop[@Type='TEResult']/Prop[@Name='TS']/Prop[@Name='StepName']/Value"/>
        </xsl:attribute>
        <xsl:attribute name="Status">
          <xsl:value-of select="Prop[@Type='TEResult']/Prop[@Name='Status']/Value"/>
        </xsl:attribute>
        <xsl:choose>
          <xsl:when test="Prop[@Type='TEResult']/Prop[@Name='Status']/Value = 'Skipped'">
            <skipped/>
          </xsl:when>
          <xsl:when test="Prop[@Type='TEResult']/Prop[@Name='Status']/Value = 'Error'">
            <error>
              <xsl:attribute name="message">
                <xsl:value-of select="normalize-space(Prop[@Type='TEResult']/Prop[@Name='Error']/Prop[@Name='Msg']/Value)"/>
              </xsl:attribute>
            </error>
          </xsl:when>
          <xsl:when test="Prop[@Type='TEResult']/Prop[@Name='Status']/Value = 'Failed'">
            <failure/>
          </xsl:when>
          <xsl:otherwise/>
        </xsl:choose>
      </testcase>
      <xsl:if test="Prop[@Type='TEResult']/Prop[@Name='TS']/Prop[@Name='StepType']/Value = 'SequenceCall'">
        <xsl:apply-templates select="Prop[@Type='TEResult']/Prop[@Name='TS']/Prop[@Name='SequenceCall']/Prop[@Name='ResultList']"/>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>