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
#include <assert.h>

#include "NPP/PluginInterface.h"
#include "NPP/SciLexer.h"
#include "Resource.h"
#include "SelectQuotedText.h"
#include "DlgAbout.h"

static constexpr int nbFunc = 3;
static const TCHAR PLUGIN_NAME[] = L"SelectQuotedText";

HINSTANCE g_hInst;
NppMessenger g_Msgr;
static FuncItem s_funcItem[nbFunc];

/////////////////////////////////////////////////////////////////////////////
// These languages are mapped by N++ to SCLEX defines. Scintilla has many
// more languages but N++ doesn't use them (yet), so they are not used here.
// Only the string related SCE defines are used. The defines used here are an
// educated guess and may not work yet as expected for all languages. If
// there is just a "return false" no useful SCE defines were found for that
// languages.

static bool CheckInString(int lexer, int style) noexcept
{
	switch (lexer)
	{
		case SCLEX_ADA:
			switch (style)
			{
				case SCE_ADA_STRING:
				case SCE_ADA_STRINGEOL:
					return true;
				default:
					return false;
			}
		case SCLEX_ASM:
			switch (style)
			{
				case SCE_ASM_STRING:
				case SCE_ASM_STRINGEOL:
					return true;
				default:
					return false;
			}
		case SCLEX_AU3:
			switch (style)
			{
				case SCE_AU3_STRING:
					return true;
				default:
					return false;
			}
		case SCLEX_BASH:
			switch (style)
			{
				case SCE_SH_STRING:
				case SCE_SH_BACKTICKS:
				case SCE_SH_HERE_DELIM:
				case SCE_SH_HERE_Q:
					return true;
				default:
					return false;
			}
		case SCLEX_BATCH:
			return false;
		case SCLEX_CAML:
			switch (style)
			{
				case SCE_CAML_CHAR:
				case SCE_CAML_STRING:
					return true;
				default:
					return false;
			}
		case SCLEX_CMAKE:
			switch (style)
			{
				case SCE_CMAKE_STRINGDQ:
				case SCE_CMAKE_STRINGLQ:
				case SCE_CMAKE_STRINGRQ:
					return true;
				default:
					return false;
			}
		case SCLEX_COBOL:
			return false;
		case SCLEX_COFFEESCRIPT:
			switch (style)
			{
				case SCE_COFFEESCRIPT_STRING:
				case SCE_COFFEESCRIPT_STRINGEOL:
				case SCE_COFFEESCRIPT_REGEX:
				case SCE_COFFEESCRIPT_STRINGRAW:
					return true;
				default:
					return false;
			}
		case SCLEX_CPP:
			switch (style)
			{
				case SCE_C_STRING:
				case SCE_C_CHARACTER:
				case SCE_C_STRINGEOL:
				case SCE_C_REGEX:
				case SCE_C_STRINGRAW:
				case SCE_C_HASHQUOTEDSTRING:
					return true;
				default:
					return false;
			}
		case SCLEX_CSS:
			switch (style)
			{
				case SCE_CSS_DOUBLESTRING:
				case SCE_CSS_SINGLESTRING:
					return true;
				default:
					return false;
			}
		case SCLEX_D:
			switch (style)
			{
				case SCE_D_STRING:
				case SCE_D_STRINGEOL:
				case SCE_D_STRINGB:
				case SCE_D_STRINGR:
					return true;
				default:
					return false;
			}
		case SCLEX_DIFF:
			return false;
		case SCLEX_F77:
		case SCLEX_FORTRAN:
			switch (style)
			{
				case SCE_F_STRING1:
				case SCE_F_STRING2:
				case SCE_F_STRINGEOL:
					return true;
				default:
					return false;
			}
		case SCLEX_GUI4CLI:
			switch (style)
			{
				case SCE_GC_STRING:
					return true;
				default:
					return false;
			}
		case SCLEX_HASKELL:
			switch (style)
			{
				case SCE_HA_STRING:
				case SCE_HA_CHARACTER:
					return true;
				default:
					return false;
			}
		case SCLEX_HTML:
		case SCLEX_XML:
			switch (style)
			{
				case SCE_H_DOUBLESTRING:
				case SCE_H_SINGLESTRING:
				case SCE_HBA_STRING:
				case SCE_HB_STRING:
				case SCE_HB_STRINGEOL:
				case SCE_HJA_DOUBLESTRING:
				case SCE_HJA_SINGLESTRING:
				case SCE_HJA_STRINGEOL:
				case SCE_HJ_DOUBLESTRING:
				case SCE_HJ_SINGLESTRING:
				case SCE_HJ_STRINGEOL:
				case SCE_HPA_CHARACTER:
				case SCE_HPA_STRING:
				case SCE_HPA_TRIPLE:
				case SCE_HPA_TRIPLEDOUBLE:
				case SCE_HPHP_COMPLEX_VARIABLE:
				case SCE_HPHP_HSTRING:
				case SCE_HPHP_HSTRING_VARIABLE:
				case SCE_HPHP_SIMPLESTRING:
				case SCE_HP_CHARACTER:
				case SCE_HP_STRING:
				case SCE_HP_TRIPLE:
				case SCE_HP_TRIPLEDOUBLE:
					return true;
				default:
					return false;
			}
		case SCLEX_INNOSETUP:
			switch (style)
			{
				case SCE_INNO_STRING_DOUBLE:
				case SCE_INNO_STRING_SINGLE:
					return true;
				default:
					return false;
			}
		case SCLEX_KIX:
			switch (style)
			{
				case SCE_KIX_STRING1:
				case SCE_KIX_STRING2:
					return true;
				default:
					return false;
			}
		case SCLEX_LISP:
			switch (style)
			{
				case SCE_LISP_STRING:
				case SCE_LISP_STRINGEOL:
					return true;
				default:
					return false;
			}
		case SCLEX_LUA:
			switch (style)
			{
				case SCE_LUA_STRING:
				case SCE_LUA_STRINGEOL:
				case SCE_LUA_CHARACTER:
				case SCE_LUA_LITERALSTRING:
					return true;
				default:
					return false;
			}
		case SCLEX_MAKEFILE:
			return false;
		case SCLEX_MATLAB:
			switch (style)
			{
				case SCE_MATLAB_STRING:
				case SCE_MATLAB_DOUBLEQUOTESTRING:
					return true;
				default:
					return false;
			}
		case SCLEX_NSIS:
			switch (style)
			{
				case SCE_NSIS_STRINGDQ:
				case SCE_NSIS_STRINGLQ:
				case SCE_NSIS_STRINGRQ:
					return true;
				default:
					return false;
			}
		case SCLEX_PASCAL:
			switch (style)
			{
				case SCE_PAS_STRING:
				case SCE_PAS_STRINGEOL:
				case SCE_PAS_CHARACTER:
					return true;
				default:
					return false;
			}
		case SCLEX_PERL:
			switch (style)
			{
				case SCE_PL_STRING:
				case SCE_PL_CHARACTER:
				case SCE_PL_STRING_Q:
				case SCE_PL_STRING_QQ:
				case SCE_PL_STRING_QX:
				case SCE_PL_STRING_QR:
				case SCE_PL_STRING_QW:
					return true;
				default:
					return false;
			}
		case SCLEX_POWERSHELL:
			switch (style)
			{
				case SCE_POWERSHELL_STRING:
				case SCE_POWERSHELL_CHARACTER:
					return true;
				default:
					return false;
			}
		case SCLEX_PROPERTIES:
			return false;
		case SCLEX_PS:
			switch (style)
			{
				case SCE_PS_TEXT:
				case SCE_PS_HEXSTRING:
				case SCE_PS_BASE85STRING:
				case SCE_PS_BADSTRINGCHAR:
					return true;
				default:
					return false;
			}
		case SCLEX_PYTHON:
			switch (style)
			{
				case SCE_P_STRING:
				case SCE_P_STRINGEOL:
				case SCE_P_TRIPLE:
				case SCE_P_TRIPLEDOUBLE:
					return true;
				default:
					return false;
			}
		case SCLEX_R:
			switch (style)
			{
				case SCE_R_STRING:
				case SCE_R_STRING2:
					return true;
				default:
					return false;
			}
		case SCLEX_RUBY:
			switch (style)
			{
				case SCE_RB_STRING:
				case SCE_RB_STRING_Q:
				case SCE_RB_STRING_QQ:
				case SCE_RB_STRING_QX:
				case SCE_RB_STRING_QR:
				case SCE_RB_STRING_QW:
					return true;
				default:
					return false;
			}
		case SCLEX_SEARCHRESULT:
			return false;
		case SCLEX_SMALLTALK:
			switch (style)
			{
				case SCE_ST_STRING:
				case SCE_ST_CHARACTER:
					return true;
				default:
					return false;
			}
		case SCLEX_SQL:
			switch (style)
			{
				case SCE_SQL_STRING:
				case SCE_SQL_QUOTEDIDENTIFIER:
					return true;
				default:
					return false;
			}
		case SCLEX_TCL:
			switch (style)
			{
				case SCE_TCL_IN_QUOTE:
				case SCE_TCL_WORD_IN_QUOTE:
					return true;
				default:
					return false;
			}
		case SCLEX_TEX:
			switch (style)
			{
				case SCE_TEX_TEXT:
					return true;
				default:
					return false;
			}
		case SCLEX_USER:
			return false;
		case SCLEX_VB:
			switch (style)
			{
				case SCE_B_STRING:
				case SCE_B_STRINGEOL:
					return true;
				default:
					return false;
			}
		case SCLEX_VERILOG:
			switch (style)
			{
				case SCE_V_STRING:
					return true;
				default:
					return false;
			}
		case SCLEX_VHDL:
			switch (style)
			{
				case SCE_VHDL_STRING:
				case SCE_VHDL_STRINGEOL:
					return true;
				default:
					return false;
			}
		case SCLEX_YAML:
			switch (style)
			{
				case SCE_YAML_TEXT:
					return true;
				default:
					return false;
			}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
//

static void SelectQuotedText() noexcept
{
	// Get current position
	const int pos = g_Msgr.GetCurrentPos();

	// Get style of that position
	const int lexer = g_Msgr.SendSciMsg(SCI_GETLEXER);
	const int style = g_Msgr.SendSciMsg(SCI_GETSTYLEAT, pos);

	// Are we within a string?
	int start = pos, end = pos;
	if (CheckInString(lexer, style))
	{
		// As long as we are within the same style we need to move on
		while (g_Msgr.SendSciMsg(SCI_GETSTYLEAT, start - 1) == style)
			start--;
		start++;
		while (g_Msgr.SendSciMsg(SCI_GETSTYLEAT, end + 1) == style)
			end++;
	}
	else
	{
		// Just get the begin and end of the current word
		start = g_Msgr.SendSciMsg(SCI_WORDSTARTPOSITION, pos, true);
		end = g_Msgr.SendSciMsg(SCI_WORDENDPOSITION, pos, true);
	}

	// Is there anything to be selected?
	if (start == end)
		return;

	// Set the selection
	g_Msgr.SetSelectionStart(start);
	g_Msgr.SetSelectionEnd(end);
}

/////////////////////////////////////////////////////////////////////////////
//

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
	g_Msgr.SetNppData(notpadPlusData);
}

/////////////////////////////////////////////////////////////////////////////
//

extern "C" __declspec(dllexport) const TCHAR* getName()
{
	return PLUGIN_NAME;
}

/////////////////////////////////////////////////////////////////////////////
//

extern "C" __declspec(dllexport) FuncItem* getFuncsArray(int *nbF)
{
	*nbF = nbFunc;
	return s_funcItem;
}

/////////////////////////////////////////////////////////////////////////////
//

extern "C" __declspec(dllexport) BOOL isUnicode()
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//

extern "C" __declspec(dllexport) void beNotified(SCNotification* notifyCode)
{
	UNREFERENCED_PARAMETER(notifyCode);
}

/////////////////////////////////////////////////////////////////////////////
// Here you can process the Npp Messages
// I will make the messages accessible little by little, according to the
// need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781

extern "C" __declspec(dllexport) LRESULT messageProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(uMsg);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Make the window center, relative the NPP-window

void CenterWindow(HWND hDlg) noexcept
{
	RECT rc;
	GetClientRect(g_Msgr.GetNppHandle(), &rc);

	POINT center{};
	const int w = rc.right - rc.left;
	const int h = rc.bottom - rc.top;
	center.x = rc.left + (w / 2);
	center.y = rc.top + (h / 2);
	ClientToScreen(g_Msgr.GetNppHandle(), &center);

	RECT dlgRect;
	GetClientRect(hDlg, &dlgRect);
	const int x = center.x - ((dlgRect.right - dlgRect.left) / 2);
	const int y = center.y - ((dlgRect.bottom - dlgRect.top) / 2);

	SetWindowPos(hDlg, HWND_TOP, x, y, -1, -1, SWP_NOSIZE | SWP_SHOWWINDOW);
}

/////////////////////////////////////////////////////////////////////////////
// The entry point of the DLL

BOOL APIENTRY DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);

    switch (reasonForCall)
    {
		case DLL_PROCESS_ATTACH:
		{
			g_hInst = (HINSTANCE) hModule;

			// Handle Select Quoted Text
			int index = 0;
			s_funcItem[index]._pFunc = SelectQuotedText;
			wcscpy_s(s_funcItem[index]._itemName, L"Select quoted text");
			s_funcItem[index]._init2Check = false;
			s_funcItem[index]._pShKey = new ShortcutKey;
			s_funcItem[index]._pShKey->_isAlt = true;
			s_funcItem[index]._pShKey->_isCtrl = false;
			s_funcItem[index]._pShKey->_isShift = false;
			s_funcItem[index]._pShKey->_key = VK_OEM_7;		// Mapped by N++ to ' key
			index++;

			// Separator
			s_funcItem[index]._pFunc = NULL;
			wcscpy_s(s_funcItem[index]._itemName, L"-SEPARATOR-");
			s_funcItem[index]._init2Check = false;
			s_funcItem[index]._pShKey = NULL;
			index++;

			// Show About Dialog
			s_funcItem[index]._pFunc = ShowAboutDlg;
			wcscpy_s(s_funcItem[index]._itemName, L"About...");
			s_funcItem[index]._init2Check = false;
			s_funcItem[index]._pShKey = NULL;
			index++;
			assert(index == nbFunc);
			break;
		}

		case DLL_PROCESS_DETACH:
		{
			// Clean up the shortcuts
			for (int i = 0; i < nbFunc; i++)
			{
				if (s_funcItem[i]._pShKey != NULL)
				{
					delete s_funcItem[i]._pShKey;
				}
			}
			break;
		}

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
			break;
    }
    return TRUE;
}
