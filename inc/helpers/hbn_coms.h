#ifndef HBN_COMS_H_
#define HBN_COMS_H_

#include <stdint.h>
#include <stddef.h>

#include <psp2/system_param.h>

#define URL_LEN 512
#define FILE_NAME_LEN 128
#define APPTREE_FILE_LEN 16

typedef enum {
    AppTreeEntryType_Category,
    AppTreeEntryType_SubCategory,
    AppTreeEntryType_Application
} AppTreeEntryType;

typedef struct AppTreeData AppTreeData;
typedef struct
{
    AppTreeData *Entries;
    int Count;
} ItemList;

struct AppTreeData
{
    char Name[512];
    int NameCount;
    int ID;
    AppTreeEntryType Type;
    char File[APPTREE_FILE_LEN];
    char IconPath[FILE_NAME_LEN];
    ItemList Items;
};


typedef struct
{
    uint8_t LatestMajorVer;
    uint8_t LatestMinorVer;
    char UpdateURL[URL_LEN];
    ItemList Items;
} BaseData;

typedef struct
{

} HomebrewInfo;

void InitializeComs(void);

int SetLanguage(int name);

int CheckForUpdates(void);

int ApplyUpdate(void);

ItemList* GetCategoryItems(void);

int GetHomebrewInfo(int id, HomebrewInfo *info);

#endif