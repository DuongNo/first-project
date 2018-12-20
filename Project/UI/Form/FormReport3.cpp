// FormReport3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormReport3.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxButtonEx.h"
#include <algorithm>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



// CFormReport3

IMPLEMENT_DYNCREATE(CFormReport3, CFormView)

CFormReport3::CFormReport3()
	: CFormView(CFormReport3::IDD)
{
	m_eIDX_Shift=eIDX_Shift_DAY;
	
}

CFormReport3::~CFormReport3()
{
}

void CFormReport3::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormReport3, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReport3 진단입니다.

#ifdef _DEBUG
void CFormReport3::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport3::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormReport3 메시지 처리기입니다.


// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame에서 ShowDialog를 수행하면 아래 순서대로 메세지를 호출한다.
//
// ShowDialog() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CFormReport3::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	sel_date=0;
	if((int)time.GetHour()<14) //현재 시간에 맞춰서 작업조 설정
		m_eIDX_Shift=eIDX_Shift_DAY;
	else if((int)time.GetHour()<22)
		m_eIDX_Shift=eIDX_Shift_SW;
	else
		m_eIDX_Shift=eIDX_Shift_GY;
	ShowHide();
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		m_alarm_map.clear();
		ClearData();
		AlarmUdate(m_eIDX_Shift);
		
	}
	else
	{

	}

	return 0;
}


// 표시 언어가 변경되었다.
LRESULT CFormReport3::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_LIST_ALARM_0_0);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXLBL_ALARM_PREV);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport3::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}
BEGIN_EVENTSINK_MAP(CFormReport3, CFormView)
	ON_EVENT(CFormReport3, IDC_GXLBL_ALARM_DAY, DISPID_CLICK, CFormReport3::ClickGxlblAlarmDay, VTS_NONE)
	ON_EVENT(CFormReport3, IDC_GXLBL_ALARM_SW, DISPID_CLICK, CFormReport3::ClickGxlblAlarmSw, VTS_NONE)
	ON_EVENT(CFormReport3, IDC_GXLBL_ALARM_GY, DISPID_CLICK, CFormReport3::ClickGxlblAlarmGy, VTS_NONE)
	ON_EVENT(CFormReport3, IDC_GXLBL_ALARM_PREV, DISPID_CLICK, CFormReport3::ClickGxlblAlarmPrev, VTS_NONE)
	ON_EVENT(CFormReport3, IDC_GXLBL_ALARM_NEXT, DISPID_CLICK, CFormReport3::ClickGxlblAlarmNext, VTS_NONE)
	ON_EVENT(CFormReport3, IDC_GXLBL_ALARM_TODAY, DISPID_CLICK, CFormReport3::ClickGxlblAlarmToday, VTS_NONE)
END_EVENTSINK_MAP()

void CFormReport3::AlarmUdate( int sel )
{
	CTime today_time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(today_time, theConfigBank.m_Option.m_OffsetHour, 0);
	CGxButtonEx* pAlarmDay = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_DAY );
	CGxButtonEx* pAlarmSw = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_SW );
	CGxButtonEx* pAlarmGy = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_GY );
	CGxButtonEx* pAlarmToday = (CGxButtonEx*)GetDlgItem( IDC_GXLBL_ALARM_TODAY );
	
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_ALARM_DATE );
	CString logpath;
	CString path;
	CString filename;
	CFileFind finder;
	CFileFind errorfinder;
	std::vector<CString> m_vecDir;
	std::vector<CString> m_vecDirTemp;
	if(time.GetDay()==today_time.GetDay())
	{
		pAlarmToday->SetBackColor(Color(Color::Lime).ToCOLORREF());

	}
	else
	{
		pAlarmToday->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	if(sel==eIDX_Shift_DAY)
	{
		pAlarmDay->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pAlarmSw->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmGy->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else if(sel==eIDX_Shift_SW)
	{
		
		pAlarmDay->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmSw->SetBackColor(Color(Color::Lime).ToCOLORREF());
		pAlarmGy->SetBackColor(Color(Color::White).ToCOLORREF());
	}
	else
	{
		pAlarmDay->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmSw->SetBackColor(Color(Color::White).ToCOLORREF());
		pAlarmGy->SetBackColor(Color(Color::Lime).ToCOLORREF());
	}
	path.Format(_T("D:\\Cheetah\\Log\\Debug\\*"));
	
	pAlarmDate->SetCaption(m_sel_time);
	BOOL bWorking = finder.FindFile(path);//해당 폴더에 전체 디렉토리를 검색
	
	while (bWorking)
	{   
		bWorking = finder.FindNextFile();
		if(finder.IsDirectory() && !finder.IsDots())
		{
			CString dir= finder.GetFileName();

			if(dir!=m_sel_time )//선택한 날짜만 지정함
			{
				continue;
			}
			logpath.Format(_T("D:\\Cheetah\\Log\\Debug\\%s\\error.txt"),dir);
			BOOL errorexist=errorfinder.FindFile(logpath);
			if(errorexist)
			{
				m_vecDirTemp.push_back(dir);	//로그가 저장된 폴더안에서 error.txt파일이 있는 경로를 저장함.
			}
			path.Delete(path.GetLength()-1);
		}
	}

	int m_vecLength=(int)m_vecDirTemp.size();
	if (m_vecLength==0) return;

	FILE *fStream;

	filename.Format(_T("D:\\Cheetah\\Log\\Debug\\%s\\ErrorLog.csv"),m_vecDirTemp[0]);
	errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));

	if (e != 0) return;
	CStdioFile		file(fStream);
	CString strBuf, strItem;
	CTokenizer t;
	int time_num=0;
	int shift;
	m_alarm_map.clear();
	while(file.ReadString(strBuf))
	{

		t = CTokenizer(strBuf);
		strItem = t.GetNextToken(_T(","));
		
		//m_today_count++; //하루동안 발생한 총 알람 수량
		//기록된 시간값 추출
		if(strItem!=_T("START TIME"))
			time_num=_ttoi(strItem.Right(9));
	
		
		if(time_num<14) //작업조 설정
		{
			shift=0;
		}
		else if (time_num<22)
		{
			shift=1;
		}
		else
		{
			shift=2;
		}
		if(shift==m_eIDX_Shift)
		{
			int alarm_code;
			pair<map<int, int>::iterator, bool > pr;
 			strItem = t.GetNextToken(_T(","));
 			strItem = t.GetNextToken(_T(","));
			strItem = t.GetNextToken(_T(","));
			//if(strItem!=_T("CODE"))
			//{
			//	pr = m_alarm_map.insert(pair<int,int>(_ttoi(strItem), 1));
			//	if(pr.second!=TRUE) //알람코드가 중복이 된다면 수량을 ++ 시킨다.
			//		m_alarm_map[_ttoi(strItem)]++;
			//	alarm_code=_ttoi(strItem);
			//	strItem = t.GetNextToken(_T(","));
			//	alarm_input[alarm_code].Alarm_name=strItem;
			//	strItem = t.GetNextToken(_T(","));
			//	if(_ttoi(strItem)==ALARM_LEVEL_WARNING)
			//		alarm_input[alarm_code].nLevel=_T("WARNING");
			//	else if(_ttoi(strItem)==ALARM_LEVEL_ALARM)
			//		alarm_input[alarm_code].nLevel=_T("ALARM");
			//	else
			//		alarm_input[alarm_code].nLevel=_T("PASSWORD");
			//}
			if(strItem!=_T("CODE"))
			{				
				alarm_code = _ttoi(strItem);

				if(alarm_code >= ALM_MAX)  alarm_code = 0;
				pr = m_alarm_map.insert(pair<int,int>(alarm_code, 1));
				
				if(pr.second!=TRUE) //알람코드가 중복이 된다면 수량을 ++ 시킨다.
				{
					m_alarm_map[alarm_code]++;					
				}				

				strItem = t.GetNextToken(_T(","));
				alarm_input[alarm_code].Alarm_name=strItem;
				strItem = t.GetNextToken(_T(","));
				if(_ttoi(strItem)==ALARM_LEVEL_WARNING)
					alarm_input[alarm_code].nLevel=_T("WARNING");
				else if(_ttoi(strItem)==ALARM_LEVEL_ALARM)
					alarm_input[alarm_code].nLevel=_T("ALARM");
				else
					alarm_input[alarm_code].nLevel=_T("PASSWORD");
			}
			
		}
		
	}
	file.Close();
	
	ListUpdate();
}
template<template <typename> class P = std::less >
struct compare_pair_second {
	template<class T1, class T2> bool operator()(const std::pair<T1, T2>&left, const std::pair<T1, T2>&right) {
		return P<T2>()(left.second, right.second);
	}
};
void CFormReport3::ClearData()
{
	CGxStaticEx* pAlarmNumber1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_1 );
	CGxStaticEx* pAlarmNumber2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_1 );
	CGxStaticEx* pAlarmNumber3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_1 );
	CGxStaticEx* pAlarmNumber4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_1 );
	CGxStaticEx* pAlarmNumber5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_1 );
	CGxStaticEx* pAlarmNumber6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_1 );
	CGxStaticEx* pAlarmNumber7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_1 );
	CGxStaticEx* pAlarmNumber8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_1 );
	CGxStaticEx* pAlarmNumber9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_1 );
	CGxStaticEx* pAlarmNumber10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_1 );
	CGxStaticEx* pAlarmName1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_2 );
	CGxStaticEx* pAlarmName2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_2 );
	CGxStaticEx* pAlarmName3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_2 );
	CGxStaticEx* pAlarmName4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_2 );
	CGxStaticEx* pAlarmName5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_2 );
	CGxStaticEx* pAlarmName6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_2 );
	CGxStaticEx* pAlarmName7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_2 );
	CGxStaticEx* pAlarmName8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_2 );
	CGxStaticEx* pAlarmName9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_2 );
	CGxStaticEx* pAlarmName10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_2 );
	CGxStaticEx* pAlarmLevel1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_3 );
	CGxStaticEx* pAlarmLevel2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_3 );
	CGxStaticEx* pAlarmLevel3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_3 );
	CGxStaticEx* pAlarmLevel4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_3 );
	CGxStaticEx* pAlarmLevel5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_3 );
	CGxStaticEx* pAlarmLevel6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_3 );
	CGxStaticEx* pAlarmLevel7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_3 );
	CGxStaticEx* pAlarmLevel8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_3 );
	CGxStaticEx* pAlarmLevel9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_3 );
	CGxStaticEx* pAlarmLevel10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_3 );
	CGxStaticEx* pAlarmCount1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_4 );
	CGxStaticEx* pAlarmCount2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_4 );
	CGxStaticEx* pAlarmCount3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_4 );
	CGxStaticEx* pAlarmCount4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_4 );
	CGxStaticEx* pAlarmCount5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_4 );
	CGxStaticEx* pAlarmCount6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_4 );
	CGxStaticEx* pAlarmCount7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_4 );
	CGxStaticEx* pAlarmCount8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_4 );
	CGxStaticEx* pAlarmCount9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_4 );
	CGxStaticEx* pAlarmCount10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_4 );

	pAlarmNumber1->SetCaption(_T(""));
	pAlarmNumber2->SetCaption(_T(""));
	pAlarmNumber3->SetCaption(_T(""));
	pAlarmNumber4->SetCaption(_T(""));
	pAlarmNumber5->SetCaption(_T(""));
	pAlarmNumber6->SetCaption(_T(""));
	pAlarmNumber7->SetCaption(_T(""));
	pAlarmNumber8->SetCaption(_T(""));
	pAlarmNumber9->SetCaption(_T(""));
	pAlarmNumber10->SetCaption(_T(""));
	pAlarmName1->SetCaption(_T(""));
	pAlarmName2->SetCaption(_T(""));
	pAlarmName3->SetCaption(_T(""));
	pAlarmName4->SetCaption(_T(""));
	pAlarmName5->SetCaption(_T(""));
	pAlarmName6->SetCaption(_T(""));
	pAlarmName7->SetCaption(_T(""));
	pAlarmName8->SetCaption(_T(""));
	pAlarmName9->SetCaption(_T(""));
	pAlarmName10->SetCaption(_T(""));
	pAlarmLevel1->SetCaption(_T(""));
	pAlarmLevel2->SetCaption(_T(""));
	pAlarmLevel3->SetCaption(_T(""));
	pAlarmLevel4->SetCaption(_T(""));
	pAlarmLevel5->SetCaption(_T(""));
	pAlarmLevel6->SetCaption(_T(""));
	pAlarmLevel7->SetCaption(_T(""));
	pAlarmLevel8->SetCaption(_T(""));
	pAlarmLevel9->SetCaption(_T(""));
	pAlarmLevel10->SetCaption(_T(""));
	pAlarmName10->SetCaption(_T(""));
	pAlarmCount1->SetCaption(_T(""));
	pAlarmCount2->SetCaption(_T(""));
	pAlarmCount3->SetCaption(_T(""));
	pAlarmCount4->SetCaption(_T(""));
	pAlarmCount5->SetCaption(_T(""));
	pAlarmCount6->SetCaption(_T(""));
	pAlarmCount7->SetCaption(_T(""));
	pAlarmCount8->SetCaption(_T(""));
	pAlarmCount9->SetCaption(_T(""));
	pAlarmCount10->SetCaption(_T(""));
}
void CFormReport3::ListUpdate()
{

	CGxStaticEx* pAlarmNumber1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_1 );
	CGxStaticEx* pAlarmNumber2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_1 );
	CGxStaticEx* pAlarmNumber3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_1 );
	CGxStaticEx* pAlarmNumber4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_1 );
	CGxStaticEx* pAlarmNumber5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_1 );
	CGxStaticEx* pAlarmNumber6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_1 );
	CGxStaticEx* pAlarmNumber7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_1 );
	CGxStaticEx* pAlarmNumber8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_1 );
	CGxStaticEx* pAlarmNumber9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_1 );
	CGxStaticEx* pAlarmNumber10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_1 );
	CGxStaticEx* pAlarmName1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_2 );
	CGxStaticEx* pAlarmName2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_2 );
	CGxStaticEx* pAlarmName3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_2 );
	CGxStaticEx* pAlarmName4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_2 );
	CGxStaticEx* pAlarmName5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_2 );
	CGxStaticEx* pAlarmName6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_2 );
	CGxStaticEx* pAlarmName7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_2 );
	CGxStaticEx* pAlarmName8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_2 );
	CGxStaticEx* pAlarmName9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_2 );
	CGxStaticEx* pAlarmName10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_2 );
	CGxStaticEx* pAlarmLevel1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_3 );
	CGxStaticEx* pAlarmLevel2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_3 );
	CGxStaticEx* pAlarmLevel3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_3 );
	CGxStaticEx* pAlarmLevel4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_3 );
	CGxStaticEx* pAlarmLevel5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_3 );
	CGxStaticEx* pAlarmLevel6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_3 );
	CGxStaticEx* pAlarmLevel7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_3 );
	CGxStaticEx* pAlarmLevel8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_3 );
	CGxStaticEx* pAlarmLevel9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_3 );
	CGxStaticEx* pAlarmLevel10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_3 );
	CGxStaticEx* pAlarmCount1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_1_4 );
	CGxStaticEx* pAlarmCount2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_2_4 );
	CGxStaticEx* pAlarmCount3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_3_4 );
	CGxStaticEx* pAlarmCount4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_4_4 );
	CGxStaticEx* pAlarmCount5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_5_4 );
	CGxStaticEx* pAlarmCount6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_6_4 );
	CGxStaticEx* pAlarmCount7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_7_4 );
	CGxStaticEx* pAlarmCount8 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_8_4 );
	CGxStaticEx* pAlarmCount9 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_9_4 );
	CGxStaticEx* pAlarmCount10 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_10_4 );
	
	ClearData();
	if((int)m_alarm_map.size()==0)
		return;
	std::vector< std::pair< int, int > > vectorData;
	for (auto iter = m_alarm_map.begin(); iter != m_alarm_map.end(); ++iter)
		vectorData.emplace_back(std::make_pair(iter->first, iter->second));
	//저장된 Map의 value값을 vector값에 대입하여 내림차순으로 정렬한다.
	std::sort(vectorData.begin(), vectorData.end(), compare_pair_second<std::greater>());
	int selnum=(int)vectorData.size();
	CString val;


	if(selnum>=1)
	{
		val.Format(_T("%d"),vectorData[0].first);
		pAlarmNumber1->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[0].first].Alarm_name);
		pAlarmName1->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[0].first].nLevel);
		pAlarmLevel1->SetCaption(val);
		val.Format(_T("%d"),vectorData[0].second);
		pAlarmCount1->SetCaption(val);
	}
	if(selnum>=2)
	{
		val.Format(_T("%d"),vectorData[1].first);
		pAlarmNumber2->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[1].first].Alarm_name);
		pAlarmName2->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[1].first].nLevel);
		pAlarmLevel2->SetCaption(val);
		val.Format(_T("%d"),vectorData[1].second);
		pAlarmCount2->SetCaption(val);
	}
	if(selnum>=3)
	{
		val.Format(_T("%d"),vectorData[2].first);
		pAlarmNumber3->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[2].first].Alarm_name);
		pAlarmName3->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[2].first].nLevel);
		pAlarmLevel3->SetCaption(val);
		val.Format(_T("%d"),vectorData[2].second);
		pAlarmCount3->SetCaption(val);
	}
	if(selnum>=4)
	{
		val.Format(_T("%d"),vectorData[3].first);
		pAlarmNumber4->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[3].first].Alarm_name);
		pAlarmName4->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[3].first].nLevel);
		pAlarmLevel4->SetCaption(val);
		val.Format(_T("%d"),vectorData[3].second);
		pAlarmCount4->SetCaption(val);
	}
	if(selnum>=5)
	{
		val.Format(_T("%d"),vectorData[4].first);
		pAlarmNumber5->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[4].first].Alarm_name);
		pAlarmName5->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[4].first].nLevel);
		pAlarmLevel5->SetCaption(val);
		val.Format(_T("%d"),vectorData[4].second);
		pAlarmCount5->SetCaption(val);
	}
	if(selnum>=6)
	{
		val.Format(_T("%d"),vectorData[5].first);
		pAlarmNumber6->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[5].first].Alarm_name);
		pAlarmName6->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[5].first].nLevel);
		pAlarmLevel6->SetCaption(val);
		val.Format(_T("%d"),vectorData[5].second);
		pAlarmCount6->SetCaption(val);
	}
	if(selnum>=7)
	{
		val.Format(_T("%d"),vectorData[6].first);
		pAlarmNumber7->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[6].first].Alarm_name);
		pAlarmName7->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[6].first].nLevel);
		pAlarmLevel7->SetCaption(val);
		val.Format(_T("%d"),vectorData[6].second);
		pAlarmCount7->SetCaption(val);
	}
	if(selnum>=8)
	{
		val.Format(_T("%d"),vectorData[7].first);
		pAlarmNumber8->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[7].first].Alarm_name);
		pAlarmName8->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[7].first].nLevel);
		pAlarmLevel8->SetCaption(val);
		val.Format(_T("%d"),vectorData[7].second);
		pAlarmCount8->SetCaption(val);
	}
	if(selnum>=9)
	{
		val.Format(_T("%d"),vectorData[8].first);
		pAlarmNumber9->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[8].first].Alarm_name);
		pAlarmName9->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[8].first].nLevel);
		pAlarmLevel9->SetCaption(val);
		val.Format(_T("%d"),vectorData[8].second);
		pAlarmCount9->SetCaption(val);
	}
	if(selnum>=10)
	{
		val.Format(_T("%d"),vectorData[9].first);
		pAlarmNumber10->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[9].first].Alarm_name);
		pAlarmName10->SetCaption(val);
		val.Format(_T("%s"),alarm_input[vectorData[9].first].nLevel);
		pAlarmLevel10->SetCaption(val);
		val.Format(_T("%d"),vectorData[9].second);
		pAlarmCount10->SetCaption(val);
	}
}

void CFormReport3::ClickGxlblAlarmDay()
{
	
	m_eIDX_Shift=eIDX_Shift_DAY;
	AlarmUdate(m_eIDX_Shift);
}


void CFormReport3::ClickGxlblAlarmSw()
{
	m_eIDX_Shift=eIDX_Shift_SW;
	AlarmUdate(m_eIDX_Shift);
}


void CFormReport3::ClickGxlblAlarmGy()
{
	m_eIDX_Shift=eIDX_Shift_GY;	
	AlarmUdate(m_eIDX_Shift);
}



void CFormReport3::ClickGxlblAlarmPrev()
{
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_ALARM_DATE );
	ClearData();
	//sel_date++;
	//time= CTime::GetCurrentTime();
	CTimeSpan span(1,0,0,0);
	time=time-span;
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());

	AlarmUdate(m_eIDX_Shift);
}


void CFormReport3::ClickGxlblAlarmNext()
{
	CGxStaticEx* pAlarmDate = (CGxStaticEx*)GetDlgItem( IDC_GXLBL_ALARM_DATE );
	ClearData();
	//sel_date++;
	CTime cur_time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(cur_time, theConfigBank.m_Option.m_OffsetHour, 0);
	if(cur_time.GetDay()==time.GetDay())//오늘 이후의 날짜는 값이 없으므로 return
	{
		return;
	}
	CTimeSpan span(1,0,0,0);

	time=time+span;
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());

	AlarmUdate(m_eIDX_Shift);
}


void CFormReport3::ClickGxlblAlarmToday()
{
	ClearData();
	time= CTime::GetCurrentTime();
	CEtc::ApplyTimeOffset(time, theConfigBank.m_Option.m_OffsetHour, 0);
	m_sel_time.Format(_T("%04u-%02u-%02u"),time.GetYear(), time.GetMonth(), time.GetDay());
	AlarmUdate(m_eIDX_Shift);
}

void CFormReport3::ShowHide()
{
	CGxStaticEx* pAlarmTotal_0_0 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_0_0 );
	CGxStaticEx* pAlarmTotal_0_1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_0_1 );
	CGxStaticEx* pAlarmTotal_0_2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_0_2 );
	CGxStaticEx* pAlarmTotal_0_3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_0_3 );
	CGxStaticEx* pAlarmTotal_0_4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_0_4 );
	CGxStaticEx* pAlarmTotal_0_5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_0_5 );
	CGxStaticEx* pAlarmTotal_0_6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_0_6 );
	CGxStaticEx* pAlarmTotal_0_7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_0_7 );
	CGxStaticEx* pAlarmTotal_1 = (CGxStaticEx*)GetDlgItem( IDC_COLORBTN_ALARM_TOTAL_1 );
	CGxStaticEx* pAlarmTotal_2 = (CGxStaticEx*)GetDlgItem( IDC_COLORBTN_ALARM_TOTAL_2 );
	CGxStaticEx* pAlarmTotal_3 = (CGxStaticEx*)GetDlgItem( IDC_COLORBTN_ALARM_TOTAL_3 );
	CGxStaticEx* pAlarmTotal_4 = (CGxStaticEx*)GetDlgItem( IDC_COLORBTN_ALARM_TOTAL_4 );
	CGxStaticEx* pAlarmTotal_5 = (CGxStaticEx*)GetDlgItem( IDC_COLORBTN_ALARM_TOTAL_5 );
	CGxStaticEx* pAlarmTotal_1_0 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_1_0 );
	CGxStaticEx* pAlarmTotal_1_1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_1_1 );
	CGxStaticEx* pAlarmTotal_1_2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_1_2 );
	CGxStaticEx* pAlarmTotal_1_3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_1_3 );
	CGxStaticEx* pAlarmTotal_1_4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_1_4 );
	CGxStaticEx* pAlarmTotal_1_5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_1_5 );
	CGxStaticEx* pAlarmTotal_1_6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_1_6 );
	CGxStaticEx* pAlarmTotal_1_7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_1_7 );
	CGxStaticEx* pAlarmTotal_2_0 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_2_0 );
	CGxStaticEx* pAlarmTotal_2_1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_2_1 );
	CGxStaticEx* pAlarmTotal_2_2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_2_2 );
	CGxStaticEx* pAlarmTotal_2_3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_2_3 );
	CGxStaticEx* pAlarmTotal_2_4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_2_4 );
	CGxStaticEx* pAlarmTotal_2_5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_2_5 );
	CGxStaticEx* pAlarmTotal_2_6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_2_6 );
	CGxStaticEx* pAlarmTotal_2_7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_2_7 );
	CGxStaticEx* pAlarmTotal_3_0 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_3_0 );
	CGxStaticEx* pAlarmTotal_3_1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_3_1 );
	CGxStaticEx* pAlarmTotal_3_2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_3_2 );
	CGxStaticEx* pAlarmTotal_3_3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_3_3 );
	CGxStaticEx* pAlarmTotal_3_4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_3_4 );
	CGxStaticEx* pAlarmTotal_3_5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_3_5 );
	CGxStaticEx* pAlarmTotal_3_6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_3_6 );
	CGxStaticEx* pAlarmTotal_3_7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_3_7 );
	CGxStaticEx* pAlarmTotal_4_0 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_4_0 );
	CGxStaticEx* pAlarmTotal_4_1 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_4_1 );
	CGxStaticEx* pAlarmTotal_4_2 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_4_2 );
	CGxStaticEx* pAlarmTotal_4_3 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_4_3 );
	CGxStaticEx* pAlarmTotal_4_4 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_4_4 );
	CGxStaticEx* pAlarmTotal_4_5 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_4_5 );
	CGxStaticEx* pAlarmTotal_4_6 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_4_6 );
	CGxStaticEx* pAlarmTotal_4_7 = (CGxStaticEx*)GetDlgItem( IDC_LIST_ALARM_TOTAL_4_7 );

	pAlarmTotal_1->ShowWindow(SW_HIDE);
	pAlarmTotal_2->ShowWindow(SW_HIDE);
	pAlarmTotal_3->ShowWindow(SW_HIDE);
	pAlarmTotal_4->ShowWindow(SW_HIDE);
	pAlarmTotal_5->ShowWindow(SW_HIDE);
	pAlarmTotal_0_0->ShowWindow(SW_HIDE);
	pAlarmTotal_0_1->ShowWindow(SW_HIDE);
	pAlarmTotal_0_2->ShowWindow(SW_HIDE);
	pAlarmTotal_0_3->ShowWindow(SW_HIDE);
	pAlarmTotal_0_4->ShowWindow(SW_HIDE);
	pAlarmTotal_0_5->ShowWindow(SW_HIDE);
	pAlarmTotal_0_6->ShowWindow(SW_HIDE);
	pAlarmTotal_0_7->ShowWindow(SW_HIDE);
	pAlarmTotal_1_0->ShowWindow(SW_HIDE);
	pAlarmTotal_1_1->ShowWindow(SW_HIDE);
	pAlarmTotal_1_2->ShowWindow(SW_HIDE);
	pAlarmTotal_1_3->ShowWindow(SW_HIDE);
	pAlarmTotal_1_4->ShowWindow(SW_HIDE);
	pAlarmTotal_1_5->ShowWindow(SW_HIDE);
	pAlarmTotal_1_6->ShowWindow(SW_HIDE);
	pAlarmTotal_1_7->ShowWindow(SW_HIDE);
	pAlarmTotal_2_0->ShowWindow(SW_HIDE);
	pAlarmTotal_2_1->ShowWindow(SW_HIDE);
	pAlarmTotal_2_2->ShowWindow(SW_HIDE);
	pAlarmTotal_2_3->ShowWindow(SW_HIDE);
	pAlarmTotal_2_4->ShowWindow(SW_HIDE);
	pAlarmTotal_2_5->ShowWindow(SW_HIDE);
	pAlarmTotal_2_6->ShowWindow(SW_HIDE);
	pAlarmTotal_2_7->ShowWindow(SW_HIDE);
	pAlarmTotal_3_0->ShowWindow(SW_HIDE);
	pAlarmTotal_3_1->ShowWindow(SW_HIDE);
	pAlarmTotal_3_2->ShowWindow(SW_HIDE);
	pAlarmTotal_3_3->ShowWindow(SW_HIDE);
	pAlarmTotal_3_4->ShowWindow(SW_HIDE);
	pAlarmTotal_3_5->ShowWindow(SW_HIDE);
	pAlarmTotal_3_6->ShowWindow(SW_HIDE);
	pAlarmTotal_3_7->ShowWindow(SW_HIDE);
	pAlarmTotal_4_0->ShowWindow(SW_HIDE);
	pAlarmTotal_4_1->ShowWindow(SW_HIDE);
	pAlarmTotal_4_2->ShowWindow(SW_HIDE);
	pAlarmTotal_4_3->ShowWindow(SW_HIDE);
	pAlarmTotal_4_4->ShowWindow(SW_HIDE);
	pAlarmTotal_4_5->ShowWindow(SW_HIDE);
	pAlarmTotal_4_6->ShowWindow(SW_HIDE);
	pAlarmTotal_4_7->ShowWindow(SW_HIDE);

}
