#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "img.h"


#define BMP_FILE_HEADER_BYTES 14
#define BMP_INFO_HEADER_BYTES 40

img loadBmp(const char* path);

int saveBmp(img* image, const char* filename);


#endif
