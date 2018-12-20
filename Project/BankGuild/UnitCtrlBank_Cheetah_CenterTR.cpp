#include "StdAfx.h"
#include "UnitCtrlBank.h"
#include "CheetahApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//void CUnitCtrlBank::CenterTRXGetPos()
//{
//	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_X_GET);
//}
//
//BOOL CUnitCtrlBank::CenterTRXGetPosChk()
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_X_GET) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
//
//void CUnitCtrlBank::CenterTRXPutPos()
//{
//	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_X_PUT);
//}
//
//BOOL CUnitCtrlBank::CenterTRXPutPosChk()
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_X_PUT) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
//
//void CUnitCtrlBank::CenterTRXMCR1Pos(int nPos)
//{
//	theDeviceMotion.TeachMove(m_nThreadID, (nPos == MCR1_1) ? TEACH_PARAM::LDTR_CENNTERTR_X_MCR1_1 : TEACH_PARAM::LDTR_CENNTERTR_X_MCR1_2);
//}
//
//BOOL CUnitCtrlBank::CenterTRXMCR1PosChk(int nPos)
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nPos == MCR1_1) ? TEACH_PARAM::LDTR_CENNTERTR_X_MCR1_1 : TEACH_PARAM::LDTR_CENNTERTR_X_MCR1_2) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
//
//void CUnitCtrlBank::CenterTRXMCR2Pos(int nPos)
//{
//	theDeviceMotion.TeachMove(m_nThreadID, (nPos == MCR2_1) ? TEACH_PARAM::LDTR_CENNTERTR_X_MCR2_1 : TEACH_PARAM::LDTR_CENNTERTR_X_MCR2_2);
//}
//
//BOOL CUnitCtrlBank::CenterTRXMCR2PosChk(int nPos)
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nPos == MCR2_1) ? TEACH_PARAM::LDTR_CENNTERTR_X_MCR2_1 : TEACH_PARAM::LDTR_CENNTERTR_X_MCR2_2) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
//
//void CUnitCtrlBank::CenterTRZGetPos()
//{
//	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_Z_GET);
//}
//
//BOOL CUnitCtrlBank::CenterTRZGetPosChk()
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_Z_GET) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
//
//void CUnitCtrlBank::CenterTRZPutPos()
//{
//	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_Z_PUT);
//}
//
//BOOL CUnitCtrlBank::CenterTRZPutPosChk()
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_Z_PUT) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
//
//void CUnitCtrlBank::CenterTRZUpPos()
//{
//	theDeviceMotion.TeachMove(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_Z_UP);
//}
//
//BOOL CUnitCtrlBank::CenterTRZUpPosChk()
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, TEACH_PARAM::LDTR_CENNTERTR_Z_UP) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
//
//void CUnitCtrlBank::CenterTRZMCR1Pos(int nPos)
//{
//	theDeviceMotion.TeachMove(m_nThreadID, (nPos == MCR1_1) ? TEACH_PARAM::LDTR_CENNTERTR_Z_MCR1_1 : TEACH_PARAM::LDTR_CENNTERTR_Z_MCR1_2);
//}
//
//BOOL CUnitCtrlBank::CenterTRZMCR1PosChk(int nPos)
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nPos == MCR1_1) ? TEACH_PARAM::LDTR_CENNTERTR_Z_MCR1_1 : TEACH_PARAM::LDTR_CENNTERTR_Z_MCR1_2) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
//
//void CUnitCtrlBank::CenterTRZMCR2Pos(int nPos)
//{
//	theDeviceMotion.TeachMove(m_nThreadID, (nPos == MCR2_1) ? TEACH_PARAM::LDTR_CENNTERTR_Z_MCR2_1 : TEACH_PARAM::LDTR_CENNTERTR_Z_MCR2_2);
//}
//
//BOOL CUnitCtrlBank::CenterTRZMCR2PosChk(int nPos)
//{
//	BOOL bRet = FALSE; 
//
//	if(theDeviceMotion.CheckTeachPosition(m_nThreadID, (nPos == MCR2_1) ? TEACH_PARAM::LDTR_CENNTERTR_Z_MCR2_1 : TEACH_PARAM::LDTR_CENNTERTR_Z_MCR2_2) == TRUE)
//		bRet = TRUE;
//
//	return bRet;
//}
