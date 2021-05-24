#include "Canvas.h"

#include <iostream>

void SetFont() {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 30;
	cfi.dwFontSize.Y = 60;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	//wcscpy_s(cfi.FaceName, L"Lucida Console");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

namespace {
	int DEFAULT_WIDTH = 5;
	int DEFAULT_HEIGHT = 3;
}

Canvas::Canvas()
{
	SetFont();
	m_ConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);


	Resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(m_ConsoleOut, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(m_ConsoleOut, &cursorInfo);
}

Canvas::~Canvas()
{
	delete m_Buffer;
}

void Canvas::Resize(int width, int height)
{
	width = max(width, DEFAULT_WIDTH);
	height = max(height, DEFAULT_HEIGHT);
	// { Left, Top, Right, Left }
	SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
	COORD windowBufSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };

	if (!SetConsoleScreenBufferSize(m_ConsoleOut, windowBufSize))
		std::cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << std::endl;

	if (!SetConsoleWindowInfo(m_ConsoleOut, TRUE, &windowSize))
		std::cout << "SetConsoleWindowInfo failed with error " << GetLastError() << std::endl;

	if (m_Buffer) delete[] m_Buffer;

	m_Buffer = new CHAR_INFO[height * width];
	m_BufferSize.X = width;
	m_BufferSize.Y = height;
	m_BufferCoord.X = 0;
	m_BufferCoord.Y = 0;

	m_WriteRegion.Left = 0;
	m_WriteRegion.Top = 0;
	m_WriteRegion.Right = width;
	m_WriteRegion.Bottom = height;

	Clear();
}

void Canvas::SetChar(int x, int y, wchar_t c)
{
	if (x < 0 || x >= m_BufferSize.X) return;
	if (y < 0 || y >= m_BufferSize.Y) return;

	const int idx = x + m_BufferSize.X * y;
	m_Buffer[idx].Char.UnicodeChar = c;
	m_Buffer[idx].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
}

wchar_t Canvas::GetChar(int x, int y)
{
	if (x < 0 || x >= m_BufferSize.X) return L'\0';
	if (y < 0 || y >= m_BufferSize.Y) return L'\0';
	const int idx = x + m_BufferSize.X * y;
	return m_Buffer[idx].Char.UnicodeChar;
}

void Canvas::SetColorText(int color)
{
	//FlushConsoleInputBuffer(m_ConsoleOut);
	SetConsoleTextAttribute(m_ConsoleOut, color);
}

void Canvas::Clear()
{
	for (int x = 0; x < m_BufferSize.X; x++)
	{
		for (int y = 0; y < m_BufferSize.Y; y++)
		{
			SetChar(x, y, L' ');
		}
	}
}

void Canvas::Render()
{
	if (!WriteConsoleOutput(m_ConsoleOut, m_Buffer, m_BufferSize, m_BufferCoord, &m_WriteRegion))
		std::cout << "WriteConsoleOutput failed - (%d)\n" << GetLastError() << std::endl;
}