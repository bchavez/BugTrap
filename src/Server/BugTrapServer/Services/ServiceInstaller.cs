/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Service installer.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.IO;
using System.Runtime.InteropServices;
using System.ComponentModel;
using System.ServiceProcess;
using System.Reflection;
using System.Security.Permissions;

namespace IntelleSoft.Services
{
	public class ServiceInstaller
	{
#region Constants Declarations
		private const uint STANDARD_RIGHTS_REQUIRED      = 0xF0000;

		private const uint SC_MANAGER_CONNECT            = 0x00001;
		private const uint SC_MANAGER_CREATE_SERVICE     = 0x00002;
		private const uint SC_MANAGER_ENUMERATE_SERVICE  = 0x00004;
		private const uint SC_MANAGER_LOCK               = 0x00008;
		private const uint SC_MANAGER_QUERY_LOCK_STATUS  = 0x00010;
		private const uint SC_MANAGER_MODIFY_BOOT_CONFIG = 0x00020;

		private const uint SC_MANAGER_ALL_ACCESS         = (STANDARD_RIGHTS_REQUIRED      |
		                                                    SC_MANAGER_CONNECT            |
		                                                    SC_MANAGER_CREATE_SERVICE     |
		                                                    SC_MANAGER_ENUMERATE_SERVICE  |
		                                                    SC_MANAGER_LOCK               |
		                                                    SC_MANAGER_QUERY_LOCK_STATUS  |
		                                                    SC_MANAGER_MODIFY_BOOT_CONFIG);

		private const uint SERVICE_WIN32_OWN_PROCESS     = 0x00000010;
		private const uint SERVICE_WIN32_SHARE_PROCESS   = 0x00000020;
		private const uint SERVICE_WIN32                 = (SERVICE_WIN32_OWN_PROCESS | SERVICE_WIN32_SHARE_PROCESS);
		private const uint SERVICE_INTERACTIVE_PROCESS   = 0x00000100;

		private const uint SERVICE_ERROR_IGNORE          = 0x00000000;
		private const uint SERVICE_ERROR_NORMAL          = 0x00000001;
		private const uint SERVICE_ERROR_SEVERE          = 0x00000002;
		private const uint SERVICE_ERROR_CRITICAL        = 0x00000003;

		private const uint SERVICE_AUTO_START            = 0x00000002;
		private const uint SERVICE_DEMAND_START          = 0x00000003;
		private const uint SERVICE_DISABLED              = 0x00000004;

		private const uint SERVICE_QUERY_CONFIG          = 0x0001;
		private const uint SERVICE_CHANGE_CONFIG         = 0x0002;
		private const uint SERVICE_QUERY_STATUS          = 0x0004;
		private const uint SERVICE_ENUMERATE_DEPENDENTS  = 0x0008;
		private const uint SERVICE_START                 = 0x0010;
		private const uint SERVICE_STOP                  = 0x0020;
		private const uint SERVICE_PAUSE_CONTINUE        = 0x0040;
		private const uint SERVICE_INTERROGATE           = 0x0080;
		private const uint SERVICE_USER_DEFINED_CONTROL  = 0x0100;

		private const uint SERVICE_ALL_ACCESS           = (STANDARD_RIGHTS_REQUIRED     |
		                                                   SERVICE_QUERY_CONFIG         |
														   SERVICE_CHANGE_CONFIG        |
														   SERVICE_QUERY_STATUS         |
														   SERVICE_ENUMERATE_DEPENDENTS |
														   SERVICE_START                |
														   SERVICE_STOP                 |
														   SERVICE_PAUSE_CONTINUE       |
														   SERVICE_INTERROGATE          |
														   SERVICE_USER_DEFINED_CONTROL);
#endregion

#region DLL Imports

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		public static extern IntPtr OpenSCManager([In] string lpMachineName,
		                                          [In] string lpDatabaseName,
												  [In] uint dwDesiredAccess);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		public static extern IntPtr CreateService([In] IntPtr hSCManager,
												  [In] string lpServiceName,
												  [In] string lpDisplayName,
												  [In] uint dwDesiredAccess,
												  [In] uint dwServiceType,
												  [In] uint dwStartType,
												  [In] uint dwErrorControl,
												  [In] string lpBinaryPathName,
												  [In] string lpLoadOrderGroup,
												  [In] UIntPtr lpdwTagId,
												  [In] string lpDependencies,
												  [In] string lpServiceStartName,
												  [In] string lpPassword);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		public static extern void CloseServiceHandle(IntPtr hSCObject);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		public static extern int StartService([In] IntPtr hService,
		                                      [In] uint dwNumServiceArgs,
											  [In, MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPWStr, SizeParamIndex = 1)] string[] lpServiceArgVectors);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		public static extern IntPtr OpenService([In] IntPtr hSCManager,
		                                        [In] string lpServiceName,
		                                        [In] uint dwDesiredAccess);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		public static extern int DeleteService([In] IntPtr hService);

#endregion

		public class ServiceAccountDescriptor
		{
			public ServiceAccountDescriptor()
			{
				this.AccountType = ServiceAccount.LocalSystem;
			}

			public ServiceAccountDescriptor(ServiceAccount accountType)
			{
				this.AccountType = accountType;
			}

			public ServiceAccountDescriptor(string accountName, string password)
			{
				this.AccountType = ServiceAccount.User;
				this.AccountName = accountName;
				this.Password = password;
			}

			public ServiceAccountDescriptor(System.ServiceProcess.ServiceProcessInstaller serviceProcessInstaller)
			{
				this.AccountType = serviceProcessInstaller.Account;
				this.AccountName = serviceProcessInstaller.Username;
				this.Password = serviceProcessInstaller.Password;
			}

			public ServiceAccount AccountType;
			public string AccountName;
			public string Password;
		};

		private static uint GetSratModeFlag(ServiceStartMode startMode)
		{
			switch (startMode)
			{
			case ServiceStartMode.Automatic:
				return SERVICE_AUTO_START;
			case ServiceStartMode.Manual:
				return SERVICE_DEMAND_START;
			case ServiceStartMode.Disabled:
				return SERVICE_DISABLED;
			default:
				throw new InvalidEnumArgumentException();
			}
		}

		private static void GetAccountFields(ServiceAccountDescriptor accountDescriptor, out string accountName, out string password)
		{
			switch (accountDescriptor.AccountType)
			{
			case ServiceAccount.LocalSystem:
				accountName = null;
				password = null;
				break;
			case ServiceAccount.LocalService:
				accountName = @"NT AUTHORITY\LocalService";
				password = string.Empty;
				break;
			case ServiceAccount.NetworkService:
				accountName = @"NT AUTHORITY\NetworkService";
				password = string.Empty;
				break;
			case ServiceAccount.User:
				accountName = accountDescriptor.AccountName.IndexOf('\\') >= 0 ? accountDescriptor.AccountName : ".\\" + accountDescriptor.AccountName;
				password = accountDescriptor.Password;
				break;
			default:
				throw new InvalidEnumArgumentException();
			}
		}

		[PermissionSetAttribute(SecurityAction.Demand, Name = "FullTrust")]
		public static void InstallService(string serviceName, string displayName, string servicePath, ServiceStartMode startMode, ServiceAccountDescriptor accountDescriptor)
		{
			uint startModeFlag = GetSratModeFlag(startMode);
			string accountName, password;
			GetAccountFields(accountDescriptor, out accountName, out password);
			if (accountDescriptor.AccountType == ServiceAccount.User)
				AccountRights.SetRights(accountDescriptor.AccountName, "SeServiceLogonRight", true);
			IntPtr hSCMHandle = IntPtr.Zero, hServHandle = IntPtr.Zero;
			try
			{
				hSCMHandle = OpenSCManager(null, null, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
				if (hSCMHandle == IntPtr.Zero)
					throw new Win32Exception();
				hServHandle = CreateService(hSCMHandle,
											serviceName,
											displayName,
											SC_MANAGER_CREATE_SERVICE,
											SERVICE_WIN32_OWN_PROCESS,
											startModeFlag,
											SERVICE_ERROR_NORMAL,
											servicePath,
											null,
											UIntPtr.Zero,
											null,
											accountName,
											password);
				if (hServHandle == IntPtr.Zero)
					throw new Win32Exception();
			}
			finally
			{
				if (hServHandle != IntPtr.Zero)
					CloseServiceHandle(hServHandle);
				if (hSCMHandle != IntPtr.Zero)
					CloseServiceHandle(hSCMHandle);
			}
		}

		[PermissionSetAttribute(SecurityAction.Demand, Name = "FullTrust")]
		public static void UninstallService(string serviceName)
		{
			IntPtr hSCMHandle = IntPtr.Zero, hServHandle = IntPtr.Zero;
			try
			{
				hSCMHandle = OpenSCManager(null, null, SC_MANAGER_CONNECT);
				if (hSCMHandle == IntPtr.Zero)
					throw new Win32Exception();
				hServHandle = OpenService(hSCMHandle,
										  serviceName,
										  STANDARD_RIGHTS_REQUIRED);
				if (hServHandle == IntPtr.Zero)
					throw new Win32Exception();
				if (DeleteService(hServHandle) == 0)
					throw new Win32Exception();
			}
			finally
			{
				if (hServHandle != IntPtr.Zero)
					CloseServiceHandle(hServHandle);
				if (hSCMHandle != IntPtr.Zero)
					CloseServiceHandle(hSCMHandle);
			}
		}

		public static void InstallService(string serviceName, string displayName, ServiceStartMode startMode, ServiceAccountDescriptor accountDescriptor)
		{
			string servicePath = Assembly.GetExecutingAssembly().Location;
			InstallService(serviceName, displayName, servicePath, startMode, accountDescriptor);
		}

		private static void FindInstallers(System.Configuration.Install.Installer installer, out System.ServiceProcess.ServiceProcessInstaller serviceProcessInstaller, out System.ServiceProcess.ServiceInstaller serviceInstaller)
		{
			serviceProcessInstaller = null;
			serviceInstaller = null;
			foreach (System.Configuration.Install.Installer nestedInstaller in installer.Installers)
			{
				if (nestedInstaller is System.ServiceProcess.ServiceProcessInstaller)
					serviceProcessInstaller = (System.ServiceProcess.ServiceProcessInstaller)nestedInstaller;
				else if (nestedInstaller is System.ServiceProcess.ServiceInstaller)
					serviceInstaller = (System.ServiceProcess.ServiceInstaller)nestedInstaller;
				if (serviceProcessInstaller != null && serviceInstaller != null)
					break;
			}
			if (serviceProcessInstaller == null || serviceInstaller == null)
				throw new ArgumentException();
		}

		public static void InstallService(System.Configuration.Install.Installer installer)
		{
			System.ServiceProcess.ServiceProcessInstaller serviceProcessInstaller;
			System.ServiceProcess.ServiceInstaller serviceInstaller;
			FindInstallers(installer, out serviceProcessInstaller, out serviceInstaller);
			ServiceAccountDescriptor accountDescriptor = new ServiceAccountDescriptor(serviceProcessInstaller);
			InstallService(serviceInstaller.ServiceName, serviceInstaller.DisplayName, serviceInstaller.StartType, accountDescriptor);
		}

		public static void UninstallService(System.Configuration.Install.Installer installer)
		{
			System.ServiceProcess.ServiceProcessInstaller serviceProcessInstaller;
			System.ServiceProcess.ServiceInstaller serviceInstaller;
			FindInstallers(installer, out serviceProcessInstaller, out serviceInstaller);
			UninstallService(serviceInstaller.ServiceName);
		}
	}
}
