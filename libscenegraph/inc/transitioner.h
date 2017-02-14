#ifndef LIB_SCENEGRAPH_TRANSITIONER_H_
#define LIB_SCENEGRAPH_TRANSITIONER_H_

#include <stddef.h>

#include "scenemanager.h"

typedef int (*Transitioner)(Scene *src, Scene *dst, float time);

#endif