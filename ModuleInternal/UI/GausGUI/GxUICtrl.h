//
// CGxUICtrl : GxOCX類의 ActiveX를 제어하기위한 여러 유틸리티 함수 모음 class
//
// 2015-09-22, jhLee
//
//

#pragma once

#include "GausGUI.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// 각종 Color 정의 
#define GXCOLOR_ON							RGB(0, 255, 0)					// 선택된 버튼 색, 밝은 Lime
#define GXCOLOR_OFF							RGB(224, 224, 224)			// 선택되지 않은 버튼 색, 은색

#define GXCOLOR_SILVER						RGB(224, 224, 224)			// 선택되지 않은 버튼 색, 은색
#define GXCOLOR_DARKGRAY					RGB(128, 128, 128)			// 어두운 재색
#define GXCOLOR_LIME							RGB(191, 255, 0)				// 선택된 버튼 색, Lime
#define GXCOLOR_BLACK						RGB(0, 0, 0)					// 글자색, 검은색
#define GXCOLOR_WHITE						RGB(255, 255, 255)			// 흰색
#define GXCOLOR_BLUE							RGB(0, 0, 255)					// 글자색 파란색
#define GXCOLOR_RED							RGB(255, 0, 0)					// 글자색 파란색

#define GXCOLOR_SELECT						RGB(192, 255, 255)			// 선택 192, 255, 192)
#define GXCOLOR_DESELECT					RGB(224, 224, 224)			// 선택 안됨 192, 255, 192)

#define GXCOLOR_INIT_READY					RGB(224, 224, 224)			// 초기화 진행 전
#define GXCOLOR_INIT_RUN					RGB(192, 192, 255)			// 초기화 진행 중
#define GXCOLOR_INIT_OK						RGB(192, 255, 192)			// 초기화 진행 성공
#define GXCOLOR_INIT_FAIL					RGB(255, 192, 192)			// 초기화 진행 실패

#define GXCOLOR_MOTOR_ON					GXCOLOR_SPRING_GREEN		//	모터축 ON
#define GXCOLOR_VACUUM_ON					RGB(255, 10, 10)			//	VACUUM ON

#define	GXCOLOR_LIGHT_PINK				RGB(255,	182,	193)
#define	GXCOLOR_PINK						RGB(255,	192,	203)
#define	GXCOLOR_CRIMSON					RGB(220,	20,		60)
#define	GXCOLOR_LAVENDER_BLUSH			RGB(255,	240,	245)
#define	GXCOLOR_PALE_VILOT_RED			RGB(219,	112,	147)
#define	GXCOLOR_HOT_PINK					RGB(255,	105,	180)
#define	GXCOLOR_DEEP_PINK					RGB(255,	20,		147)
#define	GXCOLOR_MEDIUM_VILOT_RED		RGB(199,	21,		133)
#define	GXCOLOR_ORCHID						RGB(218,	112,	214)
#define	GXCOLOR_THISTLE					RGB(216,	191,	216)
#define	GXCOLOR_PLUM						RGB(221,	160,	221)
#define	GXCOLOR_VIIOLET					RGB(238,	130,	238)
#define	GXCOLOR_MAGENTA					RGB(255,	0,		255)
#define	GXCOLOR_FUCHSIA					RGB(255,	0,		255)
#define	GXCOLOR_PURPLE						RGB(128,	0,		128)
#define	GXCOLOR_MEDIUM_ORCHID			RGB(186,	85,		211)
#define	GXCOLOR_DARK_VIOLET				RGB(148,	0,		211)
#define	GXCOLOR_DARK_ORCHID				RGB(153,	50,		204)
#define	GXCOLOR_INDIGO						RGB(75,		0,		130)
#define	GXCOLOR_BLUE_VIOLET				RGB(138,	43,		226)
#define	GXCOLOR_MEDIUM_PURPLE			RGB(147,	112,	219)
#define	GXCOLOR_MEDIUM_SLATE_BLUE		RGB(123,	104,	238)
#define	GXCOLOR_SLATE_BLUE				RGB(106,	90,		205)
#define	GXCOLOR_DARK_SLATE_BLUE			RGB(72,		61,		139)
#define	GXCOLOR_LAVENDER					RGB(230,	230,	250)
#define	GXCOLOR_GHOST_WHITE				RGB(248,	248,	255)
#define	GXCOLOR_MEDIUM_BLUE				RGB(0,		0,		205)
#define	GXCOLOR_MIDNIGHT_BLUE			RGB(25,		25,		112)
#define	GXCOLOR_NAVY						RGB(0,		0,		128)
#define	GXCOLOR_ROYAL_BLUE				RGB(62,		105,	225)
#define	GXCOLOR_CORNFLOWER_BLUE			RGB(100,	149,	237)
#define	GXCOLOR_LIGHT_STEEL_BLUE		RGB(176,	196,	222)
#define	GXCOLOR_LIGHT_SLATE_GRAY		RGB(119,	136,	153)
#define	GXCOLOR_SLATE_GRAY				RGB(112,	128,	144)
#define	GXCOLOR_DODGER_BLUE				RGB(30,		144,	255)
#define	GXCOLOR_ALICE_BLUE				RGB(240,	248,	255)
#define	GXCOLOR_STEEL_BLUE				RGB(70,		130,	180)
#define	GXCOLOR_LIGHT_SKY_BLUE			RGB(135,	206,	250)
#define	GXCOLOR_SKY_BLUE					RGB(135,	206,	235)
#define	GXCOLOR_DEEP_SKY_BLUE			RGB(0,		191,	255)
#define	GXCOLOR_POWEDER_BLUE				RGB(176,	224,	230)
#define	GXCOLOR_CADET_BLUE				RGB(176,	158,	160)
#define	GXCOLOR_AZURE						RGB(240,	255,	255)
#define	GXCOLOR_LIGHT_CYAN				RGB(224,	255,	255)
#define	GXCOLOR_PALE_TURQUOOISE			RGB(175,	238,	238)
#define	GXCOLOR_CYAN						RGB(0,		255,	255)
#define	GXCOLOR_AQUA						RGB(0,		255,	255)
#define	GXCOLOR_DARK_TURQUOISE			RGB(0,		206,	209)
#define	GXCOLOR_DARK_SLATE_GRAY			RGB(47,		79,		79)
#define	GXCOLOR_TEAL						RGB(0,		128,	128)
#define	GXCOLOR_MEDIUM_TURQUOISE		RGB(72,		2209,	204)
#define	GXCOLOR_LIGHT_SEA_GREEN			RGB(32,		178,	170)
#define	GXCOLOR_TURQUOISE					RGB(64,		224,	208)
#define	GXCOLOR_AQUAMARINE				RGB(127,	255,	212)
#define	GXCOLOR_MEDIUM_AQUAMARINE		RGB(102,	205,	170)
#define	GXCOLOR_MEDIUM_SPRING_GREEN	RGB(0,		250,	154)
#define	GXCOLOR_MINT_CREAM				RGB(245,	255,	250)
#define	GXCOLOR_SPRING_GREEN				RGB(0,		255,	127)
#define	GXCOLOR_MEDIUM_SEA_GREEN		RGB(60,		179,	113)
#define	GXCOLOR_SEA_GREEN					RGB(46,		139,	87)
#define	GXCOLOR_HONEYDEW					RGB(240,	255,	240)
#define	GXCOLOR_PALE_GREEN				RGB(152,	251,	152)
#define	GXCOLOR_DARK_SEA_GREEN			RGB(143,	188,	143)
#define	GXCOLOR_LIME_GREEN				RGB(50,		205,	50)
#define	GXCOLOR_FOREST_GREEN				RGB(34,		139,	34)
#define	GXCOLOR_CHARTREUSE				RGB(127,	255,	0)
#define	GXCOLOR_LAWN_GREEN				RGB(124,	252,	0)
#define	GXCOLOR_GREEN_YELLOW				RGB(173,	255,	47)
#define	GXCOLOR_DARK_OLIVE_GREEN		RGB(85,		107,	47)
#define	GXCOLOR_YELLOW_GREEN				RGB(154,	205,	50)
#define	GXCOLOR_OLIVE_DRAB				RGB(107,	142,	35)
#define	GXCOLOR_BEIGE						RGB(245,	245,	220)
#define	GXCOLOR_LIGHT_GOLDENROD_YELLOW	RGB(250,	250,	210)
#define	GXCOLOR_IVORY						RGB(255,	255,	240)
#define	GXCOLOR_LIGHT_YELLOW				RGB(255,	255,	224)
#define	GXCOLOR_YELLOW						RGB(255,	255,	0)
#define	GXCOLOR_OLIVE						RGB(128,	128,	0)
#define	GXCOLOR_DARK_KHAKI				RGB(128,	128,	0)
#define	GXCOLOR_LEMON_CHIFFON			RGB(255,	250,	205)
#define	GXCOLOR_PALE_GOLDENROD			RGB(238,	232,	170)
#define	GXCOLOR_KHAKI						RGB(240,	230,	170)
#define	GXCOLOR_GOLD						RGB(255,	215,	0)
#define	GXCOLOR_CORNSILK					RGB(255,	248,	220)
#define	GXCOLOR_GOLDENROD					RGB(218,	165,	32)
#define	GXCOLOR_DARK_GOLDENROD			RGB(184,	134,	11)
#define	GXCOLOR_FLORAL_WHITE				RGB(255,	250,	240)
#define	GXCOLOR_OLD_LACE					RGB(253,	245,	230)
#define	GXCOLOR_WHEAT						RGB(245,	222,	179)
#define	GXCOLOR_MOCCASIN					RGB(255,	228,	181)
#define	GXCOLOR_ORANGE						RGB(255,	165,	0)
#define	GXCOLOR_PAPAYA_WHIP				RGB(255,	239,	213)
#define	GXCOLOR_BLANCHED_ALMOND			RGB(255,	235,	205)
#define	GXCOLOR_NAVAJO_WHITE				RGB(255,	222,	173)
#define	GXCOLOR_ANTIQUE_WHITE			RGB(250,	235,	215)
#define	GXCOLOR_TAN							RGB(210,	180,	140)
#define	GXCOLOR_BURLY_WOOD				RGB(222,	180,	135)
#define	GXCOLOR_BISQUE						RGB(255,	228,	196)
#define	GXCOLOR_DARK_ORANGE				RGB(255,	140,	230)
#define	GXCOLOR_LINEN						RGB(250,	240,	230)
#define	GXCOLOR_PERU						RGB(205,	133,	63)
#define	GXCOLOR_PEACH_PUFF				RGB(255,	218,	185)
#define	GXCOLOR_SANDY_BROWN				RGB(244,	164,	96)
#define	GXCOLOR_CHOCOLATE					RGB(210,	105,	30)
#define	GXCOLOR_SADDLE_BROWN				RGB(244,	164,	96)
#define	GXCOLOR_SEASHELL					RGB(255,	245,	238)
#define	GXCOLOR_SIENNA						RGB(160,	82,		45)
#define	GXCOLOR_LIGHT_SALMON				RGB(255,	160,	122)
#define	GXCOLOR_CORAL						RGB(255,	127,	80)
#define	GXCOLOR_ORANGE_RED				RGB(255,	69,		0)
#define	GXCOLOR_DARK_SALMON				RGB(233,	150,	122)
#define	GXCOLOR_TOMATO						RGB(255,	99,		71)
#define	GXCOLOR_MISTY_ROSE				RGB(255,	228,	225)
#define	GXCOLOR_SALMON						RGB(250,	128,	114)
#define	GXCOLOR_SNOW						RGB(255,	250,	250)
#define	GXCOLOR_LIGHT_CORAL				RGB(240,	128,	128)
#define	GXCOLOR_ROSY_BROWN				RGB(188,	143,	143)
#define	GXCOLOR_INDIAN_RED				RGB(205,	92,		92)
#define	GXCOLOR_BROWN						RGB(165,	42,		42)
#define	GXCOLOR_FIRE_BRICK				RGB(178,	34,		34)
#define	GXCOLOR_MAROON						RGB(128,	0,		0)
#define	GXCOLOR_WHITE						RGB(255,	255,	255)
#define	GXCOLOR_WHITE_SMOKE				RGB(245,	245,	245)
#define	GXCOLOR_GAINSBORO					RGB(220,	220,	220)
#define	GXCOLOR_DARKBLUE					RGB(000,	000,	128)
#define	GXCOLOR_DARK_GREEN				RGB(000,	064,	000)
#define	GXCOLOR_DARK_CYAN					RGB(000,	064,	064)	
#define	GXCOLOR_DARK_RED					RGB(064,	000,	000)
#define	GXCOLOR_DARK_MAGENTA				RGB(128,	000,	128)
#define	GXCOLOR_DARK_GRAY					RGB(128,	128,	128)
#define	GXCOLOR_LIGHT_GRAY				RGB(192,	192,	192)
#define	GXCOLOR_LIGHT_BLUE				RGB(000,	000,	255)
#define	GXCOLOR_LIGHT_GREEN				RGB(000,	255,	000)
#define	GXCOLOR_LIGHT_RED					RGB(255,	000,	000)
#define	GXCOLOR_LIGHT_MAGENTA			RGB(255,	000,	255)
#define	GXCOLOR_SCRIPT						RGB(176,	6,		002)
#define	GXCOLOR_GREEN						RGB(020,	160,	000)
#define	GXCOLOR_GRAY						RGB(190,	190,	190)




class _GAUS_GUI CGxUICtrl
{
public:
	CGxUICtrl();
	~CGxUICtrl();

public:

	static BOOL AbortModalDialog();			// GausGUI 에서 생성한 Modal Dialog를 강제로 종료시켜준다. Dialog 응답은 IDABORT로 끝내며, 더이상 Dialog를 찾을 없으면 FALSE를 리턴한다.
	static void BroadcastButtonCpationIndex(CWnd* pView, UINT nIdx, UINT nID);		// 지정한 폼의 모든 GxOCX계열의 Control에게 표시 표시 언어가 변경되었음을 통보한다.
	static void BroadcastStaticCpationIndex(CWnd* pView, UINT nIdx, UINT nID);		// 지정한 폼의 모든 GxOCX계열의 Control에게 표시 표시 언어가 변경되었음을 통보한다.


	// CGxButtonEx 관련
	static void SetButtonColor(CWnd* pView, UINT nID, COLORREF cColor);			// 지정 버튼을 주어진 논리에 따라 색을 지정한다.
	static void SetButtonTextColor(CWnd* pView, UINT nID, COLORREF cColor);		// 지정 버튼을 주어진 논리에 따라 글자 색을 지정한다.
	static void SetButtonIcon(CWnd* pView, UINT nID, UINT nIdx);					// 버튼의 아이콘을 변경하여준다.
	static void SetButtonString(CWnd* pView, UINT nID, CString sMsg);
	static void SetButtonInt(CWnd* pView, UINT nID, int nValue);
	static void SetButtonDouble(CWnd* pView, UINT nID, double dbValue, UINT nPoint);
	static void SetButtonEnabled(CWnd* pView, UINT nID, BOOL bFlag);
	static void SetButtonVisible(CWnd* pView, UINT nID, BOOL bFlag);
	static void SetButtonBlink(CWnd* pView, UINT nID, BOOL bFlag);

	// CGxStaticEx 관련
	static void SetStaticColor(CWnd* pView, UINT nID, COLORREF cColor);			// 라벨의 배경 색을 지정한다.
	static void SetStaticTextColor(CWnd* pView, UINT nID, COLORREF cColor);		// 라벨의 글자색을 지정한다.
	static void SetStaticIcon(CWnd* pView, UINT nID, UINT nIdx);					// 라벨의 아이콘을 변경한다.
	static void SetStaticString(CWnd* pView, UINT nID, CString sMsg);
	static void SetStaticInt(CWnd* pView, UINT nID, int nValue);
	static void SetStaticDouble(CWnd* pView, UINT nID, double dbValue, UINT nPoint);
	static void SetStaticVisible(CWnd* pView, UINT nID, BOOL bFlag);
	static void SetStaticBlink(CWnd* pView, UINT nID, BOOL bFlag);

};

