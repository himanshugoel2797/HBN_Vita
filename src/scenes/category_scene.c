#include <psp2/kernel/threadmgr.h>

#include "scenes/com_init_thread.h"

#include "helpers/display.h"
#include "helpers/net.h"

//Show the splash screen and start downloading store data in the background
#define CLASS_NAME CategoryScene

static Texture *tex = NULL;
static uint64_t start_time = 0;

SceUID GetComTID(void);

INITIALIZE
{
    tex = LoadTexturePNG("app0:data/hbn.png");
}

RENDER
{
    StartDrawing();
    ClearScreen();
    DrawTexture(tex, 0, 0, 0xffffffff);
    Flip();
}

UPDATE
{

}

DESTROY
{
}

ENTER
{
    start_time = sceKernelGetSystemTimeWide();
}

EXIT
{
    FreeTexture(tex);
}
CONSTRUCTOR

Scene *
CategoryScene_Construct(const char *name)
{
    return NEW(CategoryScene)(name);
}