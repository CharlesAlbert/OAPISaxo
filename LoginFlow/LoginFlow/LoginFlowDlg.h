
// LoginFlowDlg.h : header file
//

#pragma once
#include "explorer.h"
#include "CppTokenResponse.h"
#include "URLHandler.h"
#include "afxwin.h"
#include "PollChartInputs.h"
#include "ChartData.h"

// CLoginFlowDlg dialog
class CLoginFlowDlg : public CDialogEx
{
// Construction
public:
	CLoginFlowDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LoginFlow_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CExplorer m_cppBrowser;
	DECLARE_EVENTSINK_MAP()
	void DownloadCompleteExplorer();

	afx_msg void OnBnClickedButtonExecute();
	afx_msg void OnCbnSelchangeComboRequests();

private:
	void SetColumn(int iIndex, std::string strColName);
private:
	CURLHandler m_urlHandler;

public:
	CComboBox m_cbRequests;
	CEdit m_eResponse;
	CPollChartInputs m_pollChartInput;
	CButton m_bExecute;
	CListCtrl m_chartDataList;
	std::unique_ptr<CChartData> m_chartData;
};
