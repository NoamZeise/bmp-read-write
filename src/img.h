#ifndef IMG_H
#define IMG_H


#define IMG_FAILURE  0;
#define IMG_SUCCESS  1;

#include <stdio.h>
#include <stdlib.h>



typedef struct s_img{
  int width;
  int height;
  int channel_count;
  int bytes_per_channel;
  char* pixelData;
} img;


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
