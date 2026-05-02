#include"LMS.h"
#include "arm_math.h"
#include "usart.h"

//FLMS算法的核心是通过最小均方误差准则来调整滤波器的系数，以使得滤波器的输出尽可能接近期望的输出。算法的基本步骤如下：
//1. 初始化：设定滤波器的初始系数（通常为零）和步长参数（控制系数调整的速度）。
//2. 输入信号：获取当前时刻的输入信号向量x(k        )和期望输出d(k)。
//3. 计算滤波器输出：根据当前的滤波器系数w(k)和输入信号x(k)，计算滤波器的输出y(k) = w(k)T * x(k)。
//  * 算法递推形式：
//  * 估计瞬时误差：        e(k) = d(k) - w(k)T * x(k)
//  * 初始条件：            w(0) = 0;
//w(k+1)=w(k)+2*niu*e(k)*x(k) 其中，w(k)为第k次迭代的滤波器系数矢量，e(k)为第k次迭代的误差，x(k)为第k次迭代的输入向量，niu为步长参数。

float lms_x[LMS_M];
float lms_w_forward[LMS_M];
float lms_w[LMS_M];

static int16_t FIR_order;
static uint8_t flag = 1;
static float *w_ptr, *w_forward_ptr, *Temp_w_ptr;
static float *x_ptr;

Adaptive_Filter_In lms_input;
Adaptive_Filter_Out lms_output;

void LMS_Theory(Adaptive_Filter_In *input, Adaptive_Filter_Out *output)
{

    float Temp;
    float x_power;
    if(flag)
    {
        flag = 0;
        FIR_order = input->lenth_x;

        memset((void *)lms_w_forward, 0, sizeof(float) * FIR_order);
        memset((void *)lms_w, 0, sizeof(float) * FIR_order);
        w_forward_ptr = lms_w_forward;
        w_ptr = lms_w;
    }

    x_ptr = input->x_ptr;

    arm_dot_prod_f32( w_ptr,x_ptr, FIR_order,&output->y);
    output->error = input->d - output->y;

    arm_dot_prod_f32(x_ptr, x_ptr, FIR_order, &x_power);
    Temp = (2.0f * LMS_NIU * output->error) / (x_power + LMS_EPSILON);
    for(uint16_t i = 0; i < FIR_order; i++)
    {
        w_forward_ptr[i]= w_ptr[i] + Temp*x_ptr[i];
    }
    Temp_w_ptr = w_forward_ptr;
    w_forward_ptr = w_ptr;
    w_ptr = Temp_w_ptr;
}


