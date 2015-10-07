// BugTrapTest.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "BugTrapTest.h"
#include "MainFrm.h"

#include "BugTrapTestDoc.h"
#include "BugTrapTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBugTrapTestApp

BEGIN_MESSAGE_MAP(CBugTrapTestApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_TEST_ACCESSVIOLATION, OnTestAccessViolation)
	ON_COMMAND(ID_TEST_CPPEXCEPTION, OnTestCppException)
END_MESSAGE_MAP()

// CBugTrapTestApp construction

CBugTrapTestApp::CBugTrapTestApp()
{
	// Setup exception handler
	BT_SetAppName(_T("BugTrap Test"));
	BT_SetSupportEMail(_T("your@email.com"));
	BT_SetFlags(BTF_DETAILEDMODE | BTF_EDITMAIL | BTF_ATTACHREPORT| BTF_SCREENCAPTURE);
	BT_SetSupportURL(_T("http://www.intellesoft.net"));

	// = BugTrapServer ===========================================
	BT_SetSupportServer(_T("localhost"), 9999);
	// - or -
	//BT_SetSupportServer(_T("127.0.0.1"), 9999);

	// = BugTrapWebServer ========================================
	//BT_SetSupportServer(_T("http://localhost/BugTrapWebServer/RequestHandler.aspx"), BUGTRAP_HTTP_PORT);

	// required for VS 2005 & 2008
	BT_InstallSehFilter();
}

// The one and only CBugTrapTestApp object

CBugTrapTestApp theApp;

// CBugTrapTestApp initialization

BOOL CBugTrapTestApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (! AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CBugTrapTestDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CBugTrapTestView));
	AddDocTemplate(pDocTemplate);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (! ProcessShellCommand(cmdInfo))
		return FALSE;
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public BTWindow<CDialog>
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : BTWindow<CDialog>(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CBugTrapTestApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CBugTrapTestApp message handlers

void CBugTrapTestApp::OnTestAccessViolation()
{
	// Throwing un-handled exception
	int* ptr = 0;
	*ptr = 0;
}

void CBugTrapTestApp::OnTestCppException()
{
	// Throwing un-handled exception
#ifdef _WIN64
	if (MessageBox(AfxGetMainWnd()->m_hWnd,
		_T("You are about to allocate 4GB memory block on Win64 system.\r\n")
		_T("Even though your system may not have enough free physical RAM, ")
		_T("Windows may try to find necessary amount of memory in a paging file. ")
		_T("This is lengthy process and it is not recommended to perform this ")
		_T("test on a system with enabled virtual memory.\r\n\r\n")
		_T("Do you really want to simulate out of memory exception?\r\n")
		_T("Did you disable virtual memory?"),
		_T("Simulating Out Of Memory Exception"), MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
	{
		return;
	}
#endif
	CArray<int> array;
	INT_PTR nArraySize = 1000000000;
	for (;;)
	{
		array.SetSize(nArraySize);
		nArraySize += nArraySize;
	}
}
