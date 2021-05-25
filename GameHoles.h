#pragma once

#include <vector>
#include <algorithm>
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
	// позиция пустой лунки
	int p_StartPosEmptyHole;
	// активный игрок (белые или черные шары)
	char p_ActivePlayer;
	// позиция активного белого шара
	int p_ActiveWhiteBall = 0;
	// позиция активного черного шара
	int p_ActiveBlackBall = 0;
	// минимальная позиция шара
	int p_MinPosition = 0;
	// максимальная позиция шара
	int p_MaxPosition = 0;
	// победитель
	char p_Winner;
	// игровое поле
	GameField p_GameField;
	Canvas p_Canvas;
	// количест шаров (набор объектов класса Ball)
	std::vector<Ball> p_Balls = {};
	// пустая лунка
	Ball p_EmptyHole;

	// выбор шара, который собираемся перемещать
	void p_ChangeActiveBall(wchar_t direction, int &active_position);
	// перемещение выбранного шара
	void p_MoveBall(int active_position);
	// пересортировка перечня шаров, необходима для того, что бы правильно менять активный шар
	void p_ReSort();
	// проверка, если ли победитель
	bool p_ThereIsWinner();
	// обновление стартовых параметров
	void p_Reset();
};

