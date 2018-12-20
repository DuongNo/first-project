
// CheetahView.h : CCheetahView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"


class CCheetahView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CCheetahView();
	DECLARE_DYNCREATE(CCheetahView)

public:
	enum{ IDD = IDD_FORM_BASE };

// Ư���Դϴ�.
public:
	CCheetahDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CCheetahView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CheetahView.cpp�� ����� ����
inline CCheetahDoc* CCheetahView::GetDocument() const
   { return reinterpret_cast<CCheetahDoc*>(m_pDocument); }
#endif

