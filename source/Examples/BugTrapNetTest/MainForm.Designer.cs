namespace BugTrapNetTest
{
	partial class MainForm
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

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.exceptionButton = new System.Windows.Forms.Button();
			this.crashDumpCheckBox = new System.Windows.Forms.CheckBox();
			this.warningMessage = new System.Windows.Forms.Label();
			this.divider = new System.Windows.Forms.GroupBox();
			this.SuspendLayout();
			// 
			// exceptionButton
			// 
			this.exceptionButton.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.exceptionButton.Location = new System.Drawing.Point(88, 91);
			this.exceptionButton.Name = "exceptionButton";
			this.exceptionButton.Size = new System.Drawing.Size(116, 23);
			this.exceptionButton.TabIndex = 0;
			this.exceptionButton.Text = "Throw Exception";
			this.exceptionButton.UseVisualStyleBackColor = true;
			this.exceptionButton.Click += new System.EventHandler(this.exceptionButton_Click);
			// 
			// crashDumpCheckBox
			// 
			this.crashDumpCheckBox.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.crashDumpCheckBox.AutoSize = true;
			this.crashDumpCheckBox.Location = new System.Drawing.Point(81, 142);
			this.crashDumpCheckBox.Name = "crashDumpCheckBox";
			this.crashDumpCheckBox.Size = new System.Drawing.Size(131, 17);
			this.crashDumpCheckBox.TabIndex = 1;
			this.crashDumpCheckBox.Text = "Generate Crash Dump";
			this.crashDumpCheckBox.UseVisualStyleBackColor = true;
			this.crashDumpCheckBox.CheckedChanged += new System.EventHandler(this.crashDumpCheckBox_CheckedChanged);
			// 
			// warningMessage
			// 
			this.warningMessage.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.warningMessage.AutoSize = true;
			this.warningMessage.ForeColor = System.Drawing.Color.Red;
			this.warningMessage.Location = new System.Drawing.Point(37, 162);
			this.warningMessage.Name = "warningMessage";
			this.warningMessage.Size = new System.Drawing.Size(218, 13);
			this.warningMessage.TabIndex = 2;
			this.warningMessage.Text = "WARNING: this may require several minutes!";
			this.warningMessage.Visible = false;
			// 
			// divider
			// 
			this.divider.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.divider.Location = new System.Drawing.Point(40, 131);
			this.divider.Name = "divider";
			this.divider.Size = new System.Drawing.Size(215, 2);
			this.divider.TabIndex = 3;
			this.divider.TabStop = false;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(292, 266);
			this.Controls.Add(this.divider);
			this.Controls.Add(this.warningMessage);
			this.Controls.Add(this.crashDumpCheckBox);
			this.Controls.Add(this.exceptionButton);
			this.Name = "MainForm";
			this.Text = "BugTrap .NET test";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button exceptionButton;
		private System.Windows.Forms.CheckBox crashDumpCheckBox;
		private System.Windows.Forms.Label warningMessage;
		private System.Windows.Forms.GroupBox divider;
	}
}

