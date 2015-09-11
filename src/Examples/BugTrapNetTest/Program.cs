using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace BugTrapNetTest
{
	static class Program
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main()
		{
			// Note:
			//
			// You  are advised to redistribute Windows forms applications
			// with  manifest  file  in order  to fix  a conflict  between
			// different  versions  of  Windows Common  Controls  library.
			// If  you won't supply  a manifest  file, BugTrap  may create
			// some handles using older version of this library regardless
			// of Application.EnableVisualStyles() call.
			//
			// The following steps may be taken to resolve this issue:
			// 1. copy app.exe.manifest  file  from  this  project to your
			// own project.
			// 2. open  project properties  and add the  following command
			// to post build events:
			// copy "$(ProjectDir)app.exe.manifest" "$(TargetPath).manifest"

			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
		}
	}
}