
// LoginFlowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoginFlow.h"
#include "LoginFlowDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoginFlowDlg dialog



CLoginFlowDlg::CLoginFlowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGINFLOW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginFlowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_cppBrowser);
	DDX_Control(pDX, IDC_EDIT_RESPONSE, m_eResponse);
	DDX_Control(pDX, IDC_COMBO_REQUESTS, m_cbRequests);
	DDX_Control(pDX, IDC_BUTTON_EXECUTE, m_bExecute);
}

BEGIN_MESSAGE_MAP(CLoginFlowDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, &CLoginFlowDlg::OnBnClickedButtonExecute)
	ON_CBN_SELCHANGE(IDC_COMBO_REQUESTS, &CLoginFlowDlg::OnCbnSelchangeComboRequests)
END_MESSAGE_MAP()


// CLoginFlowDlg message handlers

BOOL CLoginFlowDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	//Insert requests
	m_cbRequests.InsertString(0, "GetClientMe");
	m_cbRequests.InsertString(1, "PollChartData");
	m_cbRequests.SetCurSel(0);

	//Navigate to get OAPI response --> Login Page
	m_urlHandler.Navigate(m_cppBrowser);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoginFlowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoginFlowDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoginFlowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CLoginFlowDlg, CDialogEx)
	ON_EVENT(CLoginFlowDlg, IDC_EXPLORER1, 104, CLoginFlowDlg::DownloadCompleteExplorer, VTS_NONE)
END_EVENTSINK_MAP()


void CLoginFlowDlg::DownloadCompleteExplorer()
{
	if (m_urlHandler.StoreAccessToken(m_cppBrowser))
	{
		m_cppBrowser.CloseWindow();
		m_cppBrowser.EnableWindow(false);
		m_cbRequests.ShowWindow(SW_SHOW);
		m_eResponse.ShowWindow(SW_SHOW);
	}
}


void CLoginFlowDlg::OnBnClickedButtonExecute()
{
	int iSelIndex = m_cbRequests.GetCurSel();
	m_bExecute.EnableWindow(false);
	switch (iSelIndex)
	{
	case 0: //GetClientsMe request
		m_eResponse.SetWindowText(m_urlHandler.GetClientsMe().c_str());
		break;
	case 1: //Chart poll request
		if (m_pollChartInput.DoModal() == IDOK)
		{
			m_eResponse.SetWindowText(m_urlHandler.GetPollChartData(static_cast<std::string>(m_pollChartInput.m_eMode),
																    static_cast<std::string>(m_pollChartInput.m_strDT), 
																    static_cast<std::string>(m_pollChartInput.m_eUIC), 
																    static_cast<std::string>(m_pollChartInput.m_eAssetType), 
																    static_cast<std::string>(m_pollChartInput.m_eHorizon), 
																    static_cast<std::string>(m_pollChartInput.m_eCount)).c_str());
		}
		break;
	default:
		break;
	}
	m_bExecute.EnableWindow(true);
}


void CLoginFlowDlg::OnCbnSelchangeComboRequests()
{
	m_eResponse.SetWindowText("");
}
