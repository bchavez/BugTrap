/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Windows impersonation context.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Security.Principal;
using System.Reflection;
using System.ComponentModel;

namespace IntelleSoft.Services
{
    public class WindowsLogonImpersonationContext : IDisposable
    {
        #region Constants Declarations

        private const uint LOGON32_LOGON_INTERACTIVE = 2;
        private const uint LOGON32_LOGON_NETWORK = 3;
        private const uint LOGON32_LOGON_BATCH = 4;
        private const uint LOGON32_LOGON_SERVICE = 5;
        private const uint LOGON32_LOGON_UNLOCK = 7;
        private const uint LOGON32_LOGON_NETWORK_CLEARTEXT = 8;
        private const uint LOGON32_LOGON_NEW_CREDENTIALS = 9;

        private const uint LOGON32_PROVIDER_DEFAULT = 0;
        private const uint LOGON32_PROVIDER_WINNT35 = 1;
        private const uint LOGON32_PROVIDER_WINNT40 = 2;
        private const uint LOGON32_PROVIDER_WINNT50 = 3;

        #endregion

        #region DLL Imports

        [DllImport("advapi32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public static extern bool LogonUser([In] string lpszUsername,
                                            [In] string lpszDomain,
                                            [In] string lpszPassword,
                                            [In] uint dwLogonType,
                                            [In] uint dwLogonProvider,
                                            [Out] out IntPtr phToken);

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        public extern static bool CloseHandle([In] IntPtr handle);

        #endregion

        public enum LogonType : uint
        {
            Interactive = LOGON32_LOGON_INTERACTIVE,
            Network = LOGON32_LOGON_NETWORK,
            Batch = LOGON32_LOGON_BATCH,
            Service = LOGON32_LOGON_SERVICE,
            Unlock = LOGON32_LOGON_UNLOCK,
            NetworkClearText = LOGON32_LOGON_NETWORK_CLEARTEXT,
            NewCredentials = LOGON32_LOGON_NEW_CREDENTIALS
        }

        private IntPtr tokenHandle;
        private WindowsIdentity identity;
        private WindowsImpersonationContext impersonationContext;

        public WindowsLogonImpersonationContext()
        {
        }

        public WindowsLogonImpersonationContext(string accountName, string password, LogonType type)
        {
            Impersonate(accountName, password, type);
        }

        private void ParseAccountName(string accountName, out string domainName, out string userName)
        {
            int separator = accountName.IndexOf('\\');
            if (separator > 0)
            {
                domainName = accountName.Substring(0, separator);
                userName = accountName.Substring(separator + 1);
            }
            else if (separator < 0)
            {
                domainName = null;
                userName = accountName;
            }
            else
                throw new ArgumentException();
        }

        [PermissionSetAttribute(SecurityAction.Demand, Name = "FullTrust")]
        public void Impersonate(string accountName, string password, LogonType type)
        {
            if (this.tokenHandle != IntPtr.Zero ||
                this.identity != null ||
                this.impersonationContext != null)
            {
                throw new InvalidOperationException();
            }
            if (accountName == null)
            {
                throw new ArgumentNullException();
            }
            string domainName, userName;
            ParseAccountName(accountName, out domainName, out userName);
            try
            {
                if (!LogonUser(userName,
                               domainName,
                               password,
                               (uint)type,
                               LOGON32_PROVIDER_DEFAULT,
                               out this.tokenHandle))
                {
                    throw new Win32Exception();
                }
                this.identity = new WindowsIdentity(this.tokenHandle);
                this.impersonationContext = this.identity.Impersonate();
            }
            catch (Exception)
            {
                this.Undo();
                throw;
            }
        }

        public void Undo()
        {
            this.DisposeObjects();
            this.CloseHandles();
        }

        private void DisposeObjects()
        {
            if (this.impersonationContext != null)
            {
                this.impersonationContext.Undo();
                this.impersonationContext.Dispose();
                this.impersonationContext = null;
            }
            /*if (this.identity != null)
            {
                this.identity.Dispose();
                this.identity = null;
            }*/
            this.identity = null;
        }

        private void CloseHandles()
        {
            if (this.tokenHandle != IntPtr.Zero)
            {
                CloseHandle(this.tokenHandle);
                this.tokenHandle = IntPtr.Zero;
            }
        }

        void IDisposable.Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        public void Dispose()
        {
            IDisposable disp = this;
            disp.Dispose();
        }

        protected void Dispose(bool disposing)
        {
            if (!this.disposed)
            {
                if (disposing)
                {
                    this.DisposeObjects();
                }
                this.CloseHandles();
                this.disposed = true;
            }
        }

        ~WindowsLogonImpersonationContext()
        {
            this.Dispose(false);
        }

        private bool disposed;
    }
}
