#pragma once
#include "Canvas.h"

// клас дял работы с игровым полем
class GameField
{
public:
	// установка размера игрового поля
	void Resize(int width, int height);
	// отрисовка границ игрового поля
	void Draw(Canvas& canvas);

private:
	// размеры поля, будут заданы в щависимости от количества лунок
	// ширина поля
	int p_Width;
	// высота поля
	int p_Height;
};

