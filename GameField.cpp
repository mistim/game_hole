#include "GameField.h"

void GameField::Resize(int width, int height)
{
	p_Width = width;
	p_Height = height;
}

void GameField::Draw(Canvas& canvas)
{
	// горизонталные края
	for (int i = 1; i < p_Width - 1; i++) {
		canvas.SetChar(i, 0, 0x2500);
		canvas.SetChar(i, p_Height - 1, 0x2500);
	}

	// вертикальные края
	for (int i = 1; i < p_Height - 1; i++) {
		canvas.SetChar(0, i, 0x2502);
		canvas.SetChar(p_Width - 1, i, 0x2502);
	}

	// углы 
	canvas.SetChar(0, 0, 0x250C);
	canvas.SetChar(p_Width - 1, 0, 0x2510);
	canvas.SetChar(0, p_Height - 1, 0x2514);
	canvas.SetChar(p_Width - 1, p_Height - 1, 0x2518);
}
