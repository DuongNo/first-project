// DlgTMDDownLoad.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgTMDDownLoad.h"
#include "CheetahApp.h"
#include "afxdialogex.h"
#include "UI\GausGUI\GxMsgBox.h"

// CDlgTMDDownLoad ��ȭ �����Դϴ�.

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
	//log list �߰�. JSJUNG. 20170309
	
	DDX_Control(pDX, IDC_EDT_TMDBOARD, m_TmdListLog);
	
}


BEGIN_MESSAGE_MAP(CDlgTMDDownLoad, CDialogEx)
	ON_MESSAGE(UDMSG_TMDDOWNLOAD_LOG,			OnUpdateLog)		//  ���� ���°� ����Ǿ���.
END_MESSAGE_MAP()



// CDlgTMDDownLoad �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDlgTMDDownLoad, CDialogEx)
	ON_EVENT(CDlgTMDDownLoad, IDC_GXBTN_LOSSTIME_CONFIRM, DISPID_CLICK, CDlgTMDDownLoad::ClickGxbtnLosstimeConfirm, VTS_NONE)
	ON_EVENT(CDlgTMDDownLoad, IDC_GXBTN_LOSSTIME_CANCEL, DISPID_CLICK, CDlgTMDDownLoad::ClickGxbtnLosstimeCancel, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CDlgTMDDownLoad::OnInitDialog()									
{
	CDialog::OnInitDialog();

	// Edit ��Ʈ����
	CFont cFont_Edit;
	// cFont_Edit.CreateFont(15, 15, 0, 0, FW_HEAVY, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("����ü"));

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
                        _T("�������") );					   // lpszFacename 

	//GetDlgItem(IDC_GXBTN_LOSSTIME_USER)->SetFont( &cFont_Edit );
	m_TmdListLog.SetFont(&cFont_Edit);
	cFont_Edit.Detach();

	::SetWindowPos(this->m_hWnd,NULL, 300,200,600,600,TRUE);
	return 0;
}

void CDlgTMDDownLoad::ClickGxbtnLosstimeConfirm()
{
	// ���� ���߿��� �ٿ�ε� ���޵��� �Ѵ� 2017/06/16 HSK.
	CGxMsgBox	dlgMsgBox;
	if(theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_RUN || theProcBank.GetMachineState() == CONST_MACHINE_STATE::MACHINE_IDLE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
		dlgMsgBox.SetTitle(_T("�˸�"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("���� ���� ���Դϴ�.."),	//��
				_T("Machine Status Pause , Please."),		
				_T("Machine Status Pause , Please.") );		

		dlgMsgBox.DoModal();
		return;
	}
	// TMD �ٿ�ε尡 ������ �ٽö� �ٿ�ε� �ϴ°� ���´�. 2017/06/16 HSK.
	else if(theConfigBank.m_System.m_nJobProcessStep == 9)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
		dlgMsgBox.SetTitle(_T("�˸�"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("TMD �ٿ�ε尡 �������ϴ�. â�� �ݾ� �ֽʽÿ�."),	//��
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
	// TMD �ٿ�ε尡 ������ �ٽö� �ٿ�ε� �ϴ°� ���´�. 2017/06/16 HSK.
	CGxMsgBox	dlgMsgBox;
	if(theConfigBank.m_System.m_nJobProcessStep == 9)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
		dlgMsgBox.SetTitle(_T("�˸�"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
				_T("TMD �ٿ�ε尡 �������ϴ�. â�� �ݾ� �ֽʽÿ�."),	//��
				_T("TMD Download Already Finish. Close DLG"),		
				_T("TMD Download Already Finish. Close DLG") );		

		dlgMsgBox.DoModal();
		return;
	}
	else
		ShowWindow(SW_HIDE);							// ȭ���� �����.	
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
		::PostMessage( ( (CMainFrame*)AfxGetMainWnd() )->m_hWnd, UDMSG_TMDDOWNLOAD_UPDATE, 0, (LPARAM)0);	//2017-03-08, skcho, �������� ����� ������ �������� PG ��ư�� ������ �˸���.
		theConfigBank.m_System.m_bJobStart = FALSE;  //�Ϸᰡ �Ǹ� ��ư�� �ȴ����� ��.
		break;
		case 14:
		strMsg = _T("TMD FILE Download Err");
		break;
	}

	LibShowOut2(strMsg);

	return TRUE;
}