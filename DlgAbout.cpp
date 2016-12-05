/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  SelectQuotedText                                                       //
//  Copyright (c) 2016 Frank Fesevur                                       //
//                                                                         //
//  This program is free software; you can redistribute it and/or modify   //
//  it under the terms of the GNU General Public License as published by   //
//  the Free Software Foundation; either version 2 of the License, or      //
//  (at your option) any later version.                                    //
//                                                                         //
//  This program is distributed in the hope that it will be useful,        //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of         //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           //
//  GNU General Public License for more details.                           //
//                                                                         //
//  You should have received a copy of the GNU General Public License      //
//  along with this program; if not, write to the Free Software            //
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.              //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <string>
#include <commctrl.h>

#include "NPP/PluginInterface.h"
#include "SelectQuotedText.h"
#include "Resource.h"
#include "Version.h"

/////////////////////////////////////////////////////////////////////////////
//

struct VersionInfo
{
	BYTE	version[VERSION_DIGITS];
	int		date[3];
	WCHAR*	text;
};

static const int MAX_VERSION_INFO = 1;

static VersionInfo s_info[MAX_VERSION_INFO] =
{
	{	{1,0,0,0},	{2016,12, 5},	L"Initial release" }
};

static int s_showTill = MAX_VERSION_INFO;

///////////////////////////////////////////////////////////////////////////////
//

static BOOL OnInitDialog(HWND hDlg)
{
	CenterWindow(hDlg);

	// Show the relevant part of the changelog
	std::wstring txt;
	WCHAR szTmp[_MAX_PATH];
	for (int i = 0; i < s_showTill; i++)
	{
		if (!txt.empty())
			txt += L"\r\n\r\n";

		// Add the version number
		txt += L"Version ";
		swprintf(szTmp, _MAX_PATH, L"%d.%d.%d", s_info[i].version[0], s_info[i].version[1], s_info[i].version[2]);
		txt += szTmp;

		// Add the release date
		struct tm released;
		ZeroMemory(&released, sizeof(tm));
		released.tm_year = s_info[i].date[0] - 1900;
		released.tm_mon = s_info[i].date[1] - 1;
		released.tm_mday = s_info[i].date[2];

		txt += L", released on ";
		wcsftime(szTmp,_MAX_PATH, L"%d-%b-%Y", &released);
		txt += szTmp;
		txt += L"\r\n";

		// Add the changelog
		txt += s_info[i].text;
	}
	SetDlgItemText(hDlg, IDC_CHANGELOG, txt.c_str());

	// Let windows set focus
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//

static BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch(message)
	{
		case WM_INITDIALOG:
		{
			return OnInitDialog(hDlg);
		}
		case WM_NOTIFY:
		{
			switch (((LPNMHDR)lParam)->code)
			{
				case NM_CLICK:
				case NM_RETURN:
				{
					PNMLINK pNMLink = (PNMLINK) lParam;
					LITEM item = pNMLink->item;
					ShellExecute(NULL, L"open", item.szUrl, NULL, NULL, SW_SHOW);
				}
			}
			return FALSE;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDCANCEL:
				{
					EndDialog(hDlg, 0);
					return TRUE;
				}
			}
			return FALSE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Show the About Dialog, with all version information

void ShowAboutDlg()
{
	s_showTill = MAX_VERSION_INFO;
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), g_nppData._nppHandle, (DLGPROC) DlgProc);
}

///////////////////////////////////////////////////////////////////////////////
// Show the About Dialog, with version information until 'prevVer'

void ShowAboutDlgVersion(Version prevVer)
{
	s_showTill = MAX_VERSION_INFO;
	for (int i = 0; i < MAX_VERSION_INFO; i++)
	{
		Version ver(s_info[i].version);
		if (ver == prevVer)
		{
			s_showTill = i;
			break;
		}
	}

	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), g_nppData._nppHandle, (DLGPROC) DlgProc);
}
