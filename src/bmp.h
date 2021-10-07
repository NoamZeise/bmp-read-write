#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "img.h"


#define BMP_FILE_HEADER_BYTES 14
#define BMP_INFO_HEADER_BYTES 40


int loadBmp(const char* path)
{
  size_t fileSize;
  char* img_file = loadFile(path, &fileSize);
  if(!img_file) return IMG_FAILURE;


  free(img_file);
  img_file = NULL;
  return IMG_SUCCESS;
}

int saveBmp(img* image, const char* filename)
{
  if(sizeof(char) != 1)
  {
    printf("system not supported");
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
	info_header[8] = (char)image->height;
	info_header[9] = (char)(image->height >> 8);
	info_header[10] = (char)(image->height >> 16);
	info_header[11] = (char)(image->height >> 24);
	//no. of planes (must be one)
	info_header[12] = 0x01;
	info_header[13] = 0x00;
	//bits per pixel
	info_header[14] = (char)(image->bytes_per_channel * image->channel_count * 8);
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
  img test_image;
  test_image.width = 1000;
  test_image.height = 1000;
  test_image.channel_count = 4;
  test_image.bytes_per_channel = 1;
  size_t img_size = test_image.width * test_image.height * test_image.channel_count * test_image.bytes_per_channel;
  test_image.pixel_data = (char*)malloc(img_size);
  for(size_t i = 0; i < img_size; i++)
  {
    test_image.pixel_data[i] = 0xFF;
  }
  if(saveBmp(&test_image, "test.bmp"))
    printf("sucessfully saved test bmp\n");
  else
    printf("failed to save test bmp\n");


  free(test_image.pixel_data);
}




#endif
