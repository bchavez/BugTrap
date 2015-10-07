/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Socket state class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.IO;
using System.Net.Sockets;
using System.Diagnostics;

namespace BugTrapServer
{
	/// <summary>
	/// Socket state class.
	/// </summary>
	internal class SocketState : IDisposable
	{
		/// <summary>
		/// Object initializer.
		/// </summary>
		/// <param name="btService">Main service object.</param>
		/// <param name="clientSocket">The instance of client socket.</param>
		public SocketState(BugTrapService btService, Socket clientSocket)
		{
			this.btService = btService;
			this.clientSocket = clientSocket;
			this.readBuffer = new byte[1024];
			string tempFileName = Path.Combine(Path.GetTempPath(), Path.GetTempFileName());
			this.tempStream = new FileStream(tempFileName, FileMode.Create, FileAccess.ReadWrite, FileShare.None, 1024, FileOptions.DeleteOnClose);
		}

		/// <summary>
		/// Close temporary file stream.
		/// </summary>
		private void CloseStream()
		{
			try
			{
				if (this.tempStream != null)
				{
#if DEBUG
					string temFileName = this.tempStream.Name;
					this.tempStream.Close();
					Debug.Assert(!File.Exists(temFileName));
#else
					this.tempStream.Close();
#endif
				}
			}
			catch (Exception error)
			{
				this.btService.ReportNestedError(error);
			}
			finally
			{
				this.tempStream = null;
			}
		}

		/// <summary>
		/// Close socket channel.
		/// </summary>
		/// <param name="shutdown">Pass true to gracefully close the connection.</param>
		private void CloseSocket(bool shutdown)
		{
			try
			{
				if (this.clientSocket != null)
				{
					if (shutdown)
					{
						this.clientSocket.Shutdown(SocketShutdown.Both);
					}
					else
					{
						LingerOption option = new LingerOption(true, 0);
						this.clientSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Linger, option);
					}
					this.clientSocket.Close();
				}
			}
			catch (Exception error)
			{
				this.btService.ReportNestedError(error);
			}
			finally
			{
				this.clientSocket = null;
			}
		}

		/// <summary>
		/// Close socket channel and temporary file stream.
		/// </summary>
		/// <param name="shutdown">Pass true to gracefully close the connection.</param>
		public void Close(bool shutdown)
		{
			this.CloseSocket(shutdown);
			this.CloseStream();
		}

		/// <summary>
		/// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
		/// </summary>
		public void Dispose()
		{
			this.Dispose(true);
			GC.SuppressFinalize(this);
		}

		/// <summary>
		/// Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
		/// </summary>
		/// <param name="disposing">true to release both managed and unmanaged resources; false to release only unmanaged resources.</param>
		protected void Dispose(bool disposing)
		{
			if (!this.disposed)
			{
				if (disposing)
					this.Close(false);
				this.disposed = true;
			}
		}

		/// <summary>
		/// Allows an Object to attempt to free resources and perform other cleanup operations before the Object is reclaimed by garbage collection.
		/// </summary>
		~SocketState()
		{
			this.Dispose(false);
		}

		/// <summary>
		/// Get client socket instance.
		/// </summary>
		public Socket ClientSocket
		{
			get { return this.clientSocket; }
		}

		/// <summary>
		/// Get buffer used for asynchronous operations.
		/// </summary>
		public byte[] ReadBuffer
		{
			get { return this.readBuffer; }
		}

		/// <summary>
		/// get temporary file stream that keeps intermediate data.
		/// </summary>
		public FileStream TempStream
		{
			get { return this.tempStream; }
		}

		/// <summary>
		/// Instance of client socket.
		/// </summary>
		private Socket clientSocket;

		/// <summary>
		/// Buffer used for asynchronous operations.
		/// </summary>
		private byte[] readBuffer;

		/// <summary>
		/// Accumulates results of several pending operations.
		/// </summary>
		private FileStream tempStream;

		/// <summary>
		/// Instance of main service.
		/// </summary>
		private BugTrapService btService;

		/// <summary>
		/// True if object has been disposed.
		/// </summary>
		private bool disposed;
	}
}
