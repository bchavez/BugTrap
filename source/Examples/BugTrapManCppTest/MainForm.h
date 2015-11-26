#pragma once

#include "UnmanagedThread.h"

namespace BugTrapManCppTest
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace IntelleSoft::BugTrap;

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			SetupExceptionHandler();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		void SetupExceptionHandler()
		{
			// initialize exception handler
			ExceptionHandler::AppName = "BugTrapNetTest";
			ExceptionHandler::Flags = FlagsType::DetailedMode | FlagsType::ScreenCapture | FlagsType::AttachReport | FlagsType::EditMail;
			ExceptionHandler::ReportFormat = ReportFormatType::Xml;
			ExceptionHandler::DumpType = MinidumpType::NoDump;
			ExceptionHandler::SupportEMail = "your@email.com";
			ExceptionHandler::SupportURL = "http://www.intellesoft.net";
			ExceptionHandler::SupportHost = "localhost";
			ExceptionHandler::SupportPort = 9999;
		}

		System::Windows::Forms::Button^ mgdExceptionButton;
		System::Windows::Forms::Button^ unmgdExceptionButton;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->mgdExceptionButton = (gcnew System::Windows::Forms::Button());
			this->unmgdExceptionButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			//
			// mgdExceptionButton
			//
			this->mgdExceptionButton->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->mgdExceptionButton->Location = System::Drawing::Point(65, 102);
			this->mgdExceptionButton->Name = L"mgdExceptionButton";
			this->mgdExceptionButton->Size = System::Drawing::Size(162, 23);
			this->mgdExceptionButton->TabIndex = 0;
			this->mgdExceptionButton->Text = L"Throw Managed Exception";
			this->mgdExceptionButton->UseVisualStyleBackColor = true;
			this->mgdExceptionButton->Click += gcnew System::EventHandler(this, &MainForm::mgdExceptionButton_Click);
			//
			// unmgdExceptionButton
			//
			this->unmgdExceptionButton->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->unmgdExceptionButton->Location = System::Drawing::Point(65, 141);
			this->unmgdExceptionButton->Name = L"unmgdExceptionButton";
			this->unmgdExceptionButton->Size = System::Drawing::Size(162, 23);
			this->unmgdExceptionButton->TabIndex = 1;
			this->unmgdExceptionButton->Text = L"Throw Unmanaged Exception";
			this->unmgdExceptionButton->UseVisualStyleBackColor = true;
			this->unmgdExceptionButton->Click += gcnew System::EventHandler(this, &MainForm::unmgdExceptionButton_Click);
			//
			// MainForm
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->Controls->Add(this->unmgdExceptionButton);
			this->Controls->Add(this->mgdExceptionButton);
			this->Name = L"MainForm";
			this->Text = L"BugTrap Managed C++ test";
			this->ResumeLayout(false);

		}
#pragma endregion

		System::Void mgdExceptionButton_Click(System::Object^ sender, System::EventArgs^ e)
		{
			throw gcnew System::Exception();
		}

		System::Void unmgdExceptionButton_Click(System::Object^ sender, System::EventArgs^ e)
		{
			StartUnmanagedThread();
		}
	};
}

