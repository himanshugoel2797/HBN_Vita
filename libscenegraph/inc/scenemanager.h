#ifndef LIB_SCENEGRAPH_SCENEMANAGER_H_
#define LIB_SCENEGRAPH_SCENEMANAGER_H_

#include <stddef.h>
#include <stdint.h>

typedef struct SceneManager SceneManager;
typedef struct Scene Scene;

struct Scene
{
    char name[256];
    int ref_count;
    void *state;

    void (*Initialize)(Scene *cur);
    void (*Render)(Scene *cur, SceneManager *man, uint32_t time);
    void (*Update)(Scene *cur, SceneManager *man, uint32_t time);
    void (*Destroy)(Scene *cur);
    void (*Enter)(Scene *cur, Scene *prev);
    void (*Exit)(Scene *cur, Scene *next);
};

#define INITIALIZE_BASE(x) static void Initialize_##x(Scene *cur)
#define RENDER_BASE(x) static void Render_##x(Scene *cur, SceneManager *man, uint32_t time)
#define UPDATE_BASE(x) static void Update_##x(Scene *cur, SceneManager *man, uint32_t time)
#define DESTROY_BASE(x) static void Destroy_##x(Scene *cur)
#define ENTER_BASE(x) static void Enter_##x(Scene *cur, Scene *prev)
#define EXIT_BASE(x) static void Exit_##x(Scene *cur, Scene *next)

#define CONSTRUCTOR_BASE(x)                           \
    static Scene *Construct_##x(const char name[256]) \
    {                                                 \
        Scene *tmp = CreateScene(name);               \
        if (tmp == NULL)                              \
            return NULL;                              \
        tmp->Initialize = Initialize_##x;             \
        tmp->Render = Render_##x;                     \
        tmp->Update = Update_##x;                     \
        tmp->Destroy = Destroy_##x;                   \
        tmp->Enter = Enter_##x;                       \
        tmp->Exit = Exit_##x;                         \
        return tmp;                                   \
    }

#define INITIALIZE_L0(x) INITIALIZE_BASE(x)
#define RENDER_L0(x) RENDER_BASE(x)
#define UPDATE_L0(x) UPDATE_BASE(x)
#define DESTROY_L0(x) DESTROY_BASE(x)
#define ENTER_L0(x) ENTER_BASE(x)
#define EXIT_L0(x) EXIT_BASE(x)
#define CONSTRUCTOR_L0(x) CONSTRUCTOR_BASE(x)

#define INITIALIZE INITIALIZE_L0(CLASS_NAME)
#define RENDER RENDER_L0(CLASS_NAME)
#define UPDATE UPDATE_L0(CLASS_NAME)
#define DESTROY DESTROY_L0(CLASS_NAME)
#define ENTER ENTER_L0(CLASS_NAME)
#define EXIT EXIT_L0(CLASS_NAME)
#define CONSTRUCTOR CONSTRUCTOR_L0(CLASS_NAME)

#define NEW(x) Construct_##x

Scene *
CreateScene(const char name[256]);

void DestroyScene(Scene *scene);

typedef enum {
    SceneManagerType_Stack
} SceneManagerType;

struct SceneManager
{
    Scene AsScene;
    SceneManagerType Type;

    int MaxSceneCount;
    int SceneCount;
    int ActiveSceneIndex;
    Scene **Scenes;
};

SceneManager *
CreateSceneManager(SceneManagerType type,
                   const char name[256],
                   int MaxSceneCount);

void DestroySceneManager(SceneManager *man);

int PushScene(SceneManager *man,
              Scene *scene);

Scene *
PopScene(SceneManager *man);

Scene *
ReplaceCurrentScene(SceneManager *man,
                    Scene *scene);

SceneManager *
GetMainSceneManager(void);

void ExitSceneEngine(int code);

int StartSceneEngine(SceneManager *main_manager);

#endif