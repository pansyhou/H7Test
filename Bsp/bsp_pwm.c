//
// Created by 16771 on 2023/5/14.
//
#include "bsp_pwm.h"

//servo control freq about 50-330Hz
//ע�⣬���Ƶ��Խ�߿�����ԽС������ѡ��
// ����200hz ���� 5ms=5000us
// 500-2500us cmpӦ��Ҳ�Ǵ�500-2500����
//�����ȷ�Χ 500-2500us
//



/**
 *  50HZ���Ƶ�ʰٷֱȵ�������
 * @param htim ��ʱ�����
 * @param TIM_Channel TIM���ͨ��
 * @param Output_Percent ����ٷֱ� 0-100
 */

void ECF_PWM_50HZ_Output_Init(TIM_HandleTypeDef *htim, int TIM_Channel , int32_t cmp){
    //�Ϸ��Լ��
    assert_param(IS_TIM_INSTANCE(htim->Instance));
    assert_param(IS_TIM_CCX_INSTANCE(htim, TIM_Channel));
    HAL_TIM_Base_Start(htim);
    HAL_TIM_PWM_Start(htim,TIM_Channel);
    __HAL_TIM_SetCompare(htim, TIM_Channel, cmp);
}


void ECF_PWM_50HZ_Output(TIM_HandleTypeDef *htim, int TIM_Channel, const float Output_Percent) {
    //�Ϸ��Լ��
    assert_param(IS_TIM_INSTANCE(htim->Instance));
    assert_param(IS_TIM_CCX_INSTANCE(htim, TIM_Channel));
    if (Output_Percent < 0 || Output_Percent > 100) {
        return;
    }
    __HAL_TIM_SetCompare(htim, TIM_Channel, Output_Percent * 200);
}



/**
 * ��ӯskywalker�����������
 * ԭ�����������Ȼ�������
 * @param htim ��ʱ�����
 * @param TIM_Channel ��ʱ��channel
 */
//void ECF_HobbyWing_ESC_Init(TIM_HandleTypeDef *htim, int8_t TIM_Channel){
//    //�Ϸ��Լ��
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
 * ��ӯ������ƺ���
 * ע��pwm���Ʒ�Χ�� 1200��1400�����±��ˣ�
 * @param htim
 * @param TIM_Channel
 * @param pwm
 */
void ECF_HobbyWing_ESC_Control(TIM_HandleTypeDef *htim, int TIM_Channel,uint16_t pwm) {
    //��������
    if (pwm >= 1400) pwm = 1300;
    else if(pwm <= 1190)  pwm=1140;
    __HAL_TIM_SetCompare(htim, TIM_Channel,pwm);
}
