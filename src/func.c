#include "func.h"



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

void gen_mandelbrot(uint w, uint h, const char* filename)
{
  if(w > 10000 || h > 10000)
  {
    printf("width/height too large, max 10000 (would take too long to execute!)\n");
    return;
  }
  img image = fillImageStruct(w, h);
  printf("generating image - press c to cancel\n");
  float percent = 0;
  for(uint x = 0; x < image.width; x++)
  {
    for(uint y = 0; y < image.height; y++)
    {
      float speed = inMandelbrot(within(x, image.width, 3, -2), within(y, image.height, 2, -1));
      unsigned char col = (unsigned char)(speed * 255.0);
      unsigned char colour[4] = {col, col, col, 0xFF};
      setPixel(&image, x, y, colour);
    }
    percent = (float)(x) / (float)(image.width);
    printf("%u%%\r", (uint)(percent * 100.0f));
  }
  printf("100%%");
  printf("\nfinished generating image\n");
  saveBmp(&image, filename);
  printf("\nsaved image to %s\n", filename);
  free(image.pixel_data);

}
