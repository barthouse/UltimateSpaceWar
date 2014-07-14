#pragma once

#include <Windows.h>

typedef struct 
{
	float m_x;
	float m_y;
	float m_u;
	float m_v;
} Vertex2D;

bool Render2D_InitDevice(HWND hwnd);
void Render2D_CleanupDevice(void);

void Render2D_DrawVertices(Vertex2D * inVertices, int inCount);
void Render2D_Clear(void);
void Render2D_Present(void);
