/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Web request handler.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Mail;
using System.Diagnostics;
using System.Collections.Generic;
using System.Web;
using System.Xml;
using IntelleSoft.Collections;

namespace BugTrapServer
{
	/// <summary>
	/// Web request handler.
	/// </summary>
	public partial class RequestHandler : System.Web.UI.Page
	{
		/// <summary>
		/// Protocol signature.
		/// </summary>
		private const string protocolSignature = "BT01";

		/// <summary>
		/// Shared XML writer settings.
		/// </summary>
		private static XmlWriterSettings settings;

		/// <summary>
		/// Protocol message type.
		/// </summary>
		private enum MessageType
		{
			/// <summary>
			/// Compound message that includes project info and report data.
			/// </summary>
			CompundMessage = 1
		};

		/// <summary>
		/// Compound message flags.
		/// </summary>
		[Flags]
		private enum CompoundMessageFlags
		{
			/// <summary>
			/// No flags specified.
			/// </summary>
			None = 0x00,
		};

		static RequestHandler()
		{
			RequestHandler.settings = new XmlWriterSettings();
			RequestHandler.settings.ConformanceLevel = ConformanceLevel.Fragment;
			RequestHandler.settings.Indent = true;
		}

		/// <summary>
		/// Generate directory name from the application name.
		/// </summary>
		/// <param name="appTitle">Application title.</param>
		/// <returns>Directory name.</returns>
		private static string GetAppDirName(string appTitle)
		{
			const string allowedChars = " _(){}.,;!+-";
			StringBuilder dirName = new StringBuilder();
			int position = 0, length = appTitle.Length;
			while (position < length)
			{
				if (char.IsSurrogate(appTitle, position))
				{
					if (char.IsLetterOrDigit(appTitle, position))
					{
						dirName.Append(appTitle[position++]);
						dirName.Append(appTitle[position++]);
					}
				}
				else
				{
					char ch = appTitle[position++];
					if ((char.IsLetterOrDigit(ch) || allowedChars.IndexOf(ch) >= 0))
						dirName.Append(ch);
				}
			}
			return dirName.ToString();
		}

		/// <summary>
		/// Find application entry in the application item list.
		/// </summary>
		/// <param name="applicationSettings">Application settings.</param>
		/// <param name="appEntry">Application entry to search for.</param>
		/// <returns>True if appropriate entry was found and false otherwise.</returns>
		private bool FindAppEntry(ApplicationSettings applicationSettings, AppEntry appEntry)
		{
			Set<AppEntry> applicationList = applicationSettings.ApplicationList;
			if (applicationList == null || applicationList.Count == 0)
				return true;
			if (applicationList.Contains(appEntry))
				return true;
			if (appEntry.Version != null)
			{
				AppEntry appEntry2 = new AppEntry(appEntry.Name);
				if (applicationList.Contains(appEntry2))
					return true;
			}
			return false;
		}

		/// <summary>
		/// Find report file extension in the list of accepted extensions.
		/// </summary>
		/// <param name="applicationSettings">Application settings.</param>
		/// <param name="extension">Report file extension.</param>
		/// <returns>True if appropriate extension was found and false otherwise.</returns>
		private bool FindReportFileExtension(ApplicationSettings applicationSettings, string extension)
		{
			Set<string> reportFileExtensions = applicationSettings.ReportFileExtensions;
			if (reportFileExtensions == null || reportFileExtensions.Count == 0)
				return true;
			return reportFileExtensions.Contains(extension);
		}

		/// <summary>
		/// Send notification e-mail.
		/// </summary>
		/// <param name="applicationSettings">Application settings.</param>
		/// <param name="email">Notification e-mail address.</param>
		/// <param name="appTitle">Application title.</param>
		private void SendEMail(ApplicationSettings applicationSettings, string email, string appTitle)
		{
			if (!string.IsNullOrEmpty(applicationSettings.SmtpHost) &&
				!string.IsNullOrEmpty(email))
			{
				string senderAddress = applicationSettings.SenderAddress ?? string.Empty;
				MailMessage message = new MailMessage(senderAddress, email);
				message.Subject = '\"' + appTitle + "\" error report";
				message.Body = "BugTrap server received error report of \"" + appTitle + "\" on " + DateTime.Now.ToString();
				SmtpClient smtpClient;
				if (applicationSettings.SmtpPort > 0)
					smtpClient = new SmtpClient(applicationSettings.SmtpHost, applicationSettings.SmtpPort);
				else
					smtpClient = new SmtpClient(applicationSettings.SmtpHost);
				smtpClient.EnableSsl = applicationSettings.SmtpSSL;
				if (applicationSettings.SmtpUser != null)
					smtpClient.Credentials = new NetworkCredential(applicationSettings.SmtpUser, applicationSettings.SmtpPassword);
				smtpClient.Send(message);
			}
		}

		/// <summary>
		/// Handle client request.
		/// </summary>
		private void HandleRequest()
		{
			ApplicationSettings applicationSettings = (ApplicationSettings)this.Application["applicationSettings"];
			Dictionary<string, AppDirInfo> lastReportNumbers = (Dictionary<string, AppDirInfo>)this.Application["lastReportNumbers"];
			if (this.Request.Form["protocolSignature"] != RequestHandler.protocolSignature)
				throw new ApplicationException("Unsupported protocol version");
			MessageType messageType = (MessageType)byte.Parse(this.Request.Form["messageType"]);
			if (messageType != MessageType.CompundMessage)
				throw new Exception("Unsupported message type");
			CompoundMessageFlags messageFlags = (CompoundMessageFlags)uint.Parse(this.Request.Form["messageFlags"]);
			if (messageFlags != CompoundMessageFlags.None)
				throw new Exception("Unsupported message flags");
			string appName = this.Request.Form["appName"];
			string appVersion = this.Request.Form["appVersion"];
			if (appName == string.Empty)
				appName = "(UNTITLED)";
			string appTitle = appVersion == string.Empty ? appName : appName + ' ' + appVersion;
			AppEntry appEntry = new AppEntry(appName, appVersion);
			if (!this.FindAppEntry(applicationSettings, appEntry))
				throw new Exception("Report excluded by filter");
			string extension = this.Request.Form["reportFileExtension"];
			if (!this.FindReportFileExtension(applicationSettings, extension))
				throw new Exception("Invalid report file extension");
			HttpPostedFile reportDataFile = this.Request.Files["reportData"];
			if (reportDataFile == null)
				throw new Exception("Invalid report data");
			int reportSize = reportDataFile.ContentLength;
			if ((applicationSettings.MaxReportSize >= 0 && reportSize > applicationSettings.MaxReportSize) || reportSize <= 0)
				throw new Exception("Report exceeds size limit");
			string email = this.Request.Form["notificationEMail"];
			string dirName = GetAppDirName(appTitle);
			int maxReportNumber = 0, numReports = 0;
			lock (lastReportNumbers)
			{
				AppDirInfo appDirInfo;
				if (lastReportNumbers.TryGetValue(dirName, out appDirInfo))
				{
					maxReportNumber = appDirInfo.MaxReportNumber;
					numReports = appDirInfo.NumReports;
				}
				if (applicationSettings.ReportsLimit >= 0 && numReports > applicationSettings.ReportsLimit)
					throw new ApplicationException("Number of reports exceeds the limit");
				if (appDirInfo == null)
				{
					appDirInfo = new AppDirInfo();
					lastReportNumbers[dirName] = appDirInfo;
				}
				appDirInfo.NumReports = ++numReports;
				appDirInfo.MaxReportNumber = ++maxReportNumber;
			}
			string reportDir = Path.Combine(applicationSettings.ReportPath, dirName);
			Directory.CreateDirectory(reportDir);
			string fileName = Global.GetReportName(applicationSettings, maxReportNumber, extension);
			string filePath = Path.Combine(reportDir, fileName);
			Stream inputStream = null;
			FileStream outputStream = null;
			try
			{
				inputStream = reportDataFile.InputStream;
				outputStream = new FileStream(filePath, FileMode.Create, FileAccess.ReadWrite, FileShare.None);
				byte[] reportData = new byte[1024];
				while (reportSize > 0)
				{
					int chunkSize = Math.Min(reportSize, reportData.Length);
					inputStream.Read(reportData, 0, chunkSize);
					outputStream.Write(reportData, 0, chunkSize);
					reportSize -= chunkSize;
				}
				this.SendEMail(applicationSettings, email, appTitle);
			}
			finally
			{
				if (inputStream != null)
					inputStream.Close();
				if (outputStream != null)
					outputStream.Close();
			}
		}

		/// <summary>
		/// Send server response to the client.
		/// <param name="code">Error code.</param>
		/// <param name="description">Error description.</param>
		/// </summary>
		private void WriteResponse(string code, string description)
		{
			this.Response.Clear();
			this.Response.ContentType = "text/xml";
			this.Response.ContentEncoding = Encoding.UTF8;
			XmlWriter writer = XmlWriter.Create(this.Response.Output, settings);
			try
			{
				writer.WriteStartElement("result");
				writer.WriteElementString("code", code);
				writer.WriteElementString("description", description);
				writer.WriteEndElement();
			}
			finally
			{
                writer.Close();
			}
            this.Response.Flush();
		}

		/// <summary>
		/// Occurs when the server control is loaded into the Page object.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="e">An EventArgs object that contains the event data.</param>
		[DoNotObfuscate]
		protected void Page_Load(object sender, EventArgs e)
		{
			string code = "OK";
			string description = "The operation completed successfully";
			try
			{
				this.HandleRequest();
			}
			catch (Exception error)
			{
				code = "Error";
				description = error.Message;
				this.ReportNestedError(error);
			}
			finally
			{
				this.WriteResponse(code, description);
			}
		}


		/// <summary>
		/// Get inner exception object.
		/// </summary>
		/// <param name="error">Topmost exception object.</param>
		/// <returns>Inner exception object.</returns>
		private static Exception GetNestedError(Exception error)
		{
			while (error.InnerException != null)
				error = error.InnerException;
			return error;
		}

		private EventLog GetEventLog()
		{
			EventLog eventLog = (EventLog)this.Application["eventLog"];
			if (eventLog == null)
			{
				eventLog = new EventLog();
				eventLog.Source = "BugTrapWebServer";
				this.Application["eventLog"] = eventLog;
			}
			return eventLog;
		}

		/// <summary>
		/// Print error message on a console and add error message to the event log.
		/// </summary>
		/// <param name="error">Exception information.</param>
		private void ReportError(Exception error)
		{
			string message = error.ToString();
			Debug.WriteLine(message);
			ApplicationSettings applicationSettings = (ApplicationSettings)this.Application["applicationSettings"];
			if (applicationSettings.LogEvents)
			{
				EventLog eventLog = this.GetEventLog();
				eventLog.WriteEntry(message, EventLogEntryType.Error);
			}
		}

		/// <summary>
		/// Print error message on a console and add error message to the event log.
		/// </summary>
		/// <param name="error">Exception information.</param>
		private void ReportNestedError(Exception error)
		{
			error = GetNestedError(error);
			this.ReportError(error);
		}
	}
}
