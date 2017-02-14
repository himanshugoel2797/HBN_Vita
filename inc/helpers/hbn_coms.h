#ifndef HBN_COMS_H_
#define HBN_COMS_H_

#include <stdint.h>
#include <stddef.h>

#include <psp2/system_param.h>

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