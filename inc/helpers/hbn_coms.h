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

typedef struct
{
    char locale[4];
    char *string;

} LocalizedString;

typedef struct AppTreeData AppTreeData;
typedef struct
{
    AppTreeData *Entries;
    int Count;
} ItemList;

struct AppTreeData
{
    LocalizedString *Name;
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

int GetCategoryCount(void);

void GetCategoryName(int index, char name[256]);

int GetSubCategoryCount(int cat);

void GetSubCategoryName(int cat, int index, char name[256]);

int GetSubCategoryHomebrewCount(int cat, int sub_cat);

int GetSubCategoryHomebrewID(int cat, int sub_cat);

int GetHomebrewInfo(int id, HomebrewInfo *info);

#endif