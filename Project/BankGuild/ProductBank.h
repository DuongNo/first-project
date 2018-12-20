#pragma once

#include <vector>

using namespace std;

enum WorkPart
{
	DAY = 0,
	SW,
	GY
};

struct tElement
{
	int		iValue;
	UINT	uID;
};

struct ProductInfo
{
	tElement tHour;
	tElement tCount;
	tElement tDefect;
	tElement tReject;
	tElement tDefMCR;
	tElement tDefAlign;
	tElement tDefCont;	

	//20170206 byskcho
	tElement tDefEdgeAlign;
	tElement tDefContact;	
};

//2017-12-01,SKCHO
struct tRetryInfo
{
	unsigned int		nTotal;
	unsigned int  	    nSucess;
};


class CProductBank
{
public:
	CProductBank(void);
	~CProductBank(void);

protected:
	void InitData();

public:
	void LoadReportFile(CString szTime);
	//2018-04-16,NAM, Load report for monitoring 
	void LoadReportFile1(CString szTime);
	void SaveReportFile(CStdioFile* cfile);
	void SetProductData(BOOL bSt, BOOL bMCR, BOOL bAlign, BOOL bCont);
	CString CalcPercent(int iEntire, int iValue);
	//2018-03-14 NAM, Calculater Percentage (Return Double)
	double CalcPercent1(int iEntire, int iValue);
	void	CalcAllInfo();
	//2018-03-14, NAM, to get the data 
	void   ProductDayCall();
	//2018-03-16, NAM, to get the data for 1 hour ago
	void   ProductDayCall1();

	void	InfoClear();

	void SetCurrentTime(CString strTime);
	
protected:
	ProductInfo m_DataDaySum;
	ProductInfo m_DataSwSum;
	ProductInfo m_DataGySum;
	ProductInfo m_DataWholeDay;
	
	//통계화면에서만 사용하는 변수
	vector<ProductInfo> m_VecDataSum;	
	map<short, ProductInfo>	m_MapData;	


	//2017-05-25, SKCHO  실제 카운터를 저장하는 변수 - 통계화면에서 사용하는 변수와 분리
	map<short, ProductInfo>	m_MapDataBuff;	

	CString m_strCurTime;

public:
	map<short, ProductInfo> GetMapData() const { return m_MapData; }
	void SetMapData(map<short, ProductInfo> val) { m_MapData = val; }	

	vector<ProductInfo> GetVecDataSum() const { return m_VecDataSum; }
	void SetVecDataSum(vector<ProductInfo> val) { m_VecDataSum = val; }

	ProductInfo GetDataWholeDay() const { return m_DataWholeDay; }
	void SetDataWholeDay(ProductInfo val) { m_DataWholeDay = val; }

	//////////////////////////////////////////////////////////////////////////
	// Main UI Count [12/14/2016 OSC]
	tElement m_MainCountINPUT;
	tElement m_MainCountGOOD;
	tElement m_MainCountREJECT;
	tElement m_MainCountMCR;
	tElement m_MainCountCONTACTALIGN;
	tElement m_MainCountVISIONALIGN;


	//20170206 byskcho
	tElement m_MainCountEDGEALIGN;
	tElement m_MainCountCONTACT;

	//20170618 JSJUNG 지그별 컨택 COUNT
	tElement m_JigTotalCountCONTACT[4][2];
	tElement m_JigGoodCountCONTACT[4][2];

	//2017-12-01,SKCHO
	tRetryInfo m_LDROBOT_RetryInfo;
	tRetryInfo m_PATR_RetryInfo;
	tRetryInfo m_PASTAGE_RetryInfo;
	tRetryInfo m_ULDTR_RetryInfo;
	tRetryInfo m_OKTR_RetryInfo;
	tRetryInfo m_NGTR_RetryInfo;
	void InitRetryCount();


	CTime m_timeMainCount;
	void InitMainCountValue();
	void InitJigCountValue();
	void UpdateMainInputCount();
	// Cell 정보 제거시 들어온 수량 일치 하지 않아서 추가함. 17/04/22
	void UpdateMainInputCountMinus();
	void UpdateMainOutputCount(CCellInfo *pCell);
	void SaveMainCount();
	void LoadMainCount();

	//2017-06-17 JSJUNG, JIG STATEMENT 추가
	void UpdateJigOutputCount(CCellInfo *pCell);
	void SaveJigCount();
	void LoadJigCount();


	//프로그램 시작시 생산 수량 읽는 함수
	void LoadProductFile(CString szTime);
	//////////////////////////////////////////////////////////////////////////
};

