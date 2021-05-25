#pragma once

#include <vector>
#include <algorithm>
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
	// ������� ������ �����
	int p_StartPosEmptyHole;
	// �������� ����� (����� ��� ������ ����)
	char p_ActivePlayer;
	// ������� ��������� ������ ����
	int p_ActiveWhiteBall = 0;
	// ������� ��������� ������� ����
	int p_ActiveBlackBall = 0;
	// ����������� ������� ����
	int p_MinPosition = 0;
	// ������������ ������� ����
	int p_MaxPosition = 0;
	// ����������
	char p_Winner;
	// ������� ����
	GameField p_GameField;
	Canvas p_Canvas;
	// �������� ����� (����� �������� ������ Ball)
	std::vector<Ball> p_Balls = {};
	// ������ �����
	Ball p_EmptyHole;

	// ����� ����, ������� ���������� ����������
	void p_ChangeActiveBall(wchar_t direction, int &active_position);
	// ����������� ���������� ����
	void p_MoveBall(int active_position);
	// �������������� ������� �����, ���������� ��� ����, ��� �� ��������� ������ �������� ���
	void p_ReSort();
	// ��������, ���� �� ����������
	bool p_ThereIsWinner();
	// ���������� ��������� ����������
	void p_Reset();
};

