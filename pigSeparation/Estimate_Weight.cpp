// Estimate_Weight.cpp : implementation file
//

#include "stdafx.h"
#include "PigSeparation.h"
#include "Estimate_Weight.h"
#include "afxdialogex.h"
#include <iostream>

// Estimate_Weight dialog

IMPLEMENT_DYNAMIC(Estimate_Weight, CDialog)

Estimate_Weight::Estimate_Weight(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ESTIMATE_WEIGHT, pParent)
	, m_wp(_T(""))
{
	m_wp = data;
}

Estimate_Weight::~Estimate_Weight()
{

}

void Estimate_Weight::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Weight, m_wp);
}


BEGIN_MESSAGE_MAP(Estimate_Weight, CDialog)
	ON_EN_CHANGE(IDC_Weight, &Estimate_Weight::OnEnChangeWeight)
END_MESSAGE_MAP()


// Estimate_Weight message handlers







void Estimate_Weight::OnEnChangeWeight()
{
	
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

}
