#pragma once
#include "Device/DeviceMotion.h"
#include "Etc/StopWatch.h"
#include "BankGuild/CellBank.h"

//////////////////////////////////////////////////////////////////////////
// Sequenc 식별 Name
#define STATE_FUNC							_T("FUNC")
#define STATE_RESTART						_T("RESTART")
#define STATE_CELLRECEIVE					_T("CELLRECEIVE")
#define STATE_MANUALRECEIVE					_T("MANUALRECEIVE")
#define STATE_CONVMOVE						_T("CONVMOVE")
#define STATE_LDROBOTGET					_T("LDROBOTGET")
#define STATE_LDROBOTUPPERPUT				_T("LDROBOTUPPERPUT")
#define STATE_LDROBOTLOWERPUT				_T("LDROBOTLOWERPUT")
#define STATE_LOWERSHUTTLEGET				_T("LOWERSHUTTLEGET")
#define STATE_LOWERSHUTTLEPUT				_T("LOWERSHUTTLEPUT")
#define STATE_UPPERSHUTTLEGET				_T("UPPERSHUTTLEGET")
#define STATE_UPPERSHUTTLEPUT				_T("UPPERSHUTTLEPUT")
#define STATE_PATRUPPERGET					_T("PATRUPPERGET")
#define STATE_PATRLOWERGET					_T("PATRLOWERGET")
#define STATE_PATRPUT						_T("PATRPUT")
#define STATE_PASTAGEGET					_T("PASTAGEGET")
#define STATE_PASTAGE						_T("PASTAGE")
#define STATE_PASTAGEPUT					_T("PASTAGEPUT")
#define STATE_LDTRGET						_T("LDTRGET")
#define STATE_LOWERMCR						_T("LOWERMCR")
#define STATE_LDTRPUT						_T("LDTRPUT")
#define STATE_UPPERMCR						_T("UPPERMCR")

#define STATE_AMTTABLE						_T("AMTTABLE")
#define STATE_AZONE							_T("AZONE")
#define STATE_BZONE							_T("BZONE")
#define STATE_CZONE							_T("CZONE")
#define STATE_DZONE							_T("DZONE")

#define STATE_UDTRGET						_T("UDTRGET")
#define STATE_UDTRPUT						_T("UDTRPUT")
#define STATE_UDROBOTGET					_T("UDROBOTGET")
#define STATE_UDROBOTPUT1					_T("UDROBOTPUT1")
#define STATE_UDROBOTPUT2					_T("UDROBOTPUT2")

//2018-05-14,SKCHO, CONVAYOR 추가
#define STATE_UDROBOTPUT3					_T("UDROBOTPUT3")


////20180816wjp

#define STATE_GOODTRAYGETPUT                 _T("GOODTRAYGETPUT")
#define STATE_UDROBOTPICK2                   _T("UDROBOTPICK2")

#define STATE_JOBCHANGE						_T("STATE_JOBCHANGE")

#define STATE_GOODTRAYRECEIVE				_T("GOODTRAYRECEIVE")
#define STATE_GOODTRAYINLIFT				_T("GOODTRAYINLIFT")
#define STATE_GOODTRAYINLIFTPITCH			_T("GOODTRAYINLIFTPITCH")
#define STATE_GOODTRAYTRGET					_T("GOODTRAYTRGET")
#define STATE_GOODTRAYTRPUT					_T("GOODTRAYTRPUT")
#define STATE_GOODTRAYDCR					_T("GOODTRAYDCR")
#define STATE_GOODTRAYALIGN					_T("GOODTRAYALIGN")
#define STATE_GOODTRAYOUTLIFT				_T("GOODTRAYOUTLIFT")
#define STATE_GOODTRAYOUTLIFTPITCH			_T("GOODTRAYOUTLIFTPITCH")
#define STATE_GOODTRAYSEND					_T("GOODTRAYRSEND")

#define STATE_NGTRAYRECEIVE					_T("NGTRAYRECEIVE")
#define STATE_NGTRAYIN						_T("NGTRAYIN")
#define STATE_NGTRAYINLIFTUP				_T("NGTRAYINLIFTUP")
#define STATE_NGTRAYINLIFTPITCH				_T("NGTRAYINLIFTPITCH")
#define STATE_NGTRAYTRGET					_T("NGTRAYTRGET")
#define STATE_NGTRAYTRPUT					_T("NGTRAYTRPUT")
#define STATE_NGTRAYALIGN					_T("NGTRAYALIGN")
#define STATE_NGTRAYOUTLIFTPITCH			_T("NGTRAYOUTLIFTPITCH")
#define STATE_NGTRAYOUTLIFT					_T("NGTRAYOUTLIFT")
#define STATE_NGTRAYSEND					_T("NGTRAYSEND")


//2017-09-17,SKCHO
#define STATE_GOODTRAYMANUALRECEIVE			_T("GOODTRAYMANUALRECEIVE")
//2017-09-26,SCKHO, T5 RESTART
#define STATE_CH1_T5RESTART					_T("STATE_CH1_T5RESTART")
#define STATE_CH2_T5RESTART					_T("STATE_CH2_T5RESTART")
#define STATE_CH3_T5RESTART					_T("STATE_CH3_T5RESTART")
#define STATE_CH4_T5RESTART					_T("STATE_CH4_T5RESTART")
#define STATE_CH5_T5RESTART					_T("STATE_CH5_T5RESTART")
#define STATE_CH6_T5RESTART					_T("STATE_CH6_T5RESTART")
#define STATE_CH7_T5RESTART					_T("STATE_CH7_T5RESTART")
#define STATE_CH8_T5RESTART					_T("STATE_CH8_T5RESTART")

//////////////////////////////////////////////////////////////////////////
#define MAX_RTN_SIZE	20

class CUnitCtrlBank
{
public:
	CUnitCtrlBank(CString StateName, MP2100_THREAD nThreadID, BOOL bManual = FALSE, LOG_INDEX logindex = LOG_SEQUENCE);
	~CUnitCtrlBank(void) {};

	enum
	{
		stepIdle	= -1,
		stepStart	= 0,
		stepHold	= 999,
	};

protected:
	CString m_strStateName;
	int		m_nStep;
	int		m_nError;
	MP2100_THREAD		m_nThreadID;
	CStopWatch m_Timer;

	BOOL m_bManualSequence;		// 티칭 UI 등에서 얘만 단동으로 할 경우	TRUE
	LOG_INDEX m_LogIndex;

	//////////////////////////////////////////////////////////////////////////
	// Rtn관련 변수
	BYTE m_bRtnComp[MAX_RTN_SIZE];	
	BYTE m_bRtn[MAX_RTN_SIZE];
	void ResetReturnValue();	// Rtn변수들을 전부 TRUE로 Set한다.
	BOOL IsReturnOk();			// Rtn과 RtnComp를 Compair하여 OK/NG를 판단한다.
	//////////////////////////////////////////////////////////////////////////
	
public:
	virtual	int Run()=0;
	void	Start() 
	{
		m_Timer.Start();
		m_nStep = stepStart;
		m_nError = -1;
	};
	void	Reset()
	{ 
		m_nStep = stepIdle; 
		m_nError = -1;
	};
	BOOL IsStoped()
	{
		return m_nStep == stepIdle ? TRUE:FALSE;
	};
	BOOL IsHolded()
	{
		return m_nStep == stepHold ? TRUE:FALSE;
	};

	void SetError(ALARM_ID id, CString strDiscription=_T(""));
	int	GetError()	{ return m_nError; };
	static SYSTEMTIME	m_systmA_StartTime;

public:
	AZONE_CELL_EXIST isAZoneCell;

	//////////////////////////////////////////////////////////////////
	// IO Part
	//////////////////////////////////////////////////////////////////
	BOOL GetInPutIOCheck(int ID);	
	BOOL GetInPutIOCheck(INPUT_ID ID, ONOFF bValue);	
	BOOL GetOutPutIOCheck(int ID);
	void SetOutPutIO(int ID, BOOL bValue);
	void SetOutPutIO(int ID, ONOFF bValue);
	//kjpark 20161016 라이트커튼 추가
	BOOL LightCurtainOnCheck();
	void TeachModeOn(BOOL bOn) ;
	BOOL DoorInterLockChk();
	//void LampChange(int nRed, int nYellow, int nGreen, BOOL bBuzzer);
	void LampChange(int nRed, int nYellow, int nGreen, int nBuzzer);
	void LampChange(STATUS_LAMP_DESCRIPTION nState, int nBuzzer);
	BOOL TempChk();
	BOOL EMSChk();
	BOOL MainVacChk();
	BOOL FANChk();
	void IonizerChk();

	BOOL DoorInterLockChkStart();	//메시지 트루
	BOOL LightCurtainChkStart();	//메시지 트루 
	BOOL DoorKeyModeChkStart();		//메시지 트루 

	//////////////////////////////////////////////////////////////////
	BOOL GetAxisCheck(TEACH_PARAM::ID  idTeach );
	BOOL GetAxisReadyCheck(AXIS_ID axis, BOOL bIgnoreOrigin = TRUE);
	BOOL TheachMotionChk(MP2100_THREAD thr, TEACH_PARAM::ID idTeach);	//Teach move pos check
	void SetMotorStop(AXIS_ID  idAxis);
	
	BOOL CheckOriginInterlock(AXIS_ID idAxis);	
	BOOL CheckTeachMoveInterlock(TEACH_PARAM::ID idTeach, double dOffset = 0. , BOOL RunState = FALSE);	
	CString m_strLastKorMsg, m_strLastEngMsg, m_strLastVnmMsg;

	void SendPGMessage(CString strCommand, ZONE_ID zone, CString strExtraData = _T(""));
	void SendPGMessage(CString strCommand, ZONE_ID zone, JIG_CH nCh, CString strExtraData = _T(""));

	//////////Unload TR & Unload Stage

	BOOL UnloadTRVacChk(BOOL bVacOn, BOOL bCH1, BOOL bCH2);			//Cell ZIG ch1,ch2
	void UnloadTRVacOnOff(JIG_CH nCh , BOOL bon);	//Vac on/off
	void UnloadTR_BlowOff(JIG_CH nCh);
	

	void IsCell_UnloadTRVacOnOff(JIG_CH nCh , BOOL IsCell);  //2017-04-20, skcho , 셀유무로 vac / blower 동작할때 사용

	void MoveUnloadTR_X_GetPos();
	void MoveUnloadTR_X_PutPos();
	BOOL MoveUnloadTR_X_GetPosChk();
	BOOL MoveUnloadTR_X_PutPosChk();

	void MoveUnLoadTR_Z_GetPos();
	void MoveUnLoadTR_Z_PutPos();
	BOOL MoveUnLoadTR_Z_GetPosChk();
	BOOL MoveUnLoadTR_Z_PutPosChk();

	void MoveUnLoadTR_ZAxis_Up(double dSpeed = 0.0);
	BOOL MoveUnLoadTR_ZAxis_UpChk();

	void UnloadTactTime();
////////////////////Turn Table

	void TurnTable_Move();						
	BOOL TurnTable_MoveChk();

	static JIG_ID AMT_GetAutoJigID(ZONE_ID zone);

	int	AMT_GetAutoTblPos(ZONE_ID cpos);
	int	GetTablelPos(ZONE_ID cpos);
	int GetEntryTablePos(double dRange = 0.02);
	BOOL MoveToTblPos(int nTblPos, double dSpeedRatio=1.0);
	BOOL MoveToNext(BOOL bAuto,double dSpeedRatio = 1.0);

	BOOL GetZoneEnd( ZONE_ID zone );
	void SetZoneEnd( ZONE_ID zone, BOOL bValue );

	void AMT_PGSetZone();

	void ZoneTimeStart();
	void ZoneTimeEnd( ZONE_ID zone );
	void WaitTimeEnd();

	BOOL GetTurnTableMoveReadyAxisCheck();
	BOOL IsFinishTurnTable();
	BOOL OriginRetrun(AXIS_ID IdAxis);

	int TableParsing(int nTblPos );

	void B_Zone_Align_Grab(JIG_CH Ch , BOOL bFirst, BOOL bManual = FALSE);	////B_zone cam grab commend
	BOOL B_Zone_Align_Check();			//B_zone cam align check

	void CellData_SetVTAlignScore(JIG_CH ch, E_MARK index, double score);
	void CellData_SetVTAlignth(JIG_CH ch, double th, ALIGN_RESULT result);
	double	CellData_GetVTAlignth( JIG_CH ch , ZONE_ID Zone = ZONE_ID_C);
	ALIGN_RESULT	CellData_GetVTAlignResult( JIG_CH ch );

	BOOL  C_Zone_Inspection_Start(VI_ACTIVE nActive,JIG_CH nCh); //C_Zone vision Inspection 
	BOOL C_Zone_Inspection_Check(JIG_CH ch);

	void TableTurnTimeStart();
	void TableTurnTimeEnd();

	void AZoneStartTime();		
	void AZoneEndTime();	
	void AZoneWaitTime();
	void AZoneContactStartTime();		
	void AZoneContactEndTime();	

	void BZoneStartTime();		
	void BZoneEndTime();	
	void BZoneWaitTime();
	void BZoneAlignStartTime();		
	void BZoneAlignEndTime();	

	void CZoneStartTime();		
	void CZoneEndTime();	
	void CZoneWaitTime();
	void CZoneVIStartTime();		
	void CZoneVIEndTime();	
	void CZoneVIGrabStartTime();		
	void CZoneVIGrabEndTime();	
	void CZoneVIClassStartTime();		
	void CZoneVIClassEndTime();	

	void DZoneStartTime();		
	void DZoneEndTime();	
	void DZoneWaitTime();

	void C_ZONE_JudgeFinalClass(JIG_CH ch);
	BOOL C_Zone_VT_FileLoadChk(JIG_CH nCh);
	BOOL C_Zone_VT_ClassEndChk(JIG_CH nCh);

	INSP_STATE C_Zone_Get_InspState(JIG_CH nCh);



private:
	static BOOL	m_bZoneEnd[ZONE_ID_MAX];	
public:

	////////////A_ZONE
	BOOL A_Zone_ContactChk(JIG_CH nCh);
	void A_Zone_SetContactCount(JIG_CH nCh);
	int  A_Zone_GetContactCount(JIG_CH nCh);
	BOOL IsJobEnd();
	void A_Zone_SetClass(JIG_CH nCh);
	void A_Zone_TMDInfo(JIG_CH nCh);

	////////////B_ZONE
	BOOL B_Zone_ContactChk(JIG_CH nCh);
	void NGCountChk(JIG_CH nCh);
	void B_Zone_SetContactCount(JIG_CH nCh);
	void B_Zone_Cam_X(JIG_CH Ch,int pos);
	void B_Zone_Cam_Y(JIG_CH Ch,int pos);

	BOOL B_Zone_Cam_X_Check(JIG_CH Ch, int Pos);			//B_zone cam X check
	BOOL B_Zone_Cam_Y_Check(JIG_CH Ch, int Pos);			//B_zone cam Y check

	void B_Zone_Cam_Y_Ready();
	BOOL B_Zone_Cam_Y_Ready_Chk();
	void B_Zone_Cam_X_Ready();
	BOOL B_Zone_Cam_X_Ready_Chk();

	void B_Zone_SetClass(JIG_CH nCh);
	////////////C_ZONE
	void C_Zone_CamX();

	void C_Zone_LCamZ();
	void C_Zone_LCam_Focus();
	void C_Zone_LCam_Theta(double dbOffset = 0.);

	void C_Zone_RCamZ();
	void C_Zone_RCam_Focus();
	void C_Zone_RCam_Theta(double dbOffset = 0.);

	BOOL C_Zone_CamXChk();

	BOOL C_Zone_LCamZChk();
	BOOL C_Zone_LCam_FocusChk();
	BOOL C_Zone_LCam_ThetaChk(double dbOffset = 0.);

	BOOL C_Zone_RCamZChk();
	BOOL C_Zone_RCam_FocusChk();
	BOOL C_Zone_RCam_ThetaChk(double dbOffset = 0.);

	////////////D_ZONE
	BOOL D_Zone_VT_ClassEndChk(JIG_CH nCh);
	BOOL D_Zone_VT_FileLoadChk(JIG_CH nCh);
	BOOL TurnOffChk(JIG_CH nCh);

	void JudgeFinalClass(JIG_CH ch);
	CString GetMesCode(CString strDefectName);

	void WriteCellLog(CELL_POS pos);

	void Zone_Switching();

	void SetCZoneResultSkip(JIG_CH ch);
	
	// 지그 스킵 체크 함수
	BOOL JigSkipCheck();

	// 체널 스킵 체크 함수.
	void SetChSkip();
	JIG_CH GetChSkip();
	BOOL ChSkipJigIDCheck();// Ch Skip ID 체크 하는 건데 아직 안함 2017/04/26 HSK.

	// Vision 연속 알람 발생 아직 안씀 2017/04/26 HSK
	void SetVisionNGCnt(JIG_ID Jig, JIG_CH Ch, BOOL bGood);
	BOOL GetVisionNG(JIG_ID Jig, JIG_CH Ch);
	// G-AMT Cell Data 이동.
	// Repeat Insp 용 Cell Data 처리함수.
	void Zone_CellInfo_Clear(CELL_POS cellPos);
	void Set_Table_CellData();

	void CellData_SetDryRun(int i);
	void CellData_AllClear();

	void CellData_LDConvIn();
	void CellData_LDRobotFromLDConvIn();
	void CellData_ShuttleFromLDRobot(CELL_POS cellPos);
	void CellData_PATRFromShuttle(CELL_POS cellPos);
	void CellData_PAStageFromPATR();
	void CellData_LDTRFromPAStage();
	void CellData_AMTFromLDTR();
	void CellData_ToNextPos(BOOL bReverse = FALSE);
	void CellData_UDTRFromAMT();
	void CellData_UDStageFromUDTR();
	void CellData_UDRobotFromUDStage(CELL_POS cellPos);
	void CellData_UDRobotRemove();

	//20170207 byskcho
	void CellData_OKTrayFromUDRobot(CELL_POS cellPos);
	void CellData_OKTrayRemove();

	//2017-03-03, skcho NGtray로 셀 정보 이동
	void CellData_NGTrayFromUDRobot(CELL_POS cellPos);
	void CellData_NGTrayRemove();

	//2018-05-16,MGYUN,추가
	void CellData_ULDConvFromUDRobot(CELL_POS cellPos);
	void CellData_ULDConvRemove();
	//2018wjp
	void CellData_TrayFromRobot(CELL_POS cellPos,int iRow,int Col,BOOL isClear=FALSE);

	void CellData_RobotFromTray(int iRow,int Col);

	void CellData_Tray_ClearAll();

	void CellData_Tray_CreateAll();

	// G-AMT Cell Data Check.
	BOOL CellTagExist(CELL_POS pos);
	BOOL CellTagExist(CELL_POS posStart, CELL_POS posEnd, CHECK_OPTION chkoption = CHECK_OR);

	BOOL CellDataMachineChk(CELL_POS posStart,CELL_POS posEnd);

	CCellTag SearchAvtCell(CString strInnerID, ZONE_ID startZone, ZONE_ID endZone);
	// G-AMT Tray Data Check
	BOOL TrayData_Exist(TRAY_PORT port, TRAY_INFO_POS pos);	
	BOOL TrayData_CellIsEmpty(TRAY_PORT port, TRAY_INFO_POS pos);	
	BOOL TrayData_CellIsFull(TRAY_PORT port, TRAY_INFO_POS pos);	
	BOOL TrayData_TrayIsFull(TRAY_PORT port, TRAY_INFO_POS pos);	
	BOOL TrayData_ExistCell(TRAY_PORT port, TRAY_INFO_POS pos);

	void TrayData_Create(TRAY_PORT port , TRAY_INFO_POS Pos);
	void TrayData_CreateEx(TRAY_PORT port , TRAY_INFO_POS Pos);
	void TrayData_RemoveData(TRAY_PORT port , TRAY_INFO_POS Pos);
	void TrayData_from_to(TRAY_PORT port , TRAY_INFO_POS Removepos , TRAY_INFO_POS Setpos);
	void TrayData_from_toAlign( TRAY_PORT port , TRAY_INFO_POS Removepos , TRAY_INFO_POS Setpos);


	// CIM
	//void CIM_CellInfoRequest();		// Cell ID를 통해 Cell 정보 요청. [11/30/2016 OSC]
	//20170201 byskcho
	void CIM_CH1_CellInfoRequest();
	void CIM_CH2_CellInfoRequest();
	
	//2018-07-25,SKCHO
	void CIM_CellInfoRequest(CELL_POS pCellPos);
	//2018wjp
	void CIM_CellInfoRequestEx(CString strCellID);

	BOOL CIM_CellTrackIn(CELL_POS Pos);			// 설비에 들어왔다고 상위에 보고. [11/30/2016 OSC]
	void CIM_CellAllInspectionEnd();	// 모든 검사 종료
	void CIM_CellTrackOut();			// 설비에 나간다고 상위에 보고. D존에서 A존 회전시 한다
	void CIM_CellExistInMachine();			// 설비에 Cell이 있는지, 없는지 보고
	int CIM_SetRCMD(CELL_POS Pos);
	void CIM_CellClear();			

	BOOL Get_InspectSkip(CELL_POS Pos);
	void Set_InspectSkip(CELL_POS Pos, BOOL bSkip);

	//2017-05-19,skcho RCMD 결과 저장 및 불러오기
	BOOL Get_RCMDResult(CELL_POS Pos);
	void Set_RCMDResult(CELL_POS Pos, BOOL bSkip);


	void CIM_TRAYProcess(CString strCEID,CString strPort);
	void CIM_TRAYProcessReport();

	BOOL CellData_TrackInChk(CELL_POS Pos);

	//20170201 byskcho
	int CIM_CellInfoRequestCheck(CELL_POS Pos);
	BOOL CellData_CellInfoRequestChk(CELL_POS Pos);

	//20170206 byskcho
	BOOL CIM_CarrierRelease();
	void CIM_CarrierAssign();

	void CIM_CellLossReport(CELL_POS Pos); //2017-03-02, skcho, CELL 임의 추출 보고

	void CIM_CellLot_Info_Request();  //2017-03-09, skcho , Unloader tray id squence에서  S6F205 메시지
	void CIM_Attribute_Request();     //2017-03-09, skcho , Unloader tray id squence에서  S14f1 메시지
	void CIM_CELLID_Info_Request();   //2018wjp

	BOOL CIM_CarrierReportCheck();

	CString GetCellPPID(CELL_POS Pos);

	void CIM_CarrierCellLoadStart();
	void CIM_CarrierCellLoadEnd();

	BOOL ReadJobProcessResult(CELL_POS Pos);
	BOOL ReadCellInformationResult(CELL_POS Pos);
	BOOL DeleteResult(CELL_POS CH1,CELL_POS CH2);//정상적으로 처리되면 생성된 파일은 지운다.
	//2017-10-09,SKCHO
	BOOL ReadCarrierResult();
	
	//2017-12-04,SKCHO
	BOOL ReadPortEvent353();
	BOOL SavePortEvent353(CString strID);
	void DeletePortEvent353();
	
	/////////////////////////////////////////////////////////////////////////
	// Upper IF//////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	void	SendRcvEnable(ONOFF bOn);
	void	SendRcvStart(ONOFF bOn);
	void	SendRcvComp(ONOFF bOn);
	void	SendRcvPause(ONOFF bOn);
	BOOL	RcvEnableChk();
	BOOL	RcvStartChk();
	BOOL	RcvCompChk();

	/////////////////////////////////////////////////////////////////////////
	// CONVEYOR & LD ROBOT & ULD ROBOT//////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	void	FPCBUseSol(BOOL bOn);

	BOOL	LDConvCellChk(E_CONV_SEN_INDEX Sen);
	void	LDConvZeroPointSet();
	void	LDConvMove(double Speed = 0.0);
	void	LDConvStop();
	BOOL	LDConvStopChk();
	BOOL	IsCellLoadConv();

	void	LoaderAlignStart();
	BOOL	LoaderAlignEndChk();
	BOOL	LoaderAlignResultChk();

	double	LoaderAlignResultX();
	double	LoaderAlignResultY();
	double	LoaderAlignResultT();

	void	LDRobotSendAlignData(double dPosX = 0., double dPosY = 0., double dPosT = 0.);
	void	RobotCellVacOn(const BOOL bVacOn, E_ROBOT Robot);
	void	RobotBlowOff(E_ROBOT Robot);
	BOOL	RobotCellVacOnChk(E_ROBOT Robot,VAC_STATE Vac);

	void	LDRobotBitAllOff(E_ROBOT_POS Pos,BOOL bManual = TRUE);
	void	UDRobotBitAllOff(E_ROBOT_POS Pos,BOOL bManual = TRUE);

	BOOL	LDConvFPCBSenChk(SENSOR_STATE sen);


	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// SHUTTLE //////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	void Move_UpperShuttle_LoadPos();
	BOOL Chk_UpperShuttle_LoadPos();
	void Move_UpperShuttle_UnLoadPos();
	BOOL Chk_UpperShuttle_UnLoadPos();
	void Move_LowerShuttle_LoadPos();
	BOOL Chk_LowerShuttle_LoadPos();
	void Move_LowerShuttle_UnLoadPos();
	BOOL Chk_LowerShuttle_UnLoadPos();

	void UpperShuttle_VacuumOn(const BOOL bVacOn, JIG_CH Ch );
	void UpperShuttle_BlowOff( JIG_CH Ch );
	void LowerShuttle_VacuumOn(const BOOL bVacOn, JIG_CH Ch );
	void LowerShuttle_BlowOff( JIG_CH Ch );

	//20170205 byskcho
	void LowerShuttle_BlowOn( JIG_CH Ch );

	void UpperShuttle_BlowOn( JIG_CH Ch );

	BOOL UpperShuttle_VacuumChk(JIG_CH Ch ,VAC_STATE Vac);
	BOOL LowerShuttle_VacuumChk(JIG_CH Ch ,VAC_STATE Vac);

	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// Pre-align Loader TR //////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	void Move_PATR_YAxis_GetPos();
	BOOL Chk_PATR_YAxis_GetPos();
	void Move_PATR_YAxis_PutPos();
	BOOL Chk_PATR_YAxis_PutPos();
	void Move_PATR_YAxis_WaitPos();
	BOOL Chk_PATR_YAxis_WaitPos();

	void Move_PATR_ZAxis_GetUpperShuttlePos();
	BOOL Chk_PATR_ZAxis_GetUpperShuttlePos();
	void Move_PATR_ZAxis_GetLowerShuttlePos();
	BOOL Chk_PATR_ZAxis_GetLowerShuttlePos();
	void Move_PATR_ZAxis_UpPos(double dSpeed = 0.0);
	BOOL Chk_PATR_ZAxis_UpPos();
	void Move_PATR_ZAxis_PutPos();
	BOOL Chk_PATR_ZAxis_PutPos();

	void PATR_Vacuum_On(const BOOL bVacOn,JIG_CH Ch);
	//2017-11-20,YMG BLOW ON 함수
	void PATR_BlowOn(JIG_CH Ch);
	void PATR_BlowOff(JIG_CH Ch);
	BOOL PATR_Vacuum_Chk(JIG_CH Ch,VAC_STATE Vac);

	/////////////////////////////////////////////////////////////////////////
	// PREALIGN
	/////////////////////////////////////////////////////////////////////////
	void PAXPos(JIG_CH nCh, double dbOffset = 0.);
	BOOL PAXPosChk(JIG_CH nCh, double dbOffset = 0.);
	void PAYPos(JIG_CH nCh, double dbOffset = 0.);
	BOOL PAYPosChk(JIG_CH nCh, double dbOffset = 0.);
	void PATPos(JIG_CH nCh, double dbOffset = 0.);
	BOOL PATPosChk(JIG_CH nCh, double dbOffset = 0.);

	void PAStageLoadPos();
	BOOL PAStageLoadPosChk();
	void PAStageAlignPos();
	BOOL PAStageAlignPosChk();
	void PAStageUnloadPos();
	BOOL PAStageUnloadPosChk();

	void PAStageVac(BOOL bVacOn, JIG_CH Ch);
	//2017-11-20,YMG BLOW ON
	void PAStage_BlowOn(JIG_CH Ch);
	void PAStage_BlowOff(JIG_CH Ch);
	BOOL PAStageVacChk(JIG_CH Ch,VAC_STATE Vac);

	void PA_SendAlignStart( BOOL bTheta );

	ALIGN_RESULT CellData_GetPreAlignState(JIG_CH nCh);
	void CellData_SetPreAlignState(JIG_CH nCh, ALIGN_RESULT Result);
	void CellData_SetPreAlignStateXY( JIG_CH nCh, ALIGN_RESULT nState, double dX, double dY, double dScoreL, double dScoreR );
	void CellData_SetPreAlignStateT( JIG_CH nCh, ALIGN_RESULT nState, double dTh );
	double CellData_GetPreAlginX(JIG_CH nCh);
	double CellData_GetPreAlginY(JIG_CH nCh);
	double CellData_GetPreAlginT(JIG_CH nCh);
	//2018-01-22,GHLEE, 추가
	BOOL PAStageCH1FPCBSenChk(SENSOR_STATE sen);
	BOOL PAStageCH2FPCBSenChk(SENSOR_STATE sen);

	/////////////////////////////////////////////////////////////////////////
	// LD TR
	/////////////////////////////////////////////////////////////////////////
	void LDTRXGetPos();
	BOOL LDTRXGetPosChk();
	void LDTRXPutPos();
	BOOL LDTRXPutPosChk();

	void LDTR_X_LOWERMCRPos(JIG_CH nCh,double dSpeed = 0.0);
	BOOL LDTR_X_LOWERMCRPosChk(JIG_CH nCh);
	void LDTR_X_UPPERMCRPos(JIG_CH nCh);
	BOOL LDTR_X_UPPERMCRPosChk(JIG_CH nCh);

	void LDTRZGetPos();
	BOOL LDTRZGetPosChk();
	void LDTRZPutPos(double Offset = 0.);
	BOOL LDTRZPutPosChk(double Offset = 0.);
	void LDTRZUpPos(double dSpeed = 0.0);
	BOOL LDTRZUpPosChk();
	void LDTR_Z_LOWERMCRPos();
	BOOL LDTR_Z_LOWERMCRPosChk();
	void LDTR_Z_UPPERMCRPos();
	BOOL LDTR_Z_UPPERMCRPosChk();

	void LDTRVacOn(BOOL bVacOn, JIG_CH Ch);
	//2017-11-20,YMG BLOW ON
	void LDTR_BlowOn(JIG_CH Ch);
	void LDTR_BlowOff(JIG_CH Ch);
	BOOL LDTRCellVacChk(JIG_CH Ch,VAC_STATE Vac);

	void LDTRFPCBBlowOn(BOOL bBlowOn , BOOL bCh1, BOOL bCh2);

	void Trigger(MCR_UNIT Unit);
	void Trigger(MCR_UNIT Unit,JIG_CH nCh);  //2017-05-23,skcho, mcr data 채널별 저장

	CString	GetSoftTriggerData(MCR_UNIT Unit);
	int	GetSoftTriggerResult(MCR_UNIT Unit);
	CString MCR_GetLastID(JIG_CH nCh);  //2017-05-23,skcho, mcr data 채널별 저장
	BOOL SetLiveMode(BOOL bFlag,MCR_UNIT Unit);
	BOOL GetLiveMode(MCR_UNIT Unit);
	BOOL SetSoftTrigger(MCR_UNIT Unit);
	BOOL SaveResultImage(CString sFile,MCR_UNIT Unit);
	CString GetLastSavedImage(MCR_UNIT Unit);
	void SaveMCRNGImage(MCR_UNIT Unit,JIG_CH nCh);

	MCR_JUDGE CellData_UVReadingChk(MCR_UNIT Pos,JIG_CH Ch);
	void CellData_SetUVResult(MCR_UNIT Pos,JIG_CH Ch,MCR_JUDGE Success);
	
	void ContactStart(JIG_CH Ch);
	void ContactStart_BZone(JIG_CH Ch);
	void TurnOff(JIG_CH Ch);
	void DZoneOff(JIG_CH Ch);
	void Reset(JIG_CH Ch);
	void Reset_BZone(JIG_CH Ch);
	/////////////////////////////////////////////////////////////////////////
	// AMT
	/////////////////////////////////////////////////////////////////////////
	void AMTVacOn(BOOL bVacOn, JIG_ID Jig, JIG_CH Ch);
	BOOL AMTVacChk(JIG_ID Jig, JIG_CH Ch);

	void CellData_SetJIGID(JIG_ID Jig);

	CLASS_CELL CellData_GetLastClass(CELL_POS pos);

	//2017-11-17,SKCHO, JIG BLOWER ONOFF 기능 추가
	void AMTBlowerOnOff(BOOL bBlowerOnOff, JIG_ID Jig, JIG_CH Ch);
	/////////////////////////////////////////////////////////////////////////
	// ULD Buff Stage
	/////////////////////////////////////////////////////////////////////////
	void ULDBuffSTGVacOnOff(BOOL bVacOn, BOOL bCH1, BOOL bCH2);
	void ULDBuffSTG_BlowOff(BOOL bCH1, BOOL bCH2);
	BOOL ULDBuffSTGVacChk(BOOL bVacOn, BOOL bCH1, BOOL bCH2);

	void UDRobotTrayCountBitOn(int x, int y);


	void MoveUnLoadTR_ZAxis_Up_Slow(double dbOffset = 0.0);
	BOOL MoveUnLoadTR_ZAxis_Up_SlowChk(double dbOffset = 0.0);

	/////////////////////////////////////////////////////////////////////////
	//G-AMT TRAY Create.
	//////////////////////////////////////////////////////////////////////////
	// OKTRAY
	/////////////////////////////////////////////////////////////////////////

	// GOOD TRAY IN LOAD부
	void InTrayLiftZ_UpPos   ();
	BOOL InTrayLiftZ_UpChk   ();
	void InTrayLiftZ_DownPos ();
	BOOL InTrayLiftZ_DownChk ();
	void InTrayLiftZ_PitchUp (double dPitch);
	void InTrayLiftZ_PitchUpRetry();
	void InTrayLiftZ_PitchUpStop();
	void InTrayLiftZ_PitchDown (double dPitch);
	void InTrayLiftZ_PitchDownRetry();
	BOOL InTrayLiftZ_StopChk ();
	//20180816wjp
	void OutTrayLiftZ_PitchUpRetry();
	void OutTrayLiftZ_PitchUpStop();
	////////
	//2017-12-21,SKCHO,추가
	void InTrayLiftZ_AlignPos();
	BOOL InTrayLiftZ_AlignPosChk();
	//2018wjp
	void InTrayLiftZ_TopPos();
	BOOL InTrayLiftZ_TopPosChk();

	//2018-03-14,MGYUN, GOOD IN LIFT 마지막 트레이 감지 센서 체크 함수
	BOOL InTrayLiftZ_FinalSenChk(SENSOR_STATE sen);


	BOOL InTrayAlignTrayChk   (SENSOR_STATE sen);

	BOOL InTrayChk   (SENSOR_STATE sen);

	BOOL LDTrayLiftTrayChk( SENSOR_STATE sen );
	BOOL ULDTrayLiftTrayChk( SENSOR_STATE sen );

	BOOL InConv_TrayChk(E_CONV_SEN_INDEX index, SENSOR_STATE sen);

	//2017-09-17,SKCHO
	BOOL BuffConv_TrayChk( E_CONV_SEN_INDEX index, SENSOR_STATE sen );

	// GOOD TRAY OUT UnLOAD부
	void OutTrayLiftZ_UpPos     ();
	BOOL OutTrayLiftZ_UpChk     ();
	void OutTrayLiftZ_PitchUp (double dPitch);
	void OutTrayLiftZ_PitchDown	 (double dPitch);
	void OutTrayLiftZ_DownPos   ();
	BOOL OutTrayLiftZ_DownChk   ();
	BOOL OutTrayLiftZ_StopChk ();

	void OutTrayLiftZ_AlignPos   ();
	BOOL OutTrayLiftZ_AlignChk   ();

	BOOL OutConv_TrayChk(E_CONV_SEN_INDEX index, SENSOR_STATE sen);

	// GOOD TRAY TR부
	void TrayTr_VacOnOff (VAC_STATE vac);
	void TrayTr_BlowOff();
	BOOL TrayTr_VacChk  (VAC_STATE vac);

	void TrayTrY_InPos ();
	void TrayTrY_OutPos();
	BOOL TrayTrY_InChk ();
	BOOL TrayTrY_OutChk();

	void TrayTrZ_UpPos (double dSpeed = 0.0);
	void TrayTrZ_InPos (double dOffset = 0.0);
	void TrayTrZ_OutPos();
	BOOL TrayTrZ_UpChk ();
	BOOL TrayTrZ_InChk (double dOffset =0.0);
	BOOL TrayTrZ_OutChk();

	BOOL TrayLiftOVerChk(TRAY_PORT Port);

	/////////////////////////////////////////////////////////////////////////
	// NGTRAY
	/////////////////////////////////////////////////////////////////////////

	// NG TRAY IN LOAD부.
	void InNGTrayLiftZ_UpPos   ();
	BOOL InNGTrayLiftZ_UpChk   ();
	void InNGTrayLiftZ_DownPos ();
	BOOL InNGTrayLiftZ_DownChk ();
	void InNGTrayLiftZ_MidPos ();
	BOOL InNGTrayLiftZ_MidPosChk ();
	BOOL InNGTrayLiftZ_MidPosLowChk ();
	void InNGTrayLiftZ_PitchUp (double dPitch);
	void InNGTrayLiftZ_PitchDown (double dPitch);
	void InNGTrayLiftZ_PitchDownRetry();
	void InNGTrayLiftZ_PitchUpRetry();
	void InNGTrayLiftZ_PitchUpStop();

	BOOL InNGTrayLiftZ_StopChk ();

	// NG TRAY OUT UNLOAD부.
	void OutNGTrayLiftZ_UpPos     ();
	BOOL OutNGTrayLiftZ_UpChk     ();
	void OutNGTrayLiftZ_PitchUp (double dPitch);
	void OutNGTrayLiftZ_PitchDown	 (double dPitch);
	void OutNGTrayLiftZ_DownPos   ();
	BOOL OutNGTrayLiftZ_DownChk   ();
	BOOL OutNGTrayLiftZ_StopChk ();

	// NG TRAY CONVEYOR부.
	BOOL NGConv_TrayChk(TRAY_PORT Port, E_CONV_SEN_INDEX index, SENSOR_STATE sen);

	// NG TRAY TR부.
	void NGTrayTr_VacOnOff (VAC_STATE vac);
	void NGTrayTr_BlowOff();
	BOOL NGTrayTr_VacChk  (VAC_STATE vac);

	void NGTrayTrX_InPos ();
	void NGTrayTrX_OutPos();
	BOOL NGTrayTrX_InChk ();
	BOOL NGTrayTrX_OutChk();

	void NGTrayTrZ_UpPos (double dSpeed = 0.0);
	void NGTrayTrZ_InPos (double dOffset =0.0);
	void NGTrayTrZ_OutPos();
	BOOL NGTrayTrZ_UpChk ();
	BOOL NGTrayTrZ_InChk (double dOffset =0.0);
	BOOL NGTrayTrZ_OutChk();


	// TRAY I/O CONTROL부.
	void AlignCylFwdBwd(TRAY_PORT Port,CYL_STATE Cyl);
	BOOL AlignCylFwdBwdChk(TRAY_PORT Port,CYL_STATE Cyl);

	void DivisionCylFwdBwd(TRAY_PORT Port,CYL_STATE Cyl);
	BOOL DivisionCylFwdBwdChk(TRAY_PORT Port,CYL_STATE Cyl);

	void BottomAlignCylFwdBwd(TRAY_PORT Port,CYL_STATE Cyl);
	BOOL BottomAlignCylFwdBwdChk(TRAY_PORT Port,CYL_STATE Cyl);

	void BottomStoperUpDown(TRAY_PORT Port,CYL_STATE Cyl,PORT_POS Pos = TRAY_BUFFER); // TRUE가 버퍼 FALSE가 안쪽.
	BOOL BottomStoperUpDownChk(TRAY_PORT Port,CYL_STATE Cyl,PORT_POS Pos = TRAY_BUFFER);

	void NGLiftArmFwdBwd(TRAY_PORT Port,CYL_STATE Cyl);
	BOOL NGLiftArmFwdBwdChk(TRAY_PORT Port,CYL_STATE Cyl);

	void NGTrayUpDown(TRAY_PORT Port,CYL_STATE Cyl);
	BOOL NGTrayUpDownChk(TRAY_PORT Port,CYL_STATE Cyl);

	BOOL NGTrayInAlignTrayChk( SENSOR_STATE sen );
	BOOL NGInTrayChk( SENSOR_STATE sen );

	BOOL NGTrayLiftTrayChk( SENSOR_STATE sen );
	BOOL NGTrayOutLiftTrayChk( SENSOR_STATE sen );

	BOOL TrayExistChk(TRAY_PORT Port);

	void MuteOn(ONOFF bOn,TRAY_PORT Port);

	void TrayAlignStart(CString strPort);
	BOOL TrayAlignChk(CString strPort);
	void TrayAlignTeaching(CString strPort);

	void DCRTrigger(BOOL On);
	BOOL DCRChk();
	///////AGV
	void ConvRun(TRAY_PORT Port,ONOFF bOn,CWCCW Cw = CW);
	BOOL BufConvSenChk(TRAY_PORT Port,SENSOR_STATE sen);
	void BufConvRun(TRAY_PORT Port,ONOFF bOn,CWCCW Cw = CW);
	BOOL TrayReqSwChk(TRAY_PORT Port);
	BOOL TrayCompSwChk(TRAY_PORT Port);
	BOOL TrayMuteChk(TRAY_PORT Port);
	BOOL TrayMuteSWChk(TRAY_PORT Port);
	void TrayMuteTimeDelay1(TRAY_PORT Port);
	void TrayMuteTimeDelay2(TRAY_PORT Port);
	CStopWatch m_GoodInMuteTimer1;
	CStopWatch m_GoodInMuteTimer2;
	CStopWatch m_GoodOutMuteTimer1;
	CStopWatch m_GoodOutMuteTimer2;
	CStopWatch m_NGMuteTimer1;
	CStopWatch m_NGMuteTimer2;

	BOOL AGVConnectionChk(TRAY_PORT Port);
	BOOL AGVRequestChk(TRAY_PORT Port);
	BOOL AGVBusyChk(TRAY_PORT Port);
	BOOL AGVCompChk(TRAY_PORT Port);
	void SendAGVReq(TRAY_PORT Port, ONOFF bOn);
	void SendAGVConnection(TRAY_PORT Port, ONOFF bOn);
	void SendAGVStart(TRAY_PORT Port, ONOFF bOn);
	void SendAGVComp(TRAY_PORT Port, ONOFF bOn);

	//2017-08-30,SKCHO, 신규 pio
	void SendAGVPortType(TRAY_PORT Port);
	void SendAGVAbort(TRAY_PORT Port,ONOFF bOn);

	// Product Data
	void SetProductData(CELL_POS pos);
	// Main Tact
	void TactUpdate(int Tact);
	float CountTactTime(SYSTEMTIME StartTime, SYSTEMTIME EndTime);

	// FDC Offset Loading
	void LoadADBoardOffset();

	// Motor All Stop
	void MotorAllStop();

	// 트레이 배출 경고 Buzzer 추가. 17/04/26 HSK.
	void TrayOutBuzzer(BOOL bLotEnd , ONOFF bOn);

	//2017-05-23,skcho, GOOD IN Align TRAY/ NG IN Align TRAY가 비었을 경우 알람
	void TrayInBuzzer(int nType,ONOFF bOn);

	//2017-08-22 KAKAKA
	void TrayInDelayBuzzer(int nType,ONOFF bOn);

	// 네트워크 체크 2017/04/30 HSK.
	BOOL NetWorkCheck();

	//2017-06-03,SKCHO,환경안전 요청사항 - 모터 컨트롤에서 원점 잡을 경우 뮤팅 램프 안되게
	BOOL OriginStatusCheck();

	void DisplayAGVMsg();
	int GetMachinetoLampState();  //설비 상태를 램프 상태로 변환

	//2017-09-12,SKCHO, PORTEMPTY MSG
	void DisplayPortEmptyMsg();	
	//2017-11-17, YMG, DLG HIDE
	void HideAGVMsg();
	//2017-10-07,SKCHO,
	void InitBufferIOState();
	void VacuumIOState();
	BOOL GetInPutIOState(INPUT_ID nID);
	BOOL GetOutPutIOState(OUTPUT_ID nID);

	void DoorInPutIOState(); 
	void LCInPutIOState();
	void AGVSendIOState();
	void AGVReceiveIOState();
	void AutoTeachKeyIOState();
	void HybridConvIOState();
	//LIGHT CURTAIN 상태 추가
	BOOL m_bGoodInLightCurtain;
	BOOL m_bGoodOutLightCurtain;
	BOOL m_bNgInLightCurtain;

	BOOL m_bOldGoodInLightCurtain;
	BOOL m_bOldGoodOutLightCurtain;
	BOOL m_bOldNgInLightCurtain;

	//2017-12-27,KGLEE, MAIN POWER 상태 추가
	BOOL m_bMainPower;
	BOOL m_bOldMainPower;

	//2018-04-01,GHLEE, FDC 오보고 관련 테스트
	void SetFDCCellInfo(CELL_POS nPos);
	
	//2018-05-16,MGYUN, AGV 효율 관련해서 딜레이 함수 추가 
	void WaitDelayTime(DWORD dwMillisecond); //Sleep을 사용하면 다른 문제가 발생 할 수있어 추가

	/////////////////////////////////////////////////////////////////////////
	// HYBRID CONV	//2018-05-10,MGYUN,추가
	/////////////////////////////////////////////////////////////////////////
	// 보내는 
	void SendHybRobotReady(ONOFF bOn);
	void SendHybRobotRun(ONOFF bOn);
	void SendHybSendComplete(ONOFF bOn);
	// 받는 부분
	BOOL RcvHybConvReadyChk();
	BOOL RcvHybConvRunChk();
	BOOL RcvHybConvInterfaceChk();
};

// State용이 아니라 그냥 UI 등에서 유닛 제어용으로 만든 Class
class CUnitCtrlFunc : 	public CUnitCtrlBank
{
public:
	CUnitCtrlFunc(MP2100_THREAD nThreadID) : CUnitCtrlBank(STATE_FUNC, nThreadID)
	{
	};
	~CUnitCtrlFunc(void) {};

	int Run() {return 0;};
};

