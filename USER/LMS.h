#ifndef __LMS_H_
#define __LMS_H_

#include "Adaptive_Filter.h"

#define LMS_M 16
#define LMS_NIU 0.0005f
#define LMS_EPSILON 1e-7f

extern float lms_x[LMS_M];
extern Adaptive_Filter_In lms_input;
extern Adaptive_Filter_Out lms_output;

void LMS_Theory(Adaptive_Filter_In *input, Adaptive_Filter_Out *output);

#endif 