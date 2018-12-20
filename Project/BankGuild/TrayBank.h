#pragma once

#define MAX_CARRIER_NUM 100
#define TRAY_RELEASE 262//256
#define TRAY_ASSIGN 260
#define TRAY_CELLID 256

enum TRAY_INFO_POS
{
	TRAY_INFO_POS_GOOD_IN_BUFFER = 0,
	TRAY_INFO_POS_GOOD_IN_LIFT,
	TRAY_INFO_POS_GOOD_IN_ALIGN,
	TRAY_INFO_POS_GOOD_OUT_BUFFER,
	TRAY_INFO_POS_GOOD_OUT_LIFT,
	TRAY_INFO_POS_NG_OUT_LIFT_ARM,
	TRAY_INFO_POS_NG_OUT_REQ_BUFFER,
	TRAY_INFO_POS_NG_OUT_COMP_BUFFER,
	TRAY_INFO_POS_NG_IN_LIFT_ARM,
	TRAY_INFO_POS_NG_IN_BUFFER,
	TRAY_INFO_POS_NG_IN_ALIGN,
	TRAY_INFO_POS_GOOD_TR,
	TRAY_INFO_POS_NG_TR,
	TRAY_INFO_POS_MAX
};

#define MAX_STRUCT 50
struct structCELLINFO
{
	CString     m_strCell_ID;    
	CString     m_strCell_Location;
	CString	    m_strReasonCode;
	CString     m_strJudge;

};


//20170206 byskcho
//CARRIER ASSIGN REQUEST용 구조체 추가
class CCarrierInfo
{
public:
	CCarrierInfo(void)
	{
		Clear();
	}
	~CCarrierInfo(void){}

	CString     m_strInner_ID;

	CString     m_strCEID;
	CString		m_strCarrier_ID;
	CString		m_strCarrier_Type;
	CString		m_strCarrier_PPID;
	CString		m_strCarrier_Product;
	CString		m_strCarrier_StepID;
	int			m_nCarrier_S_Count;
	int			m_nCarrier_C_Count;
	CString		m_strPortNo;

	CString	    m_strSubCarrier_ID;

	int	    m_nCell_Qty;

	structCELLINFO m_stCellInfo[MAX_STRUCT];
	/*CString     m_strCell_ID;    
	CString     m_strCell_Location;
	CString	    m_strReasonCode;
	CString     m_strJudge;*/
	void Clear(void)
	{
		m_strInner_ID.Empty();
		
		m_strCEID.Empty();
		m_strCarrier_ID.Empty();
		m_strCarrier_Type.Empty();
		m_strCarrier_PPID.Empty();
		m_strCarrier_Product.Empty();
		m_strCarrier_StepID.Empty();
		m_nCarrier_S_Count = 0;
		m_nCarrier_C_Count = 0;
		m_strPortNo.Empty();
		
		m_strSubCarrier_ID.Empty();
		m_nCell_Qty = 0;	
	};

};

class CTrayInfo
{
	
private:
	BOOL m_bExist;		// Tray 존재 유무
	BOOL m_bFullTray;	// Cell이 가득찬 Tray인지 여부
	int m_nXCnt;		// X로 몇칸까지 Cell이 놓여져 있는지..
	int m_nYCnt;		// Y로 몇칸까지 Cell이 놓여져 있는지..
	BOOL m_bTrayCountFull;

	int m_nCellInCount;
	
	CString m_strTrayID;
	CString m_strTrayCellID[20];


public:
	CTrayInfo(void);
	CTrayInfo(BOOL bCreate, BOOL bFull = FALSE);
	virtual ~CTrayInfo(void);

	CCarrierInfo m_pCarrierInfo;	
	
	void Init() 
	{ 
		m_bExist = FALSE; 
		m_bFullTray = FALSE;
		m_nXCnt = 0;
		m_nYCnt = 1;
		m_nTrayCount = 0;
		m_bTrayCountFull = FALSE;
		m_nCellInCount = 0;
		m_pCarrierInfo.Clear();
	
		m_strTrayID = _T("");
		for(int i = 0; i < 20; i++)
			m_strTrayCellID[i] = _T("");
	}
	int m_nTrayCount;
	BOOL IsExist()		{ return m_bExist; }
	int GetTrayCount() { return m_nTrayCount; }
	void SetTrayFull(BOOL bFull)	{ m_bTrayCountFull = bFull; }
	BOOL GetTrayFull()	{ return m_bTrayCountFull; }
	BOOL IsCellEmpty()	{ return ( (m_nXCnt == 0) && (m_nYCnt == 1) ) ? TRUE:FALSE; }
	BOOL IsCellFull()	{ return m_bFullTray; }
	int GetXCount()		{ return m_nXCnt; }
	int GetYCount()		{ return m_nYCnt; }
	BOOL IsCell()		{ return ( (m_nXCnt == 0) && (m_nYCnt == 1) ) ? FALSE:TRUE; }
	void AddCell(CELL_POS Pos );
	//2018wjp
	void AddCell(CELL_POS Pos,CString m_strCellID,int iRow,int iCol);
	void MinusCell();
	int GetCellCount() {return m_nCellInCount;}

	void SetCellID(int Pos, CString strCellID);
	//테스트 후 삭제
	void SetCellFull(){m_bFullTray = TRUE;};
	//2018wjp
	void ReSetCellFull(){m_bFullTray = FALSE;};
	

};


class CTrayBank
{
private:
	int m_nCarrierCount;
public:
	CTrayBank(void);
	~CTrayBank(void);

	void InitTrayInfo();


	CTrayInfo m_TrayInfo[TRAY_PORT_MAX][TRAY_INFO_POS_MAX];

	CTrayInfo* GetTrayInfo(TRAY_PORT port, TRAY_INFO_POS pos);
	CTrayInfo RemoveTrayInfo(TRAY_PORT port, TRAY_INFO_POS pos);
	void SetTrayInfo(TRAY_PORT port, TRAY_INFO_POS pos, CTrayInfo info);

	//20170207 byskcho
	void SetCarrierInfo(TRAY_PORT port, TRAY_INFO_POS pos,int nCEID);

	void WriteCarrierReleaseInfo(TRAY_PORT port, TRAY_INFO_POS pos); //2017-02-27, sckho, 트레이 정보 ini 파일 생성
	void WriteCarrierAssignInfo(TRAY_PORT port, TRAY_INFO_POS pos);
	void WriteCarrierCloseInfo(TRAY_PORT port, TRAY_INFO_POS pos);
	void WriteCarrierReqCellIDInfo(TRAY_PORT port, TRAY_INFO_POS pos);//2018wjp

	void ClearCarrierReleaseInfo();
	void ClearCarrierCloseInfo();
	void ClearCarrierAssignInfo();
	void ClearCarrierReqCellIDInfo();//2018wjp

	void Copy_CarrierInfo(TRAY_PORT port, TRAY_INFO_POS frompos,TRAY_INFO_POS topos);
};

