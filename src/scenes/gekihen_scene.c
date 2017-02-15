#include <psp2/kernel/threadmgr.h>

#include "scenes/entry_scene.h"
#include "scenes/gekihen_scene.h"
#include "helpers/display.h"
#include "helpers/colors.h"

#define CLASS_NAME GekihenScene

#define SPLASH_TIME 3000000
#define SPLASH_FADE_TIME 100000

static Texture *tex = NULL;
static uint64_t start_time = 0;
static uint64_t cur_time = 0;

INITIALIZE {
    tex = LoadTexturePNG("app0:data/gekihen_splash.png");
}

RENDER {
    StartDrawing();
    SetClearColor(BG_COLOR);
    ClearScreen();
    
    int alpha = 255;
    
    if(cur_time - start_time <= SPLASH_FADE_TIME)
        alpha = (cur_time - start_time)/SPLASH_FADE_TIME;
    else if(cur_time - start_time >= SPLASH_TIME - SPLASH_FADE_TIME)
        alpha = (SPLASH_FADE_TIME - (SPLASH_TIME - (cur_time - start_time)))/SPLASH_FADE_TIME;
    
    DrawTexture(tex, 0, 0, RGBA8(alpha, alpha, alpha, 255));
    Flip();
}

UPDATE {
    cur_time = sceKernelGetSystemTimeWide();

    if (cur_time - start_time > SPLASH_TIME)
    {
        Scene *new_scene = EntryScene_Construct();
        ReplaceCurrentScene(man, new_scene);
    }
}

DESTROY {

}

ENTER {
    start_time = sceKernelGetSystemTimeWide();
    cur_time = start_time;
}

EXIT {
    FreeTexture(tex);
}
CONSTRUCTOR

Scene*
GekihenScene_Construct(void) {
    return NEW(GekihenScene) ("GekihenScene");
}