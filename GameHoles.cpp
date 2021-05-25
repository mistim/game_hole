#include "GameHoles.h"

GameHoles::GameHoles() : p_EmptyHole(0, Ball::ColorBlank, Ball::Hole)
{
	// количество лунок
	p_NumHoles = 9;
	// ширрина поля
	int width = p_NumHoles * 2 + 3;
	//высота поля
	int height = 3;

	p_NumBalls = (p_NumHoles - 1) / 2;
	p_StartPosEmptyHole = p_NumBalls * 2 + 2;

	// установка игрового поля
	p_GameField.Resize(width, height);
	p_Canvas.Resize(width, height);

	p_Reset();
}

void GameHoles::OnKeyPressed(int btnCode)
{
	// текущая позиция активного шара, используется ссылка для сохранения изминения позиции
	int& active_position = p_ActivePlayer == Ball::White ? p_ActiveWhiteBall : p_ActiveBlackBall;

	// перечень обрабатываемых
	switch (btnCode)
	{
	case 75: // нажатие стрелки влево - смена активного шара
		p_ChangeActiveBall('l', active_position);
		break;
	case 77: // нажатие стрелки вправо - смена активного шара
		p_ChangeActiveBall('r', active_position);
		break;
	case 32: // нажатие пробела - перемещение активного шара на место пустой лунки
		p_MoveBall(active_position);
		break;
	case 13: // нажатие Enter - перезапуск игры
		p_Reset();
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
		if (p_ActivePlayer == Ball::White && ball.GetPosition() == p_ActiveWhiteBall) ball.BlinkActive(dt);
		// мигание черным шаром если он активен
		if (p_ActivePlayer == Ball::Black && ball.GetPosition() == p_ActiveBlackBall) ball.BlinkActive(dt);
		// добавление шаров на игровое поле
		ball.Draw(p_Canvas);
	}

	// отрисовка пустой лунки
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

	// рендеринг всего в консоль
	p_Canvas.Render();
}

bool GameHoles::End()
{
	return false;
}

void GameHoles::p_ChangeActiveBall(wchar_t direction, int& active_position)
{
	// при выиграше нет смысла вибирать шар
	if (p_ThereIsWinner()) return;

	int prev_position_wite = 0;
	int prev_position_black = 0;

	// перед сменой активного шара, у текущего нужно востановить цвет
	for (Ball& ball : p_Balls)
		if (ball.GetPosition() == active_position) ball.RestoreColor();

	// стрелка влево
	if (direction == 'r')
	{
		// перебираем все шары
		for (Ball ball : p_Balls)
		{
			// допустивый диапазон, исключающий перемещение шара назад
			bool access_position = p_ActivePlayer == Ball::White
				// для белых - вибирать можно только те которые находятся слева от лунки
				? ball.GetPosition() < p_EmptyHole.GetPosition()
				// для черных - вибирать можно только те которые находятся справа от лунки
				: ball.GetPosition() > p_EmptyHole.GetPosition();
			
			// выбор шара справа от активного
			// при достижении следующего нужного по цвету шара после активного, производим смену позиции 
			if (ball.GetType() == p_ActivePlayer && ball.GetPosition() > active_position && access_position) {
				active_position = ball.GetPosition();
				break;
			}
		}
	}
	// стрелка вправо
	else {
		for (Ball ball : p_Balls)
		{
			// выбор шара слева от активного
			// при достищении текущего активного шара, выбираем предыдущий шар нужного цвета
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

			// запоминаем предыдущую позицию белого шара
			if (ball.GetType() == Ball::White && ball.GetPosition() < p_EmptyHole.GetPosition())
				prev_position_wite = ball.GetPosition();

			// запоминаем предыдущую позицию черного шара
			if (ball.GetType() == Ball::Black && ball.GetPosition() > p_EmptyHole.GetPosition())
				prev_position_black = ball.GetPosition();
		}
	}

	// пересортируем перечень шаров по позиции
	p_ReSort();
}

void GameHoles::p_MoveBall(int active_position)
{
	// при выиграше нет смысла перемещать шар
	if (p_ThereIsWinner()) return;

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

			// смена игрока, если ходили белые, теперь будут ходить черные и наоборот
			p_ActivePlayer = p_ActivePlayer == Ball::White ? Ball::Black : Ball::White;

			// выход из перебора
			break;
		}
	}

	// пересортируем перечень шаров по позиции
	p_ReSort();

	p_ActiveWhiteBall = 0;
	p_ActiveBlackBall = 0;
		
	// снова перебираем шары для смены активного шара
	// по умолчанию делаем активным шар, который находится возле лунки
	// для белых - слева от лунки, для черных - справа
	for (Ball ball : p_Balls)
	{
		// получаем позицию активного шара, находящегося непосредственно перед пустой лункой
		if (ball.GetPosition() < p_EmptyHole.GetPosition() && ball.GetType() == Ball::White)
			p_ActiveWhiteBall = ball.GetPosition();

		// позиция активного черного шара, находящегося непосредственно после пустой лунки
		if (ball.GetPosition() > p_EmptyHole.GetPosition() && ball.GetType() == Ball::Black && p_ActiveBlackBall == 0)
			p_ActiveBlackBall = ball.GetPosition();	
	}

	// если не получилось установить позицию для выбора белого шара для перемещения, 
	// значит они все перемещены за пустую лунку, 
	// тут либо выиграш (полностью все шары перемещены на противоположную сторону), 
	// либо пропуск хода (если все шары перемещены не на противоположную сторону, еще есть шары другого цвета)
	if (p_ActiveWhiteBall == 0 && p_ActiveBlackBall > 0)
		p_ActivePlayer = Ball::Black;

	// тоже и для черных шаров
	if (p_ActiveWhiteBall > 0 && p_ActiveBlackBall == 0)
		p_ActivePlayer = Ball::White;
}

void GameHoles::p_ReSort()
{
	// сортировка шаров по m_Position по возростанию
	sort(p_Balls.begin(), p_Balls.end(), [](Ball& lball, Ball& rball)
	{
		return lball.GetPosition() < rball.GetPosition();
	});
}

bool GameHoles::p_ThereIsWinner()
{
	// здесь накопим непрерывное количество шаров одного цвета у каждого каря игрового поля 
	size_t i_white = 0;
	size_t i_black = 0;

	for (Ball ball : p_Balls)
	{
		// если слева начиная с первой позиции находятся все черные шары, победили они
		if (ball.GetPosition() < p_StartPosEmptyHole && ball.GetType() == Ball::Black) 
			i_black++;

		// ели справа начиная с последней позиции находятся все белые шары, победили они
		if (ball.GetPosition() > p_StartPosEmptyHole && ball.GetType() == Ball::White) 
			i_white++;
	}

	// запоминаем кто подедил
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

	// по умолчанию ход начинают белые шары
	p_ActivePlayer = Ball::White;
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
	p_EmptyHole.SetPosition(p_StartPosEmptyHole);

	// добавление черных шаров
	for (int i = 1 + p_StartPosEmptyHole; i <= (p_NumBalls * 2) + p_StartPosEmptyHole; i++) {
		// через 1 символ добавляем символ шара
		if (i % 2 == 0) {
			// по умолчанию активным будет черный шар, который находится сразу после пустой лунки
			if (!p_ActiveBlackBall) p_ActiveBlackBall = i;
			// так получим максимальную позицию для шара
			p_MaxPosition = i;
			// создание объекта черного шара
			Ball ball(i, Ball::ColorBlack, Ball::Black);
			// добавление его в перечень
			p_Balls.push_back(ball);
		}
	}
}
