#include "GameHoles.h"

GameHoles::GameHoles() : p_EmptyHole(0, Ball::ColorBlank, Ball::Hole)
{
	// ���������� �����
	p_NumHoles = 9;
	// ������� ����
	int width = p_NumHoles * 2 + 3;
	//������ ����
	int height = 3;
	// � ����� ����� ���������� ������ ����
	int start_black_ball = 0;
	
	p_NumBalls = (p_NumHoles - 1) / 2;
	start_black_ball = p_NumBalls * 2 + 2;
	// �� ��������� ��� �������� ����� ����
	p_ActivePalyer = Ball::White;
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
	p_EmptyHole.SetPosition(start_black_ball);

	// ���������� ������ �����
	for (int i = 1 + start_black_ball; i <= (p_NumBalls * 2) + start_black_ball; i++) {
		// ����� 1 ������ ��������� ������ ����
		if (i % 2 == 0) {
			// �� ��������� �������� ����� ������ ���, ������� ��������� ����� ����� ������ �����
			if (!p_ActiveBlackBall) p_ActiveBlackBall = i;
			// ��� ������� ������������ ������� ��� ����
			p_MaxPositionBlack = i;
			// �������� ������� ������� ����
			Ball ball(i, Ball::ColorBlack, Ball::Black);
			// ���������� ��� � ��������
			p_Balls.push_back(ball);
		}
	}

	// ��������� �������� ����
	p_GameField.Resize(width, height);
	p_Canvas.Resize(width, height);
}

void GameHoles::OnKeyPressed(int btnCode)
{
	// ������� ������� ��������� ����, ������������ ������ ��� ���������� ��������� �������
	int& active_position = p_ActivePalyer == Ball::White ? p_ActiveWhiteBall : p_ActiveBlackBall;
	wchar_t type = p_ActivePalyer == Ball::White ? Ball::White : Ball::Black;

	// �������� ��������������
	switch (btnCode)
	{
	case 75: // ������� ������� ����� - ����� ��������� ����
		ChangeActiveBall('l', type, active_position);
		break;
	case 77: // ������� ������� ������ - ����� ��������� ����
		ChangeActiveBall('r', type, active_position);
		break;
	case 32: // ������� ������� - ����������� ��������� ���� �� ����� ������ �����
		MoveBall(active_position);
		break;
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
		if (p_ActivePalyer == Ball::White && ball.GetPosition() == p_ActiveWhiteBall) ball.BlinkActive(dt);
		// ������� ������ ����� ���� �� �������
		if (p_ActivePalyer == Ball::Black && ball.GetPosition() == p_ActiveBlackBall) ball.BlinkActive(dt);
		// ���������� ����� �� ������� ����
		ball.Draw(p_Canvas);
	}

	// ��������� ������ �����
	p_EmptyHole.Draw(p_Canvas);

	// debug
	p_Canvas.SetChar(1, 2, p_ActivePalyer);
	p_Canvas.SetChar(2, 2, 0x003A);
	p_Canvas.SetChar(3, 2, Ball::ColorEmpty);
	p_Canvas.SetNumber(4, 2, p_ActivePalyer == Ball::White ? p_ActiveWhiteBall : p_ActiveBlackBall);
	p_Canvas.SetNumber(8, 2, d_IterPosition);
	p_Canvas.SetNumber(12, 2, d_PrevPosition);

	// ��������� ����� � �������
	p_Canvas.Render();
}

bool GameHoles::End()
{
	return false;
}

void GameHoles::ChangeActiveBall(wchar_t direction, wchar_t type, int& active_position)
{
	int prev_position_wite = 0;
	int prev_position_black = 0;

	// ����� ������ ��������� ����, � �������� ����� ����������� ����
	for (Ball & ball : p_Balls)
		if (ball.GetPosition() == active_position) ball.RestoreColor();

	// ������� �����
	if (direction == 'r')
	{
		// ���������� ��� ����
		for (Ball& ball : p_Balls)
		{
			// ���������� ��������, ����������� ����������� ���� �����
			bool access_position = type == Ball::White
				// ��� ����� - �������� ����� ������ �� ������� ��������� ����� �� �����
				? ball.GetPosition() < p_EmptyHole.GetPosition()
				// ��� ������ - �������� ����� ������ �� ������� ��������� ������ �� �����
				: ball.GetPosition() > p_EmptyHole.GetPosition();
			
			// ����� ���� ������ �� ���������
			// ��� ���������� ���������� ������� �� ����� ���� ����� ���������, ���������� ����� ������� 
			if (ball.GetType() == type && ball.GetPosition() > active_position && access_position) {
				active_position = ball.GetPosition();
				break;
			}
		}
	}
	// ������� ������
	else {
		//for (std::vector<Ball>::iterator itr = p_Balls.rbegin(); itr != p_Balls.rend(); ++itr)
		//for (std::vector<Ball>::iterator itr = p_Balls.end(); itr-- != p_Balls.begin(); )
		//for (std::vector<Ball>::iterator itr = p_Balls.begin(); itr != p_Balls.end(); ++itr)
		for (Ball& ball : p_Balls)
		{
			// ����� ���� ����� �� ���������
			// ��� ���������� �������� ��������� ����, �������� ���������� ��� ������� �����
			if (ball.GetType() == type && ball.GetPosition() == active_position) {
				if (ball.GetType() == Ball::White)
					active_position = prev_position_wite < p_MinPosition ? ball.GetPosition() : prev_position_wite;

				if (ball.GetType() == Ball::Black)
					active_position = prev_position_black > 0 ? prev_position_black : ball.GetPosition();

				break;
			}

			// ���������� ���������� ������� ������ ����
			if (ball.GetType() == Ball::White && ball.GetPosition() < p_EmptyHole.GetPosition())
				prev_position_wite = ball.GetPosition();

			// ���������� ���������� ������� ������� ����
			if (ball.GetType() == Ball::Black && ball.GetPosition() > p_EmptyHole.GetPosition())
				prev_position_black = ball.GetPosition();
			
		}
	}
}

void GameHoles::MoveBall(int active_position)
{
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
			position = 0;

			// ����� ������, ���� ������ �����, ������ ����� ������ ������ � ��������
			p_ActivePalyer = p_ActivePalyer == Ball::White ? Ball::Black : Ball::White;

			// ����� �� ��������
			break;
		}
	}

	int prev_position = 0;
		
	// ����� ���������� ���� ��� ����� ��������� ����
	// �� ��������� ������ �������� ���, ������� ��������� ����� �����
	// ��� ����� - ����� �� �����, ��� ������ - ������
	for (Ball ball : p_Balls)
	{
		// ��� ���������� ������� ����� ������ �����...
		if (ball.GetPosition() > p_EmptyHole.GetPosition())
		{
			d_IterPosition = ball.GetPosition();
			// ...��� ����� ����� �������� ���������� ��� ������� ��� ��������������� ����� ������
			if (p_ActivePalyer == Ball::White)
			{
				p_ActiveWhiteBall = prev_position;
				prev_position = 0;
				break;
			}

			// ...��� ������ �������� ���, ������� ��������� ����� ����� �����
			if (p_ActivePalyer == Ball::Black && ball.GetType() == Ball::Black)
			{
				p_ActiveBlackBall = ball.GetPosition();
				break;
			}
		}
		
		// ���������� ���������� ������� ������ ����
		if (ball.GetType() == Ball::White) {
			prev_position = ball.GetPosition();
			d_PrevPosition = prev_position;
		}		
	}
}
