#pragma once

#include <vector>
#include "Game.h"
#include "GameField.h"
#include "Canvas.h"
#include "Ball.h"

// ����� ����
class GameHoles : public Game
{
public:
	GameHoles();

	// ���������� ������� ������
	void OnKeyPressed(int btnCode) override;
	// ���������� ���������
	void Update(double dt) override;
	// ����������� ��������� ����
	// todo
	bool End() override;

private:
	// ���������� �����
	int p_NumHoles;
	// ���������� ����� ������� �����
	int p_NumBalls;
	// �������� �����
	int p_ActivePalyer;
	// �������� ����� ���
	int p_ActiveWhiteBall = 0;
	// �������� ������ ���
	int p_ActiveBlackBall = 0;
	// ����������� ������� ����
	int p_MinPosition = 0;
	// ������������ ������� ����
	int p_MaxPosition = 0;
	int p_MinPositionWhite = 0;
	int p_MaxPositionWhite = 0;
	int p_MinPositionBlack = 0;
	int p_MaxPositionBlack = 0;
	// ������� ����
	GameField p_GameField;
	Canvas p_Canvas;
	// �������� ����� (����� �������� ������ Ball)
	std::vector<Ball> p_Balls = {};
	// ������ �����
	Ball p_EmptyHole;

	// ����� ��������� ����, ������� ���������� ����������
	void ChangeActiveBall(wchar_t direction, wchar_t type, int &active_position, int min, int max);
	// ����������� ��������� ����
	void MoveBall(int active_position);
};

