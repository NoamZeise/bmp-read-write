#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <pthread.h>
#include "img.h"
#include "bmp.h"

img gen_mandelbrot(uint w, uint h, double xOff, double yOff, double xScale, double yScale);

img old_genmandelbrot(uint w, uint h);