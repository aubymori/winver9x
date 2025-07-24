#include <windows.h>
#include "resource.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

enum WINVER9XMODE
{
	WV9XM_WIN95 = 0,
	WV9XM_WIN98,
	WV9XM_WIN98SE,
	WV9XM_COUNT
};

const BYTE c_rgWinverIDs[WV9XM_COUNT]
	= { IDS_WIN95,           IDS_WIN98,           IDS_WIN98 };
const BYTE c_rgWinverCopyrightIDs[WV9XM_COUNT]
	= { IDS_COPYRIGHT_WIN95, IDS_COPYRIGHT_WIN98, IDS_COPYRIGHT_WIN98SE };

INT_PTR CALLBACK WinverDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HKEY hKey;
	DWORD dwMode = 0;
	DWORD cbMode = sizeof(DWORD);
	TCHAR szBuffer[64];

	switch (uMsg)
	{
		case WM_INITDIALOG:
			RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\winver9x"), NULL, KEY_READ, &hKey);
			if (hKey)
			{
				RegQueryValueEx(hKey, TEXT("Mode"), NULL, NULL, &dwMode, &cbMode);
			}

			if (dwMode >= WV9XM_COUNT)
				dwMode = WV9XM_WIN95;

			LoadString(HINST_THISCOMPONENT, c_rgWinverIDs[dwMode], szBuffer, ARRAYSIZE(szBuffer));
			SetDlgItemText(hwnd, IDC_WINVER, szBuffer);

			LoadString(HINST_THISCOMPONENT, c_rgWinverCopyrightIDs[dwMode], szBuffer, ARRAYSIZE(szBuffer));
			SetDlgItemText(hwnd, IDC_WINVER_COPYRIGHT, szBuffer);

			return TRUE;
		case WM_COMMAND:
			EndDialog(hwnd, 0);
			return TRUE;
		default:
			return FALSE;
	}
}

int WINAPI WinMainCRTStartup(void)
{
	DialogBoxParam(HINST_THISCOMPONENT, MAKEINTRESOURCE(IDD_WINVER), NULL, WinverDlgProc, NULL);
	ExitProcess(0);
	return 0;
}