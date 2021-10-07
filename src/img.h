#ifndef IMG_H
#define IMG_H


#define IMG_FAILURE  0;
#define IMG_SUCCESS  1;

#include <stdio.h>
#include <stdlib.h>



typedef struct s_img{
  uint width;
  uint height;
  uint channel_count;
  uint bytes_per_channel;
  char* pixel_data;
} img;


void setPixel(img* image, uint x, uint y, char colour[4])
{
  if(x > image->width || x < 0 || y > image->height || y < 0)
  {
    printf("setPixel() params out of range error!");
    return;
  }
  for(size_t i = 0; i < image->channel_count; i++)
  {
    image->pixel_data[(y * (image->width * image->bytes_per_channel * image->channel_count))
                                  + (x *  image->bytes_per_channel * image->channel_count) + i] = colour[i];
  }
}


char* loadFile(const char* path, size_t* img_file_size)
{
  FILE* img_file = fopen(path, "rb"); //rb - r=read b=binary
  if(!img_file)
  {
    printf("failed to load file %s !", path);
    return NULL;
  }
  fseek(img_file, 0L, SEEK_END);
  *img_file_size = ftell(img_file);
  char *img_file_data = (char*)malloc(*img_file_size);
  if(!img_file_data)
  {
    fclose(img_file);
    printf("failed to allocate memory for %s !", path);
    return NULL;
  }
  if( 1!=fread(img_file_data , *img_file_size, 1 , img_file) )
  {
        fclose(img_file);
        free(img_file_data);
        return NULL;
  }
  fclose(img_file);
  return img_file_data;
}


#endif
