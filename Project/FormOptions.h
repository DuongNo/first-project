#pragma once



// CFormOptions �� ���Դϴ�.

class CFormOptions : public CFormView
{
	DECLARE_DYNCREATE(CFormOptions)

public:
	CFormOptions();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CFormOptions();

public:
	enum { IDD = IDD_FORM_OPTION };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};


