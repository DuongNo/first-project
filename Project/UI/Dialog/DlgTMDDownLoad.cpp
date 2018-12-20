// DlgTMDDownLoad.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgTMDDownLoad.h"
#include "CheetahApp.h"
#include "afxdialogex.h"
#include "UI\GausGUI\GxMsgBox.h"

// CDlgTMDDownLoad 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTMDDownLoad, CDialogEx)

CDlgTMDDownLoad::CDlgTMDDownLoad(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTMDDownLoad::IDD, pParent)
{
}

CDlgTMDDownLoad::~CDlgTMDDownLoad()
{
}

void CDlgTMDDownLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_TMDDOWNLOAD_LOGLIST, m_TmdListLog);
	//log list 추가. JSJUNG. 20170309
	
	DDX_Control(pDX, IDC_EDT_TMDBOARD, m_TmdListLog);
	
}


BEGIN_MESSAGE_MAP(CDlgTMDDownLoad, CDialogEx)
	ON_MESSAGE(UDMSG_TMDDOWNLOAD_LOG,			OnUpdateLog)		//  설비 상태가 변경되었다.
END_MESSAGE_MAP()



// CDlgTMDDownLoad 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDlgTMDDownLoad, CDialogEx)
	ON_EVENT(CDlgTMDDownLoad, IDC_GXBTN_LOSSTIME_CONFIRM, DISPID_CLICK, CDlgTMDDownLoad::ClickGxbtnLosstimeConfirm, VTS_NONE)
	ON_EVENT(CDlgTMDDownLoad, IDC_GXBTN_LOSSTIME_CANCEL, DISPID_CLICK, CDlgTMDDownLoad::ClickGxbtnLosstimeCancel, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDlgTMDDownLoad::OnInitDialog()									
{
	CDialog::OnInitDialog();

	// Edit 폰트변경
	CFont cFont_Edit;
	// cFont_Edit.CreateFont(15, 15, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("굴림체"));

	cFont_Edit.CreateFont(  24,									// nHeight 
                        0, 										 // nWidth 
                        0, 										 // nEscapement 
                        0, 										 // nOrientation 
                        1, 										  // nWeight 
                        0,											 // bItalic 
                        0, 										 // bUnderline 
                        0, 										 // cStrikeOut 
                        0, 										 // nCharSet 
                        OUT_DEFAULT_PRECIS, 					 // nOutPrecision 
                        0,											 // nClipPrecision 
                        DEFAULT_QUALITY,						 // nQuality
                        DEFAULT_PITCH | FF_DONTCARE,		 // nPitchAndFamily 
                        _T("나눔고딕") );					   // lpszFacename 

	//GetDlgItem(IDC_GXBTN_LOSSTIME_USER)->SetFont( &cFont_Edit );
	m_TmdListLog.SetFont(&cFont_Edit);
	cFont_Edit.Detach();

	::SetWindowPos(this->m_hWnd,NULL, 300,200,600,600,TRUE);
	return 0;
}

void CDlgTMDDownLoad::ClickGxbtnLosstimeConfirm()
{
	// 설비 런중에는 다운로드 못받도록 한다 2017/06/16 HSK.
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("설비가 진행 중입니다.."),	//한
				_T("Machine Status Pause , Please."),		
				_T("Machine Status Pause , Please.") );		

		dlgMsgBox.DoModal();
		return;
	}
	// TMD 다운로드가 끝나고 다시또 다운로드 하는걸 막는다. 2017/06/16 HSK.
	else if(theConfigBank.m_System.m_nJobProcessStep == 9)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("TMD 다운로드가 끝났습니다. 창을 닫아 주십시오."),	//한
				_T("TMD Download Already Finish. Close DLG"),		
				_T("TMD Download Already Finish. Close DLG") );		

		dlgMsgBox.DoModal();
		return;
	}
	else
		theConfigBank.m_System.m_nJobProcessStep=theConfigBank.m_System.eJOB_WaitUserConfirm;				

}


void CDlgTMDDownLoad::ClickGxbtnLosstimeCancel()
{
	// TMD 다운로드가 끝나고 다시또 다운로드 하는걸 막는다. 2017/06/16 HSK.
	CGxMsgBox	dlgMsgBox;
	if(theConfigBank.m_System.m_nJobProcessStep == 9)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("TMD 다운로드가 끝났습니다. 창을 닫아 주십시오."),	//한
				_T("TMD Download Already Finish. Close DLG"),		
				_T("TMD Download Already Finish. Close DLG") );		

		dlgMsgBox.DoModal();
		return;
	}
	else
		ShowWindow(SW_HIDE);							// 화면을 감춘다.	
}

void CDlgTMDDownLoad::LibShowOut2(CString str)
{
	static int s_nLogLines_1 = 0;

	CString strOut;

	CString strTime;

	strOut = str + _T("\r\n");

	if( s_nLogLines_1 >= 10 )
	{
		m_TmdListLog.SetSel(0, -1);
		m_TmdListLog.ReplaceSel(strOut);
		m_TmdListLog.ScrollWindow(0, 2);

		s_nLogLines_1 = 0;
	}
	else
	{
		m_TmdListLog.SetSel(-1, -1);
		m_TmdListLog.ReplaceSel(strOut);
		m_TmdListLog.ScrollWindow(0, 2);
	}

	s_nLogLines_1++;
}

LRESULT CDlgTMDDownLoad::OnUpdateLog(WPARAM wParam, LPARAM lParam)
{
	CString strMsg;

	int nStep = (LPARAM)lParam;

	switch(nStep)
	{
		case 1:
		strMsg = _T("Read Info File");
		break;
		case 2:
		strMsg = _T("Send File Path");
		break;
		case 3:
		strMsg = _T("Send TMD List");
		break;
		case 4:
		strMsg = _T("Route Err");
		break;
		case 5:
		strMsg = _T("Send TMD Copy Start");
		break;
		case 6:
		strMsg = _T("TMD List Err");
		break;
		case 7:
		strMsg = _T("Send TMD CheckSum Start ");
		break;
		case 8:
		strMsg = _T("TMD Copy Start");
		break;
		case 9:
		strMsg = _T("Send Client CheckSum Start ");
		break;
		case 10:
		strMsg = _T(" TMD CheckSum Err ");
		break;
		case 11:
		strMsg = _T("Send TMD FILE Download Start");
		break;
		case 12:
		strMsg = _T("Client CheckSum Err");
		break;
		case 13:
		strMsg = _T("Send TMD Download State");
		::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UDMSG_TMDDOWNLOAD_UPDATE, 0, (LPARAM)0);	//2017-03-08, skcho, 상위에서 명령을 받으면 유저에게 PG 버튼을 깜빡여 알린다.
		theConfigBank.m_System.m_bJobStart = FALSE;  //완료가 되면 버튼이 안눌리게 함.
		break;
		case 14:
		strMsg = _T("TMD FILE Download Err");
		break;
	}

	LibShowOut2(strMsg);

	return TRUE;
}