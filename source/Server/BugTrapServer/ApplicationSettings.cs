/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Application settings.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;
using System.Diagnostics;
using System.Xml;
using System.Xml.Schema;
using System.Xml.Serialization;
using System.ComponentModel;
using IntelleSoft.Collections;

namespace BugTrapServer
{
	[Serializable, XmlRoot("applicationSettings")]
	public sealed class ApplicationSettings
	{
		[XmlElement("logEvents", Type = typeof(bool))]
		public bool LogEvents = true;

		[XmlElement("serverPort", Type = typeof(short))]
		public short ServerPort = 9999;

		[XmlElement("reportPath", Type = typeof(string))]
		public string ReportPath = null;

		[XmlElement("reportName", Type = typeof(string))]
		public string ReportName = "error_report_%n";

		[XmlElement("reportsLimit", Type = typeof(int))]
		public int ReportsLimit = -1;

		[XmlElement("maxReportSize", Type = typeof(int))]
		public int MaxReportSize = -1;

		[XmlElement("smtpHost", Type = typeof(string))]
		public string SmtpHost = null;

		[XmlElement("smtpPort", Type = typeof(short))]
		public short SmtpPort = -1;

        [XmlElement("smtpSSL", Type = typeof(bool))]
        public bool SmtpSSL = false;

        [XmlElement("smtpUser", Type = typeof(string))]
		public string SmtpUser = null;

		[XmlElement("smtpPassword", Type = typeof(string))]
		public string SmtpPassword = null;

		[XmlElement("senderAddress", Type = typeof(string))]
		public string SenderAddress = null;

		[XmlIgnore]
		public Set<string> ReportFileExtensions = null;

		[XmlElement("reportFileExtensions")]
		public string XmlReportFileExtensionsList
		{
			get
			{
				if (this.ReportFileExtensions == null)
					return null;
				StringBuilder value = new StringBuilder();
				foreach (string extension in this.ReportFileExtensions)
				{
					if (value.Length > 0)
						value.Append(',');
					value.Append(extension);
				}
				return value.ToString();
			}
			set
			{
				if (value != null)
					this.ReportFileExtensions = new Set<string>(value.Split(new char[] { ',', ';' }), StringComparer.InvariantCultureIgnoreCase);
				else
					this.ReportFileExtensions = null;
			}
		}

		[XmlArray("applicationList"), XmlArrayItem("application", Type = typeof(AppEntry))]
		public Set<AppEntry> ApplicationList = null;
	}
}
