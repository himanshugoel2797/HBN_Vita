#include "helpers/json_helpers.h"

#include <stdlib.h>

#include <psp2/io/fcntl.h>

json_value*
ParseJsonFile(const char *path) {
    SceUID fd = 0;

    if(!(fd = sceIoOpen(path, SCE_O_RDONLY, 0777)))
        return NULL;

    SceOff len = sceIoLseek(fd, 0, SCE_SEEK_END);
    sceIoLseek(fd, 0, SCE_SEEK_SET);

    json_char *buffer = malloc(len);
    if(buffer == NULL){
        sceIoClose(fd);
        return NULL;
    }

    sceIoRead(fd, buffer, len);
    sceIoClose(fd);
    
    return json_parse(buffer, len);
}