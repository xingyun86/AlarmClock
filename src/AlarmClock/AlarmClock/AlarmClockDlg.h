
// AlarmClockDlg.h : header file
//

#pragma once

#define AC_TIMERID			1
#define AC_TIMESEC			750
#define AC_APP				"AlarmClock"
#define AC_DATETIME_KEY		"DATETIME"
#define AC_DATETIME_DEF_VAL	"1970-01-01+08:00:00"
#define AC_WAV_KEY			"WAVKEY"
#define AC_WAV_DEF_VAL		"0"
#define WM_USER_NOTIFYICON	WM_USER + WM_NOTIFY

// CAlarmClockDlg dialog
class CAlarmClockDlg : public CDialogEx
{
// Construction
public:
	CAlarmClockDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ALARMCLOCK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CTime m_cTime;
	INT m_nWavIdx;
	BOOL m_bPlay;
	NOTIFYICONDATA m_tNid;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonAlarmsound();
	afx_msg LRESULT OnUserNotify(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnRestartExplorer(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()
private:
	void CreateNotifyIcon() {
		m_tNid.cbSize = sizeof(NOTIFYICONDATA);
		m_tNid.hWnd = this->m_hWnd;
		m_tNid.uID = IDR_MAINFRAME;
		m_tNid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
		lstrcpy(m_tNid.szTip, TEXT("ÄÖÖÓÌáÐÑ"));
		m_tNid.uCallbackMessage = WM_USER_NOTIFYICON;
		m_tNid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		Shell_NotifyIcon(NIM_ADD, &m_tNid);
	}
	void DeleteNotifyIcon() {
		Shell_NotifyIcon(NIM_DELETE, &m_tNid);
	}
public:
	BOOL Play() {
		return m_bPlay;
	}
	void Play(BOOL bPlay) {
		m_bPlay = bPlay;
	}
	void WavIdx(INT nWavIdx) {
		CString sWavIdx(TEXT(""));
		this->m_nWavIdx = nWavIdx;
		sWavIdx.Format(TEXT("%d"), this->m_nWavIdx);
		theApp.WriteProfileString(TEXT(AC_APP), TEXT(AC_WAV_KEY), sWavIdx);
	}
};
