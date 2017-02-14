#include "helpers/hbn_coms.h"
#include "helpers/net.h"

#include <psp2/apputil.h>

#define DATA_STORE_URL "https://raw.githubusercontent.com/himanshugoel2797/HBN_Data/master/"

const int supportedLanguages[] = {
    SCE_SYSTEM_PARAM_LANG_ENGLISH_US,
    -1};

static int language;

void InitializeComs(void)
{
    //Default to English
    language = SCE_SYSTEM_PARAM_LANG_ENGLISH_US;

    //Detect system language and attempt to set it
    int sys_lang = language;
    SceAppUtilInitParam init_param;
    SceAppUtilBootParam boot_param;
    memset(&init_param, 0, sizeof(init_param));
    memset(&boot_param, 0, sizeof(boot_param));
    sceAppUtilInit(&init_param, &boot_param);
    sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, &sys_lang);
    SetLanguage(sys_lang);

    //Download basic data
}

int SetLanguage(int name)
{
    int *iterator = supportedLanguages;
    while (*iterator != -1)
    {
        if (*iterator == name)
        {
            language = sys_lang;
            return 0;
        }
        iterator++;
    }

    return -1;
}

int CheckForUpdates(void)
{
}

int ApplyUpdate(void)
{
}

int GetCategoryCount(void)
{
}

void GetCategoryName(int index, char name[256])
{
}

int GetSubCategoryCount(int cat)
{
}

void GetSubCategoryName(int cat, int index, char name[256])
{
}

int GetSubCategoryHomebrewCount(int cat, int sub_cat)
{
}

int GetSubCategoryHomebrewID(int cat, int sub_cat)
{
}

int GetHomebrewInfo(int id, HomebrewInfo *info)
{
}