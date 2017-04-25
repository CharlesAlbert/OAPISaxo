#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include "ATLComTime.h"


// CPollChartInputs dialog
//Class to receive inputs from used for requesting PollChart data

class CPollChartInputs : public CDialogEx
{
	DECLARE_DYNAMIC(CPollChartInputs)

public:
	CPollChartInputs(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPollChartInputs();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POLLCHART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_eUIC;
	CString m_eHorizon;
	CString m_eCount;
	CString m_eMode;
	CString m_eAssetType;
	CString m_strDT;
private:
	COleDateTime m_dtDate;
	COleDateTime m_dtTime;
};
