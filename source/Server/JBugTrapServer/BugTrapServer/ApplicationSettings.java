/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Server parameters block.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

package BugTrapServer;

import java.io.*;
import java.util.*;
import org.w3c.dom.*;
import org.w3c.dom.traversal.*;
import org.xml.sax.*;
import javax.xml.parsers.*;
import javax.xml.xpath.*;

/** Server parameters block. */
class ApplicationSettings {
	/** Parent path for error logs. */
	public String reportPath = null;
	/** Report name pattern. */
	public String reportName = "error_report_%n";
	/**
	 * Host address of SMTP server used for sending
	 * notification e-mails.
	 */
	public String smtpHost = null;
	/**
	 * Port number of SMTP server used for sending
	 * notification e-mails.
	 */
	public short smtpPort = -1;
	/**
	 * Use SSL
	 */
	public boolean smtpSSL = false;
	/** User name of SMTP account. */
	public String smtpUser = null;
	/** Password of SMTP account. */
	public String smtpPassword = null;
	/**
	 * Sender e-mail address used in notification
	 * e-mail message.
	 */
	public String senderAddress = null;
	/**
	 * List of applications handled by the server.
	 * Pass empty list to omit filtering.
	 */
	public TreeSet<AppEntry> applicationList = null;
	/** List of report file extensions accepted by the server. */
	public TreeSet<String> reportFileExtensions = null;
	/** Maximum reports number stored by the system. */
	public int reportsLimit = -1;
	/** Maximum acceptable size of network stream. */
	public int maxReportSize = -1;
	/** Server port number. */
	public short portNumber = 9999;
	/** Unique numbers of last bug reports. */
	public TreeMap<String, AppDirInfo> lastReportNumbers = new TreeMap<String, AppDirInfo>(IgnoreCaseComparator.getInstance());

	/**
	 * Load parameters block.
	 */
	public void load() throws Exception {
		this.loadSettings();
		this.loadDirectories();
	}

	/**
	 * Loads application settings from XML configuration file.
	 */
	private void loadSettings() throws Exception {
		File configFile = getConfigFile();
		InputSource input = new InputSource(configFile.toURI().toURL().toString());
		XPath xpath = XPathFactory.newInstance().newXPath();
		this.loadAppSettings(input, xpath);
		this.loadAppList(input, xpath);
	}

	/**
	 * Load parameters from "applicationSettings" section.
	 * @param input - input stream.
	 * @param xpath - XPath object.
	 */
	private void loadAppSettings(InputSource input, XPath xpath) throws Exception {
		Node appSettings = (Node)xpath.evaluate("/configuration/applicationSettings", input, XPathConstants.NODE);
		String settingValue;
		settingValue = xpath.evaluate("./serverPort/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.portNumber = Short.parseShort(settingValue);
		}
		settingValue = xpath.evaluate("./reportPath/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.reportPath = settingValue;
		}
		settingValue = xpath.evaluate("./reportName/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.reportName = settingValue;
		}
		settingValue = xpath.evaluate("./reportsLimit/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.reportsLimit = Integer.parseInt(settingValue);
		}
		settingValue = xpath.evaluate("./maxReportSize/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.maxReportSize = Integer.parseInt(settingValue);
		}
		settingValue = xpath.evaluate("./smtpHost/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.smtpHost = settingValue;
		}
		settingValue = xpath.evaluate("./smtpPort/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.smtpPort = Short.parseShort(settingValue);
		}
		settingValue = xpath.evaluate("./smtpSSL/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.smtpSSL = Boolean.valueOf(settingValue);
		}
		settingValue = xpath.evaluate("./smtpUser/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.smtpUser = settingValue;
		}
		settingValue = xpath.evaluate("./smtpPassword/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.smtpPassword = settingValue;
		}
		settingValue = xpath.evaluate("./senderAddress/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.senderAddress = settingValue;
		}
		settingValue = xpath.evaluate("./reportFileExtensions/text()", appSettings);
		if (settingValue != null && settingValue.length() > 0) {
			this.reportFileExtensions = new TreeSet<String>(IgnoreCaseComparator.getInstance());
			String[] extensions = settingValue.split(",|;");
			for (int i = 0; i < extensions.length; ++i) {
				this.reportFileExtensions.add(extensions[i]);
			}
		}
	}

	/**
	 * Load parameters from "applicationList" block.
	 * @param input - input stream.
	 * @param xpath - XPath object.
	 */
	private void loadAppList(InputSource input, XPath xpath) throws Exception {
		NodeList appItems = (NodeList)xpath.evaluate("/configuration/applicationSettings/applicationList/application", input, XPathConstants.NODESET);
		int numItems = appItems.getLength();
		this.applicationList = new TreeSet<AppEntry>();
		for (int itemNum = 0; itemNum < numItems; ++itemNum) {
			Element applicationElement = (Element)appItems.item(itemNum);
			AppEntry appEntry = new AppEntry();
			appEntry.name = applicationElement.getTextContent();
			Attr attrVersion = applicationElement.getAttributeNode("version");
			if (attrVersion != null) {
			    appEntry.version = attrVersion.getValue();
			}
			this.applicationList.add(appEntry);
		}
	}

	private String getReportNamePrefix()
	{
		StringBuffer reportNamePrefix = new StringBuffer();
		for (int index = 0; index < this.reportName.length(); ++index)
		{
			char ch = this.reportName.charAt(index);
			if (ch == '%')
				break;
			reportNamePrefix.append(ch);
		}
		return reportNamePrefix.toString();
	}

	private boolean getNextToken(int[] indexArr, char[] tokenArr)
	{
		tokenArr[0] = '\0';
		int index = indexArr[0];
		if (index >= this.reportName.length())
			return false;
		while (index < this.reportName.length())
		{
			char ch = this.reportName.charAt(index);
			if (ch == '%')
			{
				if (++index >= this.reportName.length())
					break;
				ch = this.reportName.charAt(index++);
				tokenArr[0] = ch;
				indexArr[0] = index;
				return true;
			}
			++index;
		}
		indexArr[0] = index;
		return true;
	}

	private boolean autoNumberReports()
	{
		int[] indexArr = new int[] { 0 };
		char[] tokenArr = new char[]  { '\0' };
		while (getNextToken(indexArr, tokenArr))
		{
			if (tokenArr[0] == 'n')
				return true;
		}
		return false;
	}

	private int getReportNumber(String fileName)
	{
		int dataIndex = 0;
		char tokenArr[] = new char[] { '\0' };
		int refIndexArr[] = new int[] { 0 };
		for (;;)
		{
			int refStart = refIndexArr[0];
			if (!getNextToken(refIndexArr, tokenArr))
				break;
			int length = refIndexArr[0] - refStart;
			char token = tokenArr[0];
			if (token == 'n' || token == 't')
				length -= 2;
			else
				length -= 1;
			dataIndex += length;
			if (token == 'n' || token == 't')
			{
				int dataStart = dataIndex;
				while (dataIndex < fileName.length() && Character.isDigit(fileName.charAt(dataIndex)))
					++dataIndex;
				if (token == 'n' && dataStart < dataIndex)
					return Integer.parseInt(fileName.substring(dataStart, dataIndex));
			}
		}
		return -1;
	}

	public String getReportName(int reportNumber, String extension)
	{
		StringBuffer reportName = new StringBuffer();
		int[] indexArr = new int[] { 0 };
		for (;;)
		{
			char tokenArr[] = new char[] { '\0' };
			int start = indexArr[0];
			if (!getNextToken(indexArr, tokenArr))
				break;
			int length = indexArr[0] - start;
			if (tokenArr[0] != '\0')
				length -= 2;
			reportName.append(this.reportName, start, length);
			switch (tokenArr[0])
			{
			case 'n':
				reportName.append(reportNumber);
				break;
			case 't':
				reportName.append(new Date().getTime());
				break;
			default:
				if (tokenArr[0] != '\0')
					reportName.append(tokenArr[0]);
				break;
			}
		}
		reportName.append('.');
		reportName.append(extension);
		return reportName.toString();
	}

	/**
	 * Load directories contents from disk.
	 */
	private void loadDirectories() {
		File reportPath = new File(this.reportPath);
		reportPath.mkdirs();
		File[] directories = reportPath.listFiles();
		String reportNamePrefix = getReportNamePrefix();
		String reportNameFilter = reportNamePrefix + '*';
		boolean autoNumReports = autoNumberReports();
		for (int dirIndex = 0; dirIndex < directories.length; dirIndex++) {
			if (! directories[dirIndex].isDirectory()) {
				continue;
			}
			File directory = directories[dirIndex];
			String appTitle = directory.getName();
            AppDirInfo appDirInfo = new AppDirInfo();
			File[] files = directory.listFiles();
			for (int fileIndex = 0; fileIndex < files.length; fileIndex++) {
				File file = files[fileIndex];
				if (! file.isFile())
					continue;
				String fileName = file.getName();
				if (! fileName.startsWith(reportNamePrefix))
					continue;
				++appDirInfo.numReports;
				if (autoNumReports)
				{
					int reportNumber = getReportNumber(fileName);
					if (reportNumber >= 0 && appDirInfo.maxReportNumber < reportNumber)
						appDirInfo.maxReportNumber = reportNumber;
				}
				else
					appDirInfo.maxReportNumber = appDirInfo.numReports;
			}
			this.lastReportNumbers.put(appTitle, appDirInfo);
		}
	}

	/**
	 * Get configuration file.
	 * @return configuration file object.
	 */
	private static File getConfigFile() {
		String appPath = System.getProperty("user.dir");
		File configFile = new File(appPath, "BugTrapServer.config");
		return configFile;
	}
}
