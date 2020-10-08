#pragma once


// Estimate_Weight dialog

class Estimate_Weight : public CDialog
{
	DECLARE_DYNAMIC(Estimate_Weight)

public:
	Estimate_Weight(CWnd* pParent = NULL);   // standard constructor
	virtual ~Estimate_Weight();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ESTIMATE_WEIGHT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_wp;
	afx_msg void OnEnChangeWeight();
	CString data =_T("hello");
};


