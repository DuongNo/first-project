// FormReport4.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormReport4.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormReport4

IMPLEMENT_DYNCREATE(CFormReport4, CFormView)

CFormReport4::CFormReport4()
	: CFormView(CFormReport4::IDD)
{
	IsOpenFile=FALSE;
	logpath="";
	m_iUpDownParam=0;
}

CFormReport4::~CFormReport4()
{
}

void CFormReport4::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_REPORT4_ALARMLIST, m_AlarmGrid);
}

BEGIN_MESSAGE_MAP(CFormReport4, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReport4 진단입니다.

#ifdef _DEBUG
void CFormReport4::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport4::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormReport4 메시지 처리기입니다.

void CFormReport4::AlarmGrid()
{
	if(m_AlarmGrid.GetSafeHwnd() == NULL)return;

	m_AlarmGrid.SetEditable(FALSE);
	m_AlarmGrid.SetRowCount(20);
	m_AlarmGrid.SetColumnCount(6);
	m_AlarmGrid.SetDefCellHeight(30);
	m_AlarmGrid.SetFixedColumnCount(1); 
	m_AlarmGrid.SetFixedRowCount(1);
	m_AlarmGrid.SetRowResize(FALSE);
	m_AlarmGrid.SetColumnResize(FALSE);
	m_AlarmGrid.SetListMode(FALSE);
	m_AlarmGrid.Refresh();

	m_AlarmGrid.SetColumnWidth(0,47);
	m_AlarmGrid.SetColumnWidth(1,250);
	m_AlarmGrid.SetColumnWidth(2,250);
	m_AlarmGrid.SetColumnWidth(3,90);
	m_AlarmGrid.SetColumnWidth(4,420);
	m_AlarmGrid.SetColumnWidth(5,90);

	m_AlarmGrid.SetItemText(0,0,_T("No"));
	m_AlarmGrid.SetItemText(0,1,_T("발생시간"));
	m_AlarmGrid.SetItemText(0,2,_T("조치시간"));
	m_AlarmGrid.SetItemText(0,3,_T("알람코드"));
	m_AlarmGrid.SetItemText(0,4,_T("알람명"));
	m_AlarmGrid.SetItemText(0,5,_T("레벨"));


	CRect rect;
	m_AlarmGrid.GetClientRect(rect);
	int iRect = rect.Width();

	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER;
	LOGFONT lf;
	CFont* pGridFont =m_AlarmGrid.GetFont();
	pGridFont->GetLogFont(&lf);

	memcpy(lf.lfFaceName, _T("굴림체"), sizeof(_T("굴림체")));
	lf.lfHeight = 14;
	lf.lfWeight  = FW_BOLD;

	for(int iRow = 0; iRow<m_AlarmGrid.GetRowCount(); iRow++)
	{
		for(int iCol = 0; iCol<m_AlarmGrid.GetColumnCount(); iCol++)
		{
			m_AlarmGrid.SetItemFont(iRow, iCol, &lf);
			m_AlarmGrid.SetItemFormat(iRow, iCol, dwTextStyle);
		}
	}

}
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
LRESULT CFormReport4::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	//테스트용으로 알람 하나 생성
	
	

	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		IsOpenFile=FALSE;
		AlarmGrid();
		LoadArarm();
		UpdateArarmGrid();
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormReport4::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_STR_NUM);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_HALFUP);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport4::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

void CFormReport4::LoadArarm()
{
	std::vector<CString> m_vecErrorCodeTemp;
	std::vector<CString> m_vecAlarmNameTemp;
	std::vector<CString> m_vecAlarm_StartTimeTemp;
	std::vector<CString> m_vecAlarm_EndTimeTemp;
	std::vector<CString> m_vecAlarmTakeTimeTemp;
	std::vector<CString> m_vecAlarmLevelTemp;
	//알람 검색전 현재 시간 확인
	SYSTEMTIME	time;
	CString current_time;
	GetLocalTime(&time);
	current_time.Format(_T("%04u-%02u-%02u"),time.wYear, time.wMonth, time.wDay);
	
	int nIndex = 0;

	m_vecErrorCode.clear();
	m_vecAlarmName.clear();
	m_vecAlarm_StartTime.clear();
	m_vecAlarm_EndTime.clear();   //2017-04-15,skcho, ENDTIME CLEAR추가 , 이전END시간이 현재 END시간에 표시되는에러 수정
	m_vecAlarmName.clear();
	m_vecAlarmLevel.clear();
	m_vecAlarmTakeTime.clear();

	m_vecAlarm_StartTimeTemp.clear();
	m_vecAlarmNameTemp.clear();
	m_vecAlarm_EndTimeTemp.clear();
	m_vecAlarmTakeTimeTemp.clear();
	m_vecAlarmLevelTemp.clear();
	CString path;
	CString filename;
	CFileFind finder;
	CFileFind errorfinder;
	std::vector<CString> m_vecDir;
	std::vector<CString> m_vecDirTemp;
	path.Format(_T("D:\\Cheetah\\Log\\Debug\\*"));

	BOOL bWorking = finder.FindFile(path);//해당 폴더에 전체 디렉토리를 검색
	if(!IsOpenFile)//파일 열기버튼을 클릭한게 아니라면 오늘 날짜를 기준으로 로그파일을 열게 한다.
	{
		while (bWorking)
		{   
			bWorking = finder.FindNextFile();
			if(finder.IsDirectory() && !finder.IsDots())
			{
				CString dir= finder.GetFileName();
				
				if(dir!=current_time )//오늘 날짜만 저장함
				{
					continue;
				}
				logpath.Format(_T("D:\\Cheetah\\Log\\Debug\\%s\\ErrorLog.csv"),dir);
				BOOL errorexist=errorfinder.FindFile(logpath);
				if(errorexist)
				{
					m_vecDirTemp.push_back(dir);	//로그가 저장된 폴더안에서 error.txt파일이 있는 경로를 저장함.
				}
				path.Delete(path.GetLength()-1);
			}
		}
	}
	else
	{
		m_vecDirTemp.push_back(_T("file_open"));
	}
	int m_vecLength=(int)m_vecDirTemp.size();

	//가장 최근 시간 값이 맨 위로 오도록 재정렬해서 넣어준다.
	for(int i=0; i<m_vecLength;i++)
	{
		m_vecDir.push_back(m_vecDirTemp[m_vecLength-(i+1)]);
	}
	if ((int)m_vecDir.size()==0) return;
	
	for(int i=0; i<(int)m_vecDir.size(); i++)
	{
		CString filename;
		FILE *fStream;
		
		filename.Format(IsOpenFile?_T("%s"),logpath:_T("D:\\Cheetah\\Log\\Debug\\%s\\ErrorLog.csv"),m_vecDir[i]);
		errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));

		if (e != 0)
		{
			//2017-09-17,SKCHO, ERROR LOG 파일 열려있으면 메세지
			AfxMessageBox(_T("File Open Already"), MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
			return;
		}

		CStdioFile		file(fStream);
		CString strBuf, strItem;
		CTokenizer t;
		while(file.ReadString(strBuf))
		{
			t = CTokenizer(strBuf);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("START TIME"))
				m_vecAlarm_StartTimeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("END TIME"))
				m_vecAlarm_EndTimeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("SPAN"))
				m_vecAlarmTakeTimeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("CODE"))
				m_vecErrorCodeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("NAME"))
				m_vecAlarmNameTemp.push_back(strItem);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("LEVEL"))
			{
				if(strItem==_T("0"))
					strItem=_T("WARNING");
				else if(strItem==_T("1"))
					strItem=_T("ALARM");
				else
					strItem=_T("PASSWORD");
				m_vecAlarmLevelTemp.push_back(strItem);
			}
			
			/*t = CTokenizer(strBuf);
			strItem = t.GetNextToken(_T("\t"));

			m_vecAlarmTimeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T("\t"));
			m_vecAlarmStatTemp.push_back(strItem);
			strItem = t.GetNextToken(_T("\t"));
			m_vecAlarmCodeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T("\t"));
			m_vecAlarmNameTemp.push_back(strItem);
			strItem = t.GetNextToken(_T("\t"));
			m_vecAlarmLevelTemp.push_back(strItem);*/
		}	
		file.Close();
	

	}
	m_vecLength= m_vecAlarm_StartTimeTemp.size();
	for(int j=0; j<m_vecLength;j++)
	{
		nIndex = m_vecLength-(j+1);
		m_vecErrorCode.push_back(m_vecErrorCodeTemp[nIndex]);
		m_vecAlarmName.push_back(m_vecAlarmNameTemp[nIndex]);
		m_vecAlarm_StartTime.push_back(m_vecAlarm_StartTimeTemp[nIndex]);
		m_vecAlarm_EndTime.push_back(m_vecAlarm_EndTimeTemp[nIndex]);
		m_vecAlarmTakeTimeTemp.push_back(m_vecAlarmTakeTimeTemp[nIndex]);
		m_vecAlarmLevel.push_back(m_vecAlarmLevelTemp[nIndex]);
		//m_vecAlarmTime.push_back(m_vecAlarmTimeTemp[m_vecLength-(j+1)]);
		//m_vecAlarmStat.push_back(m_vecAlarmStatTemp[m_vecLength-(j+1)]);
		//m_vecAlarmCode.push_back(m_vecAlarmCodeTemp[m_vecLength-(j+1)]);
		//m_vecAlarmName.push_back(m_vecAlarmNameTemp[m_vecLength-(j+1)]);
		//m_vecAlarmLevel.push_back(m_vecAlarmLevelTemp[m_vecLength-(j+1)]);
	}
	
	
}

void CFormReport4::UpdateArarmGrid()
{
	CString sNum;

	//m_AlarmGrid.SetRowCount(m_vecRecipe.size() + 1 );
	LOGFONT lf;
	CFont* pGridFont =m_AlarmGrid.GetFont();
	pGridFont->GetLogFont(&lf);
	sNum.Format(_T("%d"),m_vecAlarm_StartTime.size());
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_NUM,sNum);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_FILENAME,logpath);
	memcpy(lf.lfFaceName, _T("굴림체"), sizeof(_T("굴림체")));
	lf.lfHeight = 14;
	lf.lfWeight  = FW_BOLD;
	m_AlarmGrid.SetRowCount(20);
	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER;

	for(int i = 0; i < m_AlarmGrid.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;
		else			m_AlarmGrid.SetRowHeight(i, 35);

		sNum.Format(_T("%d"),i+m_iUpDownParam);
		for(int j = 0; j < m_AlarmGrid.GetColumnCount(); j++ )
		{
			m_AlarmGrid.SetItemFont(i, j, &lf);
			m_AlarmGrid.SetItemFormat(i, j, dwTextStyle);

			if ( j == 0 )	m_AlarmGrid.SetItemText(i, j, sNum);
			else			
			{
				CString szText;
				if ( m_vecAlarm_StartTime.size() > UINT(i-1 + m_iUpDownParam) )
				{
					m_AlarmGrid.SetItemText(i, 2, m_vecAlarm_EndTime[i - 1 + m_iUpDownParam] );
					m_AlarmGrid.SetItemText(i, 1, m_vecAlarm_StartTime[i - 1 + m_iUpDownParam] );
					m_AlarmGrid.SetItemText(i, 3, m_vecErrorCode[i - 1 + m_iUpDownParam] );
					m_AlarmGrid.SetItemText(i, 4, m_vecAlarmName[i - 1 + m_iUpDownParam] );
					m_AlarmGrid.SetItemText(i, 5, m_vecAlarmLevel[i - 1 + m_iUpDownParam] );
					
					//m_AlarmGrid.SetItemText(i, 5, m_vecAlarmTakeTime[i - 1 + m_iUpDownParam] );
					
				}
				else
					m_AlarmGrid.SetItemText(i,j, _T(""));
			}
		}
	}
	m_AlarmGrid.ShowScrollBar(SB_HORZ,FALSE);		//	횡 스크롤 제거
	m_AlarmGrid.ShowScrollBar(SB_VERT,FALSE);		//	종 스크롤 제거
	m_AlarmGrid.Refresh();
}
BEGIN_EVENTSINK_MAP(CFormReport4, CFormView)
	ON_EVENT(CFormReport4, IDC_GXBTN_HALFDOWN, DISPID_CLICK, CFormReport4::ClickGxbtnHalfdown, VTS_NONE)
	ON_EVENT(CFormReport4, IDC_GXBTN_DOWN, DISPID_CLICK, CFormReport4::ClickGxbtnDown, VTS_NONE)
	ON_EVENT(CFormReport4, IDC_GXBTN_HALFUP, DISPID_CLICK, CFormReport4::ClickGxbtnHalfup, VTS_NONE)
	ON_EVENT(CFormReport4, IDC_GXBTN_UP, DISPID_CLICK, CFormReport4::ClickGxbtnUp, VTS_NONE)
	ON_EVENT(CFormReport4, IDC_GXBTN__LOADFILE, DISPID_CLICK, CFormReport4::ClickGxbtn, VTS_NONE)
END_EVENTSINK_MAP()


void CFormReport4::ClickGxbtnHalfdown()
{
	if ( m_iUpDownParam + 3  >= (int)m_vecAlarm_StartTime.size() )		
		return;

	m_iUpDownParam += 3;
	UpdateArarmGrid();
}


void CFormReport4::ClickGxbtnDown()
{
	if ( m_iUpDownParam + 7 >=  (int)m_vecAlarm_StartTime.size()  )		
		return;

	m_iUpDownParam += 7;
	UpdateArarmGrid();
	
}


void CFormReport4::ClickGxbtnHalfup()
{
	if ( m_iUpDownParam - 3  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 3;

	UpdateArarmGrid();
}


void CFormReport4::ClickGxbtnUp()
{
	if ( m_iUpDownParam - 7  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 7;

	UpdateArarmGrid();
}


void CFormReport4::ClickGxbtn()
{
	char szFile[256];
  //API file dialog
  OPENFILENAMEA ofn;
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = NULL;
  ofn.lpstrFile = szFile;
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "ErrorLog.csv\0ErrorLog.csv\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = "D:\\Cheetah\\Log\\Debug";
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
   BOOL ret = GetOpenFileNameA(&ofn);
  if (ret) 
  {
   // load map and enter training mode
	  IsOpenFile=TRUE;
	  logpath=ofn.lpstrFile;
	  IsOpenFile;
	  LoadArarm();
	  UpdateArarmGrid();
  } 
}
