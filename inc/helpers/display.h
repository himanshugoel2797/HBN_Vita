#ifndef PSV2CTRL_DISP_H
#define PSV2CTRL_DISP_H

#include <stdint.h>
#include <vita2d.h>

#define TEXT_SIZE 1.0f
#define TEXT_HEIGHT 50

typedef vita2d_texture Texture;

void
InitializeDisplay(void);

void
WriteString(int x, int y, uint32_t fg, float sz, const char *str, ...);

void
SetClearColor(uint32_t color);

void
ClearScreen(void);

void
StartDrawing(void);

void
Flip(void);

Texture*
LoadTexturePNG(const char *file);

void
FreeTexture(Texture *tex);

void
DrawTexture(const Texture *tex, int x, int y);

#endif