#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "func.h"

int isDigit(const char* str)
{
  const char* digits = "0123456789";
  if(strspn(str, digits) == strlen(str))
    return 1;
  return 0;
}

int main(int argc, char** argv)
{
  int valid = 0;

  for(uint i = 1; i < argc; i++)
  {
    if(strcmp(argv[i], "--help") == 0)
    {
      valid = 1;

      printf(
        "\n"
        "functions [name] [parameter:type]...:\n"
        "'mandelbrot [save file location:string] [width:uint] [height:uint]'\n"
        "'mandelbrot+ [save file location:string] [width:uint] [height:uint] [xOffset:double] [yOffset:double] [xScale:double] [yScale:double]'\n"
        );
    }
    if(strcmp(argv[i], "mandelbrot") == 0)
    {
      if(i + 3 >= argc)
        break;

      valid = 1;

      if(isDigit(argv[i + 2]) && isDigit(argv[i + 3]))
      {
        struct timeval  tv1, tv2;
        gettimeofday(&tv1, NULL);
        clock_t begin = clock();

        img image = gen_mandelbrot( (uint)atoi(argv[i + 2]), (uint)atoi(argv[i + 3]), -2, -1, 3, 2);

        clock_t end = clock();
        gettimeofday(&tv2, NULL);
        double real_time = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
        double time = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("took %fs\n", time);
        printf("took %fs in real time\n", real_time);

        saveBmp(&image, argv[i + 1]);
        printf("saved image to %s\n", argv[i + 1]);
        free(image.pixel_data);
      }
      else
        valid = 0;
    }
    if(strcmp(argv[i], "mandelbrot+") == 0)
    {
      if(i + 7 >= argc)
        break;

      if(isDigit(argv[i + 2]) && isDigit(argv[i + 3]))
      {
        double params[4] = { 0.0, 0.0, 0.0, 0.0};
        int failed = 0;
        for(uint paramIndex = 0; paramIndex < 4; paramIndex++)
        {
          char *endptr = NULL;
          double convertedDouble = strtod(argv[i+4+paramIndex], &endptr);
          if (argv[i+paramIndex] == endptr) 
          {
            printf("failed to read input at arg index %i as double\n", i+paramIndex);
            failed = -1;
            break;
          }
          params[paramIndex] = convertedDouble;
        }
        if(failed == -1)
          break;

        img image = gen_mandelbrot( (uint)atoi(argv[i + 2]), (uint)atoi(argv[i + 3]), params[0], params[1], params[2], params[3]);

        saveBmp(&image, argv[i + 1]);
        printf("saved image to %s\n", argv[i + 1]);
        free(image.pixel_data);
        valid = 1;
      }
    }
  
    if(strcmp(argv[i], "oldmandelbrot") == 0)
    {
      if(i + 3 >= argc)
        break;

      if(isDigit(argv[i + 2]) && isDigit(argv[i + 3]))
      {
        clock_t begin = clock();
        img image = old_genmandelbrot( (uint)atoi(argv[i + 2]), (uint)atoi(argv[i + 3]));
        clock_t end = clock();
        double time = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("took %f\n", time);

        saveBmp(&image, argv[i + 1]);
        printf("saved image to %s\n", argv[i + 1]);
        free(image.pixel_data);
        valid = 1;
      }
    }
  }

  if(valid == 0)
  {
    printf("incorrect arguments, --help for functions\n");
  }

  return 0;
}
