#ifndef ADAPTIVE_FITLER_H_
#define ADAPTIVE_FITLER_H_

typedef struct
{
   float *x_ptr;
   uint16_t lenth_x;
   float d;
}Adptive_Filter_In;

typedef struct
{
    float y;
    float error;
}Adptive_Filter_Out;

