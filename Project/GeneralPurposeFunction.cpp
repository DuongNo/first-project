#include "StdAfx.h"
#include "GeneralPurposeFunction.h"
#include "MainFrm.h"

CMainFrame* g_pMainWnd = NULL;

void SetMainWnd()
{
	//-- GET MAINFRAME POINT
	//g_pMainWnd = (CMainFrame*)::AfxGetMainWnd();
	g_pMainWnd = (CMainFrame*)AfxGetMainWnd();


}

HWND GetMainWnd()
{
	//g_pMainWnd->GetSerialManagerHandler();
	return g_pMainWnd->GetSafeHwnd();
}

CMainFrame* GetMainHandler()
{
	return g_pMainWnd;
}
