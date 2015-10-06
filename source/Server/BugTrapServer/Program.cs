/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Main application class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.ServiceProcess;
using System.Diagnostics;

namespace BugTrapServer
{
	/// <summary>
	/// Main application class.
	/// </summary>
	public static class Program
	{
		/// <summary>
		/// Check program argument against the key.
		/// </summary>
		/// <param name="arg">Program argument.</param>
		/// <param name="key">Key value.</param>
		/// <returns>True if program argument matches to the key.</returns>
		private static bool CompareArgument(string arg, string key)
		{
			return ((arg[0] == '/' || arg[0] == '-') &&
				string.Compare(arg, 1, key, 0, key.Length, true) == 0);
		}

		/// <summary>
		/// The main entry point for the process.
		/// </summary>
		/// <param name="args">Program arguments.</param>
		public static void Main(string[] args)
		{
			try
			{
				if (args.Length == 1)
				{
					if (CompareArgument(args[0], "r") || CompareArgument(args[0], "run"))
					{
						Console.WriteLine("Starting service...");
						(new BugTrapService(true)).ServiceMain();
					}
					else if (CompareArgument(args[0], "i") || CompareArgument(args[0], "install"))
					{
						Console.WriteLine("Installing service...");
						IntelleSoft.Services.ServiceInstaller.InstallService(new ProjectInstaller());
						Console.WriteLine("Done!");
					}
					else if (CompareArgument(args[0], "u") || CompareArgument(args[0], "uninstall"))
					{
						Console.WriteLine("Uninstalling service...");
						IntelleSoft.Services.ServiceInstaller.UninstallService(new ProjectInstaller());
						Console.WriteLine("Done!");
					}
					else
					{
						Console.WriteLine("Usage:\nBugTrapServer {/i[nstall] | /u[ninstall] | /r[un]}");
					}
				}
				else if (args.Length == 0)
				{
					ServiceBase.Run(new BugTrapService(false));
				}
			}
			catch (Exception error)
			{
				Debug.WriteLine(error);
				Console.WriteLine(error);
			}
		}
	}
}
