/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Main service class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.ComponentModel;
using System.ServiceProcess;
using System.Diagnostics;
using System.Text;
using System.Configuration;
using System.Collections.Generic;
using System.Threading;
using System.Net;
using System.Net.Sockets;
using System.Net.Mail;
using System.IO;
using IntelleSoft.Collections;

namespace BugTrapServer
{
	/// <summary>
	/// Main service class.
	/// </summary>
	internal partial class BugTrapService : ServiceBase
	{
		/// <summary>
		/// Keeps directory information for the application.
		/// </summary>
		private class AppDirInfo
		{
			/// <summary>
			/// Maximum report number.
			/// </summary>
			public int MaxReportNumber;
			/// <summary>
			/// Number of reports stored in the directory.
			/// </summary>
			public int NumReports;
		}

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

		/// <summary>
		/// Number of running tasks.
		/// </summary>
		private int openTasks;

		/// <summary>
		/// Listener thread.
		/// </summary>
		private Thread listenerThread;

		/// <summary>
		/// True after successful start.
		/// </summary>
		private bool started;

		/// <summary>
		/// Notifies other threads about server initialization.
		/// </summary>
		readonly private ManualResetEvent serverInit = new ManualResetEvent(false);

		/// <summary>
		/// Set on when incoming socket has been accepted.
		/// </summary>
		readonly private ManualResetEvent acceptCompleted = new ManualResetEvent(false);

		/// <summary>
		/// Set on when service is stopped by the system.
		/// </summary>
		readonly private ManualResetEvent stopServiceEvent = new ManualResetEvent(false);

		/// <summary>
		/// Indicates end of client request.
		/// </summary>
		readonly private AutoResetEvent endRequestEvent = new AutoResetEvent(false);

		/// <summary>
		/// Application settings.
		/// </summary>
		readonly ApplicationSettings applicationSettings;

		/// <summary>
		/// Unique numbers of recent bug reports.
		/// </summary>
		private Dictionary<string, AppDirInfo> lastReportNumbers = new Dictionary<string, AppDirInfo>();

		/// <summary>
		/// Protocol signature.
		/// </summary>
		private const uint protocolSignature = ('B' << 0) | ('T' << 8) | ('0' << 16) | ('1' << 24);

		/// <summary>
		/// True if service runs in interactive mode.
		/// </summary>
		private bool interactiveMode;

		/// <summary>
		/// Object constructor.
		/// </summary>
		public BugTrapService(bool interactiveMode)
		{
			this.interactiveMode = interactiveMode;
			this.acceptCompleted = new ManualResetEvent(false);
			this.stopServiceEvent = new ManualResetEvent(false);
			this.endRequestEvent = new AutoResetEvent(false);
			this.applicationSettings = (ApplicationSettings)ConfigurationManager.GetSection("applicationSettings");
			this.AutoLog = this.applicationSettings.LogEvents;
			this.InitializeComponent();
			this.LoadDirectories();
		}

		private string GetReportNamePrefix()
		{
			StringBuilder reportNamePrefix = new StringBuilder();
			string reportName = this.applicationSettings.ReportName;
			for (int index = 0; index < reportName.Length; ++index)
			{
				char ch = reportName[index];
				if (ch == '%')
					break;
				reportNamePrefix.Append(ch);
			}
			return reportNamePrefix.ToString();
		}

		private bool GetNextToken(ref int index, out char token)
		{
			token = '\0';
			string reportName = this.applicationSettings.ReportName;
			if (index >= reportName.Length)
				return false;
			while (index < reportName.Length)
			{
				char ch = reportName[index];
				if (ch == '%')
				{
					if (++index >= reportName.Length)
						break;
					ch = reportName[index++];
					token = ch;
					return true;
				}
				++index;
			}
			return true;
		}

		private bool AutoNumberReports()
		{
			int index = 0;
			char token;
			while (GetNextToken(ref index, out token))
			{
				if (token == 'n')
					return true;
			}
			return false;
		}

		private int GetReportNumber(string fileName)
		{
			int dataIndex = 0, refIndex = 0;
			for (; ; )
			{
				char token;
				int refStart = refIndex;
				if (!GetNextToken(ref refIndex, out token))
					break;
				int length = refIndex - refStart;
				if (token == 'n' || token == 't')
					length -= 2;
				else
					length -= 1;
				dataIndex += length;
				if (token == 'n' || token == 't')
				{
					int dataStart = dataIndex;
					while (dataIndex < fileName.Length && char.IsDigit(fileName[dataIndex]))
						++dataIndex;
					if (token == 'n')
					{
						int dataLength = dataIndex - dataStart;
						if (dataLength > 0)
							return int.Parse(fileName.Substring(dataStart, dataLength));
					}
				}
			}
			return -1;
		}

		private string GetReportName(int reportNumber, string extension)
		{
			StringBuilder reportName = new StringBuilder();
			int index = 0;
			for (; ; )
			{
				char token;
				int start = index;
				if (!GetNextToken(ref index, out token))
					break;
				int length = index - start;
				if (token != '\0')
					length -= 2;
				reportName.Append(this.applicationSettings.ReportName, start, length);
				switch (token)
				{
				case 'n':
					reportName.Append(reportNumber);
					break;
				case 't':
					reportName.Append(DateTime.Now.Ticks);
					break;
				default:
					if (token != '\0')
						reportName.Append(token);
					break;
				}
			}
			reportName.Append('.');
			reportName.Append(extension);
			return reportName.ToString();
		}

		/// <summary>
		/// Load directories contents from the disk.
		/// </summary>
		private void LoadDirectories()
		{
			Directory.CreateDirectory(this.applicationSettings.ReportPath);
			string[] directories = Directory.GetDirectories(this.applicationSettings.ReportPath);
			string reportNamePrefix = GetReportNamePrefix();
			string reportNameFilter = reportNamePrefix + '*';
			bool autoNumberReports = AutoNumberReports();
			foreach (string directory in directories)
			{
				string dirName = Path.GetFileName(directory);
				AppDirInfo appDirInfo = new AppDirInfo();
				string[] files = Directory.GetFiles(directory, reportNameFilter);
				foreach (string file in files)
				{
					string fileName = Path.GetFileName(file);
					if (!fileName.StartsWith(reportNamePrefix))
						continue;
					++appDirInfo.NumReports;
					if (autoNumberReports)
					{
						int reportNumber = GetReportNumber(fileName);
						if (reportNumber >= 0 && appDirInfo.MaxReportNumber < reportNumber)
							appDirInfo.MaxReportNumber = reportNumber;
					}
					else
						appDirInfo.MaxReportNumber = appDirInfo.NumReports;
				}
				this.lastReportNumbers[dirName] = appDirInfo;
			}
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
		/// <param name="appEntry">Application entry to search for.</param>
		/// <returns>True if appropriate entry was found and false otherwise.</returns>
		private bool FindAppEntry(AppEntry appEntry)
		{
			Set<AppEntry> applicationList = this.applicationSettings.ApplicationList;
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
		/// <param name="extension">Report file extension.</param>
		/// <returns>True if appropriate extension was found and false otherwise.</returns>
		private bool FindReportFileExtension(string extension)
		{
			Set<string> reportFileExtensions = this.applicationSettings.ReportFileExtensions;
			if (reportFileExtensions == null || reportFileExtensions.Count == 0)
				return true;
			return reportFileExtensions.Contains(extension);
		}

		/// <summary>
		/// Represents the method that will handle the SendCompleted event.
		/// </summary>
		/// <param name="sender">The source of the event.</param>
		/// <param name="state">An AsyncCompletedEventArgs containing event data.</param>
		private void SendCompletedCallback(Object sender, AsyncCompletedEventArgs state)
		{
			this.BeginCallback();
			try
			{
				SmtpState smtpState = (SmtpState)state.UserState;
				if (state.Error != null)
					this.ReportNestedError(state.Error);
				smtpState.Dispose();
			}
			catch (Exception error)
			{
				this.ReportNestedError(error);
			}
			finally
			{
				this.EndCallback();
			}
		}

		/// <summary>
		/// Send notification e-mail.
		/// </summary>
		/// <param name="email">Notification e-mail address.</param>
		/// <param name="appTitle">Application title.</param>
		private void SendEMail(string email, string appTitle)
		{
			if (! string.IsNullOrEmpty(this.applicationSettings.SmtpHost) &&
				! string.IsNullOrEmpty(email))
			{
				string senderAddress = this.applicationSettings.SenderAddress ?? string.Empty;
				MailMessage message = new MailMessage(senderAddress, email);
				message.Subject = '\"' + appTitle + "\" error report";
				message.Body = "BugTrap server received error report of \"" + appTitle + "\" on " + DateTime.Now.ToString();
				SmtpClient smtpClient;
				if (this.applicationSettings.SmtpPort > 0)
					smtpClient = new SmtpClient(this.applicationSettings.SmtpHost, this.applicationSettings.SmtpPort);
				else
					smtpClient = new SmtpClient(this.applicationSettings.SmtpHost);
                smtpClient.EnableSsl = this.applicationSettings.SmtpSSL;

                SmtpState smtpState = new SmtpState(smtpClient, message);
				smtpClient.SendCompleted += SendCompletedCallback;
				if (this.applicationSettings.SmtpUser != null)
					smtpClient.Credentials = new NetworkCredential(this.applicationSettings.SmtpUser, this.applicationSettings.SmtpPassword);
				smtpClient.SendAsync(message, smtpState);
			}
		}

		/// <summary>
		/// Get inner exception object.
		/// </summary>
		/// <param name="error">Topmost exception object.</param>
		/// <returns>Inner exception object.</returns>
		public static Exception GetNestedError(Exception error)
		{
			while (error.InnerException != null)
				error = error.InnerException;
			return error;
		}

		/// <summary>
		/// Print error message on a console and add error message to the event log.
		/// </summary>
		/// <param name="error">Exception information.</param>
		public void ReportError(Exception error)
		{
			string message = error.ToString();
			Debug.WriteLine(message);
			if (this.applicationSettings.LogEvents)
				this.EventLog.WriteEntry(message, EventLogEntryType.Error);
		}

		/// <summary>
		/// Print error message on a console and add error message to the event log.
		/// </summary>
		/// <param name="error">Exception information.</param>
		public void ReportNestedError(Exception error)
		{
			error = GetNestedError(error);
			this.ReportError(error);
		}

		/// <summary>
		/// Begin asynchronous callback by incremented number of open task items.
		/// </summary>
		private void BeginCallback()
		{
			Interlocked.Increment(ref this.openTasks);
		}

		/// <summary>
		/// Complete asynchronous callback by decrementing number of running tasks.
		/// </summary>
		private void EndCallback()
		{
			// Decrement number of pending file operations.
			Interlocked.Decrement(ref this.openTasks);
			// Notify waiting thread about the end of operation.
			this.endRequestEvent.Set();
		}

		/// <summary>
		/// Complete asynchronous callback by release connection objects and by decrementing number of running tasks.
		/// </summary>
		/// <param name="socketState">Connection objects allocated by this task.</param>
		/// <param name="shutdown">True for normal shutdown and close for abortive shutdown.</param>
		private void EndCallback(SocketState socketState, bool shutdown)
		{
			if (socketState != null)
				socketState.Close(shutdown);
			this.EndCallback();
		}

		/// <summary>
		/// Handles client request and stores bug report in repository.
		/// </summary>
		/// <param name="state">Socket state object.</param>
		private void HandleRequestCallback(object state)
		{
			SocketState socketState = (SocketState)state;
			bool shutdown = false;
			this.BeginCallback();
			try
			{
				BinaryReader binReader = null;
				FileStream outputStream = null;
				try
				{
					// Extract data from the stream.
					FileStream tempStream = socketState.TempStream;
					tempStream.Seek(0, SeekOrigin.Begin);
					long dataSize = tempStream.Length;
					// Message size (DWORD) + protocol signature (DWORD) = 8 bytes.
					if (dataSize < 8)
						throw new ApplicationException("Invalid message size");
					binReader = new BinaryReader(tempStream, Encoding.UTF8);
					if (BugTrapService.protocolSignature != binReader.ReadUInt32())
						throw new ApplicationException("Unsupported protocol version");
					if (dataSize != binReader.ReadInt32())
						throw new ApplicationException("Invalid message size");
					MessageType messageType = (MessageType)binReader.ReadByte();
					if (messageType != MessageType.CompundMessage)
						throw new ApplicationException("Unsupported message type");
					CompoundMessageFlags messageFlags = (CompoundMessageFlags)binReader.ReadUInt32();
					if (messageFlags != CompoundMessageFlags.None)
						throw new ApplicationException("Unsupported message flags");
					string appName = binReader.ReadString();
					string appVersion = binReader.ReadString();
					string extension = binReader.ReadString();
					string email = binReader.ReadString();
					if (appName == string.Empty)
						appName = "(UNTITLED)";
					string appTitle = appVersion == string.Empty ? appName : appName + ' ' + appVersion;
					string dirName = GetAppDirName(appTitle);
					int reportSize = (int)(tempStream.Length - tempStream.Position);
					if ((this.applicationSettings.MaxReportSize >= 0 && reportSize > this.applicationSettings.MaxReportSize) || reportSize <= 0)
						throw new ApplicationException("Report exceeds size limit");
					AppEntry appEntry = new AppEntry(appName, appVersion);
					if (!this.FindAppEntry(appEntry))
						throw new ApplicationException("Report excluded by filter");
					if (!this.FindReportFileExtension(extension))
						throw new ApplicationException("Invalid report file extension");
					int maxReportNumber = 0, numReports = 0;
					lock (this.lastReportNumbers)
					{
						AppDirInfo appDirInfo;
						if (this.lastReportNumbers.TryGetValue(dirName, out appDirInfo))
						{
							maxReportNumber = appDirInfo.MaxReportNumber;
							numReports = appDirInfo.NumReports;
						}
						if (this.applicationSettings.ReportsLimit >= 0 && numReports > this.applicationSettings.ReportsLimit)
							throw new ApplicationException("Number of reports exceeds the limit");
						if (appDirInfo == null)
						{
							appDirInfo = new AppDirInfo();
							this.lastReportNumbers[dirName] = appDirInfo;
						}
						appDirInfo.NumReports = ++numReports;
						appDirInfo.MaxReportNumber = ++maxReportNumber;
					}
					string reportDir = Path.Combine(this.applicationSettings.ReportPath, dirName);
					Directory.CreateDirectory(reportDir);
					string fileName = GetReportName(maxReportNumber, extension);
					string filePath = Path.Combine(reportDir, fileName);
					outputStream = new FileStream(filePath, FileMode.Create, FileAccess.ReadWrite, FileShare.None);
					byte[] reportData = new byte[1024];
					while (reportSize > 0)
					{
						int chunkSize = Math.Min(reportSize, reportData.Length);
						binReader.Read(reportData, 0, chunkSize);
						outputStream.Write(reportData, 0, chunkSize);
						reportSize -= chunkSize;
					}
					this.SendEMail(email, appTitle);
					// Done!
					shutdown = true;
				}
				finally
				{
					if (binReader != null)
						binReader.Close();
					if (outputStream != null)
						outputStream.Close();
				}
			}
			catch (Exception error)
			{
				this.ReportNestedError(error);
			}
			finally
			{
				this.EndCallback(socketState, shutdown);
			}
		}

		/// <summary>
		/// Called when client socket receives data.
		/// </summary>
		/// <param name="ar">The result of the asynchronous operation.</param>
		private void ReceiveRequestCallback(IAsyncResult ar)
		{
			SocketState socketState = (SocketState)ar.AsyncState;
			this.BeginCallback();
			try
			{
				Socket clientSocket = socketState.ClientSocket;
				byte[] readBuffer = socketState.ReadBuffer;
				FileStream tempStream = socketState.TempStream;
				int received = clientSocket.EndReceive(ar);
				if (received == 0)
				{
					ThreadPool.QueueUserWorkItem(new WaitCallback(HandleRequestCallback), socketState);
					// Keep connection open.
					socketState = null;
				}
				else if (this.applicationSettings.MaxReportSize < 0 || tempStream.Length + received <= this.applicationSettings.MaxReportSize + 1024)
				{
					// Store data in a buffer.
					tempStream.Write(readBuffer, 0, received);
					// Request next data chunk.
					clientSocket.BeginReceive(readBuffer, 0, readBuffer.Length, SocketFlags.None, ReceiveRequestCallback, socketState);
					// Keep connection open.
					socketState = null;
				}
				else
				{
					throw new ApplicationException("Message size exceeds maximum limit");
				}
			}
			catch (Exception error)
			{
				this.ReportNestedError(error);
			}
			finally
			{
				this.EndCallback(socketState, false);
			}
		}

		/// <summary>
		/// Called when incoming socket becomes accepted.
		/// </summary>
		/// <param name="ar">The result of the asynchronous operation.</param>
		private void AcceptCallback(IAsyncResult ar)
		{
			SocketState socketState = null;
			Socket serverSocket = (Socket)ar.AsyncState;
			try
			{
				Socket clientSocket = serverSocket.EndAccept(ar);
				this.acceptCompleted.Set();
				socketState = new SocketState(this, clientSocket);
				byte[] readBuffer = socketState.ReadBuffer;
				clientSocket.BeginReceive(readBuffer, 0, readBuffer.Length, SocketFlags.None, ReceiveRequestCallback, socketState);
				// Keep connection open.
				socketState = null;
			}
			catch (ObjectDisposedException)
			{
				// Accept interrupted.
			}
			catch (Exception error)
			{
				this.ReportNestedError(error);
			}
			finally
			{
				if (socketState != null)
					socketState.Dispose();
			}
		}

		/// <summary>
		/// Service entry point.
		/// </summary>
		public void ServiceMain()
		{
			this.openTasks = 0;
			this.started = false;
			this.serverInit.Reset();
			Socket serverSocket = null;
			try
			{
				try
				{
					serverSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
					serverSocket.Bind(new IPEndPoint(IPAddress.Any, this.applicationSettings.ServerPort));
					serverSocket.Listen(10);
					this.started = true;
					if (this.interactiveMode)
						Console.WriteLine("Service is running");
				}
				finally
				{
					this.serverInit.Set();
				}
				WaitHandle[] events = new WaitHandle[] { this.acceptCompleted, this.stopServiceEvent };
				for (;;)
				{
					this.acceptCompleted.Reset();
					IAsyncResult ar = serverSocket.BeginAccept(AcceptCallback, serverSocket);
					if (WaitHandle.WaitAny(events) == 1)
					{
						while (! Interlocked.Equals(this.openTasks, 0))
						{
							this.endRequestEvent.WaitOne();
						}
						break;
					}
				}
			}
			catch (Exception error)
			{
				this.ReportNestedError(error);
			}
			finally
			{
				if (serverSocket != null)
					serverSocket.Close();
			}
		}

		/// <summary>
		/// Handle service start.
		/// </summary>
		private void StartService()
		{
			this.listenerThread = new Thread(ServiceMain);
			this.listenerThread.Name = "Listener";
			this.listenerThread.IsBackground = true;
			this.listenerThread.Start();
			this.serverInit.WaitOne();
			if (! this.started)
				throw new ApplicationException("Unable to start server listener");
		}

		/// <summary>
		/// Handle service stop.
		/// </summary>
		private void StopService()
		{
			this.stopServiceEvent.Set();
			this.listenerThread.Join();
			this.listenerThread = null;
		}

		/// <summary>
		/// Called when service is being started.
		/// </summary>
		/// <param name="args">Command line arguments.</param>
		protected override void OnStart(string[] args)
		{
			StartService();
		}

		/// <summary>
		/// Called when service is being stopped.
		/// </summary>
		protected override void OnStop()
		{
			StopService();
		}

		/// <summary>
		/// Called when service is being resumed.
		/// </summary>
		protected override void OnContinue()
		{
			StartService();
		}

		/// <summary>
		/// Called when service is being paused.
		/// </summary>
		protected override void OnPause()
		{
			StopService();
		}
	}
}
