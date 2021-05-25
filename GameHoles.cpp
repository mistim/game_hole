#include "GameHoles.h"

GameHoles::GameHoles() : p_EmptyHole(0, Ball::ColorBlank, Ball::Hole)
{
	// количество лунок
	p_NumHoles = 9;
	// ширрина поля
	int width = p_NumHoles * 2 + 3;
	//высота поля
	int height = 3;
	// с какой лунки начинаются черные шары
	int start_black_ball = 0;
	
	p_NumBalls = (p_NumHoles - 1) / 2;
	start_black_ball = p_NumBalls * 2 + 2;
	// по умолчанию ход начинают белые шары
	p_ActivePalyer = Ball::White;
	// по умолчанию активный белый шар тот, который нашодится перед пустой лункой
	p_ActiveWhiteBall = p_NumBalls * 2;

	// добавление белых шаров
	for (int i = 1; i <= p_NumBalls * 2; i++) {
		// через 1 символ добавляем символ шара
		if (i % 2 == 0) {
			// минимальная позиция шара
			if (!p_MinPosition) p_MinPosition = i;
			// создание объекта белого шара
			Ball ball(i, Ball::ColorWhite, Ball::White);
			// добавление его в перечень
			p_Balls.push_back(ball);
		}
	}

	// установка позиции для пустой лунки
	p_EmptyHole.SetPosition(start_black_ball);

	// добавление черных шаров
	for (int i = 1 + start_black_ball; i <= (p_NumBalls * 2) + start_black_ball; i++) {
		// через 1 символ добавляем символ шара
		if (i % 2 == 0) {
			// по умолчанию активным будет черный шар, который находится сразу после пустой лунки
			if (!p_ActiveBlackBall) p_ActiveBlackBall = i;
			// так получим максимальную позицию для шара
			p_MaxPositionBlack = i;
			// создание объекта черного шара
			Ball ball(i, Ball::ColorBlack, Ball::Black);
			// добавление его в перечень
			p_Balls.push_back(ball);
		}
	}

	// установка игрового поля
	p_GameField.Resize(width, height);
	p_Canvas.Resize(width, height);
}

void GameHoles::OnKeyPressed(int btnCode)
{
	// текущая позиция активного шара, используется ссылка для сохранения изминения позиции
	int& active_position = p_ActivePalyer == Ball::White ? p_ActiveWhiteBall : p_ActiveBlackBall;
	wchar_t type = p_ActivePalyer == Ball::White ? Ball::White : Ball::Black;

	// перечень обрабатываемых
	switch (btnCode)
	{
	case 75: // нажатие стрелки влево - смена активного шара
		ChangeActiveBall('l', type, active_position);
		break;
	case 77: // нажатие стрелки вправо - смена активного шара
		ChangeActiveBall('r', type, active_position);
		break;
	case 32: // нажатие пробела - перемещение активного шара на место пустой лунки
		MoveBall(active_position);
		break;
	}
}

void GameHoles::Update(double dt)
{
	// очищение игрового поля
	p_Canvas.Clear();

	// отрисовка поля
	p_GameField.Draw(p_Canvas);

	// открисовка шаров
	for (Ball& ball : p_Balls) {
		// мигание белым шаром если он активен
		if (p_ActivePalyer == Ball::White && ball.GetPosition() == p_ActiveWhiteBall) ball.BlinkActive(dt);
		// мигание черным шаром если он активен
		if (p_ActivePalyer == Ball::Black && ball.GetPosition() == p_ActiveBlackBall) ball.BlinkActive(dt);
		// добавление шаров на игровое поле
		ball.Draw(p_Canvas);
	}

	// отрисовка пустой лунки
	p_EmptyHole.Draw(p_Canvas);

	// debug
	p_Canvas.SetChar(1, 2, p_ActivePalyer);
	p_Canvas.SetChar(2, 2, 0x003A);
	p_Canvas.SetChar(3, 2, Ball::ColorEmpty);
	p_Canvas.SetNumber(4, 2, p_ActivePalyer == Ball::White ? p_ActiveWhiteBall : p_ActiveBlackBall);
	p_Canvas.SetNumber(8, 2, d_IterPosition);
	p_Canvas.SetNumber(12, 2, d_PrevPosition);

	// рендеринг всего в консоль
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

	// перед сменой активного шара, у текущего нужно востановить цвет
	for (Ball & ball : p_Balls)
		if (ball.GetPosition() == active_position) ball.RestoreColor();

	// стрелка влево
	if (direction == 'r')
	{
		// перебираем все шары
		for (Ball& ball : p_Balls)
		{
			// допустивый диапазон, исключающий перемещение шара назад
			bool access_position = type == Ball::White
				// для белых - вибирать можно только те которые находятся слева от лунки
				? ball.GetPosition() < p_EmptyHole.GetPosition()
				// для черных - вибирать можно только те которые находятся справа от лунки
				: ball.GetPosition() > p_EmptyHole.GetPosition();
			
			// выбор шара справа от активного
			// при достижении следующего нужного по цвету шара после активного, производим смену позиции 
			if (ball.GetType() == type && ball.GetPosition() > active_position && access_position) {
				active_position = ball.GetPosition();
				break;
			}
		}
	}
	// стрелка вправо
	else {
		//for (std::vector<Ball>::iterator itr = p_Balls.rbegin(); itr != p_Balls.rend(); ++itr)
		//for (std::vector<Ball>::iterator itr = p_Balls.end(); itr-- != p_Balls.begin(); )
		//for (std::vector<Ball>::iterator itr = p_Balls.begin(); itr != p_Balls.end(); ++itr)
		for (Ball& ball : p_Balls)
		{
			// выбор шара слева от активного
			// при достищении текущего активного шара, выбираем предыдущий шар нужного цвета
			if (ball.GetType() == type && ball.GetPosition() == active_position) {
				if (ball.GetType() == Ball::White)
					active_position = prev_position_wite < p_MinPosition ? ball.GetPosition() : prev_position_wite;

				if (ball.GetType() == Ball::Black)
					active_position = prev_position_black > 0 ? prev_position_black : ball.GetPosition();

				break;
			}

			// запоминаем предыдущую позицию белого шара
			if (ball.GetType() == Ball::White && ball.GetPosition() < p_EmptyHole.GetPosition())
				prev_position_wite = ball.GetPosition();

			// запоминаем предыдущую позицию черного шара
			if (ball.GetType() == Ball::Black && ball.GetPosition() > p_EmptyHole.GetPosition())
				prev_position_black = ball.GetPosition();
			
		}
	}
}

void GameHoles::MoveBall(int active_position)
{
	// перебираем все шары
	for (Ball& ball : p_Balls)
	{
		// при достижении активного шара...
		if (ball.GetPosition() == active_position)
		{
			int position = ball.GetPosition();

			// ...произвадим замену позиций акьивного шара с пустой лункой
			ball.SetPosition(p_EmptyHole.GetPosition());
			ball.RestoreColor();
			p_EmptyHole.SetPosition(position);
			position = 0;

			// смена игрока, если ходили белые, теперь будут ходить черные и наоборот
			p_ActivePalyer = p_ActivePalyer == Ball::White ? Ball::Black : Ball::White;

			// выход из перебора
			break;
		}
	}

	int prev_position = 0;
		
	// снова перебираем шары для смены активного шара
	// по умолчанию делаем активным шар, который находится возле лунки
	// для белых - слева от лунки, для черных - справа
	for (Ball ball : p_Balls)
	{
		// при достижении позиции после пустой лунки...
		if (ball.GetPosition() > p_EmptyHole.GetPosition())
		{
			d_IterPosition = ball.GetPosition();
			// ...для белых шаров выбираем предыдущий шар который был непосредственно перед лункой
			if (p_ActivePalyer == Ball::White)
			{
				p_ActiveWhiteBall = prev_position;
				prev_position = 0;
				break;
			}

			// ...для черных выбираем шар, который находится сразу после лунки
			if (p_ActivePalyer == Ball::Black && ball.GetType() == Ball::Black)
			{
				p_ActiveBlackBall = ball.GetPosition();
				break;
			}
		}
		
		// запоминаем предыдущую позицию белого шара
		if (ball.GetType() == Ball::White) {
			prev_position = ball.GetPosition();
			d_PrevPosition = prev_position;
		}		
	}
}
