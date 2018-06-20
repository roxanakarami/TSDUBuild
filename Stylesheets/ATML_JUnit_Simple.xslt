<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:trc="urn:IEEE-1636.1:2013:TestResultsCollection" xmlns:tr="urn:IEEE-1636.1:2013:TestResults" xmlns:ts="www.ni.com/TestStand/ATMLTestResults/3.0" xmlns:c="urn:IEEE-1671:2010:Common" exclude-result-prefixes="ts trc tr c">
  <xsl:strip-space elements="*"/>
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match = "trc:TestResultsCollection">
    <xsl:for-each select = "trc:TestResults" >
      <testsuite>
        <xsl:attribute name="name">Test Results</xsl:attribute>
        <xsl:attribute name="tests">
          <xsl:value-of select="tr:Extension/ts:TSResultSetProperties/ts:NumOfResults/@value"/>
        </xsl:attribute>
        <properties>
          <property name="UUT Result" >
            <xsl:attribute name="value">
              <xsl:value-of select="tr:ResultSet/tr:Outcome/@value"/>
            </xsl:attribute>
          </property>
          <property name="UUT Serial Number" >
            <xsl:attribute name="value">
              <xsl:value-of select="tr:UUT/c:SerialNumber"/>
            </xsl:attribute>
          </property>
        </properties>
        <!--process each child that has a Step ID-->
        <xsl:for-each select="tr:ResultSet/*[@testReferenceID]">
          <xsl:apply-templates select = "."/>
        </xsl:for-each>
      </testsuite>
    </xsl:for-each>
  </xsl:template>
  <!--Process test steps-->
  <xsl:template match = "tr:Test">
    <testcase>
      <xsl:attribute name="classname">
        <xsl:value-of select="tr:Extension/ts:TSStepProperties/ts:StepType"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:attribute name="Status">
        <xsl:value-of select="tr:Outcome/@value"/>
      </xsl:attribute>
      <xsl:choose>
        <xsl:when test="tr:Outcome/@value = 'NotStarted'">
          <skipped/>
        </xsl:when>
        <xsl:when test="tr:Outcome/@value = 'Aborted'">
          <error/>
        </xsl:when>
        <xsl:when test="tr:Outcome/@value = 'Failed'">
          <failure/>
        </xsl:when>
        <xsl:otherwise/>
      </xsl:choose>
    </testcase>
  </xsl:template>
  <!--Process action steps-->
  <xsl:template match = "tr:SessionAction">
    <testcase>
      <xsl:attribute name="classname">
        <xsl:value-of select="tr:Extension/ts:TSStepProperties/ts:StepType"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:attribute name="Status">
        <xsl:value-of select="tr:ActionOutcome/@value"/>
      </xsl:attribute>
      <xsl:choose>
        <xsl:when test="tr:ActionOutcome/@value = 'NotStarted'">
          <skipped/>
        </xsl:when>
        <xsl:when test="tr:ActionOutcome/@value = 'Aborted'">
          <error/>
        </xsl:when>
        <xsl:otherwise/>
      </xsl:choose>
    </testcase>
  </xsl:template>
  <!--Process sequence calls-->
  <xsl:template match = "tr:TestGroup">
    <testcase>
      <xsl:attribute name="classname">
        <xsl:value-of select="tr:Extension/ts:TSStepProperties/ts:StepType"/>
      </xsl:attribute>
      <xsl:attribute name="name">
        <xsl:value-of select="@name"/>
      </xsl:attribute>
      <xsl:attribute name="Status">
        <xsl:value-of select="tr:Outcome/@value"/>
      </xsl:attribute>
      <xsl:choose>
        <xsl:when test="tr:ActionOutcome/@value = 'NotStarted'">
          <skipped/>
        </xsl:when>
        <xsl:when test="tr:ActionOutcome/@value = 'Aborted'">
          <error/>
        </xsl:when>
        <xsl:when test="tr:Outcome/@value = 'Failed'">
          <failure/>
        </xsl:when>
        <xsl:otherwise/>
      </xsl:choose>
    </testcase>
    <xsl:for-each select="./*[@testReferenceID]">
      <xsl:apply-templates select = "."/>
    </xsl:for-each>
  </xsl:template>
</xsl:stylesheet>