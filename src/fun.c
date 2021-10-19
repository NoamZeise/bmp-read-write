#include "fun.h"



double inMandelbrot(double x, double y)
{
  //if(x>0)printf("x:%lf\ny:%lf\n\n", x, y);
  double _Complex c = x + (I*y);
  double _Complex z = 0;
  const int iterations = 200;
  for(uint i = 0; i < iterations; i++)
  {
    z = (z*z) + c;
    if(cabs(z) > 2)
      return (double)i / 70.0;
  }
  if(cabs(z) > 2)
    return 0.0;
  else
    return 1.0;
}


double within(double val, double size, double scale, double offset)
{
  return ((val / size) * scale) + offset;
}

void funImage()
{
  //create test image
    img test_image = emptyImageStruct();
    test_image.width = 10000;
    test_image.height = 6000;
    test_image.channel_count = 4;
    test_image.bytes_per_channel = 1;
    size_t size = test_image.width * test_image.height * test_image.channel_count * test_image.bytes_per_channel;
    test_image.pixel_data = (unsigned char*)malloc(size);
    //gen image
    printf("generating image\n");
    for(uint x = 0; x < test_image.width; x++)
    {
      double progress = (x / test_image.width) * 100;
      for(uint y = 0; y < test_image.height; y++)
      {
        float speed = inMandelbrot(within(x, test_image.width, 3, -2), within(y, test_image.height, 2, -1));
        unsigned char col = (unsigned char)(speed * 255.0);
        unsigned char colour[4] = {col, col, col, 0xFF};
        setPixel(&test_image, x, y, colour);
      }
    }
    printf("finished generating image\n");
  //test saving
    saveBmp(&test_image, "test.bmp");
    printf("\nsaved image width: %u\n", test_image.width);
    printf("saved image height: %u\n", test_image.height);
    printf("saved image channels: %u\n", test_image.channel_count);
    free(test_image.pixel_data);

}
