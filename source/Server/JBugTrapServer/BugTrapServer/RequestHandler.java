/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Handles incoming requests.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

package BugTrapServer;

import java.io.*;
import java.lang.reflect.Array;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;
import java.util.*;

import javax.mail.*;
import javax.mail.internet.*;

/** Handles incoming requests. */
class RequestHandler implements Runnable {
	/** Temporary file descriptor that keeps packet data. */
	private File tempFile;
	/** Packet data stored on a disk. */
	private FileInputStream fileStream;
	/** Provides buffered access to packet data. */
	private BufferedInputStream inputStream;
	/** Server parameters. */
	final private ApplicationSettings applicationSettings;
	/** Protocol signature. */
	final private static int protocolSignature = (('B' << 0) | ('T' << 8) | ('0' << 16) | ('1' << 24));
	/** Compound message that includes project info and report data. */
	final private static byte compundMessage = 1;

	/**
	 * Initialize the object.
	 * @param applicationSettings - server parameters.
	 * @param tempFile - temporary file that holds packet data.
	 */
	public RequestHandler(ApplicationSettings applicationSettings, File tempFile) throws Exception {
		this.applicationSettings = applicationSettings;
		this.tempFile = tempFile;
		this.fileStream = new FileInputStream(tempFile);
		this.inputStream = new BufferedInputStream(this.fileStream);
	}

	private static class MailAuthenticator extends javax.mail.Authenticator {
		private String mAccount;
		private String mPassphrase;

		public MailAuthenticator(String account, String passphrase) {
			mAccount = account;
			mPassphrase = passphrase;
		}

		public javax.mail.PasswordAuthentication getPasswordAuthentication() {
			return new javax.mail.PasswordAuthentication(mAccount, mPassphrase);
		}
	}

	/**
	 * Send notification e-mail.
	 * @param email - notification e-mail address.
	 * @param appTitle - application title.
	 */
	private void sendEMail(String email, String appTitle) throws Exception {
		if (this.applicationSettings.smtpHost != null && this.applicationSettings.smtpHost.length() > 0 &&
			email != null && email.length() > 0) {
			Properties properties = new Properties();
			properties.setProperty("mail.smtp.host", this.applicationSettings.smtpHost);
			if (this.applicationSettings.smtpPort > 0)
				properties.setProperty("mail.smtp.port", Short.toString(this.applicationSettings.smtpPort));
			Session session;
			if (this.applicationSettings.smtpUser != null)
			{
				properties.setProperty("mail.smtp.auth", "true");
				MailAuthenticator authenticator = new MailAuthenticator(this.applicationSettings.smtpUser, this.applicationSettings.smtpPassword);
				session = Session.getDefaultInstance(properties, authenticator);
			}
			else
			{
				properties.setProperty("mail.smtp.auth", "false");
				session = Session.getDefaultInstance(properties);
			}
			if (this.applicationSettings.smtpSSL) {
				properties.setProperty("mail.smtp.starttls.enable", "true");
			}
			Transport transport = session.getTransport("smtp");
			try {
				MimeMessage message = new MimeMessage(session);
				if (this.applicationSettings.senderAddress != null)
					message.setFrom(new InternetAddress(this.applicationSettings.senderAddress));
				InternetAddress[] address = { new InternetAddress(email) };
				message.setRecipients(Message.RecipientType.TO, address);
				message.setSubject('\"' + appTitle + "\" error report");
				message.setText("BugTrap server received error report of \"" + appTitle + "\" on " + new Date());
				transport.send(message);
			}
			finally {
				transport.close();
			}
		}
	}

	/**
	 * Find application entry in the application item list.
	 * @param appEntry - application entry to search for.
	 * @return true if appropriate entry was found and false otherwise.
	 */
	private boolean findAppEntry(AppEntry appEntry) {
		if (this.applicationSettings.applicationList == null || this.applicationSettings.applicationList.isEmpty())
			return true;
		if (this.applicationSettings.applicationList.contains(appEntry))
			return true;
		if (appEntry.version != null) {
			AppEntry appEntry2 = new AppEntry(appEntry.name);
			if (this.applicationSettings.applicationList.contains(appEntry2))
				return true;
		}
		return false;
	}

	/**
	 * Find report file extension in the list of accepted extensions.
	 * @param extension - report file extension.
	 * @return true if appropriate extension was found and false otherwise.
	 */
	private boolean findReportFileExtension(String extension) {
		if (this.applicationSettings.reportFileExtensions == null || this.applicationSettings.reportFileExtensions.isEmpty())
			return true;
		if (this.applicationSettings.reportFileExtensions.contains(extension))
			return true;
		return false;
	}

	/**
	 * Generate directory name from the application name.
	 * @param appTitle - application title.
	 */
	private static String getAppDirName(String appTitle) {
		String allowedChars = " _(){}.,;!+-";
		StringBuffer dirName = new StringBuffer();
		int position = 0, length = appTitle.length();
		while (position < length)
		{
			int codePoint = appTitle.codePointAt(position);
			int charSize = Character.charCount(codePoint);
			if (Character.isLetterOrDigit(codePoint) ||
				(charSize == 1 && allowedChars.indexOf(appTitle.charAt(position)) >= 0)) {
				dirName.appendCodePoint(codePoint);
			}
			position += charSize;
		}
		return dirName.toString();
	}

	/** Performs handler work. */
	public void run() {
		try {
			ProtoExtractor extractor = null;
			FileOutputStream outputStream = null;
			try {
				extractor = new ProtoExtractor(this.inputStream);
				int dataSize = extractor.available();
				// message size (DWORD) + protocol signature (DWORD) = 8 bytes
				if (dataSize < 8)
					throw new Exception("Invalid message size");
				if (RequestHandler.protocolSignature != extractor.readInt())
					throw new Exception("Unsupported protocol version");
				if (dataSize != extractor.readInt())
					throw new Exception("Invalid message size");
				if (RequestHandler.compundMessage != extractor.readByte())
					throw new Exception("Unsupported message type");
				int messageFlags = extractor.readInt();
				if (messageFlags != 0)
					throw new Exception("Unsupported message flags");
				String appName = extractor.readString();
				String appVersion = extractor.readString();
				String extension = extractor.readString();
				String email = extractor.readString();
				if (appName.length() == 0)
					appName = "(UNTITLED)";
				String appTitle = appVersion.length() == 0 ? appName : appName + ' ' + appVersion;
				String appDir = getAppDirName(appTitle);
				int reportSize = extractor.available();
				if ((this.applicationSettings.maxReportSize >= 0 && reportSize > this.applicationSettings.maxReportSize) || reportSize <= 0)
					throw new Exception("Report exceeds size limit");
				AppEntry appEntry = new AppEntry(appName, appVersion);
				if (! this.findAppEntry(appEntry))
					throw new Exception("Report excluded by filter");
				if (! this.findReportFileExtension(extension))
					throw new Exception("Invalid report file extension");
				int maxReportNumber = 0, numReports = 0;
				synchronized (this.applicationSettings.lastReportNumbers) {
					AppDirInfo appDirInfo = this.applicationSettings.lastReportNumbers.get(appDir);
					if (appDirInfo != null) {
						maxReportNumber = appDirInfo.maxReportNumber;
						numReports = appDirInfo.numReports;
					}
					if (this.applicationSettings.reportsLimit >= 0 && numReports > this.applicationSettings.reportsLimit)
						throw new Exception("Number of reports exceeds the limit");
					if (appDirInfo == null) {
						appDirInfo = new AppDirInfo();
						this.applicationSettings.lastReportNumbers.put(appDir, appDirInfo);
					}
					appDirInfo.numReports = ++numReports;
					appDirInfo.maxReportNumber = ++maxReportNumber;
				}
				File reportDir = new File(this.applicationSettings.reportPath, appDir);
				reportDir.mkdir();
				String fileName = this.applicationSettings.getReportName(maxReportNumber, extension);
				File reportFile = new File(reportDir, fileName);
				outputStream = new FileOutputStream(reportFile);
				byte[] reportData = new byte[1024];
				while (reportSize > 0) {
					int chunkSize = Math.min(reportSize, reportData.length);
					this.inputStream.read(reportData);
					outputStream.write(reportData, 0, chunkSize);
					reportSize -= chunkSize;
				}
				this.sendEMail(email, appTitle);
			}
			finally {
				if (outputStream != null) {
					outputStream.close();
					outputStream = null;
				}
				if (extractor != null) {
					extractor.close();
					extractor = null;
				}
				if (this.inputStream != null) {
					this.inputStream.close();
					this.inputStream = null;
				}
				if (this.fileStream != null) {
					this.fileStream.close();
					this.fileStream = null;
				}
				if (this.tempFile != null) {
					this.tempFile.delete();
					this.tempFile = null;
				}
			}
		}
		catch (Exception error) {
			error.printStackTrace();
		}
	}
}
