#include "func.h"



double inMandelbrot(double x, double y)
{
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


struct thread_arg 
{ 
  uint x;
  uint w;
  uint h;
  double xOff;
  double yOff;
  double xScale;
  double yScale;
  img* image;
};

struct thread_arg getArgs(uint x, uint w, uint h, double xOff, double yOff, double xScale, double yScale, img* image)
{
  struct thread_arg arg;
  arg.x = x;
  arg.w = w;
  arg.h = h;
  arg.xOff = xOff;
  arg.yOff = yOff;
  arg.xScale = xScale;
  arg.yScale = yScale;
  arg.image = image;
  return arg;
}

void* setMandelbrot(void* args)
{
  struct thread_arg *a = (struct thread_arg *)args;

  for(uint y = 0; y < a->h; y++)
    {
      float speed = inMandelbrot(within(a->x, a->w, a->xScale, a->xOff), within(y, a->h, a->yScale, a->yOff));
      unsigned char col = (unsigned char)(speed * 255.0);
      unsigned char colour[4] = {col, col, col, 0xFF};
      setPixel(a->image, a->x, y, colour);
    }

  pthread_exit(NULL);
}

img old_genmandelbrot(uint w, uint h)
{
  if(w > 10000 || h > 10000)
  {
    printf("width/height too large, max 10000 (would take too long to execute!)\n");
    return emptyImageStruct();
  }
  img image = fillImageStruct(w, h);
  printf("generating image\n");
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
    printf("progress: %u%%\r", (uint)(percent * 100.0f));
  }
  printf("progress: 100%%");
  printf("\nfinished generating image\n");
  return image;
}

img gen_mandelbrot(uint w, uint h, double xOff, double yOff, double xScale, double yScale)
{
  if(w > 20000 || h > 20000)
  {
    printf("width/height too large, max 20000 (dificult to save such a big file!)\n");
    return emptyImageStruct();
  }
  img image = fillImageStruct(w, h);
  printf("generating image\n");
  float percent = 0;

  pthread_t *threads = malloc(w * sizeof(pthread_t));
  struct thread_arg *args = malloc(w * sizeof(struct thread_arg));
  pthread_attr_t attr;
  uint joinedThreads = 0;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  uint index = 0;
  for(uint x = 0; x < image.width; x++)
  {
    args[index] = getArgs(x, w, h, xOff, yOff, xScale, yScale, &image);
    int err = pthread_create(&threads[index], &attr, setMandelbrot, (void*)&args[index]);
    if(err)
    {
      printf("unable to create thread! index : %d\njoining unjoined threads\n", index);
      for(uint i = joinedThreads; i < index - 1; i++)
      {
        int inerr = pthread_join(threads[i], NULL);
        if(inerr)
        {
          printf("unable to join thread in buffer at %d!\n", index);
          exit(-1);
        }
        joinedThreads++;
      }
      printf("finished joining thread\n");
      index--;
      x--;
    } 
    index++;
    percent = (float)(x) / (float)(image.width);
    printf("progress: %u%%\r", (uint)(percent * 100.0f));
  }

  pthread_attr_destroy(&attr);
  for(uint i = joinedThreads; i < w; i++)
  {
    int err = pthread_join(threads[i], NULL);
    if(err)
    {
      printf("unable to join thread at %d!\n", i);
      exit(-1);
    }
  }
  free(threads);
  free(args);

  printf("progress: %u%%\r", 100);
  printf("\nfinished generating image\n");

  return image;
}
