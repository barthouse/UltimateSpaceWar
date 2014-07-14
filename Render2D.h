#pragma once

#include <Windows.h>

#include "Vector2D.h"

typedef struct 
{
	Float2D m_position;
    Float2D m_texture;
} Vertex2D;

typedef struct _Sprite
{
    Int2D   m_tile;
    Float2D m_position;
    float   m_scale;
    float   m_rotation;
} Sprite;

bool Render2D_InitDevice(HWND hwnd);
void Render2D_CleanupDevice(void);

void Render2D_DrawSprite(Sprite * sprite);

void Render2D_DrawVertices(Vertex2D * inVertices, int inCount);
void Render2D_Clear(void);
void Render2D_Present(void);

