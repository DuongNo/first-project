#pragma once
#include "BankGuild\MCDefine.h"
#include "BankGuild\CellLogItem.h"

#include "File\GxIni.h"
#include "File\GxLogData.h"			// Cell Log를 위한 class



typedef struct DETAIL_REJECT
{
	CString strName;
	CString strXY;
}DETAILREJECT;

//Loader부 Data class
class cLoader
{
public:
		//READ_UNIT
		bool m_bAlignSuccess;	//L_ALIGN_SUCCESS : Loader에서 Reading 성공여부 기록 : GOOD/NG
		//JIG_NAME
		//CH_NAME
		//CONTECT Count : CELL_LOADING,GOOD/NG 받은 횟수 기준
		//UNLOADER_TEC


};

//Loader부 Data class
class cZoneA
{
public:	
	//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
	////////////////////////////////////////////////////////////
	//Cell LOG Part
	////////////////////////////////////////////////////////////
	CLASS_CELL	AZoneClass;
	CString		AZoneDefect;

	CString		ResultCellLoadingName;
	CLASS_CELL	CellLoadingClass;
	double		CellLoadingCurrent[6];
	CString		m_strCellLoading_IBAT;
	CString		m_strCellLoading_IVSS;
	CString		m_strCellLoading_IVDD;
	CString		m_strCellLoading_IVCI;
	CString		m_strCellLoading_IBAT2;
	CString		m_strCellLoading_IDD2;	


	CString		ContactCurrentName;
	CLASS_CELL	ContactCurrentClass;
	double		ContactCurrent[6];
	CString		m_strContactCurrent_IBAT;
	CString		m_strContactCurrent_IVSS;
	CString		m_strContactCurrent_IVDD;
	CString		m_strContactCurrent_IVCI;
	CString		m_strContactCurrent_IBAT2;
	CString		m_strContactCurrent_IDD2;	

	TMD_INFO_STATUS m_bReceive_TMD_Info;
	CString 	m_strTMD_Info;
	//2017-10-09,SKCHO	
	CString     m_strProgram1;
	CString     m_strProgram2;
	CString     m_strProgram3;
	CString     m_strProgram4;
	CString     m_strProgram5;
	//2018-05-17,SKCHO, DIC_CHECK 판정 추가
	CString		RegPinName;
	CLASS_CELL	RegPinClass;

	CString		DicCheckName;
	CLASS_CELL	DicCheckClass;

	void	Init()
	{
		AZoneClass = NONE_CELL;
		AZoneDefect.Empty();

		m_bReceive_TMD_Info = TMD_INFO_NOT_RECEIVE;
		m_strTMD_Info.Empty();

		//2017-10-09,SKCHO	
		m_strProgram1.Empty();
		m_strProgram2.Empty();
		m_strProgram3.Empty();
		m_strProgram4.Empty();
		m_strProgram5.Empty();

		ContactCurrentName.Empty();
		CellLoadingClass = NONE_CELL;
		for(int i = 0; i < 6; i++)
		{
			CellLoadingCurrent[i] = 0.;
		}
		m_strCellLoading_IBAT.Empty();
		m_strCellLoading_IVSS.Empty();
		m_strCellLoading_IVDD.Empty();
		m_strCellLoading_IVCI.Empty();
		m_strCellLoading_IBAT2.Empty();
		m_strCellLoading_IDD2.Empty();

		ResultCellLoadingName.Empty();
		ContactCurrentClass = NONE_CELL;
		for(int i = 0; i < 6; i++)
		{
			ContactCurrent[i] = 0.;
		}
		m_strContactCurrent_IBAT.Empty();
		m_strContactCurrent_IVSS.Empty();
		m_strContactCurrent_IVDD.Empty();
		m_strContactCurrent_IVCI.Empty();
		m_strContactCurrent_IBAT2.Empty();
		m_strContactCurrent_IDD2.Empty();

		//2018-05-17,SKCHO 추가
		RegPinName.Empty();
		RegPinClass= NONE_CELL;

		DicCheckName.Empty();
		DicCheckClass= NONE_CELL;
	}

};

//Loader부 Data class
class cZoneB
{
public:
	////////////////////////////////////////////////////////////
	//Cell LOG Part
	////////////////////////////////////////////////////////////
	CLASS_CELL	BZoneClass;
	CString		BZoneDefect;
	ALIGN_RESULT	m_nVI_AlignResult;
	double		m_dVI_AlignScore[E_MARK_MAX];
	double		m_dVI_AlignTH;	
	////////////////////////////////////////////////////////////
	void	Init()
	{
		BZoneClass = NONE_CELL;
		BZoneDefect.Empty();
		m_nVI_AlignResult = ALIGN_RESULT_NONE;
		m_dVI_AlignScore[E_MARK_LEFT] = 0.;
		m_dVI_AlignScore[E_MARK_RIGHT] = 0.;
		m_dVI_AlignTH = 0.;
	}

};

//Loader부 Data class
class cZoneC
{
public:
	CLASS_CELL CZoneClass;
	CString		CZoneDefect;
	INSP_STATE	InspVIState;
	CLASS_CELL	VIClass;
	CString		VIDefect;
	int			VIClassJudge;
	double		GrabTime;
	double		ClassTime;
	//2017-09-24,SKCHO, REJECT COUNT
	int         VIRejectCount;
	DETAILREJECT stRejct[MAX_REJECT_CNT];

	void	Init()
	{
		CZoneClass = NONE_CELL;
		CZoneDefect.Empty();
		InspVIState = eSTATE_Insp_None;
		VIClass = NONE_CELL;
		VIClassJudge = 2;
		VIDefect.Empty();	
		VIRejectCount =0;
		//2017-09-24,SKCHO, REJECT COUNT
		for(int i = 0; i < MAX_REJECT_CNT; i++)
		{
			stRejct[i].strName = _T("");
			stRejct[i].strXY = _T("");		
		}
		GrabTime = 0.0;
		ClassTime = 0.0;
	}
};

//Loader부 Data class
class cZoneD
{
public:
	CLASS_CELL	DZoneClass;
	CString		DZoneDefect;

	int			m_nTray;
	BOOL		TurnOffChk;
	////////////////////////////////////////////////////////////

	void	Init()
	{
		DZoneClass =NONE_CELL;
		DZoneDefect.Empty();
		m_nTray = 0;
		TurnOffChk = FALSE;
	}

};

// Cell Data 구조체 Class
class CCellInfo
{
public:

	CCellInfo(void)
	{
		Init();
		LinkCellLogItem();
	}
	~CCellInfo(void){}

	////////////////////////////////////////////////////////////
	//Cell LOG Part
	////////////////////////////////////////////////////////////
	//kjpark 20161017 CellInfo 항목 추가(Cell Log 기준)
	CString m_strInnerID;//INNER ID
	CString m_strModel;		//AMS549MD01 MODEL 레시피
	CString m_strCellID;//CELL ID
	CString m_strReadUnitMCR;//READ UNIT
	JIG_ID m_JigId;
	JIG_CH m_JigCh;
	int m_nContactCount;
	CString m_strOperation_PC_Version;//MC_Ver
	CString m_strData_PC_Version;//D_Ver	
	CString m_strUNIATS_VERSION;
	CString m_strMICRO_DLL_VERSION;
	CString m_strMACRO_DLL_VERSION;
	CString	m_strSETBIN_VERSION;	
	CString m_strUnload_Out_Tack_Time;//Unload Picker Vacuum OFF 시각 기록
	//BIN2 CELL INFO<- Not Used
	CString m_strBIN2_CELL_INFO;
	
	CString m_strPG_UI_Version;//PG_UI:신호기 Client 버전. 반드시 기록
	CString m_strPG_Program_Name;//PROGRAM NAME
	CString m_strPG_MAC_Address;//CH Card MAC

	int m_nJigNO;//JIG NO
	int m_nCIMLog;//CIM_LOG
	int m_nCIMMTP;//CIM_MTP
	int m_nCIMTsp;//CIM_TSP

	CString strReadID;

	CString m_strTotalTackTime;//이전 Unloader Picker Vacuum OFF 시점~Unloader Picker Vacuum OFF 시점
	CString m_strCellLoadTackTime;//CELL LOAD TACT:Loader Picker Vacuum ON 시점~Loader Picker Vacuum OFF 시점
	CString m_strAZone_Align_TackTime;//AZONE ALIGN TACT TIME:Align Camera 동작 시작 ~ Align 결과값 수신
	int m_nAZone_Align_Degree;//AZONE ALIGN DEGREE:Align 조정값
	CLASS_DECISION m_AZoneAlignSuccess;//AZONE ALIGN SUCCESS:Align P/F 결과값
	CString m_strCell_Unload_TackTime;//CELL UNLOAD TACT

	CLASS_CELL m_LastClass;
	CString m_LastResult;
	CString PrintDefect;//PrintName
	CString PrintCode;
	CString MesCode;
	////////////////////////////////////////////////////////////
		
	DOUBLE m_dAlign_Correction_X;//Aglign 보정치
	DOUBLE m_dAlign_Correction_Y;
	DOUBLE m_dAlign_Correction_T;	
	//kjpark 20161016 Cell Result 항목 추가
	int FinalJudge;	
	CString FinalDefect;
	cZoneA m_Zone_A;
	cZoneB m_Zone_B;
	cZoneC m_Zone_C;
	cZoneD m_Zone_D;
	
	TACK_TIME_CELL cellTime;

	//kjpark 20161021 Main Form Cell Status view 구현
	BOOL m_bIsInspectionSkip;

	/////////////////////////////////////////////
	///G AMT
	double LoaderAlignDataX;
	double LoaderAlignDataY;
	double LoaderAlignDataT;
	ALIGN_RESULT LoaderAlignResult;

	double Pre_Align_Data_x;
	double Pre_Align_Data_y;
	double Pre_Align_Data_th;
	double Pre_AlignScore[E_MARK_MAX];
	ALIGN_RESULT Pre_AlignResultT;
	ALIGN_RESULT Pre_AlignResultXY;

	BOOL	bUVReadingCheck;
	MCR_JUDGE	bUVReadingSuccess;

	
	// CIM [11/26/2016 OSC]
	CString m_strProductID;
	CString m_strCellInfoResult;

	CString m_strPPID;  //2017-03-09, skcho, cell 정보에 ppid  추가

	//20170201 byskcho
	BOOL   bCellInfoRequest;
	BOOL   bTrackInChk;
	BOOL   bTrackOutChk;

	CString   sAbstract;         //2017-03-02,skcho , TRACK IN 완료에 대한 CELL에 대해 임의추출을 하겠다는 변수 "1"-임의추출/cim 406에서 JUDGE "O"로 보냄

	// Cell Log [11/16/2016 OSC]
	CList <CCellLogItem, CCellLogItem> m_listCellLogItem;	// 이건 Init에 넣어줄 필요가 없다 [11/16/2016 OSC]
	void LinkCellLogItem();
	CString GetCellLogTitle();
	CString GetCellLogValue();

	// Tact 관련.
	double LoadTact;
	double UnLoadTact;
	double AZoneTact;
	double BZoneTact;
	double CZoneTact;
	double DZoneTact;

	//20170206 byskcho
	CString m_strEdgeResult;
	CString m_strContactResult;
	CString m_strContactResult_1st;

	int m_nContactCnt;

	JIG_ID m_nChSkipInfoJigID;

	BOOL m_bRCMDResult;

	void Init()
	{
		m_strInnerID.Empty();
		m_strModel.Empty();
		m_strCellID.Empty();
		m_strReadUnitMCR = _T("NONE");
		m_JigId = JIG_ID_A;
		m_JigCh = JIG_CH_1;
		m_nContactCount = 0;
		//20170118 byskcho 리소스 스트링에서 관리하므로 삭제
		//m_strOperation_PC_Version = PROGRAM_VERSION;
		m_strData_PC_Version.Empty();
		m_strUnload_Out_Tack_Time.Empty();
		m_strUNIATS_VERSION.Empty();
		m_strMICRO_DLL_VERSION.Empty();
		m_strMACRO_DLL_VERSION.Empty();
		m_strSETBIN_VERSION.Empty();
		m_strBIN2_CELL_INFO = _T("G");
		m_strPG_UI_Version.Empty();
		m_strPG_Program_Name.Empty();
		m_strPG_MAC_Address.Empty();
		m_nJigNO = 0;
		m_nCIMLog = 0;
		m_nCIMMTP = 0;
		m_nCIMTsp = 0;
		m_strTotalTackTime.Empty();
		m_strCellLoadTackTime.Empty();
		m_strAZone_Align_TackTime.Empty();
		m_dAlign_Correction_X = 0.0;
		m_dAlign_Correction_Y = 0.0;
		m_dAlign_Correction_T = 0.0;
		m_nAZone_Align_Degree = 0;
		//kjpark 20161021 Main Form Cell Status view 구현
		m_AZoneAlignSuccess = DECISION_PASS;
		m_strCell_Unload_TackTime.Empty();


		FinalJudge = 0;
		m_LastResult.Empty();
		m_LastClass = NONE_CELL;		
		MesCode.Empty();
		FinalDefect.Empty();
		PrintDefect.Empty();
		PrintCode.Empty();

		m_Zone_A.Init();
		m_Zone_B.Init();
		m_Zone_C.Init();
		m_Zone_D.Init();
		cellTime.Clear();

		m_bIsInspectionSkip =  FALSE;

		//////////////////////////////////////////////////////
		//// G AMT

		LoaderAlignDataX = 0.0;
		LoaderAlignDataY = 0.0;
		LoaderAlignDataT = 0.0;
		LoaderAlignResult = ALIGN_RESULT_NONE;

		Pre_Align_Data_x = 0.;
		Pre_Align_Data_y = 0.;
		Pre_Align_Data_th = 0.;
		Pre_AlignScore[E_MARK_LEFT] = 0.;
		Pre_AlignScore[E_MARK_RIGHT] = 0.;
		Pre_AlignResultXY = ALIGN_RESULT_NONE;
		Pre_AlignResultT = ALIGN_RESULT_NONE;

		bUVReadingCheck = FALSE;
		bUVReadingSuccess = MCR_RESULT_NONE;
		bTrackInChk = FALSE;
		bCellInfoRequest = FALSE;
		bTrackOutChk = FALSE;

		sAbstract.Empty();  //2017-03-02,skcho , 임의추출 플래그 

		m_strProductID.Empty();
		//2018-07-25,SKCHO, 초기값 -1로 설정
		m_strCellInfoResult=  _T("-1");
		//m_strCellInfoResult.Empty();

		strReadID = _T("1");
		m_strContactResult = _T("NONE_CELL");
		m_strContactResult_1st = _T("NONE_CELL");

		m_nContactCnt = 0;  //컨택 실행 횟수 저장 변수

		LoadTact = 0.0;
		UnLoadTact = 0.0;
		AZoneTact = 0.0;
		BZoneTact = 0.0;
		CZoneTact = 0.0;
		DZoneTact = 0.0;

		m_nChSkipInfoJigID = JIG_ID_A;

		//2017-05-19,skcho, RCMD 결과 저장
		m_bRCMDResult = FALSE;
	}	
	
};

// CCellInfo m_Cell[MAX_CELL_INFO_BUFFER]의 Index 저장소
// 설비에서 Cell이 이송될 때 각 Unit간 Cell Index만 전달하여 관리한다
class CCellTag
{
public:
	int m_nIndex;
	CCellTag(void) { Init(); }
	void Init() { m_nIndex = -1; }
	BOOL IsExist() { return (m_nIndex >= 0) ?TRUE:FALSE; }
};


#define MAX_CELL_LOG_STRING			8192
#define MAX_CELL_INFO_BUFFER		50  //1000 //2017-07-17,SKCHO

class CCellBank
{
public:
	CCellBank(void);
	~CCellBank(void);

	// Cell 생성시 발번될 Index 번호
	int m_nNextGenIndex;
	CCellInfo m_Cell[MAX_CELL_INFO_BUFFER];

	// 설비內 Cell이 존재 가능한 모든 위치만큼 버퍼로 만들어 놓고
	// 버퍼끼리 서로 tag를 주고받는다
	CCellTag m_CellTag[CELL_POS_MAX];

	CCellTag GetCellTag(CELL_POS pos);

	CCellTag GetCellTag(ZONE_ID zone, JIG_CH nCh);				// 검사부에서 갖다 쓰기 편하게 ID, Ch로 새로 함수 구성

	// 특정 구간에서 해당하는 CellTag를 찾는 함수
	CCellTag SearchCellTag(JIG_ID nJig, JIG_CH nCh, CELL_POS startPos = CELL_POS_AZONE_CH1, CELL_POS endPos = CELL_POS_DZONE_CH2);
	CCellTag SearchCellTag(CString strInnerID, CELL_POS startPos = CELL_POS_AZONE_CH1, CELL_POS endPos = CELL_POS_DZONE_CH2);

	void SetCellTag(CELL_POS pos, CCellTag tag);
	void SetCellTag(ZONE_ID zone, JIG_CH nCh, CCellTag tag);	// 검사부에서 갖다 쓰기 편하게 ID, Ch로 새로 함수 구성
	//2018wjp
	void SetTrayCellTag(CELL_POS pos, int iRow,int iCol);

	CCellTag RemoveCellTag(CELL_POS pos);

	//2017-07-15,SKCHO, CELLINFO를 지우기 위한 함수, 메인 버튼에서만 사용
	CCellTag CCellBank::RemoveCellTagCellInfo( CELL_POS pos );

	//kjpark 20161106 CellRemove 삭제
	CCellTag RemoveCellTag( ZONE_ID zone, JIG_CH nCh);

	CCellTag CreateCellTag();
	void CreateCellInfo(CELL_POS pos);

	// CellTag를 기반으로 CellInfo를 반환하는 함수
	CCellInfo* GetCellInfo(CCellTag tag);
	CCellInfo* GetCellInfo(CELL_POS pos );
	CCellInfo* GetCellInfo(ZONE_ID zone, JIG_CH nCh);
	CCellInfo* SearchCellInfo(JIG_ID nJig, JIG_CH nCh, CELL_POS startPos = CELL_POS_AZONE_CH1, CELL_POS endPos = CELL_POS_DZONE_CH2);
	CCellInfo* SearchCellInfo(int nJig, int nCh, CELL_POS startPos = CELL_POS_AZONE_CH1, CELL_POS endPos = CELL_POS_DZONE_CH2);

	//kjpark 20161018 셀 Tag shift
	CELL_POS GetCellPosition(CCellTag tag);
	void SetCellTagNextPosition(CELL_POS CurrentPos, CELL_POS NextPos);
	//kjpark 20161106 CellRemove 삭제
	void SetCellTagNextPositionZone(ZONE_ID beforeZone, ZONE_ID nextZone);

	CFile m_fileCellData;
	void WriteCellLog(CELL_POS pos);

	CFile m_fileCellData_Temp;
	void WriteCellLog_Temp(CELL_POS pos);  //2017-03-04,skcho Cell log 복사 파일 생성 추가

	void WriteTrackOutInfo(CELL_POS pos);


	//CGxLogData m_logCellData;
	//void InitialCell_DataFile();
};

