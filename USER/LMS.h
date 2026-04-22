#ifndef __LMS_H_
#define __LMS_H_

#include "AdaptiveFilter.h"

#define LMS_M 16
#define LMS_NIU 0.0001

extern float lms_x[LMS_M];
extern Adaptive_Filter_In lms_input;
extern Adaptive_Filter_Out lms_output;


#endif 