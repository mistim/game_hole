#pragma once
#include <array>
#include "Canvas.h"

// класс для работы с шаром
class Ball
{
public:
	Ball(int position, wchar_t color, wchar_t type);

	// символ белого шара
	static const wchar_t ColorWhite = 0x25CF;
	// символ черного шара
	static const wchar_t ColorBlack = 0x25CB;
	// символ пробела, используется при мигании активного шара
	static const wchar_t ColorEmpty = 0x0020;
	// символ пустой лунки
	static const wchar_t ColorBlank = 0x25CC;
	// типы 
	static const char White = 'W';	// белый шар
	static const char Black = 'B';	// черный шар
	static const char Hole = 'H';	// лунка
	
	// установка позиции
	void SetPosition(int position);
	// получение позиции
	int GetPosition();
	// получение типа
	wchar_t GetType();
	// отрисовка
	void Draw(Canvas& canvas);
	// перемещение шара
	void Move();
	// мигание активным шаром
	void BlinkActive(double dt);
	// востановление цвета примигании
	void RestoreColor();

private:
	// позиция шара
	int p_Position;
	// цвет шара
	wchar_t p_Color;
	// тип
	wchar_t p_Type;
	// время последнего обновления, используется для мигания активного шара
	double p_TimeFromLastUpdate = 0;
	// цвет, который имел шар до начала мигания
	wchar_t p_ColorRestore;
	// флаг мигания, используется для смены символа активного шара при мигании
	bool p_BlinkFlag = true;

	// усстановка цвета
	void SetColor(wchar_t color);
};

