#include <psp2/kernel/threadmgr.h>

#include "scenes/entry_scene.h"
#include "scenes/gekihen_scene.h"
#include "helpers/display.h"

#define CLASS_NAME GekihenScene

#define SPLASH_TIME 4000

static Texture *tex = NULL;
static uint64_t start_time = 0;

INITIALIZE {
    tex = LoadTexturePNG("app0:data/gekihen_splash.png");
}

RENDER {
    StartDrawing();
    ClearScreen();
    DrawTexture(tex, 0, 0);
    Flip();
}

UPDATE {

}

DESTROY {

}

ENTER {
    start_time = sceKernelGetSystemTimeWide();
}

EXIT {
    
}
CONSTRUCTOR

Scene*
GekihenScene_Construct(void) {
    return NEW(GekihenScene) ("GekihenScene");
}