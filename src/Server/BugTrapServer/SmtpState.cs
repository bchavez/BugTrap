/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: SMTP state class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.Net.Mail;

namespace BugTrapServer
{
	/// <summary>
	/// SMTP state object.
	/// </summary>
	internal class SmtpState : IDisposable
	{
		/// <summary>
		/// Initialize the object.
		/// </summary>
		/// <param name="client">STMP client.</param>
		/// <param name="message">Mail message.</param>
		public SmtpState(SmtpClient client, MailMessage message)
		{
			this.client = client;
			this.message = message;
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
					this.message.Dispose();
				this.disposed = true;
			}
		}

		/// <summary>
		/// Allows an Object to attempt to free resources and perform other cleanup operations before the Object is reclaimed by garbage collection.
		/// </summary>
		~SmtpState()
		{
			this.Dispose(false);
		}

		/// <summary>
		/// Get SMTP client.
		/// </summary>
		public SmtpClient Client
		{
			get { return this.client; }
		}

		/// <summary>
		/// Get mail message.
		/// </summary>
		public MailMessage Message
		{
			get { return this.message; }
		}

		/// <summary>
		/// SMTP client.
		/// </summary>
		private SmtpClient client;

		/// <summary>
		/// Mail message.
		/// </summary>
		private MailMessage message;

		/// <summary>
		/// True if object has been disposed.
		/// </summary>
		private bool disposed;
	}
}
