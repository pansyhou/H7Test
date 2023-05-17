//
// Created by 16771 on 2023/5/14.
//
#include "bsp_pwm.h"

//servo control freq about 50-330Hz
//注意，舵机频率越高可能力越小，折中选择
// 算他200hz 就是 5ms=5000us
// 500-2500us cmp应该也是从500-2500跳动
//脉冲宽度范围 500-2500us
//



/**
 *  50HZ输出频率百分比调整函数
 * @param htim 定时器句柄
 * @param TIM_Channel TIM输出通道
 * @param Output_Percent 输出百分比 0-100
 */

void ECF_PWM_50HZ_Output_Init(TIM_HandleTypeDef *htim, int TIM_Channel , int32_t cmp){
    //合法性检查
    assert_param(IS_TIM_INSTANCE(htim->Instance));
    assert_param(IS_TIM_CCX_INSTANCE(htim, TIM_Channel));
    HAL_TIM_Base_Start(htim);
    HAL_TIM_PWM_Start(htim,TIM_Channel);
    __HAL_TIM_SetCompare(htim, TIM_Channel, cmp);
}


void ECF_PWM_50HZ_Output(TIM_HandleTypeDef *htim, int TIM_Channel, const float Output_Percent) {
    //合法性检查
    assert_param(IS_TIM_INSTANCE(htim->Instance));
    assert_param(IS_TIM_CCX_INSTANCE(htim, TIM_Channel));
    if (Output_Percent < 0 || Output_Percent > 100) {
        return;
    }
    __HAL_TIM_SetCompare(htim, TIM_Channel, Output_Percent * 200);
}



/**
 * 好盈skywalker电调启动程序
 * 原理：油门拉最高然后拉最低
 * @param htim 定时器句柄
 * @param TIM_Channel 定时器channel
 */
//void ECF_HobbyWing_ESC_Init(TIM_HandleTypeDef *htim, int8_t TIM_Channel){
//    //合法性检查
//    assert_param(IS_TIM_INSTANCE(htim->Instance));
//    assert_param(IS_TIM_CCX_INSTANCE(htim, TIM_Channel));
//    HAL_TIM_Base_Start(htim);
//    HAL_TIM_PWM_Start(htim,TIM_Channel);
//    vTaskDelay(4000);
//    __HAL_TIM_SetCompare(htim, TIM_Channel,2000);
//    vTaskDelay(4000);
//    __HAL_TIM_SetCompare(htim, TIM_Channel,1000);
//    vTaskDelay(4000);
//}



/**
 * 好盈电调控制函数
 * 注意pwm控制范围在 1200到1400（我怕爆了）
 * @param htim
 * @param TIM_Channel
 * @param pwm
 */
void ECF_HobbyWing_ESC_Control(TIM_HandleTypeDef *htim, int TIM_Channel,uint16_t pwm) {
    //别超油门了
    if (pwm >= 1400) pwm = 1300;
    else if(pwm <= 1190)  pwm=1140;
    __HAL_TIM_SetCompare(htim, TIM_Channel,pwm);
}
