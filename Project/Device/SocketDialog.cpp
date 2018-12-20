// SocketDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SocketDialog.h"
#include "afxdialogex.h"
//kjpark 20161021 Setup ��ȣ�� UI ����
#include "UI\Form\FormSignal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CSocketDialog, CDialog)

CSocketDialog::CSocketDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSocketDialog::IDD, pParent)
{
}

CSocketDialog::~CSocketDialog()
{	
}

void CSocketDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSocketDialog, CDialog)
	ON_MESSAGE(WM_PACKET_RECEIVE, OnPacketReceive)

	ON_MESSAGE(WM_RE_CONNECT, OnReConnect)
	//kjpark 20160919 Topdlg�� connect status 
	ON_MESSAGE(WM_SOCKET_CONNECT, OnSocketConnect)
	ON_MESSAGE(WM_SOCKET_CLOSE, OnSocketClose)
	// View ȭ���� �ʱ�ȭ�ϰ� ����� Ȱ��ȭ / ��Ȱ��ȭ �Ѵ�.
	
	ON_WM_TIMER()
END_MESSAGE_MAP()

LRESULT CSocketDialog::OnReConnect( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;
	BOOL bCreate = (BOOL)lParam;

	//?  ���� 1ȸ�� �����ϴ� ������ �°� �ڵ� ��õ� �ּ� [7/1/2014 OSC]

 	if ( bCreate )
 	{	
		if (pSocket->GetSocketName() == SOCKET_LOWER_UVMCR)
 			CreateUVMcrSocket_Lower();

		if (pSocket->GetSocketName() == SOCKET_UPPER_UVMCR)
 			CreateUVMcrSocket_Upper();

		if (pSocket->GetSocketName() == SOCKET_DATAPC)
 			CreateDataSocket();

		if (pSocket->GetSocketName() == SOCKET_ACTIVE1)
 			CreateVInspSocket(VI_A1);

		if (pSocket->GetSocketName() == SOCKET_ACTIVE2)
 			CreateVInspSocket(VI_A2);

		if (pSocket->GetSocketName() == SOCKET_MITALIGN)
 			CreateMITAlignSocket();
	}
 
	return 0;
}

// ������ �õ� Ÿ�̸�
//kjpark 20160919 Topdlg�� connect status 
void CSocketDialog::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);

	if(nIDEvent == TIMER_RE_CONNECT)
	{
		//kjpark 20161006 MCR ���� ����
		if(m_pSocket_Lower_UVMCR.IsConnected() == FALSE && m_pSocket_Lower_UVMCR != NULL)
		{
			m_pSocket_Lower_UVMCR.ConnectToServer();
		}
		//kjpark 20161006 MCR ���� ����
		if(m_pSocket_Upper_UVMCR.IsConnected() == FALSE && m_pSocket_Upper_UVMCR != NULL)
		{
			m_pSocket_Upper_UVMCR.ConnectToServer();
		}
		if(m_pSocket_DataPC.IsConnected() == FALSE && m_pSocket_DataPC != NULL)
		{
			m_pSocket_DataPC.ConnectToServer();
		}
		if(m_pSocket_VisionInspect[VI_A1].IsConnected() == FALSE && m_pSocket_VisionInspect[VI_A1] != NULL)
		{
			m_pSocket_VisionInspect[VI_A1].ConnectToServer();
		}
		if(m_pSocket_VisionInspect[VI_A2].IsConnected() == FALSE && m_pSocket_VisionInspect[VI_A2] != NULL)
		{
			m_pSocket_VisionInspect[VI_A2].ConnectToServer();
		}
		if(m_pSocket_MITAlign.IsConnected() == FALSE && m_pSocket_MITAlign!=NULL)
		{			
			m_pSocket_MITAlign.ConnectToServer();
		}
		if(m_pSocket_DCR.IsConnected() == FALSE && m_pSocket_DCR != NULL)
		{
			m_pSocket_DCR.ConnectToServer();
		}
		SetTimer(TIMER_RE_CONNECT,1000,NULL);

	}
	CDialog::OnTimer(nIDEvent);
}

void CSocketDialog::DeleteAllSocket()
{
	//��� ������ �ݴ´�. 

}

BOOL CSocketDialog::GetCheckComConnected(int idx)
{
	//������ TCP/IP������¸� ��ȯ�Ѵ�.
	return FALSE;

}

void CSocketDialog::SocketClosed(CDataSocket *pSocket, CClientSocket* pClientSocket)
{
	//������ ������ ��� ó��(Message print)

}

USHORT CSocketDialog::GetCRC16_Checksum( const void *buf, int len )
{
	char *cbuf = (char*)buf;

	register int counter;
	register unsigned short crc = 0;
	for( counter = 0; counter < len; counter++)
	{
		crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *cbuf++)&0x00FF];
	}
	return crc;
}


LRESULT CSocketDialog::OnPacketReceive( WPARAM wParam, LPARAM lParam )
{
	int nRtn = 0;
	CDataSocket* pSocket = (CDataSocket *)wParam;
	int nMsgLen = pSocket->GetLastLength();

	if(pSocket->GetSocketName() == SOCKET_ACTIVE1 || pSocket->GetSocketName() == SOCKET_ACTIVE2)
	{
		SetVisionInspectDataProcessing(pSocket);
	}
	else if(pSocket->GetSocketName() == SOCKET_PGPC)
	{
		SetPatternGeneratorDataProcessing(pSocket);
	}
	//kjpark 20160912 UVMCR �Ľ�
	else if (pSocket->GetSocketName() == SOCKET_LOWER_UVMCR || pSocket->GetSocketName() == SOCKET_UPPER_UVMCR)
	{
		SetUVMCRDataProcessing(pSocket);		
	}
	else if ( (pSocket->GetSocketName() == SOCKET_MITALIGN))
	{
		SetMITAlignDataProcessing(pSocket);		
	}
	else if ( (pSocket->GetSocketName() == SOCKET_MITEDGEALIGN))
	{
		SetMITEdgeAlignDataProcessing(pSocket);		
	}
	else if (pSocket->GetSocketName() == SOCKET_ROBOTALIGN)
	{
		SetRobotAlignDataProcessing(pSocket);		
	}
	else if (pSocket->GetSocketName() == SOCKET_LD_ROBOT || pSocket->GetSocketName() == SOCKET_UD_ROBOT)
	{
		SetRobotDataProcessing(pSocket);		
	}
	else if(pSocket->GetSocketName() == SOCKET_DATAPC)
	{
		SetDataPCProcessing(pSocket);
	}
	else if(pSocket->GetSocketName() == SOCKET_DCR)
	{
		SetTrayReaderDataProcessing(pSocket);
	}
	else if ( (pSocket->GetSocketName() == SOCKET_TRAY_ALIGN))
	{
		SetTrayAlignDataProcessing(pSocket);		
	}
	return nRtn;
}

//kjpark 20160913 �˻�� ���� �޼��� �Ľ�
// ���� �����͸� û���Ѵ�.
CString CSocketDialog::PacketClear( CString strSocketName, CString strMsg )
{
	// 2�� �̻� ���޾� �پ� ���� ���� ����� �� �� �ϳ��� �����.
	int nNextIndex = 0;

	if( strSocketName == SOCKET_ACTIVE1 || strSocketName == SOCKET_ACTIVE2)
	{
		nNextIndex = strMsg.Find(_T("FDB"), 3);
	}
	else if( strSocketName == SOCKET_PGPC || strSocketName == SOCKET_MITEDGEALIGN)
	{
		nNextIndex = strMsg.Find(STX, 1);
	}
	else if(strSocketName == SOCKET_ROBOTALIGN)
	{
		// ���۹��ڰ� ��� ���Ṯ�ڷ� ã�´�
		int nFirstIndex = strMsg.Find(CR);
		int nLastIndex = strMsg.ReverseFind(CR);
		if(nFirstIndex == nLastIndex)
			nNextIndex = -1;
		else
			nNextIndex = nFirstIndex + 1;
	}

	if(nNextIndex == -1)
		strMsg = _T("");
	else if(nNextIndex > 0)
		strMsg.Delete(0, nNextIndex);

	return strMsg;
}

// ������ �̸��� ������ Index�� ��´�, Main Frame�� ���� ���� ǥ�ÿ�

int CSocketDialog::GetSocketIndex(CString sName)
{
	int nIdx = 99;

	//kjpark 20161006 MCR ���� ����
	if ( sName == SOCKET_LOWER_UVMCR )		// _T("UV_1")
		nIdx = eCOMM_Lower_UVMCR;
	else if ( sName == SOCKET_UPPER_UVMCR )		// _T("UV_1")
		nIdx = eCOMM_Upper_UVMCR;
	else if ( sName == SOCKET_PGPC )		// _T("PGPC")
		nIdx = eCOMM_SignalPC;
	else if ( sName == SOCKET_DATAPC )	//	_T("DATAPC")	//SONIC
		nIdx = eCOMM_DataPC;
	else if ( sName ==  SOCKET_ACTIVE1 )			
		nIdx = eCOMM_AVCamera1;
	else if ( sName ==  SOCKET_ACTIVE2 )			
		nIdx = eCOMM_AVCamera2;
	else if ( sName == SOCKET_MITALIGN )	
		nIdx = eCOMM_MITAlign;
	else if ( sName == SOCKET_ROBOTALIGN )	
		nIdx = eCOMM_RobotAlign;
	else if ( sName == SOCKET_LD_ROBOT )	
		nIdx = eCOMM_LdRobot;
	else if ( sName == SOCKET_UD_ROBOT )	
		nIdx = eCOMM_UdRobot;
	else if ( sName == SOCKET_DCR )	
		nIdx = eCOMM_DCR;
	else if ( sName == SOCKET_TRAY_ALIGN )	
		nIdx = eCOMM_Tray_Align;

	else if ( sName == SOCKET_MITEDGEALIGN )	
		nIdx = eCOMM_MITEdgeAlign;


	return nIdx;
}


// ���� TCP/IP ����� ���� ���¸� ��ȸ�Ѵ�.
//kjpark 20160919 Topdlg�� connect status 
BOOL CSocketDialog::GetCommConnected(int nIdx)
{
	BOOL bResult = FALSE;

	switch (nIdx)
	{
	case eCOMM_DataPC :		// ������ PC ���� ���
		if ( m_pSocket_DataPC != NULL )
		{
			bResult = m_pSocket_DataPC.IsConnected();
		}
		break;
	case eCOMM_Lower_UVMCR :		// UVMCR #1���� ���
		if ( m_pSocket_Lower_UVMCR != NULL )
		{
//			bResult = m_pSocket_Lower_UVMCR.IsConnected();
			//������ ���ܵ� -1�� ��ȯ�ϴµ� �ֱ׷����𸣰ڴ�....
			bResult = GetMainHandler()->m_pIDReader[LOWER_MCR]->IsConnect();
		}
		break;
	case eCOMM_Upper_UVMCR:
		if ( m_pSocket_Upper_UVMCR != NULL )
		{
//			bResult = m_pSocket_Upper_UVMCR.IsConnected();
			//������ ���ܵ� -1�� ��ȯ�ϴµ� �ֱ׷����𸣰ڴ�....
			bResult = GetMainHandler()->m_pIDReader[UPPER_MCR]->IsConnect();
		}
		break;
	case eCOMM_AVCamera1	:	// AVCamera PC 1 ���� ���
		if ( m_pSocket_VisionInspect[VI_A1] != NULL )
		{
			bResult = m_pSocket_VisionInspect[VI_A1].IsConnected();
		}
		break;
	case eCOMM_AVCamera2	:	// AVCamera PC 2���� ���
		if ( m_pSocket_VisionInspect[VI_A2] != NULL )
		{
			bResult = m_pSocket_VisionInspect[VI_A2].IsConnected();
		}
		break;
	case eCOMM_MITAlign :		// MIT Align�� ���.
		if ( m_pSocket_MITAlign != NULL )
		{
			bResult = m_pSocket_MITAlign.IsConnected();
		}
		break;
	case eCOMM_DCR :	
		if ( m_pSocket_DCR != NULL )
		{
			bResult = m_pSocket_DCR.IsConnected();
		}
		break;
	}

	return bResult;			// ���ӿ��� ȸ��
}

//kjpark 20160919 Topdlg�� connect status 
LRESULT CSocketDialog::OnSocketConnect( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;

	if(pSocket->IsConnected())
	{
		//!		GetLogiCtrl()->AddBuf(LOG_COMM, _T("[%s][IP: %s , %d] ���� ����."), pSocket->GetSocketName(), pSocket->GetIP(), pSocket->GetPort());
		CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
		::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pSocket->GetSocketName()), 1);		// ���� �̺�Ʈ

		if(pSocket->GetSocketName() == SOCKET_LOWER_UVMCR)
			GetMainHandler()->m_pIDReader[LOWER_MCR]->Connect();
		if (pSocket->GetSocketName() == SOCKET_UPPER_UVMCR)
			GetMainHandler()->m_pIDReader[UPPER_MCR]->Connect();
		if(pSocket->GetSocketName() == SOCKET_DATAPC)
		{
			// ����� �� ���� ���� ���� ����ȭ [11/28/2016 OSC]
			SendCmdToDataPC(EQUIP_SET_CONTROL);
			SendCmdToDataPC(EQUIP_SET_DATAPC_VERSION);

			//���� �� �� ���� Ʈ���� ���� üũ �ؼ� ���� . ó�� ���� �� ���... 2017/06/12 HSK.
			if(theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT,TRAY_INFO_POS_GOOD_IN_ALIGN) == FALSE
			&& theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_LIFT) == FALSE
			&& theUnitFunc.TrayData_Exist(GOOD_IN_TRAY_PORT, TRAY_INFO_POS_GOOD_IN_BUFFER) == FALSE
			&& theProcBank.m_bGoodTrayReq == FALSE)
			{
				//2018-05-16,MGYUN, AGV ȿ�� ���� ������ �߰�
				//theUnitFunc.WaitDelayTime(2000);
				theUnitFunc.CIM_TRAYProcess(_T("350"),_T("1"));
				theProcBank.m_bGoodTrayReq = TRUE;
			}
			//if(theUnitFunc.TrayData_Exist(GOOD_OUT_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_BUFFER) == FALSE
			//	&& theUnitFunc.TrayData_Exist(GOOD_OUT_TRAY_PORT,TRAY_INFO_POS_GOOD_OUT_LIFT) == FALSE)
			//{
			//	theUnitFunc.CIM_TRAYProcess(_T("353"),_T("2"));
			//}

			if(GetMainHandler()->GetCurUserData().sID.GetLength() > 0)
				SendCmdLoginToDataPC(EQUIP_SET_INS1_USER_LOGIN, GetMainHandler()->GetCurUserData().sID);
			else
				SendCmdLoginToDataPC(EQUIP_SET_INS1_USER_LOGOUT);
		}
	}

	return 0;
}

LRESULT CSocketDialog::OnSocketClose( WPARAM wParam, LPARAM lParam )
{
	CDataSocket* pSocket = (CDataSocket *)wParam;
	//������ ���� ���� Ŭ���̾�Ʈ �� ��� �߰�.17/04/24
	if(pSocket->IsServer())
	{
		CClientSocket* pClientSocket = (CClientSocket *)lParam;
		if(pClientSocket->IsConnected() == FALSE)
		{
			CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
			::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pClientSocket->GetSocketName()), 0);		// ���� �̺�Ʈ
		}
	}
	else
	{
		if(pSocket->IsConnected() == FALSE)
		{
			CMainFrame* pMainFrm = (CMainFrame*)::AfxGetMainWnd();
			::PostMessage(pMainFrm->m_hWnd, UDMSG_COMM_STATE, GetSocketIndex(pSocket->GetSocketName()), 0);		// ���� �̺�Ʈ
		}
	}
	return 0;
}

void CSocketDialog::setSocketConnect()
{
	CreateVInspSocket(VI_A1);
	CreateVInspSocket(VI_A2);
	CreateUVMcrSocket_Lower();
	CreateUVMcrSocket_Upper();
	CreateDCRSocket();
	CreateMITAlignSocket();
	CreateDataSocket();

	CreatePatternGeneratorSocket();
	CreateMITEdgeAlignSocket();
	CreateRobotAlignSocket();
	CreateRobotSocket(E_LD_ROBOT);
	CreateRobotSocket(E_UD_ROBOT);
	CreateTrayAlignSocket();

	SetTimer(TIMER_RE_CONNECT,1000,NULL);
}

//kjpark 20161006 MCR ���� ����
void CSocketDialog::CreateUVMcrSocket_Lower()
{

	if(m_pSocket_Lower_UVMCR)
	{
		m_pSocket_Lower_UVMCR.Close();
	}

	m_pSocket_Lower_UVMCR.SetServer(FALSE);
	if(m_pSocket_Lower_UVMCR.Create())
	{
		m_pSocket_Lower_UVMCR.SetReciveHwnd(this->m_hWnd); // ���� Dlg �ּ� �Է�
	}

	int nPort = 23;

	m_pSocket_Lower_UVMCR.SetSocketName(SOCKET_LOWER_UVMCR);
	m_pSocket_Lower_UVMCR.SetIP(LOWER_MCR_CAMERA_IP);
	m_pSocket_Lower_UVMCR.SetPort(nPort);
	ConnectUvSocket_Lower();
}

//kjpark 20161006 MCR ���� ����
BOOL CSocketDialog::ConnectUvSocket_Lower()
{
	BOOL bRet = FALSE;
	if(m_pSocket_Lower_UVMCR.ConnectToServer() == TRUE)
		bRet = TRUE;

	return bRet;
}

//kjpark 20161006 MCR ���� ����
void CSocketDialog::CreateUVMcrSocket_Upper()
{
	if(m_pSocket_Upper_UVMCR)
	{
		m_pSocket_Upper_UVMCR.Close();
	}

	m_pSocket_Upper_UVMCR.SetServer(FALSE);
	if(m_pSocket_Upper_UVMCR.Create())
	{
		m_pSocket_Upper_UVMCR.SetReciveHwnd(this->m_hWnd); // ���� Dlg �ּ� �Է�
	}

	int nPort = 23;		

	m_pSocket_Upper_UVMCR.SetSocketName(SOCKET_UPPER_UVMCR);
	m_pSocket_Upper_UVMCR.SetIP(UPPER_MCR_CAMERA_IP);
	m_pSocket_Upper_UVMCR.SetPort(nPort);
	ConnectUvSocket_Upper();
}

BOOL CSocketDialog::ConnectUvSocket_Upper()
{
	BOOL bRet = FALSE;
	if(m_pSocket_Upper_UVMCR.ConnectToServer() == TRUE)
		bRet = TRUE;
		
	return bRet;
}

void CSocketDialog::CreatePatternGeneratorSocket()
{
	
	if(m_pSocket_PatternGenerator)
	{
		m_pSocket_PatternGenerator.Close();
	}

	m_pSocket_PatternGenerator.SetServer(TRUE);				// ������ �����Ѵ�.

	CString strIP;
	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 140 );
	int nPort = 3000;

	m_pSocket_PatternGenerator.SetPort(nPort);

	ClientSocketName socketName;
	socketName.Name = SOCKET_PGPC;
	socketName.IP = strIP;
	socketName.Port = nPort;

	m_pSocket_PatternGenerator.AddClientName( socketName );
	m_pSocket_PatternGenerator.SetReciveHwnd(this->m_hWnd);

	if(m_pSocket_PatternGenerator.Create(m_pSocket_PatternGenerator.GetPort()))
	{
		m_pSocket_PatternGenerator.Listen();											// ������ �����Ѵ�.
	}


}

void CSocketDialog::CreateVInspSocket(const VI_ACTIVE active )
{

	if(m_pSocket_VisionInspect[active])
	{
		m_pSocket_VisionInspect[active].Close();
	}

	m_pSocket_VisionInspect[active].SetServer(FALSE);

	if(m_pSocket_VisionInspect[active].Create())
	{
		m_pSocket_VisionInspect[active].SetReciveHwnd(this->m_hWnd); // ���� Dlg �ּ� �Է�

	}

	CString strIP;
	int nPort;

	if (active == VI_A1)
	{
		strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 111 );
		nPort = 7000;		
		m_pSocket_VisionInspect[active].SetSocketName(SOCKET_ACTIVE1);
	}
	else
	{
		strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 112 );
		nPort = 7000;		
		m_pSocket_VisionInspect[active].SetSocketName(SOCKET_ACTIVE2);
	}

	m_pSocket_VisionInspect[active].SetIP(strIP);
	m_pSocket_VisionInspect[active].SetPort(nPort);
	ConnectVISocket(active);

}
BOOL CSocketDialog::ConnectVISocket(const VI_ACTIVE active)
{
	return m_pSocket_VisionInspect[active].ConnectToServer();
}

void CSocketDialog::CreateMITAlignSocket()
{
	if(m_pSocket_MITAlign)
	{
		m_pSocket_MITAlign.Close();
	}

	m_pSocket_MITAlign.SetServer(FALSE);
	if(m_pSocket_MITAlign.Create())
	{
		m_pSocket_MITAlign.SetReciveHwnd(this->m_hWnd); // ���� Dlg �ּ� �Է�
	}

	CString strIP;
	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 150 );
	int nPort = 2000;

	m_pSocket_MITAlign.SetSocketName(SOCKET_MITALIGN);
	m_pSocket_MITAlign.SetIP(strIP);
	m_pSocket_MITAlign.SetPort(nPort);
	ConnectMITAlignSocket();
}

BOOL CSocketDialog::ConnectMITAlignSocket()
{
	return m_pSocket_MITAlign.ConnectToServer();
}

void CSocketDialog::CreateMITEdgeAlignSocket()
{
	if(m_pSocket_MITEdgeAlign)
	{
		m_pSocket_MITEdgeAlign.Close();
	}

	m_pSocket_MITEdgeAlign.SetServer(TRUE);				// ������ �����Ѵ�.

	CString strIP;
#if(AMT_TYPE == 1)
	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 140 );
	int nPort = 8000;
#else
	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 220 );
	int nPort = 8000;
#endif
	m_pSocket_MITEdgeAlign.SetPort(nPort);

	ClientSocketName socketName;
	socketName.Name = SOCKET_MITEDGEALIGN;
	socketName.IP = strIP;
	socketName.Port = nPort;

	m_pSocket_MITEdgeAlign.AddClientName( socketName );
	m_pSocket_MITEdgeAlign.SetReciveHwnd(this->m_hWnd);

	if(m_pSocket_MITEdgeAlign.Create(m_pSocket_MITEdgeAlign.GetPort()))
	{
		m_pSocket_MITEdgeAlign.Listen();											// ������ �����Ѵ�.
	}
}

void CSocketDialog::CreateRobotAlignSocket()
{
	if(m_pSocket_RobotAlign)
	{
		m_pSocket_RobotAlign.Close();
	}

	m_pSocket_RobotAlign.SetServer(TRUE);				// ������ �����Ѵ�.

	CString strIP;
	int nPort;
	ClientSocketName socketName;


	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 170 );
	nPort = 9002;
	socketName.Name = SOCKET_ROBOTALIGN;
	
	socketName.IP = strIP;
	socketName.Port = nPort;

	m_pSocket_RobotAlign.SetPort(nPort);

	m_pSocket_RobotAlign.AddClientName( socketName );
	m_pSocket_RobotAlign.SetReciveHwnd(this->m_hWnd);

	if(m_pSocket_RobotAlign.Create(m_pSocket_RobotAlign.GetPort()))
	{
		m_pSocket_RobotAlign.Listen();											// ������ �����Ѵ�.
	}
}
void CSocketDialog::CreateRobotSocket(E_ROBOT Robot)
{
	if(m_pSocket_Robot[Robot])
	{
		m_pSocket_Robot[Robot].Close();
	}

	m_pSocket_Robot[Robot].SetServer(TRUE);				// ������ �����Ѵ�.

	CString strIP;
	int nPort;
	ClientSocketName socketName;

	if (Robot == E_LD_ROBOT)
	{
		strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 170  );
		nPort = 9000;
		socketName.Name = SOCKET_LD_ROBOT;
	}
	else
	{
		strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 171  );
		nPort = 9001;
		socketName.Name = SOCKET_UD_ROBOT;
	}

	socketName.IP = strIP;
	socketName.Port = nPort;

	m_pSocket_Robot[Robot].SetPort(nPort);

	m_pSocket_Robot[Robot].AddClientName( socketName );
	m_pSocket_Robot[Robot].SetReciveHwnd(this->m_hWnd);

	if(m_pSocket_Robot[Robot].Create(m_pSocket_Robot[Robot].GetPort()))
	{
		m_pSocket_Robot[Robot].Listen();											// ������ �����Ѵ�.
	}
}
//kjpark 20161021 Setup ��ȣ�� UI ����
void CSocketDialog::SetAddLog(CString strValue)
{
	theLog[LOG_SOCKET_PG].AddBuf(strValue);
	CFormSignal* pView = (CFormSignal*)GetMainHandler()->GetGUIFrame()->GetView(eFORM_Setup1);
	if(pView->m_bIsShwoWindow)
		pView->SetLogListUpdate(strValue);
}


void CSocketDialog::CreateDataSocket()
{
	if(m_pSocket_DataPC)
	{
		m_pSocket_DataPC.Close();
	}

	m_pSocket_DataPC.SetServer(FALSE);

	if(m_pSocket_DataPC.Create())
	{
		m_pSocket_DataPC.SetReciveHwnd(this->m_hWnd); // ���� Dlg �ּ� �Է�

	}

	CString strIP;
	int nPort;
	//20161228 byskcho
	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 110 );
	//strIP.Format(_T("%s"),theConfigBank.m_System.m_strDataPc_IP);
	nPort = 6000;
	
	m_pSocket_DataPC.SetSocketName(SOCKET_DATAPC);
	m_pSocket_DataPC.SetIP(strIP);
	m_pSocket_DataPC.SetPort(nPort);

	ConnectDataSocket();
}

BOOL CSocketDialog::ConnectDataSocket()
{
	BOOL bRet = FALSE;
	if(m_pSocket_DataPC.ConnectToServer() == TRUE)
	{
		bRet = TRUE;
	}
	return bRet;
}

void CSocketDialog::CreateDCRSocket()
{

	if(m_pSocket_DCR)
	{
		m_pSocket_DCR.Close();
	}

	m_pSocket_DCR.SetServer(FALSE);
	if(m_pSocket_DCR.Create())
	{
		m_pSocket_DCR.SetReciveHwnd(this->m_hWnd); // ���� Dlg �ּ� �Է�
	}

	CString strIP;
	int nPort;

	//20170207 byskcho
	//ip 180������ ����
#if(AMT_TYPE == 1)
	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 180 );
	nPort = 10000;
#else
	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 200 );
	nPort = 1000;
#endif
	m_pSocket_DCR.SetSocketName(SOCKET_DCR);
	m_pSocket_DCR.SetIP(strIP);
	m_pSocket_DCR.SetPort(nPort);

	ConnectDCRSocket();
}

BOOL CSocketDialog::ConnectDCRSocket()
{
	BOOL bRet = FALSE;
	if(m_pSocket_DCR.ConnectToServer() == TRUE)
		bRet = TRUE;

	return bRet;
}

void CSocketDialog::CreateTrayAlignSocket()
{
	if(m_pSocket_Tray_Align)
	{
		m_pSocket_Tray_Align.Close();
	}

	m_pSocket_Tray_Align.SetServer(TRUE);				// ������ �����Ѵ�.

	CString strIP;
	strIP.Format(_T("%d.%d.%d.%d"), 192, 168, 10, 220 );
	int nPort = 8001;

	m_pSocket_Tray_Align.SetPort(nPort);

	ClientSocketName socketName;
	socketName.Name = SOCKET_TRAY_ALIGN;
	socketName.IP = strIP;
	socketName.Port = nPort;

	m_pSocket_Tray_Align.AddClientName( socketName );
	m_pSocket_Tray_Align.SetReciveHwnd(this->m_hWnd);

	if(m_pSocket_Tray_Align.Create(m_pSocket_Tray_Align.GetPort()))
	{
		m_pSocket_Tray_Align.Listen();											// ������ �����Ѵ�.
	}
}

BOOL CSocketDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
