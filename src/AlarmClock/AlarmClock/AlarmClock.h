
// AlarmClock.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CAlarmClockApp:
// See AlarmClock.cpp for the implementation of this class
//

class CAlarmClockApp : public CWinApp
{
public:
	CAlarmClockApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

private:
	HANDLE m_hMutex = NULL;
};

extern CAlarmClockApp theApp;
