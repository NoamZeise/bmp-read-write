#ifndef IMG_H
#define IMG_H


#define IMG_FAILURE  0;
#define IMG_SUCCESS  1;

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned int uint;

typedef struct s_img{
  uint32_t width;
  uint32_t height;
  uint32_t channel_count;
  uint32_t bytes_per_channel;
  unsigned char* pixel_data;
} img;

img emptyImageStruct()
{
  img image;
  image.width = 0;
  image.height = 0;
  image.channel_count = 0;
  image.bytes_per_channel = 0;
  image.pixel_data = NULL;
  return image;
}


void setPixel(img* image, uint32_t x, uint32_t y, unsigned char colour[4])
{
  if(x > image->width || x < 0 || y > image->height || y < 0)
  {
    printf("setPixel() params out of range error!\n");
    return;
  }
  for(size_t i = 0; i < image->channel_count; i++)
  {
    image->pixel_data[(y * (image->width * image->bytes_per_channel * image->channel_count))
                                  + (x *  image->bytes_per_channel * image->channel_count) + i] = colour[i];
  }
}


unsigned char* loadFile(const char* path, size_t* img_file_size)
{
  //open file
  FILE* img_file = fopen(path, "rb");
  if(!img_file)
  {
    printf("failed to load file %s !\n", path);
    return NULL;
  }

  //get file size
  if(fseek(img_file, 0L, SEEK_END) != 0)
  {
    printf("failed to find end of file at %s\n", path);
    return NULL;
  }
  *img_file_size = ftell(img_file);
   if(fseek(img_file, 0L, SEEK_SET) != 0)
  {
    printf("failed to find start of file at %s\n", path);
    return NULL;
  }

  //create buffer for file data
  unsigned char *img_file_data = (unsigned char*)malloc(*img_file_size);
  if(!img_file_data)
  {
    fclose(img_file);
    printf("failed to allocate memory for %s !\n", path);
    return NULL;
  }

  //read file data into buffer
  if(fread(img_file_data , *img_file_size, 1 , img_file) != 1)
  {
    printf("failed to load image at %s into buffer\n", path);
    fclose(img_file);
    free(img_file_data);
    return NULL;
  }
  fclose(img_file);
  return img_file_data;
}


#endif
