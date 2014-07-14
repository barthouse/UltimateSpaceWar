#pragma once

#include "Render2D.h"

typedef enum {
	eKey_space,
	eKey_w,
	eKey_a,
	eKey_s,
	eKey_d,
	eKey_x,
	eKey_count
} Key;

void DrawVertices(Vertex2D * inVertices, int inCount);
void Clear();
void Present();
void GetKeys(bool * outKeyPressed);
void DrawSprite(Sprite * sprite);
