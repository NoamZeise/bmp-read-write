#include "img.h"

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

img fillImageStruct(unsigned int width, unsigned int height)
{
  img image;
  image.width = width;
  image.height = height;
  image.channel_count = 4;
  image.bytes_per_channel = 1;
  size_t size = image.width * image.height * image.channel_count * image.bytes_per_channel;
  image.pixel_data = (unsigned char*)malloc(size);
  return image;
}

void setPixel(img* image, uint32_t x, uint32_t y, unsigned char* colour)
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
