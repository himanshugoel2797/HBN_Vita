#include <psp2/kernel/threadmgr.h>

#include "scenes/com_init_thread.h"
#include "scenes/category_scene.h"

#include "helpers/display.h"
#include "helpers/net.h"

//Show the splash screen and start downloading store data in the background
#define CLASS_NAME ComInitThreadScene

//3 second splash screen
#define SPLASH_TIME 3000000

static uint64_t start_time = 0;
static uint64_t cur_time = 0;

SceUID GetComTID(void);

INITIALIZE
{

}

RENDER
{
    int cntr = (cur_time - start_time) / 250000;
    cntr = cntr % 3 + 1;

    StartDrawing();
    ClearScreen();
    WriteString(960/2 - 50, 544/2, 0xFFFFFFFF, 1.0f, "Loading%.*s", cntr, "...");
    Flip();
}

UPDATE
{
    cur_time = sceKernelGetSystemTimeWide();

    int status;
    if(sceKernelGetThreadExitStatus(GetComTID(), &status) == 0){
        sceKernelDeleteThread(GetComTID());

        Scene *new_scene = CategoryScene_Construct("MainCategoryScene");
        ReplaceCurrentScene(man, new_scene);
    }
}

DESTROY
{
}

ENTER
{
    start_time = sceKernelGetSystemTimeWide();
    cur_time = start_time;
}

EXIT
{
}
CONSTRUCTOR

Scene *
ComInitThreadScene_Construct(void)
{
    return NEW(ComInitThreadScene)("EntryScene");
}