SelectQuotedText
================

Simple Notepad++ plug-in that selects the text in quotes. It uses the Scintilla
lexers Notepad++ is based on to determine what "quoted text" is. If no quoted
text is found, the current word is selected.

## How to install ##

Download the lastest version from https://github.com/ffes/selectquotedtext/releases
and copy `SelectQuotedText.dll` in the `plugins` directory. Then (re)start
Notepad++.


## How to use ##

When the cursor is in a quoted text, press `Alt+'` to select that text.
You can adjust the hotkey in `Settings`, `Shortcut Mapper`, `Plugin commands`.


## Known issues ##

*  In C and C++ the string `L"Text"` is not selected properly.

*  In Python a text between triple quotes is not selected properly.

*  In HTML a URL in quotes is recognized as URL (it is underlined and clickable)
   by the lexer and therefore the plug-in will just select the current word.


## Version 1.0.0 (2016-12-05) ##

*  Initial release
