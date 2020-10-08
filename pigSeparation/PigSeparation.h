
// PigSeparation.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPigSeparationApp:
// See PigSeparation.cpp for the implementation of this class
//

class CPigSeparationApp : public CWinApp
{
public:
	CPigSeparationApp();

// Overrides
public:
	virtual BOOL InitInstance();
	CString estimate;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPigSeparationApp theApp;