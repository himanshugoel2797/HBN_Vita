#include <psp2/kernel/threadmgr.h>

#include "scenes/entry_scene.h"
#include "scenes/gekihen_scene.h"

#include "helpers/display.h"
#include "helpers/net.h"

//Show the splash screen and start downloading store data in the background
#define CLASS_NAME EntryScene

//3 second splash screen
#define SPLASH_TIME 3000000

static Texture *tex = NULL;
static uint64_t start_time = 0;

INITIALIZE
{
    tex = LoadTexturePNG("app0:data/hbn.png");
}

RENDER
{
    StartDrawing();
    SetClearColor(0xffffffff);
    ClearScreen();
    DrawTexture(tex, 0, 0);
    Flip();
}

UPDATE
{
    uint64_t cur_time = sceKernelGetSystemTimeWide();

    if (cur_time - start_time > SPLASH_TIME)
    {
        Scene *new_scene = GekihenScene_Construct();
        ReplaceCurrentScene(man, new_scene);
    }
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
}
CONSTRUCTOR

Scene *
EntryScene_Construct(void)
{
    return NEW(EntryScene)("EntryScene");
}