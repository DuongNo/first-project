#pragma once


#include <atlimage.h>			// for CImage

#include "GausGUI.h"

// CGxImage
class _GAUS_GUI CGxImage : public CStatic
{
	DECLARE_DYNAMIC(CGxImage)

public:
	CGxImage();
	virtual ~CGxImage();


private:
	BOOL		m_bStretchMode;							// 객체 크기에 맞도록 이미지를 늘리거나 줄이는 모드인가 ? 기본값=FALSE
	BOOL		m_bLoadDone;
	CImage	m_ImgDisplay;
	CString	m_sFilename;								// 읽어들인 파일 이름

public:

	BOOL	GetImageLoaded();								// 이미지를 Load 하였나 ?
	
	void	SetStretchMode(BOOL bMode);				// 스트레치 모드를 지정한다.
	void	LoadImage(CString strFilename);
	void	ClearImage();									// 이미지를 삭제한다.
	CString GetFilename();								// 불러들인 이미지 파일이름

	// 2015-09-22, jhLee, 추가함수
	void DrawImage(CDC* pDC);							// 이미지를 직접 그려주는 함수
	void SetText(int x, int y, CString sMsg);		// 이미지에 글자를 출력한다.

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


