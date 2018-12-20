// FormReport5.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "FormReport5.h"

#include "GUIDefine.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormReport5

IMPLEMENT_DYNCREATE(CFormReport5, CFormView)

CFormReport5::CFormReport5()
	: CFormView(CFormReport5::IDD)
{
	IsOpenFile=FALSE;
	logpath="";
	m_iUpDownParam=0;
}

CFormReport5::~CFormReport5()
{
}

void CFormReport5::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_REPORT5_IDLELIST, m_IDLEGrid);
}

BEGIN_MESSAGE_MAP(CFormReport5, CFormView)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

END_MESSAGE_MAP()


// CFormReport5 진단입니다.

#ifdef _DEBUG
void CFormReport5::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormReport5::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormReport5 메시지 처리기입니다.

void CFormReport5::IDLEGrid()
{
	if(m_IDLEGrid.GetSafeHwnd() == NULL)return;

	m_IDLEGrid.SetEditable(FALSE);
	m_IDLEGrid.SetRowCount(20);
	m_IDLEGrid.SetColumnCount(6);
	m_IDLEGrid.SetDefCellHeight(30);
	m_IDLEGrid.SetFixedColumnCount(1); 
	m_IDLEGrid.SetFixedRowCount(1);
	m_IDLEGrid.SetRowResize(FALSE);
	m_IDLEGrid.SetColumnResize(FALSE);
	m_IDLEGrid.SetListMode(FALSE);
	m_IDLEGrid.Refresh();

	m_IDLEGrid.SetColumnWidth(0,47);
	m_IDLEGrid.SetColumnWidth(1,200);
	m_IDLEGrid.SetColumnWidth(2,350);
	m_IDLEGrid.SetColumnWidth(3,350);
	m_IDLEGrid.SetColumnWidth(4,200);

	m_IDLEGrid.SetItemText(0,0,_T("No"));
	m_IDLEGrid.SetItemText(0,1,_T("IDLE NAME"));
	m_IDLEGrid.SetItemText(0,2,_T("START TIME"));
	m_IDLEGrid.SetItemText(0,3,_T("END TIME"));
	m_IDLEGrid.SetItemText(0,4,_T("SPAN"));



	CRect rect;
	m_IDLEGrid.GetClientRect(rect);
	int iRect = rect.Width();

	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER;
	LOGFONT lf;
	CFont* pGridFont =m_IDLEGrid.GetFont();
	pGridFont->GetLogFont(&lf);

	memcpy(lf.lfFaceName, _T("굴림체"), sizeof(_T("굴림체")));
	lf.lfHeight = 14;
	lf.lfWeight  = FW_BOLD;

	for(int iRow = 0; iRow<m_IDLEGrid.GetRowCount(); iRow++)
	{
		for(int iCol = 0; iCol<m_IDLEGrid.GetColumnCount(); iCol++)
		{
			m_IDLEGrid.SetItemFont(iRow, iCol, &lf);
			m_IDLEGrid.SetItemFormat(iRow, iCol, dwTextStyle);
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
LRESULT CFormReport5::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	//테스트용으로 알람 하나 생성
	
	

	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		IsOpenFile=FALSE;
		IDLEGrid();
		LoadIDLE();
		UpdateIDLEGrid();
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormReport5::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastStaticCpationIndex(this, wParam, IDC_GXSTC_STR_NUM);
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_HALFUP);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormReport5::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}

void CFormReport5::LoadIDLE()
{
	std::vector<CString> m_vecErrorCodeTemp;
	std::vector<CString> m_vecIDLENameTemp;
	std::vector<CString> m_vecIDLE_StartTimeTemp;
	std::vector<CString> m_vecIDLE_EndTimeTemp;
	std::vector<CString> m_vecIDLETakeTimeTemp;
	std::vector<CString> m_vecIDLELevelTemp;
	//알람 검색전 현재 시간 확인
	SYSTEMTIME	time;
	CString current_time;
	GetLocalTime(&time);
	current_time.Format(_T("%04u-%02u-%02u"),time.wYear, time.wMonth, time.wDay);
	
	int nIndex = 0;

	m_vecErrorCode.clear();
	m_vecIDLEName.clear();
	m_vecIDLE_StartTime.clear();
	m_vecIDLE_EndTime.clear();   //2017-04-15,skcho, ENDTIME CLEAR추가 , 이전END시간이 현재 END시간에 표시되는에러 수정
	m_vecIDLEName.clear();
	m_vecIDLELevel.clear();
	m_vecIDLETakeTime.clear();

	m_vecIDLE_StartTimeTemp.clear();
	m_vecIDLENameTemp.clear();
	m_vecIDLE_EndTimeTemp.clear();
	m_vecIDLETakeTimeTemp.clear();
	m_vecIDLELevelTemp.clear();
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
				logpath.Format(_T("D:\\Cheetah\\Log\\Debug\\%s\\IDLELog.csv"),dir);
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
		
		filename.Format(IsOpenFile?_T("%s"),logpath:_T("D:\\Cheetah\\Log\\Debug\\%s\\IDLELog.csv"),m_vecDir[i]);
		errno_t e = _tfopen_s(&fStream, filename.GetString(), _T("r,ccs=UTF-8"));

		if (e != 0)
		{
			//2017-09-17,SKCHO, IDLE LOG 파일 열려있으면 메세지
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
			if(strItem!=_T("IDLE NAME"))
				m_vecIDLENameTemp.push_back(strItem);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("START TIME"))
				m_vecIDLE_StartTimeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("END TIME"))
				m_vecIDLE_EndTimeTemp.push_back(strItem);
			strItem = t.GetNextToken(_T(","));
			if(strItem!=_T("SPAN"))
				m_vecIDLETakeTimeTemp.push_back(strItem);
	
		}	
		file.Close();
	

	}
	m_vecLength= m_vecIDLE_StartTimeTemp.size();
	for(int j=0; j<m_vecLength;j++)
	{
		nIndex = m_vecLength-(j+1);
		m_vecIDLEName.push_back(m_vecIDLENameTemp[nIndex]);
		m_vecIDLE_StartTime.push_back(m_vecIDLE_StartTimeTemp[nIndex]);
		m_vecIDLE_EndTime.push_back(m_vecIDLE_EndTimeTemp[nIndex]);
		m_vecIDLETakeTime.push_back(m_vecIDLETakeTimeTemp[nIndex]);

	}
	
	
}

void CFormReport5::UpdateIDLEGrid()
{
	CString sNum;

	//m_IDLEGrid.SetRowCount(m_vecRecipe.size() + 1 );
	LOGFONT lf;
	CFont* pGridFont =m_IDLEGrid.GetFont();
	pGridFont->GetLogFont(&lf);
	sNum.Format(_T("%d"),m_vecIDLE_StartTime.size());
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_NUM,sNum);
	CGxUICtrl::SetStaticString(this, IDC_GXSTC_FILENAME,logpath);
	memcpy(lf.lfFaceName, _T("굴림체"), sizeof(_T("굴림체")));
	lf.lfHeight = 14;
	lf.lfWeight  = FW_BOLD;
	m_IDLEGrid.SetRowCount(20);
	DWORD dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER;

	for(int i = 0; i < m_IDLEGrid.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;
		else			m_IDLEGrid.SetRowHeight(i, 35);

		sNum.Format(_T("%d"),i+m_iUpDownParam);
		for(int j = 0; j < m_IDLEGrid.GetColumnCount(); j++ )
		{
			m_IDLEGrid.SetItemFont(i, j, &lf);
			m_IDLEGrid.SetItemFormat(i, j, dwTextStyle);

			if ( j == 0 )	m_IDLEGrid.SetItemText(i, j, sNum);
			else			
			{
				CString szText;
				if ( m_vecIDLE_StartTime.size() > UINT(i-1 + m_iUpDownParam) )
				{
					m_IDLEGrid.SetItemText(i, 3, m_vecIDLE_EndTime[i - 1 + m_iUpDownParam] );
					m_IDLEGrid.SetItemText(i, 2, m_vecIDLE_StartTime[i - 1 + m_iUpDownParam] );
					m_IDLEGrid.SetItemText(i, 1, m_vecIDLEName[i - 1 + m_iUpDownParam] );
					m_IDLEGrid.SetItemText(i, 4, m_vecIDLETakeTime[i - 1 + m_iUpDownParam] );
					
				}
				else
					m_IDLEGrid.SetItemText(i,j, _T(""));
			}
		}
	}
	m_IDLEGrid.ShowScrollBar(SB_HORZ,FALSE);		//	횡 스크롤 제거
	m_IDLEGrid.ShowScrollBar(SB_VERT,FALSE);		//	종 스크롤 제거
	m_IDLEGrid.Refresh();
}
BEGIN_EVENTSINK_MAP(CFormReport5, CFormView)
	ON_EVENT(CFormReport5, IDC_GXBTN_HALFDOWN, DISPID_CLICK, CFormReport5::ClickGxbtnHalfdown, VTS_NONE)
	ON_EVENT(CFormReport5, IDC_GXBTN_DOWN, DISPID_CLICK, CFormReport5::ClickGxbtnDown, VTS_NONE)
	ON_EVENT(CFormReport5, IDC_GXBTN_HALFUP, DISPID_CLICK, CFormReport5::ClickGxbtnHalfup, VTS_NONE)
	ON_EVENT(CFormReport5, IDC_GXBTN_UP, DISPID_CLICK, CFormReport5::ClickGxbtnUp, VTS_NONE)
	ON_EVENT(CFormReport5, IDC_GXBTN__LOADFILE, DISPID_CLICK, CFormReport5::ClickGxbtn, VTS_NONE)
END_EVENTSINK_MAP()


void CFormReport5::ClickGxbtnHalfdown()
{
	if ( m_iUpDownParam + 3  >= (int)m_vecIDLE_StartTime.size() )		
		return;

	m_iUpDownParam += 3;
	UpdateIDLEGrid();
}


void CFormReport5::ClickGxbtnDown()
{
	if ( m_iUpDownParam + 7 >=  (int)m_vecIDLE_StartTime.size()  )		
		return;

	m_iUpDownParam += 7;
	UpdateIDLEGrid();
	
}


void CFormReport5::ClickGxbtnHalfup()
{
	if ( m_iUpDownParam - 3  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 3;

	UpdateIDLEGrid();
}


void CFormReport5::ClickGxbtnUp()
{
	if ( m_iUpDownParam - 7  < 0 )		
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -= 7;

	UpdateIDLEGrid();
}


void CFormReport5::ClickGxbtn()
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
  ofn.lpstrFilter = "IDLELog.csv\0IDLELog.csv\0";
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
	  LoadIDLE();
	  UpdateIDLEGrid();
  } 
}
