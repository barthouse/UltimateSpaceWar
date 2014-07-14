#include <assert.h>
#include <stdio.h>

#include <Windows.h>

#include "Framework.h"
#include "Render2D.h"

void GetKeys(bool * outKeyPressed)
{
	outKeyPressed[eKey_space] = ::GetAsyncKeyState(' ') != 0;
	outKeyPressed[eKey_w] = ::GetAsyncKeyState('W') != 0;
	outKeyPressed[eKey_a] = ::GetAsyncKeyState('A') != 0;
	outKeyPressed[eKey_s] = ::GetAsyncKeyState('S') != 0;
	outKeyPressed[eKey_d] = ::GetAsyncKeyState('D') != 0;
	outKeyPressed[eKey_x] = ::GetAsyncKeyState('X') != 0;
	
}

void DrawVertices(Vertex2D * inVertices, int inCount)
{
	Render2D_DrawVertices(inVertices, inCount);
}

void Clear()
{
	Render2D_Clear();
}

void Present()
{
	Render2D_Present();
}

void DrawSprite(Sprite * sprite)
{
    Render2D_DrawSprite(sprite);
}