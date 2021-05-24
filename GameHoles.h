#pragma once

#include <vector>
#include "Game.h"
#include "GameField.h"
#include "Canvas.h"
#include "Ball.h"

// класс игры
class GameHoles : public Game
{
public:
	GameHoles();

	// обработчик нажатия клавиш
	void OnKeyPressed(int btnCode) override;
	// обновление элементов
	void Update(double dt) override;
	// определение окончания игры
	// todo
	bool End() override;

private:
	// количество лунок
	int p_NumHoles;
	// количество шаров каждого цвета
	int p_NumBalls;
	// активный игрок
	int p_ActivePalyer;
	// активный белый шар
	int p_ActiveWhiteBall = 0;
	// активный черный шар
	int p_ActiveBlackBall = 0;
	// минимальная позиция шара
	int p_MinPosition = 0;
	// максимальная позиция шара
	int p_MaxPosition = 0;
	int p_MinPositionWhite = 0;
	int p_MaxPositionWhite = 0;
	int p_MinPositionBlack = 0;
	int p_MaxPositionBlack = 0;
	// игровое поле
	GameField p_GameField;
	Canvas p_Canvas;
	// количест шаров (набор объектов класса Ball)
	std::vector<Ball> p_Balls = {};
	// пустая лунка
	Ball p_EmptyHole;

	// смена активного шара, который собираемся перемещать
	void ChangeActiveBall(wchar_t direction, wchar_t type, int &active_position, int min, int max);
	// перемешение активного шара
	void MoveBall(int active_position);
};

