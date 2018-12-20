// CDlgUserLogin �޽��� ó�����Դϴ�.
// DlgUserLogin.cpp : ���� �����Դϴ�.
//

//- WHLEE Login ������ �۾��� 

#include "stdafx.h"
#include "DlgUserLogin.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "MainFrm.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgUserLogin ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgUserLogin, CDialog)

CDlgUserLogin::CDlgUserLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserLogin::IDD, pParent)
{
	m_bIDInputed = FALSE;
}

CDlgUserLogin::~CDlgUserLogin()
{
}

void CDlgUserLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUserLogin, CDialog)

END_MESSAGE_MAP()


// CDlgUserLogin �޽��� ó�����Դϴ�.

BEGIN_EVENTSINK_MAP(CDlgUserLogin, CDialog)
	ON_EVENT_RANGE(CDlgUserLogin, IDC_GXBTN_A,IDC_GXBTN_NINE, DISPID_CLICK, CDlgUserLogin::ClickGxbtnText, VTS_I4)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_ENTER, DISPID_CLICK, CDlgUserLogin::ClickGxbtnEnter, VTS_NONE)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_CLEAR, DISPID_CLICK, CDlgUserLogin::ClickGxbtnClear, VTS_NONE)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_CANCEL, DISPID_CLICK, CDlgUserLogin::ClickGxbtnCancel, VTS_NONE)
	ON_EVENT(CDlgUserLogin, IDC_GXBTN_LOGINOUT, DISPID_CLICK, CDlgUserLogin::ClickGxbtnLoginout, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgUserLogin::ClickGxbtnText(UINT nID)
{
	CString szInputText;

	CGxButtonEx *btnClicked = (CGxButtonEx*)GetDlgItem(nID);
	szInputText = btnClicked->GetCaption();

	if ( m_bIDInputed == FALSE )
		DispUserID(szInputText.GetBuffer());
	else
		DispUserPw(szInputText.GetBuffer());
}
void CDlgUserLogin::DispUserID(CString strText)
{
	// strText�� �޾� ������ text�� �׸� �ٿ��� lable�� ǥ���Ѵ�.;

	//ID���� �ִ� 8�ڸ� �̻� �Է����� ���ϰ� �Ѵ�.;
	if ( m_strID.GetLength() >= eMAX_UserIDLength )
	{
		return;
	}

	m_strID	=	m_strID + strText;
	//m_pGUICtrl->ApplyStaticString(this, IDC_GXLBL_ID_INPUT, m_strID);

	CGxStaticEx* pLblID = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_ID_INPUT);

	if ( pLblID == NULL )	return;

	pLblID->SetCaption(m_strID.GetBuffer());
}


void CDlgUserLogin::DispUserPw(CString strText)
{	
	if(m_strPw.GetLength() > 11 )
		return;

	// strText�� �޾� ������ text�� �׸� �ٿ��� lable�� ǥ���Ѵ�.;
	m_strPw	=	m_strPw + strText;

	if (m_strPw == _T("") )	// �����ε� Label�� ǥ���� ������ ����;
		return;

	// Password�� *�� ǥ���ϱ�
	int nLen = m_strPw.GetLength();

	CString strDispPw = _T("");
	CString strTextStar = _T("*");

	for(int i=0; i<nLen; i++)
	{
		strDispPw = strDispPw + strTextStar;
	}

	//m_pGUICtrl->ApplyStaticString(this,IDC_GXLBL_PASS_INPUT, strDispPw);	// *�� ǥ���Ѵ�.;

	CGxStaticEx* pLblPW = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_PASS_INPUT);

	if ( pLblPW == NULL )	return;

	pLblPW->SetCaption(strDispPw.GetBuffer());

}


void CDlgUserLogin::ClickGxbtnEnter()
{	
	if ( m_bIDInputed == FALSE )		//	���� ID �Է��� Enter�� ��
	{
		m_bIDInputed = TRUE;
	}
	else							//	PASSWORD �Է��� Enter�� ��
	{	
		CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
		// ���������ϴ°Ÿ� �׳� �ٷ� CIM���� ��û���� [12/1/2016 OSC]
		if(theConfigBank.m_Option.m_bUseLoginRequest)
		{
			theSocketInterFace.SendCmdLoginToDataPC(EQUIP_SET_USER_LOGIN_REQUEST, m_strID, m_strPw);
			CUserAccount user;
			user.sID = m_strID;
			user.sPassword = m_strPw;
			user.nLevel = pMain->getUserType(m_strID, m_strPw);
			ChangeUser(user);
		}
		else
		{
			// PM��� ������ �����۾���(BZone)�� �ϵ���... [12/1/2016 OSC]

			if(pMain->getModePM())
			{
				int iUSerType = pMain->getUserType(m_strID, m_strPw);

				if ( !pMain->FindUser(m_strID) )		{	AfxMessageBox(_T("Check the ID"));	ClearLoginData();	return;		}

				if  (iUSerType < 1 )			{	AfxMessageBox(_T("Check the password")); ClearLoginData();	return;	}		//	��� Ʋ��				
				else if ( iUSerType == 2 || 3)
				{
					///MODIFY NEED
					//--------------------------- �α��� ����		
					// - PM ��� ��������
// 					::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_Main, (LPARAM)0);
// 					pMain->setModePM(FALSE);

					// - PM ��� ��������				


					//ChangeCurUser(m_strID, iUSerType);		// ����� ����
				}
			}
			if (pMain->FindUser(m_strID) == FALSE )	
			{
				AfxMessageBox(_T("Check the ID"));
			}
			else
			{
				CUserAccount user;
				user.sID = m_strID;
				user.sPassword = m_strPw;
				user.nLevel = pMain->getUserType(m_strID, m_strPw);
				if (user.nLevel == 0)
					AfxMessageBox(_T("Check the password"));
				else
					ChangeUser(user);
			}
		}
	}
}

void CDlgUserLogin::ChangeUser( CUserAccount user )
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->SetCurUserData(user);
	if(theConfigBank.m_Option.m_bUseLoginRequest)
		theSocketInterFace.SendCmdLoginToDataPC(EQUIP_SET_INS1_USER_LOGIN);
}

void CDlgUserLogin::ClearLoginData()
{
	m_strID = _T("");
	m_strPw = _T("");
	m_bIDInputed = FALSE;		

	CGxStaticEx* pLblID = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_ID_INPUT);
	CGxStaticEx* pLblPW = (CGxStaticEx*)GetDlgItem(IDC_GXLBL_PASS_INPUT);

	if ( pLblID == NULL || pLblPW == NULL )	return;

	pLblID->SetCaption(_T(""));
	pLblPW->SetCaption(_T(""));
}
void CDlgUserLogin::ClickGxbtnClear()
{
	ClearLoginData();
}

void CDlgUserLogin::ClickGxbtnCancel()
{
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

BOOL CDlgUserLogin::OnInitDialog()									///MODIFY NEED   �ӽû��� Dialog�� �����Ǹ� login������ Load�Ѵ�.
{
	CDialog::OnInitDialog();

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	pMain->LoadUserAccount();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CDlgUserLogin::PreTranslateMessage(MSG* pMsg)						//	[W] Ű������ ���� �ɼǵ� ����
{
	if(pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4)
		return TRUE;
	if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )
		return TRUE;

	return CDialog::PreTranslateMessage(pMsg);
}


void CDlgUserLogin::ClickGxbtnLoginout()
{
	OnOK();
}
