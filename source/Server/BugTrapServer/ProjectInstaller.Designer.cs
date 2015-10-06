namespace BugTrapServer
{
    partial class ProjectInstaller
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.btServerProcessInstaller = new System.ServiceProcess.ServiceProcessInstaller();
			this.btServerInstaller = new System.ServiceProcess.ServiceInstaller();
			//
			// btServerProcessInstaller
			//
			this.btServerProcessInstaller.Account = System.ServiceProcess.ServiceAccount.LocalSystem;
			this.btServerProcessInstaller.Password = null;
			this.btServerProcessInstaller.Username = null;
			//
			// btServerInstaller
			//
			this.btServerInstaller.Description = "Handles requests from mobile devices and provides them with GPS information";
			this.btServerInstaller.DisplayName = "BugTrap Server";
			this.btServerInstaller.ServiceName = "BugTrap Server";
			this.btServerInstaller.StartType = System.ServiceProcess.ServiceStartMode.Automatic;
			//
			// ProjectInstaller
			//
			this.Installers.AddRange(new System.Configuration.Install.Installer[] {
            this.btServerProcessInstaller,
            this.btServerInstaller});

        }

        #endregion

        private System.ServiceProcess.ServiceProcessInstaller btServerProcessInstaller;
        private System.ServiceProcess.ServiceInstaller btServerInstaller;
    }
}