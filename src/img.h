#ifndef IMG_H
#define IMG_H


#define IMG_FAILURE  0;
#define IMG_SUCCESS  1;

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct s_img{
  uint32_t width;
  uint32_t height;
  uint32_t channel_count;
  uint32_t bytes_per_channel;
  unsigned char* pixel_data;
} img;

img emptyImageStruct();

void setPixel(img* image, uint32_t x, uint32_t y, unsigned char* colour);

unsigned char* loadFile(const char* path, size_t* img_file_size);

#endif
