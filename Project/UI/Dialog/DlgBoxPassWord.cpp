// DlgBoxKeyBoard.cpp : implementation file
//

#include "stdafx.h"
//#include "CheetahApp.h"
#include "DlgBoxPassWord.h"
#include "MainFrm.h"



// CDlgBoxPassWord dialog

IMPLEMENT_DYNAMIC(CDlgBoxPassWord, CDialog)

CDlgBoxPassWord::CDlgBoxPassWord(CWnd* pParent /*=NULL*/, CString strCaption, CString strValue, BOOL bNotUsePoint, double dMin, double dMax)
	: CDialog(CDlgBoxPassWord::IDD, pParent)
{
	m_strCaption	= strCaption;
	m_strValue		= strValue;
	m_bNotUsePoint	= bNotUsePoint;
	m_dMin			= dMin;
	m_dMax			= dMax;
}

CDlgBoxPassWord::~CDlgBoxPassWord()
{
}

void CDlgBoxPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_NAME,		m_EditNum);
	DDX_Control(pDX, IDC_BTN_NUM_00, 	m_btnNum[0]);
	DDX_Control(pDX, IDC_BTN_NUM_01, 	m_btnNum[1]);
	DDX_Control(pDX, IDC_BTN_NUM_02, 	m_btnNum[2]);
	DDX_Control(pDX, IDC_BTN_NUM_03, 	m_btnNum[3]);
	DDX_Control(pDX, IDC_BTN_NUM_04, 	m_btnNum[4]);
	DDX_Control(pDX, IDC_BTN_NUM_05, 	m_btnNum[5]);
	DDX_Control(pDX, IDC_BTN_NUM_06, 	m_btnNum[6]);
	DDX_Control(pDX, IDC_BTN_NUM_07, 	m_btnNum[7]);
	DDX_Control(pDX, IDC_BTN_NUM_08, 	m_btnNum[8]);
	DDX_Control(pDX, IDC_BTN_NUM_09, 	m_btnNum[9]);
	DDX_Control(pDX, IDC_BTN_NUM_POINT, m_btnPoint);
	DDX_Control(pDX, IDC_BTN_CLEAR,		m_btnClear);
	DDX_Control(pDX, IDC_BTN_CANCEL,	m_btnCancel);
	DDX_Control(pDX, IDC_BTN_ENTER,		m_btnEnter);


}


BEGIN_MESSAGE_MAP(CDlgBoxPassWord, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBoxPassWord::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgBoxPassWord::OnBnClickedCancel)
	
	ON_BN_CLICKED(IDC_BTN_NUM_POINT, &CDlgBoxPassWord::OnBnClkPoint)

	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgBoxPassWord::OnBnClkClear)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgBoxPassWord::OnBnClkCancel)
	ON_BN_CLICKED(IDC_BTN_ENTER, &CDlgBoxPassWord::OnBnClkEnter)

	
	ON_BN_CLICKED(IDC_BTN_NUM_00, &CDlgBoxPassWord::OnBnClickedBtnNum00)
	ON_BN_CLICKED(IDC_BTN_NUM_01, &CDlgBoxPassWord::OnBnClickedBtnNum01)
	ON_BN_CLICKED(IDC_BTN_NUM_02, &CDlgBoxPassWord::OnBnClickedBtnNum02)
	ON_BN_CLICKED(IDC_BTN_NUM_03, &CDlgBoxPassWord::OnBnClickedBtnNum03)
	ON_BN_CLICKED(IDC_BTN_NUM_04, &CDlgBoxPassWord::OnBnClickedBtnNum04)
	ON_BN_CLICKED(IDC_BTN_NUM_05, &CDlgBoxPassWord::OnBnClickedBtnNum05)
	ON_BN_CLICKED(IDC_BTN_NUM_06, &CDlgBoxPassWord::OnBnClickedBtnNum06)
	ON_BN_CLICKED(IDC_BTN_NUM_07, &CDlgBoxPassWord::OnBnClickedBtnNum07)
	ON_BN_CLICKED(IDC_BTN_NUM_08, &CDlgBoxPassWord::OnBnClickedBtnNum08)
	ON_BN_CLICKED(IDC_BTN_NUM_09, &CDlgBoxPassWord::OnBnClickedBtnNum09)

END_MESSAGE_MAP()


// CDlgBoxPassWord message handlers
BOOL CDlgBoxPassWord::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	GetFont()->GetLogFont (&lf);

	lf.lfWeight = FW_BOLD;
	lf.lfHeight = 25;
	m_fontBold.CreateFontIndirect (&lf);

	for(int i=0; i<10;i++)
		m_btnNum[i].SetFont (&m_fontBold);


	m_EditNum.SetFont (&m_fontBold);
	m_btnPoint.SetFont (&m_fontBold);

	m_btnClear.SetFont (&m_fontBold);
	m_btnCancel.SetFont (&m_fontBold);
	m_btnEnter.SetFont (&m_fontBold);


	// TODO:  Add extra initialization here
	SetWindowText(m_strCaption);

	m_btnPoint.EnableWindow(!m_bNotUsePoint);
	m_EditNum.SetWindowText(m_strValue);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgBoxPassWord::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	OnBnClkEnter();
}

void CDlgBoxPassWord::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
	OnBnClkCancel();
}
void CDlgBoxPassWord::BnClkNum(int nNum)
{
	CString strTmp;
	strTmp.Format(_T("%s%d"), m_strValue, nNum);
	m_strValue = strTmp;

	m_EditNum.SetWindowText(m_strValue);
}

void CDlgBoxPassWord::OnBnClkPoint()
{
	CString strTmp;
	strTmp.Format(_T("%s."), m_strValue);
	m_strValue = strTmp;

	m_EditNum.SetWindowText(m_strValue);
}

void CDlgBoxPassWord::OnBnClkClear()
{
	m_strValue = _T("");
	m_EditNum.SetWindowText(m_strValue);
}
void CDlgBoxPassWord::OnBnClkCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CDlgBoxPassWord::OnBnClkEnter()
{
	// TODO: Add your control notification handler code here
	CString strValue;
	
	m_EditNum.GetWindowText(strValue);

	//if(m_strValue=="")
	if(strValue == "")
	{
		
// 		g_pMainFrm->MsgBox(MSG_ETC[CONST_MSG_ETC::W_NO_VAL].strName,_T("경고(警告)"),MB_OK);
		return ;
	}

	if(m_bNotUsePoint && strValue.Find('.')>=0)
	{
// 		g_pMainFrm->MsgBox(MSG_ETC[CONST_MSG_ETC::W_NO_TYPE].strName,_T("경고(警告)"),MB_OK);
		return ;		
	}

	m_strReturnText	=	strValue;

	OnOK();
}




//////////////////////////////////////////////////////////////////////////
// 숫자;
void CDlgBoxPassWord::OnBnClickedBtnNum00()
{
	BnClkNum(0);
}

void CDlgBoxPassWord::OnBnClickedBtnNum01()
{
	BnClkNum(1);
}

void CDlgBoxPassWord::OnBnClickedBtnNum02()
{
	BnClkNum(2);
}

void CDlgBoxPassWord::OnBnClickedBtnNum03()
{
	BnClkNum(3);
}

void CDlgBoxPassWord::OnBnClickedBtnNum04()
{
	BnClkNum(4);
}

void CDlgBoxPassWord::OnBnClickedBtnNum05()
{
	BnClkNum(5);
}

void CDlgBoxPassWord::OnBnClickedBtnNum06()
{
	BnClkNum(6);
}

void CDlgBoxPassWord::OnBnClickedBtnNum07()
{
	BnClkNum(7);	
}

void CDlgBoxPassWord::OnBnClickedBtnNum08()
{
	BnClkNum(8);	
}

void CDlgBoxPassWord::OnBnClickedBtnNum09()
{
	BnClkNum(9);	
}


CString CDlgBoxPassWord::GetValue()
{
	return m_strReturnText;
}
