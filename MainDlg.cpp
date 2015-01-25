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

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

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
	UnRegisterAllHotKey();
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::RegisterAllHotKey()
{
	RegisterHotKey(m_hWnd, LEFT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, VK_LEFT);
	RegisterHotKey(m_hWnd, RIGHT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, VK_RIGHT);
	RegisterHotKey(m_hWnd, UP_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, VK_UP);
	RegisterHotKey(m_hWnd, DOWN_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, VK_DOWN);

	RegisterHotKey(m_hWnd, TOP_LEFT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, '1');
	RegisterHotKey(m_hWnd, TOP_RIGHT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, '2');
	RegisterHotKey(m_hWnd, BOTTOM_LEFT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, '3');
	RegisterHotKey(m_hWnd, BOTTOM_RIGHT_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, '4');

	RegisterHotKey(m_hWnd, CENTER_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, 'C');
	RegisterHotKey(m_hWnd, MAXIMIZE_HOTKEY, MOD_ALT | MOD_CONTROL | MOD_SHIFT, 'M');
}

LRESULT CMainDlg::OnHotKey( UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/ )
{
	int idHotKey = (int) wParam; 
	UINT fuModifiers = (UINT) LOWORD(lParam); 
	UINT uVirtKey = (UINT) HIWORD(lParam);

	CPoint pt;
	GetCursorPos(&pt);
	HWND hWnd = GetCurrentWnd(pt);
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rc,0);
	CRect rcWorkArea(rc);
	CRect rcClient;
	::GetClientRect(hWnd, rcClient);

	switch (idHotKey)
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
	default:
		break;
	}

	return 0;
}

void CMainDlg::UnRegisterAllHotKey()
{
	for (int i = FIRST_HOTKEY + 1; i < ALL_HOTKEY; ++i)
	{
		UnregisterHotKey(m_hWnd, i);
	}
}
