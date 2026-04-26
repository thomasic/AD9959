#ifndef ADAPTIVE_FILTER_H_
#define ADAPTIVE_FILTER_H_

#include "main.h"
#include <stdint.h>

typedef struct
{
   float *x_ptr;
   float d;
   int16_t lenth_x;
}Adaptive_Filter_In;

typedef struct
{
    float y;
    float error;
}Adaptive_Filter_Out;

#endif