#ifndef HBN_FILE_H_
#define HBN_FILE_H_

#include <stdbool.h>

#define TMP_DATA_PATH "ux0:data/THBN00001"

void InitializeFileStore(void);

bool Exists(const char *path, bool IsDir);

void ConcatPath(char buffer[512], const char *path_base, const char *part);

void GetTmpFileName(char name[64], const char *ext);

#endif