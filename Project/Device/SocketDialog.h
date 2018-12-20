#pragma once
#include "Socket/DataSocket.h"
#include "resource.h"
#include "BankGuild/CellBank.h"
//kjpark 20161021 Setup 신호기 UI 구현
#include "GUIDefine.h"
#include "SockDefineDataPC.h"
#include "ETC/Queue.h"

// CSocketDialog 대화 상자입니다.
//class CCellBank;
class CSocketDialog : public CDialog
{
	DECLARE_DYNAMIC(CSocketDialog)

public:
	CSocketDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSocketDialog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CSOCKETDIALOG };

public:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg LRESULT OnPacketReceive( WPARAM wParam, LPARAM lParam );
	//kjpark 20160919 Topdlg에 connect status 
	afx_msg LRESULT OnSocketConnect( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSocketClose( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()

public:
	void DeleteAllSocket();					//모든 소켓을 닫는다. 
	void SocketClosed(CDataSocket *pSocket, CClientSocket* pClientSocket);
	USHORT GetCRC16_Checksum(const void *buf, int len);

	LRESULT OnReConnect( WPARAM wParam, LPARAM lParam );
	//kjpark 20160919 Topdlg에 connect status 	
	int GetSocketIndex(CString sName);
	BOOL GetCommConnected(int nIdx);				// 2014-12-11, jhLee, 지정 통신 연결 확인

	CString	PacketClear(CString strSocketName, CString strMsg);

	//////////////////////////////////////
	//			Pattern Genarator		//
	//////////////////////////////////////
	CDataSocket m_pSocket_PatternGenerator;

	BOOL GetCheckComConnected(int idx);							//지정한 TCP/IP연결상태를 반환한다.
	BOOL SendMassageToPatternGenerator(CString strCmdType,int jig, int ch, CString strExtraData = _T(""));//신호기로 메시지를 보낸다.  

	void SetPatternGeneratorDataProcessing(CDataSocket* pSocket = NULL);
	CString	ParsePatternGeneratorMessage(CString strMsg);		// 신호기에서 수신된 내용을 처리한다.

	void PatternGeneratorACK(CCellInfo* pCell, CString strJudge);
	


	//2017-02-26, jhLee, Job file download관련 명령 추가
	BOOL PG_TMD_SendRoute(CString sData);									// Job file을 받을 MES server의 경로를 지정한다.
	BOOL PG_TMD_SendFileList(CString sData);								// download할 파일 리스트를 전송한다.
	BOOL PG_TMD_SendCopyStart();												// MES Server로부터 복사를 지령한다.
	BOOL PG_TMD_SendChecksumStart();											// Download한 파일들의 Checksum 검사한다.
	BOOL PG_TMD_SendClientChecksum();										// Download한 파일과 각 신호기의 현재 파일과의 Checksum 비교를 한다.
	BOOL PG_TMD_SendDownloadStart();											// Download한 파일들을 각 신호기에게 전송한다.
	BOOL PG_TMD_SendInfo(int nJig, int nCh);								// 상시 점검을 위한 TMD_INFO 명령을 전송한다.



	//kjpark 20161006 MCR 상하 수정
	//////////////////////////////////////
	//				UV MCR 		//
	//////////////////////////////////////
	CDataSocket m_pSocket_Lower_UVMCR;
	CDataSocket m_pSocket_Upper_UVMCR;
	//kjpark 20160912 UVMCR 파싱
	void SetUVMCRDataProcessing(CDataSocket* pSocket = NULL);
	BOOL ParseUVMCRMessage(CString strReadingText);
	BOOL ParseUVMCRMessage(int uv, CString strReadingText);
	//////////////////////////////////////
	//			Vision Inspection		//
	//////////////////////////////////////
	CDataSocket m_pSocket_VisionInspect[VI_MAX];
	void SetVisionInspectDataProcessing(CDataSocket* pSocket = NULL);	
	CString ParseVIMessage( CDataSocket *pSocket, int nVi, CString strMsg );
	//BOOL SendMsgToVt(const VI_ACTIVE active, const int nCmdType, CString strData1);
	BOOL SendMsgToVt(const VI_ACTIVE active, const int nCmdType, CString strData1, CString strData2);
	BOOL WriteVtRecipeInfo();
	//////////////////////////////////////
	//			MIT_Align		    //
	//////////////////////////////////////
	CDataSocket m_pSocket_MITAlign;
	void SetMITAlignDataProcessing(CDataSocket* pSocket);
	int ParseMITAlignMessage(PBYTE pMsg, int nLen);
	BOOL SendMsgToContactAlign( JIG_ID jig, JIG_CH ch, CString strInnerID1, CString strInnerID2, CString strCellID1, CString strCellID2 );
	BOOL SendMsgToVisionAlign( JIG_CH ch, BOOL bFirst, CString strInnerID, CString strCellID );
		
	//////////////////////////////////////
	//			MIT_EdgeAlign		    //
	//////////////////////////////////////

	CDataSocket m_pSocket_MITEdgeAlign;
	void SetMITEdgeAlignDataProcessing(CDataSocket* pSocket);
	CString ParseMITEdgeAlignMessage(CString strMsg);
	BOOL SendMsgToMITEdgeAlign( CString strCommand, CString strParam1, CString strParam2 );
	BOOL SendMsgToContactSave(JIG_ID jig, JIG_CH ch);
	BOOL SendMsgToVisionSave(JIG_CH ch, BOOL bFirst);
	BOOL SendMsgToGrabMode();

	//////////////////////////////////////
	//			     Robot				//
	//////////////////////////////////////
	CDataSocket m_pSocket_RobotAlign;
	CDataSocket m_pSocket_Robot[E_ROBOT_MAX];
	void SetRobotAlignDataProcessing(CDataSocket* pSocket);
	CString ParseRobotAlignMessage(CDataSocket *pSocket ,CString strMsg);
	BOOL SendMsgToRobotAlign( double dPosX = 0., double dPosY = 0., double dPosT = 0.);
	void SetRobotDataProcessing(CDataSocket* pSocket);
	CString ParseRobotMessage();
	BOOL SendMsgToRobot(E_ROBOT Robot);

	DWORD SwapDWORD(DWORD dwValue);
	//////////////////////////////////////
	//				Data PC				//
	//////////////////////////////////////
	CQueue m_QueueDataPC;
	CStopWatch m_timerDataPCMsg;
	CDataSocket m_pSocket_DataPC;
	void SetDataPCProcessing(CDataSocket* pSocket = NULL);
	void ParseDataPCMessage();
	BOOL SendCmdToDataPC(int nMsgType, CString strData = _T(""));
	BOOL SendCmdTrayToDataPC(int nMsgType, CString strData1 , CString strData2);
	BOOL SendCmdCellToDataPC(int nMsgType, CCellInfo *pCell);
	BOOL SendCmdAlarmToDataPC(ALARM_ID id, BOOL bSet);
	BOOL SendCmdStateToDataPC(int nMsgType, E_EQST_MATCH state);
	BOOL SendCmdRecipeToDataPC(int nMsgType, CString strRecipeName, CString strOldRecipeName = _T(""));
	BOOL SendCmdLoginToDataPC(int nMsgType, CString strID = _T(""), CString strPW = _T(""));
	BOOL SendCmdTPSystemToDataPC(CString strCode, CString strName);
	BOOL SendToDataPC(tag_Message_Unicode msgU);

	//////////////////////////////////////
	//				Tray Reader			//
	//////////////////////////////////////
	CDataSocket m_pSocket_DCR;
	void SetTrayReaderDataProcessing(CDataSocket* pSocket = NULL);
	BOOL ParseTrayReaderMessage(CString strMsg);

	//////////////////////////////////////
	//			Tray_align			    //
	//////////////////////////////////////
	CDataSocket m_pSocket_Tray_Align;
	void SetTrayAlignDataProcessing(CDataSocket* pSocket);
	CString ParseTrayAlignMessage(CString strMsg);
	BOOL SendMsgToTrayAlign( CString strCommand, CString strParam1 );


	//20170206 byskcho
	//Carrier Assign Request보고용
	BOOL SendCmdCarrierAssignToDataPC( int nMsgType, CCarrierInfo *pCarrier);



	// 2017-02-27, jhLee, 현재 진행중인 Job file download 과정을 상위에 보고하도록 한다.
	BOOL SendToTMDDownloadState(CString sAction, CString sResult);

	//2017-03-09,skcho,  S14F1 
	BOOL SendToAttributRequest(CString sEqpID, CString sObjType, CString sObjID);
	
	BOOL SendCmdUnitInterlockToDataPC(int nMsgType,int nJig = 1,CString strData = _T(""),CString strInit = _T(""));  //2017-11-01,SKCHO, UNIT INTERLOCK 102 보고 전용 함수

	BOOL SendCmdUnitInterlockComfirmToDataPC(int nMsgType,int nChannel,CString strData);  //514보고
//kjpark 20160912 Cell info class 추가
#ifdef CMiLRe
private:
	afx_msg void OnTimer(UINT nIDEvent);



#endif 

//kjpark 20160912 socket test
#ifdef CMiLRe
public:
	void setSocketConnect();

	// Data PC
	void CreateDataSocket();
	BOOL ConnectDataSocket();

	// Lower MCR
	void CreateUVMcrSocket_Lower();
	BOOL ConnectUvSocket_Lower();

	// Upper MCR
	void CreateUVMcrSocket_Upper();
	BOOL ConnectUvSocket_Upper();

	// Vision
	void CreateVInspSocket(const VI_ACTIVE active);
	BOOL ConnectVISocket(const VI_ACTIVE active);

	// MITPrealign
	void CreateMITAlignSocket();	
	BOOL ConnectMITAlignSocket();

	//2차분 Tray Reader.
	void CreateDCRSocket();
	BOOL ConnectDCRSocket();

	
	// PG
	void CreatePatternGeneratorSocket();	

	// MITEdgeAlignServerSocket
	void CreateMITEdgeAlignSocket();

	// Robot
	void CreateRobotAlignSocket();

	void CreateRobotSocket(E_ROBOT Robot);

	// Tray AlignServerSocket
	void CreateTrayAlignSocket();

	//kjpark 20161021 Setup 신호기 UI 구현
	CString m_strLogValue;
	void SetAddLog(CString strValue);

#endif

	virtual BOOL OnInitDialog();
};
