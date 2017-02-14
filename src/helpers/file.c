#include "helpers/file.h"

#include <psp2/io/fcntl.h>
#include <psp2/io/dirent.h>
#include <psp2/io/stat.h>

static int tmp_name_idx = 0;

void FileStoreInitialize(void)
{
    if (!Exists(TMP_DATA_PATH, true))
        sceIoMkdir(TMP_DATA_PATH, 0777);
}

bool Exists(const char *path, bool IsDir)
{
    SceUID fd = 0;

    if (!IsDir)
    {
        if (!(fd = sceIoOpen(path, SCE_O_RDONLY, 0777)))
            return false;

        sceIoClose(fd);
        return true;
    }
    else
    {
        if (!(fd = sceIoDopen(path)))
            return false;

        sceIoDclose(fd);
        return true;
    }
}

void ConcatPath(char buffer[512], const char *path_base, const char *part)
{
    strcpy(buffer, path_base);
    strcat(buffer, part);
}

void GetTmpFileName(char name[64], const char *ext)
{
    snprintf(name, "/tmp%d.%s" tmp_name_idx++, ext);
}