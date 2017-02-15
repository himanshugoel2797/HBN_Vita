#include "scenemanager.h"
#include "scenes/gekihen_scene.h"

#include "helpers/display.h"
#include "helpers/net.h"
#include "helpers/file.h"
#include "helpers/hbn_coms.h"

#include <psp2/kernel/threadmgr.h>

static SceUID com_tid = 0;
int com_init_thread(SceSize args, void *argp) {
    InitializeComs();

    sceKernelExitThread(0);
    return 0;
}

SceUID GetComTID(void) {
    return com_tid;
}

int main()
{
    //Initialize required services
    InitializeNet();
    InitializeDisplay();
    InitializeFileStore();

    com_tid = sceKernelCreateThread("com_init_thread", com_init_thread, 128, 0x1000, 0, 0, NULL);
    sceKernelStartThread(com_tid, 0, NULL);

    //TODO create a scene to wait on and delete the com_init_thread while displaying the loading screen
    //TODO then start working on a scene for the categories
    //TODO then start working on a generic scene for a subcategory
    //TODO then start working on a generic scene for a homebrew

    //Setup scene management
    SceneManager *main_scene_man = CreateSceneManager(SceneManagerType_Stack, "MainSceneMan", 8);

    //Create the entry scene
    Scene *s = GekihenScene_Construct();

    PushScene(main_scene_man, s);

    //Start the SceneEngine
    return StartSceneEngine(main_scene_man);
}