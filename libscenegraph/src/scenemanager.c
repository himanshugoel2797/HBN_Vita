#include "scenemanager.h"
#include "transitioner.h"

#include <stdlib.h>
#include <string.h>


static void 
SceneMan_Initialize(Scene *cur){

}

static void 
SceneMan_Render(Scene *cur, SceneManager *man, uint32_t time){
    SceneManager *sceneMan = (SceneManager*)cur;
    if(sceneMan->ActiveSceneIndex >= 0 && sceneMan->Scenes[sceneMan->ActiveSceneIndex] != NULL)
        sceneMan->Scenes[sceneMan->ActiveSceneIndex]->Render(sceneMan->Scenes[sceneMan->ActiveSceneIndex], sceneMan, time);
}

static void 
SceneMan_Update(Scene *cur, SceneManager *man, uint32_t time){
    SceneManager *sceneMan = (SceneManager*)cur;
    if(sceneMan->ActiveSceneIndex >= 0 && sceneMan->Scenes[sceneMan->ActiveSceneIndex] != NULL)
        sceneMan->Scenes[sceneMan->ActiveSceneIndex]->Update(sceneMan->Scenes[sceneMan->ActiveSceneIndex], sceneMan, time);
}

static void 
SceneMan_Destroy(Scene *cur){

}

static void 
SceneMan_Enter(Scene *cur, Scene *prev){
    SceneManager *sceneMan = (SceneManager*)cur;
    if(sceneMan->ActiveSceneIndex >= 0 && sceneMan->Scenes[sceneMan->ActiveSceneIndex] != NULL)
        sceneMan->Scenes[sceneMan->ActiveSceneIndex]->Enter(sceneMan->Scenes[sceneMan->ActiveSceneIndex], prev);
}

static void 
SceneMan_Exit(Scene *cur, Scene *next){
    SceneManager *sceneMan = (SceneManager*)cur;
    if(sceneMan->ActiveSceneIndex >= 0 && sceneMan->Scenes[sceneMan->ActiveSceneIndex] != NULL)
        sceneMan->Scenes[sceneMan->ActiveSceneIndex]->Exit(sceneMan->Scenes[sceneMan->ActiveSceneIndex], next);
}


#define SCENE_BASE_INDEX 0

Scene*
CreateScene(const char name[256])
{
    Scene* scene = calloc(1, sizeof(Scene));
    if(scene == NULL)
        return NULL;

    strcpy(scene->name, name);
    return scene;
}

void
DestroyScene(Scene* scene)
{
    if(scene == NULL)
        return;
    
    free(scene);
}

SceneManager* 
CreateSceneManager(SceneManagerType     type,
                   const char           name[256],
                   int                  MaxSceneCount)
{
    SceneManager* sceneMan = calloc(1, sizeof(SceneManager));
    if(sceneMan == NULL)
        return NULL;

    //Setup handlers
    strcpy(sceneMan->AsScene.name, name);
    sceneMan->AsScene.Initialize = SceneMan_Initialize;
    sceneMan->AsScene.Update = SceneMan_Update;
    sceneMan->AsScene.Render = SceneMan_Render;
    sceneMan->AsScene.Destroy = SceneMan_Destroy;
    sceneMan->AsScene.Enter = SceneMan_Enter;
    sceneMan->AsScene.Exit = SceneMan_Exit;

    sceneMan->MaxSceneCount = MaxSceneCount;
    sceneMan->SceneCount = 0;
    sceneMan->ActiveSceneIndex = -1;
    sceneMan->Type = type;
    sceneMan->Scenes = calloc(MaxSceneCount, sizeof(Scene*));
    if(sceneMan->Scenes == NULL){
        free(sceneMan);
        return NULL;
    }

    return sceneMan;
}

void
DestroySceneManager(SceneManager    *man)
{
    if(man == NULL)
        return;

    for(int i = SCENE_BASE_INDEX; i < man->SceneCount; i++){
        if(man->Scenes[i] != NULL)
            man->Scenes[i]->ref_count--;
    }

    free(man->Scenes);
    free(man);
}

int
PushScene(SceneManager   *man, 
          Scene          *scene)
{
    if(man == NULL)
        return -1;

    if(scene == NULL)
        return -1;

    if(man->SceneCount == man->MaxSceneCount)
        return -2;

    Scene* prev = NULL;
    if(man->SceneCount > 0)
        prev = man->Scenes[man->SceneCount - 1];
    
    man->Scenes[man->SceneCount] = scene;
    scene->ref_count++;
    if(scene->ref_count == 1)
        scene->Initialize(scene);

    man->ActiveSceneIndex = man->SceneCount++;

    if(prev != NULL)
        prev->Exit(prev, scene);

    scene->Enter(scene, prev);
    return man->ActiveSceneIndex;
}

Scene*
PopScene(SceneManager    *man)
{
    if(man == NULL)
        return NULL;

    if(man->ActiveSceneIndex < 0)
        return NULL;

    Scene* scene = man->Scenes[man->ActiveSceneIndex];
    man->Scenes[man->ActiveSceneIndex--] = NULL;
    scene->ref_count--;


    Scene* next = NULL;
    if(man->ActiveSceneIndex >= 0)
        next = man->Scenes[man->ActiveSceneIndex];

    scene->Exit(scene, next);

    if(next != NULL)
        next->Enter(next, scene);

    return scene;
}

Scene*
ReplaceCurrentScene(SceneManager    *man,
                    Scene           *scene)
{
    Scene *tmp = PopScene(man);
    PushScene(man, scene);
    return tmp;
}

///Main SceneManager

///Globals
static SceneManager *main_scene_man = NULL;
static int engine_exit_code = 0;
static int engine_exit_flag = 0;

///Methods
SceneManager*
GetMainSceneManager(void)
{
    return main_scene_man;
}

void
ExitSceneEngine(int code){
    engine_exit_code = code;
    engine_exit_flag = 1;
}

int
StartSceneEngine(SceneManager *main_manager)
{
    if(main_manager == NULL)
        return -1;

    main_scene_man = main_manager;
    uint32_t timer = 0;

    while(!engine_exit_flag){
        main_scene_man->AsScene.Update(&main_scene_man->AsScene, NULL, timer);
        main_scene_man->AsScene.Render(&main_scene_man->AsScene, NULL, timer);
        timer++;
    }

    return engine_exit_code;
}