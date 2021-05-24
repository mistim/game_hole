#pragma once

#include <Windows.h>
#include <chrono>
#include "Canvas.h"

// базовый клас
class Game
{
	using Clock = std::chrono::system_clock;
	using Time = std::chrono::time_point<Clock>;

public:
	Game();

	virtual void OnKeyPressed(int btnCode) = 0;
	virtual void Update(double dt) = 0;
	virtual bool End() = 0;

	void Run();

public:
	HANDLE p_ConsoleIn;
};

