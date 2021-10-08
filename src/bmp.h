#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img.h"


#define BMP_FILE_HEADER_BYTES 14
#define BMP_INFO_HEADER_BYTES 40


int checkValidBMPFile(size_t file_size, char *file_data)
{
   //check file is big enough 
  if(file_size < BMP_FILE_HEADER_BYTES + BMP_INFO_HEADER_BYTES)
  {
    printf("file is an inappropriate size\n");
    return 0;
  }
  //check file has a bmp ID field
  if(file_data[0] != 'B' || file_data[1] != 'M')
  {
    printf("file does not have a BMP file signature\n");
    return 0;
  }
  //check internal file specified size matches actual file size
  size_t interal_bmp_file_size = (size_t)(file_data[2] | file_data[3] << 8 | file_data[4] << 16 | file_data[5] << 24);
  if(interal_bmp_file_size != 0)
  {
    printf("error, file might be corrupt, the file's size was %d, but the file specified a size of %d", file_size, interal_bmp_file_size);
    return 0;
  }
  //check if file specified header size matches FILEHEADER + BITMAPINFOHEADER size
  size_t internal_bmp_header_size = (size_t)(file_data[10] | file_data[11] << 8 | file_data[12] << 16 | file_data[13] << 24);
  if(internal_bmp_header_size != BMP_FILE_HEADER_BYTES + BMP_INFO_HEADER_BYTES)
  {
    printf("error, loader does not support a header of size %d, only BITMAPINFOHEADER is supported\n");
    return 0;
  }

  return 1;
}

int fillImageInfoFromFile(img *img, char *file_data)
{
  //header offset
  size_t ho = BMP_FILE_HEADER_BYTES;
  char info_header[BMP_INFO_HEADER_BYTES];

  size_t interal_bmp_header_size =
     (size_t)(file_data[ho + 0] | file_data[ho + 1] << 8 | file_data[ho + 2] << 16 | file_data[ho + 3] << 24);   
  if(interal_bmp_header_size != BMP_INFO_HEADER_BYTES) 
  {
    printf("header is not BITMAPINFOHEADER, only BITMAPINFOHEADER is supported!\n");
    return 0;
  }

  img->width = (uint)(file_data[ho + 4] | file_data[ho + 5] << 8 | file_data[ho + 6] << 16 | file_data[ho + 7] << 24);   

  img->height = (uint)(file_data[ho + 8] | file_data[ho + 9] << 8 | file_data[ho + 10] << 16 | file_data[ho + 11] << 24); 
	//no. of planes 12-13
	//bits per pixel
	size_t bits_per_pixel = (uint)(file_data[ho + 14] | file_data[ho + 15] << 8);
  if(bits_per_pixel == 32)
  {
    img->bytes_per_channel = 1;
    img->channel_count = 4;
  }
  else if(bits_per_pixel == 24)
  {
    img->bytes_per_channel = 1;
    img->channel_count = 3;
  }
  else
  {
    printf("only 3/4 channel 1 byte per channel supported!\n");
    return 0;
  }
	//type of compression 16 - 19
  //pixel data size 20-24
  /*
  size_t pixel_data_size = 
    (size_t)(file_data[ho + 20] | file_data[ho + 21] << 8 | file_data[ho + 22] << 16 | file_data[ho + 23] << 24);
  size_t actual_data_width = pixel_data_size / img->height;

  size_t data_width_in_bytes = img->width * (bits_per_pixel / 8);
  size_t padding = actual_data_width - data_width_in_bytes; */


  return 1;
}


img loadBmp(const char* path)
{
  img image = emptyImageStruct();

  //load file 
  size_t file_size;
  char* file_data = loadFile(path, &file_size);
  if(!file_data) 
  {
    free(file_data);
    printf("failed to load file at %s\n", path);
    return image;
  }

  //check file valid

  if(!checkValidBMPFile(file_size, file_data))
  {
    free(file_data);
    printf("error at file: %s - invalid BMP file\n", path);
    return image;
  }

  //read info header to fill out image struct info
  if(!fillImageInfoFromFile(&image, file_data))
  {
    printf("error at file: %s - failed to fill image info from file info header\n", path);
    return image;
  }

  //read pixel data into pixel array

  free(file_data);
  return image;
}

int saveBmp(img* image, const char* filename)
{
  if(sizeof(char) != 1)
  {
    printf("system not supported, char isn't 1 byte\n");
    return IMG_FAILURE;
  }

  //data sizes

  size_t data_width = image->width * image->bytes_per_channel * image->channel_count;
	size_t pixel_data_padding = 4 - (data_width % 4);
	if (data_width % 4 == 0)
		pixel_data_padding = 0;
	data_width += pixel_data_padding;

  size_t pixel_data_size = data_width * image->height;
  size_t file_size = BMP_FILE_HEADER_BYTES + BMP_INFO_HEADER_BYTES + pixel_data_size;

  //file header

  char file_header_data[BMP_FILE_HEADER_BYTES];
  file_header_data[0] = 'B';
  file_header_data[1] = 'M';
  //filesize
  file_header_data[2] = (char)file_size;
  file_header_data[3] = (char)(file_size >> 8);
  file_header_data[4] = (char)(file_size >> 16);
  file_header_data[5] = (char)(file_size >> 24);
  //reserved
	file_header_data[6] = 0;
	file_header_data[7] = 0;
	file_header_data[8] = 0;
	file_header_data[9] = 0;
  //headersize
	size_t header_size = BMP_FILE_HEADER_BYTES + BMP_INFO_HEADER_BYTES;
	file_header_data[10] = (char)header_size;
	file_header_data[11] = (char)(header_size >> 8);
	file_header_data[12] = (char)(header_size >> 16);
	file_header_data[13] = (char)(header_size >> 24);

  //info header
  char info_header[BMP_INFO_HEADER_BYTES];
	//info header size
	info_header[0] = (char)BMP_INFO_HEADER_BYTES;
	info_header[1] = (char)(BMP_INFO_HEADER_BYTES >> 8);
	info_header[2] = (char)(BMP_INFO_HEADER_BYTES >> 16);
	info_header[3] = (char)(BMP_INFO_HEADER_BYTES >> 24);
	//image width
	info_header[4] = (char)image->width;
	info_header[5] = (char)(image->width >> 8);
	info_header[6] = (char)(image->width >> 16);
	info_header[7] = (char)(image->width >> 24);
	//image height
	info_header[8]  = (char)image->height;
	info_header[9]  = (char)(image->height >> 8);
	info_header[10] = (char)(image->height >> 16);
	info_header[11] = (char)(image->height >> 24);
	//no. of planes (must be one)
	info_header[12] = 0x01;
	info_header[13] = 0x00;
	//bits per pixel
	info_header[14] = (char) (image->bytes_per_channel * image->channel_count * 8);
	info_header[15] = (char)((image->bytes_per_channel * image->channel_count * 8) >> 8);
	//type of compression (0 - no compression)
	info_header[16] = 0;
	info_header[17] = 0;
	info_header[18] = 0;
	info_header[19] = 0;
	//size of image data
	info_header[20] = (char)pixel_data_size;
	info_header[21] = (char)(pixel_data_size >> 8);
	info_header[22] = (char)(pixel_data_size >> 16);;
	info_header[23] = (char)(pixel_data_size >> 24);;
	//horizonal pixels per meter on target device (usually set to zero)
	info_header[24] = 0;
	info_header[25] = 0;
	info_header[26] = 0;
	info_header[27] = 0;
	//vertical pixels per meter on target device (usually set to zero)
	info_header[28] = 0;
	info_header[29] = 0;
	info_header[30] = 0;
	info_header[31] = 0;
	//no. of colours used, if zero, number is calculated;
	info_header[32] = 0;
	info_header[33] = 0;
	info_header[34] = 0;
	info_header[35] = 0;
	//no. of important colours, if zero all are important
  info_header[36] = 0;
	info_header[37] = 0;
	info_header[38] = 0;
	info_header[39] = 0;

  char* bmp_pixel_data = (char*)malloc(pixel_data_size);
  size_t bytes_padded = 0;
  for(size_t i = 0; i < pixel_data_size; i++)
  {
      size_t mod_width = i % (pixel_data_size / image->height);
      //if end of row, pad data
      if(mod_width <= (pixel_data_size / image->height) - pixel_data_padding
      && mod_width >= (pixel_data_size / image->height) - pixel_data_padding)
      {
        bytes_padded++;
        bmp_pixel_data[i] = 0x00;
      }
      else
      {
        bmp_pixel_data[i] = image->pixel_data[i - bytes_padded];
      }
  }

  //copy to final buffer
  char* bmp_data_buffer =
    (char*)malloc(file_size);
  memcpy(bmp_data_buffer,                         file_header_data, BMP_FILE_HEADER_BYTES);
  memcpy(bmp_data_buffer + BMP_FILE_HEADER_BYTES, info_header,      BMP_INFO_HEADER_BYTES);
  memcpy(bmp_data_buffer + header_size,           bmp_pixel_data,   pixel_data_size);
  free(bmp_pixel_data);

  //copy to image
  FILE* img_file = fopen(filename, "wb");
  if(!img_file)
  {
    printf("failed to save bmp file %s", filename);
    return IMG_FAILURE;
  }
  fwrite(bmp_data_buffer, file_size, 1, img_file);

  //close image file and free final buffer
  fclose(img_file);
  free(bmp_data_buffer);

  return IMG_SUCCESS;
}

void testBmpFileSaving()
{

  img test_image = loadBmp("test.bmp");

  free(test_image.pixel_data);
}




#endif
