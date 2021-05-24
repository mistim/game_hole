#pragma once
#include <array>
#include "Canvas.h"

// ����� ��� ������ � �����
class Ball
{
public:
	Ball(int position, wchar_t color, wchar_t type);

	// ������ ������ ����
	static const wchar_t ColorWhite = 0x25CF;
	// ������ ������� ����
	static const wchar_t ColorBlack = 0x25CB;
	// ������ �������, ������������ ��� ������� ��������� ����
	static const wchar_t ColorEmpty = 0x0020;
	// ������ ������ �����
	static const wchar_t ColorBlank = 0x25CC;
	// ���� 
	static const char White = 'W';	// ����� ���
	static const char Black = 'B';	// ������ ���
	static const char Hole = 'H';	// �����
	
	// ��������� �������
	void SetPosition(int position);
	// ��������� �������
	int GetPosition();
	// ��������� ����
	wchar_t GetType();
	// ���������
	void Draw(Canvas& canvas);
	// ����������� ����
	void Move();
	// ������� �������� �����
	void BlinkActive(double dt);
	// ������������� ����� ����������
	void RestoreColor();

private:
	// ������� ����
	int p_Position;
	// ���� ����
	wchar_t p_Color;
	// ���
	wchar_t p_Type;
	// ����� ���������� ����������, ������������ ��� ������� ��������� ����
	double p_TimeFromLastUpdate = 0;
	// ����, ������� ���� ��� �� ������ �������
	wchar_t p_ColorRestore;
	// ���� �������, ������������ ��� ����� ������� ��������� ���� ��� �������
	bool p_BlinkFlag = true;

	// ���������� �����
	void SetColor(wchar_t color);
};

