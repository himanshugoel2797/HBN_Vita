#include "helpers/hbn_coms.h"
#include "helpers/net.h"
#include "helpers/file.h"
#include "helpers/json_helpers.h"

#include <string.h>

#include <psp2/apputil.h>

#define DATA_STORE_URL "https://raw.githubusercontent.com/himanshugoel2797/HBN_Data/master/"
#define BASE_FILENAME "/base.json"
#define HOMEBREW_FILENAME "/homebrew.json"

#define CUR_MAJOR_VER 0
#define CUR_MINOR_VER 1

struct LocaleNameMapping{
    char name[4];
    int val;
};

const int supportedLanguages[] = {
    SCE_SYSTEM_PARAM_LANG_ENGLISH_US,
    -1};

const struct LocaleNameMapping name_mappings[] = {
    {"us", SCE_SYSTEM_PARAM_LANG_ENGLISH_US},
    {"", -1}
};

static int language;
static BaseData base_data;

static void ParseItems(json_value *val, ItemList *itemList);
static void FillItemList(json_value *val, ItemList *itemList);

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
            for(int j = 0; j < value->u.array.length; j++){
                
                json_value *array_item = value->u.array.values[j];
                char *locale_name = array_item->u.object.values[0].name;
                json_value *value = array_item->u.object.values[0].value;

                int k = 0;
                while(name_mappings[k].val != -1) {
                    if((name_mappings[k].val == language) && (strcmp(name_mappings[k].name, locale_name) == 0))
                        strcpy(item->Name, value->u.string.ptr);

                    k++;
                }
            }
        }
        else if (strcmp(name, "File") == 0)
        {
            char url[URL_LEN];
            char tmp_path[64];

            if(strlen(value->u.string.ptr) > 0){
                ConcatPath(url, DATA_STORE_URL, value->u.string.ptr);
                GetTmpFileName(tmp_path, ".json");
                DownloadFile(url, tmp_path);

                json_value *tmp_file = ParseJsonFile(tmp_path);
                FillItemList(tmp_file, &item->Items);
            }
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
            base_data.LatestMajorVer = value->u.array.values[0]->u.integer;
            base_data.LatestMinorVer = value->u.array.values[1]->u.integer;
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
    return (base_data.LatestMajorVer != CUR_MAJOR_VER) | (base_data.LatestMinorVer != CUR_MINOR_VER);
}

int ApplyUpdate(void)
{
    if(CheckForUpdates()){
        //TODO

        return 1;
    }

    return 0;
}

ItemList* GetCategoryItems(void)
{
    return &base_data.Items;
}

int GetHomebrewInfo(int id, HomebrewInfo *info)
{
    return 0;
}