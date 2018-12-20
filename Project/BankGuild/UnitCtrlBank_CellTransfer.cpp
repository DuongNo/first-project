#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL	CUnitCtrlBank::CheckBufferStage_OnCell(int nChannelNumber)
{
	BOOL bRet = FALSE;

	return bRet;
}

int		CUnitCtrlBank::GetCellTransferPosition()
{
	//Cell Transfer의 위치를 반환.
	int nRet = 0;

	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_BUFFER_STG) == TRUE)
		nRet = CELLTR_BUFFER_POS;
	else if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_MCR) == TRUE)
		nRet = CELLTR_MCR_POS;
	else if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_PREALIGN_STG) == TRUE)
		nRet = CELLTR_PREALIGN_POS;

	return nRet;
}					
void	CUnitCtrlBank::SetMCRReadingID(int nMCRPos)
{
	//Cell Info에 MCR ID 삽입. 
}

BOOL	CUnitCtrlBank::CheckCellTransferOnCell(int nChannelNumber)
{
	//Cell Transfer이 셀을 들고 있는지 체크. 
	//TRUE	: Cell O 
	//FALSE	: Cell X 
	
	BOOL bRet = FALSE; 

	return bRet;
}
BOOL	CUnitCtrlBank::CheckCellTransfer_ZAxis_DownPos()
{
	//Cell Trnasfer의 Z-Axis가 하강 위치에 있는지 체크. 
	//실린더. 
	BOOL bRet = FALSE;
	if(GetInPutIOCheck(X_CELL_TR_DOWN) == ON && GetInPutIOCheck(X_CELL_TR_UP) == OFF)
		bRet = TRUE;

	return bRet;
}
BOOL	CUnitCtrlBank::CheckCellTransfer_ZAxis_UpPos()
{
	//Cell Trnasfer의 Z-Axis가 상승 위치에 있는지 체크. 
	BOOL bRet = FALSE;
	if(GetInPutIOCheck(X_CELL_TR_DOWN) == OFF && GetInPutIOCheck(X_CELL_TR_UP) == ON)
		bRet = TRUE;

	return bRet;
}

BOOL	CUnitCtrlBank::CheckCellTransfer_XAxis_BufferStagePos()
{
	//Cell Transfer가 BufferStage side에 있는지 체크.
	BOOL bRet = FALSE;
	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_BUFFER_STG) == TRUE)
		bRet = TRUE;

	return bRet;
}
BOOL	CUnitCtrlBank::CheckCellTransfer_XAxis_MCRReadPos()
{
	//Cell Trnasfer가 MCR Read side에 있는지 체크.
	BOOL bRet = FALSE;
	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_MCR) == TRUE)
		bRet = TRUE;

	return bRet;
}
BOOL	CUnitCtrlBank::CheckCellTransfer_XAxis_MCRReadPos2nd()
{
	//특정 위치에 있는지 검사하는방법 문의 해야함. 
	return FALSE;
}
BOOL	CUnitCtrlBank::CheckCellTransfer_XAxis_MCRReadPos1st()
{
	//특정 위치에 있는지 검사하는방법 문의 해야함. 
	return FALSE;
}

BOOL	CUnitCtrlBank::CheckCellTransfer_PrealignStagePos()
{
	//Cell Transfer가 PrealignStage side에 있는지 체크.
	BOOL bRet = FALSE;
	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_PREALIGN_STG) == TRUE)
		bRet = TRUE;

	return bRet;
}

BOOL	CUnitCtrlBank::CheckCellTransfer_MCR_ID(int nMCRPos)
{
	//Cell Trnasfer의 셀들이 Cell ID를 가지고 있는지 체크. 
	BOOL nRet = FALSE;

	return nRet;
}

void	CUnitCtrlBank::MoveCellTransfer_XAxis_ToBufferStage()
{
	//Cell transfer를 Buffer Stage 측으로 이동. 
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_BUFFER_STG);
}
void	CUnitCtrlBank::MoveCellTransfer_XAxis_ToMCRRead()
{
	//Cell transfer를 MCR Read 측으로 이동. 
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_MCR);
}
void	CUnitCtrlBank::MoveCellTransfer_XAxis_PreAligenStage()
{
	//Cell transfer를 MCR Read 측으로 이동. 
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_PREALIGN_STG);
}
void	CUnitCtrlBank::MoveCellTransfer_XAxis_MCRPitch(int nMCRPos)
{
	//MCCELLTR_MCR_POS3rd = 0, CELLTR_MCR_POS2nd, CELLTR_MCR_POS1st 
	//TR 셀당 간격은 177mm 
	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LD_CELL_TR_X_to_MCR);
	//Cell TR 3Ch 기준 MCR 위치에서 추가 이동. 
}
														
void	CUnitCtrlBank::MoveCellTransfer_ZAxis_Down()
{
	//Cell Transfer의 Z축을 하강한다. 
	SetOutPutIO(Y_CELL_TR_UP, OFF);
	SetOutPutIO(Y_CELL_TR_DOWN, ON);
}
void	CUnitCtrlBank::MoveCellTransfer_ZAxis_Up()
{
	//Cell Transfer의 Z축을 상승한다. 
	SetOutPutIO(Y_CELL_TR_UP, ON);
	SetOutPutIO(Y_CELL_TR_DOWN, OFF);
}

BOOL	CUnitCtrlBank::CheckCellTransfer_Vacuum_OnOff(int nChannelNumber)
{
	//Cell Trnasfer의 Vacuum상태를 체크. 
	BOOL bRet = FALSE;
	if(nChannelNumber == CELLTR_CH1)
	{
		if(GetInPutIOCheck(X_CELL_TR_CH1_VAC) == ON)
			bRet= TRUE;
	}
	else if(nChannelNumber == CELLTR_CH1)
	{
		if(GetInPutIOCheck(X_CELL_TR_CH2_VAC) == ON)
			bRet= TRUE;
	}
	else if(nChannelNumber == CELLTR_CH1)
	{
		if(GetInPutIOCheck(X_CELL_TR_CH3_VAC) == ON)
			bRet= TRUE;
	}

	return bRet;
}
void	CUnitCtrlBank::OnOffCellTransfer_Vacuum(BOOL nOnOFF,int nChannelNumber)
{
	//Cell Transfer의 Vacuum을 On/Off한다. TRUE:On, FALSE:OFF
	if(nOnOFF == CELLTR_VACUUM_OFF)
	{
		if(nChannelNumber == CELLTR_CH1)
			SetOutPutIO(Y_CELL_TR_CH1_CELL_VAC, OFF);
		else if(nChannelNumber == CELLTR_CH2)
			SetOutPutIO(Y_CELL_TR_CH2_CELL_VAC, OFF);
		else if(nChannelNumber == CELLTR_CH3)
			SetOutPutIO(Y_CELL_TR_CH3_CELL_VAC, OFF);
	}
	else 
	{
		if(nChannelNumber == CELLTR_CH1)
			SetOutPutIO(Y_CELL_TR_CH1_CELL_VAC, ON);
		else if(nChannelNumber == CELLTR_CH2)
			SetOutPutIO(Y_CELL_TR_CH2_CELL_VAC, ON);
		else if(nChannelNumber == CELLTR_CH3)
			SetOutPutIO(Y_CELL_TR_CH3_CELL_VAC, ON);
	}
}

void	CUnitCtrlBank::OnOffCellTransfer_BlowVacuum(BOOL nOnOFF,int nChannelNumber)
{
	if(nOnOFF == CELLTR_VACUUM_OFF)
	{
		if(nChannelNumber == CELLTR_CH1)
			SetOutPutIO(Y_CELL_TR_CH1_VAC_RELEASE, OFF);
		else if(nChannelNumber == CELLTR_CH2)
			SetOutPutIO(Y_CELL_TR_CH2_VAC_RELEASE, OFF);
		else if(nChannelNumber == CELLTR_CH3)
			SetOutPutIO(Y_CELL_TR_CH3_VAC_RELEASE, OFF);
	}
	else 
	{
		if(nChannelNumber == CELLTR_CH1)
			SetOutPutIO(Y_CELL_TR_CH1_VAC_RELEASE, ON);
		else if(nChannelNumber == CELLTR_CH2)
			SetOutPutIO(Y_CELL_TR_CH2_VAC_RELEASE, ON);
		else if(nChannelNumber == CELLTR_CH3)
			SetOutPutIO(Y_CELL_TR_CH3_VAC_RELEASE, ON);
	}
}

void	CUnitCtrlBank::SendMCRReadSignal()
{
	//MCR ID를 읽도록 신호를 보낸다. 
}


//미완성 Function list.
//void	CUnitCtrlBank::SetMCRReadingID(int nMCRPos)
//BOOL	CUnitCtrlBank::CheckCellTransfer_XAxis_MCRReadPos2nd()
//BOOL	CUnitCtrlBank::CheckCellTransfer_XAxis_MCRReadPos1st()
//void	CUnitCtrlBank::MoveCellTransfer_XAxis_MCRPitch(int nMCRPos)
//void	CUnitCtrlBank::SendMCRReadSignal()