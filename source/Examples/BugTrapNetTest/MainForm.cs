using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;

using IntelleSoft.BugTrap;

namespace BugTrapNetTest
{
	public partial class MainForm : Form
	{
		private LogFile log;

		private void SetupExceptionHandler()
		{
			// initialize exception handler
			ExceptionHandler.AppName = "BugTrapNetTest";
			ExceptionHandler.Flags = FlagsType.DetailedMode | FlagsType.ScreenCapture | FlagsType.AttachReport | FlagsType.EditMail;
			ExceptionHandler.ReportFormat = ReportFormatType.Xml;
			ExceptionHandler.DumpType = MinidumpType.NoDump;
			ExceptionHandler.SupportEMail = "your@email.com";
			ExceptionHandler.SupportURL = "http://www.intellesoft.net";
			ExceptionHandler.SupportHost = "localhost";
			ExceptionHandler.SupportPort = 9999;
			// register custom log file
			this.log = new LogFile("test.log", LogFormatType.Xml);
			LogFiles.Add(new LogFileEntry(this.log.FileName));
			this.log.LogSizeInEntries = 3;
		}

		public MainForm()
		{
			InitializeComponent();
			SetupExceptionHandler();
		}

		private void exceptionButton_Click(object sender, EventArgs e)
		{
			this.log.Append("entering exceptionButton_Click({0}, {1})", sender.GetType(), e.GetType());
			throw new Exception();
		}

		private void crashDumpCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			bool check = this.crashDumpCheckBox.Checked;
			if (check)
				ExceptionHandler.DumpType = MinidumpType.WithFullMemory;
			else
				ExceptionHandler.DumpType = MinidumpType.NoDump;
			this.warningMessage.Visible = check;
		}
	}
}