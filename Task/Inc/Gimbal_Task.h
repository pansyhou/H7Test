#ifndef __GIMBAL_TASK_H
#define __GIMBAL_TASK_H
#include "RemoteDeal.h"
#include "GimbalMotor.h"
#include "fsm.h"
#include "SYSInit.h"

/**GImbal main structure **/

typedef  struct
{
#ifdef configUSE_H7
    FDCAN_HandleTypeDef *fdcan1Handle;
    FDCAN_HandleTypeDef *fdcan2Handle;
#endif
    REMOTE_t *RC;   //遥控器主结构体
    FSM_t *Gimbal_Fsm;
    Grasp_t *Graps;
    //    void (*Lift_Up_fun)(Lift_UP_t*,int16_t);
    void (*PowerOff)(Grasp_t*);//云台断电
}Gimbal_t;

static void Gimbal_Init(void);

void Gimbal_Task(void *pvParameters);
#endif // !__GIMBAL_TASK_H

