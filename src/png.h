#ifndef PNG_H
#define PNG_H

#include <stdio.h>
#include <stdlib.h>
#include "img.h"


const unsigned char fileHeader[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};

int loadPng(const char* path);

#endif
