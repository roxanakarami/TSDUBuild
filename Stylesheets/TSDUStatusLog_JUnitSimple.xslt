<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:ts="www.ni.com/TestStand/DeploymentUtilityLog/1.0" exclude-result-prefixes="ts">
  <xsl:strip-space elements="*"/>
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <xsl:template match="ts:tsdu_log_messages">
    <testsuite>
      <xsl:attribute name="name">TSD Status Log</xsl:attribute>
      <xsl:attribute name="tests">
        <xsl:value-of select="count(ts:deployment/ts:log_message)"/>
      </xsl:attribute>
      <xsl:for-each select="ts:deployment/ts:log_message">
        <testcase>
          <xsl:attribute name="name">
            <xsl:value-of select="normalize-space(ts:message)"/>
          </xsl:attribute>
          <xsl:attribute name="classname">
            <xsl:value-of select="@phase"/>
          </xsl:attribute>
          <xsl:if test = "@severity = 'Warn'">
            <failure>
              <xsl:attribute name="message">See the Detailed Status log for more information</xsl:attribute>
            </failure>
          </xsl:if>
          <xsl:if test = "@severity = 'Error'">
            <error>
              <xsl:attribute name="message">See the Detailed Status log for more information</xsl:attribute>
            </error>
          </xsl:if>
        </testcase>
      </xsl:for-each>
    </testsuite>
  </xsl:template>
</xsl:stylesheet>