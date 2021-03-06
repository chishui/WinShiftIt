#include "stdafx.h"
#include "MainDlg.h"

enum {
	FIRST_HOTKEY = 0,
	LEFT_HOTKEY, 
	RIGHT_HOTKEY,
	UP_HOTKEY,
	DOWN_HOTKEY,
	TOP_LEFT_HOTKEY,
	TOP_RIGHT_HOTKEY,
	BOTTOM_LEFT_HOTKEY,
	BOTTOM_RIGHT_HOTKEY,
	CENTER_HOTKEY,
	MAXIMIZE_HOTKEY,
	INCREASE_HOTKEY,
	REDUCE_HOTKEY,
	ALL_HOTKEY
};

#define MENU_ID(hotkeyid) (hotkeyid + 1000)
#define DE_MENU_ID(menuid) (menuid - 1000)
#define SETTING_MENU_ID 5000

static HWND GetCurrentWnd(CPoint pt)
{
	HWND hWnd = WindowFromPoint(pt);
	HWND hParent = hWnd;
	while (hWnd)
	{
		hParent = hWnd;
		hWnd = GetParent(hWnd);	
	}
	return hParent;
}
CMainDlg::CMainDlg() 
{

}


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	RegisterAllHotKey();
	//create tray
	CreateTray();
	WM_TASKBARCREATED = RegisterWindowMessage(_T("TaskbarCreated"));
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowWindow(SW_HIDE);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
	UnRegisterAllHotKey();

	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::RegisterAllHotKey()
{
	RegisterHotKey(m_hWnd, LEFT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, VK_LEFT);
	RegisterHotKey(m_hWnd, RIGHT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, VK_RIGHT);
	RegisterHotKey(m_hWnd, UP_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, VK_UP);
	RegisterHotKey(m_hWnd, DOWN_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, VK_DOWN);

	RegisterHotKey(m_hWnd, TOP_LEFT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, '1');
	RegisterHotKey(m_hWnd, TOP_RIGHT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, '2');
	RegisterHotKey(m_hWnd, BOTTOM_LEFT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, '3');
	RegisterHotKey(m_hWnd, BOTTOM_RIGHT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, '4');

	RegisterHotKey(m_hWnd, CENTER_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, 'C');
	RegisterHotKey(m_hWnd, MAXIMIZE_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, 'M');

	RegisterHotKey(m_hWnd, INCREASE_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, VK_OEM_PLUS);
	RegisterHotKey(m_hWnd, REDUCE_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_WIN, VK_OEM_MINUS);
}

LRESULT CMainDlg::OnHotKey( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	int idHotKey = (int) wParam; 
	UINT fuModifiers = (UINT) LOWORD(lParam); 
	UINT uVirtKey = (UINT) HIWORD(lParam);

	CPoint pt;
	GetCursorPos(&pt);
	HWND hWnd = GetCurrentWnd(pt);

	RelayoutWindow(hWnd, wParam);

	return 0;
}

void CMainDlg::UnRegisterAllHotKey()
{
	for (int i = FIRST_HOTKEY + 1; i < ALL_HOTKEY; ++i)
	{
		UnregisterHotKey(m_hWnd, i);
	}
}


LRESULT CMainDlg::OnTrayIcon( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	switch (lParam)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			HMENU hMenu = CreatePopupMenu();
			AppendMenu(hMenu, MF_STRING, MENU_ID(LEFT_HOTKEY), L"Left\t\tCtrl+Win+Alt ��");
			AppendMenu(hMenu, MF_STRING, MENU_ID(RIGHT_HOTKEY), L"Right\t\tCtrl+Win+Alt ��");
			AppendMenu(hMenu, MF_STRING, MENU_ID(UP_HOTKEY), L"Top\t\tCtrl+Win+Alt ��");
			AppendMenu(hMenu, MF_STRING, MENU_ID(DOWN_HOTKEY), L"Bottom\t\tCtrl+Win+Alt ��");
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hMenu, MF_STRING, MENU_ID(TOP_LEFT_HOTKEY), L"Top Left\t\tCtrl+Win+Alt 1");
			AppendMenu(hMenu, MF_STRING, MENU_ID(TOP_RIGHT_HOTKEY), L"Top Right\t\tCtrl+Win+Alt 2");
			AppendMenu(hMenu, MF_STRING, MENU_ID(BOTTOM_LEFT_HOTKEY), L"Bottom Left\t\tCtrl+Win+Alt 3");
			AppendMenu(hMenu, MF_STRING, MENU_ID(BOTTOM_RIGHT_HOTKEY), L"Bottom Right\t\tCtrl+Win+Alt 4");
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hMenu, MF_STRING, MENU_ID(CENTER_HOTKEY), L"Center\t\tCtrl+Win+Alt C");
			AppendMenu(hMenu, MF_STRING, MENU_ID(MAXIMIZE_HOTKEY), L"Maximize\t\tCtrl+Win+Alt M");
			AppendMenu(hMenu, MF_STRING, MENU_ID(INCREASE_HOTKEY), L"Increase\t\tCtrl+Win+Alt =");
			AppendMenu(hMenu, MF_STRING, MENU_ID(REDUCE_HOTKEY), L"Reduce\t\tCtrl+Win+Alt -");
			AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
			AppendMenu(hMenu, MF_STRING, SETTING_MENU_ID, L"Settings");
			AppendMenu(hMenu, MF_STRING, 1, L"Exit");
			POINT pt;
			GetCursorPos(&pt);
			SetForegroundWindow(m_hWnd);
			TrackPopupMenu(hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
			DestroyMenu(hMenu);

		}
		break;
	}

	return 0;
}

LRESULT CMainDlg::OnCommand( UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled )
{
	switch (LOWORD(wParam))
	{
	case MENU_ID(LEFT_HOTKEY):
	case MENU_ID(RIGHT_HOTKEY):
	case MENU_ID(UP_HOTKEY):
	case MENU_ID(DOWN_HOTKEY):
	case MENU_ID(TOP_LEFT_HOTKEY):
	case MENU_ID(TOP_RIGHT_HOTKEY):
	case MENU_ID(BOTTOM_LEFT_HOTKEY):
	case MENU_ID(BOTTOM_RIGHT_HOTKEY):
	case MENU_ID(CENTER_HOTKEY):
	case MENU_ID(MAXIMIZE_HOTKEY):
	case MENU_ID(INCREASE_HOTKEY):
	case MENU_ID(REDUCE_HOTKEY):
		RelayoutWindow(NULL, DE_MENU_ID(LOWORD(wParam)));
		break;
	case SETTING_MENU_ID:
		{	
			ShowWindow(SW_SHOW);
			CenterWindow();
		}	
		break;
	default:
		bHandled = FALSE;
		break;
	}
	return 0;
}

void CMainDlg::RelayoutWindow( HWND hWnd, DWORD dwKey )
{
	static const int SIZE_CHANGED = 30;
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rc,0);
	CRect rcWorkArea(rc);
	CRect rcClient;
	::GetClientRect(hWnd, rcClient);

	BOOL bIsMaximized = ::IsZoomed(hWnd);
	if (dwKey == MAXIMIZE_HOTKEY && bIsMaximized)
	{
		::PostMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		return;
	}
	if (bIsMaximized)
	{	
		::SendMessageTimeout(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0, SMTO_ABORTIFHUNG, 100, NULL);
	}

	switch (dwKey)
	{
	case LEFT_HOTKEY:
		{
			::MoveWindow(hWnd, 0, 0, rcWorkArea.Width() / 2, rcWorkArea.Height(), TRUE);
		}
		break;
	case RIGHT_HOTKEY:
		{
			::MoveWindow(hWnd, rcWorkArea.Width() / 2, 0, rcWorkArea.Width() / 2, rcWorkArea.Height(), TRUE);		
		}
		break;
	case UP_HOTKEY:
		{
			::MoveWindow(hWnd, 0, 0, rcWorkArea.Width(), rcWorkArea.Height()/ 2, TRUE);			
		}
		break;
	case DOWN_HOTKEY:
		{
			::MoveWindow(hWnd, 0, rcWorkArea.Height() / 2, rcWorkArea.Width(), rcWorkArea.Height() / 2, TRUE);	
		}
		break;
	case TOP_LEFT_HOTKEY:
		{
			::MoveWindow(hWnd, 0, 0, rcWorkArea.Width() / 2, rcWorkArea.Height() / 2, TRUE);
		}
		break;
	case TOP_RIGHT_HOTKEY:
		{
			::MoveWindow(hWnd, rcWorkArea.Width() / 2, 0, rcWorkArea.Width() / 2, rcWorkArea.Height() /2, TRUE);		
		}
		break;
	case BOTTOM_LEFT_HOTKEY:
		{
			::MoveWindow(hWnd, 0, rcWorkArea.Height()/ 2, rcWorkArea.Width() / 2, rcWorkArea.Height() / 2, TRUE);		
		}
		break;
	case BOTTOM_RIGHT_HOTKEY:
		{
			::MoveWindow(hWnd, rcWorkArea.Width() / 2, rcWorkArea.Height() / 2, rcWorkArea.Width() / 2, rcWorkArea.Height() / 2, TRUE);		
		}
		break;
	case CENTER_HOTKEY:
		{
			::SetWindowPos(hWnd, NULL, rcWorkArea.left + (rcWorkArea.Width() - rcClient.Width()) / 2, rcWorkArea.top + (rcWorkArea.Height() - rcClient.Height()) / 2, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE);
		}
		break;
	case MAXIMIZE_HOTKEY:
		{
			::PostMessage(hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, NULL);
		}
		break;
	case INCREASE_HOTKEY:
		{
			CRect rcWindow;
			::GetWindowRect(hWnd, rcWindow);
			rcWindow.InflateRect(SIZE_CHANGED, SIZE_CHANGED, SIZE_CHANGED, SIZE_CHANGED);
			int nWidth = rcWindow.Width();
			int nHeight = rcWindow.Height();
			if (rcWindow.left < rcWorkArea.left || rcWindow.right > rcWorkArea.right)
			{
				rcWindow.left = rcWorkArea.left;
				rcWindow.right = rcWindow.left + nWidth > rcWorkArea.right ? rcWorkArea.right : rcWindow.left + nWidth;
			}
			if (rcWindow.top < rcWorkArea.top || rcWindow.bottom > rcWorkArea.bottom)
			{
				rcWindow.top = rcWorkArea.top;
				rcWindow.bottom = rcWindow.top + nHeight > rcWorkArea.bottom ? rcWorkArea.bottom : rcWindow.top + nHeight;
			}
			::MoveWindow(hWnd, rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), TRUE);		

		}
		break;
	case REDUCE_HOTKEY:
		{
			CRect rcWindow;
			::GetWindowRect(hWnd, rcWindow);
			rcWindow.InflateRect(-SIZE_CHANGED, -SIZE_CHANGED, -SIZE_CHANGED, -SIZE_CHANGED);
			::MoveWindow(hWnd, rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), TRUE);	
		}
		break;
	default:
		break;
	}
}


LRESULT CMainDlg::OnTaskBarCreated( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/ )
{
	CreateTray();
	return 0;
}

void CMainDlg::CreateTray()
{
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	::ZeroMemory(&m_nid, sizeof(NOTIFYICONDATA));
	m_nid.cbSize =  sizeof(NOTIFYICONDATA);
	m_nid.hWnd = m_hWnd;
	m_nid.hIcon = hIconSmall;
	m_nid.uFlags = NIF_ICON | NIF_TIP |NIF_MESSAGE;
	_tcsncpy(m_nid.szTip, CString(MAKEINTRESOURCE(IDR_MAINFRAME)), 128);
	m_nid.uCallbackMessage = WM_TRAYICON;

	Shell_NotifyIcon(NIM_ADD,&m_nid);
}

static BOOL SetAutoRun(BOOL bAutoRun)
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);

	HKEY hKey;
	LSTATUS lRet = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &hKey);
	if(lRet != ERROR_SUCCESS)
		return FALSE;

	if (bAutoRun)
	{
		CString strVal = szPath;
		lRet = RegSetValueEx(hKey, _T("WinShiftIt"), 0, REG_SZ, (const BYTE *)strVal.GetBuffer(0), sizeof(TCHAR)*strVal.GetLength());
		strVal.ReleaseBuffer();

		SHDeleteValue( HKEY_LOCAL_MACHINE , _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), _T("WinShiftIt") );
	} 
	else
	{
		lRet = RegDeleteValue(hKey, _T("WinShiftIt"));
	}
	RegCloseKey(hKey);
	if(lRet != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
LRESULT CMainDlg::OnBnClickedCheckAutorun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CButton check;
	check.Attach(GetDlgItem(IDC_CHECK_AUTORUN));
	SetAutoRun(check.GetCheck());
	return 0;
}
