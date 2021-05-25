#include "GameHoles.h"

GameHoles::GameHoles() : p_EmptyHole(0, Ball::ColorBlank, Ball::Hole)
{
	// ���������� �����
	p_NumHoles = 9;
	// ������� ����
	int width = p_NumHoles * 2 + 3;
	//������ ����
	int height = 3;

	p_NumBalls = (p_NumHoles - 1) / 2;
	p_StartPosEmptyHole = p_NumBalls * 2 + 2;

	// ��������� �������� ����
	p_GameField.Resize(width, height);
	p_Canvas.Resize(width, height);

	p_Reset();
}

void GameHoles::OnKeyPressed(int btnCode)
{
	// ������� ������� ��������� ����, ������������ ������ ��� ���������� ��������� �������
	int& active_position = p_ActivePlayer == Ball::White ? p_ActiveWhiteBall : p_ActiveBlackBall;

	// �������� ��������������
	switch (btnCode)
	{
	case 75: // ������� ������� ����� - ����� ��������� ����
		p_ChangeActiveBall('l', active_position);
		break;
	case 77: // ������� ������� ������ - ����� ��������� ����
		p_ChangeActiveBall('r', active_position);
		break;
	case 32: // ������� ������� - ����������� ��������� ���� �� ����� ������ �����
		p_MoveBall(active_position);
		break;
	case 13: // ������� Enter - ���������� ����
		p_Reset();
	}
}

void GameHoles::Update(double dt)
{
	// �������� �������� ����
	p_Canvas.Clear();

	// ��������� ����
	p_GameField.Draw(p_Canvas);

	// ���������� �����
	for (Ball& ball : p_Balls) {
		// ������� ����� ����� ���� �� �������
		if (p_ActivePlayer == Ball::White && ball.GetPosition() == p_ActiveWhiteBall) ball.BlinkActive(dt);
		// ������� ������ ����� ���� �� �������
		if (p_ActivePlayer == Ball::Black && ball.GetPosition() == p_ActiveBlackBall) ball.BlinkActive(dt);
		// ���������� ����� �� ������� ����
		ball.Draw(p_Canvas);
	}

	// ��������� ������ �����
	p_EmptyHole.Draw(p_Canvas);

	if (p_ThereIsWinner())
	{
		if (p_Winner == Ball::White)
			p_Canvas.SetText(1, 2, "Winner is Player 1!");

		if (p_Winner == Ball::Black)
			p_Canvas.SetText(1, 2, "Winner is Player 2!");
	}
	else
	{
		if (p_ActivePlayer == Ball::White)
			p_Canvas.SetText(7, 2, "Player 1");

		if (p_ActivePlayer == Ball::Black)
			p_Canvas.SetText(7, 2, "Player 2");
	}

	// ��������� ����� � �������
	p_Canvas.Render();
}

bool GameHoles::End()
{
	return false;
}

void GameHoles::p_ChangeActiveBall(wchar_t direction, int& active_position)
{
	// ��� �������� ��� ������ �������� ���
	if (p_ThereIsWinner()) return;

	int prev_position_wite = 0;
	int prev_position_black = 0;

	// ����� ������ ��������� ����, � �������� ����� ����������� ����
	for (Ball& ball : p_Balls)
		if (ball.GetPosition() == active_position) ball.RestoreColor();

	// ������� �����
	if (direction == 'r')
	{
		// ���������� ��� ����
		for (Ball ball : p_Balls)
		{
			// ���������� ��������, ����������� ����������� ���� �����
			bool access_position = p_ActivePlayer == Ball::White
				// ��� ����� - �������� ����� ������ �� ������� ��������� ����� �� �����
				? ball.GetPosition() < p_EmptyHole.GetPosition()
				// ��� ������ - �������� ����� ������ �� ������� ��������� ������ �� �����
				: ball.GetPosition() > p_EmptyHole.GetPosition();
			
			// ����� ���� ������ �� ���������
			// ��� ���������� ���������� ������� �� ����� ���� ����� ���������, ���������� ����� ������� 
			if (ball.GetType() == p_ActivePlayer && ball.GetPosition() > active_position && access_position) {
				active_position = ball.GetPosition();
				break;
			}
		}
	}
	// ������� ������
	else {
		for (Ball ball : p_Balls)
		{
			// ����� ���� ����� �� ���������
			// ��� ���������� �������� ��������� ����, �������� ���������� ��� ������� �����
			if (ball.GetType() == p_ActivePlayer && ball.GetPosition() == active_position) {
				if (ball.GetType() == Ball::White)
				{
					active_position = prev_position_wite < p_MinPosition ? ball.GetPosition() : prev_position_wite;
					break;
				}

				if (ball.GetType() == Ball::Black)
				{
					active_position = prev_position_black > 0 ? prev_position_black : ball.GetPosition();
					break;
				}
			}

			// ���������� ���������� ������� ������ ����
			if (ball.GetType() == Ball::White && ball.GetPosition() < p_EmptyHole.GetPosition())
				prev_position_wite = ball.GetPosition();

			// ���������� ���������� ������� ������� ����
			if (ball.GetType() == Ball::Black && ball.GetPosition() > p_EmptyHole.GetPosition())
				prev_position_black = ball.GetPosition();
		}
	}

	// ������������� �������� ����� �� �������
	p_ReSort();
}

void GameHoles::p_MoveBall(int active_position)
{
	// ��� �������� ��� ������ ���������� ���
	if (p_ThereIsWinner()) return;

	// ���������� ��� ����
	for (Ball& ball : p_Balls)
	{
		// ��� ���������� ��������� ����...
		if (ball.GetPosition() == active_position)
		{
			int position = ball.GetPosition();

			// ...���������� ������ ������� ��������� ���� � ������ ������
			ball.SetPosition(p_EmptyHole.GetPosition());
			ball.RestoreColor();
			p_EmptyHole.SetPosition(position);

			// ����� ������, ���� ������ �����, ������ ����� ������ ������ � ��������
			p_ActivePlayer = p_ActivePlayer == Ball::White ? Ball::Black : Ball::White;

			// ����� �� ��������
			break;
		}
	}

	// ������������� �������� ����� �� �������
	p_ReSort();

	p_ActiveWhiteBall = 0;
	p_ActiveBlackBall = 0;
		
	// ����� ���������� ���� ��� ����� ��������� ����
	// �� ��������� ������ �������� ���, ������� ��������� ����� �����
	// ��� ����� - ����� �� �����, ��� ������ - ������
	for (Ball ball : p_Balls)
	{
		// �������� ������� ��������� ����, ������������ ��������������� ����� ������ ������
		if (ball.GetPosition() < p_EmptyHole.GetPosition() && ball.GetType() == Ball::White)
			p_ActiveWhiteBall = ball.GetPosition();

		// ������� ��������� ������� ����, ������������ ��������������� ����� ������ �����
		if (ball.GetPosition() > p_EmptyHole.GetPosition() && ball.GetType() == Ball::Black && p_ActiveBlackBall == 0)
			p_ActiveBlackBall = ball.GetPosition();	
	}

	// ���� �� ���������� ���������� ������� ��� ������ ������ ���� ��� �����������, 
	// ������ ��� ��� ���������� �� ������ �����, 
	// ��� ���� ������� (��������� ��� ���� ���������� �� ��������������� �������), 
	// ���� ������� ���� (���� ��� ���� ���������� �� �� ��������������� �������, ��� ���� ���� ������� �����)
	if (p_ActiveWhiteBall == 0 && p_ActiveBlackBall > 0)
		p_ActivePlayer = Ball::Black;

	// ���� � ��� ������ �����
	if (p_ActiveWhiteBall > 0 && p_ActiveBlackBall == 0)
		p_ActivePlayer = Ball::White;
}

void GameHoles::p_ReSort()
{
	// ���������� ����� �� m_Position �� �����������
	sort(p_Balls.begin(), p_Balls.end(), [](Ball& lball, Ball& rball)
	{
		return lball.GetPosition() < rball.GetPosition();
	});
}

bool GameHoles::p_ThereIsWinner()
{
	// ����� ������� ����������� ���������� ����� ������ ����� � ������� ���� �������� ���� 
	size_t i_white = 0;
	size_t i_black = 0;

	for (Ball ball : p_Balls)
	{
		// ���� ����� ������� � ������ ������� ��������� ��� ������ ����, �������� ���
		if (ball.GetPosition() < p_StartPosEmptyHole && ball.GetType() == Ball::Black) 
			i_black++;

		// ��� ������ ������� � ��������� ������� ��������� ��� ����� ����, �������� ���
		if (ball.GetPosition() > p_StartPosEmptyHole && ball.GetType() == Ball::White) 
			i_white++;
	}

	// ���������� ��� �������
	if (i_white == p_NumBalls)
		p_Winner = Ball::White;

	if (i_black == p_NumBalls)
		p_Winner = Ball::Black;

	return i_white == p_NumBalls || i_black == p_NumBalls;
}

void GameHoles::p_Reset()
{
	p_Balls.clear();
	p_Winner = '\0';

	// �� ��������� ��� �������� ����� ����
	p_ActivePlayer = Ball::White;
	// �� ��������� �������� ����� ��� ���, ������� ��������� ����� ������ ������
	p_ActiveWhiteBall = p_NumBalls * 2;

	// ���������� ����� �����
	for (int i = 1; i <= p_NumBalls * 2; i++) {
		// ����� 1 ������ ��������� ������ ����
		if (i % 2 == 0) {
			// ����������� ������� ����
			if (!p_MinPosition) p_MinPosition = i;
			// �������� ������� ������ ����
			Ball ball(i, Ball::ColorWhite, Ball::White);
			// ���������� ��� � ��������
			p_Balls.push_back(ball);
		}
	}

	// ��������� ������� ��� ������ �����
	p_EmptyHole.SetPosition(p_StartPosEmptyHole);

	// ���������� ������ �����
	for (int i = 1 + p_StartPosEmptyHole; i <= (p_NumBalls * 2) + p_StartPosEmptyHole; i++) {
		// ����� 1 ������ ��������� ������ ����
		if (i % 2 == 0) {
			// �� ��������� �������� ����� ������ ���, ������� ��������� ����� ����� ������ �����
			if (!p_ActiveBlackBall) p_ActiveBlackBall = i;
			// ��� ������� ������������ ������� ��� ����
			p_MaxPosition = i;
			// �������� ������� ������� ����
			Ball ball(i, Ball::ColorBlack, Ball::Black);
			// ���������� ��� � ��������
			p_Balls.push_back(ball);
		}
	}
}
