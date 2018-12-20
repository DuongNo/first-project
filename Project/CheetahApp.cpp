
// Cheetah.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"

#include "CheetahApp.h"
#include "MainFrm.h"
#include "CheetahDoc.h"
#include "CheetahView.h"

#include <locale.h> 

#include "StateMachine/ThreadFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CCheetahApp

BEGIN_MESSAGE_MAP(CCheetahApp, CWinApp)
	  
END_MESSAGE_MAP()


// CCheetahApp ����

CCheetahApp::CCheetahApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("YWDSP.BaseFrame.Cheetah.Prototype"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.

	//////////////////////////////////////////////////////////////////////////
	// Mempory Leak ����
	BOOL bCheck = 0;
	if(bCheck)
	{
		long num = 27944;
		// 		_CrtMemDumpAllObjectsSince(0); 
		_CrtSetBreakAlloc(num);
	}
	//////////////////////////////////////////////////////////////////////////

}

// ������ CCheetahApp ��ü�Դϴ�.

CCheetahApp			theApp;
CThreadInitializer	theThreadInitializer;
CLogUnit				theLog[LOG_MAX];
CAxisParamBank		theAxisBank;
CCellBank			theCellBank;
CIOParamBank		theIOBank;

//20170118 byskcho
CLossTimeBank        theLossTimeBank;
//20161227 byskcho
CFDCBank			theFDCBank;
CMonitorBank        theMonitorBank;
//
CConfigBank			theConfigBank;
CProcDataBank		theProcBank;
CRecipeBank			theRecipeBank;
CTeachBank			theTeachBank;
CUnitCtrlFunc		theUnitFunc(MP2100_THREAD_MAIN_PROCESS);
CDeviceMotion		theDeviceMotion;
CDeviceIO			theDeviceIO;
cDeviceMelsec		theDeviceMelsec;

//kjpark 20160912 socket test
CSocketDialog		theSocketInterFace;
CSerialManager		theSerialInterFace;

CProductBank		theProductBank;
// CCheetahApp �ʱ�ȭ

BOOL CCheetahApp::InitInstance()
{

	// Start of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ���α׷� �ߺ� ���� ���� (Mutex �̿�)

	// �ߺ� ��������� Mutex ����
    HANDLE hMutexOneInstance = ::CreateMutex(NULL, TRUE, _T("G-AMT")); 
	
    // ���� �̹� ������� �ִٸ� Instance�� �̹� ������
    BOOL bDuplicateInstance = (::GetLastError() == ERROR_ALREADY_EXISTS); 
	
    if( hMutexOneInstance ) ::ReleaseMutex(hMutexOneInstance); 
	
    // �̹� �ϳ��� Instance�� �����ϸ� ���α׷� ����
    if( bDuplicateInstance ) {
        AfxMessageBox(_T("The program is already running."), MB_ICONERROR | MB_OK);
        return FALSE;
    }
	// end of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//////////////////////////////////////////////////////////////////////////
	// Parameter Load
	theConfigBank.ReadSystemFile();	// �갡 �ֿ켱���� Read�Ǿ�� �Ѵ�
	theConfigBank.ReadOptionFile();
	theConfigBank.ReadNGCodeFile();
	theConfigBank.ReadUnLoadRunInfo();//2017-03-06, skcho, jobchange�� ���� ���� �о�´�.(unload tray id�� ���� ����)

	theRecipeBank.ReadLastRecipeFile();

	theTeachBank.InitializeTeachParam();		// File Read���� ���� ������ ����
	//20170201 byskcho
	//������ ������  ƼĪ�� ���� ���� ����
	//theTeachBank.ReadTeachFile()
	theTeachBank.ReadTeachFile(theRecipeBank.m_strRecipeName);	

	theProcBank.ReadAlarmFile();

	theAxisBank.InitializeAxisType();	// File Read���� ���� ������ ����
	theAxisBank.ReadAxisParam();

	theIOBank.InitializeIOParam();

	//20161227 byskcho
	theFDCBank.InitItem();

	theProductBank.LoadMainCount();

	//20170612 JSJUNG
	theProductBank.LoadJigCount();

	//20170118 byskcho
	theLossTimeBank.LoadLosstimeItemList();
	// �񰡵� ������ �ӽ� ����ϱ����� �ʱ�ȭ
	theLossTimeBank.InitLosstimeInfo();
	theLossTimeBank.LoadLosstimeInfo();
	theLossTimeBank.InitialDataFile();

	//2017-05-25, SKCHO  ��� ���� ���� ���� ����
	theProcBank.InitTime();
	theProductBank.LoadProductFile(theProcBank.m_szCurrent_time.GetBuffer());
	//////////////////////////////////////////////////////////////////////////
	// Log Initialize
	InitializeLog();
	//////////////////////////////////////////////////////////////////////////
	// Device Open
	if(theDeviceMotion.Open_Mp2100(MP2100_THREAD_MAIN_PROCESS) == FALSE)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("MP2100 Board Open Fail"));
		AfxMessageBox(_T("Err : MP2100 Board Open Fail"));
	}

	if(theDeviceMotion.Open_NMC2() == FALSE)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("NMC Open Fail"));
		AfxMessageBox(_T("Err : NMC Open Fail"));
	}

	if(theDeviceIO.Open() == FALSE)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("DeviceNet Board Open Fail"));
	}


	// �κ� �Ἵ CC Link ���.
	if(theDeviceMelsec.Open(TRUE) == FALSE)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("CCLink Melsec  Open failed."));
		AfxMessageBox(_T("Err : CCLink Melsec  Open failed."));
	}

	if(theDeviceMelsec.Open(FALSE) == FALSE)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("Optical Melsec Open failed."));
		AfxMessageBox(_T("Err : Optical Melsec Open failed."));
	}

	//////////////////////////////////////////////////////////////////////////
	// Create Thread
	DWORD dwThreadDelay = 300;	// MP2100�� ���ÿ� �������� Open�ϸ� �ȵǼ� Delay �� [11/6/2016 OSC]
	Sleep(dwThreadDelay);

	if(theThreadInitializer.CreateThread(THREAD_INDEX_MAIN_SEQUENCE, MainSequenceThr, (LPVOID)THREAD_INDEX_MAIN_SEQUENCE) == FALSE)
	{
		AfxMessageBox(_T("MainSequenceThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}

	Sleep(dwThreadDelay);
	if(theThreadInitializer.CreateThread(THREAD_INDEX_LOADER_INTERFACE, CellRcvThr, (LPVOID)THREAD_INDEX_LOADER_INTERFACE) == FALSE)
	{
		AfxMessageBox(_T("CellRcvThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}

	Sleep(dwThreadDelay);
	if(theThreadInitializer.CreateThread(THREAD_INDEX_AGV_INTERFACE, AGVInterfaceThr, (LPVOID)THREAD_INDEX_AGV_INTERFACE) == FALSE)
	{
		AfxMessageBox(_T("AGVInterfaceThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}
	

	//2017-09-17,SKCHO GOOD TRAY MANUAL THREAD
	Sleep(dwThreadDelay);
	if(theThreadInitializer.CreateThread(THREAD_INDEX_TRAY_MANUAL_INTERFACE, GoodTrayManualThr, (LPVOID)THREAD_INDEX_TRAY_MANUAL_INTERFACE) == FALSE)
	{
		AfxMessageBox(_T("Good Tray Manual Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}
	

	Sleep(dwThreadDelay);
	//kjpark 20161019 IO Check Thread �߰�
	if(theThreadInitializer.CreateThread(THREAD_INDEX_IO_CHECK, IOCheckThr, (LPVOID)THREAD_INDEX_IO_CHECK) == FALSE)
	{
		AfxMessageBox(_T("IOCheckThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}
	
	Sleep(dwThreadDelay);

	if(theThreadInitializer.CreateLoadVTFileThread( LoadVTFileThr, (LPVOID)THREAD_INDEX_VT_FILE) == FALSE)
	{
		AfxMessageBox(_T("LoadVTFileThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}


	//2016-12-27,skcho  FDC Motoring Thread 
	Sleep(dwThreadDelay);
	if(theThreadInitializer.CreateThread(THREAD_INDEX_FDC_MONITOR, FDCMonitorThr, (LPVOID)THREAD_INDEX_FDC_MONITOR) == FALSE)
	{
		AfxMessageBox(_T("FDCMonitorThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}

	
	//2017-06-04,SKCHO  VACUUM FDC Motoring Thread 
	Sleep(dwThreadDelay);
	if(theThreadInitializer.CreateThread(THREAD_INDEX_FDC_MONITOR_VACUUM, VAC_FDCMonitorThr, (LPVOID)THREAD_INDEX_FDC_MONITOR_VACUUM) == FALSE)
	{
		AfxMessageBox(_T("VACC_FDCMonitorThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}

	
	//2017-03-08,skcho TMD DOWN LOAD ������
	Sleep(dwThreadDelay);
	if(theThreadInitializer.CreateThread(THREAD_INDEX_TMD_DOWNLOAD,TMDDownLoadThr, (LPVOID)THREAD_INDEX_TMD_DOWNLOAD) == FALSE)
	{
		AfxMessageBox(_T("TMDDownLoadThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}

	//2017-09-26,SKCHO, T5 RESTART ������ �߰�
	/*Sleep(dwThreadDelay);
	if(theThreadInitializer.CreateThread(THREAD_INDEX_T5_RESTART,T5ReStartThr, (LPVOID)THREAD_INDEX_T5_RESTART) == FALSE)
	{
		AfxMessageBox(_T("T5ReStartThr Thread �� ���� ���� ���߽��ϴ�."));
		return FALSE;
	}
	*/
	//Sleep(dwThreadDelay);
	//if(theThreadInitializer.CreateThread(THREAD_INDEX_MELSEC_MONITOR,MelSecMonitorThr, (LPVOID)THREAD_INDEX_MELSEC_MONITOR) == FALSE)
	//{
	//	AfxMessageBox(_T("MelSecMonitorThr Thread �� ���� ���� ���߽��ϴ�."));
	//	return FALSE;
	//}
	
	// �ܼ� ������Ʈ���� �޸𸮸� ������ ��� â�� ��µ� �� �ֵ��� �߰�
	// _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCheetahDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CCheetahView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// GDI+ 
	GdiplusStartup(&gdiplusToken, &gdiplusstartupinput, NULL);

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
	theLog[LOG_SYSTEM].AddBuf(_T("Program Start - Version %s"),ReadVersion());
	theProcBank.m_bProgramInitFinish = TRUE;

	//2018-03-08,SKCHO,SW ���� �߰�
	theMonitorBank.WriteSwVersion(ReadVersion());

	theProcBank.m_tmPGMStart = CTime::GetCurrentTime();		// ���α׷� ���� �ð� ����

	::GetLocalTime( &tmStart );			// ���� �ð�
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.


	//
	// Visual Studio 2005, 2008 ~ ���� TRACE �� ��� �� �ѱ��� ����� ���,
	// _CrtDbgReport: String too long or IO Error ������ �߻���,
	//
	// #include <locale.h>     include �� ��
	//
	// _tsetlocale(LC_ALL, _T("korean"));  ȣ��
	//
	CString a;
	a.Format(_T("%s"), _tsetlocale(LC_ALL, _T("korean"))); 
	//kjpark 20160912 socket test
	theSocketInterFace.Create(CSocketDialog::IDD, m_pMainWnd);
	theSocketInterFace.ShowWindow(SW_HIDE);

	theSocketInterFace.setSocketConnect();	

	//2017-08-30,SKCHO,LOADING ON �׽ÿ�
	theUnitFunc.SendAGVPortType(GOOD_IN_TRAY_PORT);
	//2017-08-30,SKCHO, UNLOADING ON �׽ÿ�
	theUnitFunc.SendAGVPortType(GOOD_OUT_TRAY_PORT);

	//2018-03-08,SKCHO,CNRK
	long Data[8];
	theDeviceMelsec.RobotWordRecvBuf(LDROBOT_TEACH_MOVE_XYZT_VAL,sizeof(long)*8,Data);
	theMonitorBank.WriteLDRobotPOS( (double)Data[0]/100, (double)Data[1]/100, (double)Data[2]/100, (double)Data[3]/100);
		
	theDeviceMelsec.RobotWordRecvBuf(UDROBOT_TEACH_MOVE_XYZT_VAL,sizeof(long)*8,Data);
	theMonitorBank.WriteULDRobotCH1POS( (double)Data[0]/100, (double)Data[1]/100, (double)Data[2]/100, (double)Data[3]/100 );
	theMonitorBank.WriteULDRobotCH2POS( (double)Data[0]/100, (double)Data[1]/100, (double)Data[2]/100, (double)Data[3]/100 );
			
	return TRUE;
}

int CCheetahApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	//kjpark 20160912 socket test
	theSocketInterFace.DestroyWindow ();
	theDeviceMelsec.Close(TRUE);
	theDeviceMelsec.Close(FALSE);
	// GDI+ ��
	GdiplusShutdown(gdiplusToken);


	AfxOleTerm(FALSE);

	theLog[LOG_SYSTEM].AddBuf(_T("Program Exit\n"));


	// _CrtDumpMemoryLeaks();				// Meory Leak ���� ǥ��

	return CWinApp::ExitInstance();
}

void CCheetahApp::InitializeLog()
{	
	theLog[LOG_SYSTEM].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SYSTEM].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SYSTEM].SetFileName(_T("System.txt"));
	theLog[LOG_SYSTEM].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SYSTEM].SetOffsetMin(0);
	theLog[LOG_SYSTEM].SetAddTime(TRUE);
	theLog[LOG_SYSTEM].SetAddDay(FALSE);
	theLog[LOG_SYSTEM].SetPartition(FALSE);

	theLog[LOG_SEQUENCE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE].SetSubFolderName(_T("Sequence"));
	theLog[LOG_SEQUENCE].SetFileName(_T("Sequence.txt"));
	theLog[LOG_SEQUENCE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE].SetOffsetMin(0);
	theLog[LOG_SEQUENCE].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE].SetAddDay(FALSE);
	//2017-09-14,SKCHO, ���� �뷮�� ũ�Ƿ� �ð����� ����
	//theLog[LOG_SEQUENCE].SetPartition(FALSE);
	theLog[LOG_SEQUENCE].SetPartition(TRUE);

	theLog[LOG_SOCKET].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SOCKET].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SOCKET].SetFileName(_T("Socket.txt"));
	theLog[LOG_SOCKET].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SOCKET].SetOffsetMin(0);
	theLog[LOG_SOCKET].SetAddTime(TRUE);
	theLog[LOG_SOCKET].SetAddDay(FALSE);
	theLog[LOG_SOCKET].SetPartition(FALSE);

	theLog[LOG_CIM].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_CIM].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_CIM].SetSubFolderName(_T("Cim"));
	theLog[LOG_CIM].SetFileName(_T("Cim.txt"));
	theLog[LOG_CIM].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_CIM].SetOffsetMin(0);
	theLog[LOG_CIM].SetAddTime(TRUE);
	theLog[LOG_CIM].SetAddDay(FALSE);
	//2017-09-14,SKCHO, �ð����� ����
	//theLog[LOG_CIM].SetPartition(FALSE);
	theLog[LOG_CIM].SetPartition(TRUE);

	theLog[LOG_SERIAL].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SERIAL].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SERIAL].SetSubFolderName(_T("Serial"));
	theLog[LOG_SERIAL].SetFileName(_T("Serial.txt"));
	theLog[LOG_SERIAL].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SERIAL].SetOffsetMin(0);
	theLog[LOG_SERIAL].SetAddTime(TRUE);
	theLog[LOG_SERIAL].SetAddDay(FALSE);
	theLog[LOG_SERIAL].SetPartition(TRUE);

	theLog[LOG_ERROR].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_ERROR].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_ERROR].SetFileName(_T("Error.txt"));
	theLog[LOG_ERROR].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_ERROR].SetOffsetMin(0);
	theLog[LOG_ERROR].SetAddTime(TRUE);
	theLog[LOG_ERROR].SetAddDay(FALSE);
	theLog[LOG_ERROR].SetPartition(FALSE);

	theLog[LOG_MP2100].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_MP2100].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_MP2100].SetFileName(_T("MP2100.txt"));
	theLog[LOG_MP2100].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_MP2100].SetOffsetMin(0);
	theLog[LOG_MP2100].SetAddTime(TRUE);
	theLog[LOG_MP2100].SetAddDay(FALSE);
	theLog[LOG_MP2100].SetPartition(FALSE);

	theLog[LOG_NMC].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_NMC].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_NMC].SetFileName(_T("NMC.txt"));
	theLog[LOG_NMC].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_NMC].SetAddTime(TRUE);
	theLog[LOG_NMC].SetAddDay(FALSE);
	theLog[LOG_NMC].SetPartition(FALSE);

	theLog[LOG_MOTOR_ERROR].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_MOTOR_ERROR].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_MOTOR_ERROR].SetFileName(_T("MotorError.txt"));
	theLog[LOG_MOTOR_ERROR].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_MOTOR_ERROR].SetAddTime(TRUE);
	theLog[LOG_MOTOR_ERROR].SetAddDay(FALSE);
	theLog[LOG_MOTOR_ERROR].SetPartition(FALSE);

	theLog[LOG_SEQUENCE_TRAY].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE_TRAY].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE_TRAY].SetSubFolderName(_T("Sequence_Tray"));
	theLog[LOG_SEQUENCE_TRAY].SetFileName(_T("Sequence_Tray.txt"));
	theLog[LOG_SEQUENCE_TRAY].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE_TRAY].SetOffsetMin(0);
	theLog[LOG_SEQUENCE_TRAY].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE_TRAY].SetAddDay(FALSE);
	theLog[LOG_SEQUENCE_TRAY].SetPartition(TRUE);

	//2017-12-27,SKCHO GMS�α� �߰�
	theLog[LOG_SERIAL_GMS].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SERIAL_GMS].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SERIAL_GMS].SetSubFolderName(_T("GMS"));
	theLog[LOG_SERIAL_GMS].SetFileName(_T("Serial_GMS.txt"));
	theLog[LOG_SERIAL_GMS].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SERIAL_GMS].SetOffsetMin(0);
	theLog[LOG_SERIAL_GMS].SetAddTime(TRUE);
	theLog[LOG_SERIAL_GMS].SetAddDay(FALSE);
	theLog[LOG_SERIAL_GMS].SetPartition(TRUE);

	theLog[LOG_VISION].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_VISION].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_VISION].SetSubFolderName(_T("Vision"));
	theLog[LOG_VISION].SetFileName(_T("Vision.txt"));
	theLog[LOG_VISION].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_VISION].SetOffsetMin(0);
	theLog[LOG_VISION].SetAddTime(TRUE);
	theLog[LOG_VISION].SetAddDay(FALSE);
	//2017-09-14,SKCHO, �ð����� ����
	//theLog[LOG_VISION].SetPartition(FALSE);
	theLog[LOG_VISION].SetPartition(TRUE);

	theLog[LOG_MITALIGN].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_MITALIGN].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_MITALIGN].SetSubFolderName(_T("MITAlign"));
	theLog[LOG_MITALIGN].SetFileName(_T("MITAlign.txt"));
	theLog[LOG_MITALIGN].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_MITALIGN].SetOffsetMin(0);
	theLog[LOG_MITALIGN].SetAddTime(TRUE);
	theLog[LOG_MITALIGN].SetAddDay(FALSE);
	//2017-09-14,SKCHO, �ð����� ����
	//theLog[LOG_MITALIGN].SetPartition(FALSE);
	theLog[LOG_MITALIGN].SetPartition(TRUE);

	theLog[LOG_MITEDGEALIGN].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_MITEDGEALIGN].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_MITEDGEALIGN].SetSubFolderName(_T("MITEdgeAlign"));
	theLog[LOG_MITEDGEALIGN].SetFileName(_T("MITEdgeAlign.txt"));
	theLog[LOG_MITEDGEALIGN].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_MITEDGEALIGN].SetOffsetMin(0);
	theLog[LOG_MITEDGEALIGN].SetAddTime(TRUE);
	theLog[LOG_MITEDGEALIGN].SetAddDay(FALSE);
	//2017-09-14,SKCHO, �ð����� ����
	//theLog[LOG_MITEDGEALIGN].SetPartition(FALSE);
	theLog[LOG_MITEDGEALIGN].SetPartition(TRUE);

	theLog[LOG_NACHIROBOT].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_NACHIROBOT].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_NACHIROBOT].SetFileName(_T("NachiRobot.txt"));
	theLog[LOG_NACHIROBOT].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_NACHIROBOT].SetOffsetMin(0);
	theLog[LOG_NACHIROBOT].SetAddTime(TRUE);
	theLog[LOG_NACHIROBOT].SetAddDay(FALSE);
	theLog[LOG_NACHIROBOT].SetPartition(FALSE);

	theLog[LOG_AGV].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_AGV].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_AGV].SetFileName(_T("AGV.txt"));
	theLog[LOG_AGV].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_AGV].SetOffsetMin(0);
	theLog[LOG_AGV].SetAddTime(TRUE);
	theLog[LOG_AGV].SetAddDay(FALSE);
	theLog[LOG_AGV].SetPartition(FALSE);

	theLog[LOG_OP2PG_PACKET].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_OP2PG_PACKET].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_OP2PG_PACKET].SetSubFolderName(_T("OP2PGPacket"));
	theLog[LOG_OP2PG_PACKET].SetFileName(_T("OP2PGPacket.txt"));
	theLog[LOG_OP2PG_PACKET].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_OP2PG_PACKET].SetOffsetMin(0);
	theLog[LOG_OP2PG_PACKET].SetAddTime(TRUE);
	theLog[LOG_OP2PG_PACKET].SetAddDay(FALSE);
	//2017-09-14,SKCHO, �ð����� ����
	//theLog[LOG_OP2PG_PACKET].SetPartition(FALSE);
	theLog[LOG_OP2PG_PACKET].SetPartition(TRUE);

	theLog[LOG_SOCKET_PG].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SOCKET_PG].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SOCKET_PG].SetSubFolderName(_T("Socket_PG"));
	theLog[LOG_SOCKET_PG].SetFileName(_T("Socket_PG.txt"));
	theLog[LOG_SOCKET_PG].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SOCKET_PG].SetOffsetMin(0);
	theLog[LOG_SOCKET_PG].SetAddTime(TRUE);
	theLog[LOG_SOCKET_PG].SetAddDay(FALSE);
	//2017-09-14,SKCHO, �ð����� ����
	//theLog[LOG_SOCKET_PG].SetPartition(FALSE);
	theLog[LOG_SOCKET_PG].SetPartition(TRUE);

	theLog[LOG_UNLOADING].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_UNLOADING].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_UNLOADING].SetFileName(_T("UnLoading_Time.txt"));
	theLog[LOG_UNLOADING].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_UNLOADING].SetOffsetMin(0);
	theLog[LOG_UNLOADING].SetAddTime(TRUE);
	theLog[LOG_UNLOADING].SetAddDay(FALSE);
	theLog[LOG_UNLOADING].SetPartition(FALSE);

	theLog[LOG_TRAY_READER].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_TRAY_READER].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_TRAY_READER].SetFileName(_T("Tray_Reader.txt"));
	theLog[LOG_TRAY_READER].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_TRAY_READER].SetOffsetMin(0);
	theLog[LOG_TRAY_READER].SetAddTime(TRUE);
	theLog[LOG_TRAY_READER].SetAddDay(FALSE);
	theLog[LOG_TRAY_READER].SetPartition(FALSE);

	theLog[LOG_TRAY_ALIGN].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_TRAY_ALIGN].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_TRAY_ALIGN].SetFileName(_T("Tray_Align.txt"));
	theLog[LOG_TRAY_ALIGN].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_TRAY_ALIGN].SetOffsetMin(0);
	theLog[LOG_TRAY_ALIGN].SetAddTime(TRUE);
	theLog[LOG_TRAY_ALIGN].SetAddDay(FALSE);
	theLog[LOG_TRAY_ALIGN].SetPartition(FALSE);

	theLog[LOG_OPERATION].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_OPERATION].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_OPERATION].SetFileName(_T("Operation.txt"));
	theLog[LOG_OPERATION].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_OPERATION].SetAddTime(TRUE);
	theLog[LOG_OPERATION].SetAddDay(FALSE);
	theLog[LOG_OPERATION].SetPartition(FALSE);

	theLog[LOG_UV_MCR].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_UV_MCR].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_UV_MCR].SetFileName(_T("UVMCR.txt"));
	theLog[LOG_UV_MCR].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_UV_MCR].SetOffsetMin(0);
	theLog[LOG_UV_MCR].SetAddTime(TRUE);
	theLog[LOG_UV_MCR].SetAddDay(FALSE);
	theLog[LOG_UV_MCR].SetPartition(FALSE);

	theLog[LOG_LOAD_DELAY_TACT].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_LOAD_DELAY_TACT].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_LOAD_DELAY_TACT].SetFileName(_T("LoadDelayTact.txt"));
	theLog[LOG_LOAD_DELAY_TACT].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_LOAD_DELAY_TACT].SetOffsetMin(0);
	theLog[LOG_LOAD_DELAY_TACT].SetAddTime(TRUE);
	theLog[LOG_LOAD_DELAY_TACT].SetAddDay(FALSE);
	theLog[LOG_LOAD_DELAY_TACT].SetPartition(FALSE);

	theLog[LOG_TURN_TABLE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_TURN_TABLE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_TURN_TABLE].SetFileName(_T("TURNTABLE.txt"));
	theLog[LOG_TURN_TABLE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_TURN_TABLE].SetOffsetMin(0);
	theLog[LOG_TURN_TABLE].SetAddTime(TRUE);
	theLog[LOG_TURN_TABLE].SetAddDay(FALSE);
	theLog[LOG_TURN_TABLE].SetPartition(FALSE);

	theLog[LOG_TEACH_MOVE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_TEACH_MOVE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_TEACH_MOVE].SetFileName(_T("TEACHMOVE.txt"));
	theLog[LOG_TEACH_MOVE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_TEACH_MOVE].SetOffsetMin(0);
	theLog[LOG_TEACH_MOVE].SetAddTime(TRUE);
	theLog[LOG_TEACH_MOVE].SetAddDay(FALSE);
	theLog[LOG_TEACH_MOVE].SetPartition(FALSE);

	theLog[LOG_LDRB_TEACH_MOVE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_LDRB_TEACH_MOVE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_LDRB_TEACH_MOVE].SetFileName(_T("LDRB_TEACHMOVE.txt"));
	theLog[LOG_LDRB_TEACH_MOVE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_LDRB_TEACH_MOVE].SetOffsetMin(0);
	theLog[LOG_LDRB_TEACH_MOVE].SetAddTime(TRUE);
	theLog[LOG_LDRB_TEACH_MOVE].SetAddDay(FALSE);
	theLog[LOG_LDRB_TEACH_MOVE].SetPartition(FALSE);

	theLog[LOG_UDRB_TEACH_MOVE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_UDRB_TEACH_MOVE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_UDRB_TEACH_MOVE].SetFileName(_T("UDRB_TEACHMOVE.txt"));
	theLog[LOG_UDRB_TEACH_MOVE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_UDRB_TEACH_MOVE].SetOffsetMin(0);
	theLog[LOG_UDRB_TEACH_MOVE].SetAddTime(TRUE);
	theLog[LOG_UDRB_TEACH_MOVE].SetAddDay(FALSE);
	theLog[LOG_UDRB_TEACH_MOVE].SetPartition(FALSE);
}

// CCheetahApp �޽��� ó����

CString CCheetahApp::ReadVersion()
{
	CString szVer=_T("") ;
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);		//���� ���ҽ����� ������ ������

	if (hRsrc != NULL)
	{
		HGLOBAL hGlobalMemory = LoadResource(NULL, hRsrc);			//	�۷ι��ڵ鿡 ���ҽ�����
		if (hGlobalMemory != NULL)
		{
			void *pVersionResouece = LockResource(hGlobalMemory);	//	�ڵ鿡�� ���� �ܾ��
			void *pVersion;
			UINT uLength;

			// �Ʒ��ٿ� 041204B0�� ���ҽ� ����(*.rc)���� ������.

			if( VerQueryValue(pVersionResouece, _T("StringFileInfo\\041204B0\\ProductVersion"), &pVersion, &uLength) != 0 )
			{
				szVer = reinterpret_cast<wchar_t*>(pVersion);			
			}
			FreeResource(hGlobalMemory);
		}
	}
	return szVer;
}