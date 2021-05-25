#include "Ball.h"

Ball::Ball(int position, wchar_t color, char type)
{
	p_Position = position;
	p_Color = color;
	p_ColorRestore = color;
	p_Type = type;
}

void Ball::SetPosition(int position)
{
	p_Position = position;
}

void Ball::Draw(Canvas& canvas)
{
	canvas.SetChar(p_Position, 1, p_Color);
}

void Ball::BlinkActive(double dt)
{
	p_TimeFromLastUpdate += dt;

	// каждые N секунды происходит смена символа (цвета) активного шара
	if (p_TimeFromLastUpdate >= 250) {
		if (p_BlinkFlag)
			p_Color = ColorEmpty;
		else
			p_Color = p_ColorRestore;

		p_TimeFromLastUpdate = 0;
		p_BlinkFlag = !p_BlinkFlag;
	}
}

void Ball::RestoreColor()
{
	p_Color = p_ColorRestore;
}

int Ball::GetPosition()
{
	return p_Position;
}

char Ball::GetType()
{
	return p_Type;
}

void Ball::p_SetColor(wchar_t color)
{
	p_Color = color;
}
