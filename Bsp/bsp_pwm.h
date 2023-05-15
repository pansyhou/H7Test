//
// Created by 16771 on 2023/5/14.
//

#ifndef H7TEST_BSP_PWM_H
#define H7TEST_BSP_PWM_H

#include "tim.h"
extern void ECF_PWM_50HZ_Output(TIM_HandleTypeDef *htim, int8_t TIM_Channel, const float Output_Percent);
extern void ECF_HobbyWing_ESC_Init(TIM_HandleTypeDef *htim, int8_t TIM_Channel);
extern void ECF_HobbyWing_ESC_Control(TIM_HandleTypeDef *htim, int8_t TIM_Channel,uint16_t pwm);
extern void ECF_PWM_50HZ_Output_Init(TIM_HandleTypeDef *htim, int8_t TIM_Channel , int32_t cmp);

#endif //H7TEST_BSP_PWM_H
