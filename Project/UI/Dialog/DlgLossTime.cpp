// DlgLossTime.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "UI\Dialog\DlgLossTime.h"
#include "afxdialogex.h"
#include "UI\Dialog\DlgLossTimeList.h"
#include "UI\GausGUI\GxStaticEx.h"
// DlgLossTime 대화 상자입니다.

IMPLEMENT_DYNAMIC(DlgLossTime, CDialog)

DlgLossTime::DlgLossTime(CWnd* pParent /*=NULL*/)
	: CDialog(DlgLossTime::IDD, pParent)
{

}

DlgLossTime::~DlgLossTime()
{
}

void DlgLossTime::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgLossTime, CDialog)
END_MESSAGE_MAP()


// DlgLossTime 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(DlgLossTime, CDialog)
	ON_EVENT(DlgLossTime, IDC_GXBTN_LOSSTIME_FIRST, DISPID_CLICK, DlgLossTime::ClickGxbtnLosstimeFirst, VTS_NONE)
	ON_EVENT(DlgLossTime, IDC_GXBTN_LOSSTIME_SECOND, DISPID_CLICK, DlgLossTime::ClickGxbtnLosstimeSecond, VTS_NONE)
	ON_EVENT(DlgLossTime, IDC_GXBTN_LOSSTIME_THIRD, DISPID_CLICK, DlgLossTime::ClickGxbtnLosstimeThird, VTS_NONE)
	ON_EVENT(DlgLossTime, IDC_GXBTN_LOSSTIME_SELECT, DISPID_CLICK, DlgLossTime::ClickGxbtnLosstimeSelect, VTS_NONE)
END_EVENTSINK_MAP()


void DlgLossTime::ClickGxbtnLosstimeFirst()
{
	theProcBank.Loss_info.bFirstClass = TRUE;
	theProcBank.Loss_info.bSecondClass = FALSE;
	theProcBank.Loss_info.bThirdClass = FALSE;
	DIgLossTimeList Dlg;
	Dlg.DoModal();

	CGxStaticEx* str = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LOSSTIME_FIRST);
	str->SetCaption(theProcBank.Loss_info.strFirstClass);

	ClickGxbtnLosstimeSecond();
}


void DlgLossTime::ClickGxbtnLosstimeSecond()
{
	if(theProcBank.Loss_info.strFirstClass == _T(""))
		return;
	theProcBank.Loss_info.bFirstClass = FALSE;
	theProcBank.Loss_info.bSecondClass = TRUE;
	theProcBank.Loss_info.bThirdClass = FALSE;
	DIgLossTimeList Dlg;
	Dlg.DoModal();

	CGxStaticEx* str = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LOSSTIME_SECOND);
	str->SetCaption(theProcBank.Loss_info.strSecondClass);

	ClickGxbtnLosstimeThird();
}


void DlgLossTime::ClickGxbtnLosstimeThird()
{
	if(theProcBank.Loss_info.strSecondClass == _T(""))
		return;
	theProcBank.Loss_info.bFirstClass = FALSE;
	theProcBank.Loss_info.bSecondClass = FALSE;
	theProcBank.Loss_info.bThirdClass = TRUE;
	DIgLossTimeList Dlg;
	Dlg.DoModal();

	CGxStaticEx* str = (CGxStaticEx*)GetDlgItem(IDC_GXSTC_LOSSTIME_THIRD);
	str->SetCaption(theProcBank.Loss_info.strThirdClass);
}


void DlgLossTime::ClickGxbtnLosstimeSelect()
{
	if(theProcBank.Loss_info.strThirdClass == _T(""))
		return;

	OnOK();
}
