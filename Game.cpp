#include "Game.h"

#include <iostream>
#include <conio.h>

Game::Game()
{
	p_ConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
}

void Game::Run()
{
	Time last = Clock::now();
	while (!End())
	{
		//FPS fps;

		if (_kbhit())
		{
			OnKeyPressed(_getch());
			if (!FlushConsoleInputBuffer(p_ConsoleIn))
				std::cout << "FlushConsoleInputBuffer failed with error " << GetLastError() << std::endl;
		}

		Update(static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - last).count()));
		last = Clock::now();

		Sleep(1);
	}
}