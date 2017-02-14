#include "helpers/hbn_coms.h"
#include "helpers/net.h"
#include "helpers/file.h"
#include "helpers/json_helpers.h"

#include <string.h>

#include <psp2/apputil.h>

#define DATA_STORE_URL "https://raw.githubusercontent.com/himanshugoel2797/HBN_Data/master/"
#define BASE_FILENAME "/base.json"

struct LocaleNameMapping{
    char name[4];
    int val;
};

const int supportedLanguages[] = {
    SCE_SYSTEM_PARAM_LANG_ENGLISH_US,
    -1};

const struct LocaleNameMapping name_mappings[] = {
    {"us", SCE_SYSTEM_PARAM_LANG_ENGLISH_US}
    {"", -1}
}

static int language;
static BaseData base_data;

static void ParseItem(json_value *val, AppTreeData *item)
{
    int cnt = val->u.object.length;
    for (int i = 0; i < cnt; i++)
    {
        char *name = val->u.object.values[i].name;
        json_value *value = val->u.object.values[i].value;

        if (strcmp(name, "Name") == 0)
        {
            //TODO parse the localized names
        }
        else if (strcmp(name, "File") == 0)
        {
            char url[URL_LEN];
            char tmp_path[64];
            ConcatPath(url, DATA_STORE_URL, value->u.string.ptr);
            GetTmpFileName(tmp_path, ".json");
            DownloadFile(url, tmp_path);
            strcpy(item->File, tmp_path);
        }
        else if (strcmp(name, "Icon") == 0)
        {
            strcpy(item->IconPath, value->u.string.ptr);
        }
        else if (strcmp(name, "Type") == 0)
        {
            item->Type = value->u.integer;
        }else if (strcmp(name, "Items") == 0)
        {
            ParseItems(value, &item->Items);
        }
    }
}

static void ParseItems(json_value *val, ItemList *itemList)
{
    itemList->Count = val->u.array.length;
    itemList->Entries = malloc(itemList->Count * sizeof(AppTreeData));
    if (itemList->Entries == NULL)
        return;

    for (int i = 0; i < itemList->Count; i++)
        ParseItem(val->u.array.values[i], &itemList->Entries[i]);
}

static void FillItemList(json_value *val, ItemList *itemList)
{
    for (int i = 0; i < val->u.object.length; i++)
    {
        if (strcmp(val->u.object.values[i].name, "Items") == 0)
        {
            json_value *items = val->u.object.values[i].value;
            ParseItems(items, itemList);
        }
    }
}

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
    DownloadFile(DATA_STORE_URL "base.json", TMP_DATA_PATH BASE_FILENAME);

    //Pull in associated files by parsing the base file
    json_value *base_data_json = ParseJsonFile(TMP_DATA_PATH BASE_FILENAME);
    if (base_data_json == NULL)
        return; //TODO Error and exit!

    for (int i = 0; i < base_data_json->u.object.length; i++)
    {
        char *name = base_data_json->u.object.values[i].name;
        json_value *value = base_data_json->u.object.values[i].value;

        if (strcmp(name, "LatestAppVer") == 0)
        {
        }
        else if (strcmp(name, "AppDownloadURL") == 0)
        {
            strcpy(base_data.UpdateURL, value->u.string.ptr);
        }
        else if (strcmp(name, "Items") == 0)
        {
            FillItemList(base_data_json, &base_data.Items);
        }
    }
}

int SetLanguage(int name)
{
    int iterator = 0;
    while (supportedLanguages[iterator] != -1)
    {
        if (supportedLanguages[iterator] == name)
        {
            language = name;
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