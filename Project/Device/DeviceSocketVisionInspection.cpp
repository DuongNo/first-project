#pragma once
#include "stdafx.h"
#include "SocketDialog.h"
#include "CheetahApp.h"
#include "BankGuild\RecipeBank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif

void CSocketDialog::SetVisionInspectDataProcessing(CDataSocket* pSocket)
{
	int nMsgLen = pSocket->GetLastLength();

	wchar_t wszMsg[4096];
	if(nMsgLen > 4095)
		nMsgLen = 4095;
	CEtc::ConvertUnicode(pSocket->GetLastMsg(), nMsgLen, wszMsg, 4096);
	wszMsg[nMsgLen] = NULL;
	CString strMsg;
	strMsg.Format(_T("%s"), wszMsg);

	theLog[LOG_VISION].AddBuf( _T("%s -> %s\t%s"), pSocket->GetSocketName(), SOCKET_MAIN, strMsg);

	int i = 0;
	while (strMsg.Find(_T("FDB"), 0) != -1)
	{
		if( i > 0) 
			theLog[LOG_VISION].AddBuf( _T("%dth Parsing %s"), i+1, strMsg);

		strMsg = ParseVIMessage(pSocket, (pSocket->GetSocketName() == SOCKET_ACTIVE1) ? VI_A1:VI_A2, strMsg);
		i++;
	}
	if(strMsg.GetLength() > 0)
		theLog[LOG_VISION].AddBuf(_T("Parsing After %s"), strMsg);
}

CString CSocketDialog::ParseVIMessage( CDataSocket *pSocket, int nVi, CString strMsg )
{
	BOOL bParsed = FALSE;	// 제대로 처리 됬는지 확인하는 변수
	CTokenizer t(strMsg);

	CString command = t.GetNextToken(_T("."));
	if(command == "FDB")
	{
		command = t.GetNextToken(_T("."));
		if (command == "GRAB") 
		{
			command = t.GetNextToken(_T("."));
			if (command == "START") 
			{
				command = t.GetNextToken(_T("."));
				if (command == "OK") 
				{
					bParsed = TRUE;
					command = t.GetNextToken(_T("."));

					CCellTag tagCell;
					CCellInfo* pCell = NULL;
					if (nVi == VI_A1)
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_1);
					else
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_2);

					pCell = theCellBank.GetCellInfo(tagCell);

					if(tagCell.IsExist())
					{
						if(pCell->m_Zone_C.InspVIState == eSTATE_Insp_None)
						{
							pCell->m_Zone_C.InspVIState = eSTATE_Insp_Run;
						}
					}
				}
				else if (command == "ERR") 
				{
					// GRAB.START.ERR
					// 일단 GRAB.START.OK가 날라온 것처럼 처리하자
					bParsed = TRUE;
					CCellTag tagCell;
					CCellInfo* pCell = NULL;

					if (nVi == VI_A1)
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_1);
					else
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_2);
					
					pCell = theCellBank.GetCellInfo(tagCell);
			
					if(tagCell.IsExist())
					{
						if(pCell->m_Zone_C.InspVIState == eSTATE_Insp_None)
						{
							pCell->m_Zone_C.InspVIState = eSTATE_Insp_Run;
						}
					}
				}
			}
			else if (command == "END") 
			{
				command = t.GetNextToken(_T("."));
				if (command == "OK") 
				{
					bParsed = TRUE;
					command = t.GetNextToken(_T("."));
					CCellTag tagCell;
					CCellInfo* pCell = NULL;

					if (nVi == VI_A1)
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_1);
					else
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_2);

					pCell = theCellBank.GetCellInfo(tagCell);

					if(tagCell.IsExist())
					{
						if(pCell->m_Zone_C.InspVIState != eSTATE_Insp_End)
						{
							pCell->m_Zone_C.InspVIState = eSTATE_Insp_GrabEnd;
							theLog[LOG_VISION].AddBuf( _T("%s GRAB END"), pCell->m_strInnerID);
						}
					}
				}
				else if (command == "ERR") 
				{
					// GRAB.END.ERR이 온다고 Grab이 끝난것이 아니다. [6/18/2010 OSC]
					// GRAB.END.OK는 나중에 또 날라옴
					// 일단 GRAB.START.OK가 날라온 것처럼 처리하자
					bParsed = TRUE;
					CCellTag tagCell;
					CCellInfo* pCell = NULL;

					if (nVi == VI_A1)
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_1);
					else
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_2);

					pCell = theCellBank.GetCellInfo(tagCell);

					if(tagCell.IsExist())
					{
						if(pCell->m_Zone_C.InspVIState == eSTATE_Insp_None)
							pCell->m_Zone_C.InspVIState = eSTATE_Insp_Run;
					}
				}
			}
		}
		else if(command == "CLASS")
		{
 			command = t.GetNextToken(_T("."));
			if(command == "END") 
			{
				command = t.GetNextToken(_T("."));
				if(command == "OK")
				{
					bParsed = TRUE;
					command = t.GetNextToken(_T("."));
					CCellTag tagCell;
					CCellInfo* pCell = NULL;

					tagCell = theUnitFunc.SearchAvtCell(command.Left(7), ZONE_ID_C, ZONE_ID_D);
/*
					if (nVi == VI_A1)
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_1);
					else
						tagCell = theCellBank.GetCellTag(ZONE_ID_C, JIG_CH_2);
*/
					pCell = theCellBank.GetCellInfo(tagCell);

					if(tagCell.IsExist())
					{
						pCell->m_Zone_C.InspVIState = eSTATE_Insp_ClassEnd;
						theLog[LOG_VISION].AddBuf( _T("Class.End.Ok(%s) JIG ID : %d JIG CH : %d"), pCell->m_strInnerID , pCell->m_JigId, pCell->m_JigCh);
					}
					else
						theLog[LOG_VISION].AddBuf( _T("Cell Not Found"));
				}
				else if (command == "ING") 
				{	
					bParsed = TRUE;
				}
			}
		} 
		else if(command.Left(5) == "ALARM") 
		{
			bParsed = TRUE;
		}
		else if(command == "RECIPE")
		{
			command = t.GetNextToken(_T("."));
			if (command == "CHANGE") 
			{
				command = t.GetNextToken(_T("."));
				if (command.Left(2) == "OK")
				{
					bParsed = TRUE;
				}
			}
		}
		else if(command == "MANUAL")
		{
			command = t.GetNextToken(_T("."));
			if (command.Left(7) == "INSPEND") 
			{
				bParsed = TRUE;
			}
		}
		else if(command.Left(7) == "CONNECT") 
		{
			bParsed = TRUE;
		}
		else if(command.Left(10) == "NOTCONNECT") 
		{
			bParsed = TRUE;
		}
		else if(command == "POL")
		{
			bParsed = TRUE;
		}
	}

	// 처리가 안된 문자는 로그 다시 기록
	if(bParsed == FALSE)
	{
		theLog[LOG_VISION].AddBuf( _T("Parsing Fail\t%s"), strMsg);
		return _T("");
	}

	// 두개 붙어서 나올 것을 대비해 다음 FDB까지 지운다
	return PacketClear(pSocket->GetSocketName(), strMsg);
} 
//BOOL CSocketDialog::SendMsgToVt(const VI_ACTIVE active, const int nCmdType, CString strData1)
BOOL CSocketDialog::SendMsgToVt(const VI_ACTIVE active, const int nCmdType, CString strData1, CString strData2)
{
	CString strMsg;

	if(active!=VI_A1 && active!=VI_A2)
	{
		AfxMessageBox(_T("SendMsgToVt 실패\n nIndex error!"));
		return FALSE;
	}
	const int nInx = active - VI_A1;

	if(m_pSocket_VisionInspect[active].IsConnected() == FALSE)
		return FALSE;

	switch(nCmdType)
	{
	case CMD_GRAB_START:
//		strMsg.Format( _T("MOT.GRAB.START.%s"), strData1);
		if(theConfigBank.m_Option.m_bGrabMCRUse)
			strMsg.Format( _T("MOT.GRAB.START.%s.%s"), strData1, strData2);
		else
			strMsg.Format( _T("MOT.GRAB.START.%s"), strData1);
		break;
	case CMD_VT_RECEIPE_CHANGE: 
		WriteVtRecipeInfo();
		strMsg.Format( _T("MOT.RECIPE.CHANGE"));
		break;
	default:
		return FALSE;
	}

	char szBuff[256];
	CEtc::ConvertMultibyte((wchar_t*)strMsg.GetString(), szBuff, 256);
	int nSndLen = m_pSocket_VisionInspect[active].Send(szBuff, strlen(szBuff));

	if(nSndLen==strlen(szBuff))
	{
		if(active ==VI_A1)
			theLog[LOG_VISION].AddBuf( _T("운영 -> %s\t%s"), SOCKET_ACTIVE1, strMsg);
		else if(active ==VI_A2)
			theLog[LOG_VISION].AddBuf( _T("운영 -> %s\t%s"), SOCKET_ACTIVE2, strMsg);

		return TRUE;
	}
	else
	{
		if(active ==VI_A1)
			theLog[LOG_VISION].AddBuf( _T("운영 -> %s\t전송 실패[%s]"), SOCKET_ACTIVE1, strMsg);
		else if(active ==VI_A2)
			theLog[LOG_VISION].AddBuf( _T("운영 -> %s\t전송 실패[%s]"), SOCKET_ACTIVE2, strMsg);

		return FALSE;
	}
}


BOOL CSocketDialog::WriteVtRecipeInfo()
{
	CString szPath[VI_MAX];
	CString szSystemPath[VI_MAX];
	szSystemPath[VI_A1] = _T("Z:");
	szSystemPath[VI_A2] = _T("Y:");
	CIni ini;
	for(int i = 0; i < VI_MAX; i++)
	{
		//////////////////////////////////////////////////////////////////////////
		// 폴더 확인 및 생성
		szPath[i].Format(_T("%s\\Change Recipe\\"), szSystemPath[i]);
		if(CFileSupport::DirectoryCheck(szPath[i]) == FALSE)
		{
			if(CFileSupport::CreateDirectory(szPath[i]) == FALSE)
			{
				AfxMessageBox(_T("Make sure network drive is connected"));
				return FALSE;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		szPath[i] += _T("ChangeRecipe.ini");
		ini.SetIniFilename(szPath[i]);

		ini.WriteString(_T("Current Recipe"), _T("Recipe Name"), theRecipeBank.m_strRecipeName);
		ini.WriteString(_T("Current Recipe"), _T("Insp Start At Non Contact"), _T("FALSE"));
	}
	return TRUE;
}
