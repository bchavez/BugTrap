/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Account rights manager. Used by service installer.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.ComponentModel;
using System.Text;

namespace IntelleSoft.Services
{
	public class AccountRights
	{
#region Type Definitions

		[StructLayout(LayoutKind.Sequential)]
		private struct s_LSA_UNICODE_STRING
		{
			public ushort Length;
			public ushort MaximumLength;
			public IntPtr Buffer;
		}

		[StructLayout(LayoutKind.Sequential)]
		private class c_LSA_UNICODE_STRING
		{
			public ushort Length;
			public ushort MaximumLength;
			public IntPtr Buffer;
		}

		[StructLayout(LayoutKind.Sequential)]
		private class c_LSA_OBJECT_ATTRIBUTES
		{
			public int Length;
			public IntPtr RootDirectory;
			public s_LSA_UNICODE_STRING ObjectName;
			public uint Attributes;
			public IntPtr SecurityDescriptor;
			public IntPtr SecurityQualityOfService;
		}

#endregion

#region Constants Declarations

		private const int STATUS_SUCCESS                    = 0;
		private const int ERROR_INSUFFICIENT_BUFFER        = 122;

		private const uint POLICY_VIEW_LOCAL_INFORMATION   = 0x00000001;
		private const uint POLICY_VIEW_AUDIT_INFORMATION   = 0x00000002;
		private const uint POLICY_GET_PRIVATE_INFORMATION  = 0x00000004;
		private const uint POLICY_TRUST_ADMIN              = 0x00000008;
		private const uint POLICY_CREATE_ACCOUNT           = 0x00000010;
		private const uint POLICY_CREATE_SECRET            = 0x00000020;
		private const uint POLICY_CREATE_PRIVILEGE         = 0x00000040;
		private const uint POLICY_SET_DEFAULT_QUOTA_LIMITS = 0x00000080;
		private const uint POLICY_SET_AUDIT_REQUIREMENTS   = 0x00000100;
		private const uint POLICY_AUDIT_LOG_ADMIN          = 0x00000200;
		private const uint POLICY_SERVER_ADMIN             = 0x00000400;
		private const uint POLICY_LOOKUP_NAMES             = 0x00000800;
		private const uint POLICY_NOTIFICATION             = 0x00001000;

#endregion

#region DLL Imports

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
		private static extern bool LookupAccountName([In] string lpSystemName,
		                                             [In] string lpAccountName,
		                                             [In] IntPtr pSid,
		                                             [In, Out] ref int cbSid,
		                                             [Out] StringBuilder domainName,
		                                             [In, Out] ref int cbDomain,
		                                             [Out] out int peUse);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode)]
		private static extern int LsaClose(IntPtr ObjectHandle);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode)]
		private static extern int LsaOpenPolicy([In] c_LSA_UNICODE_STRING SystemName,
		                                        [In] c_LSA_OBJECT_ATTRIBUTES ObjectAttributes,
		                                        [In] uint DesiredAccess,
		                                        [Out] out IntPtr PolicyHandle);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode)]
		private static extern int LsaAddAccountRights([In] IntPtr PolicyHandle,
		                                              [In] IntPtr AccountSid,
		                                              [In] s_LSA_UNICODE_STRING[] UserRights,
		                                              [In] int CountOfRights);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode)]
		private static extern int LsaRemoveAccountRights([In] IntPtr PolicyHandle,
		                                                 [In] IntPtr AccountSid,
		                                                 [In] byte AllRights,
		                                                 [In] s_LSA_UNICODE_STRING[] UserRights,
		                                                 [In] int CountOfRights);

		[DllImport("advapi32.dll", CharSet = CharSet.Unicode)]
		private static extern int LsaNtStatusToWinError(int Status);

#endregion

		public static void SetRights(string accountName, string privilegeName, bool enable)
		{
			if (accountName.StartsWith(".\\"))
				accountName = accountName.Substring(2);
			c_LSA_OBJECT_ATTRIBUTES lsaObjAttrs = new c_LSA_OBJECT_ATTRIBUTES();
			lsaObjAttrs.Length = Marshal.SizeOf(lsaObjAttrs);
			IntPtr lsaPolicy = IntPtr.Zero, pAccSid = IntPtr.Zero, privilegeHandle = IntPtr.Zero;
			try
			{
				int status = LsaOpenPolicy(null, lsaObjAttrs, POLICY_CREATE_ACCOUNT | POLICY_LOOKUP_NAMES, out lsaPolicy);
				if (status != STATUS_SUCCESS)
					throw new Win32Exception(LsaNtStatusToWinError(status));
				int cbSid = 128, cchRefDomain = 16;
				pAccSid = Marshal.AllocHGlobal(cbSid);
				StringBuilder refDomain = new StringBuilder(cchRefDomain);
				int sidNameUse, cbNextSid = cbSid, cchNextRefDomain = cchRefDomain;
				while (!LookupAccountName(null, accountName, pAccSid, ref cbNextSid, refDomain, ref cchNextRefDomain, out sidNameUse))
				{
					if (Marshal.GetLastWin32Error() != ERROR_INSUFFICIENT_BUFFER)
						throw new Win32Exception();
					if (cbSid < cbNextSid)
					{
						Marshal.ReAllocHGlobal(pAccSid, (IntPtr)cbNextSid);
						cbSid = cbNextSid;
					}
					if (cchRefDomain < cchNextRefDomain)
					{
						refDomain.EnsureCapacity(cchNextRefDomain);
						cchRefDomain = cchNextRefDomain;
					}
				}
				if (privilegeName.Length > 0x7FFE)
					throw new ArgumentOutOfRangeException();
				privilegeHandle = Marshal.StringToHGlobalUni(privilegeName);
				s_LSA_UNICODE_STRING[] lsaPrivilege = new s_LSA_UNICODE_STRING[1];
				lsaPrivilege[0] = new s_LSA_UNICODE_STRING();
				lsaPrivilege[0].Buffer = privilegeHandle;
				lsaPrivilege[0].Length = (ushort)(privilegeName.Length * UnicodeEncoding.CharSize);
				lsaPrivilege[0].MaximumLength = (ushort)((privilegeName.Length + 1) * UnicodeEncoding.CharSize);
				if (enable)
					status = LsaAddAccountRights(lsaPolicy, pAccSid, lsaPrivilege, lsaPrivilege.Length);
				else
					status = LsaRemoveAccountRights(lsaPolicy, pAccSid, 0, lsaPrivilege, lsaPrivilege.Length);
				if (status != STATUS_SUCCESS)
					throw new Win32Exception(LsaNtStatusToWinError(status));
			}
			finally
			{
				if (privilegeHandle != IntPtr.Zero)
					Marshal.FreeHGlobal(privilegeHandle);
				if (pAccSid != IntPtr.Zero)
					Marshal.FreeHGlobal(pAccSid);
				if (lsaPolicy != IntPtr.Zero)
					LsaClose(lsaPolicy);
			}
		}
	}
}
