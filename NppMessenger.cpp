/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  NppMessenger v1.0.0                                                    //
//  Copyright (c) 2019-2022 Frank Fesevur                                  //
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

#include "NppMessenger.h"

NppMessenger::NppMessenger() noexcept
{
	m_nppData._nppHandle = nullptr;
	m_nppData._scintillaMainHandle = nullptr;
	m_nppData._scintillaSecondHandle = nullptr;
}

/**
 *  @brief Brief description
 *
 *  @param [in] notpadPlusData Description for notpadPlusData
 *  @return Return description
 *
 *  @details More details
 */
NppMessenger::NppMessenger(const NppData& notpadPlusData) noexcept
{
	SetNppData(notpadPlusData);
}

/**
 *  @brief Brief description
 *
 *  @param [in] notpadPlusData Description for notpadPlusData
 *  @return Return description
 *
 *  @details More details
 */
void NppMessenger::SetNppData(const NppData& notpadPlusData) noexcept
{
	m_nppData = notpadPlusData;
}

/**
 *  @brief Brief description
 *
 *  @param [in] uMsg   Description for uMsg
 *  @param [in] wParam Description for wParam
 *  @param [in] lParam Description for lParam
 *  @return Return description
 *
 *  @details More details
 */
LRESULT NppMessenger::SendNppMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) const noexcept
{
	return SendMessage(m_nppData._nppHandle, uMsg, wParam, lParam);
}

/**
 *  @brief Brief description
 *
 *  @param [in] uMsg   Description for uMsg
 *  @param [in] wParam Description for wParam
 *  @param [in] lParam Description for lParam
 *  @return Return description
 *
 *  @details More details
 */
LRESULT NppMessenger::SendSciMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) const noexcept
{
	int currentEdit = 0;
	SendMessage(m_nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM) &currentEdit);

	const HWND hWndScintilla = (currentEdit == 0) ? m_nppData._scintillaMainHandle : m_nppData._scintillaSecondHandle;
	return SendMessage(hWndScintilla, uMsg, wParam, lParam);
}

/**
 *  @brief Brief description
 *
 *  @return Return description
 *
 *  @details More details
 */
int NppMessenger::GetLineCount() const noexcept
{
	return (int) SendSciMsg(SCI_GETLINECOUNT);
}

/**
 *  @brief Brief description
 *
 *  @param [in] line Description for line
 *  @return Return description
 *
 *  @details More details
 */
int NppMessenger::GetLineLength(const int line) const noexcept
{
	return (int) SendSciMsg(SCI_LINELENGTH, (WPARAM) line);
}

/**
 *  @brief Brief description
 *
 *  @param [in] line    Description for line
 *  @param [in] textbuf Description for textbuf
 *  @return Return description
 *
 *  @details More details
 */
int NppMessenger::GetLine(const int line, char* textbuf) const noexcept
{
	return (int) SendSciMsg(SCI_GETLINE, (WPARAM) line, (LPARAM) textbuf);
}

/**
 *  @brief Brief description
 *
 *  @return Return description
 *
 *  @details More details
 */

int NppMessenger::GetCurrentPos() const noexcept
{
	return (int) SendSciMsg(SCI_GETCURRENTPOS);
}

/**
 *  @brief Brief description
 *
 *  @return Return description
 *
 *  @details More details
 */

void NppMessenger::SetSelectionStart(const int start) const noexcept
{
	SendSciMsg(SCI_SETSELECTIONSTART, start);
}

/**
 *  @brief Brief description
 *
 *  @return Return description
 *
 *  @details More details
 */

void NppMessenger::SetSelectionEnd(const int end) const noexcept
{
	SendSciMsg(SCI_SETSELECTIONEND, end);
}

/**
 *  @brief Brief description
 *
 *  @param [in] width Description for width
 *  @return Return description
 *
 *  @details More details
 */
void NppMessenger::SetTabWidth(const int width) const noexcept
{
	SendSciMsg(SCI_SETTABWIDTH, width);
}

/**
 *  @brief Brief description
 *
 *  @param [in] usetabs Description for usetabs
 *  @return Return description
 *
 *  @details More details
 */
void NppMessenger::SetUseTabs(const bool usetabs) const noexcept
{
	SendSciMsg(SCI_SETUSETABS, usetabs ? 1 : 0);
}

/**
 *  @brief Brief description
 *
 *  @param [in] uMsg Description for uMsg
 *  @param [in] str  Description for str
 *  @return Return description
 *
 *  @details More details
 */
bool NppMessenger::GetStringFromNpp(const UINT uMsg, std::wstring& str) const
{
	WCHAR szStr[_MAX_PATH] = { 0 };
	if (SendNppMsg(uMsg, _MAX_PATH, (LPARAM) szStr))
	{
		str = szStr;
		return true;
	}

	return false;
}

/**
 *  @brief Brief description
 *
 *  @param [in] filename Description for filename
 *  @return Return description
 *
 *  @details More details
 */
bool NppMessenger::GetFilename(std::wstring& filename) const
{
	return GetStringFromNpp(NPPM_GETFILENAME, filename);
}

/**
 *  @brief Brief description
 *
 *  @param [in] path Description for path
 *  @return Return description
 *
 *  @details More details
 */
bool NppMessenger::GetCurrentFullPath(std::wstring& path) const
{
	return GetStringFromNpp(NPPM_GETFULLCURRENTPATH, path);
}

/**
 *  @brief Brief description
 *
 *  @param [in] path Description for path
 *  @return Return description
 *
 *  @details More details
 */
bool NppMessenger::GetPluginConfigDir(std::wstring& path) const
{
	return GetStringFromNpp(NPPM_GETPLUGINSCONFIGDIR, path);
}

/**
 *  @brief Brief description
 *
 *  @param [in] lang Description for lang
 *  @return Return description
 *
 *  @details More details
 */
void NppMessenger::SetLanguage(const LangType lang) const noexcept
{
	SendNppMsg(NPPM_SETCURRENTLANGTYPE, 0, lang);
}

/**
 *  @brief Brief description
 *
 *  @return Return description
 *
 *  @details More details
 */
LangType NppMessenger::GetLanguage() const noexcept
{
	LangType lang = L_EXTERNAL;
	SendNppMsg(NPPM_GETCURRENTLANGTYPE, 0, (LPARAM) &lang);
	return lang;
}

/**
 *  @brief Brief description
 *
 *  @return Return description
 *
 *  @details More details
 */

COLORREF NppMessenger::GetEditorDefaultForegroundColor() const noexcept
{
	return (COLORREF) SendNppMsg(NPPM_GETEDITORDEFAULTFOREGROUNDCOLOR);
}

/**
 *  @brief Brief description
 *
 *  @return Return description
 *
 *  @details More details
 */

COLORREF NppMessenger::GetEditorDefaultBackgroundColor() const noexcept
{
	return (COLORREF) SendNppMsg(NPPM_GETEDITORDEFAULTBACKGROUNDCOLOR);
}
