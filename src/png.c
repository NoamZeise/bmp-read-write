#include "png.h"


int loadPng(const char* path)
{
  size_t fileSize;
  unsigned char* img_file = loadFile(path, &fileSize);
  if(!img_file) return IMG_FAILURE;

  return IMG_FAILURE
//todo load png

  return IMG_SUCCESS;
}
