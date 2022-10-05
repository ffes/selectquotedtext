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

#pragma once

#include <string>
#include "NPP/PluginInterface.h"

class NppMessenger
{
public:
	NppMessenger() noexcept;
	NppMessenger(const NppData& notpadPlusData) noexcept;

	void SetNppData(const NppData& notpadPlusData) noexcept;
	HWND GetNppHandle() const noexcept { return m_nppData._nppHandle; };

	LRESULT SendNppMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const noexcept;
	LRESULT SendSciMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const noexcept;

	int		GetLineCount() const noexcept;
	int		GetLineLength(int line) const noexcept;
	int		GetLine(int line, char* textbuf) const noexcept;
	LangType GetLanguage() const noexcept;

	int		GetCurrentPos() const noexcept;
	void	SetSelectionStart(const int start) const noexcept;
	void	SetSelectionEnd(const int end) const noexcept;

	bool	GetCurrentFullPath(std::wstring& path) const;
	bool	GetFilename(std::wstring& filename) const;
	bool	GetPluginConfigDir(std::wstring& path) const;

	void	SetTabWidth(int width) const noexcept;
	void	SetUseTabs(bool usetabs) const noexcept;
	void	SetLanguage(LangType lang) const noexcept;

	COLORREF GetEditorDefaultForegroundColor() const noexcept;
	COLORREF GetEditorDefaultBackgroundColor() const noexcept;

protected:
	NppData m_nppData;

	bool	GetStringFromNpp(UINT uMsg, std::wstring& str) const;
};
