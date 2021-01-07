
// AlarmClockDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "AlarmClock.h"
#include "AlarmClockDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAlarmClockDlg dialog



CAlarmClockDlg::CAlarmClockDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ALARMCLOCK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAlarmClockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

const UINT WMEX_TASKBARCREATED = ::RegisterWindowMessage(TEXT("TaskbarCreated"));

BEGIN_MESSAGE_MAP(CAlarmClockDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CAlarmClockDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ALARMSOUND, &CAlarmClockDlg::OnBnClickedButtonAlarmsound)
	ON_MESSAGE(WM_USER_NOTIFYICON, OnUserNotify)
	ON_BN_CLICKED(IDCANCEL, &CAlarmClockDlg::OnBnClickedCancel)
	ON_REGISTERED_MESSAGE(WMEX_TASKBARCREATED, OnRestartExplorer)
END_MESSAGE_MAP()


// CAlarmClockDlg message handlers

BOOL CAlarmClockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CreateNotifyIcon();

	SetWindowText(TEXT("ÄÖÖÓÌáÐÑ"));
	SetDlgItemText(IDOK, TEXT("Éè¶¨ÄÖÖÓ"));
	SetDlgItemText(IDCANCEL, TEXT("¹Ø±Õ´°¿Ú"));

	INT nYear = 0, nMonth = 0, nDate = 0, nHour = 0, nMin = 0, nSec = 0;
	CString sOldDateTime = theApp.GetProfileString(TEXT(AC_APP), TEXT(AC_DATETIME_KEY), TEXT(AC_DATETIME_DEF_VAL));
	CString sOldWavIdx = theApp.GetProfileString(TEXT(AC_APP), TEXT(AC_WAV_KEY), TEXT(AC_WAV_DEF_VAL));
	_stscanf(sOldDateTime, TEXT("%d-%d-%d+%d:%d:%d"), &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);
	m_cTime = CTime(nYear, nMonth, nDate, nHour, nMin, nSec);
	m_nWavIdx = _ttoi(sOldWavIdx);
	m_nWavIdx = (m_nWavIdx >= IDR_WAVE1 && m_nWavIdx <= IDR_WAVE10) ? m_nWavIdx : IDR_WAVE1;
	CDateTimeCtrl* pDateTime = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_DATETIME);
	if (pDateTime != NULL)
	{
		pDateTime->SetFormat(TEXT("yyyy-MM-dd+HH:mm:ss"));
		pDateTime->SetTime(&m_cTime);
	}

	SetTimer(AC_TIMERID, AC_TIMESEC, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAlarmClockDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
		switch (nID)
		{
		case SC_CLOSE:
		{
			DeleteNotifyIcon();
		}
		break;
		case SC_MINIMIZE:
		{
			ShowWindow(SW_HIDE);
		}
		break;
		default:
			break;
		}
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAlarmClockDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAlarmClockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAlarmClockDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	CDateTimeCtrl* pDateTime = (CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_DATETIME);
	if (pDateTime != NULL)
	{
		pDateTime->GetTime(m_cTime);
		CString sNewDateTime = m_cTime.Format("%Y-%m-%d+%H:%M:%S");
		theApp.WriteProfileString(TEXT(AC_APP), TEXT(AC_DATETIME_KEY), sNewDateTime);
		KillTimer(AC_TIMERID);
		Play(FALSE);
		SetTimer(AC_TIMERID, AC_TIMESEC, NULL);
	}
}


void CAlarmClockDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnTimer(nIDEvent);

	switch (nIDEvent)
	{
	case AC_TIMERID:
	{
		if ((time(NULL) - m_cTime.GetTime()) >= 0LL)
		{
			KillTimer(AC_TIMERID);
			CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)[](void *p)->DWORD
				{
					INT nCounter = 100;
					CAlarmClockDlg* thiz = (CAlarmClockDlg*)p;
					if (thiz != NULL)
					{
						PlaySound(MAKEINTRESOURCE(thiz->m_nWavIdx), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP | SND_ASYNC);
						thiz->Play(TRUE);
						while (nCounter-- && thiz->Play())
						{
							Sleep(100);
						}
						PlaySound(NULL, NULL, NULL);
						thiz->Play(FALSE);
					}
					return 0;
				}
			, this, 0, NULL));
		}
	}
	break;
	default:
		break;
	}
}

void CAlarmClockDlg::OnBnClickedButtonAlarmsound()
{
	// TODO: Add your control notification handler code here
	INT nVal = 0;
	CMenu menu = {};
	POINT pt = { 0 };
	::GetCursorPos(&pt);
	menu.CreatePopupMenu();
	for (INT i = IDR_WAVE1; i <= IDR_WAVE10; i++)
	{
		CString text(TEXT(""));
		text.Format(TEXT("ÄÖÁå%d"), i - IDR_WAVE1 + 1);
		menu.AppendMenu(MF_STRING, i, text);
	}
	nVal = menu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN, pt.x, pt.y, this);
	if(nVal >= IDR_WAVE1 && nVal <= IDR_WAVE10)
	{
		this->WavIdx(nVal);
	}
	menu.Detach();
	menu.DestroyMenu();
}

LRESULT  CAlarmClockDlg::OnUserNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam != IDR_MAINFRAME)
	{
		return TRUE;
	}
	switch (lParam)
	{
	case WM_RBUTTONUP:
	{
		INT nVal = 0;
		CMenu menu = {};
		POINT pt = { 0 };
		::GetCursorPos(&pt);
		menu.CreatePopupMenu();   
		menu.AppendMenu(MF_STRING, WM_DESTROY, TEXT("¹Ø±Õ"));
		nVal = menu.TrackPopupMenu(TPM_RETURNCMD | TPM_LEFTALIGN, pt.x, pt.y, this);
		switch (nVal)
		{
		case IDCANCEL:
		{
			OnCancel();
		}
		break;
		default:
			break;
		}
		menu.Detach();
		menu.DestroyMenu();
	}
	break;
	case WM_LBUTTONDBLCLK:
	{
		ShowWindow(SW_RESTORE);
		SetForegroundWindow();
		SetFocus();
	}
	break;
	}
	return 0;
}

LRESULT CAlarmClockDlg::OnRestartExplorer(WPARAM wParam, LPARAM lParam)
{
	CreateNotifyIcon();
	return TRUE;
}

void CAlarmClockDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	DeleteNotifyIcon();
	CDialogEx::OnCancel();
}