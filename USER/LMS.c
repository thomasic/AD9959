#include"LMS.h"
#include "arm_math.h"

 * 算法递推形式：
 * 估计瞬时误差：        e(k) = d(k) - w(k)T * x(k)
 * 估计滤波系数矢量：    w(k+1) = w(k) + 2 niu e(k)x(k)
 * 初始条件：            w(0) = 0;


float lms_x[LMS_M];
float lms_w_forward[LMS_M];
float lms_w[LMS_M];

Adaptive_Filter_In lms_input;
Adaptive_Filter_Out lms_output;

void LMS_Theory(Adaptive_Filter_In *input, Adaptive_Filter_Out *output)
{

    int16_t i;
    static int16_t FIR_order;
    static uint8_t flag = 1;
    static float *w_ptr, *w_forward_ptr, *Temp_w_ptr;
    static float *x_ptr;
    float Temp;
    if(flag)
    {
        flag = 0;
        FIR_order = input->lenth_x;

        memset((void *)lms_w_forward, 0, sizeof(float) * LMS_M);
        memset((void *)lms_w, 0, sizeof(float) * LMS_M);

        w_forward_ptr = lms_w_forward;
        w_ptr = lms_w;

        x_ptr = input->x_ptr;
    }

    arm_dot_prod_f32( x_ptr,w_ptr, FIR_order,&output->y);
    output->error = input->d - output->y;

    temp = 2*LMS_NIU*output->error; 
    for(i = 0; i < FIR_order; i+=8)
    {
        w_forward_ptr[i]      = w_ptr[i]     + temp*x_ptr[i];
        w_forward_ptr[i + 1]  = w_ptr[i + 1] + temp*x_ptr[i + 1];
        w_forward_ptr[i + 2]  = w_ptr[i + 2] + temp*x_ptr[i + 2];
        w_forward_ptr[i + 3]  = w_ptr[i + 3] + temp*x_ptr[i + 3];
        w_forward_ptr[i + 4]  = w_ptr[i + 4] + temp*x_ptr[i + 4];
        w_forward_ptr[i + 5]  = w_ptr[i + 5] + temp*x_ptr[i + 5];
        w_forward_ptr[i + 6]  = w_ptr[i + 6] + temp*x_ptr[i + 6];
        w_forward_ptr[i + 7]  = w_ptr[i + 7] + temp*x_ptr[i + 7];
    }

    Temp_w_ptr = w_forward_ptr;
    w_forward_ptr = w_ptr;
    w_ptr = Temp_w_ptr;
}
