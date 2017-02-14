#include "scenemanager.h"
#include "scenes/entry_scene.h"

#include "helpers/display.h"
#include "helpers/net.h"


int main(){

    //Initialize required services
    InitializeNet();
    InitializeDisplay();

    //Setup scene management
    SceneManager *main_scene_man = CreateSceneManager(SceneManagerType_Stack, "MainSceneMan", 8);

    //Create the entry scene
    Scene *s = EntryScene_Construct();

    PushScene(main_scene_man, s);

    //Start the SceneEngine
    return StartSceneEngine(main_scene_man);
}