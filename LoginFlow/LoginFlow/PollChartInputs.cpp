// PollChartInputs.cpp : implementation file
//

#include "stdafx.h"
#include "LoginFlow.h"
#include "PollChartInputs.h"
#include "afxdialogex.h"


// CPollChartInputs dialog

IMPLEMENT_DYNAMIC(CPollChartInputs, CDialogEx)

CPollChartInputs::CPollChartInputs(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_POLLCHART, pParent)
	, m_eUIC(_T("21"))
	, m_eHorizon(_T("1"))
	, m_eCount(_T("1200"))
	, m_eMode(_T("FROM"))
	, m_eAssetType(_T("FxSpot"))
	, m_dtDate(COleDateTime::GetCurrentTime())
	, m_dtTime(COleDateTime::GetCurrentTime())
{
}

CPollChartInputs::~CPollChartInputs()
{
}

void CPollChartInputs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_eUIC);
	DDX_Text(pDX, IDC_EDIT4, m_eHorizon);
	DDX_Text(pDX, IDC_EDIT5, m_eCount);
	DDX_Text(pDX, IDC_EDIT1, m_eMode);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dtDate);
	DDX_Text(pDX, IDC_EDIT3, m_eAssetType);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_dtTime);
}


BEGIN_MESSAGE_MAP(CPollChartInputs, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPollChartInputs::OnBnClickedOk)
END_MESSAGE_MAP()


// CPollChartInputs message handlers


void CPollChartInputs::OnBnClickedOk()
{
	UpdateData(true);
	
	//convert date time to dd-mm-yyyyThh:mm:ss format
	m_strDT.Format("%d-%02d-%02dT%02d:%02d:%02dZ",
		m_dtDate.GetYear(), m_dtDate.GetMonth(), m_dtDate.GetDay(),
		m_dtTime.GetHour(), m_dtTime.GetMinute(), m_dtTime.GetSecond());
	
	CDialogEx::OnOK();
}
