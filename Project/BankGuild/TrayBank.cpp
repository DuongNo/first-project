#include "StdAfx.h"
#include "TrayBank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTrayInfo::CTrayInfo(void)
{

}
CTrayInfo::CTrayInfo( BOOL bCreate, BOOL bFull /*= FALSE*/ )
{
	Init(); 
	m_bExist = bCreate;

	// Tray 2번에 투입되는 거는 가득찬 Tray다
	if(bFull)
	{
		m_nXCnt = theRecipeBank.m_Tray.m_nCountX;
		m_nYCnt = theRecipeBank.m_Tray.m_nCountY;
		m_bFullTray = TRUE;
	}

	
}
CTrayInfo::~CTrayInfo(void)
{

}

void CTrayInfo::AddCell(CELL_POS Pos )
{
	CString strCell_Location = _T("");
	CCellInfo *pCellInfo = NULL;
	//pCellInfo = theCellBank.GetCellInfo(CELL_POS_GOOD_TRAY);

	pCellInfo = theCellBank.GetCellInfo(Pos);
	
	m_nXCnt++;
	if(m_nXCnt > theRecipeBank.m_Tray.m_nCountX)
	{
		m_nYCnt++;
		m_nXCnt = 1;
	}

	//2017-03-03,skcho,good tray에서만 사용
	if(Pos == CELL_POS_GOOD_TRAY)
	{
		if(theProcBank.GOODTrayBitX == 1)
		{
			strCell_Location.Format(_T("A0%d"),theProcBank.GOODTrayBitY);
		}
		else
		{
			strCell_Location.Format(_T("B0%d"),theProcBank.GOODTrayBitY);
		}

		// Tray 놓을 Bit 카운트. Cell 갯수 카운트랑 변수를 같이 사용 하려 했으나 계속 버그 발생 하여 따로 만듬..
		//2017-03-05,skcho, cell 위치(strCell_Location)와 맞게 하기 위해서 addcell로 이동
		theProcBank.GOODTrayBitX++;				
		if(theProcBank.GOODTrayBitX > theRecipeBank.m_Tray.m_nCountX)
		{
			theProcBank.GOODTrayBitX = 1;
			theProcBank.GOODTrayBitY++;
		}
		//if(theProcBank.GOODTrayBitY > theRecipeBank.m_Tray.m_nCountY)
		//	theProcBank.GOODTrayBitY = 1;	
		//20170227 byskcho
		//cell 정보를 tray에 담을때 구조체에 데이터 저장
		m_pCarrierInfo.m_stCellInfo[m_nCellInCount].m_strCell_ID = pCellInfo->m_strCellID;
		m_pCarrierInfo.m_stCellInfo[m_nCellInCount].m_strJudge = _T("G");//pCellInfo->m_LastResult; //GOOD이므로 "G"로 보냄 
		m_pCarrierInfo.m_stCellInfo[m_nCellInCount].m_strCell_Location = strCell_Location; 
		m_pCarrierInfo.m_stCellInfo[m_nCellInCount].m_strReasonCode = _T("");
	
		m_nCellInCount++;

		m_pCarrierInfo.m_nCell_Qty= m_nCellInCount;
	}

	//2017-03-11,skcho attribute 요청에서 받은 데이터로 대체
	//int nMaxCell = theRecipeBank.m_Tray.m_nCountX*theRecipeBank.m_Tray.m_nCountY;
	int nMaxCell = theConfigBank.m_UnloadRunInfo.GetCellInTrayQtyInt(); 

	//2017-03-05,skcho 전역변수로 셀 위치 카운트 변경
	int nNowCell = m_nXCnt*m_nYCnt;
	theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("X : %d Y: %d NOW : %d MAX : %d  - %d"),m_nXCnt,m_nYCnt,nNowCell,nMaxCell,m_nCellInCount );
	//int nNowCell = theProcBank.GOODTrayBitX*theProcBank.GOODTrayBitY;
	//theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("X : %d Y: %d NOW : %d MAX : %d "),theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY,nNowCell,nMaxCell );
	if(nNowCell >= nMaxCell)
	{
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("TRAY CELL FULL X: %d, Y: %d"),m_nXCnt,m_nYCnt);
		//theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("TRAY CELL FULL X: %d, Y: %d"),theProcBank.GOODTrayBitX,theProcBank.GOODTrayBitY);
		m_bFullTray = TRUE;		
	}
}

//2018wjp
void CTrayInfo::AddCell(CELL_POS Pos,CString m_strCellID,int iRow,int iCol)
{
	CString strCell_Location = _T("");
	m_nXCnt++;
	if(m_nXCnt > theRecipeBank.m_Tray.m_nCountX)
	{
		m_nYCnt++;
		m_nXCnt = 1;
	}

	if(Pos == CELL_POS_GOOD_TRAY)
	{
		if(iRow == 1)
		{
			strCell_Location.Format(_T("A0%d"),iCol);
		}
		else
		{
			strCell_Location.Format(_T("B0%d"),iCol);
		}
		m_pCarrierInfo.m_stCellInfo[m_nCellInCount].m_strCell_ID = m_strCellID;
		m_pCarrierInfo.m_stCellInfo[m_nCellInCount].m_strJudge = _T("G");
		m_pCarrierInfo.m_stCellInfo[m_nCellInCount].m_strCell_Location = strCell_Location; 
		m_pCarrierInfo.m_stCellInfo[m_nCellInCount].m_strReasonCode = _T("");

		m_nCellInCount++;

		m_pCarrierInfo.m_nCell_Qty= m_nCellInCount;
	}

	int nMaxCell = theConfigBank.m_UnloadRunInfo.GetCellInTrayQtyInt(); 
	int nNowCell = m_nXCnt*m_nYCnt;
	theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("X : %d Y: %d NOW : %d MAX : %d  - %d"),m_nXCnt,m_nYCnt,nNowCell,nMaxCell,m_nCellInCount );
	if(nNowCell >= nMaxCell)
	{
		theLog[LOG_SEQUENCE_TRAY].AddBuf( _T("TRAY CELL FULL X: %d, Y: %d"),m_nXCnt,m_nYCnt);
		m_bFullTray = TRUE;		
	}
}

void CTrayInfo::MinusCell()
{
	m_bFullTray = FALSE;
	if(m_nXCnt == 1)
	{
		m_nYCnt--;
		m_nXCnt = theRecipeBank.m_Tray.m_nCountX;
	}
	else
	{
		m_nXCnt--;
	}
	//2018wjp
	m_nCellInCount--;
	m_nCellInCount=m_nCellInCount<=0?0:m_nCellInCount;
	m_pCarrierInfo.m_nCell_Qty= m_nCellInCount;
}

void CTrayInfo::SetCellID(int Pos, CString strCellID)
{
	m_strTrayCellID[Pos] = strCellID;
}

CTrayBank::CTrayBank(void)
{
	InitTrayInfo();
}


CTrayBank::~CTrayBank(void)
{
}

void CTrayBank::InitTrayInfo()
{
	for(int i = 0; i < TRAY_PORT_MAX; i++)
	{
		for(int j = 0; j < TRAY_INFO_POS_MAX; j++)
			m_TrayInfo[i][j].Init();
	}

	theProcBank.GOODTrayBitX = 1;
	theProcBank.GOODTrayBitY = 1;
	theProcBank.NGTrayBitX = 1;
	theProcBank.NGTrayBitY = 1;
}

CTrayInfo* CTrayBank::GetTrayInfo( TRAY_PORT port, TRAY_INFO_POS pos )
{
	return &m_TrayInfo[port][pos];
}

CTrayInfo CTrayBank::RemoveTrayInfo( TRAY_PORT port, TRAY_INFO_POS pos )
{
	CTrayInfo info = m_TrayInfo[port][pos];
	m_TrayInfo[port][pos].Init();
	return info;
}

void CTrayBank::SetTrayInfo( TRAY_PORT port, TRAY_INFO_POS pos, CTrayInfo info )
{
	m_TrayInfo[port][pos] = info;
}

//2017-03-05, skcho, carrier data 복사
void CTrayBank::Copy_CarrierInfo(TRAY_PORT port, TRAY_INFO_POS frompos,TRAY_INFO_POS topos)
{
	m_TrayInfo[port][topos].m_pCarrierInfo = m_TrayInfo[port][frompos].m_pCarrierInfo; 
}
/*void CTrayBank::SetCarrierInfo(TRAY_PORT port, TRAY_INFO_POS pos,int nCEID)
{
	CString str;
	int nTrayCnt = 0;
	static int nTotalCell = 0;  //2017-03-05,skcho 셀 개수 누적 카운트 변수
	static CString strTrayID = _T("");

	nTrayCnt = theProcBank.GetGoodAlignCount();

	//2017-05-13, skcho, 위치 이동
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_PPID = "0";
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Product = "0";
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_StepID = "0";

	m_TrayInfo[port][pos].m_pCarrierInfo.m_strPortNo = "U"; //2017-04-14,skcho, CIM QUAL - 포트넘보 U 픽스
	str.Format(_T("%d"),nCEID);
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCEID = str;

	//뚜껑 열때 한번만 실행
	if(nCEID == TRAY_RELEASE)  //release
	{
		m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID =theProcBank.m_strTrayID;
		m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID = "0";

		//2017-03-09, skcho, jobchange에 의한 보고 이면 13 아니면 11
		//if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)//PPID가 다른경우 기존 검사제품 lot end 처리
	   //	{
		//	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "13";
		//}
		//else
		//{
			m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "11";
		//}

		m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count= 0;//nTrayCnt-1;
		//삭제
		//lot end나 job change로 덮기 보고할때
		if(theProcBank.m_bTrayCloseRelease)
		{
			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = nTotalCell;	
			if(nTrayCnt <= 2)		
			{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID = theProcBank.m_structSubCarrierID[0].m_strSubCarrier_ID;
				m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty = nTotalCell;	
			}
		}
		else
		{
			//정상적인 lot 개수로 보고할때 덮기 보고
			//--if(nTrayCnt > 1)
			//--{		
			//--	m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = nTotalCell;				
			//--}
			//--else
			//--{
				//처음 뚜껑열기보고	
				m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = 0;
				m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty = 0;
				nTotalCell = 0;
				strTrayID = theProcBank.m_strTrayID;
				theProcBank.m_strTrayOpenRelease = theProcBank.m_strTrayID;
			//--}
		//}
		//위치 이동
		WriteCarrierReleaseInfo(port,pos);  //2017-02-27, sckho, 트레이 정보 ini 파일에 쓰기
		theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_RELEASE_REQUEST,NULL);
	}
	else //assing
	{
		//뚜껑 덮기 보고
		if(theProcBank.m_bCarrierAssign == TRUE)
		{
			//--m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID = theProcBank.m_strTrayID; //뚜껑아이디
			//--theProcBank.m_strCloseTrayID = theProcBank.m_strTrayID;
		}
		else
		{
			//--m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID =  _T("");
			//--m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID = theProcBank.m_strTrayID;
		}
		
		//--if(nTrayCnt > MAX_STRUCT-1) nTrayCnt = 1;
		//--theProcBank.m_structSubCarrierID[nTrayCnt-1].m_strSubCarrier_ID = theProcBank.m_strTrayID;  //2017-03-09,skcho, 서브 캐리어 ID 저장 배열

		//--str.Format(_T("%d"),m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty);
		//--theProcBank.m_structSubCarrierID[nTrayCnt-1].m_strCellQty = str;

			//뚜껑 덮기 보고
		if(theProcBank.m_bCarrierAssign == TRUE)
		{
			m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID = theProcBank.m_strTrayID; //뚜껑아이디
			theProcBank.m_strCloseTrayID = theProcBank.m_strTrayID;

			//뚜껑보고 LotEnd, jobchange에 의한 보고 이면 13 아니면11. 20170512, JSJUNG
			if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)//PPID가 다른경우 기존 검사제품 lot end 처리
	   		{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "13";
			}
			else
			{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "11";
			}

			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count= nTrayCnt-1;
			
			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = nTotalCell;

			WriteCarrierCloseInfo(port,pos); 
			theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_CLOSE_REQUEST,NULL);

		}
		else
		{

			m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID =  _T("");
			m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID = theProcBank.m_strTrayID;


			if(nTrayCnt > MAX_STRUCT-1) nTrayCnt = 1;
			theProcBank.m_structSubCarrierID[nTrayCnt-1].m_strSubCarrier_ID = theProcBank.m_strTrayID;  //2017-03-09,skcho, 서브 캐리어 ID 저장 배열

			str.Format(_T("%d"),m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty);
			theProcBank.m_structSubCarrierID[nTrayCnt-1].m_strCellQty = str;

			//2017-03-09, skcho, jobchange에 의한 보고 이면 3 아니면 1
			if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)//PPID가 다른경우 기존 검사제품 lot end 처리
	   		{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "3";
			}
			else
			{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "1";
			}

			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count= 1;	

			//assign sub carrier에서는 총수량과 셀 수량을 같게 함 - 확인필요
			//str.Format(_T("%d"),m_TrayInfo[port][pos].GetCellCount());
			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty;		

			//2017-03-05,skcho, 셀 개수 누적카운트
			nTotalCell += m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty;

			WriteCarrierAssignInfo(port,pos);  //2017-02-27, sckho, 트레이 정보 ini 파일에 쓰기
			theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_ASSIGN_REQUEST,NULL);
		}

		//--m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count= 1;

		//assign sub carrier에서는 총수량과 셀 수량을 같게 함 - 확인필요
		//str.Format(_T("%d"),m_TrayInfo[port][pos].GetCellCount());
	    //--m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty;		

		//2017-03-05,skcho, 셀 개수 누적카운트
		//--nTotalCell += m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty;
		
}

	//2017-03-25,skcho, Release보고와 Assign에 필요한 파일 분리
	//--if(nCEID == TRAY_RELEASE)
	//--{
		//--WriteCarrierReleaseInfo(port,pos);  //2017-02-27, sckho, 트레이 정보 ini 파일에 쓰기
	//--}
	//--else if(nCEID == TRAY_ASSIGN)
	//--{
		//--WriteCarrierAssignInfo(port,pos);  //2017-02-27, sckho, 트레이 정보 ini 파일에 쓰기
	//--}
	
}*/
void CTrayBank::SetCarrierInfo(TRAY_PORT port, TRAY_INFO_POS pos,int nCEID)
{
	CString str;
	int nTrayCnt = 0;
	static int nTotalCell = 0;  //2017-03-05,skcho 셀 개수 누적 카운트 변수
	static CString strTrayID = _T("");

	//2017-07-07,SKCHO, 262중간보고 안하기 위해서 신규 변수 추가
	//--nTrayCnt = theProcBank.GetGoodAlignCount();
	nTrayCnt = theProcBank.GetSubGoodAlignCount();


	//2017-05-13, skcho, 위치 이동
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_PPID = "0";
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Product = "0";
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_StepID = "0";

	// U -> UI01로 변경. 2017/06/12 HSK.
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strPortNo = "UI01"; //2017-04-14,skcho, CIM QUAL - 포트넘보 U 픽스
	str.Format(_T("%d"),nCEID);
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCEID = str;

	//뚜껑 열때 한번만 실행
	if(nCEID == TRAY_RELEASE)  //release
	{
		m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID =theProcBank.m_strTrayID;
		m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID = "0";

		m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "11";
	
		m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count= 0;//nTrayCnt-1;
	
		//처음 뚜껑열기보고	
		m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = 0;
		m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty = 0;
		nTotalCell = 0;
		strTrayID = theProcBank.m_strTrayID;

		//2017-07-04,SKCHO, 351가 262 Release보고 보다 먼저 하므로 351보고 tray id에 이전 값이 들어가므로 위치 변경하고 삭제함 
		//2017-05-13 JSJUNG, 뚜껑 열기 351보고 
		//theProcBank.m_strTrayOpenRelease = m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID;//theProcBank.m_strTrayID;

		//위치 이동
		WriteCarrierReleaseInfo(port,pos);  //2017-02-27, sckho, 트레이 정보 ini 파일에 쓰기
		theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_RELEASE_REQUEST,NULL);
	}
	//2018wjpCIM
	else if(nCEID==TRAY_CELLID)
	{
		m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID =theProcBank.m_strTrayID;
		//m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID = "0";

		m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "1";
	
		m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count= 1;
	
		//처음 뚜껑열기보고	
		m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = 0;
		m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty = 0;
		nTotalCell = 0;
		strTrayID = "";

		WriteCarrierReqCellIDInfo(port,pos);  
		theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_GET_CARRIER_CELLID_REQUEST,NULL);
	}
	else //assing
	{
		//뚜껑 덮기 보고
		if(theProcBank.m_bCarrierAssign == TRUE)
		{
			m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID = theProcBank.m_strTrayID; //뚜껑아이디
			theProcBank.m_strCloseTrayID = theProcBank.m_strTrayID;

			//뚜껑보고 LotEnd, jobchange에 의한 보고 이면 13 아니면11. 20170512, JSJUNG
			if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)//PPID가 다른경우 기존 검사제품 lot end 처리
	   		{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "13";
			}
			else
			{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "11";
			}

			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count= nTrayCnt-1;
			
			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = nTotalCell;

			WriteCarrierCloseInfo(port,pos); //2017-05-13,skcho ,신규생성
			theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_CLOSE_REQUEST,NULL);

		}
		else
		{
			m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID =  _T("");
			m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID = theProcBank.m_strTrayID;

			if(nTrayCnt > MAX_STRUCT-1 || nTrayCnt == 0) nTrayCnt = 1;

			theProcBank.m_structSubCarrierID[nTrayCnt-1].m_strSubCarrier_ID = theProcBank.m_strTrayID;  //2017-03-09,skcho, 서브 캐리어 ID 저장 배열

			str.Format(_T("%d"),m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty);
			theProcBank.m_structSubCarrierID[nTrayCnt-1].m_strCellQty = str;

			//2017-03-09, skcho, jobchange에 의한 보고 이면 3 아니면 1
			if(theConfigBank.m_Option.m_bUseLotEnd || theConfigBank.m_UnloadRunInfo.GetJobChangeType() == JOBCHANGE_CASE1)//PPID가 다른경우 기존 검사제품 lot end 처리
	   		{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "3";
			}
			else
			{
				m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type = "1";
			}

			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count= 1;	

			//assign sub carrier에서는 총수량과 셀 수량을 같게 함 - 확인필요
			//str.Format(_T("%d"),m_TrayInfo[port][pos].GetCellCount());
			m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count = m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty;		

			//2017-03-05,skcho, 셀 개수 누적카운트
			nTotalCell += m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty;

			WriteCarrierAssignInfo(port,pos);  //2017-02-27, sckho, 트레이 정보 ini 파일에 쓰기
			theSocketInterFace.SendCmdCarrierAssignToDataPC(EQUIP_SET_CARRIER_ASSIGN_REQUEST,NULL);
		}	
		
	}
	
}

//2018wjpCIM
void CTrayBank::WriteCarrierReqCellIDInfo(TRAY_PORT port, TRAY_INFO_POS pos)
{
	CString strCellNum = _T("");
	CString strCarrierNum = _T("");
	int nCellQty = 0;	
	int nSubCarrierQty = 0;
	int i = 0;

	//디렉토리 없을 때 파일 생성 안되어 추가 2017/05/04 HSK.
	if(CFileSupport::DirectoryCheck(_T("D:\\Cheetah\\Runtime\\")) == FALSE)
	{
		if(CFileSupport::CreateDirectory(_T("D:\\Cheetah\\Runtime\\")) == FALSE)
			return;
	}

	ClearCarrierReqCellIDInfo();

	CIni ini(theConfigBank.m_System.m_strCarrierReqCellIDReportPath);
	m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID=theProcBank.m_strTrayID;
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_id")			, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_type")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_ppid")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_PPID);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_product")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Product);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_stepid")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_StepID);
	ini.WriteLong(_T("CARRIER LIST")		, _T("Carrier_s_count")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count);
	ini.WriteLong(_T("CARRIER LIST")		, _T("Carrier_c_count")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count);
	ini.WriteString(_T("CARRIER LIST")		, _T("Portno")				, m_TrayInfo[port][pos].m_pCarrierInfo.m_strPortNo);

	nSubCarrierQty = m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count;
	ini.WriteLong(_T("SUB CARRIER LIST")		, _T("Carrier_s_count")		, nSubCarrierQty	);
	 
	if(nSubCarrierQty == 1)
	{
		ini.WriteString(_T("SUB CARRIER LIST")		, _T("Sub_carrier_id")	, m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID);
		ini.WriteLong(_T("SUB CARRIER LIST")		, _T("Cell_qty")				, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty	);
	}
	else
	{
		for(i = 0; i< nSubCarrierQty; i++)
		{
			strCarrierNum.Format(_T("Sub_carrier_id%d"),i);
			ini.WriteString(_T("SUB CARRIER LIST")		, strCarrierNum	, theProcBank.m_structSubCarrierID[i].m_strSubCarrier_ID);
		    
			strCarrierNum.Format(_T("Sub_carrier_Cell%d"),i);
			ini.WriteString(_T("SUB CARRIER LIST")		, strCarrierNum	, theProcBank.m_structSubCarrierID[i].m_strCellQty);
		
		}
	}	

}

//2017-03-25,skcho, Release보고와 Assign에 필요한 파일 분리
//2017-02-27, sckho, 트레이 정보 ini 파일 생성
void CTrayBank::WriteCarrierReleaseInfo(TRAY_PORT port, TRAY_INFO_POS pos)
{
	CString strCellNum = _T("");
	CString strCarrierNum = _T("");
	int nCellQty = 0;	
	int nSubCarrierQty = 0;
	int i = 0;

	//디렉토리 없을 때 파일 생성 안되어 추가 2017/05/04 HSK.
	if(CFileSupport::DirectoryCheck(_T("D:\\Cheetah\\Runtime\\")) == FALSE)
	{
		if(CFileSupport::CreateDirectory(_T("D:\\Cheetah\\Runtime\\")) == FALSE)
			return;
	}

	ClearCarrierReleaseInfo();

	CIni ini(theConfigBank.m_System.m_strCarrierReleaseReportPath);

	//duong
	//m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID=_T("1234567");

	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_id")			, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_type")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_ppid")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_PPID);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_product")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Product);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_stepid")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_StepID);
	ini.WriteLong(_T("CARRIER LIST")		, _T("Carrier_s_count")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count);
	ini.WriteLong(_T("CARRIER LIST")		, _T("Carrier_c_count")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count);
	ini.WriteString(_T("CARRIER LIST")		, _T("Portno")				, m_TrayInfo[port][pos].m_pCarrierInfo.m_strPortNo);

	nSubCarrierQty = m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count;
	ini.WriteLong(_T("SUB CARRIER LIST")		, _T("Carrier_s_count")		, nSubCarrierQty	);
	 
	if(nSubCarrierQty == 1)
	{
		ini.WriteString(_T("SUB CARRIER LIST")		, _T("Sub_carrier_id")	, m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID);
		ini.WriteLong(_T("SUB CARRIER LIST")		, _T("Cell_qty")				, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty	);
	}
	else
	{
		for(i = 0; i< nSubCarrierQty; i++)
		{
			strCarrierNum.Format(_T("Sub_carrier_id%d"),i);
			ini.WriteString(_T("SUB CARRIER LIST")		, strCarrierNum	, theProcBank.m_structSubCarrierID[i].m_strSubCarrier_ID);
		    
			strCarrierNum.Format(_T("Sub_carrier_Cell%d"),i);
			ini.WriteString(_T("SUB CARRIER LIST")		, strCarrierNum	, theProcBank.m_structSubCarrierID[i].m_strCellQty);
		
		}
	}	

}
void CTrayBank::WriteCarrierCloseInfo(TRAY_PORT port, TRAY_INFO_POS pos)
{
	CString strCellNum = _T("");
	CString strCarrierNum = _T("");
	int nCellQty = 0;	
	int nSubCarrierQty = 0;
	int i = 0;

	//디렉토리 없을 때 파일 생성 안되어 추가 2017/05/04 HSK.
	if(CFileSupport::DirectoryCheck(_T("D:\\Cheetah\\Runtime\\")) == FALSE)
	{
		if(CFileSupport::CreateDirectory(_T("D:\\Cheetah\\Runtime\\")) == FALSE)
			return;
	}

	ClearCarrierCloseInfo();

	CIni ini(theConfigBank.m_System.m_strCarrierCloseReportPath);

	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_id")			, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_type")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_ppid")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_PPID);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_product")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Product);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_stepid")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_StepID);
	ini.WriteLong(_T("CARRIER LIST")		, _T("Carrier_s_count")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count);
	ini.WriteLong(_T("CARRIER LIST")		, _T("Carrier_c_count")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count);
	ini.WriteString(_T("CARRIER LIST")		, _T("Portno")				, m_TrayInfo[port][pos].m_pCarrierInfo.m_strPortNo);

	nSubCarrierQty = m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count;
	ini.WriteLong(_T("SUB CARRIER LIST")		, _T("Carrier_s_count")		, nSubCarrierQty	);
	 
	//2017-05-21,skcho, 260보고시 subcarrier 개수가 1일 경우 carrier id 보고 안되는 버그 수정 삭제
	//if(nSubCarrierQty == 1)
	//{
	//	ini.WriteString(_T("SUB CARRIER LIST")		, _T("Sub_carrier_id")	, m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID);
	//	ini.WriteLong(_T("SUB CARRIER LIST")		, _T("Cell_qty")				, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty	);
	//}
	//else
	//{
		for(i = 0; i< nSubCarrierQty; i++)
		{
			strCarrierNum.Format(_T("Sub_carrier_id%d"),i);
			ini.WriteString(_T("SUB CARRIER LIST")		, strCarrierNum	, theProcBank.m_structSubCarrierID[i].m_strSubCarrier_ID);
		    
			strCarrierNum.Format(_T("Sub_carrier_Cell%d"),i);
			ini.WriteString(_T("SUB CARRIER LIST")		, strCarrierNum	, theProcBank.m_structSubCarrierID[i].m_strCellQty);
		
		}
	//}	

}
//2017-03-25,skcho, Release보고와 Assign에 필요한 파일 분리
//2017-02-27, sckho, 트레이 정보 ini 파일 생성
void CTrayBank::WriteCarrierAssignInfo(TRAY_PORT port, TRAY_INFO_POS pos)
{
	CString strCellNum = _T("");
	CString strCarrierNum = _T("");
	int nCellQty = 0;	
	int nSubCarrierQty = 0;
	int i = 0;

	//디렉토리 없을 때 파일 생성 안되어 추가 2017/05/04 HSK.
	if(CFileSupport::DirectoryCheck(_T("D:\\Cheetah\\Runtime\\")) == FALSE)
	{
		if(CFileSupport::CreateDirectory(_T("D:\\Cheetah\\Runtime\\")) == FALSE)
			return;
	}

	ClearCarrierAssignInfo();

	CIni ini(theConfigBank.m_System.m_strCarrierAssignReportPath);

	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_id")			, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_ID);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_type")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Type);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_ppid")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_PPID);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_product")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_Product);
	ini.WriteString(_T("CARRIER LIST")		, _T("Carrier_stepid")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_strCarrier_StepID);
	ini.WriteLong(_T("CARRIER LIST")		, _T("Carrier_s_count")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count);
	ini.WriteLong(_T("CARRIER LIST")		, _T("Carrier_c_count")		, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_C_Count);
	ini.WriteString(_T("CARRIER LIST")		, _T("Portno")				, m_TrayInfo[port][pos].m_pCarrierInfo.m_strPortNo);

	nSubCarrierQty = m_TrayInfo[port][pos].m_pCarrierInfo.m_nCarrier_S_Count;
	ini.WriteLong(_T("SUB CARRIER LIST")		, _T("Carrier_s_count")		, nSubCarrierQty	);
	 
	ini.WriteString(_T("SUB CARRIER LIST")		, _T("Sub_carrier_id")	, m_TrayInfo[port][pos].m_pCarrierInfo.m_strSubCarrier_ID);

	ini.WriteLong(_T("SUB CARRIER LIST")		, _T("Cell_qty")				, m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty	);
	
	nCellQty = m_TrayInfo[port][pos].m_pCarrierInfo.m_nCell_Qty;
	for( i = 0; i < nCellQty; i++)
	{
		strCellNum.Format(_T("CELL%d"),i);
		ini.WriteString(strCellNum		, _T("Cell id")						, m_TrayInfo[port][pos].m_pCarrierInfo.m_stCellInfo[i].m_strCell_ID			);
		ini.WriteString(strCellNum		, _T("Location no")					, m_TrayInfo[port][pos].m_pCarrierInfo.m_stCellInfo[i].m_strCell_Location			);
		ini.WriteString(strCellNum		, _T("Judge")						, m_TrayInfo[port][pos].m_pCarrierInfo.m_stCellInfo[i].m_strJudge			);
		ini.WriteString(strCellNum		, _T("Reasoncode")					, m_TrayInfo[port][pos].m_pCarrierInfo.m_stCellInfo[i].m_strReasonCode			);
	}


}

//2018wjpCIM
void CTrayBank::ClearCarrierReqCellIDInfo()
{
	SYSTEMTIME timeBk;

	CString strBk1,strBk,szFileDate,szFileTime;

	GetLocalTime(&timeBk);

	szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeBk.wYear, (WORD)timeBk.wMonth, (WORD)timeBk.wDay);
	strBk.Format(_T("D:\\Cheetah\\Runtime\\%s\\"),szFileDate);

	if(CFileSupport::DirectoryCheck(strBk) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strBk) == FALSE)
			return;
	}

	szFileTime.Format(_T("%02u%02u%02u-%03u"),timeBk.wHour,timeBk.wMinute,timeBk.wSecond,timeBk.wMilliseconds);

	strBk1.Format(_T("%s%s_Carrier_Req_CellID_Report_bk.ini"),strBk,szFileTime);

	if(CFileSupport::FileCheck(theConfigBank.m_System.m_strCarrierReqCellIDReportPath))
	{
		::CopyFile(theConfigBank.m_System.m_strCarrierReqCellIDReportPath, strBk1, FALSE);
		::DeleteFile(theConfigBank.m_System.m_strCarrierReqCellIDReportPath);
	}
}

void CTrayBank::ClearCarrierReleaseInfo()
{	
	SYSTEMTIME timeBk;
	
	CString strBk1,strBk,szFileDate,szFileTime;

	GetLocalTime(&timeBk);

	szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeBk.wYear, (WORD)timeBk.wMonth, (WORD)timeBk.wDay);
	strBk.Format(_T("D:\\Cheetah\\Runtime\\%s\\"),szFileDate);
	
	if(CFileSupport::DirectoryCheck(strBk) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strBk) == FALSE)
			return;
	}

	szFileTime.Format(_T("%02u%02u%02u-%03u"),timeBk.wHour,timeBk.wMinute,timeBk.wSecond,timeBk.wMilliseconds);
	
	strBk1.Format(_T("%s%s_Carrier_ReleaseReport_bk.ini"),strBk,szFileTime);

	if(CFileSupport::FileCheck(theConfigBank.m_System.m_strCarrierReleaseReportPath))
	{
		::CopyFile(theConfigBank.m_System.m_strCarrierReleaseReportPath, strBk1, FALSE);
		::DeleteFile(theConfigBank.m_System.m_strCarrierReleaseReportPath);
	}

}

void CTrayBank::ClearCarrierCloseInfo()
{	
	SYSTEMTIME timeBk;
	
	CString strBk1,strBk,szFileDate,szFileTime;

	GetLocalTime(&timeBk);

	szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeBk.wYear, (WORD)timeBk.wMonth, (WORD)timeBk.wDay);
	strBk.Format(_T("D:\\Cheetah\\Runtime\\%s\\"),szFileDate);
	
	if(CFileSupport::DirectoryCheck(strBk) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strBk) == FALSE)
			return;
	}

	szFileTime.Format(_T("%02u%02u%02u-%03u"),timeBk.wHour,timeBk.wMinute,timeBk.wSecond,timeBk.wMilliseconds);

	strBk1.Format(_T("%s%s_Carrier_CloseReport_bk.ini"),strBk,szFileTime);

	if(CFileSupport::FileCheck(theConfigBank.m_System.m_strCarrierCloseReportPath))
	{
		::CopyFile(theConfigBank.m_System.m_strCarrierCloseReportPath, strBk1, FALSE);
		::DeleteFile(theConfigBank.m_System.m_strCarrierCloseReportPath);
	}
}

void CTrayBank::ClearCarrierAssignInfo()
{	
	SYSTEMTIME timeBk;
	
	CString strBk1,strBk,szFileDate,szFileTime;

	GetLocalTime(&timeBk);

	szFileDate.Format(_T("%04u-%02u-%02u"),(WORD)timeBk.wYear, (WORD)timeBk.wMonth, (WORD)timeBk.wDay);
	strBk.Format(_T("D:\\Cheetah\\Runtime\\%s\\"),szFileDate);
	
	if(CFileSupport::DirectoryCheck(strBk) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strBk) == FALSE)
			return;
	}

	szFileTime.Format(_T("%02u%02u%02u-%03u"),timeBk.wHour,timeBk.wMinute,timeBk.wSecond,timeBk.wMilliseconds);
	strBk1.Format(_T("%s%s_Carrier_AssignReport_bk.ini"),strBk,szFileTime);
	
	if(CFileSupport::FileCheck(theConfigBank.m_System.m_strCarrierAssignReportPath))
	{
		::CopyFile(theConfigBank.m_System.m_strCarrierAssignReportPath, strBk1, FALSE);
		::DeleteFile(theConfigBank.m_System.m_strCarrierAssignReportPath);
	}
}
