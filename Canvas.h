#pragma once

#include<Windows.h>

class Canvas
{
public:
	Canvas();
	~Canvas();

	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);
	void SetColorText(int color);
	void Resize(int width, int height);
	void Render();
	void Clear();

private:
	HANDLE m_ConsoleOut;
	CHAR_INFO* m_Buffer = nullptr;
	COORD m_BufferSize;
	COORD m_BufferCoord;
	SMALL_RECT m_WriteRegion;
};

