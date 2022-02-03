#include <stdio.h>

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
        );
    }
    if(strcmp(argv[i], "mandelbrot") == 0)
    {
      if(i + 3 >= argc)
        break;

      valid = 1;

      if(isDigit(argv[i + 2]) && isDigit(argv[i + 3]))
      {
        gen_mandelbrot( (uint)atoi(argv[i + 2]), (uint)atoi(argv[i + 3]) , argv[i + 1]);
      }
    }
  }

  if(valid == 0)
  {
    printf("incorrect arguments, --help for functions\n");
  }

  return 0;
}
