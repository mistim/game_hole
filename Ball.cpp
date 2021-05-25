#include "Ball.h"

Ball::Ball(int position, wchar_t color, wchar_t type)
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

void Ball::SetColor(wchar_t color)
{
	p_Color = color;
}

void Ball::Draw(Canvas& canvas)
{
	canvas.SetChar(p_Position, 1, p_Color);
	//wchar_t c;
	/*switch (p_Position)
	{
	case 2:
		c = p_Type == 'W' ? 0x2461 : 0x2777;
		break;
	case 4:
		c = p_Type == 'W' ? 0x2463 : 0x2779;
		break;
	case 6:
		c = p_Type == 'W' ? 0x2465 : 0x277B;
		break;
	case 8:
		c = p_Type == 'W' ? 0x2467 : 0x277D;
		break;
	case 10:
		c = p_Type == 'W' ? 0x2469 : 0x277F;
		break;
	case 12:
		c = p_Type == 'W' ? 0x246B : 0x24EC;
		break;
	case 14:
		c = p_Type == 'W' ? 0x246D : 0x24EE;
		break;
	case 16:
		c = p_Type == 'W' ? 0x246F : 0x24F0;
		break;
	case 18:
		c = p_Type == 'W' ? 0x2471 : 0x24F2;
		break;
	}
	canvas.SetChar(p_Position, 1, c);*/
}

void Ball::Move()
{
	// если следу€ща€ за шаром лунка пуста - переместить шар в нее

	// иначе переместить в следующую свободную лунку за ближайшим шаром
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

wchar_t Ball::GetType()
{
	return p_Type;
}
