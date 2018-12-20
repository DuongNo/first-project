#pragma once
#include "BankGuild\MCDefine.h"
#include "BankGuild\CellLogItem.h"

#include "File\GxIni.h"
#include "File\GxLogData.h"			// Cell Log�� ���� class



typedef struct DETAIL_REJECT
{
	CString strName;
	CString strXY;
}DETAILREJECT;

//Loader�� Data class
class cLoader
{
public:
		//READ_UNIT
		bool m_bAlignSuccess;	//L_ALIGN_SUCCESS : Loader���� Reading �������� ��� : GOOD/NG
		//JIG_NAME
		//CH_NAME
		//CONTECT Count : CELL_LOADING,GOOD/NG ���� Ƚ�� ����
		//UNLOADER_TEC


};

//Loader�� Data class
class cZoneA
{
public:	
	//kjpark 20161017 CellInfo �׸� �߰�(Cell Log ����)
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
	//2018-05-17,SKCHO, DIC_CHECK ���� �߰�
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

		//2018-05-17,SKCHO �߰�
		RegPinName.Empty();
		RegPinClass= NONE_CELL;

		DicCheckName.Empty();
		DicCheckClass= NONE_CELL;
	}

};

//Loader�� Data class
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

//Loader�� Data class
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

//Loader�� Data class
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

// Cell Data ����ü Class
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
	//kjpark 20161017 CellInfo �׸� �߰�(Cell Log ����)
	CString m_strInnerID;//INNER ID
	CString m_strModel;		//AMS549MD01 MODEL ������
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
	CString m_strUnload_Out_Tack_Time;//Unload Picker Vacuum OFF �ð� ���
	//BIN2 CELL INFO<- Not Used
	CString m_strBIN2_CELL_INFO;
	
	CString m_strPG_UI_Version;//PG_UI:��ȣ�� Client ����. �ݵ�� ���
	CString m_strPG_Program_Name;//PROGRAM NAME
	CString m_strPG_MAC_Address;//CH Card MAC

	int m_nJigNO;//JIG NO
	int m_nCIMLog;//CIM_LOG
	int m_nCIMMTP;//CIM_MTP
	int m_nCIMTsp;//CIM_TSP

	CString strReadID;

	CString m_strTotalTackTime;//���� Unloader Picker Vacuum OFF ����~Unloader Picker Vacuum OFF ����
	CString m_strCellLoadTackTime;//CELL LOAD TACT:Loader Picker Vacuum ON ����~Loader Picker Vacuum OFF ����
	CString m_strAZone_Align_TackTime;//AZONE ALIGN TACT TIME:Align Camera ���� ���� ~ Align ����� ����
	int m_nAZone_Align_Degree;//AZONE ALIGN DEGREE:Align ������
	CLASS_DECISION m_AZoneAlignSuccess;//AZONE ALIGN SUCCESS:Align P/F �����
	CString m_strCell_Unload_TackTime;//CELL UNLOAD TACT

	CLASS_CELL m_LastClass;
	CString m_LastResult;
	CString PrintDefect;//PrintName
	CString PrintCode;
	CString MesCode;
	////////////////////////////////////////////////////////////
		
	DOUBLE m_dAlign_Correction_X;//Aglign ����ġ
	DOUBLE m_dAlign_Correction_Y;
	DOUBLE m_dAlign_Correction_T;	
	//kjpark 20161016 Cell Result �׸� �߰�
	int FinalJudge;	
	CString FinalDefect;
	cZoneA m_Zone_A;
	cZoneB m_Zone_B;
	cZoneC m_Zone_C;
	cZoneD m_Zone_D;
	
	TACK_TIME_CELL cellTime;

	//kjpark 20161021 Main Form Cell Status view ����
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

	CString m_strPPID;  //2017-03-09, skcho, cell ������ ppid  �߰�

	//20170201 byskcho
	BOOL   bCellInfoRequest;
	BOOL   bTrackInChk;
	BOOL   bTrackOutChk;

	CString   sAbstract;         //2017-03-02,skcho , TRACK IN �Ϸῡ ���� CELL�� ���� ���������� �ϰڴٴ� ���� "1"-��������/cim 406���� JUDGE "O"�� ����

	// Cell Log [11/16/2016 OSC]
	CList <CCellLogItem, CCellLogItem> m_listCellLogItem;	// �̰� Init�� �־��� �ʿ䰡 ���� [11/16/2016 OSC]
	void LinkCellLogItem();
	CString GetCellLogTitle();
	CString GetCellLogValue();

	// Tact ����.
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
		//20170118 byskcho ���ҽ� ��Ʈ������ �����ϹǷ� ����
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
		//kjpark 20161021 Main Form Cell Status view ����
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

		sAbstract.Empty();  //2017-03-02,skcho , �������� �÷��� 

		m_strProductID.Empty();
		//2018-07-25,SKCHO, �ʱⰪ -1�� ����
		m_strCellInfoResult=  _T("-1");
		//m_strCellInfoResult.Empty();

		strReadID = _T("1");
		m_strContactResult = _T("NONE_CELL");
		m_strContactResult_1st = _T("NONE_CELL");

		m_nContactCnt = 0;  //���� ���� Ƚ�� ���� ����

		LoadTact = 0.0;
		UnLoadTact = 0.0;
		AZoneTact = 0.0;
		BZoneTact = 0.0;
		CZoneTact = 0.0;
		DZoneTact = 0.0;

		m_nChSkipInfoJigID = JIG_ID_A;

		//2017-05-19,skcho, RCMD ��� ����
		m_bRCMDResult = FALSE;
	}	
	
};

// CCellInfo m_Cell[MAX_CELL_INFO_BUFFER]�� Index �����
// ���񿡼� Cell�� �̼۵� �� �� Unit�� Cell Index�� �����Ͽ� �����Ѵ�
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

	// Cell ������ �߹��� Index ��ȣ
	int m_nNextGenIndex;
	CCellInfo m_Cell[MAX_CELL_INFO_BUFFER];

	// ����Ү Cell�� ���� ������ ��� ��ġ��ŭ ���۷� ����� ����
	// ���۳��� ���� tag�� �ְ�޴´�
	CCellTag m_CellTag[CELL_POS_MAX];

	CCellTag GetCellTag(CELL_POS pos);

	CCellTag GetCellTag(ZONE_ID zone, JIG_CH nCh);				// �˻�ο��� ���� ���� ���ϰ� ID, Ch�� ���� �Լ� ����

	// Ư�� �������� �ش��ϴ� CellTag�� ã�� �Լ�
	CCellTag SearchCellTag(JIG_ID nJig, JIG_CH nCh, CELL_POS startPos = CELL_POS_AZONE_CH1, CELL_POS endPos = CELL_POS_DZONE_CH2);
	CCellTag SearchCellTag(CString strInnerID, CELL_POS startPos = CELL_POS_AZONE_CH1, CELL_POS endPos = CELL_POS_DZONE_CH2);

	void SetCellTag(CELL_POS pos, CCellTag tag);
	void SetCellTag(ZONE_ID zone, JIG_CH nCh, CCellTag tag);	// �˻�ο��� ���� ���� ���ϰ� ID, Ch�� ���� �Լ� ����
	//2018wjp
	void SetTrayCellTag(CELL_POS pos, int iRow,int iCol);

	CCellTag RemoveCellTag(CELL_POS pos);

	//2017-07-15,SKCHO, CELLINFO�� ����� ���� �Լ�, ���� ��ư������ ���
	CCellTag CCellBank::RemoveCellTagCellInfo( CELL_POS pos );

	//kjpark 20161106 CellRemove ����
	CCellTag RemoveCellTag( ZONE_ID zone, JIG_CH nCh);

	CCellTag CreateCellTag();
	void CreateCellInfo(CELL_POS pos);

	// CellTag�� ������� CellInfo�� ��ȯ�ϴ� �Լ�
	CCellInfo* GetCellInfo(CCellTag tag);
	CCellInfo* GetCellInfo(CELL_POS pos );
	CCellInfo* GetCellInfo(ZONE_ID zone, JIG_CH nCh);
	CCellInfo* SearchCellInfo(JIG_ID nJig, JIG_CH nCh, CELL_POS startPos = CELL_POS_AZONE_CH1, CELL_POS endPos = CELL_POS_DZONE_CH2);
	CCellInfo* SearchCellInfo(int nJig, int nCh, CELL_POS startPos = CELL_POS_AZONE_CH1, CELL_POS endPos = CELL_POS_DZONE_CH2);

	//kjpark 20161018 �� Tag shift
	CELL_POS GetCellPosition(CCellTag tag);
	void SetCellTagNextPosition(CELL_POS CurrentPos, CELL_POS NextPos);
	//kjpark 20161106 CellRemove ����
	void SetCellTagNextPositionZone(ZONE_ID beforeZone, ZONE_ID nextZone);

	CFile m_fileCellData;
	void WriteCellLog(CELL_POS pos);

	CFile m_fileCellData_Temp;
	void WriteCellLog_Temp(CELL_POS pos);  //2017-03-04,skcho Cell log ���� ���� ���� �߰�

	void WriteTrackOutInfo(CELL_POS pos);


	//CGxLogData m_logCellData;
	//void InitialCell_DataFile();
};

