// DlgBoxKeyBoard.cpp : implementation file
//

#include "stdafx.h"
//#include "CheetahApp.h"
#include "DlgBoxKeyBoard.h"

#include "MainFrm.h"



// CDlgBoxKeyBoard dialog

IMPLEMENT_DYNAMIC(CDlgBoxKeyBoard, CDialog)

CDlgBoxKeyBoard::CDlgBoxKeyBoard(CWnd* pParent /*=NULL*/, CString strCaption, CString strValue, BOOL bNotUsePoint, double dMin, double dMax)
	: CDialog(CDlgBoxKeyBoard::IDD, pParent)
{
	m_strCaption	= strCaption;
	m_strValue		= strValue;
	m_bNotUsePoint	= bNotUsePoint;
	m_dMin			= dMin;
	m_dMax			= dMax;
}

CDlgBoxKeyBoard::~CDlgBoxKeyBoard()
{
}

void CDlgBoxKeyBoard::DoDataExchange(CDataExchange* pDX)
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
	DDX_Control(pDX, IDC_BTN_NUM_MINUS, m_btnMinus);
	DDX_Control(pDX, IDC_BTN_CLEAR,		m_btnClear);
	DDX_Control(pDX, IDC_BTN_CANCEL,	m_btnCancel);
	DDX_Control(pDX, IDC_BTN_ENTER,		m_btnEnter);

	DDX_Control(pDX, IDC_BTN_STRING_A, m_btnChar[0]);	// A
	DDX_Control(pDX, IDC_BTN_STRING_B, m_btnChar[1]);	// B
	DDX_Control(pDX, IDC_BTN_STRING_C, m_btnChar[2]);	// C
	DDX_Control(pDX, IDC_BTN_STRING_D, m_btnChar[3]);	// D
	DDX_Control(pDX, IDC_BTN_STRING_E, m_btnChar[4]);	// E
	DDX_Control(pDX, IDC_BTN_STRING_F, m_btnChar[5]);	// F
	DDX_Control(pDX, IDC_BTN_STRING_G, m_btnChar[6]);	// G
	DDX_Control(pDX, IDC_BTN_STRING_H, m_btnChar[7]);	// H
	DDX_Control(pDX, IDC_BTN_STRING_I, m_btnChar[8]);	// I
	DDX_Control(pDX, IDC_BTN_STRING_J, m_btnChar[9]);	// J
	DDX_Control(pDX, IDC_BTN_STRING_K, m_btnChar[10]);	// K
	DDX_Control(pDX, IDC_BTN_STRING_L, m_btnChar[11]);	// L
	DDX_Control(pDX, IDC_BTN_STRING_M, m_btnChar[12]);	// M
	DDX_Control(pDX, IDC_BTN_STRING_N, m_btnChar[13]);	// N
	DDX_Control(pDX, IDC_BTN_STRING_O, m_btnChar[14]);	// O
	DDX_Control(pDX, IDC_BTN_STRING_P, m_btnChar[15]);	// P
	DDX_Control(pDX, IDC_BTN_STRING_Q, m_btnChar[16]);	// Q
	DDX_Control(pDX, IDC_BTN_STRING_R, m_btnChar[17]);	// R
	DDX_Control(pDX, IDC_BTN_STRING_S, m_btnChar[18]);	// S
	DDX_Control(pDX, IDC_BTN_STRING_T, m_btnChar[19]);	// T
	DDX_Control(pDX, IDC_BTN_STRING_U, m_btnChar[20]);	// U
	DDX_Control(pDX, IDC_BTN_STRING_V, m_btnChar[21]);	// V
	DDX_Control(pDX, IDC_BTN_STRING_W, m_btnChar[22]);	// W
	DDX_Control(pDX, IDC_BTN_STRING_X, m_btnChar[23]);	// X
	DDX_Control(pDX, IDC_BTN_STRING_Y, m_btnChar[24]);	// Y
	DDX_Control(pDX, IDC_BTN_STRING_Z, m_btnChar[25]);	// Y
	DDX_Control(pDX, IDC_BTN_STRING_UNDER_BAR, m_btnUnderBar);	// UNDER BAR
}


BEGIN_MESSAGE_MAP(CDlgBoxKeyBoard, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBoxKeyBoard::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgBoxKeyBoard::OnBnClickedCancel)
	
	ON_BN_CLICKED(IDC_BTN_NUM_POINT, &CDlgBoxKeyBoard::OnBnClkPoint)
	ON_BN_CLICKED(IDC_BTN_NUM_MINUS, &CDlgBoxKeyBoard::OnBnClkMinus)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlgBoxKeyBoard::OnBnClkClear)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgBoxKeyBoard::OnBnClkCancel)
	ON_BN_CLICKED(IDC_BTN_ENTER, &CDlgBoxKeyBoard::OnBnClkEnter)
	ON_BN_CLICKED(IDC_BTN_STRING_UNDER_BAR, &CDlgBoxKeyBoard::OnBnClkUnderBar)
	
	ON_BN_CLICKED(IDC_BTN_NUM_00, &CDlgBoxKeyBoard::OnBnClickedBtnNum00)
	ON_BN_CLICKED(IDC_BTN_NUM_01, &CDlgBoxKeyBoard::OnBnClickedBtnNum01)
	ON_BN_CLICKED(IDC_BTN_NUM_02, &CDlgBoxKeyBoard::OnBnClickedBtnNum02)
	ON_BN_CLICKED(IDC_BTN_NUM_03, &CDlgBoxKeyBoard::OnBnClickedBtnNum03)
	ON_BN_CLICKED(IDC_BTN_NUM_04, &CDlgBoxKeyBoard::OnBnClickedBtnNum04)
	ON_BN_CLICKED(IDC_BTN_NUM_05, &CDlgBoxKeyBoard::OnBnClickedBtnNum05)
	ON_BN_CLICKED(IDC_BTN_NUM_06, &CDlgBoxKeyBoard::OnBnClickedBtnNum06)
	ON_BN_CLICKED(IDC_BTN_NUM_07, &CDlgBoxKeyBoard::OnBnClickedBtnNum07)
	ON_BN_CLICKED(IDC_BTN_NUM_08, &CDlgBoxKeyBoard::OnBnClickedBtnNum08)
	ON_BN_CLICKED(IDC_BTN_NUM_09, &CDlgBoxKeyBoard::OnBnClickedBtnNum09)
	ON_BN_CLICKED(IDC_BTN_STRING_A, &CDlgBoxKeyBoard::OnBnClickedBtnStringA)
	ON_BN_CLICKED(IDC_BTN_STRING_B, &CDlgBoxKeyBoard::OnBnClickedBtnStringB)
	ON_BN_CLICKED(IDC_BTN_STRING_C, &CDlgBoxKeyBoard::OnBnClickedBtnStringC)
	ON_BN_CLICKED(IDC_BTN_STRING_D, &CDlgBoxKeyBoard::OnBnClickedBtnStringD)
	ON_BN_CLICKED(IDC_BTN_STRING_E, &CDlgBoxKeyBoard::OnBnClickedBtnStringE)
	ON_BN_CLICKED(IDC_BTN_STRING_F, &CDlgBoxKeyBoard::OnBnClickedBtnStringF)
	ON_BN_CLICKED(IDC_BTN_STRING_G, &CDlgBoxKeyBoard::OnBnClickedBtnStringG)
	ON_BN_CLICKED(IDC_BTN_STRING_H, &CDlgBoxKeyBoard::OnBnClickedBtnStringH)
	ON_BN_CLICKED(IDC_BTN_STRING_I, &CDlgBoxKeyBoard::OnBnClickedBtnStringI)
	ON_BN_CLICKED(IDC_BTN_STRING_J, &CDlgBoxKeyBoard::OnBnClickedBtnStringJ)
	ON_BN_CLICKED(IDC_BTN_STRING_K, &CDlgBoxKeyBoard::OnBnClickedBtnStringK)
	ON_BN_CLICKED(IDC_BTN_STRING_L, &CDlgBoxKeyBoard::OnBnClickedBtnStringL)
	ON_BN_CLICKED(IDC_BTN_STRING_M, &CDlgBoxKeyBoard::OnBnClickedBtnStringM)
	ON_BN_CLICKED(IDC_BTN_STRING_N, &CDlgBoxKeyBoard::OnBnClickedBtnStringN)
	ON_BN_CLICKED(IDC_BTN_STRING_O, &CDlgBoxKeyBoard::OnBnClickedBtnStringO)
	ON_BN_CLICKED(IDC_BTN_STRING_P, &CDlgBoxKeyBoard::OnBnClickedBtnStringP)
	ON_BN_CLICKED(IDC_BTN_STRING_Q, &CDlgBoxKeyBoard::OnBnClickedBtnStringQ)
	ON_BN_CLICKED(IDC_BTN_STRING_R, &CDlgBoxKeyBoard::OnBnClickedBtnStringR)
	ON_BN_CLICKED(IDC_BTN_STRING_S, &CDlgBoxKeyBoard::OnBnClickedBtnStringS)
	ON_BN_CLICKED(IDC_BTN_STRING_T, &CDlgBoxKeyBoard::OnBnClickedBtnStringT)
	ON_BN_CLICKED(IDC_BTN_STRING_U, &CDlgBoxKeyBoard::OnBnClickedBtnStringU)
	ON_BN_CLICKED(IDC_BTN_STRING_V, &CDlgBoxKeyBoard::OnBnClickedBtnStringV)
	ON_BN_CLICKED(IDC_BTN_STRING_W, &CDlgBoxKeyBoard::OnBnClickedBtnStringW)
	ON_BN_CLICKED(IDC_BTN_STRING_X, &CDlgBoxKeyBoard::OnBnClickedBtnStringX)
	ON_BN_CLICKED(IDC_BTN_STRING_Y, &CDlgBoxKeyBoard::OnBnClickedBtnStringY)
	ON_BN_CLICKED(IDC_BTN_STRING_Z, &CDlgBoxKeyBoard::OnBnClickedBtnStringZ)
END_MESSAGE_MAP()


// CDlgBoxKeyBoard message handlers
BOOL CDlgBoxKeyBoard::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lf;
	GetFont()->GetLogFont (&lf);

	lf.lfWeight = FW_BOLD;
	lf.lfHeight = 25;
	m_fontBold.CreateFontIndirect (&lf);

	for(int i=0; i<10;i++)
		m_btnNum[i].SetFont (&m_fontBold);
	for(int i=0; i<26;i++)
		m_btnChar[i].SetFont (&m_fontBold);

	m_EditNum.SetFont (&m_fontBold);
	m_btnPoint.SetFont (&m_fontBold);
	m_btnMinus.SetFont (&m_fontBold);
	m_btnClear.SetFont (&m_fontBold);
	m_btnCancel.SetFont (&m_fontBold);
	m_btnEnter.SetFont (&m_fontBold);
	m_btnUnderBar.SetFont (&m_fontBold);

	// TODO:  Add extra initialization here
	SetWindowText(m_strCaption);

	m_btnPoint.EnableWindow(!m_bNotUsePoint);
	m_EditNum.SetWindowText(m_strValue);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CDlgBoxKeyBoard::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
	OnBnClkEnter();
}

void CDlgBoxKeyBoard::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
	OnBnClkCancel();
}
void CDlgBoxKeyBoard::BnClkNum(int nNum)
{
	CString strTmp;
	strTmp.Format(_T("%s%d"), m_strValue, nNum);
	m_strValue = strTmp;

	m_EditNum.SetWindowText(m_strValue);
}
void CDlgBoxKeyBoard::BnClkChar(CString strChar)
{
	CString strTmp;
	strTmp.Format(_T("%s%s"), m_strValue, strChar);
	m_strValue = strTmp;

	m_EditNum.SetWindowText(m_strValue);
}
void CDlgBoxKeyBoard::OnBnClkPoint()
{
	CString strTmp;
	strTmp.Format(_T("%s."), m_strValue);
	m_strValue = strTmp;

	m_EditNum.SetWindowText(m_strValue);
}
void CDlgBoxKeyBoard::OnBnClkMinus()
{
	CString strTmp;
	strTmp.Format(_T("%s-"), m_strValue);
	m_strValue = strTmp;

	m_EditNum.SetWindowText(m_strValue);
}
void CDlgBoxKeyBoard::OnBnClkClear()
{
	m_strValue = _T("");
	m_EditNum.SetWindowText(m_strValue);
}
void CDlgBoxKeyBoard::OnBnClkCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CDlgBoxKeyBoard::OnBnClkEnter()
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


void CDlgBoxKeyBoard::OnBnClkUnderBar()
{
	CString strTmp;
	strTmp.Format(_T("%s_"), m_strValue);
	m_strValue = strTmp;

	m_EditNum.SetWindowText(m_strValue);
}


//////////////////////////////////////////////////////////////////////////
// 숫자;
void CDlgBoxKeyBoard::OnBnClickedBtnNum00()
{
	BnClkNum(0);
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum01()
{
	BnClkNum(1);
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum02()
{
	BnClkNum(2);
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum03()
{
	BnClkNum(3);
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum04()
{
	BnClkNum(4);
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum05()
{
	BnClkNum(5);
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum06()
{
	BnClkNum(6);
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum07()
{
	BnClkNum(7);	
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum08()
{
	BnClkNum(8);	
}

void CDlgBoxKeyBoard::OnBnClickedBtnNum09()
{
	BnClkNum(9);	
}


//////////////////////////////////////////////////////////////////////////
// 문자;

void CDlgBoxKeyBoard::OnBnClickedBtnStringA()
{
	BnClkChar(_T("A"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringB()
{
	BnClkChar(_T("B"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringC()
{
	BnClkChar(_T("C"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringD()
{
	BnClkChar(_T("D"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringE()
{
	BnClkChar(_T("E"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringF()
{
	BnClkChar(_T("F"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringG()
{
	BnClkChar(_T("G"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringH()
{
	BnClkChar(_T("H"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringI()
{
	BnClkChar(_T("I"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringJ()
{
	BnClkChar(_T("J"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringK()
{
	BnClkChar(_T("K"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringL()
{
	BnClkChar(_T("L"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringM()
{
	BnClkChar(_T("M"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringN()
{
	BnClkChar(_T("N"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringO()
{
	BnClkChar(_T("O"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringP()
{
	BnClkChar(_T("P"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringQ()
{
	BnClkChar(_T("Q"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringR()
{
	BnClkChar(_T("R"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringS()
{
	BnClkChar(_T("S"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringT()
{
	BnClkChar(_T("T"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringU()
{
	BnClkChar(_T("U"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringV()
{
	BnClkChar(_T("V"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringW()
{
	BnClkChar(_T("W"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringX()
{
	BnClkChar(_T("X"));	
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringY()
{
	BnClkChar(_T("Y"));
}

void CDlgBoxKeyBoard::OnBnClickedBtnStringZ()
{
	BnClkChar(_T("Z"));
}

CString CDlgBoxKeyBoard::GetValue()
{
	return m_strReturnText;
}
