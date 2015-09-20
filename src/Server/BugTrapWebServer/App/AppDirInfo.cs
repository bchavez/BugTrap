/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Directory information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;

namespace BugTrapServer
{
	/// <summary>
	/// Keeps directory information for the application.
	/// </summary>
	public class AppDirInfo
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
}
