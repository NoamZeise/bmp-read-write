#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include "img.h"


#define FILE_HEADER_BYTES 14;
#define INFO_HEADER_BYTES 40;


int loadBmp(const char* path)
{
  size_t fileSize;
  char* img_file = loadFile(path, &fileSize);
  if(!img_file) return IMG_FAILURE;



  return IMG_SUCCESS;
}

int saveBmp(img* image, const char* filename)
{
  FILE* img_file = fopen(filename, "wb");
  if(!img_file)
  {
    printf("failed to save bmp file %s", filename);
    return IMG_FAILURE;
  }

  fclose(img_file);

  return IMG_FAILURE;
}

void testBmpFileSaving()
{
  img test_image;
  test_image.width = 10;
  test_image.height = 10;
  test_image.channel_count = 4;
  test_image.bytes_per_channel = 3;
  saveBmp(&test_image, "test.bmp");
}




#endif
