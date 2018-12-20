// DlgStatement.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgStatement.h"
#include "afxdialogex.h"

//GxGUIFrame ���
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"

// CDlgStatement ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgStatement, CDialogEx)

CDlgStatement::CDlgStatement(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgStatement::IDD, pParent)
{

}

CDlgStatement::~CDlgStatement()
{
}

void CDlgStatement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgStatement, CDialogEx)
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,	OnGUIUpdateTimer)			// View/Dialog�� ȭ�� ���ſ� Ÿ�̸��� ���� �� ���� ����


	ON_MESSAGE(UDMSG_CONTACT_JIGCNT,	OnJigCountUpdate)	
END_MESSAGE_MAP()


// CDlgStatement �޽��� ó�����Դϴ�.
// DlgStatement.cpp : ���� �����Դϴ�.
//
BEGIN_EVENTSINK_MAP(CDlgStatement, CDialogEx)
	ON_EVENT(CDlgStatement, IDC_GXLBL_OK, DISPID_CLICK, CDlgStatement::ClickGxlblOk, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblClear, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_A1_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblA1Clear, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_A2_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblA2Clear, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_B1_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblB1Clear, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_B2_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblB2Clear, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_C1_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblC1Clear, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_C2_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblC2Clear, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_D1_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblD1Clear, VTS_NONE)
	ON_EVENT(CDlgStatement, IDC_GXLBL_D2_CLEAR, DISPID_CLICK, CDlgStatement::ClickGxlblD2Clear, VTS_NONE)
END_EVENTSINK_MAP()


LRESULT CDlgStatement::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : ȭ�� ������ ǥ�� �ʱ�ȭ�� ���ְ� ���ſ� Ÿ�̸Ӹ� ����/���� �����ش�.
	BOOL bFlag = (BOOL)wParam;
	// ȭ�� ���� Ÿ�̸Ӹ� �����Ű��� �����ΰ� ?
	if ( bFlag )
	{
		SetTimer(2001, 500, NULL);
		::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
	}
	else
	{
		KillTimer(2001);
	}

	return 0;
}

void CDlgStatement::ClickGxlblOk()
{
	ShowWindow(SW_HIDE);
}

//2017-06-18 JSJUNG, JIG CONTACT COUNT �߰�
LRESULT CDlgStatement::OnJigCountUpdate(WPARAM wParam, LPARAM lParam)
{
	
	double dCONTACTRate[4][2] = {0};
	
	CString strContactTotal[4][2];
	CString strContactGood[4][2];
	
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 2 ; j++)
		{
			if((double)theProductBank.m_JigGoodCountCONTACT[i][j].iValue > 0)
			{
				dCONTACTRate[i][j] = ((double)theProductBank.m_JigGoodCountCONTACT[i][j].iValue / (double)theProductBank.m_JigTotalCountCONTACT[i][j].iValue) * 100.;
			}

			strContactTotal[i][j].Format(_T("%d"), theProductBank.m_JigTotalCountCONTACT[i][j].iValue);
			CGxUICtrl::SetStaticString(this, theProductBank.m_JigTotalCountCONTACT[i][j].uID, strContactTotal[i][j]);
	
			strContactGood[i][j].Format(_T("%d (%.1f%%)"), theProductBank.m_JigGoodCountCONTACT[i][j].iValue, dCONTACTRate[i][j] );
			CGxUICtrl::SetStaticString(this, theProductBank.m_JigGoodCountCONTACT[i][j].uID, strContactGood[i][j]);
		}

	}

	

	return 0;
}


void CDlgStatement::ClickGxlblClear()
{
	for(int i = 0 ; i < 4 ; i++)
	{
		for(int j = 0 ; j < 2 ; j++)
		{
			theProductBank.m_JigTotalCountCONTACT[i][j].iValue = 0;
			theProductBank.m_JigGoodCountCONTACT[i][j].iValue = 0;
		}
	}
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}


void CDlgStatement::ClickGxlblA1Clear()
{
	theProductBank.m_JigTotalCountCONTACT[0][0].iValue = 0;
	theProductBank.m_JigGoodCountCONTACT[0][0].iValue = 0;
	
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}


void CDlgStatement::ClickGxlblA2Clear()
{
	theProductBank.m_JigTotalCountCONTACT[0][1].iValue = 0;
	theProductBank.m_JigGoodCountCONTACT[0][1].iValue = 0;
	
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}


void CDlgStatement::ClickGxlblB1Clear()
{
	theProductBank.m_JigTotalCountCONTACT[1][0].iValue = 0;
	theProductBank.m_JigGoodCountCONTACT[1][0].iValue = 0;
	
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}


void CDlgStatement::ClickGxlblB2Clear()
{
	theProductBank.m_JigTotalCountCONTACT[1][1].iValue = 0;
	theProductBank.m_JigGoodCountCONTACT[1][1].iValue = 0;
	
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}


void CDlgStatement::ClickGxlblC1Clear()
{
	theProductBank.m_JigTotalCountCONTACT[2][0].iValue = 0;
	theProductBank.m_JigGoodCountCONTACT[2][0].iValue = 0;
	
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}


void CDlgStatement::ClickGxlblC2Clear()
{
	theProductBank.m_JigTotalCountCONTACT[2][1].iValue = 0;
	theProductBank.m_JigGoodCountCONTACT[2][1].iValue = 0;
	
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}


void CDlgStatement::ClickGxlblD1Clear()
{
	theProductBank.m_JigTotalCountCONTACT[3][0].iValue = 0;
	theProductBank.m_JigGoodCountCONTACT[3][0].iValue = 0;
	
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}


void CDlgStatement::ClickGxlblD2Clear()
{
	theProductBank.m_JigTotalCountCONTACT[3][1].iValue = 0;
	theProductBank.m_JigGoodCountCONTACT[3][1].iValue = 0;
	
	theProductBank.SaveJigCount();
	::PostMessage(m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}
